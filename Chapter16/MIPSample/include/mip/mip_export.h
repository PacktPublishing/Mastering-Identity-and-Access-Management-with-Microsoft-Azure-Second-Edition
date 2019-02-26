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
/**
 * @brief A file export/import macros.
 * 
 * @file mip_export.h
 */

#ifndef API_MIP_MIP_EXPORT_H_
#define API_MIP_MIP_EXPORT_H_
/** @cond DOXYGEN_HIDE */
#ifdef _WIN32

#define __CDECL __cdecl

#ifdef MIP_IMPLEMENTATION
#define MIP_API __declspec(dllexport)
#elif NO_MIP_IMPLEMENTATION
#define MIP_API
#else // MIP_IMPLEMENTATION
#define MIP_API __declspec(dllimport)
#endif  // MIP_IMPLEMENTATION

#else  // _WIN32

#define __CDECL

#ifdef MIP_IMPLEMENTATION
#define MIP_API __attribute__((visibility("default")))
#else // MIP_IMPLEMENTATION
#define MIP_API
#endif  // MIP_IMPLEMENTATION

#endif // _WIN32
/** @endcond */

#endif  // API_MIP_MIP_EXPORT_H_
