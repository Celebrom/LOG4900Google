#pragma once
#include <vector>
class AbstractState
{
public:
	AbstractState();
	AbstractState(std::vector<std::string> lines);
	~AbstractState();
	virtual std::string returnJson(std::vector<std::string> lines);
};

