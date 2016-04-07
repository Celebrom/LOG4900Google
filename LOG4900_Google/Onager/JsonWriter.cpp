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

#include "JsonWriter.h"

bool JsonWriter::firstEvent = true;
void JsonWriter::writeChromeEvents(std::wstring path, std::vector<std::string>& chromeEventLines)
{
	std::ofstream outputFile;
	if (firstEvent)
	{
		outputFile.open(path);
		firstEvent = false;
		outputFile << "{\"traceEvents\":[";
	}
	else
		outputFile.open(path, std::ios_base::app);

	for (unsigned int i = 0; i < chromeEventLines.size(); ++i)
	{
		if (i < chromeEventLines.size() - 1)
			outputFile << chromeEventLines[i] << ",\n";
		else if (i == chromeEventLines.size() - 1)
			outputFile << chromeEventLines[i] << "\n";
	}
	outputFile.close();
}

bool JsonWriter::firstStack = true;
void JsonWriter::writeStacks(std::wstring path, std::unordered_map<base::Tid, std::vector<std::string>>& stackEventLines, bool lastStack)
{
	std::ofstream outputFile(path, std::ios_base::app);

	if (firstStack)
	{
		outputFile << "],\n\"stacks\":{";
		firstStack = false;
	}
		
	for (auto& it = stackEventLines.begin(); it != stackEventLines.end(); ++it)
	{
		outputFile << "\"" << (*it).first << "\":[";
		for (unsigned int i = 0; i < (*it).second.size(); ++i)
		{
			if (i < (*it).second.size() - 1)
				outputFile << (*it).second[i] << ",\n";
			else if (i == (*it).second.size() - 1)
				outputFile << (*it).second[i] << "\n";
		}

		if (!lastStack)
			outputFile << "],\n";
		else
			outputFile << "]}}";
	}
	
	outputFile.close();
}