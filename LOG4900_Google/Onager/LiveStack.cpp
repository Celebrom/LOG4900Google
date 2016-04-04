#include "LiveStack.h"
#include "HeaderDefinition.h"
int LiveStack::idCounter = 1;

std::vector<std::string> LiveStack::GetFinalLines()
{
	return std::vector<std::string>();
}

/*
  Compares a stack to another and stores the completed functions.
  A function is completed when it appears in a stack and doesn't appears in the next stack
  nextStack: as its name defines it
  return: returns a vector of completed functions
*/
std::vector<std::string> LiveStack::Update(base::History<Stack>::Element nextStack)
{
	lastTimestamp = nextStack.start_ts;
	Stack stack = nextStack.value;

	int iLive  = 0;
	int iStack = stack.size() - 1;
	int liveSize = lines.size();
	while (iLive < liveSize && iStack >= 0)
	{
		if (lines[iLive].GetName() != stack[iStack])
			break;

		++iLive; --iStack;
	}

	std::vector<std::string> completedFunctions;
	for (int i = iLive; i < liveSize; ++i)
	{
		lines[i].SetEndTimestamp(nextStack.start_ts);
		completedFunctions.push_back(lines[i].ToJson());
	}
	lines.erase(std::begin(lines) + iLive, std::end(lines));

	for (int i = iStack; i >= 0; --i)
	{
		int parentID = 0;
		if (lines.size() > 0)
			parentID = lines.back().GetID();
		lines.push_back(StackLine(idCounter++, stack[i], parentID, nextStack.start_ts));
	}

	return completedFunctions;
}