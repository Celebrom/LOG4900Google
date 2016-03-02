#include "CSVBlock.h"

CSVBlock::CSVBlock()
{
	this->timestamp = 0;
}

void CSVBlock::Reset(std::string tid, int timestamp)
{
	this->tid = tid;
	this->timestamp = timestamp;
	lines.clear();
}