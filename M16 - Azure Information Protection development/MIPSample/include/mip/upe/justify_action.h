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
 * @brief This file contains the JustifyAction class.
 * 
 * @file justify_action.h
 */

#ifndef API_MIP_UPE_JUSTIFY_ACTION_H_
#define API_MIP_UPE_JUSTIFY_ACTION_H_

#include <string>

#include "mip/upe/action.h"

namespace mip {

/**
 * @brief Justify Action requires providing a justification to a label 
 * downgrade and setting the response in the execution state. 
 * @see mip::ExecutionState::IsDowngradeJustified
 */
class JustifyAction : public Action {
public:
  /** @cond DOXYGEN_HIDE */  
  virtual ~JustifyAction() { }
protected:
  JustifyAction(const std::string& id) : Action(id) { }
  /** @endcond */
};

} //namespace mip

#endif // API_MIP_UPE_JUSTIFY_ACTION_H_
