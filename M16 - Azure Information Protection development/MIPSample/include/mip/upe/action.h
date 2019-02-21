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
 */
/**
 * @brief A file containing the Action base class and the ActionType enumerator.
 *
 * @file action.h
 */

#ifndef API_MIP_UPE_ACTION_H_
#define API_MIP_UPE_ACTION_H_

#include <string>

namespace mip {

/**
 * @brief Different action types.
 * 
 * @note CUSTOM is the generic action type. Every other action type is a specific action with a specific meaning.
 */
enum class ActionType : unsigned int {
  ADD_CONTENT_FOOTER = 1 << 0,     /**< Add a content footer to the document action type.*/
  ADD_CONTENT_HEADER = 1 << 1,     /**< Add a content header to the document action type.*/
  ADD_WATERMARK = 1 << 2,          /**< Add a water mark to the entire document action type.*/
  CUSTOM = 1 << 3,                 /**< A custom defined action type.*/
  JUSTIFY = 1 << 4,                /**< A justify action type.*/
  METADATA = 1 << 5,               /**< A Meta data change action type.*/
  PROTECT_ADHOC = 1 << 6,          /**< A protect by adhoc policy action type.*/
  PROTECT_BY_TEMPLATE = 1 << 7,    /**< A protect by template action type.*/
  PROTECT_DO_NOT_FORWARD = 1 << 8, /**< A protect by do not forward action type.*/
  REMOVE_CONTENT_FOOTER = 1 << 9,  /**< Remove content footer action type.*/
  REMOVE_CONTENT_HEADER = 1 << 10, /**< Remove content header action type.*/
  REMOVE_PROTECTION = 1 << 11,     /**< Remove protection action type.*/
  REMOVE_WATERMARK = 1 << 12,      /**< Remove watermarking action type.*/
  APPLY_LABEL = 1 << 13,           /**< Apply label action type.*/
  RECOMMEND_LABEL = 1 << 14,       /**< Recommend label action type.*/
};

/**
 * @brief Or (|) operator for Action type enum.
 */
inline ActionType operator| (ActionType a, ActionType b) {
  return static_cast<ActionType>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}
/**
 * @brief And (&) operator for Action type enum.
 */
inline ActionType operator& (ActionType a, ActionType b) {
  return static_cast<ActionType>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}
/**
 * @brief Xor (^) operator for Action type enum.
 */
inline ActionType operator^ (ActionType a, ActionType b) {
  return static_cast<ActionType>(static_cast<unsigned int>(a) ^ static_cast<unsigned int>(b));
}

/**
 * @brief  Interface for an action. Each action translates to a step that needs to be taken by the application to apply
 * the label (as defined in the policy)
 */
class Action {
public:
  /**
   * @brief Get the type of Action.
   *
   * @return ActionType The type of derived action this base class can be cast to.
   */
  virtual ActionType GetType() const = 0;
  /** @cond DOXYGEN_HIDE */
  virtual ~Action() {}
  virtual void Link() {}
  virtual const std::string& GetId() const { return mId; }

protected:
  Action() {}
  Action(const std::string& id) : mId(id) {}

private:
  std::string mId;
  /** @endcond */
};

}  // namespace mip

#endif  // API_MIP_UPE_ACTION_H_
