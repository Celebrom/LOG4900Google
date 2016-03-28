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

#pragma once
#include <unordered_map>
#include "stateIO\IoStateManager.h"

class Converter
{
public:
	Converter(){ stateIO = new IoStateManager(); };
	~Converter(){ delete stateIO; };

	std::string IOLineToJSON(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd);
	std::string DiskLineToJSON(std::vector<std::string>& diskEndEvent);
	std::string EventToJSON(std::vector<std::string>& line);
	std::string CSwitchToJson(std::vector<std::string>& CSwitchEvent, std::string type, unsigned int id);

private:
	std::string getPhase(std::string &word);
	IoStateManager* stateIO;
};