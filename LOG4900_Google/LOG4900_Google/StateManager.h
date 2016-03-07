#pragma once
#ifndef __STATE_MANAGER_H_
#define _STATE_MANAGER_H_

#include <string>
#include "AbstractState.h"
class StateManager
{
public:
	StateManager();
	~StateManager();
	int getStateId();
	AbstractState* getCurrentState();
	void changeStateTo(unsigned int state);
	int fromStringToIntIO(std::string value);

private:
	static AbstractState* currentState_;
	int	   idState_;
};


#endif // __STATE_MANAGER_H_
