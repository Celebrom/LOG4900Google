#pragma once
#include "AbstractState.h"

class FileIOWriteState : public AbstractState{
public:
	FileIOWriteState();
	~FileIOWriteState();
	virtual std::string returnJson(std::vector<std::string> lines);
};