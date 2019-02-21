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
 * @brief A file containing the LoggerDelegate class to be used to override MIP
 * logger
 * 
 * @file logger_delegate.h
 */


#ifndef API_MIP_LOGGER_DELEGATE_H_
#define API_MIP_LOGGER_DELEGATE_H_

#include <memory>
#include <string>

namespace mip {
/**
 * @brief Different log levels used across the MIP SDK.
 */
enum class LogLevel {
  Trace, /* Log statement is of type Trace */
  Info, /* Log statement is of type Info */
  Warning, /* Log statement is of type Warning */
  Error, /* Log statement is of type Error */
};

/**
 * @brief A class that defines the interface to the MIP SDK logger.
 */
class LoggerDelegate {
public:
   /**
   * @brief Initialize the logger.
   *
   * @param storagePath the path to the location where persistent state, including logs, may be stored.
   * @param logLevel the lowest log level that should trigger a logging event.
   */
  virtual void Init(const std::string& storagePath, LogLevel logLevel) = 0;
  /**
   * @brief Get the lowest logl evel that would trigger a logging event.
   * 
   * @return the lowest log level that would trigger a logging event.
   */
  virtual LogLevel GetLogLevel() const = 0;
   /**
   * @brief Flush the logger
   */
  virtual void Flush() = 0;
   /**
   * @brief Write a log statement to log file
   * 
   * @param level the log level for the log statement.
   * @param message the message for the log statement.
   * @param function the function name for the log statement.
   * @param file the file name where log statement was generated.
   * @param line the line number where the log statement was generated.
   */
  virtual void WriteToLog (
      const LogLevel level,
      const std::string& message,
      const std::string& function,
      const std::string& file,
      const int32_t line) = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~LoggerDelegate() {}
protected:
  LoggerDelegate() {}
   /** @endcond */
};

}  // namespace mip
#endif  // API_MIP_LOGGER_DELEGATE_H_

