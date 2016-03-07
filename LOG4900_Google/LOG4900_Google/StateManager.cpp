#include "StateManager.h"


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
	FileIoFSCTL,
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
		//currentState_ = new FileIOCreateState();
		break;
	case FILEIOCLEANUP:
		//currentState_ = new FileIOCleanUp();
		break;
	case FILEIOCLOSE:
		//currentState_ = new FileIOClose();
		break;
	case FILEIOFLUSH:
		//currentState_ = new FileIOFlush();
		break;
	case FILEIOREAD:
		//currentState_ = new FileIORead();
		break;
	case FILEIOWRITE:
		//currentState_ = new FileIOWrite();
		break;
	case FILEIOSETINFO:
		//currentState_ = new FileIOSetInfo();
		break;
	case FILEIOQUERYINFO:
		//currentState_ = new FileIOQueryInfo();
		break;
	case FileIoFSCTL:
		//currentState_ = new FileIOFSCTL();
		break;
	case FILEIODELETE:
		//currentState_ = new FileIODelete();
		break;
	case FILEIORENAME:
		//currentState_ = new FileIORename();
		break;
	case FILEIODIRENUM:
		//currentState_ = new FileIODirEnum();
		break;
	case FILEIODIRNOTIFY:
		//currentState_ = new FileIODirNotify();
		break;
	case FILEIOOPEND:
		//currentState_ = new FileIOOpEnd();
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
		return FileIoFSCTL;
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
