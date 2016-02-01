#include "etw_reader/etw_reader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <ctime>

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

void parseHeader(std::ifstream& file, std::unordered_map<std::string, std::vector<std::string>>& header)
{
		int lineNb = 0;
		std::string event;

		while (file.good())
		{
				getline(file, event);

				std::vector<std::string> tokens;
				tokenize(event, tokens, ",");

				tokens = removeSpaces(tokens);

				std::string eventType = tokens[0];

				tokens.erase(tokens.begin());

				header[eventType] = tokens;

				lineNb++;

				if (eventType == "EndHeader")
						return;
		}
}

void parseLines(std::ifstream& file, std::vector<std::vector<std::string>>& lines)
{
		std::string tempLine;

		while (file.good())
		{
				std::vector<std::string> line;

				getline(file, tempLine);

				std::vector<std::string> tokens;
				tokenize(tempLine, tokens, ",");

				tokens = removeSpaces(tokens);

				if (tokens[0] == "chrome//win:Info")
				{
						for (auto token : tokens)
						{
								line.push_back(token);
						}

						lines.push_back(line);
				}
		}
}

void writeJSON(std::ifstream& file, char* argvPath, std::unordered_map<std::string, std::vector<std::string>>& header, std::vector<std::vector<std::string>>& lines)
{
		std::vector<std::string> fileName;
		tokenize(argvPath, fileName, ".");
		std::ofstream outputFile(fileName[0] + ".json");
		
		std::string event;

		for (auto line : lines)
		{

				std::string eventType = line[0];

				std::vector<std::string> eventInfo = header[eventType];

				outputFile << "{";

				for (int i = 0; i < eventInfo.size(); i++)
				{
						if (eventInfo[i] != "" && line[i + 1] != "")
						{
								outputFile << "\"" + eventInfo[i] + "\":" + line[i + 1];

								if (i != eventInfo.size() - 1)
										outputFile << ",";
						}
				}

				outputFile << "}\n" ;
		}

		outputFile.close();
}

void convertEtlToCSV(char* argvPath, etw_insights::ETWReader& etwReader)
{
		std::string pathTemp = std::string(argvPath);
		const std::wstring path(pathTemp.begin(), pathTemp.end());
		etwReader.Open(path);
}

void convertCSVToJSON(char* argvPath)
{
		// Opening and reading .csv
		std::ifstream file(argvPath);

		std::unordered_map<std::string, std::vector<std::string>> header;
		std::vector<std::vector<std::string>> lines;

		double duration;
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		std::cout << "parseheaderStart: " << duration << '\n';

		parseHeader(file, header);

		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		std::cout << "parseLinesStart: " << duration << '\n';

		parseLines(file, lines);

		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		std::cout << "writeJSONStart: " << duration << '\n';

		writeJSON(file, argvPath, header, lines);

		file.close();
}

std::fstream& GotoLine(std::fstream& file, unsigned int num){
		file.seekg(std::ios::beg);
		for (int i = 0; i < num - 1; ++i){
				file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		return file;
}



int main(int argc, char* argv[])
{
		//std::clock_t start;
		double duration;

		start = std::clock();

	etw_insights::ETWReader etwReader;

	// Look if there is no arguments
	if (argc <= 1)
		return 0;
	/// TODO si un argv est un .etl est caller, appeler sa methode dans etw_reader pour convertir en .csv
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

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	std::cout << "printf: " << duration << '\n';

	system("PAUSE");

	return 1;
}

