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

#pragma once
#include <cstdio>
#include <locale>
#include <iostream>
#include <unordered_map>
#include "Timer.h"
#include "Parser.h"
#include "LiveStack.h"
#include "JsonWriter.h"
#include "../base/file.h"		
#include "MemoryMapper.h"
#include "../base/command_line.h"
#include "../etw_reader/etw_reader.h"
#include <boost/iostreams/device/mapped_file.hpp>
#include "ConvertCsvToJson.h"

Timer timer;
Parser parser;
bool verbose = false;

void ShowUsage() {
	std::cout
			<< "Usage: .exe --trace <trace_file_path> -v"
			<< std::endl
			<< "-v : Verbose"
			<< std::endl
			<< std::endl;
}

void convertETLToCSV(std::wstring path)
{
	etw_insights::ETWReader etwReader;
	etwReader.Open(path);
}

// TODO: We should only need one of the path by using the same method for the Chrome Event and Flame graph
void convertCSVToJSON(std::wstring etl_path, std::wstring csv_path)
{		
	SystemHistory system_history;
	if (!WriteHistoryFromTrace(etl_path, &system_history)) {
	LOG(ERROR) << "Error while generating history from trace.";
	return;
	}

	if (verbose) timer.showElapsedTime("Temps de fin d'ecriture des Stacks");
}

void convertETLToJSON(std::wstring etl_path)
{
	std::wstring csv_path = etl_path + L".csv";
	if (!base::FilePathExists(csv_path))
			convertETLToCSV(etl_path);

	convertCSVToJSON(etl_path, csv_path);
}

int wmain(int argc, wchar_t* argv[], wchar_t* /*envp */[])
{
	base::CommandLine command_line(argc, argv);

	if (command_line.GetNumSwitches() == 0) 
	{
		ShowUsage();
		return 1;
	}

	std::wstring trace_path = command_line.GetSwitchValue(L"trace");
	if (trace_path.empty())
	{
		std::cout << "Please specify a trace path (--trace)." << std::endl << std::endl;
		ShowUsage();
		return 1;
	}

	verbose = command_line.HasSwitch(L"v");
	
	std::wstring file_type = trace_path.substr(trace_path.length() - 3, 3);
	if (file_type.empty() || file_type != L"etl")
	{
		std::cout << "Please make sure the trace is of ETW type (.etl)" << std::endl << std::endl;
		ShowUsage();
		return 1;
	}

	convertETLToJSON(trace_path);
			
	if (verbose)
		timer.showElapsedTime("\n\n\nDuree totale de l'application:  ");

	return 1;
}
