/**
*
* Copyright (c) Microsoft Corporation.
* All rights reserved.
*
* This code is licensed under the MIT License.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*/


#include "utils.h"
#include <fstream>
#include <functional>
#include <memory>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif

using std::function;
using std::ifstream;
using std::runtime_error;
using std::string;
using std::unique_ptr;

template<typename T>
using custom_unique_ptr = unique_ptr<T, function<void(T*)>>;

static const char kPathSeparatorWindows = '\\';
static const char kPathSeparatorUnix = '/';
static const char kExtensionSeparator = '.';
static const char kPathSeparatorCStringWindows[] = { kPathSeparatorWindows, '\0' };
static const char kPathSeparatorCStringUnix[] = { kPathSeparatorUnix, '\0' };
static const char kPathSeparatorsAll[] = { kPathSeparatorWindows, kPathSeparatorUnix, '\0' };

namespace sample {	
	namespace utils {
		string Execute(const char* cmd) {
			char buffer[128];
			string result = "";

			custom_unique_ptr<FILE> pipe(POPEN(cmd, "r"), [](FILE* f) { PCLOSE(f); });
			if (nullptr == pipe.get())
				throw runtime_error("popen() failed");

			while (!feof(pipe.get())) {
				if (fgets(buffer, 128, pipe.get()) != NULL)
					result += buffer;
			}

			return result;
		}

		bool FileExists(const char* path) {
			ifstream file(path);
			return file.good();
		}

		string GetFileExtension(const string& filePath) {
			string fileName = GetFileName(filePath);
			auto index = fileName.rfind(kExtensionSeparator);
			if (index == string::npos) return "";
			return fileName.substr(index); // Include the dot in the file extension
		}

		string GetFileName(const string& filePath) {
			auto index = filePath.find_last_of(kPathSeparatorsAll);
			if (index == string::npos) return filePath;
			return filePath.substr(index + 1);
		}

		string GetOutputFileNameModified(const string& input, const string& modification)
		{
			auto result = input;
			auto fileExtension = sample::utils::GetFileExtension(result);
			auto resultWithoutExtension = result.substr(0, result.length() - fileExtension.length());
			return resultWithoutExtension + modification + fileExtension;			
		}
	}
} // namespace