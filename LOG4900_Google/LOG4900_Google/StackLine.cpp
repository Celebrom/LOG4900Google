#include "StackLine.h"

StackLine::StackLine(int id, std::string name, int parent, int beginTimestamp)
{
	this->id = id;
	this->name = name;
	this->parent = parent;
	this->beginTimestamp = beginTimestamp;
	this->endTimestamp = -1;
}

std::string StackLine::ToJson()
{
	return "";
}