/*
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

#ifndef API_MIP_FILE_FILE_EXPORT_H_
#define API_MIP_FILE_FILE_EXPORT_H_

#ifdef _WIN32

#ifdef FILE_IMPLEMENTATION
#define FILE_API __declspec(dllexport)
#elif NO_FILE_IMPLEMENTATION
#define FILE_API
#else
#define FILE_API __declspec(dllimport)
#endif  // FILE_IMPLEMENTATION


#else  // _WIN32

#ifdef FILE_IMPLEMENTATION
#define FILE_API __attribute__((visibility("default")))
#else // FILE_IMPLEMENTATION
#define FILE_API
#endif  // FILE_IMPLEMENTATION

#endif // _WIN32

#endif  // API_MIP_FILE_FILE_EXPORT_H_
