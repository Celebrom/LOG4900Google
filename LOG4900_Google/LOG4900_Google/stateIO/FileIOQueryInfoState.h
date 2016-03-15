#pragma once
#include "AbstractState.h"

class FileIOQueryInfoState : public AbstractState{
public:
	FileIOQueryInfoState();
	~FileIOQueryInfoState();
	virtual std::string returnJson(std::vector<std::string> lines);
};