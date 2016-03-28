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

#include "Utils.h"

std::vector<std::string> Utils::removeSpaces(std::vector<std::string> tokens)
{
	std::vector<std::string> updatedTokens;

	for each(std::string token in tokens)
	{
		updatedTokens.push_back(trim(token));
	}
	return updatedTokens;
}

// trim from start
std::string& Utils::ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
std::string& Utils::rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
std::string& Utils::trim(std::string &s) {
	return ltrim(rtrim(s));
}

void Utils::formatFileName(std::string &FileName)
{
	FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());
	std::replace(FileName.begin(), FileName.end(), '\\', '/');
}

std::string Utils::extractPidFromString(std::string& word)
{
	unsigned first = word.find("(");
	unsigned last = word.find(")");
	return word.substr(first + 1, last - first - 1);
}