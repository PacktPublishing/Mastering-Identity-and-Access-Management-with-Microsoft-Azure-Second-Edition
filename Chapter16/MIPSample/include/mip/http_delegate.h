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
 * @brief Contains HttpDelegate interface definition used to override MIP HTTP stack
 * 
 * @file http_delegate.h
 */


#ifndef API_MIP_HTTP_DELEGATE_H_
#define API_MIP_HTTP_DELEGATE_H_

#include <memory>

#include "mip/http_request.h"
#include "mip/http_response.h"

namespace mip {

/**
 * @brief Interface for overriding HTTP handling
 */
class HttpDelegate {
public:
   /**
   * @brief Send HTTP request
   *
   * @param request HTTP request
   * @param context The same opaque client context that was passed to the API that resulted in this HTTP request
   * 
   * @return HTTP response
   */
  virtual std::shared_ptr<HttpResponse> Send(
      const std::shared_ptr<HttpRequest>& request,
      const std::shared_ptr<void>& context) = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~HttpDelegate() {}
   /** @endcond */
};

}  // namespace mip
#endif  // API_MIP_HTTP_DELEGATE_H_

