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
 * @brief A file Containing the CustomAction class.
 * 
 * @file custom_action.h
 */

#ifndef API_MIP_UPE_CUSTOM_ACTION_H_
#define API_MIP_UPE_CUSTOM_ACTION_H_

#include <string>
#include <utility>
#include <vector>

#include "mip/upe/action.h"
#include "mip/common_types.h"

namespace mip {

/**
 * @brief CustomAction is a generic action class that captures all the sub-properties
 * of the action as a property bag. The caller is responsible to understand
 * the meaning of the action. 
 */
class CustomAction : public Action {
public:
  /**
   * @brief Get action name.
   * 
   * @return an action name if one exists else an empty string.
   */
  virtual const std::string& GetName() const = 0;
  /**
   * @brief Get the properties key value pair list 
   * 
   * @return a key value pair list.
   */
  virtual const std::vector<std::pair<std::string, std::string>>& GetProperties() const = 0;
  /** @cond DOXYGEN_HIDE */
  virtual ~CustomAction() { }
protected:
  CustomAction(const std::string& id) : Action(id) { }
  /** @endcond */
};

}  // namespace mip

#endif  // API_MIP_UPE_CUSTOM_ACTION_H_
