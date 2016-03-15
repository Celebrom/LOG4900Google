#pragma once
#include "AbstractState.h"

class FileIOCreateState : public AbstractState{
public:
	FileIOCreateState();
	~FileIOCreateState();
	virtual std::string returnJson(std::vector<std::string> lines);
};

