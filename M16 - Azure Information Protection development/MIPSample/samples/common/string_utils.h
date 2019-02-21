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

#ifndef SAMPLES_COMMON_STRING_UTILS_H_
#define SAMPLES_COMMON_STRING_UTILS_H_

#include <string>

#ifdef _WIN32
#define FILENAME_STRING(str) ConvertStringToWString(str)
#else // _WIN32
#define FILENAME_STRING(str) str
#endif

#ifndef __ANDROID__

// Converts UTF-8 string to UTF-16
std::wstring ConvertStringToWString(const std::string& str);

// Converts UTF-16 string to UTF-8
std::string ConvertWStringToString(const std::wstring& str);

#endif

#endif // SAMPLES_COMMON_STRING_UTILS_H_
