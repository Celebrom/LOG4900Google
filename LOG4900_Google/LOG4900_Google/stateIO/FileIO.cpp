
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
std::string FileIOCreateState::returnJson(std::vector<std::string> lines){
	/*FileIoCreate,  TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, Options, Attributes, ShareAccess, FileName, ParsedOptions, ParsedAttributes, ParsedShareAccess*/
		std::string line = "\"ts\":" + lines[1] + ", " +
		"\"pid\":" + extractPidFromString(lines[2]) + ", "
		"\"tid\":" + lines[3] + ", ";
	// (jump the repetition)
	line += "\"cpu\":" + lines[6] + ", " +
		ifNotEmpty("ip", lines[7]) +
		ifNotEmpty("fo", lines[8]) +
		ifNotEmpty("op", lines[9]) +
		ifNotEmpty("at", lines[10]) +
		ifNotEmpty("sa", lines[11]) +
		ifNotEmpty("fn", lines[12]) +
		ifNotEmpty("po", lines[13]) +
		ifNotEmpty("pa", lines[14]) +
		ifNotEmpty("ps", lines[15]);
	return line;
}

/*2- FileIOCleanUpState*/
FileIOCleanUpState::FileIOCleanUpState(){}
FileIOCleanUpState::~FileIOCleanUpState(){}
std::string FileIOCleanUpState::returnJson(std::vector<std::string> lines){
	/*FileIoCleanup,TimeStamp, Process Name ( PID),   ThreadID, LoggingProcessName(PID), LoggingThreadID, CPU,IrpPtr, FileObject, FileName*/
	/* FileIoCleanup, 14195148, chrome.exe (12192), 12352, chrome.exe (12192), 12352, 2, 0xffffe00018843568, 0xffffe000142b0070, "C:\Users\Alexandre\AppData\Local\Google\Chrome\User Data\Default\databases\Databases.db-journal"*/
	std::string line = "\"ts\":" + lines[1] + ", " +
		"\"pid\":" + extractPidFromString(lines[2]) + ", "
		"\"tid\":" + lines[3] + ", ";
	// (jump the repetition)
	line += "\"cpu\":" + lines[6] + ", " +
		ifNotEmpty("ip", lines[7]) +
		ifNotEmpty("fo", lines[8]) +
		ifNotEmpty("fn", lines[9]);
	return line;
}

/*3- FileIOCloseState*/
FileIOCloseState::FileIOCloseState(){}
FileIOCloseState::~FileIOCloseState(){}
std::string FileIOCloseState::returnJson(std::vector<std::string> lines){
	/*FileIoClose,  TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU,IrpPtr, FileObject, FileName*/
	/* FileIoClose, 14195158, chrome.exe (12192), 12352, chrome.exe (12192), 12352, 2, 0xffffe00018843568, 0xffffe000142b0070, "C:\Users\Alexandre\AppData\Local\Google\Chrome\User */
	std::string line = "\"ts\":" + lines[1] + ", " +
		"\"pid\":" + extractPidFromString(lines[2]) + ", "
		"\"tid\":" + lines[3] + ", ";
	// (jump the repetition)
	line += "\"cpu\":" + lines[6] + ", " +
		ifNotEmpty("ip", lines[7]) +
		ifNotEmpty("fo", lines[8]) +
		ifNotEmpty("fn", lines[9]);
	return line;
}

/*4- FileIODeleteState*/
FileIODeleteState::FileIODeleteState(){}
FileIODeleteState::~FileIODeleteState(){}
std::string FileIODeleteState::returnJson(std::vector<std::string> lines){
	/*FileIoDelete, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
	std::string line = "\"ts\":" + lines[1] + ", " +
		"\"pid\":" + extractPidFromString(lines[2]) + ", "
		"\"tid\":" + lines[3] + ", ";
	// (jump the repetition)
	line += "\"cpu\":" + lines[6] + ", " +
		ifNotEmpty("ip", lines[7]) +
		ifNotEmpty("fo", lines[8]) +
		ifNotEmpty("ei", lines[9]) +
		ifNotEmpty("ic", lines[10]) +
		ifNotEmpty("fn", lines[11]);
	return line;
}

/*5- FileIODirEnumState*/
FileIODirEnumState::FileIODirEnumState(){}
FileIODirEnumState::~FileIODirEnumState(){}
std::string FileIODirEnumState::returnJson(std::vector<std::string> lines){
	/*FileIoDirEnum, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, FileIndex, Size, InfoClass, FileName, FileName*/
	return "";
}

/*6- FileIODirNotifyState*/
FileIODirNotifyState::FileIODirNotifyState(){}
FileIODirNotifyState::~FileIODirNotifyState(){}
std::string FileIODirNotifyState::returnJson(std::vector<std::string> lines){
	/*FileIoDirNotify, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, FileIndex, Size, InfoClass, FileName, FileName*/
	std::string line = "\"ts\":" + lines[1] + ", " +
		"\"pid\":" + extractPidFromString(lines[2]) + ", "
		"\"tid\":" + lines[3] + ", ";
	// (jump the repetition)
	line += "\"cpu\":" + lines[6] + ", " +
		ifNotEmpty("ip", lines[7]) +
		ifNotEmpty("fo", lines[8]) +
		ifNotEmpty("fi", lines[9]) +
		ifNotEmpty("sz", lines[10]) +
		ifNotEmpty("ic", lines[11]) +
		ifNotEmpty("fn", lines[12]); //avoid the repetition by not repeating the FileName
	return line;
}

/*7- FileIOFlushState*/
FileIOFlushState::FileIOFlushState(){}
FileIOFlushState::~FileIOFlushState(){}
std::string FileIOFlushState::returnJson(std::vector<std::string> lines){
	/*FileIoFlush, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, FileName*/
	std::string line = "\"ts\":" + lines[1] + ", " +
		"\"pid\":" + extractPidFromString(lines[2]) + ", "
		"\"tid\":" + lines[3] + ", ";
	line += "\"cpu\":" + lines[6] + ", " +
		ifNotEmpty("ip", lines[7]) +
		ifNotEmpty("fo", lines[8]) +
		ifNotEmpty("fn", lines[9]);
	return line;
}

/*8- FileIOFSCTLState*/
FileIOFSCTLState::FileIOFSCTLState(){}
FileIOFSCTLState::~FileIOFSCTLState(){}
std::string FileIOFSCTLState::returnJson(std::vector<std::string> lines){
	/*FileIoFSCTL, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
	std::string line = "\"ts\":" + lines[1] + ", " +
		"\"pid\":" + extractPidFromString(lines[2]) + ", "
		"\"tid\":" + lines[3] + ", ";

	line += "\"cpu\":" + lines[6] + ", " +
		ifNotEmpty("ip", lines[7]) +
		ifNotEmpty("fo", lines[8]) +
		ifNotEmpty("ei", lines[9]) +
		ifNotEmpty("ic", lines[10]) +
		ifNotEmpty("fn", lines[11]);
	return line;
}

/*9- FileIOOpEndState*/
FileIOOpEndState::FileIOOpEndState(){}
FileIOOpEndState::~FileIOOpEndState(){}
std::string FileIOOpEndState::returnJson(std::vector<std::string> lines){
	/*FileIoOpEnd, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ElapsedTime, Status, ExtraInfo, Type, FileName*/
	/*FileIoOpEnd, 1626934, chrome.exe(9336), 6332, chrome.exe(9336), 6332, 0, 0xffffe00014cb3b58, 0xffffe000186e3070, 233, 0x00000000, 0x0000000000004000*/
	
	std::string line = "\"ts\":" + lines[1] + ", " +
						"\"pid\":" + extractPidFromString(lines[2]) + ", "
						"\"tid\":" + lines[3] + ", ";
	
	line += "\"cpu\":" + lines[6] + ", " +
		ifNotEmpty("ip", lines[7]) +
		ifNotEmpty("fo", lines[8]) +
		ifNotEmpty("et", lines[9]) +
		ifNotEmpty("st", lines[10]) +
		ifNotEmpty("ei", lines[11]) +
		ifNotEmpty("ty", lines[12]) +
		ifNotEmpty("fn", lines[13]);
		return line;	
}

/*10- FileIOQueryInfoState*/
FileIOQueryInfoState::FileIOQueryInfoState(){}
FileIOQueryInfoState::~FileIOQueryInfoState(){}
std::string FileIOQueryInfoState::returnJson(std::vector<std::string> lines){
	/*FileIoQueryInfo, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
	/*FileIoQueryInfo, 14195102, chrome.exe (12192), 12352, chrome.exe (12192), 12352, 2, 0xffffe0001cbb8508, 0xffffe000142b0070, 0x0000000000000000, FileBasicInformation, "C:\Users\Alexandre\AppData\Local\Google\Chrome\User Data\Default\databases\Databases.db-journal"*/
	
	std::string line = "\"ts\":" + lines[1] + ", " +
		"\"pid\":" + extractPidFromString(lines[2]) + ", "
		"\"tid\":" + lines[3] + ", ";
	
	line += "\"cpu\":" + lines[6] + ", " +
		ifNotEmpty("ip", lines[7]) +
		ifNotEmpty("fo", lines[8]) +
		ifNotEmpty("ei", lines[9]) +
		ifNotEmpty("ic", lines[10]) +
		ifNotEmpty("fn", lines[11]);
	return line;
}

/*11- FileIOReadState*/
FileIOReadState::FileIOReadState(){}
FileIOReadState::~FileIOReadState(){}
std::string FileIOReadState::returnJson(std::vector<std::string> lines){
	/*FileIoRead, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ByteOffset, Size, Flags, ExtraFlags, Priority, FileName, ParsedFlags*/
	/*FileIoRead, 14195127, chrome.exe (12192), 12352, chrome.exe (12192), 12352, 2, 0xffffe00018843568, 0xffffe000142b0070, 0x0000000000, 0x00000001, 0x00060900, 0x00000000, Normal, */
	
	std::string line = "\"ts\":" + lines[1] + ", " +
		"\"pid\":" + extractPidFromString(lines[2]) + ", "
		"\"tid\":" + lines[3] + ", ";
	
	line += "\"cpu\":" + lines[6] + ", " +
		ifNotEmpty("ip", lines[7]) +
		ifNotEmpty("fo", lines[8]) +
		ifNotEmpty("bo", lines[9]) +
		ifNotEmpty("sz", lines[10]) +
		ifNotEmpty("fs", lines[11]) +
		ifNotEmpty("efs", lines[12]) + 
		ifNotEmpty("pt", lines[13]) + 
		ifNotEmpty("fn", lines[14]) + 
		ifNotEmpty("pf", lines[15]) ;
	return line;
}

/*12- FileIORenameState*/
FileIORenameState::FileIORenameState(){}
FileIORenameState::~FileIORenameState(){}
std::string FileIORenameState::returnJson(std::vector<std::string> lines){
	/*FileIoRename, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
	
	std::string line = "\"ts\":" + lines[1] + ", " +
		"\"pid\":" + extractPidFromString(lines[2]) + ", "
		"\"tid\":" + lines[3] + ", ";
	
	line += "\"cpu\":" + lines[6] + ", " +
		ifNotEmpty("ip", lines[7]) +
		ifNotEmpty("fo", lines[8]) +
		ifNotEmpty("ei", lines[9]) +
		ifNotEmpty("ic", lines[10]) +
		ifNotEmpty("fn", lines[11]);
	return line;
}

/*13- FileIOSetInfoState*/
FileIOSetInfoState::FileIOSetInfoState(){}
FileIOSetInfoState::~FileIOSetInfoState(){}
std::string FileIOSetInfoState::returnJson(std::vector<std::string> lines){
	/*FileIoSetInfo, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ExtraInfo, InfoClass, FileName*/
	
	std::string line = "\"ts\":" + lines[1] + ", " +
		"\"pid\":" + extractPidFromString(lines[2]) + ", "
		"\"tid\":" + lines[3] + ", ";
	
	line += "\"cpu\":" + lines[6] + ", " +
		ifNotEmpty("ip", lines[7]) +
		ifNotEmpty("fo", lines[8]) +
		ifNotEmpty("ei", lines[9]) +
		ifNotEmpty("ic", lines[10]) +
		ifNotEmpty("fn", lines[11]);
	return line;
}

/*14- FileIOWriteState*/
FileIOWriteState::FileIOWriteState(){}
FileIOWriteState::~FileIOWriteState(){}
std::string FileIOWriteState::returnJson(std::vector<std::string> lines){
	/*FileIoWrite, TimeStamp, Process Name ( PID), ThreadID, LoggingProcessName ( PID), LoggingThreadID, CPU, IrpPtr, FileObject, ByteOffset, Size, Flags, ExtraFlags, Priority, FileName, ParsedFlags*/
	
	std::string line = "\"ts\":" + lines[1] + ", " +
		"\"pid\":" + extractPidFromString(lines[2]) + ", "
		"\"tid\":" + lines[3] + ", ";
	
	line += "\"cpu\":" + lines[6] + ", " +
		ifNotEmpty("ip", lines[7]) +
		ifNotEmpty("fo", lines[8]) +
		ifNotEmpty("bo", lines[9]) +
		ifNotEmpty("sz", lines[10]) +
		ifNotEmpty("fs", lines[11]) +
		ifNotEmpty("efs", lines[12]) +
		ifNotEmpty("pt", lines[13]) +
		ifNotEmpty("fn", lines[14]) +
		ifNotEmpty("pf", lines[15]);
	return line;
}

