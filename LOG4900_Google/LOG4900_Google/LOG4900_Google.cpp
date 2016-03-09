#include "CSVBlock.h"
#include "StackBlock.h"
#include "../etw_reader/etw_reader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <functional> 
#include <cctype>
#include <locale>
#include <boost/iostreams/device/mapped_file.hpp>
#include "stateIO\StateManager.h"

std::clock_t start;
StateManager stateIO;

// Functions declarations
std::string convertEventToJSON(std::vector<std::string>& lines);
std::vector<std::string> removeSpaces(std::vector<std::string> tokens);
const char*& parseHeader(const char*& pos, const char*& end, std::unordered_map<std::string, std::vector<std::string>>& header);
void parseLines(const char*& pos, const char*& end, std::vector<std::string>& chromeEventLines, std::unordered_map<std::string, std::vector<std::string>>& stackEventLines);
void writeJSON(char* path, std::vector<std::string>& chromeEventLines, std::unordered_map<std::string, std::vector<std::string>> stackEventLines);
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

void parseLines(const char*& pos, const char*& end, std::vector<std::string>& chromeEventLines, std::unordered_map<std::string, std::vector<std::string>>& stackEventLines)
{
		std::string tempLine = "";
		//types de I/O
		std::unordered_set<std::string> typesIO{ "FileIoCreate", "FileIoCleanup", "FileIoClose", 
												 "FileIoFlush", "FileIoRead","FileIoWrite",
												 "FileIoSetInfo", "FileIoQueryInfo", "FileIoFSCTL",
												 "FileIoDelete", "FileIoRename", "FileIoDirEnum",
												 "FileIoDirNotify", "FileIoOpEnd"};
		
		std::unordered_map<std::string, std::vector<std::vector<std::string>>> tidCompletePhaseStacks;

		CSVBlock currentStack;
		std::unordered_map<std::string, StackBlock> tidStacks;
		std::unordered_map<std::string, std::string> timestampProcess;

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
								tidCompletePhaseStacks[tokens[3]].push_back(tokens);
						 }
						 else if (tokens[10] == "\"Complete End\"")
						 {
								auto completeStackIter = tidCompletePhaseStacks.find(tokens[3]);
								if (completeStackIter != tidCompletePhaseStacks.end() && tidCompletePhaseStacks[tokens[3]].size() > 0)
								{
										std::vector<std::vector<std::string>> completeStack = (*completeStackIter).second;
										std::vector<std::string> complete = completeStack.back();
								
										int completeTS       = std::stoi(complete[1]);
										int completeEndTS    = std::stoi(tokens[1]);
										std::string duration = std::to_string(completeEndTS - completeTS);
										complete.push_back(duration);
								
										std::string eventJSON = convertEventToJSON(complete);
										chromeEventLines.push_back(eventJSON);
								
										tidCompletePhaseStacks[tokens[3]].pop_back();
								}
								else
								{
										std::cout << "Something wrong happened: Complete End without Complete" << std::endl;
								}
						 }
						 else
						 {
							std::string eventJSON = convertEventToJSON(tokens);
							chromeEventLines.push_back(eventJSON);
						 }
				}
				else if (tokens[0] == "Stack")
				{
					int ts = std::stoi(tokens[1]);
					if (currentStack.Empty())
						currentStack.Reset(tokens[2], ts);

					if (timestampProcess[tokens[1]].find("chrome.exe") != std::string::npos)
					{
						if (tokens[2] != currentStack.GetTID() || ts != currentStack.GetTimestamp()) // Reached a new stack
						{
							std::vector<std::string> endedLines = tidStacks[tokens[2]].Update(currentStack);
							stackEventLines[tokens[2]].insert(std::end(stackEventLines[tokens[2]]), std::begin(endedLines), std::end(endedLines));

							currentStack.Reset(tokens[2], ts);
							currentStack.AddLine(tokens);
						}
						else
						{
							currentStack.AddLine(tokens);
						}
					}
				}

				//si le premier tokens est un FileIO et que le troisième est "chrome.exe"
				else if ((typesIO.find(tokens[0]) != typesIO.end()) && (tokens[2].find("chrome.exe") != std::string::npos))
				{
					//logique pour les I/O
				}
				else if (tokens[0] == "SampledProfile" || tokens[0] == "ReadyThread" || tokens[0] == "CSwitch")
				{
					timestampProcess[tokens[1]] = tokens[2];
				}

				pos = ++newPos;
		}

		if (tidStacks.size() >= 0)
		{
			if (!currentStack.Empty())
				tidStacks[currentStack.GetTID()].Update(currentStack);

			for (auto& stack : tidStacks)
			{
				std::vector<StackLine> finalLines = stack.second.GetLines();
				for (StackLine& line : finalLines)
				{
					line.SetEndTimestamp(currentStack.GetTimestamp());
					stackEventLines[stack.first].push_back(line.ToJson());
				}
			}
		}
}

void writeJSON(char* path, std::vector<std::string>& chromeEventLines, std::unordered_map<std::string, std::vector<std::string>> stackEventLines)
{
		std::vector<std::string> fileName;
		tokenize(path, fileName, ".");
		std::ofstream outputFile(fileName[0] + ".json");

		outputFile << "{\"traceEvents\":[";
		for (int i = 0; i < chromeEventLines.size(); ++i)
		{
			if (i != chromeEventLines.size() - 1)
				outputFile << chromeEventLines[i] << ",\n";
		}
		outputFile << "],\n\"stacks\":[";
		for (auto& it = stackEventLines.begin(); it != stackEventLines.end(); ++it)
		{
			outputFile << "\"" + (*it).first + "\":[";
			for (int i = 0; i < (*it).second.size(); ++i)
			{
				if (i != (*it).second.size() - 1)
					outputFile << (*it).second[i] << ",\n";
			}
			
			if (it != --stackEventLines.end())
				outputFile << "],\n";
			else
				outputFile << "]";
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
			if (line[i] != "" && line[i + 1] != "\"\"")
		  		args += "\"" + line[i] + "\":";
			if (line[i] != "" && line[i + 1] == "\"\"")
			{
				args.erase(args.end() - 2, args.end());
				args += "," + line[i] + "\"";
			}
			break;
		case 12:
		case 14:
		case 16:
			line[i].erase(std::remove(line[i].begin(), line[i].end(), '"'), line[i].end());
			if (line[i][0] == ' ')
				line[i].erase(std::remove(line[i].begin(), line[i].begin() + 1, ' '), line[i].begin() + 1);
			if (line[i] != "" && line[i + 1] != "\"\"")
				args += "\"" + line[i] + "\",";
			if (line[i] != "" && line[i + 1] == "\"\"")
				args += "\"" + line[i] + "\"";
			break;
		case 17:
			dur += ",\"dur\": " + line[i];
		}
	}
	args += "}";
	return "{" + pid + tid + ts + phase + cat + name + args + dur + "}";
}


std::string convertEventIOToJSON(std::vector<std::string>& line)
{
	stateIO.changeStateTo(stateIO.fromStringToIntIO(line[0]));
	return stateIO.getCurrentState()->returnJson(line);
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

		std::vector<std::string> chromeEventLines;
		std::unordered_map<std::string, std::vector<std::string>> stackEventLines;
		parseLines(posBeginLines, end, chromeEventLines, stackEventLines);

		showElapseTime("\n\n\nTemps de fin de parsing des lignes du fichier: ");

		writeJSON(path, chromeEventLines, stackEventLines);

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
