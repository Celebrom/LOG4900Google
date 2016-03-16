#pragma once
#include "AbstractState.h"

class FileIOFlushState : public AbstractState{
public:
	FileIOFlushState();
	~FileIOFlushState();
	virtual std::string returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd);
};