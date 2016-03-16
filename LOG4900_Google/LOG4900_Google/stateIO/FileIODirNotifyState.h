#pragma once
#include "AbstractState.h"

class FileIODirNotifyState : public AbstractState{
public:
	FileIODirNotifyState();
	~FileIODirNotifyState();
	virtual std::string returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd);
};