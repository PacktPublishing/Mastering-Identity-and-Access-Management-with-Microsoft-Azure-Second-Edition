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
 * @brief Defines IStream interface
 * 
 * @file stream.h
 */

#ifndef API_MIP_STREAM_UTILS_H_
#define API_MIP_STREAM_UTILS_H_

#include <future>
#include <memory>
#include <string>
#include <vector>

#include "mip/mip_export.h"
#include "mip/stream.h"

namespace mip {
class Stream;

/**
 * @brief Creates a Stream from a std::istream
 * 
 * @param stdIStream Backing std::istream
 * 
 * @return Stream wrapping a std::istream
 */
MIP_API std::shared_ptr<mip::Stream> CreateStreamFromStdStream(const std::shared_ptr<std::istream>& stdIStream);

/**
 * @brief Creates a Stream from a std::ostream
 * 
 * @param stdOStream Backing std::ostream
 * 
 * @return Stream wrapping a std::ostream
 */
MIP_API std::shared_ptr<mip::Stream> CreateStreamFromStdStream(const std::shared_ptr<std::ostream>& stdOStream);

/**
 * @brief Creates a Stream from a std::iostream
 * 
 * @param stdIOStream Backing std::iostream
 * 
 * @return Stream wrapping a std::iostream
 */
MIP_API std::shared_ptr<mip::Stream> CreateStreamFromStdStream(const std::shared_ptr<std::iostream>& stdIOStream);

/**
 * @brief Creates an Stream from a buffer
 * 
 * @param buffer Pointer to a buffer
 * 
 * @return size Size of buffer
 */
MIP_API std::shared_ptr<mip::Stream> CreateStreamFromBuffer(uint8_t* buffer, const int64_t size);


} // namespace mip

#endif // API_MIP_STREAM_UTILS_H_
