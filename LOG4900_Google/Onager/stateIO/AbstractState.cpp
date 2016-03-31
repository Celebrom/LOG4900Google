#include "AbstractState.h"

AbstractState::AbstractState(){}

AbstractState::~AbstractState(){}

std::string AbstractState::returnJson(std::vector<std::string>* FileIoEvent, const std::vector<std::string>& OpEnd){
	return "";
}

std::string AbstractState::ifNotEmpty(const std::string tag, const std::string value)
{
		if (value != "")
				return "\"" + tag + "\":\"" + value + "\"";
	  return "";
}

std::string AbstractState::commonJson(const std::vector<std::string>& FileIoEvent, const std::vector<std::string>& OpEnd)
{
		return "\"cat\":\"IO\",\"ph\":\"X\",\"ts\":" + FileIoEvent[1] + "," +
				"\"dur\":" + OpEnd[9] + "," +
				"\"pid\":" + Utils::extractPidFromString(FileIoEvent[2]) + "," +
				"\"tid\":" + FileIoEvent[3] + "," +
				"\"args\":{";
}
