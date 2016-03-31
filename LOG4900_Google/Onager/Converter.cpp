/*
Copyright 2015 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Converter.h"

std::string Converter::IOLineToJSON(std::vector<std::string>* FileIoEvent, const std::vector<std::string>& OpEnd)
{
	stateIO->changeStateTo(stateIO->fromStringToIntIO((*FileIoEvent)[0]));
	return stateIO->getCurrentState()->returnJson(FileIoEvent, OpEnd);
}

std::string Converter::DiskLineToJSON(std::vector<std::string>& diskEndEvent)
{
	std::string jsonLine = "";

	if (diskEndEvent[0] == "DiskRead" || diskEndEvent[0] == "DiskWrite")
	{
		Utils::formatFileName(&diskEndEvent[15]);

		jsonLine = "{\"name\":\"[" + diskEndEvent[0] + "]" + diskEndEvent[15] + "\"," +
				"\"cat\":\"Disk\",\"ph\":\"X\",\"ts\":" + diskEndEvent[1] + "," +
				"\"dur\":" + diskEndEvent[8] + "," +
				"\"pid\":" + Utils::extractPidFromString(diskEndEvent[2]) + "," +
				"\"tid\":" + diskEndEvent[3] + "," +
				"\"args\":{";

		if (diskEndEvent[12] != "")
			jsonLine += "\"I/O Pri\":\"" + diskEndEvent[12] + "\",";

		if (diskEndEvent[7] != "")
			jsonLine += "\"IOSize\":\"" + diskEndEvent[7] + "\"";

		if (jsonLine[jsonLine.size() - 1] == ',')
			jsonLine = jsonLine.substr(0, jsonLine.size() - 1);

		return jsonLine + "}}";
	}
	else if (diskEndEvent[0] == "DiskFlush")
	{
		jsonLine = "{\"name\":\"[" + diskEndEvent[0] + "]\"," +
				"\"cat\":\"Disk\",\"ph\":\"X\",\"ts\":" + diskEndEvent[1] + "," +
				"\"dur\":" + diskEndEvent[6] + "," +
				"\"pid\":" + Utils::extractPidFromString(diskEndEvent[2]) + "," +
				"\"tid\":" + diskEndEvent[3] + "," +
				"\"args\":{";

		if (diskEndEvent[10] != "")
			jsonLine += "\"I/O Pri\":\"" + diskEndEvent[10] + "\"";

		return jsonLine + "}}";
	}

	return jsonLine;
}

std::string Converter::EventToJSON(std::vector<std::string>& line)
{
	//dirty code
	std::string name = "";
	std::string cat = "";
	std::string phase = "";
	std::string pid = "";
	std::string tid = "";
	std::string ts = "";
	std::string args = "\"args\":{";
	std::string dur = "";
	std::string event = "";
	std::string outputText = "";
	std::string eventType = "";
	std::vector<std::string> eventInfo = std::vector < std::string >();

	for (unsigned int i = 0; i < line.size(); i++)
	{
		switch (i)
		{
		case 1:
				ts += "\"ts\":" + line[i] + ",";
				break;
		case 2:
				pid += "\"pid\":" + Utils::extractPidFromString(line[i]) + ",";
				break;
		case 3:
				tid += "\"tid\":" + line[i] + ",";
				break;
		case 9:
				line[i].erase(std::remove(line[i].begin(), line[i].end(), '"'), line[i].end());
				if (line[i][0] == ' ')
						line[i].erase(std::remove(line[i].begin(), line[i].begin() + 1, ' '), line[i].begin() + 1);
				name += "\"name\":\"" + line[i] + "\",";
				break;
		case 10:
				phase += "\"ph\":\"" + getPhase(line[i]) + "\",";
				break;
		case 11:
		case 13:
		case 15:
				line[i].erase(std::remove(line[i].begin(), line[i].end(), '"'), line[i].end());
				if (line[1] == "3672358")
						int banane = 0;
				if (line[i][0] == ' ')
						line[i].erase(std::remove(line[i].begin(), line[i].begin() + 1, ' '), line[i].begin() + 1);
				if (line[i] != "" && line[i + 1] != "\"\"" && line[i + 1] != "\"\"\"\"")
						args += "\"" + line[i] + "\":";
				if (line[i] != "" && args != "\"args\":{" && line[i + 1] == "\"\"")
				{
						args.erase(args.end() - 1, args.end());
						//args += "," + line[i] + "\"";
				}
				break;
		case 12:
		case 14:
		case 16:
				line[i].erase(std::remove(line[i].begin(), line[i].end(), '"'), line[i].end());
				if (line[i][0] == ' ')
						line[i].erase(std::remove(line[i].begin(), line[i].begin() + 1, ' '), line[i].begin() + 1);
				if (line[i] != "" && line[i + 1] != "\"\"")
				{
						if (line[11] == "url")
								int l = 0;
						if (i != 16)
								args += "\"" + line[i] + "\",";
						else
								args += "\"" + line[i] + "\"";
				}
				if (line[i] != "" && line[i + 1] == "\"\"")
						args += "\"" + line[i] + "\"";
				break;
		case 17:
				dur += ",\"dur\": " + line[line.size() - 1];
				break;
		}
	}
	args += "}";
	return "{" + pid + tid + ts + phase + cat + name + args + dur + "}";
}

std::string Converter::CSwitchToJson(std::vector<std::string>& CSwitchEvent, std::string type, unsigned int id)
{
	std::string JsonLine = "";

	if (type == "New Process")
	{
		JsonLine = "{\"name\":\"On_CPU\",\"pid\":" + Utils::extractPidFromString(CSwitchEvent[2]) + "," +
				"\"tid\":" + CSwitchEvent[3] + "," +
				"\"ts\":" + CSwitchEvent[1] + "," +
				"\"cat\":\"CSwitch\"," +
				"\"id\":\"0x" + std::to_string(id) + "\","
				"\"ph\":\"b\",\"args\":{}}";
	}

	else if (type == "Old Process")
	{
		JsonLine = "{\"name\":\"On_CPU\",\"pid\":" + Utils::extractPidFromString(CSwitchEvent[8]) + "," +
				"\"tid\":" + CSwitchEvent[9] + "," +
				"\"ts\":" + CSwitchEvent[1] + "," +
				"\"cat\":\"CSwitch\"," +
				"\"id\":\"0x" + std::to_string(id) + "\","
				"\"ph\":\"e\",\"args\":{}}";
	}

	return JsonLine;
}

std::string Converter::getPhase(std::string &word)
{
		//Event type
		// Duration: B(begin) E(end)
		// Complete: X
		// Instant: i, I (dreprecated)
		// Counter: C
		// Async: b(nestable start), n (nestable instant), e(nestable end)
		//		  Deprecated
		//        S(start), T(step into), p(step past), F(end)
		// Flow: s(start), t(step), f(end)
		// Sample: P
		// Object: N(created), O(snapshot), D(destroyed)
		// Metadata: M
		// Memory Dump: V(global), V(process)
		// Mark: R
		// Clock Sync: c
		// Context: (,)
		if (word == "\"Complete\"")
				return "X";
		else if (word == "\"Begin\"")
				return "B";
		else if (word == "\"End\"")
				return "E";
		else if (word == "\"Instant\"")
				return "i";
		else if (word == "\"Counter\"")
				return "C";
		else if (word == "\"Async End\"")
				//return "e"; //new version
				return "F";  //deprecated version
		else if (word == "\"Async Begin\"")
				//return "b"; //new version
				return "S";	  //deprecated version
		else if (word == "\"Async Step Into\"")
				return "T";
		else if (word == "\"Sample\"")
				return "P";
		else if (word == "\"Metadata\"")
				return "M";
		else if (word == "\"Mark\"")
				return "R";
		else if (word == "")
				return "";
		else
				return "error";
}

