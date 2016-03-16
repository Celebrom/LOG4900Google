#pragma once
#include "AbstractState.h"

class FileIOFSCTLState : public AbstractState{
public:
	FileIOFSCTLState();
	~FileIOFSCTLState();
	virtual std::string returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd);
};