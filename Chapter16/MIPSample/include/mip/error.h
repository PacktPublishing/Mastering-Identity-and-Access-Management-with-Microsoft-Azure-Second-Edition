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
 * @brief A file containing the MIP SDK error types.
 * 
 * @file error.h
 */

#ifndef API_MIP_ERROR_H_
#define API_MIP_ERROR_H_

#include <exception>
#include <memory>
#include <string>

#include "mip/common_types.h"

namespace mip {

enum class ErrorType : unsigned int {
  BAD_INPUT_ERROR, /**< Caller passed bad input. */
  FILE_IO_ERROR, /**< General File IO error. */
  NETWORK_ERROR, /**< General network issues; for example, unreachable service. */
  TRANSIENT_NETWORK_ERROR, /**< Transient network issues; for example, bad gateway. */
  INTERNAL_ERROR, /**< Internal unexpected errors. */
  JUSTIFICATION_REQUIRED, /**< Justification should be provided to complete the action on the file. */
  NOT_SUPPORTED_OPERATION, /**< The requested operation is not yet supported. */
  PRIVILEGED_REQUIRED, /**< Can't override privileged label when new label method is standard. */
  ACCESS_DENIED, /**< The user could not get access to the content. For example, no permissions, content revoked */
  CONSENT_DENIED, /**< An operation that required consent from user was not granted consent. */
  POLICY_SYNC_ERROR, /**< An attempt to sync policy data failed. */
};

/**
 * @brief Base class for all errors that will be reported (thrown or returned) from MIP SDK.
 */
class Error : public std::exception {
public:
  /**
   * @brief Get the error message.
   * 
   * @return the error message
   */
  char const* what() const NOEXCEPT override { return mMessage.c_str(); }
  /**
   * @brief Clone the error
   * 
   * @return a clone of the error.
   */
  virtual std::shared_ptr<Error> Clone() const = 0;
  /**
   * @brief Get the error type.
   * 
   * @return the error type.
   */
  virtual ErrorType GetErrorType() const { return mType; }
  /**
   * @brief Get the error name.
   * 
   * @return the error name. 
   */
  virtual const std::string& GetErrorName() const { return mName; }
  /**
   * @brief Get the error message.
   * 
   * @return the error message.
   */
  virtual const std::string& GetMessage() const { return mMessage; }
  /**
   * @brief Set the error message.
   * 
   * @param msg the error message.
   */
  virtual void SetMessage(const std::string& msg) { mMessage = msg; }

  /** @cond DOXYGEN_HIDE */
  virtual ~Error() {}
protected:
  explicit Error(const std::string& message, const std::string& name, ErrorType type)
      : mMessage(message), mName(name), mType(type) {}
  /** @endcond */

private:
  std::string mMessage;
  std::string mName;
  ErrorType mType;
};

/**
 * @brief Bad input error, thrown when the input to an SDK API is invalid.
 */
class BadInputError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit BadInputError(const std::string& message, const std::string& name = "BadInputError")
      : Error(message, name, ErrorType::BAD_INPUT_ERROR) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<BadInputError>(GetMessage()); }
  /** @endcond */
};

/**
 * @brief File IO error.
 */
class FileIOError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit FileIOError(const std::string& message, const std::string& name = "FileIOError")
      : Error(message, name, ErrorType::FILE_IO_ERROR) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<FileIOError>(GetMessage()); }
  /** @endcond */ 
};

/**
 * @brief Networking error. Caused by unexpected behavior when making network calls to service endpoints.
 */
class NetworkError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit NetworkError(const std::string& message, const std::string& name = "NetworkError")
      : Error(message, name, ErrorType::NETWORK_ERROR) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<NetworkError>(GetMessage()); }
  /** @endcond */ 
};

/**
 * @brief Transient networking error. Caused by unexpected behavior when making network calls to service endpoints.
 * The operation can be retried as this error is a transient error.
 */
class TransientNetworkError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit TransientNetworkError(const std::string& message, const std::string& name = "TransientNetworkError")
      : Error(message, name, ErrorType::TRANSIENT_NETWORK_ERROR) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<TransientNetworkError>(GetMessage()); }
  /** @endcond */
};

/**
 * @brief Internal error. This error is thrown when something unexpected happens during execution.
 */
class InternalError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit InternalError(const std::string& message, const std::string& name = "InternalError")
      : Error(message, name, ErrorType::INTERNAL_ERROR) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<InternalError>(GetMessage()); }
  /** @endcond */ 
};

/**
 * @brief The operation requested by the application is not supported by the SDK.
 */
class NotSupportedError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit NotSupportedError(const std::string& message, const std::string& name = "NotSupportedError")
      : Error(message, name, ErrorType::NOT_SUPPORTED_OPERATION) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<NotSupportedError>(GetMessage()); }
  /** @endcond */ 
};

/**
 * @brief Current label was assigned as a privileged operation (The equivalent to an administrator operation),
 * therefore it can't be overriden.
 */
class PrivilegedRequiredError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit PrivilegedRequiredError(const std::string& message, const std::string& name = "PrivilegedRequiredError")
      : Error(message, name, ErrorType::PRIVILEGED_REQUIRED) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<PrivilegedRequiredError>(GetMessage()); }
  /** @endcond */ 
};

/**
 * @brief The user could not get access to the content. For example, no permissions, content revoked.
 */
class AccessDeniedError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit AccessDeniedError(const std::string& message, const std::string& name = "AccessDeniedError")
      : Error(message, name, ErrorType::ACCESS_DENIED) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<AccessDeniedError>(GetMessage()); }
  /** @endcond */ 
};

/**
 * @brief An operation that required consent from user was not granted consent.
 */
class ConsentDeniedError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit ConsentDeniedError(const std::string& message, const std::string& name = "ConsentDeniedError")
      : Error(message, name, ErrorType::CONSENT_DENIED) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<ConsentDeniedError>(GetMessage()); }
  /** @endcond */ 
};

/**
 * @brief An attempt to sync policy data failed.
 */
class PolicySyncError : public Error {
public:
  /** @cond DOXYGEN_HIDE */
  explicit PolicySyncError(const std::string& message, const std::string& name = "PolicySyncError")
      : Error(message, name, ErrorType::POLICY_SYNC_ERROR) {}
  std::shared_ptr<Error> Clone() const override { return std::make_shared<PolicySyncError>(GetMessage()); }
  /** @endcond */
};

} // namespace mip

#endif  // API_MIP_ERROR_H_
