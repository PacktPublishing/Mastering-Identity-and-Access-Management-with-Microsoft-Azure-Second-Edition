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
 * @brief Defines ProtectionDescriptor interface
 * 
 * @file protection_descriptor.h
 */

#ifndef API_MIP_PROTECTION_DESCRIPTOR_H_
#define API_MIP_PROTECTION_DESCRIPTOR_H_

#include <chrono>
#include <map>
#include <memory>
#include <string>

#include "mip/user_rights.h"
#include "mip/user_roles.h"

namespace mip {

/**
 * @brief Describes whether protection is based off a template or ad-hoc (custom)
 */
enum class ProtectionType {
  TemplateBased = 0, /**< Handle was created from a template */
  Custom = 1, /**< Handle was created ad hoc */
};

/**
* @brief Description of protection associated with a piece of content
*/
class ProtectionDescriptor {
public:

  /**
   * @brief Gets type of protection, whether it originated from protection SDK template or not
   * 
   * @return Type of protection
   */
  virtual ProtectionType GetProtectionType() const = 0;

  /**
   * @brief Gets owner for the protection
   * 
   * @return Owner of protection
   */
  virtual std::string GetOwner() const = 0;

  /**
   * @brief Gets protection name
   * 
   * @return Protection name
   */
  virtual std::string GetName() const = 0;

  /**
   * @brief Gets protection description
   * 
   * @return Protection description
   */
  virtual std::string GetDescription() const = 0;

  /**
   * @brief Gets the protection template ID, if any
   * 
   * @return Template ID
   */
  virtual std::string GetTemplateId() const = 0;

  /**
   * @brief Gets the label ID, if any
   * 
   * @return Label ID
   * 
   * @note This property will only be populated in ProtectionDescriptors for preexisting protected content. It is
   * a field populated by the server at the moment protected content is consumed.
   */
  virtual std::string GetLabelId() const = 0;

  /**
  * @brief Gets collection of users-to-rights mappings

  * @return Collection of users-to-rights mappings
  *
  * @note The value of the UserRights property will be empty if the current user doesn't have access to this
  * information (that is, if the user is not the owner and does not have the VIEWRIGHTSDATA right).
  */
  virtual std::vector<UserRights> GetUserRights() const = 0;

  /**
   * @brief Gets collection of users-to-roles mappings
   * 
   * @return Collection of users-to-roles mappings
   */
  virtual std::vector<UserRoles> GetUserRoles() const = 0;

  /**
   * @brief Gets protection expiration time
   * 
   * @return Protection expiration time
   */
  virtual std::chrono::time_point<std::chrono::system_clock> GetContentValidUntil() const = 0;

  /**
   * @brief Gets if protection allows offline content access or not
   * 
   * @return If protection allows offline content access or not (default = true)
   */
  virtual bool DoesAllowOfflineAccess() const = 0;

  /**
   * @brief Gets protection referrer address
   * 
   * @return Protection referrer address
   * 
   * @note The referrer is a URI that is displayable to the user if they cannot unprotect the content. It contains
   * information on how that user can gain permission to access the content.
   */
  virtual std::string GetReferrer() const = 0;

  /**
   * @brief Gets app-specific data that was encrypted
   * 
   * @return App-specific data
   * 
   * @note A ProtectionHandler may hold a dictionary of app-specific data that was encrypted by the protection
   * service. This encrypted data is independent of the signed data accessible via ProtectionDescriptor::GetSignedAppData
   */
  virtual std::map<std::string, std::string> GetEncryptedAppData() const = 0;

  /**
   * @brief Gets the app-specific data that was signed
   * 
   * @return App-specific data
   * 
   * @note A ProtectionHandler may hold a dictionary of app-specific data that was signed by the protection service.
   * This signed data is independent of the encrypted data accessible via ProtectionDescriptor::GetEncryptedAppData
   */
  virtual std::map<std::string, std::string> GetSignedAppData() const = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~ProtectionDescriptor() {}
protected:
  ProtectionDescriptor() {}
  /** @endcond */
};
} // namespace mip

#endif //API_MIP_PROTECTION_DESCRIPTOR_H_
