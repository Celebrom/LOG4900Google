
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


/*
fo : FileObject
ip : IrpPtr
et : ElapsedTime
st : Status
ei: ExtraInfo
ty : Type
fn : FileName
ic : InfoClass
fi : FileIndex
op : options
at : Attributes
sa : sharedAccess
po : ParsedOptions
pa : ParsedAttributes
ps : ParsedShareAccess
pf : ParsedFlags
bo : ByteOffset
sz : Size
fs : Flags
efs : ExtraFlags
pt : Priority
*/


/*1- FileIOCreateState*/
FileIOCreateState::FileIOCreateState(){}
FileIOCreateState::~FileIOCreateState(){}
std::string FileIOCreateState::returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd){
	/*FileIoCreate,  TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, Options, Attributes, ShareAccess, FileName, ParsedOptions, ParsedAttributes, ParsedShareAccess*/
		std::string line = "\"ts\":" + FileIoEvent[1] + ", " +
		"\"pid\":" + extractPidFromString(FileIoEvent[2]) + ", "
		"\"tid\":" + FileIoEvent[3] + ", ";
	// (jump the repetition)
	line += "\"cpu\":" + FileIoEvent[6] + ", " +
		ifNotEmpty("ip", FileIoEvent[7]) +
		ifNotEmpty("fo", FileIoEvent[8]) +
		ifNotEmpty("op", FileIoEvent[9]) +
		ifNotEmpty("at", FileIoEvent[10]) +
		ifNotEmpty("sa", FileIoEvent[11]) +
		ifNotEmpty("fn", FileIoEvent[12]) +
		ifNotEmpty("po", FileIoEvent[13]) +
		ifNotEmpty("pa", FileIoEvent[14]) +
		ifNotEmpty("ps", FileIoEvent[15]);
	return line;
}

/*2- FileIOCleanUpState*/
FileIOCleanUpState::FileIOCleanUpState(){}
FileIOCleanUpState::~FileIOCleanUpState(){}
std::string FileIOCleanUpState::returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd){
	/*FileIoCleanup,TimeStamp, Process Name ( PID),   ThreadID, LoggingProcessName(PID), LoggingThreadID, CPU,IrpPtr, FileObject, FileName*/
	/* FileIoCleanup, 14195148, chrome.exe (12192), 12352, chrome.exe (12192), 12352, 2, 0xffffe00018843568, 0xffffe000142b0070, "C:\Users\Alexandre\AppData\Local\Google\Chrome\User Data\Default\databases\Databases.db-journal"*/
	std::string line = "\"ts\":" + FileIoEvent[1] + ", " +
		"\"pid\":" + extractPidFromString(FileIoEvent[2]) + ", "
		"\"tid\":" + FileIoEvent[3] + ", ";
	// (jump the repetition)
	line += "\"cpu\":" + FileIoEvent[6] + ", " +
		ifNotEmpty("ip", FileIoEvent[7]) +
		ifNotEmpty("fo", FileIoEvent[8]) +
		ifNotEmpty("fn", FileIoEvent[9]);
	return line;
}

/*3- FileIOCloseState*/
FileIOCloseState::FileIOCloseState(){}
FileIOCloseState::~FileIOCloseState(){}
std::string FileIOCloseState::returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd){
	/*FileIoClose,  TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU,IrpPtr, FileObject, FileName*/
	/* FileIoClose, 14195158, chrome.exe (12192), 12352, chrome.exe (12192), 12352, 2, 0xffffe00018843568, 0xffffe000142b0070, "C:\Users\Alexandre\AppData\Local\Google\Chrome\User */
	std::string line = "\"ts\":" + FileIoEvent[1] + ", " +
		"\"pid\":" + extractPidFromString(FileIoEvent[2]) + ", "
		"\"tid\":" + FileIoEvent[3] + ", ";
	// (jump the repetition)
	line += "\"cpu\":" + FileIoEvent[6] + ", " +
		ifNotEmpty("ip", FileIoEvent[7]) +
		ifNotEmpty("fo", FileIoEvent[8]) +
		ifNotEmpty("fn", FileIoEvent[9]);
	return line;
}

/*4- FileIODeleteState*/
FileIODeleteState::FileIODeleteState(){}
FileIODeleteState::~FileIODeleteState(){}
std::string FileIODeleteState::returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd){
	/*FileIoDelete, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
	std::string line = "\"ts\":" + FileIoEvent[1] + ", " +
		"\"pid\":" + extractPidFromString(FileIoEvent[2]) + ", "
		"\"tid\":" + FileIoEvent[3] + ", ";
	// (jump the repetition)
	line += "\"cpu\":" + FileIoEvent[6] + ", " +
		ifNotEmpty("ip", FileIoEvent[7]) +
		ifNotEmpty("fo", FileIoEvent[8]) +
		ifNotEmpty("ei", FileIoEvent[9]) +
		ifNotEmpty("ic", FileIoEvent[10]) +
		ifNotEmpty("fn", FileIoEvent[11]);
	return line;
}

/*5- FileIODirEnumState*/
FileIODirEnumState::FileIODirEnumState(){}
FileIODirEnumState::~FileIODirEnumState(){}
std::string FileIODirEnumState::returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd){
	/*FileIoDirEnum, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, FileIndex, Size, InfoClass, FileName, FileName*/
	return "";
}

/*6- FileIODirNotifyState*/
FileIODirNotifyState::FileIODirNotifyState(){}
FileIODirNotifyState::~FileIODirNotifyState(){}
std::string FileIODirNotifyState::returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd){
	/*FileIoDirNotify, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, FileIndex, Size, InfoClass, FileName, FileName*/
	std::string line = "\"ts\":" + FileIoEvent[1] + ", " +
		"\"pid\":" + extractPidFromString(FileIoEvent[2]) + ", "
		"\"tid\":" + FileIoEvent[3] + ", ";
	// (jump the repetition)
	line += "\"cpu\":" + FileIoEvent[6] + ", " +
		ifNotEmpty("ip", FileIoEvent[7]) +
		ifNotEmpty("fo", FileIoEvent[8]) +
		ifNotEmpty("fi", FileIoEvent[9]) +
		ifNotEmpty("sz", FileIoEvent[10]) +
		ifNotEmpty("ic", FileIoEvent[11]) +
		ifNotEmpty("fn", FileIoEvent[12]); //avoid the repetition by not repeating the FileName
	return line;
}

/*7- FileIOFlushState*/
FileIOFlushState::FileIOFlushState(){}
FileIOFlushState::~FileIOFlushState(){}
std::string FileIOFlushState::returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd){
	/*FileIoFlush, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, FileName*/
	std::string line = "\"ts\":" + FileIoEvent[1] + ", " +
		"\"pid\":" + extractPidFromString(FileIoEvent[2]) + ", "
		"\"tid\":" + FileIoEvent[3] + ", ";
	line += "\"cpu\":" + FileIoEvent[6] + ", " +
		ifNotEmpty("ip", FileIoEvent[7]) +
		ifNotEmpty("fo", FileIoEvent[8]) +
		ifNotEmpty("fn", FileIoEvent[9]);
	return line;
}

/*8- FileIOFSCTLState*/
FileIOFSCTLState::FileIOFSCTLState(){}
FileIOFSCTLState::~FileIOFSCTLState(){}
std::string FileIOFSCTLState::returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd){
	/*FileIoFSCTL, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
	std::string line = "\"ts\":" + FileIoEvent[1] + ", " +
		"\"pid\":" + extractPidFromString(FileIoEvent[2]) + ", "
		"\"tid\":" + FileIoEvent[3] + ", ";

	line += "\"cpu\":" + FileIoEvent[6] + ", " +
		ifNotEmpty("ip", FileIoEvent[7]) +
		ifNotEmpty("fo", FileIoEvent[8]) +
		ifNotEmpty("ei", FileIoEvent[9]) +
		ifNotEmpty("ic", FileIoEvent[10]) +
		ifNotEmpty("fn", FileIoEvent[11]);
	return line;
}

/*9- FileIOOpEndState*/
FileIOOpEndState::FileIOOpEndState(){}
FileIOOpEndState::~FileIOOpEndState(){}
std::string FileIOOpEndState::returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd){
	/*FileIoOpEnd, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ElapsedTime, Status, ExtraInfo, Type, FileName*/
	/*FileIoOpEnd, 1626934, chrome.exe(9336), 6332, chrome.exe(9336), 6332, 0, 0xffffe00014cb3b58, 0xffffe000186e3070, 233, 0x00000000, 0x0000000000004000*/
	
	std::string line = "\"ts\":" + FileIoEvent[1] + ", " +
						"\"pid\":" + extractPidFromString(FileIoEvent[2]) + ", "
						"\"tid\":" + FileIoEvent[3] + ", ";
	
	line += "\"cpu\":" + FileIoEvent[6] + ", " +
		ifNotEmpty("ip", FileIoEvent[7]) +
		ifNotEmpty("fo", FileIoEvent[8]) +
		ifNotEmpty("et", FileIoEvent[9]) +
		ifNotEmpty("st", FileIoEvent[10]) +
		ifNotEmpty("ei", FileIoEvent[11]) +
		ifNotEmpty("ty", FileIoEvent[12]) +
		ifNotEmpty("fn", FileIoEvent[13]);
		return line;	
}

/*10- FileIOQueryInfoState*/
FileIOQueryInfoState::FileIOQueryInfoState(){}
FileIOQueryInfoState::~FileIOQueryInfoState(){}
std::string FileIOQueryInfoState::returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd){
	/*FileIoQueryInfo, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
	/*FileIoQueryInfo, 14195102, chrome.exe (12192), 12352, chrome.exe (12192), 12352, 2, 0xffffe0001cbb8508, 0xffffe000142b0070, 0x0000000000000000, FileBasicInformation, "C:\Users\Alexandre\AppData\Local\Google\Chrome\User Data\Default\databases\Databases.db-journal"*/
	
	std::string line = "\"ts\":" + FileIoEvent[1] + ", " +
		"\"pid\":" + extractPidFromString(FileIoEvent[2]) + ", "
		"\"tid\":" + FileIoEvent[3] + ", ";
	
	line += "\"cpu\":" + FileIoEvent[6] + ", " +
		ifNotEmpty("ip", FileIoEvent[7]) +
		ifNotEmpty("fo", FileIoEvent[8]) +
		ifNotEmpty("ei", FileIoEvent[9]) +
		ifNotEmpty("ic", FileIoEvent[10]) +
		ifNotEmpty("fn", FileIoEvent[11]);
	return line;
}

/*11- FileIOReadState*/
FileIOReadState::FileIOReadState(){}
FileIOReadState::~FileIOReadState(){}
std::string FileIOReadState::returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd){
	/*FileIoRead, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ByteOffset, Size, Flags, ExtraFlags, Priority, FileName, ParsedFlags*/
	/*FileIoRead, 14195127, chrome.exe (12192), 12352, chrome.exe (12192), 12352, 2, 0xffffe00018843568, 0xffffe000142b0070, 0x0000000000, 0x00000001, 0x00060900, 0x00000000, Normal, */
	
	std::string line = "\"ts\":" + FileIoEvent[1] + ", " +
		"\"pid\":" + extractPidFromString(FileIoEvent[2]) + ", "
		"\"tid\":" + FileIoEvent[3] + ", ";
	
	line += "\"cpu\":" + FileIoEvent[6] + ", " +
		ifNotEmpty("ip", FileIoEvent[7]) +
		ifNotEmpty("fo", FileIoEvent[8]) +
		ifNotEmpty("bo", FileIoEvent[9]) +
		ifNotEmpty("sz", FileIoEvent[10]) +
		ifNotEmpty("fs", FileIoEvent[11]) +
		ifNotEmpty("efs", FileIoEvent[12]) + 
		ifNotEmpty("pt", FileIoEvent[13]) + 
		ifNotEmpty("fn", FileIoEvent[14]) + 
		ifNotEmpty("pf", FileIoEvent[15]) ;
	return line;
}

/*12- FileIORenameState*/
FileIORenameState::FileIORenameState(){}
FileIORenameState::~FileIORenameState(){}
std::string FileIORenameState::returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd){
	/*FileIoRename, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
	
	std::string line = "\"ts\":" + FileIoEvent[1] + ", " +
		"\"pid\":" + extractPidFromString(FileIoEvent[2]) + ", "
		"\"tid\":" + FileIoEvent[3] + ", ";
	
	line += "\"cpu\":" + FileIoEvent[6] + ", " +
		ifNotEmpty("ip", FileIoEvent[7]) +
		ifNotEmpty("fo", FileIoEvent[8]) +
		ifNotEmpty("ei", FileIoEvent[9]) +
		ifNotEmpty("ic", FileIoEvent[10]) +
		ifNotEmpty("fn", FileIoEvent[11]);
	return line;
}

/*13- FileIOSetInfoState*/
FileIOSetInfoState::FileIOSetInfoState(){}
FileIOSetInfoState::~FileIOSetInfoState(){}
std::string FileIOSetInfoState::returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd){
	/*FileIoSetInfo, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
	
	std::string line = "\"ts\":" + FileIoEvent[1] + ", " +
		"\"pid\":" + extractPidFromString(FileIoEvent[2]) + ", "
		"\"tid\":" + FileIoEvent[3] + ", ";
	
	line += "\"cpu\":" + FileIoEvent[6] + ", " +
		ifNotEmpty("ip", FileIoEvent[7]) +
		ifNotEmpty("fo", FileIoEvent[8]) +
		ifNotEmpty("ei", FileIoEvent[9]) +
		ifNotEmpty("ic", FileIoEvent[10]) +
		ifNotEmpty("fn", FileIoEvent[11]);
	return line;
}

/*14- FileIOWriteState*/
FileIOWriteState::FileIOWriteState(){}
FileIOWriteState::~FileIOWriteState(){}
std::string FileIOWriteState::returnJson(std::vector<std::string> FileIoEvent, std::vector<std::string> OpEnd){
	/*FileIoWrite, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ByteOffset, Size, Flags, ExtraFlags, Priority, FileName, ParsedFlags*/
	
	std::string line = "\"ts\":" + FileIoEvent[1] + ", " +
		"\"pid\":" + extractPidFromString(FileIoEvent[2]) + ", "
		"\"tid\":" + FileIoEvent[3] + ", ";
	
	line += "\"cpu\":" + FileIoEvent[6] + ", " +
		ifNotEmpty("ip", FileIoEvent[7]) +
		ifNotEmpty("fo", FileIoEvent[8]) +
		ifNotEmpty("bo", FileIoEvent[9]) +
		ifNotEmpty("sz", FileIoEvent[10]) +
		ifNotEmpty("fs", FileIoEvent[11]) +
		ifNotEmpty("efs", FileIoEvent[12]) +
		ifNotEmpty("pt", FileIoEvent[13]) +
		ifNotEmpty("fn", FileIoEvent[14]) +
		ifNotEmpty("pf", FileIoEvent[15]);
	return line;
}

