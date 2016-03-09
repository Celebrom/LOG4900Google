#pragma once
#include "AbstractState.h"

class FileIODeleteState : public AbstractState{
public:
	FileIODeleteState();
	~FileIODeleteState();
	virtual std::string returnJson(std::vector<std::string> lines);
};