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
 * @brief A file Containing the AddWatermarkAction class.
 *
 * @file add_watermark_action.h
 */

#ifndef API_MIP_UPE_ADD_WATERMARK_ACTION_H_
#define API_MIP_UPE_ADD_WATERMARK_ACTION_H_

#include <string>

#include "mip/common_types.h"
#include "mip/upe/action.h"

namespace mip {
/**
 * @brief An action class that specifies adding watermark.
 */
class AddWatermarkAction : public Action {
public:
  /**
   * @brief An API used to mark the watermark element.
   *
   * @return the name that should be used for the UI element that holds the watermark.
   * The same name will be returned in RemoveWatermarkingAction in case the watermark
   * needs to be removed.
   */
  virtual const std::string& GetUIElementName() = 0;
  /**
   * @brief An API used to get the water mark layout.
   * 
   * @return WatermarkLayout the watermarking layout in th form of an enum HORIZONTAL|DIAGONAL.
  ,
   */
  virtual WatermarkLayout GetLayout() const = 0;
  /**
   * @brief Get the text that is meant to go into the watermark.
   *
   * @return content header text.
   */
  virtual const std::string& GetText() const = 0;
  /**
   * @brief Get the font name used to display the watermark.
   *
   * @return Font name. Default value is Calibri if nothing is set by the policy.
   */
  virtual const std::string& GetFontName() const = 0;
  /**
   * @brief Get the font size used to display the watermark.
   *
   * @return font size as an integer.
   */
  virtual int GetFontSize() const = 0;
  /**
   * @brief Get the font color used to display the watermark.
   *
   * @return font color as a string (for example, "#000000").
   */
  virtual const std::string& GetFontColor() const = 0;
  /** @cond DOXYGEN_HIDE */
  virtual ~AddWatermarkAction() {}

protected:
  AddWatermarkAction(const std::string& id) : Action(id) {}
  /** @endcond */
};

}  // namespace mip

#endif  // API_MIP_UPE_ADD_WATERMARK_ACTION_H_
