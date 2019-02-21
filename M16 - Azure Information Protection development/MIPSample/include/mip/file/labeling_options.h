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

#ifndef API_MIP_FILE_LABELING_OPTIONS_H_
#define API_MIP_FILE_LABELING_OPTIONS_H_

#include <string>
#include <vector>

#include "mip/common_types.h"

namespace mip {

/**
 * @brief Interface for configuring labeling options for the SetLabel/DeleteLabel methods
 */
class LabelingOptions {
public:
  LabelingOptions(AssignmentMethod method, ActionSource actionSource)
    : mAssignmentMethod(method),
      mActionSource(actionSource),
      mIsDowngradeJustified(false) {
  }

  AssignmentMethod GetAssignmentMethod() const {
    return mAssignmentMethod;
  }

  ActionSource GetActionSource() const {
    return mActionSource;
  }

  bool IsDowngradeJustified() const {
    return mIsDowngradeJustified;
  }

  const std::string& GetJustificationMessage() const {
    return mJustificationMessage;
  }

  const std::vector<std::pair<std::string, std::string>>& GetExtendedProperties() const {
    return mExtendedProperties;
  }

  void SetDowngradeJustification(bool isDowngradeJustified, const std::string& justificationMessage) {
    mIsDowngradeJustified = isDowngradeJustified;
    mJustificationMessage = justificationMessage;
  }

  void SetExtendedProperties(const std::vector<std::pair<std::string, std::string>>& extendedProperties) {
    mExtendedProperties = extendedProperties;
  }

private:
  AssignmentMethod mAssignmentMethod;
  ActionSource mActionSource;
  bool mIsDowngradeJustified;
  std::string mJustificationMessage;
  std::vector<std::pair<std::string, std::string>> mExtendedProperties;
};

} //namespace mip

#endif // API_MIP_FILE_LABELING_OPTIONS_H_
