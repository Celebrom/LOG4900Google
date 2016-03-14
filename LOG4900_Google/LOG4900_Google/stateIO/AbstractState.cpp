#include "AbstractState.h"


AbstractState::AbstractState()
{
}


AbstractState::~AbstractState()
{
}

std::string AbstractState::returnJson(std::vector<std::string> lines){
	return "";
}

std::string AbstractState::extractPidFromString(std::string &word)
{
	unsigned first = word.find("(");
	unsigned last = word.find(")");
	return word.substr(first + 1, last - first - 1);
}

std::string AbstractState::ifNotEmpty(std::string tag, std::string value)
{
	if (value != "")
		return "\"" + tag + "\":" + value + ", ";
	return "";
}
