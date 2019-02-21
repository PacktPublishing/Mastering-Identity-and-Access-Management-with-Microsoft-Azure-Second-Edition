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
 * @brief A file Containing the common types used by the upe, file and protection modules.
 * 
 * @file common_types.h
 */

#ifndef API_MIP_COMMON_TYPES_H_
#define API_MIP_COMMON_TYPES_H_

#include <cstring>
#include <exception>
#include <map>
#include <string>

/** @cond DOXYGEN_HIDE */
#define UNUSED(x) (void)x

#ifdef MSVC12
#define NOEXCEPT
#else
#define NOEXCEPT noexcept
#endif
/** @encond */
namespace mip {
/**
 * @brief Layout for watermarks.
 */
enum class WatermarkLayout : unsigned int {
  HORIZONTAL, /**< Watermark layout is horizontal */
  DIAGONAL, /**< Watermark layout is diagonal */
};

/**
 * @brief Alignment for content marks (content header or content footer).
 */
enum class ContentMarkAlignment : unsigned int {
  LEFT, /**< Content marking is aligned to the left */
  RIGHT, /**< Content marking is aligned to the right */
  CENTER, /**< Content marking is centered */
};

/**
 * @brief The assignment method of the label on the document. Whether the Assignment of the label was done 
 * automatically, standard or as a privileged operation (The equivalent to an administrator operation). 
 */
enum class AssignmentMethod : unsigned int {
  STANDARD, /**< Label assignment method is standard */
  PRIVILEGED, /**< Label assignment method is privileged */
  AUTO, /**< Label assignment method is automatic */
};

/**
* @brief defines what triggered the SetLabel event
*/
enum class ActionSource : unsigned int {
  MANUAL, /**< Selected manually by user */
  AUTOMATIC, /**< Set by policy conditions */
  RECOMMENDED, /**< Set by user after label was recommended by policy conditions */
  DEFAULT, /**< Set by default in policy */
  MANDATORY /**< Set by user after policy enforced to set a label */
};

/**
* @brief Defines what state of the data is the application acting upon 
*/
enum class ContentState : unsigned int {
  REST, /**< Inactive data stored physically in databases/file/warehouses */
  MOTION, /**< Data traversing a network or temporarily residing in computer memory to be read or updated */
  USE, /**< Active data under constant change stored physically in databases/file/warehouses etc */
};

/**
 * @brief Content format.
 */
enum class ContentFormat : unsigned int {
  DEFAULT, /**< Content format is standard file format */
  EMAIL, /**< Content format is email format */
};

/**
 * @brief Converts AssignmentMethod enum to a string description
 * 
 * @param method an assignment method.
 * @return a string description of the assignment method.
 */
inline std::string GetAssignmentMethodString(AssignmentMethod method) {
  static std::string kMethodArray[] = { "Standard", "Privileged", "Auto" };
  return kMethodArray[static_cast<int>(method)];
}

/**
* @brief Get the action source name.
*
* @param actionSource The action source.
* @return a string representation of the action source.
*/
inline static std::string GetActionSourceString(ActionSource actionSource) {
  static std::string kMethodArray[] = { "Manual", "Automatic", "Recommended", "Default", "Mandatory" };
  return kMethodArray[static_cast<int>(actionSource)];
}

/**
* @brief Get the content state name.
*
* @param actionSource The state of the content being worked upon.
* @return a string representation of the content state.
*/
inline static std::string GetContentStateString(mip::ContentState state) {
  static std::string kMethodArray[] = {"REST", "MOTION", "USE"};
  return kMethodArray[static_cast<int>(state)];
}

/**
 * @brief Abstraction for identity.
 */
class Identity {
public:
  /**
   * @brief Default Identity constructor used when a user email address is not known.
   */
  Identity() { }
  /**
   * @brief Identity constructor used when a user email address is known.
   * 
   * @param email user email address.
   */
  explicit Identity(const std::string& email) : mEmail(email) { mDelegatedEmail = ""; }

  /**
   * @brief Get the email.
   * 
   * @return the email.
   */
  const std::string& GetEmail() const { return mEmail; }

  /**
   * @brief Sets the delegated email, A delegated email address is an on behalf of user for which the opertations are performed.
   *
   * @param delegatedEmail the delegation email.
   */
  void SetDelegatedEmail(const std::string& delegatedEmail) { mDelegatedEmail = delegatedEmail; }

  /**
   * @brief Get the delegated email, A delegated email address is an on behalf of user for which the opertations are performed.
   * 
   * @return the delegated email.
   */
  const std::string& GetDelegatedEmail() const { return mDelegatedEmail; }  

private:
  std::string mEmail;
  std::string mDelegatedEmail;

};

/**
 * @brief Delegate for auth related operations.
 */
class AuthDelegate {
public:
  /**
   * @brief a class that contains all the information required from the calling application in order to 
   * generate an oauth2 token.
   */
  class OAuth2Challenge {
  public:
    /**
     * @brief Construct a new OAuth2Challenge object
     *
     * @param authority the authority the token needs to be generated against.
     * @param resource  the resource the token is set to.
     * @param scope  the scope the token is set to.
     */
    OAuth2Challenge(const std::string& authority, const std::string& resource, const std::string& scope = std::string())
      : mAuthority(authority),
        mResource(resource),
        mScope(scope) {
    }
    /**
     * @brief Get the authority string.
     *
     * @return the authority string.
     */
    const std::string& GetAuthority() const { return mAuthority; }
    /**
     * @brief Get the resource string
     *
     * @return the resource string.
     */
    const std::string& GetResource() const { return mResource; }
    /**
     * @brief Get the scope string
     *
     * @return the scope string.
     */
    const std::string& GetScope() const { return mScope; }
  private:
    std::string mAuthority;
    std::string mResource;
    std::string mScope;
  };

  /**
   * @brief A class defining how the MIP SDK expects the oauth2 token to be 
   * passed back into the SDK.
   */
  class OAuth2Token {
  public:
    /**
     * @brief Construct a new OAuth2Token object
     */
    OAuth2Token() {}
    /**
     * @brief Construct a new OAuth2Token object from an accessToken.
     *
     * @param accessToken The actual access token passed into the SDK.
     */
    OAuth2Token(const std::string& accessToken) : mAccessToken(accessToken) {}
    /**
     * @brief Get the Access token string.
     *
     * @return the access token string.
     */
    const std::string& GetAccessToken() const { return mAccessToken; }
    /**
     * @brief Set the Access Token string.
     *
     * @param accessToken the access token string.
     */
    void SetAccessToken(const std::string& accessToken) { mAccessToken = accessToken; }
  private:
    std::string mAccessToken;
  };

  /**
   * @brief This method is called when an auth token is required for the policy engine with the given identity and the given challenge.
   * The client should return whether acquiring token was successful. If successful, it should initialize the given token object.
   *
   * @param identity
   * @param challenge
   * @param token
   */
  virtual bool AcquireOAuth2Token(
      const mip::Identity& identity,
      const OAuth2Challenge& challenge,
      OAuth2Token& token) = 0;
  /** @cond DOXYGEN_HIDE */
  virtual ~AuthDelegate() { }
protected:
  AuthDelegate() { }
  /** @endcond */
};

/**
 * @brief A user's response when consent is requested to connect to a service endpoint.
 */ 
enum class Consent : unsigned int {
  AcceptAlways, /**< Consent, and remember this decision */
  Accept,       /**< Consent, just one time */
  Reject        /**< Do not consent */
};

/**
 * @brief Delegate for consent related operations.
 * 
 * @note This delegate is implemented by a client application to know when a
 * consent request notification should be displayed to the user.
 */
class ConsentDelegate {
public:
  /**
   * @brief Called when SDK requires user consent to connect to a service
   * endpoint.
   * 
   * @param url The URL for which the SDK requires user consent
   * 
   * @return a Consent enum with the user's decision.
   * 
   * @note When the SDK requests user consent with this method, the client
   * application should present the URL to the user. Client applications should
   * provide some means of obtaining user consent and return the appropriate
   * Consent enum that corresponds to the user's decision.
   */
  virtual Consent GetUserConsent(const std::string& url) = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~ConsentDelegate() { }
protected:
  ConsentDelegate() { }
  /** @endcond */
};

/**
 * @brief A struct that includes application specific information 
 */
struct ApplicationInfo {
  std::string applicationId;     /**< Application identifier as set in the aad portal. */
  std::string applicationName;   /**< Application name */
  std::string applicationVersion;/**< The version of the application being used */
};

/**
 * @brief Name of the setting to explicitly specify policy data.
 * 
 * @return the custom settings key.
 */
inline const std::string& GetCustomSettingPolicyDataName() {
  static const std::string kValue = "policy_data";
  return kValue;
}
/**
 * @brief Name of the setting to explicitly specify file path to export SCC policy data to.
 * 
 * @return the custom settings key.
 */
inline const std::string& GetCustomSettingExportPolicyFileName() {
  static const std::string kValue = "export_policy_file";
  return kValue;
}
/**
 * @brief Name of the setting to explicitly specify policy data file path.
 * 
 * @return the custom settings key.
 */
inline const std::string& GetCustomSettingPolicyDataFile() {
  static const std::string kValue = "policy_file";
  return kValue;
}

/** @cond DOXYGEN_HIDE */
inline const std::string& GetCustomSettingDisableServiceDiscovery() {
  static const std::string kValue = "disable_service_discovery";
  return kValue;
}
/** @endcond */

/** @cond DOXYGEN_HIDE */
class CaseInsensitiveComparator {
public:
  bool operator() (const std::string& lhs, const std::string& rhs) const {
    if (lhs.size() != rhs.size())
      return lhs.size() < rhs.size();

#ifdef _WIN32
    return _strnicmp(lhs.c_str(), rhs.c_str(), lhs.size()) < 0;
#else
    return strncasecmp(lhs.c_str(), rhs.c_str(), lhs.size()) < 0;
#endif // WIN32
  }
};
/** @endcond */

}  // namespace mip

#endif  // API_MIP_COMMON_TYPES_H_