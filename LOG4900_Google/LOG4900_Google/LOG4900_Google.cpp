#include "etw_reader/etw_reader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>




//http://stackoverflow.com/questions/236129/split-a-string-in-c
template < class ContainerT >
void tokenize(const std::string& str, ContainerT& tokens,
		const std::string& delimiters = " ", bool trimEmpty = false)
{
		std::string::size_type pos, lastPos = 0;

		using value_type = typename ContainerT::value_type;
		using size_type = typename ContainerT::size_type;

		while (true)
		{
				pos = str.find_first_of(delimiters, lastPos);
				if (pos == std::string::npos)
				{
						pos = str.length();

						if (pos != lastPos || !trimEmpty)
								tokens.push_back(value_type(str.data() + lastPos,
								(size_type)pos - lastPos));

						break;
				}
				else
				{
						if (pos != lastPos || !trimEmpty)
								tokens.push_back(value_type(str.data() + lastPos,
								(size_type)pos - lastPos));
				}

				lastPos = pos + 1;
		}
}

std::vector<std::string> removeSpaces(std::vector<std::string> tokens)
{
		std::vector<std::string> updatedTokens;

		for each(std::string token in tokens)
		{
				token.erase(remove(token.begin(), token.end(), ' '), token.end());
				updatedTokens.push_back(token);
		}
		return updatedTokens;
}

void parseHeader(std::ifstream& file, std::unordered_map<std::string, std::vector<std::string>>& header)
{
		int lineNb = 0;
		std::string event;

		while (file.good())
		{
				getline(file, event);

				std::vector<std::string> tokens;
				tokenize(event, tokens, ",");

				tokens = removeSpaces(tokens);

				std::string eventType = tokens[0];
				tokens.erase(tokens.begin());

				header[eventType] = tokens;

				lineNb++;
		}
}




int main(int argc, char* argv[])
{
	etw_insights::ETWReader etwReader;

	// Look if there is no arguments
	if (argc <= 1)
		return 0;
	/// TODO si un argv est un .etl est caller, appeler sa methode dans etw_reader pour convertir en .csv
	else if (std::string(argv[1]).find(".etl") != std::string::npos)
	{
			//do stuff if is a .etl
			std::string pathTemp = std::string(argv[1]);
			const std::wstring path(pathTemp.begin(), pathTemp.end());
			etwReader.Open(path);
	}
	else if (std::string(argv[1]).find(".csv") != std::string::npos)
	{
			//do stuff if is a .csv
	}
	else
			return 0;

	/// TODO si un argv est un .csv ou .txt, appeler ma methode

	// Opening and reading .csv
	//std::ifstream file(argv[1]);

	//std::unordered_map<std::string, std::vector<std::string>> header;
	//parseHeader(file, header);

	std::cout << "allo";
	system("pause");

	return 1;
}

