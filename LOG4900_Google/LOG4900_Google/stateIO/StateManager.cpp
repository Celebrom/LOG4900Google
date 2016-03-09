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
	currentState_ = new AbstractState();
	idState_ = 0;
}


StateManager::~StateManager()
{
	if (currentState_ != 0){
		delete currentState_;
		currentState_ = 0;
	}
}

int StateManager::getStateId(){
	return idState_;
}

AbstractState* StateManager::getCurrentState(){
	if (currentState_ == 0)
	{
		currentState_ = new AbstractState();
		idState_ = 0;
	}
	return currentState_;
}

void StateManager::changeStateTo(unsigned int state){
	idState_ = state;
	if (currentState_ == 0)
	{
		currentState_ = new AbstractState();
		idState_ = NEUTRAL;
	}
	switch (state)
	{
	case FILEIOCREATE:
		currentState_ = new FileIOCreateState();
		break;
	case FILEIOCLEANUP:
		currentState_ = new FileIOCleanUpState();
		break;
	case FILEIOCLOSE:
		currentState_ = new FileIOCloseState();
		break;
	case FILEIOFLUSH:
		currentState_ = new FileIOFlushState();
		break;
	case FILEIOREAD:
		currentState_ = new FileIOReadState();
		break;
	case FILEIOWRITE:
		currentState_ = new FileIOWriteState();
		break;
	case FILEIOSETINFO:
		currentState_ = new FileIOSetInfoState();
		break;
	case FILEIOQUERYINFO:
		currentState_ = new FileIOQueryInfoState();
		break;
	case FILEIOFSCTL:
		currentState_ = new FileIOFSCTLState();
		break;
	case FILEIODELETE:
		currentState_ = new FileIODeleteState();
		break;
	case FILEIORENAME:
		currentState_ = new FileIORenameState();
		break;
	case FILEIODIRENUM:
		currentState_ = new FileIODirEnumState();
		break;
	case FILEIODIRNOTIFY:
		currentState_ = new FileIODirNotifyState();
		break;
	case FILEIOOPEND:
		currentState_ = new FileIOOpEndState();
		break;
	default:
		break;
	}
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
