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
 * @brief This file contains the ExecutionState class.
 *
 * @file execution_state.h
 */
#ifndef API_MIP_UPE_EXECUTION_STATE_H_
#define API_MIP_UPE_EXECUTION_STATE_H_

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "mip/common_types.h"
#include "mip/protection_descriptor.h"
#include "mip/upe/action.h"
#include "mip/upe/classification_result.h"

namespace mip {

namespace auditmetadatakeys {

  /**
 * @brief Audit metadata keys in string representation.
 */
  inline std::string Sender() { return "Sender"; }
  inline std::string Recipients() { return "Recipients"; }
  inline std::string LastModifiedBy() { return "LastModifiedBy"; }
  inline std::string LastModifiedDate() { return "LastModifiedDate"; }
}

/**
 * @brief Interface for all the state needed to execute the engine.
 *
 * @note Clients should only call the methods to obtain the state that is needed.
 * Hence, for efficiency, clients may want to implement this interface such that
 * the corresponding state is computed dynamically instead of computing in advance.
 */
class ExecutionState {
public:
  /**
   * @brief Gets the sensitivity label ID that should be applied on the document.
   *
   * @return sensitivity label ID to be applied to the content if exists else empty to remove label.
   */
  virtual std::string GetNewLabelId() const = 0;

  /**
   * @brief Gets the source for a new label action.
   *
   * @return action source.
   */
  virtual ActionSource GetNewLabelActionSource() const = 0;

  /**
   * @brief Gets the content identifier that describes the document.
   *    example for a file: [path\filename]
   *    example for an email: [Subject:Sender]
   *
   * @return content identifier to be applied to the content.
   * 
   * @note This value is used by auditing as a human-readable description of the content
   */
  virtual std::string GetContentIdentifier() const = 0;

  /**
   * @brief Gets the state of the content while the application is interacting with it.
   *
   * @return State of the content data
   */
  virtual ContentState GetContentState() const = 0;

  /**
   * @brief Implementation should pass if justification to downgrade an existing label was given.
   *
   * @return true if downgrade is justified--along with the justification message--else false
   * @see mip::JustifyAction
   */
  virtual std::pair<bool, std::string> IsDowngradeJustified() const = 0;

  /**
   * @brief Get the new label's assignment method.
   * 
   * @return the assignment method STANDARD, PRIVILEGED, AUTO.
   * @see mip::AssignmentMethod
   */
  virtual AssignmentMethod GetNewLabelAssignmentMethod() const = 0;

  /**
  * @brief Return new label's extended properties
  * 
  * @return the extended properties applied to the content.
  */
  virtual std::vector<std::pair<std::string, std::string>> GetNewLabelExtendedProperties() const = 0;

  /**
   * @brief Get the meta-data items from the content.
   * 
   * @return the metadata applied to the content.
   * @note Each metadata item is a pair of name and value.
   */
  virtual std::vector<std::pair<std::string, std::string>> GetContentMetadata(
      const std::vector<std::string>& names,
      const std::vector<std::string>& namePrefixes) const = 0;
  /**
   * @brief Get the Protection Descriptor
   * 
   * @return the Protection Descriptor
   */
  virtual std::shared_ptr<ProtectionDescriptor> GetProtectionDescriptor() const = 0;
  /**
   * @brief Gets the content format.
   * 
   * @return DEFAULT, EMAIL
   * @see mip::ContentFormat
   */
  virtual ContentFormat GetContentFormat() const = 0;
  /**
  * @brief Gets a masked enum describing all the supported action types. 
  * 
  * @return a masked enum describing all the supported action types.
  * 
  * @note ActionType::Justify must be supported. When a policy and label change requires justification, 
  * a justification action will always be returned.
  */
  virtual ActionType GetSupportedActions() const = 0;

  /**
   * @brief Return a map of classification results.
   *
   * @param classificationId a list of classification IDs.
   * @return a list of classification result.
   */
  virtual std::map<std::string, std::shared_ptr<ClassificationResult>> GetClassificationResults(
      const std::vector<std::string>& /*classificationIds*/) const { 
    return std::map<std::string, std::shared_ptr<ClassificationResult>>();
  }

  /**
   * @brief Return a map of application specific audit key-value pairs.
   *
   * @return a list of application specific audit metadata
   * 
   * @note Registered Key:Value pairs
   * Sender: Email Id for the sender
   * Recipients: Represents a JSON array of recipients for an email 
   * LastModifiedBy: Email Id for the user who last modified the content 
   * LastModifiedDate: Date the content was last modified 
   */
  virtual std::map<std::string, std::string> GetAuditMetadata() const { 
    return std::map<std::string, std::string>();
  }

  /** @cond DOXYGEN_HIDE */
  virtual ~ExecutionState() { }

protected:
  ExecutionState() { }
  /** @endcond */
};

}  // namespace mip

#endif  // API_MIP_UPE_EXECUTION_STATE_H_
