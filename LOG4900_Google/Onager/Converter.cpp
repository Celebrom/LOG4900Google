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
#include "Enum.h"
#include "etw_reader/etw_reader.h"

std::string Converter::IOLineToJSON(const etw_insights::ETWReader::Line& fileIoEvent, const etw_insights::ETWReader::Line& opEndEvent)
{
		stateIO->changeStateTo(stateIO->fromStringToIntIO(fileIoEvent.type()));
		return stateIO->getCurrentState()->returnJson(fileIoEvent, opEndEvent);
}

std::string Converter::DiskLineToJSON(const etw_insights::ETWReader::Line& diskEndEvent)
{
		std::string jsonLine = "";

		if (diskEndEvent.type() == "DiskRead" || diskEndEvent.type() == "DiskWrite")
		{
			std::string fileName = diskEndEvent.GetFieldAsString("FileName");
			Utils::formatFileName(fileName); // TODO: Might not get a good reference

			jsonLine = "{\"name\":\"[" + diskEndEvent.type() + "]" + fileName + "\"," +
					"\"cat\":\"Disk\",\"ph\":\"X\",\"ts\":" + diskEndEvent.GetFieldAsString("TimeStamp") + "," + // TODO: TimeStamp as ulong instead of string
					"\"dur\":" + diskEndEvent.GetFieldAsString("ElapsedTime") + "," +
					"\"pid\":" + Utils::extractPidFromString(diskEndEvent.GetFieldAsString("Process Name ( PID)")) + "," +
					"\"tid\":" + diskEndEvent.GetFieldAsString("ThreadID") + "," +
					"\"args\":{";

			if (diskEndEvent.GetFieldAsString("I/O Pri") != "") // TODO: Might get weird with the slash
				jsonLine += "\"I/O Pri\":\"" + diskEndEvent.GetFieldAsString("I/O Pri") + "\",";

			if (diskEndEvent.GetFieldAsString("IOSize") != "")
				jsonLine += "\"IOSize\":\"" + diskEndEvent.GetFieldAsString("IOSize") + "\"";

			if (jsonLine[jsonLine.size() - 1] == ',')
				jsonLine = jsonLine.substr(0, jsonLine.size() - 1);

			return jsonLine + "}}";
		}
		else if (diskEndEvent.type() == "DiskFlush")
		{
				jsonLine = "{\"name\":\"[" + diskEndEvent.type() + "]\"," +
						"\"cat\":\"Disk\",\"ph\":\"X\",\"ts\":" + diskEndEvent.GetFieldAsString("TimeStamp") + "," +
						"\"dur\":" + diskEndEvent.GetFieldAsString("ElapsedTime") + "," +
						"\"pid\":" + Utils::extractPidFromString(diskEndEvent.GetFieldAsString("Process Name ( PID)")) + "," +
						"\"tid\":" + diskEndEvent.GetFieldAsString("ThreadID") + "," +
						"\"args\":{";

				if (diskEndEvent.GetFieldAsString("I/O Pri") != "")
					jsonLine += "\"I/O Pri\":\"" + diskEndEvent.GetFieldAsString("I/O Pri") + "\"";

				return jsonLine + "}}";
		}

		return jsonLine;
}

std::string Converter::EventToJSON(const etw_insights::ETWReader::Line& event, const std::string duration)
{
		std::string pid = "\"pid\":" + Utils::extractPidFromString(event.GetFieldAsString("Process Name ( PID)")) + ",";
		std::string tid = "\"tid\":" + event.GetFieldAsString("ThreadID") + ",";
		std::string ts  = "\"ts\":"  + event.GetFieldAsString("TimeStamp") + ",";

		std::string dirtyName = event.GetFieldAsString("Name");
		clean(&dirtyName);
		std::string name = "\"name\":\"" + dirtyName + "\",";

		std::string phase = "\"ph\":\"" + getPhase(event.GetFieldAsString("Phase")) + "\",";
		std::string cat = "";

		std::string args = "\"args\":{";
		std::string argName1  = event.GetFieldAsString("Arg Name 1");
		std::string argValue1 = event.GetFieldAsString("Arg Value 1");
		std::string argName2  = event.GetFieldAsString("Arg Name 2");
		std::string argValue2 = event.GetFieldAsString("Arg Value 2");
		std::string argName3  = event.GetFieldAsString("Arg Name 3");
		std::string argValue3 = event.GetFieldAsString("Arg Value 3");
		clean(&argName1);
		clean(&argValue1);
		clean(&argName2);
		clean(&argValue2);
		clean(&argName3);
		clean(&argValue3);

		if (argName1 != "" && argValue1 != "\"\"" && argValue1 != "\"\"\"\"")
			args += "\"" + argName1 + "\":";
		if (argName1 != "" && args != "\"args\":{" && argValue1 == "\"\"")
			args.erase(args.end() - 1, args.end());
		if (argValue1 != "")
			args += "\"" + argValue1 + "\"";
		if (argName2 != "")
			args += ",";
		
		if (argName2 != "" && argValue2 != "\"\"" && argValue2 != "\"\"\"\"")
			args += "\"" + argName2 + "\":";
		if (argName2 != "" && args != "\"args\":{" && argValue2 == "\"\"")
			args.erase(args.end() - 1, args.end());
		if (argValue2 != "")
			args += "\"" + argValue2 + "\"";
		if (argName3 != "")
			args += ",";
		
		
		if (argName3 != "" && argValue3 != "\"\"" && argValue3 != "\"\"\"\"")
			args += "\"" + argName3 + "\":";
		if (argName3 != "" && args != "\"args\":{" && argValue3 == "\"\"")
			args.erase(args.end() - 1, args.end());
		if (argValue3 != "")
			args += "\"" + argValue3 + "\"";
		args += "}";

		std::string dur = ",\"dur\": " + duration;

		return "{" + pid + tid + ts + phase + cat + name + args + dur + "}";
}

std::string Converter::CSwitchToJson(const etw_insights::ETWReader::Line& cSwitchEvent, std::string type, unsigned int id)
{
		std::string JsonLine = "";

		if (type == "New Process")
		{
			JsonLine = "{\"name\":\"On_CPU(" + cSwitchEvent.GetFieldAsString("New TID") + ")\"" + "," + 
						"\"pid\":" + Utils::extractPidFromString(cSwitchEvent.GetFieldAsString("New Process Name ( PID)")) + "," +
						"\"tid\":" + cSwitchEvent.GetFieldAsString("New TID") + "," +
						"\"ts\":" + cSwitchEvent.GetFieldAsString("TimeStamp") + "," +
						"\"cat\":\"CSwitch\"," +
						"\"id\":\"0x" + std::to_string(id) + "\","
						"\"ph\":\"b\",\"args\":{}}";
		}
		else if (type == "Old Process")
		{
			JsonLine = "{\"name\":\"On_CPU(" + cSwitchEvent.GetFieldAsString("Old TID") + ")\"" + "," +
						"\"pid\":" + Utils::extractPidFromString(cSwitchEvent.GetFieldAsString("Old Process Name ( PID)")) + "," +
						"\"tid\":" + cSwitchEvent.GetFieldAsString("Old TID") + "," +
						"\"ts\":" + cSwitchEvent.GetFieldAsString("TimeStamp") + "," +
						"\"cat\":\"CSwitch\"," +
						"\"id\":\"0x" + std::to_string(id) + "\","
						"\"ph\":\"e\",\"args\":{}}";
		}

		return JsonLine;
}

void Converter::clean(std::string* field)
{
	field->erase(std::remove(field->begin(), field->end(), '"'), field->end());
	if ((*field)[0] == ' ')
		field->erase(std::remove(field->begin(), field->begin() + 1, ' '), field->begin() + 1);
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

