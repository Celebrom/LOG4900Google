#pragma once
#include "StackLine.h"
#include "../base/history.h"
#include "../etw_reader/stack.h"
#include <string>
#include <vector>

using namespace etw_insights;
class LiveStack
{
public:
	std::vector<std::string> GetFinalLines();
	std::vector<std::string> Update(base::History<Stack>::Element nextStack);

private:
	std::vector<StackLine> lines;
	base::Timestamp lastTimestamp;

	static int idCounter;
};