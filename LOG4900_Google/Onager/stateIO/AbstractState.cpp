#include "AbstractState.h"

AbstractState::AbstractState(){}

AbstractState::~AbstractState(){}

std::string AbstractState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
	return "";
}

std::string AbstractState::ifNotEmpty(std::string tag, std::string value)
{
		if (value != "")
				return "\"" + tag + "\":\"" + value + "\"";
	  return "";
}

std::string AbstractState::commonJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd)
{
		std::string pid = FileIoEvent.GetFieldAsString("Process Name ( PID)");
		return "\"cat\":\"IO\",\"ph\":\"X\",\"ts\":" + FileIoEvent.GetFieldAsString("TimeStamp") + "," +
				"\"dur\":" + OpEnd.GetFieldAsString("ElapsedTime") + "," +
				"\"pid\":" + Utils::extractPidFromString(pid) + "," + 
				"\"tid\":" + FileIoEvent.GetFieldAsString("ThreadID") + "," +
				"\"args\":{";
}

