#include "StackBlock.h"
#include "HeaderDefinition.h"
int StackBlock::idCounter = 1;

std::vector<StackLine> StackBlock::GetLines()
{
	return lines;
}

std::vector<std::string> StackBlock::Update(CSVBlock currentBlock)
{
	//Vérifier le point de différence entre StackBlock et CSVBlock
	int iStack = 0;
	int iCSV = currentBlock.Size() - 1;
	const std::vector<std::vector<std::string>> csvLines = currentBlock.GetLines();

	int stackSize = lines.size();
	while (iStack < stackSize && iCSV >= 0)
	{
		if (lines[iStack].GetName() != csvLines[iCSV][(int)Stack::ImageFunction])
			break;

		++iStack;
		--iCSV;
	}

	//Sortir les lignes qui ne sont plus dans le nouveau stack
	std::vector<std::string> endedLines; 
	int currentTimestamp = currentBlock.GetTimestamp();
	for (int i = iStack; i < stackSize; ++i)
	{								   
		lines[i].SetEndTimestamp(currentTimestamp);
		endedLines.push_back(lines[i].ToJson());
	}								   
	lines.erase(std::begin(lines)+iStack, std::end(lines));			   
									   
	//Mettre à jour le StackBlock
	for (int i = iCSV; i >= 0; --i)
	{
		std::string name = csvLines[i][(int)Stack::ImageFunction];

		int parentID = 0;
		if (lines.size() > 0)
			parentID = lines.back().GetID();
		lines.push_back(StackLine(idCounter++, name, parentID, currentTimestamp));
	}

	return endedLines;
}