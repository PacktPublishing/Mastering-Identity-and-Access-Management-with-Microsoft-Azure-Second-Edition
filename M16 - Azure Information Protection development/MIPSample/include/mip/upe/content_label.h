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
 * @brief A file Containing the ContentLabel class.
 * 
 * @file content_label.h
 */

#ifndef API_MIP_UPE_CONTENT_LABEL_H_
#define API_MIP_UPE_CONTENT_LABEL_H_

#include "mip/common_types.h"
#include "mip/upe/label.h"

namespace mip {

/**
 * @brief Abstraction for a Microsoft Information Protection label that is applied to a piece of content, typically a
 * document.
 * @note It also holds properties for a specific applied label instance.
 */
class ContentLabel {
public:
  /**
   * @brief Get the creation time of the label
   * 
   * @return Creation time as a GMT string.
   */
  virtual const std::string& GetCreationTime() const = 0;
  /**
   * @brief Get the assignment method of the label. 
   * 
   * @return AssignmentMethod STANDARD | PRIVILEGED | AUTO.
   * @see mip::AssignmentMethod
   */
  virtual AssignmentMethod GetAssignmentMethod() const = 0;
  /**
   * @brief Gets extended properties.
   * 
   * @return Extended properties.
   */
  virtual const std::vector<std::pair<std::string, std::string>>& GetExtendedProperties() const = 0;
  /**
  * @brief Gets if protection was applied by the label or not.
  *
  * @return true if there is template protection and it was by this label, else false.
  */
  virtual bool IsProtectionAppliedFromLabel() const = 0;
  /**
   * @brief Get the actual label object applied on the content.
   * 
   * @return the label object applied on the content.
   * @see mip::Label
   */
  virtual std::shared_ptr<Label> GetLabel() const = 0;
  /** @cond DOXYGEN_HIDE */
  virtual ~ContentLabel() { }
protected:
  ContentLabel() { }
  /** @endcond */
};

} // namespace mip

#endif  // API_MIP_UPE_CONTENT_LABEL_H_