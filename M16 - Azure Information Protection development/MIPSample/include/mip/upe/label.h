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
 * @brief This file contains the Label class.
 * 
 * @file label.h
 */

#ifndef API_MIP_UPE_LABEL_H_
#define API_MIP_UPE_LABEL_H_

#include <memory>
#include <string>
#include <vector>

#include "mip/mip_export.h"

namespace mip {

/**
 * @brief Abstraction for a single Microsoft Information Protection label.
 */
class Label {
public:
  /**
   * @brief Get the label ID.
   * 
   * @return the label ID.
   */
  virtual const std::string& GetId() const = 0;
  /**
   * @brief Get the label name.
   * 
   * @return the label name.
   */
  virtual const std::string& GetName() const = 0;
  /**
   * @brief Get the label description.
   * 
   * @return the label description.
   */
  virtual const std::string& GetDescription() const = 0;
  /**
   * @brief Get the color the label should be displayed. 
   * 
   * @return color value the string format. "#RRGGBB" where each of RR, GG, BB is a hexadecimal 0-f.
   */
  virtual const std::string& GetColor() const = 0;
  /**
  * @brief Get the Sensitivity of the label.
  *
  * @return a numerical value. Higher value defines higher Sensitivity.
  */
  virtual int GetSensitivity() const = 0;
  /**
   * @brief Get the tooltip description of the label.
   * 
   * @return a tooltip string. 
   */
  virtual const std::string& GetTooltip() const = 0;
  /**
   * @brief Gets a boolean signaling if the label is active.
   * 
   * @note Only active labels can be applied. Inactive labels cannot be applied, and are used for display purposes only.
   * @return true if label is active, else false.
   */
  virtual bool IsActive() const = 0;
  /**
   * @brief Get the parent label.
   * 
   * @return a weak pointer to the parent label if exists else an empty pointer.
   */
  virtual std::weak_ptr<Label> GetParent() const = 0;
  /**
   * @brief Get the children labels of the current label.
   * 
   * @return a vector of shared pointers to labels. 
   */
  virtual const std::vector<std::shared_ptr<Label>>& GetChildren() const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~Label() {}

protected:
  Label() {}
  /** @endcond */
};

}  // namespace mip

#endif  // API_MIP_UPE_LABEL_H_
