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

#ifndef LOG4900GOOGLEJO_LOG4900GOOGLE_ONAGER_LIVESTACK_H
#define LOG4900GOOGLEJO_LOG4900GOOGLE_ONAGER_LIVESTACK_H

#include "StackLine.h"
#include "../base/history.h"
#include "../etw_reader/stack.h"
#include <string>
#include <vector>

using namespace etw_insights;
class LiveStack
{
public:
	std::vector<std::string> GetFinalLines();
	std::vector<std::string> Update(base::History<Stack>::Element nextStack);

private:
	std::vector<StackLine> lines;
	base::Timestamp lastTimestamp;

	static int idCounter;
};

#endif /* LOG4900GOOGLEJO_LOG4900GOOLE_ONAGER_LIVESTACK_H */