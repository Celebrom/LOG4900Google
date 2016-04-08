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

enum class IO
{
	type = 0,
	process = 1,
	timestamp = 2,
	TID = 4,
	IrpPtr = 7,
	FileObj = 8
};

enum class opEnd
{
	type = 0,
	process = 1,
	timestamp = 2,
	TID = 4,
	IrpPtr = 7,
	FileObj = 8,
	IoType = 12
};

enum class disk
{
	process = 1
};

enum class cSwitch
{
	newProcess = 2,
	newTID = 3,
	oldProcess = 4,
	oldTID = 9
};

enum class Cevent
{
	TS = 2,
	TID = 4,
	phase = 10
};