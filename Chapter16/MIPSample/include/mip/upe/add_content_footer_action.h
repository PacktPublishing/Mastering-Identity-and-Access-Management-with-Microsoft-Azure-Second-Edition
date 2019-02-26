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
 * @brief A file Containing the AddContentFooterAction class.
 *
 * @file add_content_footer_action.h
 */

#ifndef API_MIP_UPE_ADD_CONTENT_FOOTER_ACTION_H_
#define API_MIP_UPE_ADD_CONTENT_FOOTER_ACTION_H_

#include <string>

#include "mip/common_types.h"
#include "mip/upe/action.h"

namespace mip {
/**
 * @brief An action class that specifies adding a content footer to the document.
 */
class AddContentFooterAction : public Action {
public:
  /**
   * @brief An API used to mark the content footer element.
   *
   * @return the name that should be used for the UI element that holds the content footer.
   * The same name will be returned in RemoveContentFooterAction in case the content footer
   * needs to be removed.
   */
  virtual const std::string& GetUIElementName() = 0;
  /**
   * @brief Get the text that is meant to go into the content footer.
   *
   * @return content footer text.
   */
  virtual const std::string& GetText() const = 0;
  /**
   * @brief Get the font name, used to display the content footer.
   *
   * @return Font name. Default value is Calibri if nothing is set by the policy.
   */
  virtual const std::string& GetFontName() const = 0;
  /**
   * @brief Get the font size used to display the content footer.
   *
   * @return font size as an integer.
   */
  virtual int GetFontSize() const = 0;
  /**
   * @brief Get the font color used to display the content footer.
   *
   * @return font color as a string (for example, "#000000").
   */
  virtual const std::string& GetFontColor() const = 0;
  /**
   * @brief Get the alignment of the footer.
   * @return The ContentMarkAlignment enumerator: LEFT|RIGHT|CENTER.
   * @see ContentMarkAlignment
   */
  virtual ContentMarkAlignment GetAlignment() const = 0;
  /**
   * @brief Get the margin of the footer from the bottom.
   *
   * @return the margins from the bottom of the document (for example, 10 mm).
   */
  virtual int GetMargin() const = 0;
  /** @cond DOXYGEN_HIDE */
  virtual ~AddContentFooterAction() {}

protected:
  AddContentFooterAction(const std::string& id) : Action(id) {}
  /** @endcond */
};

}  // namespace mip

#endif  // API_MIP_UPE_ADD_CONTENT_FOOTER_ACTION_H_
