#pragma once
#include "AbstractState.h"

class FileIOReadState : public AbstractState{
public:
	FileIOReadState();
	~FileIOReadState();
	virtual std::string returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd);
};