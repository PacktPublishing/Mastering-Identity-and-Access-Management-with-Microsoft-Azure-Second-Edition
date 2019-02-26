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
 * @brief A file that contains version macros.
 * 
 * @file version.h
 */

#ifndef API_MIP_VERSION_H_
#define API_MIP_VERSION_H_

#include "version.inc"
/** @cond DOXYGEN_HIDE */
#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

// Each of these are limited to 65535 (16 bits)
#define VERSION_MAJOR VER_MAJOR
#define VERSION_MINOR VER_MINOR
#define VERSION_PATCH VER_PATCH

#define VER_FILE_VERSION VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VER_SUFFIX_NUM

#define VER_FILE_VERSION_STR STRINGIZE(VER_STR)
/** @endcond */
#endif // API_MIP_VERSION_H_
