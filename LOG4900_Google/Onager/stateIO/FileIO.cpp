/*
Copyright 2015 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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
#include <algorithm>
#include <etw_reader/etw_reader.h>


/*1- FileIOCreateState*/
FileIOCreateState::FileIOCreateState(){}
FileIOCreateState::~FileIOCreateState(){}
std::string FileIOCreateState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
		std::string fileName = FileIoEvent.GetFieldAsString("FileName");
		Utils::formatFileName(fileName); // TODO: Might not get a good reference

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[12];
		return "{\"name\":\"[" + FileIoEvent.GetFieldAsString("FileIoCreate") + "]" + fileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*2- FileIOCleanUpState*/
FileIOCleanUpState::FileIOCleanUpState(){}
FileIOCleanUpState::~FileIOCleanUpState(){}
std::string FileIOCleanUpState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
	std::string fileName = FileIoEvent.GetFieldAsString("FileName");
	Utils::formatFileName(fileName); // TODO: Might not get a good reference

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[9];
	return "{\"name\":\"[" + FileIoEvent.GetFieldAsString("FileIoCleanup") + "]" + fileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*3- FileIOCloseState*/
FileIOCloseState::FileIOCloseState(){}
FileIOCloseState::~FileIOCloseState(){}
std::string FileIOCloseState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
	std::string fileName = FileIoEvent.GetFieldAsString("FileName");
	Utils::formatFileName(fileName); // TODO: Might not get a good reference

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[9];
	return "{\"name\":\"[" + FileIoEvent.GetFieldAsString("FileIoClose") + "]" + fileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*4- FileIODeleteState*/
FileIODeleteState::FileIODeleteState(){}
FileIODeleteState::~FileIODeleteState(){}
std::string FileIODeleteState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
	std::string fileName = FileIoEvent.GetFieldAsString("FileName");
	Utils::formatFileName(fileName); // TODO: Might not get a good reference

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[11];
	return "{\"name\":\"[" + FileIoEvent.GetFieldAsString("FileIoDelete") + "]" + fileName + "\"," +
			commonJson(FileIoEvent, OpEnd) + "}}";
}

/*5- FileIODirEnumState*/
FileIODirEnumState::FileIODirEnumState(){}
FileIODirEnumState::~FileIODirEnumState(){}
std::string FileIODirEnumState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
	std::string fileName = FileIoEvent.GetFieldAsString("FileName");
	Utils::formatFileName(fileName); // TODO: Might not get a good reference

	//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[12];
	return "{\"name\":\"[" + FileIoEvent.GetFieldAsString("FileIoDirEnum") + "]" + fileName + "\"," +
			commonJson(FileIoEvent, OpEnd) + 
			ifNotEmpty("Size", FileIoEvent.GetFieldAsString("Size")) + "}}";
}

/*6- FileIODirNotifyState*/
FileIODirNotifyState::FileIODirNotifyState(){}
FileIODirNotifyState::~FileIODirNotifyState(){}
std::string FileIODirNotifyState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
	std::string fileName = FileIoEvent.GetFieldAsString("FileName");
	Utils::formatFileName(fileName); // TODO: Might not get a good reference

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[12];
	return "{\"name\":\"[" + FileIoEvent.GetFieldAsString("FileIoDirNotify") + "]" + fileName + "\"," +
				commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("Size", FileIoEvent.GetFieldAsString("Size")) + "}}";
}

/*7- FileIOFlushState*/
FileIOFlushState::FileIOFlushState(){}
FileIOFlushState::~FileIOFlushState(){}
std::string FileIOFlushState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
	std::string fileName = FileIoEvent.GetFieldAsString("FileName");
	Utils::formatFileName(fileName); // TODO: Might not get a good reference
	
		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[9];
	return "{\"name\":\"[" + FileIoEvent.GetFieldAsString("FileIoFlush") + "]" + fileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*8- FileIOFSCTLState*/
FileIOFSCTLState::FileIOFSCTLState(){}
FileIOFSCTLState::~FileIOFSCTLState(){}
std::string FileIOFSCTLState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
	std::string fileName = FileIoEvent.GetFieldAsString("FileName");
	Utils::formatFileName(fileName); // TODO: Might not get a good reference

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[11];
	return "{\"name\":\"[" + FileIoEvent.GetFieldAsString("FileIoFSCTL") + "]" + fileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*9- FileIOOpEndState*/
FileIOOpEndState::FileIOOpEndState(){}
FileIOOpEndState::~FileIOOpEndState(){}
std::string FileIOOpEndState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
	std::string fileName = FileIoEvent.GetFieldAsString("FileName");
	Utils::formatFileName(fileName); // TODO: Might not get a good reference

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[13];
	return "{\"name\":\"[" + FileIoEvent.GetFieldAsString("FileIoOpEnd") + "]" + fileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*10- FileIOQueryInfoState*/
FileIOQueryInfoState::FileIOQueryInfoState(){}
FileIOQueryInfoState::~FileIOQueryInfoState(){}
std::string FileIOQueryInfoState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
	std::string fileName = FileIoEvent.GetFieldAsString("FileName");
	Utils::formatFileName(fileName); // TODO: Might not get a good reference


		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[11];
	return "{\"name\":\"[" + FileIoEvent.GetFieldAsString("FileIoQueryInfo") + "]" + fileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*11- FileIOReadState*/
FileIOReadState::FileIOReadState(){}
FileIOReadState::~FileIOReadState(){}
std::string FileIOReadState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
	std::string fileName = FileIoEvent.GetFieldAsString("FileName");
	Utils::formatFileName(fileName); // TODO: Might not get a good reference

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[14];
	return "{\"name\":\"[" + FileIoEvent.GetFieldAsString("FileIoRead") + "]" + fileName + "\"," +
				commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("Priority", FileIoEvent.GetFieldAsString("Priority")) + "," +
				ifNotEmpty("Size", FileIoEvent.GetFieldAsString("Size")) + "}}";
}

/*12- FileIORenameState*/
FileIORenameState::FileIORenameState(){}
FileIORenameState::~FileIORenameState(){}
std::string FileIORenameState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
	std::string fileName = FileIoEvent.GetFieldAsString("FileName");
	Utils::formatFileName(fileName); // TODO: Might not get a good reference

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[11];
	return "{\"name\":\"[" + FileIoEvent.GetFieldAsString("FileIoRename") + "]" + fileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*13- FileIOSetInfoState*/
FileIOSetInfoState::FileIOSetInfoState(){}
FileIOSetInfoState::~FileIOSetInfoState(){}
std::string FileIOSetInfoState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
	std::string fileName = FileIoEvent.GetFieldAsString("FileName");
	Utils::formatFileName(fileName); // TODO: Might not get a good reference

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[11];
	return "{\"name\":\"[" + FileIoEvent.GetFieldAsString("FileIoSetInfo") + "]" + fileName + "\"," +
				commonJson(FileIoEvent, OpEnd) + "}}";
}

/*14- FileIOWriteState*/
FileIOWriteState::FileIOWriteState(){}
FileIOWriteState::~FileIOWriteState(){}
std::string FileIOWriteState::returnJson(const etw_insights::ETWReader::Line& FileIoEvent, const etw_insights::ETWReader::Line& OpEnd){
	std::string fileName = FileIoEvent.GetFieldAsString("FileName");
	Utils::formatFileName(fileName); // TODO: Might not get a good reference

		//FileIoType = FileIoEvent[0]; //FileName = FileIoEvent[14];
	return "{\"name\":\"[" + FileIoEvent.GetFieldAsString("FileIoWrite") + "]" + fileName + "\"," +
				commonJson(FileIoEvent, OpEnd) +
				ifNotEmpty("Priority", FileIoEvent.GetFieldAsString("Priority")) + "," +
				ifNotEmpty("Size", FileIoEvent.GetFieldAsString("Size")) + "}}";
}

