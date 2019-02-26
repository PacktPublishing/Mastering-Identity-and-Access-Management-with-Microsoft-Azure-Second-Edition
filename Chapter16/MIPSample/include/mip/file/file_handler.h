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

#ifndef API_MIP_FILE_FILE_HANDLER_H_
#define API_MIP_FILE_FILE_HANDLER_H_

#include <memory>

#include "mip/file/labeling_options.h"
#include "mip/file/file_export.h"
#include "mip/protection/protection_handler.h"
#include "mip/protection_descriptor.h"
#include "mip/stream.h"
#include "mip/upe/content_label.h"

namespace mip {

/**
 * @brief Interface for all file handling functions.
 */
class FileHandler {
public:

  /**
   * @brief Observer interface for clients to get notifications events related to file handler.
   * 
   * @note All errors inherit from mip::Error.
   * @note Client should not call the engine back on the thread that calls the observer.
   */
  class Observer {
  public:
  /** @cond DOXYGEN_HIDE */
    virtual ~Observer() {}
  /** @endcond */

    /**
     * @brief Called when the handler is created successfully.
     */
    virtual void OnCreateFileHandlerSuccess(
      const std::shared_ptr<FileHandler>& fileHandler,
      const std::shared_ptr<void>& context) { UNUSED(fileHandler); UNUSED(context); }

    /**
     * @brief Called when creating the handler failed.
     */
    virtual void OnCreateFileHandlerFailure(
      const std::exception_ptr& error,
      const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when committing the changes to the file were successful.
     */
    virtual void OnCommitSuccess(
      bool committed, 
      const std::shared_ptr<void>& context) { UNUSED(committed); UNUSED(context); }

    /**
     * @brief Called when committing the changes to the file failed.
     */
    virtual void OnCommitFailure(
      const std::exception_ptr& error, 
      const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

  protected:
  /** @cond DOXYGEN_HIDE */
    Observer() {}
    /** @endcond */    
  };

  /**
   * @brief Starts retrieving the sensitivity label from the file.
   */
  virtual std::shared_ptr<ContentLabel> GetLabel() = 0;

  /**
   * @brief Starts retrieving the protection policy from the file.
   */
  virtual std::shared_ptr<ProtectionHandler> GetProtection() = 0;
  
  /**
   * @brief Checks wether a file is protected or not.
   */
  FILE_API static bool __CDECL IsProtected(const std::string& filePath);
    
  /**
   * @brief Checks wether a file is protected or not.
   */
  FILE_API static bool __CDECL IsProtected(const std::shared_ptr<Stream>& stream, const std::string& filePath);

  /**
   * @brief Sets the sensitivity label to the file.
   * 
   * @note Changes won't be written to the file until CommitAsync is called.
   *       Privileged and Auto method allows the API to override any existing label
   * @warning Throws JustificationRequiredError when setting the label requires the operation to be justified (via the labelingOptions parameter).
   */
  virtual void SetLabel(const std::string& labelId, const LabelingOptions& labelingOptions) = 0;

  /**
  * @brief Deletes the sensitivity label from the file.
  * 
  * @note Changes won't be written to the file until CommitAsync is called.
  *       Privileged and Auto method allows the API to override any existing label
  * @warning Throws JustificationRequiredError when setting the label requires the operation to be justified (via the labelingOptions parameter).
  */
  virtual void DeleteLabel(const LabelingOptions& labelingOptions) = 0;

  /**
  * @brief Sets either custom or template-based permissions (according to protectionDescriptor->GetProtectionType) to the file.
  * 
  * @note Changes won't be written to the file until CommitAsync is called.
  */
  virtual void SetProtection(const std::shared_ptr<ProtectionDescriptor>& protectionDescriptor) = 0;

  /**
   * @brief Removes protection from the file. If the file is labeled, the label will be lost.
   * 
   * @note Changes won't be written to the file until CommitAsync is called.
   */
  virtual void RemoveProtection() = 0;

  /**
   * @brief Writes the changes to the file specified by the |outputFilePath| parameter.
   * 
   * @note FileHandler::Observer will be called upon success or failure.
   */
  virtual void CommitAsync(const std::string& outputFilePath, const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Writes the changes to the stream specified by the |outputStream| parameter.
   * 
   * @note FileHandler::Observer will be called upon success or failure.
   */
  virtual void CommitAsync(const std::shared_ptr<Stream>& outputStream, const std::shared_ptr<void>& context) = 0;

 /**
   * @brief To be called when the changes have been committed to disk
   * @param contentIdentifier example for a file: "C:\mip-sdk-for-cpp\files\audit.docx"  [path\filename]
   *                          example for an email:  "RE: Audit design:user1@contoso.com"  [Subject:Sender]
   * @note Fires an Audit event
   */
  virtual void NotifyCommitSuccessful(const std::string& contentIdentifier) = 0;

  /**
   * @brief Calculates the output file name and extension based on the original file name and the accumulated changes.
   */
  virtual std::string GetOutputFileName() = 0;

/** @cond DOXYGEN_HIDE */
  virtual ~FileHandler() {}
  /** @endcond */

protected:
/** @cond DOXYGEN_HIDE */
  FileHandler() {}
  /** @endcond */
};

} //namespace mip

#endif // API_MIP_FILE_FILE_HANDLER_H_
