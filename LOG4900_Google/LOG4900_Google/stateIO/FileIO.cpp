
#include "FileIOCreateState.h"
#include "FileIOCleanUpState.h"
#include "FileIOCloseState.h"
#include "FileIODeleteState.h"
#include "FileIODirEnumState.h"
#include "FileIODirNotifyState.h"
#include "FileIOFlushState.h"
#include "FileIOFSCTLState.h"
#include "FileIOOpEndState.h"
#include "FileIOQueryInfoState.h"
#include "FileIOReadState.h"
#include "FileIORenameState.h"
#include "FileIOSetInfoState.h"
#include "FileIOWriteState.h"
#include "AbstractState.h"
#include <functional> 
#include <algorithm>


/*1- FileIOCreateState*/
FileIOCreateState::FileIOCreateState(){}
FileIOCreateState::~FileIOCreateState(){}
std::string FileIOCreateState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
		std::string FileName = FileIoEvent[12];
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[12];
		return "{ \"name\": \"[" + FileIoEvent[0] + "]" + FileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*2- FileIOCleanUpState*/
FileIOCleanUpState::FileIOCleanUpState(){}
FileIOCleanUpState::~FileIOCleanUpState(){}
std::string FileIOCleanUpState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
		std::string FileName = FileIoEvent[9];
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[9];
		return "{ \"name\": \"[" + FileIoEvent[0] + "] " + FileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*3- FileIOCloseState*/
FileIOCloseState::FileIOCloseState(){}
FileIOCloseState::~FileIOCloseState(){}
std::string FileIOCloseState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
		std::string FileName = FileIoEvent[9];
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[9];
		return "{ \"name\": \"[" + FileIoEvent[0] + "] " + FileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*4- FileIODeleteState*/
FileIODeleteState::FileIODeleteState(){}
FileIODeleteState::~FileIODeleteState(){}
std::string FileIODeleteState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
		std::string FileName = FileIoEvent[11];
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[11];
		return "{ \"name\": \"[" + FileIoEvent[0] + "] " + FileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*5- FileIODirEnumState*/
FileIODirEnumState::FileIODirEnumState(){}
FileIODirEnumState::~FileIODirEnumState(){}
std::string FileIODirEnumState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
		std::string FileName = FileIoEvent[12];
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[12];
		return "{ \"name\": \"[" + FileIoEvent[0] + "] " + FileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + 
				ifNotEmpty("Size", FileIoEvent[10]) + "}}";
}

/*6- FileIODirNotifyState*/
FileIODirNotifyState::FileIODirNotifyState(){}
FileIODirNotifyState::~FileIODirNotifyState(){}
std::string FileIODirNotifyState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
		std::string FileName = FileIoEvent[12];
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[12];
		return "{ \"name\": \"[" + FileIoEvent[0] + "] " + FileName + "\"," +
				commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("Size", FileIoEvent[10]) + "}}";
}

/*7- FileIOFlushState*/
FileIOFlushState::FileIOFlushState(){}
FileIOFlushState::~FileIOFlushState(){}
std::string FileIOFlushState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
		std::string FileName = FileIoEvent[9];
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[9];
		return "{ \"name\": \"[" + FileIoEvent[0] + "] " + FileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*8- FileIOFSCTLState*/
FileIOFSCTLState::FileIOFSCTLState(){}
FileIOFSCTLState::~FileIOFSCTLState(){}
std::string FileIOFSCTLState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
		std::string FileName = FileIoEvent[11];
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[11];
		return "{ \"name\": \"[" + FileIoEvent[0] + "] " + FileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*9- FileIOOpEndState*/
FileIOOpEndState::FileIOOpEndState(){}
FileIOOpEndState::~FileIOOpEndState(){}
std::string FileIOOpEndState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
		std::string FileName = FileIoEvent[13];
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[13];
		return "{ \"name\": \"[" + FileIoEvent[0] + "] " + FileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*10- FileIOQueryInfoState*/
FileIOQueryInfoState::FileIOQueryInfoState(){}
FileIOQueryInfoState::~FileIOQueryInfoState(){}
std::string FileIOQueryInfoState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
		std::string FileName = FileIoEvent[11];
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[11];
		return "{ \"name\": \"[" + FileIoEvent[0] + "] " + FileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*11- FileIOReadState*/
FileIOReadState::FileIOReadState(){}
FileIOReadState::~FileIOReadState(){}
std::string FileIOReadState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
		std::string FileName = FileIoEvent[14];
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[14];
		return "{ \"name\": \"[" + FileIoEvent[0] + "] " + FileName + "\"," +
				commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("Priority", FileIoEvent[13]) + ", " +
				ifNotEmpty("Size", FileIoEvent[10]) + "}}";
}

/*12- FileIORenameState*/
FileIORenameState::FileIORenameState(){}
FileIORenameState::~FileIORenameState(){}
std::string FileIORenameState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
		std::string FileName = FileIoEvent[11];
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[11];
		return "{ \"name\": \"[" + FileIoEvent[0] + "] " + FileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*13- FileIOSetInfoState*/
FileIOSetInfoState::FileIOSetInfoState(){}
FileIOSetInfoState::~FileIOSetInfoState(){}
std::string FileIOSetInfoState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
		std::string FileName = FileIoEvent[11];
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[11];
		return "{ \"name\": \"[" + FileIoEvent[0] + "] " + FileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*14- FileIOWriteState*/
FileIOWriteState::FileIOWriteState(){}
FileIOWriteState::~FileIOWriteState(){}
std::string FileIOWriteState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
		std::string FileName = FileIoEvent[14];
		FileName.erase(std::remove(FileName.begin(), FileName.end(), '"'), FileName.end());

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[14];
		return "{ \"name\": \"[" + FileIoEvent[0] + "] " + FileName + "\"," +
				commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("Priority", FileIoEvent[13]) + ", " +
				ifNotEmpty("Size", FileIoEvent[10]) + "}}";
}

