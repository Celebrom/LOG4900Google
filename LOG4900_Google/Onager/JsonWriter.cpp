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

void JsonWriter::writeChromeEvents(std::ofstream& outputFile, std::vector<std::string>& chromeEventLines, bool isLast)
{
	if (isLast)
	{
		for (unsigned int i = 0; i < chromeEventLines.size(); ++i)
		{
			if (i < chromeEventLines.size() - 1)
				outputFile << chromeEventLines[i] << ",\n";
			else if (i == chromeEventLines.size() - 1)
				outputFile << chromeEventLines[i] << "\n";
		}
	}
	else
	{
		for (unsigned int i = 0; i < chromeEventLines.size(); ++i)
			outputFile << chromeEventLines[i] << ",\n";
	}
}

void JsonWriter::writeStacks(std::ofstream& outputFile, std::vector<std::string> threadCompletedFunctions, bool isLast)
{
	if (isLast)
	{
		for (unsigned int i = 0; i < threadCompletedFunctions.size(); ++i)
		{
			if (i < threadCompletedFunctions.size() - 1)
				outputFile << threadCompletedFunctions[i] << ",\n";
			else if (i == threadCompletedFunctions.size() - 1)
				outputFile << threadCompletedFunctions[i] << "\n";
		}
	}
	else
	{
		for (unsigned int i = 0; i < threadCompletedFunctions.size(); ++i)
			outputFile << threadCompletedFunctions[i] << ",\n";
	}
}