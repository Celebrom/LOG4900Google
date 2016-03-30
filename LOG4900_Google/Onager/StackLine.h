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

#ifndef LOG4900GOOGLEJO_LOG4900GOOGLE_ONAGER_STACKLINE_H
#define LOG4900GOOGLEJO_LOG4900GOOGLE_ONAGER_STACKLINE_H

#include <string>
#include <algorithm>

class StackLine
{
public:
	StackLine(int id, std::string name, int parent, int beginTimestamp);

	std::string ToJson();

	int GetID() { return id; }
	std::string GetName() { return name; }
	int GetParent() { return parent; }
	int GetBeginTimestamp() { return beginTimestamp; }
	int GetEndTimestamp() { return endTimestamp; }
	int GetDuration() { return endTimestamp == -1 ? -1 : (endTimestamp - beginTimestamp); }
	
	void SetEndTimestamp(int timestamp) { endTimestamp = timestamp; }

private:
	int id;
	std::string name;
	int parent;
	int beginTimestamp;
	int endTimestamp;
};

#endif // LOG4900GOOGLEJO_LOG4900GOOLE_ONAGER_STACKLINE_H