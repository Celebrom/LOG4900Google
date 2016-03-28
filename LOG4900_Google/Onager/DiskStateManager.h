#pragma once
#include <string>
#include "AbstractState.h"/
class DiskStateManager
{
public:
		DiskStateManager();
		~DiskStateManager();
		int getStateId();
		AbstractState* getCurrentState();
		void changeStateTo(unsigned int state);
		int fromStringToIntDisk(std::string value);

private:
		AbstractState* currentState_;
		int	   idState_;
		std::vector<AbstractState *> stateVector;
};
