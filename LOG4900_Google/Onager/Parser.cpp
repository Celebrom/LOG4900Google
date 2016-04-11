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
#include "JsonWriter.h"

void Parser::parseStacks(SystemHistory& system_history, std::wstring path, std::ofstream& outputFile)
{
	bool first = true;

	// Traverse all threads.
	for (auto threads_it = system_history.threads_begin(); threads_it != system_history.threads_end(); ++threads_it)
	{
		std::string process_name = system_history.GetProcessName(threads_it->second.parent_process_id());
		if (process_name != "chrome.exe") continue;

		auto& threadStacks = threads_it->second.Stacks();
		auto  stacksEnd = threadStacks.IteratorEnd();

		// Write tid and open writing for that tid
		if (first)
			outputFile << "\"" << threads_it->first << "\":[";
		else
			outputFile << ",\n\"" << threads_it->first << "\":[";

		// In bracket to safely remove threadCompletedFunctions from memory
		{
			LiveStack liveStack;
			std::vector<std::string> threadCompletedFunctions;
			std::unordered_map<base::Tid, std::vector<std::string>> completedFunctions;

			// Traverse all stacks comparing first and second to see what functions ended
			for (auto it = threadStacks.IteratorFromTimestamp(0); it != stacksEnd; ++it)
			{
				std::vector<std::string> actualCompletedFunctions = liveStack.Update((*it));
				threadCompletedFunctions.insert(std::end(threadCompletedFunctions), std::begin(actualCompletedFunctions), std::end(actualCompletedFunctions));
				
				if (threadCompletedFunctions.size() >= 100000)
				{
					auto nextIt = it + 1;
					if (nextIt == stacksEnd)
						JsonWriter::writeStacks(outputFile, threadCompletedFunctions, true);
					else
						JsonWriter::writeStacks(outputFile, threadCompletedFunctions, false);
					threadCompletedFunctions.clear();
				}
			}
			std::vector<std::string> finalCompletedFunctions = liveStack.GetFinalLines();
			threadCompletedFunctions.insert(std::end(threadCompletedFunctions), std::begin(finalCompletedFunctions), std::end(finalCompletedFunctions));

			JsonWriter::writeStacks(outputFile, threadCompletedFunctions, true);
		}

		outputFile << "]";

		first = false;
	}
}