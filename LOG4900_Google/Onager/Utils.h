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

#ifndef LOG4900GOOGLEJO_LOG4900GOOGLE_ONAGER_UTILS_H
#define LOG4900GOOGLEJO_LOG4900GOOGLE_ONAGER_UTILS_H

#include <string>
#include <algorithm>
#include <vector>
#include <functional> 
#include <cctype>

class Utils
{
public:
		Utils(){};

		template < class ContainerT >
		static void tokenize(const std::string& str, ContainerT& tokens, const std::string& delimiters = " ", bool trimEmpty = false)
		{   //http://stackoverflow.com/questions/236129/split-a-string-in-c
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

		static std::vector<std::string> removeSpaces(std::vector<std::string> tokens);
		/*TODO: optimize constant references*/
		static inline std::string& ltrim(std::string &s);
		static inline std::string& rtrim(std::string &s);
		static inline std::string& trim(std::string &s);

		static void formatFileName(std::string *FileName);

		static std::string extractPidFromString(const std::string& word);

private:
		
};

#endif // LOG4900GOOGLEJO_LOG4900GOOLE_ONAGER_UTILS_H
