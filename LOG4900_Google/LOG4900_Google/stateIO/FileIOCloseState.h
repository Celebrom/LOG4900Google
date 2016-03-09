#pragma once
#include "AbstractState.h"

class FileIOCloseState : public AbstractState{
public:
	FileIOCloseState();
	~FileIOCloseState();
	virtual std::string returnJson(std::vector<std::string> lines);
};