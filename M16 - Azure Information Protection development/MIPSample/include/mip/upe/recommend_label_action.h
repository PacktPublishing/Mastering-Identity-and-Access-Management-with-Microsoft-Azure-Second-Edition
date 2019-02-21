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
 * @brief This file contains the RecommendLabelAction class.
 * 
 * @file recommend_label_action.h
 */

#ifndef API_MIP_UPE_RECOMMEND_LABEL_ACTION_H_
#define API_MIP_UPE_RECOMMEND_LABEL_ACTION_H_

#include <string>

#include "mip/upe/action.h"

namespace mip {

/**
 * @brief Recommend label actions is meant to suggest a label to the users. Suppressing this call after a user ignores 
 * the recommended label should be done through the supported actions on the execution state.
 */
class RecommendLabelAction : public Action {
public:
/**
   * @brief Get the label ID suggested.
   * 
   * @return the label ID.
   */
  virtual const std::string& GetLabelId() const = 0;
  
  /** @cond DOXYGEN_HIDE */  
  virtual ~RecommendLabelAction() { }
protected:
  RecommendLabelAction(const std::string& id) : Action(id) { }
  /** @endcond */
};

} //namespace mip

#endif // API_MIP_UPE_RECOMMEND_LABEL_ACTION_H_
