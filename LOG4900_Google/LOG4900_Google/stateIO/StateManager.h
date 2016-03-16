#pragma once
#include <string>
#include "AbstractState.h"/
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
	AbstractState* currentState_;
	int	   idState_;
	std::vector<AbstractState *> stateVector;
};

