#include "StackLine.h"

StackLine::StackLine(int id, std::string name, int parent, int beginTimestamp)
{
	this->id = id;
	this->name = formatFileName(name);
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

std::string StackLine::formatFileName(std::string& FileName)
{
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());
		std::replace(FileName.begin(), FileName.end(), '\\', '/');
		return FileName;
}