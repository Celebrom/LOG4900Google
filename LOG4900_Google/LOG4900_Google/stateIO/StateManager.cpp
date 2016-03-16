#include "StateManager.h"
#include "AbstractState.h"
#include "FileIOCreateState.h"
#include "FileIOCleanUpState.h"
#include "FileIOCloseState.h"
#include "FileIOFlushState.h"
#include "FileIOReadState.h"
#include "FileIOWriteState.h"
#include "FileIOSetInfoState.h"
#include "FileIOQueryInfoState.h"
#include "FileIOFSCTLState.h"
#include "FileIODeleteState.h"
#include "FIleIORenameState.h"
#include "FileIODirNotifyState.h"
#include "FileIOOpEndState.h"
#include "FileIODirEnumState.h"
#include "FileIODirNotifyState.h"


enum typeIO{
	NEUTRAL,
	FILEIOCREATE, 
	FILEIOCLEANUP,
	FILEIOCLOSE,
	FILEIOFLUSH, 
	FILEIOREAD, 
	FILEIOWRITE,
	FILEIOSETINFO, 
	FILEIOQUERYINFO, 
	FILEIOFSCTL,
	FILEIODELETE,
	FILEIORENAME,
	FILEIODIRENUM,
	FILEIODIRNOTIFY,
	FILEIOOPEND
};


StateManager::StateManager()
{
	
	idState_ = NEUTRAL;

	stateVector.push_back(new AbstractState());
	stateVector.push_back(new FileIOCreateState());
	stateVector.push_back(new FileIOCleanUpState());
	stateVector.push_back(new FileIOCloseState());
	stateVector.push_back(new FileIOFlushState());
	stateVector.push_back(new FileIOReadState());
	stateVector.push_back(new FileIOWriteState());
	stateVector.push_back(new FileIOSetInfoState());
	stateVector.push_back(new FileIOQueryInfoState());
	stateVector.push_back(new FileIOFSCTLState());
	stateVector.push_back(new FileIODeleteState());
	stateVector.push_back(new FileIORenameState());
	stateVector.push_back(new FileIODirEnumState());
	stateVector.push_back(new FileIODirNotifyState());
	stateVector.push_back(new FileIOOpEndState());

	currentState_ = stateVector[0];
}


StateManager::~StateManager()
{
	for (auto state : stateVector)
	{
		delete state;
	}
}

int StateManager::getStateId(){
	return idState_;
}

AbstractState* StateManager::getCurrentState(){
	if (currentState_ == 0)
	{
		currentState_ = new AbstractState();
		idState_ = NEUTRAL;
	}
	return currentState_;
}

void StateManager::changeStateTo(unsigned int state){
	idState_ = state;	
	currentState_ = stateVector[state];
}


int StateManager::fromStringToIntIO(std::string value){
	if (value == "FileIoCreate")
		return FILEIOCREATE;
	else if (value == "FileIoCleanup")
		return FILEIOCLEANUP;
	else if (value == "FileIoClose")
		return FILEIOCLOSE;
	else if (value == "FileIoFlush")
		return FILEIOFLUSH;
	else if (value == "FileIoRead")
		return FILEIOREAD;
	else if (value == "FileIoWrite")
		return FILEIOWRITE;
	else if (value == "FileIoSetInfo")
		return FILEIOSETINFO;
	else if (value == "FileIoQueryInfo")
		return FILEIOQUERYINFO;
	else if (value == "FileIoFSCTL")
		return FILEIOFSCTL;
	else if (value == "FileIoDelete")
		return FILEIODELETE;
	else if (value == "FileIoRename")
		return FILEIORENAME;
	else if (value == "FileIoDirEnum")
		return FILEIODIRENUM;
	else if (value == "FileIoDirNotify,")
		return FILEIODIRNOTIFY;
	else if (value == "FileIoOpEnd")
		return FILEIOOPEND;
	else
		return -1;
}
