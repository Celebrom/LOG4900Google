using namespace std;
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

vector<string> removeSpaces(vector<string> tokens)
{
		vector<string> updatedTokens;

		for each(string token in tokens)
		{
				token.erase(remove(token.begin(), token.end(), ' '), token.end());
				updatedTokens.push_back(token);
		}
		return updatedTokens;
}

void parseHeader(ifstream& file, unordered_map<string, vector<string>>& header)
{
		int lineNb = 0;
		string event;

		while (file.good())
		{
				getline(file, event);

				vector<string> tokens;
				tokenize(event, tokens, ",");

				tokens = removeSpaces(tokens);

				string eventType = tokens[0];
				tokens.erase(tokens.begin());

				header[eventType] = tokens;

				lineNb++;
		}
}

//vector<string> convertToChromeFormat()
//{
//		return;
//}


int main(int argc, char* argv[])
{

	// Look if there is no arguments
	if (argc <= 1)
		return 0;

	// Opening and reading .csv
	ifstream file(argv[1]);

	unordered_map<string, vector<string>> header;
	parseHeader(file, header);



	for (auto& bob : header)
	{
			cout <<  bob.first << endl;
			for (int i = 0; i < bob.second.size(); i++)
					cout << " " << bob.second[i] << endl;
	}

	system("pause");

	return 1;
}

