#include "../etw_reader/etw_reader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <functional> 
#include <cctype>
#include <locale>
#include <boost/iostreams/device/mapped_file.hpp>

std::clock_t start;

// Functions declarations
std::string convertEventToJSON(std::vector<std::string>& lines);
std::vector<std::string> removeSpaces(std::vector<std::string> tokens);
const char*& parseHeader(const char*& pos, const char*& end, std::unordered_map<std::string, std::vector<std::string>>& header);
void parseLines(const char*& pos, const char*& end, std::vector<std::string>& lines);
void writeJSON(char* path, std::vector<std::string>& lines);
std::string convertEventToJSON(std::vector<std::string>& line);
boost::iostreams::mapped_file mapFileToMem(char* path);
void convertEtlToCSV(char* argvPath);
void convertCSVToJSON(char* path);
void showElapseTime(std::string text);
int main(int argc, char** argv);
static inline std::string &ltrim(std::string &s);
static inline std::string &rtrim(std::string &s);
static inline std::string &trim(std::string &s);


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
				updatedTokens.push_back(trim(token));
		}
		return updatedTokens;
}

// trim from start
static inline std::string &ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
		return ltrim(rtrim(s));
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

void parseLines(const char*& pos, const char*& end, std::vector<std::string>& lines)
{
		std::string tempLine = "";
		std::vector<std::string> line = std::vector<std::string>();

		std::unordered_map<std::string, std::vector<std::vector<std::string>>> tidCompleteStacks;

		while(pos && pos != end)
		{
				const char* newPos = static_cast<const char*>(memchr(pos, '\n', end - pos));
				tempLine.assign(pos, newPos - pos);

				std::vector<std::string> tokens;
				tokenize(tempLine, tokens, ",");
				tokens = removeSpaces(tokens);

				if (tokens[0] == "Chrome//win:Info")
				{
						 if (tokens[10] == "\"Complete\"")
						 {
								tidCompleteStacks[tokens[3]].push_back(tokens);
						 }
						 else if (tokens[10] == "\"Complete End\"")
						 {
								auto completeStackIter = tidCompleteStacks.find(tokens[3]);
								if (completeStackIter != tidCompleteStacks.end() && tidCompleteStacks[tokens[3]].size() > 0)
								{
										std::vector<std::vector<std::string>> completeStack = (*completeStackIter).second;
										std::vector<std::string> complete = completeStack.back();
								
										int completeTS       = std::stoi(complete[1]);
										int completeEndTS    = std::stoi(tokens[1]);
										std::string duration = std::to_string(completeEndTS - completeTS);
										complete.push_back(duration);
								
										std::string eventJSON = convertEventToJSON(complete);
										lines.push_back(eventJSON);
								
										tidCompleteStacks[tokens[3]].pop_back();
								}
								else
								{
										std::cout << "Something wrong happened: Complete End without Complete" << std::endl;
								}
						 }
						 else
						 {
						   std::string eventJSON = convertEventToJSON(tokens);
							 lines.push_back(eventJSON);
						 }
				}

				pos = ++newPos;
		}
}

void writeJSON(char* path, std::vector<std::string>& lines)
{
		std::vector<std::string> fileName;
		tokenize(path, fileName, ".");
		std::ofstream outputFile(fileName[0] + ".json");

		outputFile << "{\"traceEvents\":[";

		for (auto line : lines)
		{
				outputFile << line << ",\n";
		}

		outputFile << "]}";

		outputFile.close();
}

std::string extractPidFromString(std::string &word)
{
	unsigned first = word.find("(");
	unsigned last = word.find(")");
	return word.substr(first + 1, last - first -1);
}

std::string getPhase(std::string &word)
{
	//Event type
	// Duration: B(begin) E(end)
	// Complete: X
	// Instant: i, I (dreprecated)
	// Counter: C
	// Async: b(nestable start), n (nestable instant), e(nestable end)
	//		  Deprecated
	//        S(start), T(step into), p(step past), F(end)
	// Flow: s(start), t(step), f(end)
	// Sample: P
	// Object: N(created), O(snapshot), D(destroyed)
	// Metadata: M
	// Memory Dump: V(global), V(process)
	// Mark: R
	// Clock Sync: c
	// Context: (,)
	if (word == "\"Complete\"")
		return "X";
	else if (word == "\"Begin\"")
		return "B";
	else if (word == "\"End\"")
		return "E";
	else if (word == "\"Instant\"")
		return "i";
	else if (word == "\"Counter\"")
		return "C";
	else if (word == "\"Async End\"")
		//return "e"; //new version
		return "F";  //deprecated version
	else if (word == "\"Async Begin\"")
		//return "b"; //new version
		return "S";	  //deprecated version
	else if (word == "\"Async Step Into\"")
		return "T";
	else if (word == "\"Sample\"")
		return "P";
	else if (word == "\"Metadata\"")
		return "M";
	else if (word == "\"Mark\"")
		return "R";
	else if (word == "")
		return "";
	else
		return "error";
}

std::string convertEventToJSON(std::vector<std::string>& line)
{
	//dirty code
	std::string name = "";
	std::string cat = "";
	std::string phase = "";
	std::string pid = "";
	std::string tid = "";
	std::string ts = "";
	std::string args = "\"args\":{";
	std::string dur = "";

	//dirty code

	std::string event = "";
	std::string outputText = "";
	std::string eventType = "";
	std::vector<std::string> eventInfo = std::vector < std::string >();
	for (int i = 0; i < line.size(); i++)
	{
		switch (i)
		{
		case 1:
			ts += "\"ts\":" + line[i] + ",";
			break;
		case 2:
			pid += "\"pid\":" + extractPidFromString(line[i]) + ",";
			break;
		case 3:
			tid += "\"tid\":" + line[i] + ",";
			break;
		case 9:
			line[i].erase(std::remove(line[i].begin(), line[i].end(), '"'), line[i].end());
			if (line[i][0] == ' ')
			  line[i].erase(std::remove(line[i].begin(), line[i].begin() + 1, ' '), line[i].begin() + 1);
			name += "\"name\":\"" + line[i] + "\",";
			break;
		case 10:
			phase += "\"ph\":\"" + getPhase(line[i]) + "\",";
			break;
		case 11:
		case 13:
		case 15:
			line[i].erase(std::remove(line[i].begin(), line[i].end(), '"'), line[i].end());
			if (line[i][0] == ' ')
					line[i].erase(std::remove(line[i].begin(), line[i].begin() + 1, ' '), line[i].begin() + 1);
		  if (line[i] != "")
		  		args += "\"" + line[i] + "\":";
			break;
		case 12:
		case 14:
		case 16:
			line[i].erase(std::remove(line[i].begin(), line[i].end(), '"'), line[i].end());
			if (line[i][0] == ' ')
				line[i].erase(std::remove(line[i].begin(), line[i].begin() + 1, ' '), line[i].begin() + 1);
			if (line[i] != "")
				args += "\"" + line[i] + "\",";
			break;
		case 17:
			dur += ",\"dur\": " + line[i] + ",";
		}
	}
	args += "}";
	return "{" + pid + tid + ts + phase + cat + name + args + dur + "}";
}

boost::iostreams::mapped_file mapFileToMem(char* path)
{
		boost::iostreams::mapped_file mmap(path, boost::iostreams::mapped_file::readonly);
		return mmap;
}

void convertEtlToCSV(char* argvPath)
{
		etw_insights::ETWReader etwReader;

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

		showElapseTime("\n\n\nTemps de fin de parsing du header: ");

		std::vector<std::string> lines;
		parseLines(posBeginLines, end, lines);

		showElapseTime("\n\n\nTemps de fin de parsing des lignes du fichier: ");

		writeJSON(path, lines);

		showElapseTime("\n\n\nTemps de fin d'ecriture du JSON: ");
		
		//munmap pas necessaire parce que map desallouer a la fin du process
		//boost::iostreams::mapped_file munmap(mmap, mmap.size());
}

void showElapseTime(std::string text)
{
		std::cout << text << (std::clock() - start) / (double)CLOCKS_PER_SEC << '\n';
}



int main(int argc, char** argv)
{
	double duration = 0;
	start = std::clock();

	// Look if there is no arguments
	if (argc <= 1)
		return 0;
	else if (std::string(argv[1]).find(".etl") != std::string::npos)
	{
			//do stuff if is a .etl
			convertEtlToCSV(argv[1]);
			convertCSVToJSON(argv[1]);
			
	}
	else if (std::string(argv[1]).find(".csv") != std::string::npos)
	{
			//do stuff if is a .csv
			convertCSVToJSON(argv[1]);
	}
	else
			return 0;
			
	showElapseTime("\n\n\nDuree totale de l'application:  ");

	system("PAUSE");

	return 1;
}
