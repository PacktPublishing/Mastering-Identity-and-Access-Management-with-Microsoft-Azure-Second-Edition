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

#include "auth_delegate_impl.h"

#include <stdexcept>

#include "auth.h"

using std::runtime_error;
using std::string;

namespace sample {
namespace auth {

AuthDelegateImpl::AuthDelegateImpl(
    const string& password,
    const string& clientId,
    const string& sccToken,
    const string& protectionToken,
    const string& workingDirectory)
    : mPassword(password),
      mClientId(clientId),
      mSccToken(sccToken),
      mProtectionToken(protectionToken),
      mWorkingDirectory(workingDirectory) {
}

bool AuthDelegateImpl::AcquireOAuth2Token(
    const mip::Identity& identity,
    const OAuth2Challenge& challenge,
    OAuth2Token& token) {
  if (challenge.GetResource() == "https://syncservice.o365syncservice.com/") {
    if (!mSccToken.empty()) {
      token.SetAccessToken(mSccToken);
      return true;
    }
  } else {
    if (!mProtectionToken.empty()) {
      token.SetAccessToken(mProtectionToken);
      return true;
    }
  }

  if (mPassword.empty())
    throw runtime_error("Empty password");

  const string& tokenStr = AcquireToken(identity.GetEmail(), mPassword, mClientId, challenge.GetResource(), challenge.GetAuthority(), mWorkingDirectory);
  token.SetAccessToken(tokenStr);
  return true;
}

} // namespace sample
} // namespace auth
