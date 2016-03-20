#pragma once
#include <vector>
class AbstractState
{
public:
	AbstractState();
	AbstractState(std::vector<std::string>& lines);
	~AbstractState();

	std::string extractPidFromString(std::string& word);
	std::string ifNotEmpty(std::string tag, std::string value);
	void formatFileName(std::string& FileName);
	virtual std::string returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd);
	std::string commonJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd);
};

