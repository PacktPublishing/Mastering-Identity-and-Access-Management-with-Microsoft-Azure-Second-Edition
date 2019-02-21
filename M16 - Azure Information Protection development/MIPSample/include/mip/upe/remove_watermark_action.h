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
 * @brief A file Containing the RemoveWatermarkAction class.
 * 
 * @file remove_watermark_action.h
 */
#ifndef API_MIP_UPE_REMOVE_WATERMARK_ACTION_H_
#define API_MIP_UPE_REMOVE_WATERMARK_ACTION_H_

#include <string>
#include <vector>

#include "mip/upe/action.h"

namespace mip {

/**
 * @brief An action class that specifies removing the watermarking from the document.
 */
class RemoveWatermarkAction : public Action {
public:
  /**
   * @brief Gets a list of names that should be used to find the UI elements that
   * should be removed.
   * 
   * @return a list of ui element names. 
   */
  virtual const std::vector<std::string>& GetUIElementNames() = 0;
  /** @cond DOXYGEN_HIDE */
  virtual ~RemoveWatermarkAction() { }
protected:
  RemoveWatermarkAction(const std::string& id) : Action(id) { }
  /** @endcond */
};

}  // namespace mip

#endif  // API_MIP_UPE_REMOVE_WATERMARK_ACTION_H_
