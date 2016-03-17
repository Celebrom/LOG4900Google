
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


/*1- FileIOCreateState*/
FileIOCreateState::FileIOCreateState(){}
FileIOCreateState::~FileIOCreateState(){}
std::string FileIOCreateState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	  /*FileIoCreate,  TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, Options, Attributes, ShareAccess, FileName, ParsedOptions, ParsedAttributes, ParsedShareAccess*/
		std::string line = commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("cpu", FileIoEvent[6]) + ", " +
				ifNotEmpty("IrpPtr", FileIoEvent[7]) + ", " +
				ifNotEmpty("FileObject", FileIoEvent[8]) + ", " +
				ifNotEmpty("options", FileIoEvent[9]) + ", " +
				ifNotEmpty("Attributes", FileIoEvent[10]) + ", " +
				ifNotEmpty("sharedAccess", FileIoEvent[11]) + ", " +
				ifNotEmpty("FileName", FileIoEvent[12]) + ", " +
				ifNotEmpty("ParsedOptions", FileIoEvent[13]) + ", " +
				ifNotEmpty("ParsedAttributes", FileIoEvent[14]) + ", " +
		    ifNotEmpty("ParsedShareAccess", FileIoEvent[15]);
	  return line + '}';
}

/*2- FileIOCleanUpState*/
FileIOCleanUpState::FileIOCleanUpState(){}
FileIOCleanUpState::~FileIOCleanUpState(){}
std::string FileIOCleanUpState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	/*FileIoCleanup,TimeStamp, Process Name ( PID),   ThreadID, LoggingProcessName(PID), LoggingThreadID, CPU,IrpPtr, FileObject, FileName*/
	/* FileIoCleanup, 14195148, chrome.exe (12192), 12352, chrome.exe (12192), 12352, 2, 0xffffe00018843568, 0xffffe000142b0070, "C:\Users\Alexandre\AppData\Local\Google\Chrome\User Data\Default\databases\Databases.db-journal"*/
		std::string line = commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("cpu", FileIoEvent[6]) + ", " +
				ifNotEmpty("IrpPtr", FileIoEvent[7]) + ", " +
				ifNotEmpty("FileObject", FileIoEvent[8]) + ", " +
				ifNotEmpty("FileName", FileIoEvent[9]);
		return line + '}';
}

/*3- FileIOCloseState*/
FileIOCloseState::FileIOCloseState(){}
FileIOCloseState::~FileIOCloseState(){}
std::string FileIOCloseState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	/*FileIoClose,  TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU,IrpPtr, FileObject, FileName*/
	/* FileIoClose, 14195158, chrome.exe (12192), 12352, chrome.exe (12192), 12352, 2, 0xffffe00018843568, 0xffffe000142b0070, "C:\Users\Alexandre\AppData\Local\Google\Chrome\User */
		std::string line = commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("cpu", FileIoEvent[6]) + ", " +
				ifNotEmpty("IrpPtr", FileIoEvent[7]) + ", " +
				ifNotEmpty("FileObject", FileIoEvent[8]) + ", " +
				ifNotEmpty("FileName", FileIoEvent[9]);
		return line + '}';
}

/*4- FileIODeleteState*/
FileIODeleteState::FileIODeleteState(){}
FileIODeleteState::~FileIODeleteState(){}
std::string FileIODeleteState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	/*FileIoDelete, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
		std::string line = commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("cpu", FileIoEvent[6]) + ", " +
				ifNotEmpty("IrpPtr", FileIoEvent[7]) + ", " +
				ifNotEmpty("FileObject", FileIoEvent[8]) + ", " +
				ifNotEmpty("ExtraInfo", FileIoEvent[9]) + ", " +
				ifNotEmpty("InfoClass", FileIoEvent[10]) + ", " +
				ifNotEmpty("FileName", FileIoEvent[11]);		
		return line + '}';
}

/*5- FileIODirEnumState*/
FileIODirEnumState::FileIODirEnumState(){}
FileIODirEnumState::~FileIODirEnumState(){}
std::string FileIODirEnumState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	/*FileIoDirEnum, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, FileIndex, Size, InfoClass, FileName, FileName*/
		std::string line = commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("cpu", FileIoEvent[6]) + ", " +
				ifNotEmpty("IrpPtr", FileIoEvent[7]) + ", " +
				ifNotEmpty("FileObject", FileIoEvent[8]) + ", " +
				ifNotEmpty("FileIndex", FileIoEvent[9]) + ", " +
				ifNotEmpty("Size", FileIoEvent[10]) + ", " +
				ifNotEmpty("InfoClass", FileIoEvent[11]) + ", " +
				ifNotEmpty("FileName", FileIoEvent[12]);
		return line + '}';
}

/*6- FileIODirNotifyState*/
FileIODirNotifyState::FileIODirNotifyState(){}
FileIODirNotifyState::~FileIODirNotifyState(){}
std::string FileIODirNotifyState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	/*FileIoDirNotify, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, FileIndex, Size, InfoClass, FileName, FileName*/
		std::string line = commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("cpu", FileIoEvent[6]) + ", " +
				ifNotEmpty("IrpPtr", FileIoEvent[7]) + ", " +
				ifNotEmpty("FileObject", FileIoEvent[8]) + ", " +
				ifNotEmpty("FileIndex", FileIoEvent[9]) + ", " +
				ifNotEmpty("Size", FileIoEvent[10]) + ", " +
				ifNotEmpty("InfoClass", FileIoEvent[11]) + ", " +
				ifNotEmpty("FileName", FileIoEvent[12]);
		return line + '}';
}

/*7- FileIOFlushState*/
FileIOFlushState::FileIOFlushState(){}
FileIOFlushState::~FileIOFlushState(){}
std::string FileIOFlushState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	/*FileIoFlush, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, FileName*/
		std::string line = commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("cpu", FileIoEvent[6]) + ", " +
				ifNotEmpty("IrpPtr", FileIoEvent[7]) + ", " +
				ifNotEmpty("FileObject", FileIoEvent[8]) + ", " +
				ifNotEmpty("FileName", FileIoEvent[9]);
		return line + '}';
}

/*8- FileIOFSCTLState*/
FileIOFSCTLState::FileIOFSCTLState(){}
FileIOFSCTLState::~FileIOFSCTLState(){}
std::string FileIOFSCTLState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	/*FileIoFSCTL, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
		std::string line = commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("cpu", FileIoEvent[6]) + ", " +
				ifNotEmpty("IrpPtr", FileIoEvent[7]) + ", " +
				ifNotEmpty("FileObject", FileIoEvent[8]) + ", " +
				ifNotEmpty("ExtraInfo", FileIoEvent[9]) + ", " +
				ifNotEmpty("InfoClass", FileIoEvent[10]) + ", " +
				ifNotEmpty("FileName", FileIoEvent[11]);
		return line + '}';
}

/*9- FileIOOpEndState*/
FileIOOpEndState::FileIOOpEndState(){}
FileIOOpEndState::~FileIOOpEndState(){}
std::string FileIOOpEndState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	/*FileIoOpEnd, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ElapsedTime, Status, ExtraInfo, Type, FileName*/
	/*FileIoOpEnd, 1626934, chrome.exe(9336), 6332, chrome.exe(9336), 6332, 0, 0xffffe00014cb3b58, 0xffffe000186e3070, 233, 0x00000000, 0x0000000000004000*/
		std::string line = commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("cpu", FileIoEvent[6]) + ", " +
				ifNotEmpty("IrpPtr", FileIoEvent[7]) + ", " +
				ifNotEmpty("FileObject", FileIoEvent[8]) + ", " +
				ifNotEmpty("ExtraInfo", FileIoEvent[9]) + ", " +
				ifNotEmpty("Status", FileIoEvent[10]) + ", " +
				ifNotEmpty("ExtraInfo", FileIoEvent[11]) + ", " +
				ifNotEmpty("Type", FileIoEvent[12]) + ", " +
				ifNotEmpty("FileName", FileIoEvent[13]);
		return line + '}';
}

/*10- FileIOQueryInfoState*/
FileIOQueryInfoState::FileIOQueryInfoState(){}
FileIOQueryInfoState::~FileIOQueryInfoState(){}
std::string FileIOQueryInfoState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	/*FileIoQueryInfo, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
	/*FileIoQueryInfo, 14195102, chrome.exe (12192), 12352, chrome.exe (12192), 12352, 2, 0xffffe0001cbb8508, 0xffffe000142b0070, 0x0000000000000000, FileBasicInformation, "C:\Users\Alexandre\AppData\Local\Google\Chrome\User Data\Default\databases\Databases.db-journal"*/
		std::string line = commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("cpu", FileIoEvent[6]) + ", " +
				ifNotEmpty("IrpPtr", FileIoEvent[7]) + ", " +
				ifNotEmpty("FileObject", FileIoEvent[8]) + ", " +
				ifNotEmpty("ExtraInfo", FileIoEvent[9]) + ", " +
				ifNotEmpty("InfoClass", FileIoEvent[10]) + ", " +
				ifNotEmpty("FileName", FileIoEvent[11]);
		return line + '}';
}

/*11- FileIOReadState*/
FileIOReadState::FileIOReadState(){}
FileIOReadState::~FileIOReadState(){}
std::string FileIOReadState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	/*FileIoRead, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ByteOffset, Size, Flags, ExtraFlags, Priority, FileName, ParsedFlags*/
	/*FileIoRead, 14195127, chrome.exe (12192), 12352, chrome.exe (12192), 12352, 2, 0xffffe00018843568, 0xffffe000142b0070, 0x0000000000, 0x00000001, 0x00060900, 0x00000000, Normal, */
		std::string line = commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("cpu", FileIoEvent[6]) + ", " +
				ifNotEmpty("IrpPtr", FileIoEvent[7]) + ", " +
				ifNotEmpty("FileObject", FileIoEvent[8]) + ", " +
				ifNotEmpty("ByteOffset", FileIoEvent[9]) + ", " +
				ifNotEmpty("Size", FileIoEvent[10]) + ", " +
				ifNotEmpty("Flags", FileIoEvent[11]) + ", " +
				ifNotEmpty("ExtraFlags", FileIoEvent[12]) + ", " +
				ifNotEmpty("Priority", FileIoEvent[13]) + ", " +
				ifNotEmpty("FileName", FileIoEvent[14]) + ", " +
				ifNotEmpty("ParsedFlags", FileIoEvent[15]) ;
		return line + '}';
}

/*12- FileIORenameState*/
FileIORenameState::FileIORenameState(){}
FileIORenameState::~FileIORenameState(){}
std::string FileIORenameState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	/*FileIoRename, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
		std::string line = commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("cpu", FileIoEvent[6]) + ", " +
				ifNotEmpty("IrpPtr", FileIoEvent[7]) + ", " +
				ifNotEmpty("FileObject", FileIoEvent[8]) + ", " +
				ifNotEmpty("ExtraInfo", FileIoEvent[9]) + ", " +
				ifNotEmpty("InfoClass", FileIoEvent[10]) + ", " +
				ifNotEmpty("FileName", FileIoEvent[11]);
		return line + '}';
}

/*13- FileIOSetInfoState*/
FileIOSetInfoState::FileIOSetInfoState(){}
FileIOSetInfoState::~FileIOSetInfoState(){}
std::string FileIOSetInfoState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	/*FileIoSetInfo, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
		std::string line = commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("cpu", FileIoEvent[6]) + ", " +
				ifNotEmpty("IrpPtr", FileIoEvent[7]) + ", " +
				ifNotEmpty("FileObject", FileIoEvent[8]) + ", " +
				ifNotEmpty("ExtraInfo", FileIoEvent[9]) + ", " +
				ifNotEmpty("InfoClass", FileIoEvent[10]) + ", " +
				ifNotEmpty("FileName", FileIoEvent[11]);
		return line + '}';
}

/*14- FileIOWriteState*/
FileIOWriteState::FileIOWriteState(){}
FileIOWriteState::~FileIOWriteState(){}
std::string FileIOWriteState::returnJson(std::vector<std::string>& FileIoEvent, std::vector<std::string>& OpEnd){
	/*FileIoWrite, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ByteOffset, Size, Flags, ExtraFlags, Priority, FileName, ParsedFlags*/
		std::string line = commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("cpu", FileIoEvent[6]) + ", " +
				ifNotEmpty("IrpPtr", FileIoEvent[7]) + ", " +
				ifNotEmpty("FileObject", FileIoEvent[8]) + ", " +
				ifNotEmpty("ByteOffset", FileIoEvent[9]) + ", " +
				ifNotEmpty("Size", FileIoEvent[10]) + ", " +
				ifNotEmpty("Flags", FileIoEvent[11]) + ", " +
				ifNotEmpty("ExtraFlags", FileIoEvent[12]) + ", " +
				ifNotEmpty("Priority", FileIoEvent[13]) + ", " +
				ifNotEmpty("FileName", FileIoEvent[14]) + ", " +
				ifNotEmpty("ParsedFlags", FileIoEvent[15]);
		return line + '}';
}

