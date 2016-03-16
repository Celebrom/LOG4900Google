#include "AbstractState.h"


AbstractState::AbstractState()
{
}


AbstractState::~AbstractState()
{
}

std::string AbstractState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	return "";
}

std::string AbstractState::extractPidFromString(std::string& word)
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

std::string AbstractState::commonJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd)
{
		std::string commonLine = "{ \"name\": \"" + FileIoEvent[0] + "\","+
				                      " \"cat\" : \"IO\","+
															" \"ph\" : \"X\","+
															" \"ts\" : " + FileIoEvent[1] + ","+
															" \"dur\" : " + OpEnd[9] + ","+
															" \"pid\" : " + extractPidFromString(FileIoEvent[2]) + ","+
															" \"tid\" : " + FileIoEvent[3] + ",";
		return commonLine;
}
