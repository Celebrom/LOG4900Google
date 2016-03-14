#include "LiveStack.h"
#include "../base/command_line.h"
#include "../base/file.h"
#include "../etw_reader/etw_reader.h"
#include "../etw_reader/generate_history_from_trace.h"
#include "../etw_reader/system_history.h"
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
#include <codecvt>
#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

using namespace etw_insights;

std::clock_t start;

// Functions declarations
std::string convertEventToJSON(std::vector<std::string>& lines);
std::vector<std::string> removeSpaces(std::vector<std::string> tokens);
const char*& parseHeader(const char*& pos, const char*& end, std::unordered_map<std::string, std::vector<std::string>>& header);
void parseLines(const char*& pos, const char*& end, std::vector<std::string>& chromeEventLines);
void writeJSON(std::wstring path, std::vector<std::string>& chromeEventLines, std::unordered_map<std::string, std::vector<std::string>> stackEventLines);
std::string convertEventToJSON(std::vector<std::string>& line);
boost::iostreams::mapped_file mapFileToMem(char* path);
void convertETLToJSON(std::wstring path, std::wstring type);
void convertETLToCSV(std::wstring path);
void convertCSVToJSON(std::wstring etl_path, std::wstring csv_path);
void showElapsedTime(std::string text);
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

void parseLines(const char*& pos, const char*& end, std::vector<std::string>& chromeEventLines)
{
		std::string tempLine = "";
		std::unordered_map<std::string, std::vector<std::vector<std::string>>> tidCompletePhaseStacks;

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
				pos = ++newPos;
		}
}

void writeJSON(std::wstring path, std::vector<std::string>& chromeEventLines, std::unordered_map<base::Tid, std::vector<std::string>> stackEventLines)
{
		std::ofstream outputFile(path);

		outputFile << "{\"traceEvents\":[";
		for (int i = 0; i < chromeEventLines.size(); ++i)
		{
			if (i < chromeEventLines.size() - 1)
				outputFile << chromeEventLines[i] << ",\n";
			else
				outputFile << chromeEventLines[i];
		}
		outputFile << "]";
		//outputFile << ",\n\"stacks\":[";
		//for (auto& it = stackEventLines.begin(); it != stackEventLines.end(); ++it)
		//{
		//	outputFile << "\"" << (*it).first << "\":[";
		//	for (int i = 0; i < (*it).second.size(); ++i)
		//	{
		//		if (i != (*it).second.size() - 1)
		//			outputFile << (*it).second[i] << ",\n";
		//	}
		//	
		//	if (it != --stackEventLines.end())
		//		outputFile << "],\n";
		//	else
		//		outputFile << "]";
		//}
		//outputFile << "]";
		outputFile << "}";

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

void parseStacks(SystemHistory& system_history, std::unordered_map<base::Tid, std::vector<std::string>>& completedFunctions)
{
	// Traverse all threads.
	for (auto threads_it = system_history.threads_begin(); threads_it != system_history.threads_end(); ++threads_it)
	{
		std::string process_name = system_history.GetProcessName(threads_it->second.parent_process_id());
		if (process_name != "chrome.exe") continue;

		auto& threadStacks = threads_it->second.Stacks();
		auto  stacksEnd    = threadStacks.IteratorEnd();

		LiveStack liveStack;
		std::vector<std::string> threadCompletedFunctions;
		// Traverse all stacks comparing first and second to see what functions ended
		for (auto it = threadStacks.IteratorFromTimestamp(0); it != stacksEnd; ++it)
		{
			std::vector<std::string> actualCompletedFunctions = liveStack.Update((*it));
			threadCompletedFunctions.insert(std::end(threadCompletedFunctions), std::begin(actualCompletedFunctions), std::end(actualCompletedFunctions));
		}
		std::vector<std::string> finalCompletedFunctions = liveStack.GetFinalLines();
		threadCompletedFunctions.insert(std::end(threadCompletedFunctions), std::begin(finalCompletedFunctions), std::end(finalCompletedFunctions));

		completedFunctions[(*threads_it).first] = threadCompletedFunctions;
	}
}

boost::iostreams::mapped_file mapFileToMem(std::wstring path)
{
	boost::filesystem::path p(path);
	boost::iostreams::mapped_file mmap(p, boost::iostreams::mapped_file::readonly);
	return mmap;
}

void convertETLToJSON(std::wstring etl_path)
{
	std::wstring csv_path = etl_path + L".csv";
	if (!base::FilePathExists(csv_path))
		convertETLToCSV(etl_path);

	convertCSVToJSON(etl_path, csv_path);
}

void convertETLToCSV(std::wstring path)
{
	etw_insights::ETWReader etwReader;
	etwReader.Open(path);
}

// TODO: We should only need one of the path by using the same method for the Chrome Event and Flame graph
void convertCSVToJSON(std::wstring etl_path, std::wstring csv_path)
{
	boost::iostreams::mapped_file mmap = mapFileToMem(csv_path);
	auto pos = mmap.const_data();
	auto end = pos + mmap.size();
	
	std::unordered_map<std::string, std::vector<std::string>> header;
	const char*& posBeginLines = parseHeader(pos, end, header);
	showElapsedTime("Temps de fin de parsing du header");


	std::vector<std::string> chromeEventLines;
	parseLines(posBeginLines, end, chromeEventLines);
	showElapsedTime("Temps de fin de parsing des lignes du fichier");


	//SystemHistory system_history;
	//if (!GenerateHistoryFromTrace(etl_path, &system_history)) {
	//	LOG(ERROR) << "Error while generating history from trace.";
	//	return;
	//}
	std::unordered_map<base::Tid, std::vector<std::string>> completedFunctions;
	//parseStacks(system_history, completedFunctions);


	std::wstring json_path = csv_path + L".json";
	writeJSON(json_path, chromeEventLines, completedFunctions);
	showElapsedTime("Temps de fin d'ecriture du JSON");

		
	//munmap pas necessaire parce que map desallouer a la fin du process
	//boost::iostreams::mapped_file munmap(mmap, mmap.size());
}

void showElapsedTime(std::string text)
{
	std::cout << "\n\n" << text << ": " << (std::clock() - start) / (double)CLOCKS_PER_SEC << "\n";
}

void ShowUsage() {
	std::cout
		<< "Usage: .exe --trace <trace_file_path>"
		<< std::endl
		<< std::endl;
}

int wmain(int argc, wchar_t* argv[], wchar_t* /*envp */[])
{
	start = std::clock();

	base::CommandLine command_line(argc, argv);

	if (command_line.GetNumSwitches() == 0) 
	{
		ShowUsage();
		return 1;
	}

	std::wstring trace_path = command_line.GetSwitchValue(L"trace");
	if (trace_path.empty())
	{
		std::cout << "Please specify a trace path (--trace)." << std::endl << std::endl;
		ShowUsage();
		return 1;
	}

	std::wstring file_type = trace_path.substr(trace_path.length() - 3, 3);
	if (file_type.empty() || file_type != L"etl")
	{
		std::cout << "Please make sure the trace is of ETW type (.etl)" << std::endl << std::endl;
		ShowUsage();
		return 1;
	}

	convertETLToJSON(trace_path);
			
	showElapsedTime("\n\n\nDuree totale de l'application:  ");
	system("PAUSE");

	return 1;
}
