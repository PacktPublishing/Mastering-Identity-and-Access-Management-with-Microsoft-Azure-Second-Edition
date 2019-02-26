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
 * @brief A file containing the Stream interface/class definition.
 * 
 * @file stream.h
 */

#ifndef API_MIP_STREAM_H_
#define API_MIP_STREAM_H_

#include <future>
#include <vector>

namespace mip {

/**
 * @brief A class that defines the interface between the MIP SDK and stream-based content.
 */
class Stream {
public:
  /**
   * @brief Read into a buffer from the stream.
   * 
   * @param buffer pointer to a buffer
   * @param bufferLength buffer size.
   * @return number of bytes read.
   */
  virtual int64_t Read(uint8_t* buffer, int64_t bufferLength) = 0;

  /**
   * @brief Write into the stream from a buffer.
   *
   * @param buffer pointer to a buffer
   * @param bufferLength buffer size.
   * @return number of bytes written.
   */
  virtual int64_t Write(const uint8_t* buffer, int64_t bufferLength) = 0;

  /**
   * @brief flush the stream.
   * 
   * @return true if successful else false.
   */
  virtual bool Flush() = 0;

  /**
   * @brief Seek specific position within the stream.
   * 
   * @param position to seek into stream.
   */
  virtual void Seek(int64_t position) = 0;

  /**
   * @brief A check if stream can be read from.
   * 
   * @return true if readable else false.
   */
  virtual bool CanRead() const = 0;

  /**
   * @brief A check if stream can be written to.
   * 
   * @return true if writeable else false.
   */
  virtual bool CanWrite() const = 0;

  /**
   * @brief Get the current position within the stream. 
   * 
   * @return position within the stream.
   */
  virtual int64_t Position() = 0;

  /**
   * @brief Get the size of the content within the stream.
   * 
   * @return the stream size. 
   */
  virtual int64_t Size() = 0;

  /**
   * @brief Set the stream size.
   * 
   * @param stream size. 
   */
  virtual void Size(int64_t value) = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~Stream() { }

protected:
  Stream() { }
  /** @endcond */
}; // class Stream

} //namespace mip

#endif // API_MIP_STREAM_H_
