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
 * @brief Defines ProtectionEngine interface
 * 
 * @file protection_engine.h
 */

#ifndef API_MIP_PROTECTION_PROTECTION_ENGINE_H_
#define API_MIP_PROTECTION_PROTECTION_ENGINE_H_

#include <memory>
#include <string>
#include <vector>

#include "mip/common_types.h"
#include "mip/protection/protection_common_types.h"
#include "mip/protection/protection_handler.h"
#include "mip/protection_descriptor.h"

namespace mip {

/**
 * @brief Manages protection-related actions related to a specific identity
 */
class ProtectionEngine {
public:
  /**
   * @brief Interface that receives notifications related to ProtectionEngine
   * 
   * @note This interface must be implemented by applications using the protection SDK
   */
  class Observer {
  public:
    /**
     * @brief Called when templates were retrieved successfully
     *
     * @param templateIds A reference to the list of templates retrieved
     * @param context The same context that was passed to ProtectionEngine::GetTemplatesAsync
     *
     * @note An application can pass any type of context (for example, std::promise, std::function) to
     * ProtectionEngine::GetTemplatesAsync and that same context will be forwarded as-is to
     * ProtectionEngine::Observer::OnGetTemplatesSuccess or ProtectionEngine::Observer::OnGetTemplatesFailure
     */
    virtual void OnGetTemplatesSuccess(
        const std::shared_ptr<std::vector<std::string>>& templateIds,
        const std::shared_ptr<void>& context) { UNUSED(templateIds); UNUSED(context); }

    /**
     * @brief Called when retrieving templates generated an error
     *
     * @param error Error that occurred while retrieving templates
     * @param context The same context that was passed to ProtectionEngine::GetTemplatesAsync
     *
     * @note An application can pass any type of context (for example, std::promise, std::function) to
     * ProtectionEngine::GetTemplatesAsync and that same context will be forwarded as-is to
     * ProtectionEngine::Observer::OnGetTemplatesSuccess or ProtectionEngine::Observer::OnGetTemplatesFailure
     */
    virtual void OnGetTemplatesFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when rights were retrieved successfully
     *
     * @param rights A reference to the list of rights retrieved
     * @param context The same context that was passed to ProtectionEngine::GetRightsForLabelIdAsync
     *
     * @note An application can pass any type of context (for example, std::promise, std::function) to
     * ProtectionEngine::GetRightsForLabelIdAsync and that same context will be forwarded as-is to
     * ProtectionEngine::Observer::OnGetRightsForLabelIdSuccess or ProtectionEngine::Observer::OnGetRightsForLabelIdFailure
     */
    virtual void OnGetRightsForLabelIdSuccess(
        const std::shared_ptr<std::vector<std::string>>& rights,
        const std::shared_ptr<void>& context) { UNUSED(rights); UNUSED(context); }

    /**
     * @brief Called when retrieving rights for a label ID for the user
     *
     * @param error Error that occurred while retrieving rights
     * @param context The same context that was passed to ProtectionEngine::GetRightsForLabelIdAsync
     *
     * @note An application can pass any type of context (for example, std::promise, std::function) to
     * ProtectionEngine::GetRightsForLabelIdAsync and that same context will be forwarded as-is to
     * ProtectionEngine::Observer::OnGetRightsForLabelIdSuccess or ProtectionEngine::Observer::OnGetRightsForLabelIdFailure
     */
    virtual void OnGetRightsForLabelIdFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when label IDs were retrieved successfully
     *
     * @param lableIds A reference to the list of lable IDs retrieved
     * @param context The same context that was passed to ProtectionEngine::GetGrantingLabelIdsAsync
     *
     * @note An application can pass any type of context (for example, std::promise, std::function) to
     * ProtectionEngine::GetGrantingLabelIdsAsync and that same context will be forwarded as-is to
     * ProtectionEngine::Observer::OnGetGrantingLabelIdsSuccess or ProtectionEngine::Observer::OnGetGrantingLabelIdsFailure
     */
    virtual void OnGetGrantingLabelIdsSuccess(
        const std::shared_ptr<std::vector<std::string>>& lableIds,
        const std::shared_ptr<void>& context) { UNUSED(lableIds); UNUSED(context); }

    /**
     * @brief Called when retrieving label IDs for the user
     *
     * @param error Error that occurred while retrieving label IDs
     * @param context The same context that was passed to ProtectionEngine::GetGrantingLabelIdsAsync
     *
     * @note An application can pass any type of context (for example, std::promise, std::function) to
     * ProtectionEngine::GetGrantingLabelIdsAsync and that same context will be forwarded as-is to
     * ProtectionEngine::Observer::OnGetGrantingLabelIdsSuccess or ProtectionEngine::Observer::OnGetGrantingLabelIdsFailure
     */
    virtual void OnGetGrantingLabelIdsFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /** @cond DOXYGEN_HIDE */
    virtual ~Observer() { }
  protected:
    Observer() { }
    /** @endcond */
  };
  
  /**
   * @brief Settings used by ProtectionEngine during its creation and throughout its lifetime
   */
  class Settings {
  public:
    /**
     * @brief ProtectionEngine::Settings constructor for creating a new engine
     * 
     * @param identity Identity that will be associated with ProtectionEngine
     * @param clientData customizable client data that can be stored with the engine when unloaded and can be retrieved
     * from a loaded engine.
     * @param locale Engine output will be provided in this locale.
     * 
     */
    Settings(
        const Identity& identity,
        const std::string& clientData,
        const std::string& locale = "en-US" )
        : mIdentity(identity),
          mClientData(clientData),
          mLocale(locale) {
    }

    /**
     * @brief ProtectionEngine::Settings constructor for loading an existing engine
     * 
     * @param engineId Unique identifier of engine that will be loaded
     * @param clientData customizable client data that can be stored with the engine when unloaded and can be retrieved
     * from a loaded engine.
     * @param locale Engine output will be provided in this locale.
     * 
     */
    Settings(
        const std::string& engineId,
        const std::string& clientData,
        const std::string& locale = "en-US" )
        : mEngineId(engineId),
          mClientData(clientData),
          mLocale(locale) {
    }

    /**
     * @brief Gets the engine ID.
     * 
     * @return Engine ID
     */
    const std::string& GetEngineId() const { return mEngineId; }

    /**
     * @brief Sets the engine ID.
     * 
     * @param engineId engine ID.
     */
    void SetEngineId(const std::string& engineId) { mEngineId = engineId; }

    /**
     * @brief Gets the user Identity associated with the engine
     * 
     * @return User Identity associated with the engine
     */
    const Identity& GetIdentity() const { return mIdentity; }

    /**
     * @brief Sets the user Identity associated with the engine
     * 
     * @param identity User Identity associated with the engine
     */
    void SetIdentity(const Identity& identity) { mIdentity = identity; }

    /**
     * @brief Gets custom data specified by client
     * 
     * @return Custom data specified by client
     */
    const std::string& GetClientData() const { return mClientData; }

    /**
     * @brief Sets custom data specified by client
     * 
     * @param Custom data specified by client
     */
    void SetClientData(const std::string& clientData) { mClientData = clientData; }

    /**
     * @brief Gets the locale in which engine data will be written
     * 
     * @return Locale in which engine data will be written
     */
    const std::string& GetLocale() const { return mLocale; }

    /**
     * @brief Sets name/value pairs used for testing and experimentation
     * 
     * @param customSettings Name/value pairs used for testing and experimentation
     */
    void SetCustomSettings(const std::vector<std::pair<std::string, std::string>>& value) { mCustomSettings = value; }

    /**
     * @brief Gets name/value pairs used for testing and experimentation
     * 
     * @return Name/value pairs used for testing and experimentation
     */
    const std::vector<std::pair<std::string, std::string>>& GetCustomSettings() const { return mCustomSettings; }

    /**
    * @brief Sets the engine session ID, used for correlation of logging/telemetry
    * 
    * @param sessionId Engine session ID, used for correlation of logging/telemetry
    */
    void SetSessionId(const std::string& sessionId) {
      mSessionId = sessionId;
    }

    /**
    * @brief Gets the engine session ID
    * 
    * @return Engine session ID
    */
    const std::string& GetSessionId() const {
      return mSessionId;
    }

    /**
    * @brief Optionally sets the cloud endpoint base URL
    * 
    * @param cloudEndpointBaseUrl the base URL used by all service requests (for example, "https://api.aadrm.com")
    * 
    * @note If base URL is not specified, it will be determined via DNS lookup of the engine identity's domain.
    */
    void SetCloudEndpointBaseUrl(const std::string& cloudEndpointBaseUrl) {
      mCloudEndpointBaseUrl = cloudEndpointBaseUrl;
    }

    /**
    * @brief Gets the cloud base URL used by all service requests, if specified
    * 
    * @return base URL
    */
    const std::string& GetCloudEndpointBaseUrl() const {
      return mCloudEndpointBaseUrl;
    }

  /** @cond DOXYGEN_HIDE */
  private:
    std::string mEngineId;
    Identity mIdentity;
    std::string mClientData;
    std::vector<std::pair<std::string, std::string>> mCustomSettings;
    std::string mCloudEndpointBaseUrl;
    std::string mLocale;
    std::string mSessionId;
  /** @endcond */
  };

  /**
   * @brief Gets the engine settings
   * 
   * @return Engine settings
   */
  virtual const Settings& GetSettings() const = 0;

  /**
   * @brief Get collection of templates available to a user
   *
   * @param observer A class implementing the ProtectionEngine::Observer interface
   * @param context Client context that will be opaquely passed back to observers and optional HttpDelegate
   */
  virtual void GetTemplatesAsync(
    const std::shared_ptr<ProtectionEngine::Observer>& observer,
    const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Get collection of templates available to a user
   *
   * @param context Client context that will be opaquely passed to optional HttpDelegate
   * 
   * @return List of template IDs
   */
  virtual std::vector<std::string> GetTemplates(const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Get collection of rights available to a user for a label ID
   *
   * @param documentId Document ID associated with the document metadata
   * @param labelId Label ID associated with the document metadata with which the document created
   * @param ownerEmail owner of the document
   * @param observer A class implementing the ProtectionEngine::Observer interface
   * @param context This same context will be forwarded to ProtectionEngine::Observer::OnGetRightsForLabelIdSuccess
   * or ProtectionEngine::Observer::OnGetRightsForLabelIdFailure
   */
  virtual void GetRightsForLabelIdAsync(
    const std::string& documentId,
    const std::string& labelId,
    const std::string& ownerEmail,
    const std::shared_ptr<ProtectionEngine::Observer>& observer,
    const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Get collection of rights available to a user for a labelId
   *
   * @param documentId Document ID associated with the document metadata
   * @param labelId Label ID associated with the document metadata with which the document created
   * @param ownerEmail Owner of the document
   * @param context This same context will be forwarded to optional HttpDelegate
   * 
   * @return List of rights
   */
  virtual std::vector<std::string> GetRightsForLabelId(
    const std::string& documentId,
    const std::string& labelId,
    const std::string& ownerEmail,
    const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Get collection of label IDs available to a user
   *
   * @param observer A class implementing the ProtectionEngine::Observer interface
   * @param context This same context will be forwarded to ProtectionEngine::Observer::OnGetRightsForLabelIdSuccess
   * or ProtectionEngine::Observer::OnGrantingLabelIdsFailure
   */
  virtual void GetGrantingLabelIdsAsync(
    const std::shared_ptr<ProtectionEngine::Observer>& observer,
    const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Get collection of label IDs available to a user
   *
   * @param context This same context will be forwarded to optional HttpDelegate
   * 
   * @return List of label IDs
   */
  virtual std::vector<std::string> GetGrantingLabelIds(
    const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Creates a protection handler where rights/roles are assigned to specific users
   * 
   * @param descriptor A ProtectionDescriptor describing the protection configuration
   * @param options Creation options
   * @param observer A class implementing the ProtectionHandler::Observer interface
   * @param context Client context that will be opaquely passed back to observers and optional HttpDelegate
   */
  virtual void CreateProtectionHandlerFromDescriptorAsync(
      const std::shared_ptr<ProtectionDescriptor>& descriptor,
      const ProtectionHandlerCreationOptions& options,
      const std::shared_ptr<ProtectionHandler::Observer>& observer,
      const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Creates a protection handler where rights/roles are assigned to specific users
   * 
   * @param descriptor A ProtectionDescriptor describing the protection configuration
   * @param options Creation options
   * @param context Client context that will be opaquely passed back to optional HttpDelegate
   * 
   * @return ProtectionHandler
   */
  virtual std::shared_ptr<ProtectionHandler> CreateProtectionHandlerFromDescriptor(
      const std::shared_ptr<ProtectionDescriptor>& descriptor,
      const ProtectionHandlerCreationOptions& options,
      const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Creates a protection handler from a serialized publishing license
   * 
   * @param serializedPublishingLicense A serialized publishing license
   * @param options Creation options
   * @param observer A class implementing the ProtectionHandler::Observer interface
   * @param context Client context that will be opaquely passed back to observers and optional HttpDelegate
   */
  virtual void CreateProtectionHandlerFromPublishingLicenseAsync(
      const std::vector<uint8_t>& serializedPublishingLicense,
      const ProtectionHandlerCreationOptions& options,
      const std::shared_ptr<ProtectionHandler::Observer>& observer,
      const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Creates a protection handler from a serialized publishing license
   * 
   * @param serializedPublishingLicense A serialized publishing license
   * @param options Creation options
   * @param observer A class implementing the ProtectionHandler::Observer interface
   * @param context Client context that will be opaquely passed back to optional HttpDelegate
   * 
   * @return ProtectionHandler
   */
  virtual std::shared_ptr<ProtectionHandler> CreateProtectionHandlerFromPublishingLicense(
      const std::vector<uint8_t>& serializedPublishingLicense,
      const ProtectionHandlerCreationOptions& options,
      const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Creates a protection handler from a publishing license context
   *
   * @param publishingLicenseContext A pre-processed form of the serialized publishing license
   * @param options Creation options
   * @param observer A class implementing the ProtectionHandler::Observer interface
   * @param context Client context that will be opaquely passed back to observers and optional HttpDelegate
   */
  virtual void CreateProtectionHandlerFromPublishingLicenseContextAsync(
      const PublishingLicenseContext& publishingLicenseContext,
      const ProtectionHandlerCreationOptions& options,
      const std::shared_ptr<ProtectionHandler::Observer>& observer,
      const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Creates a protection handler from a publishing license context
   *
   * @param publishingLicenseContext A pre-processed form of the serialized publishing license
   * @param options Creation options
   * @param observer A class implementing the ProtectionHandler::Observer interface
   * @param context Client context that will be opaquely passed back to optional HttpDelegate
   * 
   * @return ProtectionHandler
   */
  virtual std::shared_ptr<ProtectionHandler> CreateProtectionHandlerFromPublishingLicenseContext(
      const PublishingLicenseContext& publishingLicenseContext,
      const ProtectionHandlerCreationOptions& options,
      const std::shared_ptr<void>& context) = 0;

  /** @cond DOXYGEN_HIDE */
  virtual ~ProtectionEngine() {}
protected:
  ProtectionEngine() {}
  /** @endcond */
};
} // namespace mip

#endif // API_MIP_PROTECTION_PROTECTION_ENGINE_H_
