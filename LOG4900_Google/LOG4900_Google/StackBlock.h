#pragma once
#include "CSVBlock.h"
#include "StackLine.h"
#include <string>
#include <vector>

class StackBlock
{
public:
	std::vector<StackLine> GetLines();
	std::vector<std::string> Update(CSVBlock currentBlock);

private:
	std::vector<StackLine> lines;

	static int idCounter;
};