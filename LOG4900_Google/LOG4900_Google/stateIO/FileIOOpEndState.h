#pragma once
#include "AbstractState.h"

class FileIOOpEndState : public AbstractState{
public:
	FileIOOpEndState();
	~FileIOOpEndState();
	virtual std::string returnJson(std::vector<std::string> lines);
};