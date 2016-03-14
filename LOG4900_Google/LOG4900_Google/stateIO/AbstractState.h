#pragma once
#include <vector>
class AbstractState
{
public:
	AbstractState();
	AbstractState(std::vector<std::string> lines);
	std::string extractPidFromString(std::string &word);
	std::string ifNotEmpty(std::string tag, std::string value);
	~AbstractState();
	virtual std::string returnJson(std::vector<std::string> lines);
};

