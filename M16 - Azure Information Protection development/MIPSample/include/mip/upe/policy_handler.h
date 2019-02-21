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
 * @brief This file contains the PolicyEngine class which includes the PolicyEngine::Settings class.
 * 
 * @file policy_handler.h
 */

#ifndef API_MIP_UPE_POLICY_HANDLER_H_
#define API_MIP_UPE_POLICY_HANDLER_H_

#include <memory>
#include <string>
#include <vector>

#include "mip/common_types.h"
#include "mip/upe/action.h"
#include "mip/upe/content_label.h"
#include "mip/upe/execution_state.h"
#include "mip/upe/label.h"

namespace mip {
/**
 * @brief This class provides an interface for all policy handler functions on a file.
 */
class PolicyHandler {
public:
  /**
   * @brief Get the sensitivity label from existing content.
   * 
   * @param state Current state of the content
   * @return the label currently applied to the content. If not labeled, returns empty.
   */
  virtual std::shared_ptr<ContentLabel> GetSensitivityLabel(const ExecutionState& state) = 0;

  /**
   * @brief Executes the rules in the handler based on the provided state and returns the list of actions 
   * to be executed.
   * 
   * @param state the current execution state of the content the rules are running on.
   * @return list of actions that should be applied on the content.
   */
  virtual std::vector<std::shared_ptr<Action>> ComputeActions(const ExecutionState& state) = 0;

  /**
   * @brief Called once the computed actions have been applied, and the data committed to disk
   * 
   * @param state the current execution state of the content after the actions have been committed
   * @note: This call sends an audit event 
   */
  virtual void NotifyCommittedActions(const ExecutionState& state) = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~PolicyHandler() {}
  /** @endcond */
};

}  // namespace mip

#endif  // API_MIP_UPE_POLICY_HANDLER_H_
