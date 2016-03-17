#pragma once
#include "AbstractState.h"

class FileIOCleanUpState : public AbstractState{
public:
	FileIOCleanUpState();
	~FileIOCleanUpState();
	virtual std::string returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd);
};