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

#ifndef LOG4900GOOGLEJO_LOG4900GOOGLE_ONAGER_STATEIO_FILEIOFLUSHSTATE_H
#define LOG4900GOOGLEJO_LOG4900GOOGLE_ONAGER_STATEIO_FILEIOFLUSHSTATE_H

#include "AbstractState.h"

class FileIOFlushState : public AbstractState{
public:
	FileIOFlushState();
	~FileIOFlushState();
	virtual std::string returnJson(std::vector<std::string>* FileIoEvent, const std::vector<std::string>& OpEnd);
};

#endif // LOG4900GOOGLEJO_LOG4900GOOLE_ONAGER_STATEIO_FILEIOFLUSHSTATE_H