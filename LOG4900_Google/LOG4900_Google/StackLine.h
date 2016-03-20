#pragma once
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
	std::string formatFileName(std::string& FileName);

	int id;
	std::string name;
	int parent;
	int beginTimestamp;
	int endTimestamp;
};