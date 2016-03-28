/*
Copyright 2015 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Parser.h"

const char*& Parser::parseHeader(const char*& pos, const char*& end, std::unordered_map<std::string, std::vector<std::string>>& header)
{
	std::vector<std::string> tokens;
	std::string eventType;
	std::string line = "";

	while (eventType.find("EndHeader") == std::string::npos)
	{
		const char* newPos = static_cast<const char*>(memchr(pos, '\n', end - pos));
		line.assign(pos, newPos - pos);

		tokens.clear();
		Utils::tokenize(line, tokens, ",");

		tokens = Utils::removeSpaces(tokens);

		eventType = tokens[0];

		tokens.erase(tokens.begin());

		header[eventType] = tokens;

		pos = ++newPos;
	}
	return pos;
}

void Parser::parseLines(const char*& pos, const char*& end, std::vector<std::string>& chromeEventLines)
{
	std::string tempLine = "";
	unsigned int AsyncId = 0;
	//types de I/O
	std::unordered_set<std::string> typesDisk{ "DiskRead", "DiskWrite", "DiskFlush" };
	std::unordered_set<std::string> typesIO{ "FileIoCreate", "FileIoCleanup", "FileIoClose",
			"FileIoFlush", "FileIoRead", "FileIoWrite",
			"FileIoSetInfo", "FileIoQueryInfo", "FileIoFSCTL",
			"FileIoDelete", "FileIoRename", "FileIoDirEnum",
			"FileIoDirNotify", "FileIoOpEnd" };

	std::unordered_map<std::string, std::vector<std::vector<std::string>>> tidCompletePhaseStacks;
	std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> tidFileIoStacks;
	std::unordered_map<std::string, unsigned int> CSwitchStacks;

	while (pos && pos != end)
	{
		const char* newPos = static_cast<const char*>(memchr(pos, '\n', end - pos));
		tempLine.assign(pos, newPos - pos);

		std::vector<std::string> tokens;
		Utils::tokenize(tempLine, tokens, ",");
		tokens = Utils::removeSpaces(tokens);

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

					int completeTS = std::stoi(complete[1]);
					int completeEndTS = std::stoi(tokens[1]);
					std::string duration = std::to_string(completeEndTS - completeTS);
					complete.push_back(duration);

					std::string eventJSON = converter->EventToJSON(complete);
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
				std::string eventJSON = converter->EventToJSON(tokens);
				chromeEventLines.push_back(eventJSON);
			}
		}
		//Handling of FileIOEvent
		else if ((typesIO.find(tokens[0]) != typesIO.end()) && (tokens[2].find("chrome.exe") != std::string::npos))
		{
			if (*(typesIO.find(tokens[0])) == "FileIoOpEnd")
			{   // We look for the same Tid
				auto FileIoStackIter = tidFileIoStacks.find(tokens[3]);
				if (FileIoStackIter != tidFileIoStacks.end())
				{   //We look for the same FileObject
					auto FileIoEvent = FileIoStackIter->second.find(tokens[8]);
					//IrpPtr compare          //FileObject compare              //Type compare         
					if (FileIoEvent != FileIoStackIter->second.end() && FileIoEvent->second[7] == tokens[7] && FileIoEvent->second[8] == tokens[8] && FileIoEvent->second[0] == tokens[12])
					{
						chromeEventLines.push_back(converter->IOLineToJSON(FileIoEvent->second, tokens));
						FileIoStackIter->second.erase(tokens[8]);
					}
				}
			}
			else
			{
				tidFileIoStacks[tokens[3]][tokens[8]] = tokens;
			}
		}
		// Handling of DiskEvent
		else if ((typesDisk.find(tokens[0]) != typesDisk.end()) && (tokens[2].find("chrome.exe") != std::string::npos))
		{
			chromeEventLines.push_back(converter->DiskLineToJSON(tokens));
		}
		else if (tokens[0] == "CSwitch")
		{   // New Process
			if (tokens[2].find("chrome.exe") != std::string::npos)
			{
				CSwitchStacks[tokens[3]] = AsyncId++;
				chromeEventLines.push_back(converter->CSwitchToJson(tokens, "New Process", AsyncId - 1));
			}
			// Old Process
			if (tokens[8].find("chrome.exe") != std::string::npos)
			{
				chromeEventLines.push_back(converter->CSwitchToJson(tokens, "Old Process", CSwitchStacks[tokens[9]]));
			}
		}
		pos = ++newPos;
	}
}

void Parser::parseStacks(SystemHistory& system_history, std::unordered_map<base::Tid, std::vector<std::string>>& completedFunctions)
{
	// Traverse all threads.
	for (auto threads_it = system_history.threads_begin(); threads_it != system_history.threads_end(); ++threads_it)
	{
		std::string process_name = system_history.GetProcessName(threads_it->second.parent_process_id());
		if (process_name != "chrome.exe") continue;

		auto& threadStacks = threads_it->second.Stacks();
		auto  stacksEnd = threadStacks.IteratorEnd();

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