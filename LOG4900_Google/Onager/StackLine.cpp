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
#include "StackLine.h"
#include "Utils.h"

StackLine::StackLine(int id, std::string name, int parent, int beginTimestamp)
{
    Utils::formatFileName(name);
	this->id = id;
	this->name = name;
	this->parent = parent;
	this->beginTimestamp = beginTimestamp;
	this->endTimestamp = -1;
}

std::string StackLine::ToJson()
{
	return "{\"id\":" + std::to_string(id)
			+ ",\"name\":\"" + name + "\""
			+ ",\"parent\":" + std::to_string(parent) 
			+ ",\"begin\":" + std::to_string(beginTimestamp)
			+ ",\"duration\":" + std::to_string(endTimestamp - beginTimestamp)
			+ "}";
}