/**
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

#ifndef SAMPLES_COMMON_AUTH_DELEGATE_IMPL_H_
#define SAMPLES_COMMON_AUTH_DELEGATE_IMPL_H_

#include <string>

#include "mip/common_types.h"

namespace sample {
namespace auth {

class AuthDelegateImpl final : public mip::AuthDelegate {
public:
  AuthDelegateImpl() = delete;

  AuthDelegateImpl(
      const std::string& password,
      const std::string& clientId,
      const std::string& sccToken,
      const std::string& protectionToken,
      const std::string& workingDirectory = "");

  bool AcquireOAuth2Token(const mip::Identity& identity, const OAuth2Challenge& challenge, OAuth2Token& token) override;

private:
  std::string mPassword;
  std::string mClientId;
  std::string mSccToken;
  std::string mProtectionToken;
  std::string mWorkingDirectory;
};

} // namespace sample
} // namespace auth

#endif // SAMPLES_COMMON_AUTH_DELEGATE_IMPL_H_
