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
 * @brief Defines ProtectionProfile interface
 * 
 * @file protection_profile.h
 */

#ifndef API_MIP_PROTECTION_PROTECTION_PROFILE_H_
#define API_MIP_PROTECTION_PROTECTION_PROFILE_H_

#include <memory>
#include <string>

#include "mip/common_types.h"
#include "mip/http_delegate.h"
#include "mip/logger_delegate.h"
#include "mip/mip_export.h"
#include "mip/protection/protection_common_types.h"
#include "mip/protection/protection_engine.h"

namespace mip {

/**
 * @brief ProtectionProfile is the root class for performing protection operations
 * 
 * @note An application needs to create a ProtectionProfile before performing any protection operations
 */
class ProtectionProfile {
public:
  /**
   * @brief Interface that receives notifications related to ProtectionProfile
   * 
   * @note This interface must be implemented by applications using the protection SDK
   */
  class Observer {
  public:
    /** @cond DOXYGEN_HIDE */
    virtual ~Observer() { }
    /** @endcond */

    /**
     * @brief Called when profile was loaded successfully
     * 
     * @param profile A reference to the newly created ProtectionProfile
     * @param context The same context that was passed to ProtectionProfile::LoadAsync
     * 
     * @note An application can pass any type of context (for example, std::promise, std::function) to
     * ProtectionProfile::LoadAsync and that same context will be forwarded as-is to
     * ProtectionProfile::Observer::OnLoadSuccess or ProtectionProfile::Observer::OnLoadFailure
     */
    virtual void OnLoadSuccess(
        const std::shared_ptr<ProtectionProfile>& profile,
        const std::shared_ptr<void>& context) { UNUSED(profile); UNUSED(context); }

    /**
     * @brief Called when loading a profile caused an error
     * 
     * @param error Error that occurred while loading
     * @param context The same context that was passed to ProtectionProfile::LoadAsync
     * 
     * @note An application can pass any type of context (for example, std::promise, std::function) to
     * ProtectionProfile::LoadAsync and that same context will be forwarded as-is to
     * ProtectionProfile::Observer::OnLoadSuccess or ProtectionProfile::Observer::OnLoadFailure
     */
    virtual void OnLoadFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when list of engines was generated successfully.
     * 
     * @param engineIds a list of engine IDs the are available.
     * @param context The same context that was passed to ProtectionProfile::ListEnginesAsync
     */
    virtual void OnListEnginesSuccess(
        const std::vector<std::string>& engineIds,
        const std::shared_ptr<void>& context) { UNUSED(engineIds); UNUSED(context); }

    /**
     * @brief Called when listing engines resulted in an error.
     * 
     * @param error the error that caused the list engines operation to fail.
     * @param context The same context that was passed to ProtectionProfile::ListEnginesAsync
     */
    virtual void OnListEnginesFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when a new engine was added successfully.
     * 
     * @param engine Newly created engine
     * @param context The same context that was passed to ProtectionProfile::AddEngineAsync
     */
    virtual void OnAddEngineSuccess(
        const std::shared_ptr<ProtectionEngine>& engine,
        const std::shared_ptr<void>& context) { UNUSED(engine); UNUSED(context); }

    /**
     * @brief Called when adding a new engine resulted in an error.
     * 
     * @param error the error that caused the add engine operation to fail.
     * @param context The same context that was passed to ProtectionProfile::AddEngineAsync
     */
    virtual void OnAddEngineFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when an engine was deleted successfully.
     * 
     * @param context The same context that was passed to ProtectionProfile::DeleteEngineAsync
     */
    virtual void OnDeleteEngineSuccess(
        const std::shared_ptr<void>& context) { UNUSED(context); }

    /**
     * @brief Called when deleting an engine resulted in an error.
     * 
     * @param error the error that caused the delete engine operation to fail.
     * @param context The same context that was passed to ProtectionProfile::DeleteEngineAsync
     */
    virtual void OnDeleteEngineFailure(
        const std::exception_ptr& error,
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /** @cond DOXYGEN_HIDE */
  protected:
    Observer() { }
    /** @endcond */
  };

  /**
   * @brief Settings used by ProtectionProfile during its creation and throughout its lifetime
   */
  class Settings {
  public:
    /**
     * @brief ProtectionProfile::Settings constructor that specifies an observer to be used for async operations
     * 
     * @param path File path under which logging, telemetry, and other protection collateral is stored
     * @param useInMemoryStorage Store any cached state in memory rather than on disk
     * @param authDelegate Callback object to be used for authentication, implemented by client application
     * @param observer Observer instance that will receive notifications of events related to ProtectionProfile
     * @param applicationInfo Info about application that is consuming the protection SDK
     */
    Settings(
        const std::string& path,
        bool useInMemoryStorage,
        const std::shared_ptr<AuthDelegate>& authDelegate,
        const std::shared_ptr<ConsentDelegate>& consentDelegate,
        const std::shared_ptr<ProtectionProfile::Observer>& observer,
        const ApplicationInfo& applicationInfo)
        : mPath(path),
          mUseInMemoryStorage(useInMemoryStorage),
          mAuthDelegate(authDelegate),
          mConsentDelegate(consentDelegate),
          mObserver(observer),
          mApplicationInfo(applicationInfo),
          mLogLevel(LogLevel::Info) {
    }

    /**
     * @brief ProtectionProfile::Settings constructor, used for synchronous operations
     * 
     * @param path File path under which logging, telemetry, and other protection collateral is stored
     * @param useInMemoryStorage Store any cached state in memory rather than on disk
     * @param authDelegate Callback object to be used for authentication, implemented by client application
     * @param applicationInfo Info about application which is consuming the protection SDK
     */
    Settings(
        const std::string& path,
        bool useInMemoryStorage,
        const std::shared_ptr<AuthDelegate>& authDelegate,
        const std::shared_ptr<ConsentDelegate>& consentDelegate,
        const ApplicationInfo& applicationInfo)
        : Settings(path, useInMemoryStorage, authDelegate, consentDelegate, nullptr /*observer*/, applicationInfo) {
    }

    /** @cond DOXYGEN_HIDE */
    ~Settings() { }
    /** @endcond */

    /**
     * @brief Gets the path under which logging, telemetry, and other protection collateral is stored
     * 
     * @return Path under which logging, telemetry, and other protection collateral is stored
     */
    const std::string& GetPath() const { return mPath; }

    /**
     * @brief Get whether or caches are stored in memory only (as opposed to on disk)
     * 
     * @return true if caches are stored in memory only
     */
    bool GetUseInMemoryStorage() const { return mUseInMemoryStorage; }

    /**
     * @brief Gets the auth delegate used for acquiring authentication tokens
     * 
     * @return Auth delegate used for acquiring authentication tokens
     */
    std::shared_ptr<AuthDelegate> GetAuthDelegate() const { return mAuthDelegate; }

    /**
     * @brief Gets the consent delegate used for connecting to services
     * 
     * @return Consent delegate used for connecting to services
     */
    std::shared_ptr<ConsentDelegate> GetConsentDelegate() const { return mConsentDelegate; }

    /**
     * @brief Gets the observer that receives notifications of events related to ProtectionProfile
     * 
     * @return Observer that receives notifications of events related to ProtectionProfile
     */
    std::shared_ptr<ProtectionProfile::Observer> GetObserver() const { return mObserver; }

    /**
     * @brief Gets info about application that is consuming the protection SDK
     * 
     * @return Info about application that is consuming the protection SDK
     */
    const ApplicationInfo& GetApplicationInfo() const { return mApplicationInfo; }

    /**
     * @brief Opts out of all telemetry gathering
     */
    void OptOutTelemetry() { mIsTelemetryOptedOut = true; }

    /**
     * @brief Gets if telemetry gathering should be disabled or not
     * 
     * @return If telemetry gathering should be disabled or not
     */
    bool IsTelemetryOptedOut() const { return mIsTelemetryOptedOut; }

    /**
     * @brief Get the logger delegate (if any) provided by the application
     * 
     * @return Logger
     */
    std::shared_ptr<LoggerDelegate> GetLoggerDelegate() const { return mLoggerDelegate; }

    /**
     * @brief Override default logger
     * 
     * @param loggerDelegate Logging callback interface implemented by client applications
     * 
     * @note This method should be called by client applications that use their own logger implementation
     */
    void SetLoggerDelegate(const std::shared_ptr<LoggerDelegate>& loggerDelegate) { mLoggerDelegate = loggerDelegate; }

    /**
     * @brief Get the HTTP delegate (if any) provided by the application
     * 
     * @return HTTP delegate to be used for HTTP operations
     */
    std::shared_ptr<HttpDelegate> GetHttpDelegate() const { return mHttpDelegate; }

    /**
     * @brief Override default HTTP stack with client's own
     * 
     * @param httpDelegate HTTP callback interface implemented by client application
     */
    void SetHttpDelegate(const std::shared_ptr<HttpDelegate>& httpDelegate) { mHttpDelegate = httpDelegate; }

    /**
     * @brief Gets if telemetry initialization should be skipped or not
     * 
     * @return If telemetry initialization should be skipped or not
     */
    bool GetSkipTelemetryInit() const { return mSkipTelemetryInit; }

    /**
     * @brief Disables telemetry initialization
     * 
     * @note This method is not typically called by client applications, rather it is used by File SDK to prevent
     * duplicate initialization
     */
    void SetSkipTelemetryInit() { mSkipTelemetryInit = true; }

    /**
     * @brief Disables new features
     * 
     * @note For applications that don't want to try new features
     */
    void SetNewFeaturesDisabled() { mNewFeaturesDisable = true; }

    /**
     * @brief Gets if new features are disabled or not
     * 
     * @return If new features are disabled or not
     */
    bool AreNewFeaturesDisabled() const { return mNewFeaturesDisable; }

    /**
     * @brief Sets the session ID
     * 
     * @param sessionId Session ID that will be used to correlate logs/telemetry
     */
    void SetSessionId(const std::string& sessionId) { mSessionId = sessionId; }

    /**
     * @brief Gets the session ID
     * 
     * @return Session ID that will be used to correlate logs/telemetry
     */
    const std::string& GetSessionId() const { return mSessionId; }

    /**
     * @brief Set the minimum log level that will trigger a logging event.
     *
     * @param logLevel minimum log level that will trigger a logging event.
     * @return true 
     */
    void SetMinimumLogLevel(LogLevel logLevel) { mLogLevel = logLevel; }
    /**
     * @brief Get the Minimum Log Level object
     * 
     * @return minimum log level that will trigger a logging event.
     */
    LogLevel GetMinimumLogLevel() const { return mLogLevel; }

  private:
    std::string mPath;
    bool mUseInMemoryStorage;
    std::shared_ptr<AuthDelegate> mAuthDelegate;
    std::shared_ptr<ConsentDelegate> mConsentDelegate;
    std::shared_ptr<ProtectionProfile::Observer> mObserver;
    std::shared_ptr<LoggerDelegate> mLoggerDelegate;
    std::shared_ptr<HttpDelegate> mHttpDelegate;
    ApplicationInfo mApplicationInfo;
    bool mIsTelemetryOptedOut = false;
    bool mSkipTelemetryInit = false;
    bool mNewFeaturesDisable = false;
    std::string mSessionId;
    LogLevel mLogLevel;
  };

  /**
   * @brief Starts loading a profile based on the provided settings.
   * 
   * @param settings Settings used by ProtectionProfile during its initialization and throughout its lifetime
   * @param context This same context will be forwarded to ProtectionProfile::Observer::OnLoadSuccess or
   * ProtectionProfile::Observer::OnLoadFailure as-is.
   * 
   * @note ProtectionProfile::Observer will be called upon success or failure.
   */
  MIP_API static void __CDECL LoadAsync(const Settings& settings, const std::shared_ptr<void>& context);

  /**
   * @brief Loading a profile based on the provided settings.
   * 
   * @param settings Settings used by ProtectionProfile during its initialization and throughout its lifetime
   * 
   * @return Newly created profile
   */
  MIP_API static std::shared_ptr<ProtectionProfile> __CDECL Load(const Settings& settings);

  /**
   * @brief Gets library version
   *
   * @return Library version
   */
  MIP_API static const char* __CDECL GetVersion();

  /**
   * @brief Gets settings used by ProtectionProfile during its initialization and throughout its lifetime
   * 
   * @return Settings used by ProtectionProfile during its initialization and throughout its lifetime
   */
  virtual const Settings& GetSettings() const = 0;

  /**
   * @brief Starts list engines operation.
   *
   * @param context Client context that will be opaquely passed back to observers
   * 
   * @note ProtectionProfile::Observer will be called upon success or failure.
   */
  virtual void ListEnginesAsync(const std::shared_ptr<void>& context) = 0;

  /**
   * @brief List engines.
   *
   * @return Cached engine IDs
   */
  virtual std::vector<std::string> ListEngines() = 0;

  /**
   * @brief Starts adding a new protection engine to the profile.
   * 
   * @param settings the mip::ProtectionEngine::Settings object that specifies the engine's settings.
   * @param context Client context that will be opaquely passed back to observers
   * 
   * @note ProtectionProfile::Observer will be called upon success or failure.
   */
  virtual void AddEngineAsync(const ProtectionEngine::Settings& settings, const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Add a new protection engine to the profile.
   *
   * @param settings the mip::ProtectionEngine::Settings object that specifies the engine's settings.
   *
   * @return Newly created ProtectionEngine
   */
  virtual std::shared_ptr<ProtectionEngine> AddEngine(const ProtectionEngine::Settings& settings) = 0;

  /**
   * @brief Starts deleting the protection engine with the given ID. All data for the given engine will be deleted.
   * 
   * @param id the unique engine ID.
   * @param context Client context that will be opaquely passed back to observers
   * 
   * @note ProtectionProfile::Observer will be called upon success or failure.
   */
  virtual void DeleteEngineAsync(const std::string& engineId, const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Delete the protection engine with the given ID. All data for the given engine will be deleted.
   *
   * @param id the unique engine ID.
   */
  virtual void DeleteEngine(const std::string& engineId) = 0;

  /**
   * @brief Creates a holder for details of a Publishing License and can be used to create a Protection Handler
   *
   * @param serializedPublishingLicense the serialized publishing license.
   *
   * @return a holder for details of a Publishing License
   */
  MIP_API static PublishingLicenseContext __CDECL GetPublishingLicenseContext(
      const std::vector<uint8_t>& serializedPublishingLicense);

  /** @cond DOXYGEN_HIDE */
  virtual ~ProtectionProfile() { }
  /** @endcond */
};

} // namespace mip

#endif // API_MIP_PROTECTION_PROTECTION_PROFILE_H_
