#include "../etw_reader/etw_reader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <boost/iostreams/device/mapped_file.hpp>

std::clock_t start;


//http://stackoverflow.com/questions/236129/split-a-string-in-c
template < class ContainerT >
void tokenize(const std::string& str, ContainerT& tokens,
		const std::string& delimiters = " ", bool trimEmpty = false)
{
		std::string::size_type pos, lastPos = 0;

		using value_type = typename ContainerT::value_type;
		using size_type = typename ContainerT::size_type;

		while (true)
		{
				pos = str.find_first_of(delimiters, lastPos);
				if (pos == std::string::npos)
				{
						pos = str.length();

						if (pos != lastPos || !trimEmpty)
								tokens.push_back(value_type(str.data() + lastPos,
								(size_type)pos - lastPos));

						break;
				}
				else
				{
						if (pos != lastPos || !trimEmpty)
								tokens.push_back(value_type(str.data() + lastPos,
								(size_type)pos - lastPos));
				}

				lastPos = pos + 1;
		}
}

std::vector<std::string> removeSpaces(std::vector<std::string> tokens)
{
		std::vector<std::string> updatedTokens;

		for each(std::string token in tokens)
		{
				token.erase(remove(token.begin(), token.end(), ' '), token.end());
				updatedTokens.push_back(token);
		}
		return updatedTokens;
}

const char*& parseHeader(const char*& pos, const char*& end, std::unordered_map<std::string, std::vector<std::string>>& header)
{
		std::vector<std::string> tokens;
		std::string eventType;
		std::string line = "";
		

		while (eventType.find("EndHeader") == std::string::npos)
		{
				const char* newPos = static_cast<const char*>(memchr(pos, '\n', end - pos));
				line.assign(pos, newPos - pos);

				tokens.clear();
				tokenize(line, tokens, ",");

				tokens = removeSpaces(tokens);

				eventType = tokens[0];

				tokens.erase(tokens.begin());

				header[eventType] = tokens;

				pos = ++newPos;
		}
		return pos;
}

void parseLines(const char*& pos, const char*& end, std::vector<std::vector<std::string>>& lines)
{
		std::string tempLine = "";
		std::vector<std::string> line = std::vector<std::string>();

		while(pos && pos != end)
		{
				const char* newPos = static_cast<const char*>(memchr(pos, '\n', end - pos));
				tempLine.assign(pos, newPos - pos);

				std::vector<std::string> tokens;
				tokenize(tempLine, tokens, ",");

				tokens = removeSpaces(tokens);

				if (tokens[0] == "Chrome//win:Info")
				{
						lines.push_back(tokens);
				}

				pos = ++newPos;
		}
}

void writeJSON(char* path, std::unordered_map<std::string, std::vector<std::string>>& header, std::vector<std::vector<std::string>>& lines)
{
		std::vector<std::string> fileName;
		tokenize(path, fileName, ".");
		std::ofstream outputFile(fileName[0] + ".json");
		
		std::string event = "";
		std::string outputText = "";
		std::string eventType = "";
		std::vector<std::string> eventInfo = std::vector < std::string > ();

		for (auto line : lines)
		{
				outputText = "";

				eventType = line[0];

				eventInfo = header[eventType];

				outputText += "{";

				for (int i = 0; i < eventInfo.size(); i++)
				{
						if (eventInfo[i] != "" && line[i + 1] != "")
						{
								outputText += "\"" + eventInfo[i] + "\":" + line[i + 1];

								if (i != eventInfo.size() - 1)
										outputText += ",";
						}
				}
				outputFile << outputText + "}\r\n";
		}

		outputFile.close();
}

std::string convertEventToJSON(std::unordered_map<std::string, std::vector<std::string>>& header, std::vector<std::string>& lines)
{
	//Event type
	// Duration: B(begin) E(end)
	// Complete: X
	// Instant: i, I (dreprecated)
	// Counter: C
	// Async: b(nestable start), n (nestable instant), e(nestable end)
	// Flow: s(start), t(step), f(end)
	// Sample: P
	// Object: N(created), O(snapshot), D(destroyed)
	// Metadata: M
	// Memory Dump: V(global), V(process)
	// Mark: R
	// Clock Sync: c
	// Context: (,)

	//dirty code
	std::string name = "";
	std::string ph = "";
	std::string cat = "";
	std::string phase = "";
	std::string pid = "";
	std::string tid = "";
	std::string ts = "";
	std::string args = "args:{";

	//dirty code

	std::string event = "";
	std::string outputText = "";
	std::string eventType = "";
	std::vector<std::string> eventInfo = std::vector < std::string >();
	for (int i = 0; i < lines.size(); i++)
	{
		switch (i)
		{
		case 0:
			ts += "\"ts\": " + lines[i] + ",";
			break;
		case 1:
			pid += "\"pid\": " + lines[i] + ",";
			break;
		case 2:
			tid += "\"tid\": " + lines[i] + ",";
			break;
		case 4:
			name += "\"name\": \"" + lines[i] + "\",";
			break;
		case 5:
			phase += "\"ph\": \"" + lines[i] + "\",";
			break;
		// over repeating dirty hardcoded
		case 6:
			args += "\"" + lines[i] + ":";
			break;
		case 7:
			args += "\"" + lines[i] + "\",";
			break;
		case 8:
			args += "\"" + lines[i] + ":";
			break;
		case 9:
			args += "\"" + lines[i] + "\",";
			break;
		case 10:
			args += "\"" + lines[i] + ":";
			break;
		case 11:
			args += "\"" + lines[i] + "\",";
			break;
		}
	}
	args += "}";
	return "{" + name + cat + ph + pid + tid + ts + "}";
}

boost::iostreams::mapped_file mapFileToMem(char* path)
{
		boost::iostreams::mapped_file mmap(path, boost::iostreams::mapped_file::readonly);
		return mmap;
}

void convertEtlToCSV(char* argvPath, etw_insights::ETWReader& etwReader)
{
		std::string pathTemp = std::string(argvPath);
		const std::wstring path(pathTemp.begin(), pathTemp.end());
		etwReader.Open(path);
}

void convertCSVToJSON(char* path)
{
		// Opening and reading .csv
		boost::iostreams::mapped_file mmap = mapFileToMem(path);
		auto pos = mmap.const_data();
		auto end = pos + mmap.size();

		std::unordered_map<std::string, std::vector<std::string>> header;
		const char*& posBeginLines = parseHeader(pos, end, header);
		double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		std::cout << "\n\n\nTemps de fin de parsing du header: " << duration << '\n';

		std::vector<std::vector<std::string>> lines;
		parseLines(posBeginLines, end, lines);

		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		std::cout << "\n\n\nTemps de fin de parsing des lignes du fichier: " << duration << '\n';

		writeJSON(path, header, lines);
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		std::cout << "\n\n\nTemps de fin d'ecriture du JSON: " << duration << '\n';
		
		//munmap pas necessaire parce que map desallouer a la fin du process
		//boost::iostreams::mapped_file munmap(mmap, mmap.size());
}





int main(int argc, char** argv)
{
	//std::clock_t start;
	double duration = 0;

	etw_insights::ETWReader etwReader;

	// Look if there is no arguments
	if (argc <= 1)
		return 0;
	/// TODO si un argv est un .etl est caller, appeler sa methode dans etw_reader pour convertir en .csv
	else
	{
			start = std::clock();
			convertCSVToJSON(argv[1]);
	}
	/*
	else if (std::string(argv[1]).find(".etl") != std::string::npos)
	{
			//do stuff if is a .etl
			convertEtlToCSV(argv[1], etwReader);
			convertCSVToJSON(argv[1]);
			
	}
	else if (std::string(argv[1]).find(".csv") != std::string::npos)
	{
			//do stuff if is a .csv
			convertCSVToJSON(argv[1]);
	}
	else
			return 0;
			*/

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "\n\n\nDuree totale de l'application:  " << duration << '\n';

	system("PAUSE");

	return 1;
}





/*
boost::iostreams::mapped_file mmap = mapFileToMem(path);
auto f = mmap.const_data();
auto l = f + mmap.size();

uintmax_t m_numLines = 0;
while (f && f != l)
if ((f = static_cast<const char*>(memchr(f, '\n', l - f))))
m_numLines++, f++;

std::cout << "m_numLines = " << m_numLines << "\n";
*/