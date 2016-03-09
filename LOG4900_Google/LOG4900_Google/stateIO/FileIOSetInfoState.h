#pragma once
#include "AbstractState.h"

class FileIOSetInfoState : public AbstractState{
public:
	FileIOSetInfoState();
	~FileIOSetInfoState();
	virtual std::string returnJson(std::vector<std::string> lines);
};