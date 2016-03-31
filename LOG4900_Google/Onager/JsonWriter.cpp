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

void JsonWriter::write(std::wstring path, const std::vector<std::string>& chromeEventLines, const std::unordered_map<base::Tid, std::vector<std::string>>& stackEventLines)
{
	std::ofstream outputFile(path);

	outputFile << "{\"traceEvents\":[";
	for (unsigned int i = 0; i < chromeEventLines.size(); ++i)
	{
		if (i < chromeEventLines.size() - 1)
			outputFile << chromeEventLines[i] << ",\n";
		else if (i == chromeEventLines.size() - 1)
			outputFile << chromeEventLines[i] << "\n";
	}
	outputFile << "],\n\"stacks\":{";
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

		if (it != --stackEventLines.end())
			outputFile << "],\n";
		else
			outputFile << "]";
	}
	outputFile << "}}";

	outputFile.close();
}