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
#include <etw_reader/generate_history_from_trace.h>

Timer timer;
Parser parser;

void ShowUsage() {
	std::cout
			<< "Usage: .exe --trace <trace_file_path>"
			<< std::endl
			<< std::endl;
}

void convertETLToCSV(std::wstring path)
{
	etw_insights::ETWReader etwReader;
	etwReader.Open(path);
}

/* TODO: We should only need one of the path by using the same method for the Chrome Event and Flame graph */
void convertCSVToJSON(std::wstring etl_path, std::wstring csv_path)
{
	boost::iostreams::mapped_file mmap = MemoryMapper::mapFileToMem(csv_path);
	auto pos = mmap.const_data();
	auto end = pos + mmap.size();

	std::unordered_map<std::string, std::vector<std::string>> header;
	const char*& posBeginLines = parser.parseHeader(pos, end, header);
	timer.showElapsedTime("Temps de fin de parsing du header");

	std::vector<std::string>* chromeEventLines = new std::vector<std::string>();
	parser.parseLines(posBeginLines, end, chromeEventLines);
	timer.showElapsedTime("Temps de fin de parsing des lignes du fichier");
		
	SystemHistory system_history;
	if (!GenerateHistoryFromTrace(etl_path, &system_history)) {
	LOG(ERROR) << "Error while generating history from trace.";
	return;
	}
	std::unordered_map<base::Tid, std::vector<std::string>> completedFunctions;
	parser.parseStacks(system_history, completedFunctions);

	std::wstring json_path = csv_path + L".json";
	JsonWriter::write(json_path, *chromeEventLines, completedFunctions);
	timer.showElapsedTime("Temps de fin d'ecriture du JSON");

	/* munmap is necessary because map is desallocated at the end of the process
	   boost::iostreams::mapped_file munmap(mmap, mmap.size()); */
	delete chromeEventLines;
	chromeEventLines = 0;
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
	
	std::wstring file_type = trace_path.substr(trace_path.length() - 3, 3);
	if (file_type.empty() || file_type != L"etl")
	{
		std::cout << "Please make sure the trace is of ETW type (.etl)" << std::endl << std::endl;
		ShowUsage();
		return 1;
	}

	convertETLToJSON(trace_path);
			
	timer.showElapsedTime("\n\n\nDuree totale de l'application:  ");
	system("PAUSE");

	return 1;
}
