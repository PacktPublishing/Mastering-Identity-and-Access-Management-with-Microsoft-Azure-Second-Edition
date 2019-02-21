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

#ifndef API_MIP_FILE_FILE_PROFILE_H_
#define API_MIP_FILE_FILE_PROFILE_H_

#include <memory>
#include <string>
#include <vector>

#include "mip/common_types.h"
#include "mip/error.h"
#include "mip/file/file_engine.h"
#include "mip/file/file_export.h"
#include "mip/http_delegate.h"
#include "mip/logger_delegate.h"

namespace mip {

/**
 * @brief FileProfile class is the root class for using the Microsoft Information Protection operations.
 * 
 * A typical application will only need one Profile.
 */
class FileProfile {
public:

  /**
   * @brief Observer interface for clients to get notifications for profile related events.
   * 
   * @note All errors inherit from mip::Error.
   * @note Client should not call the engine back on the thread that calls the observer.
   */
  class Observer {
  public:
    virtual ~Observer() {}

    /**
     * @brief Called when profile was loaded successfully.
     */
    virtual void OnLoadSuccess(
        const std::shared_ptr<mip::FileProfile>& profile, 
        const std::shared_ptr<void>& context) { UNUSED(profile); UNUSED(context); }

    /**
     * @brief Called when loading a profile caused an error.
     */
    virtual void OnLoadFailure(const std::exception_ptr& error, const std::shared_ptr<void>& context) {
        UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when list of engines was generated successfully.
     */
    virtual void OnListEnginesSuccess(
        const std::vector<std::string>& engineIds, 
        const std::shared_ptr<void>& context) { UNUSED(engineIds); UNUSED(context); }

    /**
     * @brief Called when listing engines caused an error.
     */
    virtual void OnListEnginesFailure(
        const std::exception_ptr& error, 
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when an engine was unloaded successfully.
     */
    virtual void OnUnloadEngineSuccess(const std::shared_ptr<void>& context) { UNUSED(context); }

    /**
     * @brief Called when unloading an engine caused an error.
     */
    virtual void OnUnloadEngineFailure(
        const std::exception_ptr& error, 
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when a new engine was added successfully.
     */
    virtual void OnAddEngineSuccess(
        const std::shared_ptr<mip::FileEngine>& engine, 
        const std::shared_ptr<void>& context) { UNUSED(engine); UNUSED(context); }

    /**
     * @brief Called when adding a new engine caused an error.
     */
    virtual void OnAddEngineFailure(const std::exception_ptr& error, const std::shared_ptr<void>& context) { 
        UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when an engine was deleted successfully.
     */
    virtual void OnDeleteEngineSuccess(const std::shared_ptr<void>& context) { UNUSED(context); }

    /**
     * @brief Called when deleting an engine caused an error.
     */
    virtual void OnDeleteEngineFailure(
        const std::exception_ptr& error, 
        const std::shared_ptr<void>& context) { UNUSED(error); UNUSED(context); }

    /**
     * @brief Called when the policy has changed for the engine with the given ID.
     */
    virtual void OnPolicyChanged(const std::string& engineId) { UNUSED(engineId); }

  protected:
    Observer() {}
  };

  /**
   * @brief Settings used by FileProfile during its creation and throughout its lifetime
   */
  class Settings {
  public:
    /**
     * @brief FileProfile::Settings constructor
     * 
     * @param path File path under which logging, telemetry, and other persistent state is stored
     * @param useInMemoryStorage true if all state should be stored in memory, false if state can be cached to disk
     * @param authDelegate Auth delegate used for acquiring authentication tokens
     * @param observer Observer instance that will receive notifications of events related to FileProfile 
     * @param applicationInfo Info about application that is consuming the SDK
     */
    Settings(
        const std::string& path,
        bool useInMemoryStorage,
        std::shared_ptr<AuthDelegate> authDelegate,
        std::shared_ptr<ConsentDelegate> consentDelegate,
        std::shared_ptr<Observer> observer,
        const ApplicationInfo& applicationInfo)
      : mPath(path),
        mUseInMemoryStorage(useInMemoryStorage),
        mAuthDelegate(authDelegate),
        mConsentDelegate(consentDelegate),
        mObserver(observer),
        mApplicationInfo(applicationInfo),
        mLogLevel(LogLevel::Info) {
    }
    

    /** @cond DOXYGEN_HIDE */
    ~Settings() {}
    /** @endcond */

    /**
     * @brief Gets the path under which logging, telemetry, and other persistent state is stored
     * 
     * @return Path under which logging, telemetry, and other persistent state is stored
     */
    const std::string& GetPath() const { return mPath; }

    /**
     * @brief Gets if all state should be stored in memory (as opposed to on disk)
     * 
     * @return If all state should be stored in memory (as opposed to on disk)
     */
    bool GetUseInMemoryStorage() const { return mUseInMemoryStorage; }

    /**
     * @brief Gets the auth delegate used for acquiring authentication tokens
     * 
     * @return Auth delegate used for acquiring authentication tokens
     */
    std::shared_ptr<AuthDelegate> GetAuthDelegate() const { return mAuthDelegate; }

    /**
     * @brief Gets the consent delegate used to request user consent connecting to services
     * 
     * @return Consent delegate used for requesting user consent
     */
    std::shared_ptr<ConsentDelegate> GetConsentDelegate() const { return mConsentDelegate; }

    /**
     * @brief Gets the observer that receives notifications of events related to FileProfile
     * 
     * @return Observer that receives notifications of events related to FileProfile
     */
    std::shared_ptr<Observer> GetObserver() const { return mObserver; }

    /**
     * @brief Gets info about application that is consuming the SDK
     * 
     * @return Info about application that is consuming the SDK
     */
    const ApplicationInfo GetApplicationInfo() const { return mApplicationInfo; }

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
     * @brief Sets the session ID
     * 
     * @param sessionId Session ID that will be used to correlate logs/telemetry
     */
    void SetSessionId(const std::string& sessionId) {
      mSessionId = sessionId;
    }

    /**
     * @brief Gets the session ID
     * 
     * @return Session ID that will be used to correlate logs/telemetry
     */
    const std::string& GetSessionId() const {
      return mSessionId;
    }

    /**
     * @brief Set the lowest log level that will trigger a logging event.
     *
     * @param logLevel lowest log level that will trigger a logging event.
     * @return true 
     */
    void SetMinimumLogLevel(LogLevel logLevel) { mLogLevel = logLevel; }
    /**
     * @brief Get the lowest log level that will trigger a logging event
     * 
     * @return lowest log level that will trigger a logging event.
     */
    LogLevel GetMinimumLogLevel() const { return mLogLevel; }
  private:
    std::string mPath;
    bool mUseInMemoryStorage;
    std::shared_ptr<AuthDelegate> mAuthDelegate;
    std::shared_ptr<ConsentDelegate> mConsentDelegate;
    std::shared_ptr<Observer> mObserver;
    ApplicationInfo mApplicationInfo;
    bool mIsTelemetryOptedOut = false;
    bool mSkipTelemetryInit = false;
    bool mNewFeaturesDisable = false;
    std::string mSessionId;
    std::shared_ptr<LoggerDelegate> mLoggerDelegate;
    std::shared_ptr<HttpDelegate> mHttpDelegate;
    LogLevel mLogLevel;
  };

  /**
   * @brief Starts loading a profile based on the provided |settings|.
   * 
   * @note FileProfile::Observer will be called upon success or failure.
   */
  FILE_API static void __CDECL LoadAsync(const Settings& settings, const std::shared_ptr<void>& context);

  /**
   * @brief Gets library version.
   *
   * @return Version string
   */
  FILE_API static const char* __CDECL GetVersion();

/** @cond DOXYGEN_HIDE */
  virtual ~FileProfile() {}
  /** @endcond */

  /**
   * @brief Returns the profile settings.
   */
  virtual const Settings& GetSettings() const = 0;

  /**
   * @brief Starts list engines operation.
   * 
   * @note FileProfile::Observer will be called upon success or failure.
   */
  virtual void ListEnginesAsync(const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Starts unloading the file engine with the given ID.
   * 
   * @note FileProfile::Observer will be called upon success or failure.
   */
  virtual void UnloadEngineAsync(const std::string& id, const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Starts adding a new file engine to the profile.
   * 
   * @note FileProfile::Observer will be called upon success or failure.
   */
  virtual void AddEngineAsync(
      const FileEngine::Settings& settings,
      const std::shared_ptr<void>& context) = 0;

  /**
   * @brief Starts deleting the file engine with the given ID. All data for the given profile will be deleted.
   * 
   * @note FileProfile::Observer will be called upon success or failure.
   */
  virtual void DeleteEngineAsync(const std::string& id, const std::shared_ptr<void>& context) = 0;

protected:
/** @cond DOXYGEN_HIDE */
  FileProfile() {}
  /** @endcond */
};

} // namespace mip

#endif  // API_MIP_FILE_FILE_PROFILE_H_
