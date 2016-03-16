#pragma once
#include "AbstractState.h"

class FileIODirEnumState : public AbstractState{
public:
	FileIODirEnumState();
	~FileIODirEnumState();
	virtual std::string returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd);
};