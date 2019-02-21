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
 * @brief Defines ProtectionHandler interface
 * 
 * @file protection_handler.h
 */

#ifndef API_MIP_PROTECTION_PROTECTION_HANDLER_H_
#define API_MIP_PROTECTION_PROTECTION_HANDLER_H_

#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "mip/common_types.h"
#include "mip/protection_descriptor.h"
#include "mip/stream.h"
#include "mip/user_rights.h"
#include "mip/user_roles.h"

namespace mip {
class ICryptoProvider;

/**
 * @brief Bit flags that dictate additional policy creation behavior
 */
enum ProtectionHandlerCreationOptions {
  None = 0x0, /**< None */
  /**
   * @brief Do not allow UI and network operations.
   * 
   * @note The SDK will only attempt an offline creation from cache. If network connectivity is
   * required, the operation will fail. For example, this option may be used by an application that wishes to open
   * content only if it can be done without a network connection.
   */
  OfflineOnly = 0x1,
  AllowAuditedExtraction = 0x2, /**< Content can be opened in a non-protection-SDK-aware app */
  PreferDeprecatedAlgorithms = 0x4, /**< Use deprecated crypto algorithms (ECB) for backwards compatibility */
};

/**
 * @brief ProtectionHandlerCreationOptions bitwise OR operator
 * 
 * @param a Left value
 * @param b Right value
 * 
 * @return Bitwise OR of ProtectionHandlerCreationOptions
 */
inline ProtectionHandlerCreationOptions operator|(
    ProtectionHandlerCreationOptions a, 
    ProtectionHandlerCreationOptions b) {
  return static_cast<ProtectionHandlerCreationOptions>(static_cast<int>(a) | static_cast<int>(b));
}

/**
 * @brief Manages protection-related actions for a specific protection configuration
 */
class ProtectionHandler {
public:
  /**
   * @brief Interface that receives notifications related to ProtectionHandler
   * 
   * @note This interface must be implemented by applications using the protection SDK
   */
  class Observer {
  public:
    /**
     * @brief Called when ProtectionHandler was created successfully
     *
     * @param protectionHandler The newly created ProtectionHandler
     * @param context The same context that was passed to ProtectionEngine::CreateProtectionHandlerFromDescriptorAsync or
     * ProtectionEngine::CreateProtectionHandlerFromPublishingLicenseAsync
     *
     * @note An application can pass any type of context (for example, std::promise, std::function) to
     * ProtectionEngine::CreateProtectionHandlerFromDescriptorAsync or
     * ProtectionEngine::CreateProtectionHandlerFromPublishingLicenseAsync, and that same context will be forwarded
     * as-is to ProtectionEngine::Observer::OnCreateProtectionHandlerSuccess or
     * ProtectionEngine::Observer::OnCreateProtectionHandlerFailure
     */
    virtual void OnCreateProtectionHandlerSuccess(
        const std::shared_ptr<ProtectionHandler>& protectionHandler,
        const std::shared_ptr<void>& context) { UNUSED(protectionHandler); UNUSED(context); };

    /**
     * @brief Called when ProtectionHandler creation failed
     *
     * @param error Failure that occurred during creation
     * @param context The same context that was passed to ProtectionEngine::CreateProtectionHandlerFromDescriptorAsync or
     * ProtectionEngine::CreateProtectionHandlerFromPublishingLicenseAsync
     *
     * @note An application can pass any type of context (for example, std::promise, std::function) to
     * ProtectionEngine::CreateProtectionHandlerFromDescriptorAsync or
     * ProtectionEngine::CreateProtectionHandlerFromPublishingLicenseAsync, and that same context will be forwarded
     * as-is to ProtectionEngine::Observer::OnCreateProtectionHandlerSuccess or
     * ProtectionEngine::Observer::OnCreateProtectionHandlerFailure
     */
    virtual void OnCreateProtectionHandlerFailure(
        const std::exception_ptr& error, 
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /** @cond DOXYGEN_HIDE */
    virtual ~Observer() { }
    /** @endcond */
  };

  /**
   * @brief Create a protected stream that will allow for encryption/decryption of content
   * 
   * @param backingStream Backing stream from which to read/write
   * @param contentStartPosition Starting position (in bytes) within the backing stream where protected content begins
   * @param contentSize Size (in bytes) of protected content within backing stream
   * 
   * @return Protected stream
   */
  virtual std::shared_ptr<Stream> CreateProtectedStream(
      const std::shared_ptr<Stream>& backingStream,
      int64_t contentStartPosition,
      int64_t contentSize) = 0;

  /**
   * @brief Encrypt a buffer
   *
   * @param offsetFromStart Relative position of inputBuffer from the very beginning of the cleartext content
   * @param inputBuffer Buffer of cleartext content that will be encrypted
   * @param inputBufferSize Size (in bytes) of input buffer
   * @param outputBuffer Buffer into which encrypted content will be copied
   * @param outputBufferSize Size (in bytes) of output buffer
   * @param isFinal If input buffer contains the final cleartext bytes or not
   * 
   * @return actual size (in bytes) of encrypted content
   */
  virtual int64_t EncryptBuffer(
      int64_t offsetFromStart,
      const uint8_t* inputBuffer,
      int64_t inputBufferSize,
      uint8_t* outputBuffer,
      int64_t outputBufferSize,
      bool isFinal) = 0;

  /**
   * @brief Decrypt a buffer
   *
   * @param offsetFromStart Relative position of inputBuffer from the very beginning of the encrypted content
   * @param inputBuffer Buffer of encrypted content that will be decrypted
   * @param inputBufferSize Size (in bytes) of input buffer
   * @param outputBuffer Buffer into which decrypted content will be copied
   * @param outputBufferSize Size (in bytes) of output buffer
   * @param isFinal If input buffer contains the final encrypted bytes or not
   * 
   * @return actual size (in bytes) of decrypted content
   */
  virtual int64_t DecryptBuffer(
      int64_t offsetFromStart,
      const uint8_t* inputBuffer,
      int64_t inputBufferSize,
      uint8_t* outputBuffer,
      int64_t outputBufferSize,
      bool isFinal) = 0;

  /**
   * @brief Calculates size (in bytes) of content if it were to be encrypted with this ProtectionHandler
   * 
   * @param unprotectedLength Size (in bytes) of unprotected content
   * @param includesFinalBlock Describes if the unprotected content in question includes the final block or not.
   *  For example, in CBC4k encryption mode, non-final protected blocks are the same size as unprotected blocks, but
   *  final protected blocks are larger than their unprotected counterparts.
   * 
   * @return Size (in bytes) of protected content
   */
  virtual int64_t GetProtectedContentLength(int64_t unprotectedLength, bool includesFinalBlock) = 0;

  /**
   * @brief Gets the block size (in bytes) for the cipher mode used by this ProtectionHandler
   * 
   * @return Block size (in bytes)
   */
  virtual int64_t GetBlockSize() = 0;

  /**
   * @brief Gets the rights granted to the user/identity associated with this ProtectionHandler
   * 
   * @return Rights granted to the user
   */
  virtual std::vector<std::string> GetRights() const = 0;

  /**
   * @brief Checks if protection handler grants user access to the specified right
   * 
   * @param right Right to check
   * 
   * @return If protection handler grants user access to the specified right or not
   */
  virtual bool AccessCheck(const std::string& right) const = 0;

  /**
   * @brief Gets user associated with the protection handler
   *
   * @return User associated with protection handler
   */
  virtual const std::string GetIssuedTo() = 0;

  /**
   * @brief Gets email address of content owner
   * 
   * @return Email address of content owner
   */
  virtual const std::string GetOwner() = 0;

  /**
   * @brief Gets if the current user is the content owner or not
   * 
   * @return If the current user is the content owner or not
   */
  virtual bool IsIssuedToOwner() = 0;

  /**
   * @brief Gets protection details
   * 
   * @return Protection details
   */
  virtual std::shared_ptr<ProtectionDescriptor> GetProtectionDescriptor() = 0;

  /**
   * @brief Gets unique identifier for the document/content
   * 
   * @return Unique content identifier
   */
  virtual const std::string GetContentId() = 0;

  /**
   * @brief Gets if protection handler uses deprecated crypto algorithms (ECB) for backward compatibility or not
   * 
   * @return If protection handler uses deprecated crypto algorithms or not
   */
  virtual bool DoesUseDeprecatedAlgorithms() = 0;

  /**
   * @brief Gets if protection handler grants user 'audited extract' right or not
   * 
   * @return If protection handler grants user 'audited extract' right or not
   */
  virtual bool IsAuditedExtractAllowed() = 0;

  /**
   * @brief Serialize ProtectionHandler into a publishing license (PL)
   * 
   * @return Serialized publishing license
   */
  virtual const std::vector<uint8_t> GetSerializedPublishingLicense() = 0;

  /** @cond DOXYGEN_HIDE */
  virtual const std::shared_ptr<ICryptoProvider> GetCryptoProvider() = 0;
  virtual ~ProtectionHandler() {}

  /** @endcond */
};
} // namespace mip
#endif // API_MIP_PROTECTION_PROTECTION_HANDLER_H_
