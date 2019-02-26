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

#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#ifdef __linux__
#include <unistd.h>
#ifndef MAX_PATH
#define MAX_PATH 4096
#endif
#endif // __linux__

#include "cxxopts.hpp"

#include "auth_delegate_impl.h"
#include "consent_delegate_impl.h"
#include "file_handler_observer.h"
#include "mip/common_types.h"
#include "mip/version.h"
#include "mip/file/file_handler.h"
#include "mip/file/file_profile.h"
#include "mip/file/labeling_options.h"
#include "mip/protection_descriptor.h"
#include "mip/protection/protection_descriptor_builder.h"
#include "mip/upe/policy_engine.h"
#include "mip/user_rights.h"
#include "mip/protection/protection_handler.h"
#include "profile_observer.h"
#include "string_utils.h"

using mip::ActionSource;
using mip::AssignmentMethod;
using mip::FileHandler;
using mip::FileEngine;
using mip::FileProfile;
using mip::ContentState;
using mip::Identity;
using mip::PolicyEngine;
using mip::ProtectionDescriptor;
using mip::ProtectionDescriptorBuilder;
using mip::ProtectionHandler;
using mip::LabelingOptions;
using mip::UserRights;
using sample::auth::AuthDelegateImpl;
using sample::consent::ConsentDelegateImpl;
using std::cout;
using std::cin;
using std::endl;
using std::getline;
using std::istream;
using std::ifstream;
using std::make_shared;
using std::ostringstream;
using std::ostream_iterator;
using std::shared_ptr;
using std::static_pointer_cast;
using std::string;
using std::stringstream;
using std::vector;
using std::pair;

namespace {

static const string kApplicationName = "Microsoft Information Protection File SDK Sample";

static const char kPathSeparatorWindows = '\\';
static const char kPathSeparatorUnix = '/';
static const char kExtensionSeparator = '.';

// Explicit null character at the end is required since array initializer does NOT add it.
static const char kPathSeparatorCStringWindows[] = {kPathSeparatorWindows, '\0'};
static const char kPathSeparatorCStringUnix[] = {kPathSeparatorUnix, '\0'};
static const char kPathSeparatorsAll[] = {kPathSeparatorWindows, kPathSeparatorUnix, '\0'};

string GetFileName(const string& filePath) {
  auto index = filePath.find_last_of(kPathSeparatorsAll);
  if (index == string::npos) return filePath;
  return filePath.substr(index + 1);
}

string GetFileExtension(const string& filePath) {
  string fileName = GetFileName(filePath);
  auto index = fileName.rfind(kExtensionSeparator);
  if (index == string::npos) return "";
  return fileName.substr(index); // Include the dot in the file extension
}

bool EqualsIgnoreCase(const string& a, const string& b) {
  auto size = a.size();
  if (b.size() != size) {
    return false;
  }
    
  for (size_t i = 0; i < size; i++) {
    if (tolower(a[i]) != tolower(b[i])) {
      return false;
    }
  }

  return true;
}

// Get the current label and protection on this file and print to console label and protection information
void GetLabel(
  const shared_ptr<FileHandler>& fileHandler) {
  auto protection = fileHandler->GetProtection(); // Get the current protection on the file
  auto label = fileHandler->GetLabel(); //Get the current label on the file

  if (!label && !protection) {
    cout << "File is neither labeled nor protected" << endl;
    return;
  }

  if (label) {
    bool isPrivileged = label->GetAssignmentMethod() == AssignmentMethod::PRIVILEGED;
    auto extendedProperties = label->GetExtendedProperties();
    cout << "File is labeled as: " << label->GetLabel()->GetName() << endl;
    cout << "Id: " << label->GetLabel()->GetId() << endl;

    if (const shared_ptr<mip::Label> parent = label->GetLabel()->GetParent().lock()) {
      cout << "Parent label: " << parent->GetName() << endl;
      cout << "Parent Id: " << parent->GetId() << endl;
    }
    cout << "Set time: " << label->GetCreationTime() << endl;
    cout << "Privileged: " << (isPrivileged ? "True" : "False") << endl;
    if (!extendedProperties.empty()) {
      cout << "Extended Properties: " << endl;
    }
    for (size_t j = 0; j < extendedProperties.size(); j++) {
      cout << "Key: " << extendedProperties[j].first << ", Value: " << extendedProperties[j].second << endl;
    }

  } else
    cout << "File is not labeled" << endl;

  if (protection) {
    cout << "File is protected with ";

    const shared_ptr<ProtectionDescriptor> protectionDescriptor = protection->GetProtectionDescriptor();
    if (protectionDescriptor->GetProtectionType() == mip::ProtectionType::TemplateBased)
      cout << "template." << endl;
    else
      cout << "custom permissions." << endl;

    cout << "Name: " << protectionDescriptor->GetName() << endl;
    cout << "Template Id: " << protectionDescriptor->GetTemplateId() << endl;
    for (const auto& usersRights : protectionDescriptor->GetUserRights()) {
      cout << "Rights: ";
      auto rights = usersRights.Rights();
      copy(rights.cbegin(), rights.cend() - 1, ostream_iterator<string>(cout, ", "));
      cout << *rights.crbegin() << endl;

      cout << "For Users: ";
      auto users = usersRights.Users();
      copy(users.cbegin(), users.cend() - 1, ostream_iterator<string>(cout, "; "));
      cout << *users.crbegin() << endl;
    }
  }
}

string CreateOutput(FileHandler* fileHandler) {
  auto outputFileName = fileHandler->GetOutputFileName();
  auto fileExtension = GetFileExtension(outputFileName);
  auto outputFileNameWithoutExtension = outputFileName.substr(0, outputFileName.length() - fileExtension.length());

  if (EqualsIgnoreCase(fileExtension, ".pfile")) {
    fileExtension = GetFileExtension(outputFileNameWithoutExtension) + fileExtension;
    outputFileNameWithoutExtension = outputFileName.substr(0, outputFileName.length() - fileExtension.length());
  }

  return outputFileNameWithoutExtension + "_modified" + fileExtension;
}

void SetLabel(
  const shared_ptr<FileHandler>& fileHandler,
  const string& labelId,
  AssignmentMethod method,
  const string& justificationMessage,
  const vector<pair<string, string>>& extendedProperties) {

  LabelingOptions labelingOptions(method, mip::ActionSource::MANUAL);
  labelingOptions.SetDowngradeJustification(!justificationMessage.empty(), justificationMessage);
  labelingOptions.SetExtendedProperties(extendedProperties);

  if (labelId.empty()) {
    fileHandler->DeleteLabel(labelingOptions); // Delete the current label from the file
  } else {
    fileHandler->SetLabel(labelId, labelingOptions); // Set a label with label Id to the file
  }

  auto outputFilePath = CreateOutput(fileHandler.get());

  auto commitPromise = make_shared<std::promise<bool>>();
  auto commitFuture = commitPromise->get_future();
  fileHandler->CommitAsync(outputFilePath, commitPromise);
  auto committed = commitFuture.get();

  if (committed) {
    cout << "New file created: " << outputFilePath << endl;
    //Triggers audit event
    fileHandler->NotifyCommitSuccessful(outputFilePath);
  } else {
    if (remove(outputFilePath.c_str()) != 0) {
      throw std::runtime_error("unable to delete outputfile");
    }
  }
}

void Unprotect(
  const shared_ptr<FileHandler>& fileHandler,
  const string& filePath) {

    cout << filePath << endl;
  auto isProtected = FileHandler::IsProtected(filePath);
  // Note that only checking if the file is protected does not require any network IO or auth
  if (!isProtected) {
    cout << "File is not protected, no change made." << endl;
    return;
  }

  fileHandler->RemoveProtection(); // Remove the protection from the file
  auto outputFilePath = CreateOutput(fileHandler.get());

  auto commitPromise = make_shared<std::promise<bool>>();
  auto commitFuture = commitPromise->get_future();
  fileHandler->CommitAsync(outputFilePath, commitPromise);
  auto committed = commitFuture.get();

  if (committed) {
    cout << "New file created: " << outputFilePath << endl;
  } else {
    if (remove(outputFilePath.c_str()) != 0) {
      throw std::runtime_error("unable to delete outputfile");
    }
  }
}

// Print the labels and sublabels to the console
void ListLabels(const vector<shared_ptr<mip::Label>>& labels, const string& delimiter = "") {
  for (const auto& label : labels) {
    const string labelDescription = 
      label->GetDescription().size() < 70 ?
      label->GetDescription() :
      label->GetDescription().substr(0, 70) + "...";

    cout << delimiter << "Label ID: " << label->GetId() << "\n" <<
        delimiter << "Label name: " << label->GetName() << "\n" <<
        delimiter << "Label description: " << labelDescription << "\n" << endl;
    
    const vector<shared_ptr<mip::Label>>& childLabels = label->GetChildren();
    if (!childLabels.empty()) {
      cout << delimiter << "Child labels:" << endl;
      ListLabels(childLabels, delimiter + "  ");
    }
  }
}

void ProtectWithPermissions(
  const shared_ptr<FileHandler>& fileHandler,
  const shared_ptr<ProtectionDescriptorBuilder> descriptorBuilder) {
  const auto protectionDescriptor = descriptorBuilder->Build();
  fileHandler->SetProtection(protectionDescriptor);
  auto outputFilePath = CreateOutput(fileHandler.get());

  auto commitPromise = make_shared<std::promise<bool>>();
  auto commitFuture = commitPromise->get_future();
  fileHandler->CommitAsync(outputFilePath, commitPromise);
  auto committed = commitFuture.get();

  if (committed) {
    cout << "New file created: " << outputFilePath << endl;
  } else {
    if (remove(outputFilePath.c_str()) != 0) {
      throw std::runtime_error("unable to delete outputfile");
    }
  }
}

void ProtectWithCustomPermissions(
  const shared_ptr<FileHandler>& fileHandler,
  const string& usersList, 
  const string& rightsList) {
  vector<string> userList;
  stringstream usersListstream(usersList);
  while (usersListstream.good())
  {
    string substr;
    getline(usersListstream, substr, ',');
    userList.push_back(substr);
  }
  vector<string> rightList;
  string right;
  stringstream rightsListstream(rightsList);
  while (rightsListstream.good())
  {
    string substr;
    getline(rightsListstream, substr, ',');
    rightList.push_back(substr);
  }

  const UserRights usersRights(userList, rightList);
  ProtectWithPermissions(fileHandler, ProtectionDescriptorBuilder::CreateFromUserRights(vector<UserRights>({ usersRights })));
}

string ReadPolicyFile(const string& policyPath) {
  ifstream ifs(FILENAME_STRING(policyPath));
  if (ifs.fail())
    throw std::runtime_error("Failed to read path: " + policyPath);

  cout << "Using policy from file: " << policyPath << endl;

  ostringstream policyContent;
  policyContent << ifs.rdbuf();
  ifs.close();
  return policyContent.str();
}

shared_ptr<FileProfile> CreateProfile(
    const shared_ptr<mip::AuthDelegate>& authDelegate,
    const shared_ptr<mip::ConsentDelegate>& consentDelegate) {
  const shared_ptr<ProfileObserver> sampleProfileObserver = make_shared<ProfileObserver>();
  const FileProfile::Settings profileSettings(
      "file_sample_storage",
      true,
      authDelegate,
      consentDelegate,
      sampleProfileObserver,
      mip::ApplicationInfo{ "000", "FileSampleApp" , "1.0.0.0"});

  auto loadPromise = make_shared<std::promise<shared_ptr<FileProfile>>>();
  auto loadFuture = loadPromise->get_future();
  FileProfile::LoadAsync(profileSettings, loadPromise); // Getting the profile
  return loadFuture.get();
}

shared_ptr<FileEngine> GetFileEngine(
    const shared_ptr<FileProfile>& fileProfile,
    const string& username,
    const string& protectionBaseUrl,
    const string& policyPath,
    bool exportPolicy,
    bool protectionOnly,
    const string& locale) {
  FileEngine::Settings settings(Identity(username), "" /*clientData*/, locale);
  settings.SetProtectionCloudEndpointBaseUrl(protectionBaseUrl);
  settings.SetProtectionOnlyEngine(protectionOnly);

  if (!policyPath.empty()) { // If Policy path was given, saving the policy in custom setting
    if (exportPolicy)
      settings.SetCustomSettings({ { mip::GetCustomSettingExportPolicyFileName(), policyPath } }); //Save the path to the policy in custom setting
    else
      settings.SetCustomSettings({ { mip::GetCustomSettingPolicyDataName(), ReadPolicyFile(policyPath) } }); //Save the content of the policy in custom setting
  }

  auto addEnginePromise = make_shared<std::promise<shared_ptr<FileEngine>>>();
  auto addEngineFuture = addEnginePromise->get_future();
  fileProfile->AddEngineAsync(settings, addEnginePromise); // Getting the engine
  return addEngineFuture.get();
}

shared_ptr<FileHandler> GetFileHandler(const shared_ptr<FileEngine>& fileEngine, const string& filePath, const ContentState contentState) {
  auto createFileHandlerPromise = make_shared<std::promise<shared_ptr<FileHandler>>>();
  auto createFileHandlerFuture = createFileHandlerPromise->get_future();
  // Here content identifier is same as the filePath
  fileEngine->CreateFileHandlerAsync(filePath, filePath, contentState, false /*AuditDiscoveryEnabled*/, make_shared<FileHandlerObserver>(), createFileHandlerPromise); // create the file handler
  return createFileHandlerFuture.get();
}

string GetWorkingDirectory(int argc, char* argv[]) {
  string fileSamplePath;
  size_t position;
#ifdef _WIN32
  if (argc > 0) {
    fileSamplePath = string(argv[0]);
    position = fileSamplePath.find_last_of(kPathSeparatorWindows);
  }
#endif // _WIN32

#ifdef __linux__
  char result[MAX_PATH];
  auto count = readlink("/proc/self/exe", result, MAX_PATH);
  fileSamplePath = string(result, (count > 0) ? count : 0);
  if (count > 0) {
    position = fileSamplePath.find_last_of(kPathSeparatorUnix);
  }
#endif // __linux__
  fileSamplePath = fileSamplePath.substr(0, position + 1);
  return fileSamplePath;
}

} // namespace

int main_impl(int argc, char* argv[]) {
  try {
    const int argCount = argc; // need to save it as cxxopts change it while parsing
    auto fileSampleWorkingDirectory = GetWorkingDirectory(argc, argv);
    auto helpString =kApplicationName + " Version: " + VER_FILE_VERSION_STR;
    cxxopts::Options options("file_sample", helpString);
    options.positional_help("<Extra args>");

    options.add_options()
      // Action choice
      ("f,file", "Path to the file to work on.", cxxopts::value<string>(), "File path")
      ("g,getfilestatus", "Show the labels and protection that applies on the file.")
      ("s,setlabel", "Set a label with <labelId>. If downgrading label - will apply "
        "<justification message>, if needed and specified.", cxxopts::value<string>())
      ("d,delete", "Delete the current label from the file with <justification message>, if needed and specified.")
      ("p,protect", "Protect with custom permissions protection to comma-separated user list."
        "<rights> as permissions to those users", cxxopts::value<string>())
      ("r,rights", "Comma-separated list of rights to users", cxxopts::value<string>())
      ("templateid", "Protect using Template ID", cxxopts::value<string>())
      ("l,listlabels", "Show all available labels with their ID values.")
      ("u,unprotect", "Remove protection from the given file.")
      
      // Action-dependent options
      ("standard", "The label will be standard label adn will override standard label only.", cxxopts::value<bool>())
      ("privileged", "The label will be privileged label and will override any label.", cxxopts::value<bool>())
      ("auto", "The label will be standard label and will override any label.", cxxopts::value<bool>())
      ("j,justification", "Justification message to apply with set or remove label.", cxxopts::value<string>())
      
      // Auth options
      ("username", "Set username for authentication.", cxxopts::value<string>())
      ("password", "Set password for authentication.", cxxopts::value<string>())
      ("clientid", "Set ClientID for authentication.", cxxopts::value<string>())
      ("scctoken", "Set authentication token for SCC.", cxxopts::value<string>())
      ("protectiontoken", "Set authentication token for protection.", cxxopts::value<string>())
      ("protectionbaseurl", "Cloud endpoint base url for protection operations (e.g. https://api.aadrm.com)", cxxopts::value<string>())
      
      // Other options
      ("contentState", "(Optional) Set contentState of content. ['motion'|'use'|'rest'] (Default='rest')", cxxopts::value<string>())
      ("policy", "Set path for local policy file.", cxxopts::value<string>())
      ("exportpolicy", "Set path to export downloaded policy to.", cxxopts::value<string>())
      ("extendedkey", "Set an extended property key.", cxxopts::value<string>())
      ("extendedvalue", "Set the extended property value.", cxxopts::value<string>())
      ("locale", "Set the locale/language (default 'en-US')", cxxopts::value<string>())
      ("h,help", "Print help and exit.")
      ("version", "Display version information.");

    options.parse_positional({ "file", "setlabel", "justification", "username", "password", "policy", "exportpolicy" });
    options.parse(argc, argv);

    // help
    if (options.count("help") || argCount <= 1) {
      cout << options.help({ "" }) << endl;
      return 0;
    }

    if (options.count("version"))
    {
      cout << VER_FILE_VERSION_STR;
      return 0;
    }

    string locale = "en-US";
    if (options.count("locale"))
      locale = options["locale"].as<string>();

    // Parse auth options
    auto username = options["username"].as<string>();
    auto password = options["password"].as<string>();
    auto clientId = options["clientid"].as<string>();
    
    auto sccToken = options["scctoken"].as<string>();
    auto protectionToken = options["protectiontoken"].as<string>();

    auto protectionBaseUrl = options["protectionbaseurl"].as<string>();

    auto policyPath = options["policy"].as<string>();
    auto exportPolicyPath = options["exportpolicy"].as<string>();
    bool exportPolicy = false;
    if (policyPath.empty() && !exportPolicyPath.empty()) {
      exportPolicy = true;
      policyPath = exportPolicyPath;
    }

    const auto protectionOnly = options.count("unprotect") || options.count("protect") || options.count("templateid");
    const auto hasAuthentication = (!username.empty() && !password.empty()) || (!protectionToken.empty() && (protectionOnly || !sccToken.empty()));
    
    if (!username.empty() && !password.empty() && !sccToken.empty() && !protectionToken.empty()) {
      cout << "Only one authentication method supported. Please pass username and password or tokens";
      return 0;
    }

    if (!hasAuthentication && policyPath.empty()) {
      cout << "No authentication and no policy path was found";
      return 0;
    }

    if ((options.count("setlabel") || options.count("protect")) && username.empty() && protectionBaseUrl.empty()) {
      cout << "When applying a label, either username or protectionbaseurl must be specified";
      return 0;
    }

    auto authDelegate = make_shared<AuthDelegateImpl>(password, clientId, sccToken, protectionToken, fileSampleWorkingDirectory);
    auto consentDelegate = make_shared<ConsentDelegateImpl>();
    auto profile = CreateProfile(authDelegate, consentDelegate);
    auto fileEngine = GetFileEngine(profile, username, protectionBaseUrl, policyPath, exportPolicy, protectionOnly, locale);
    
    if (exportPolicy) {
      cout << "Policy file exported to: " << exportPolicyPath << endl;
      return 0;
    }

    // listlabels
    if (options.count("listlabels")) {
      const vector<shared_ptr<mip::Label>> labels(fileEngine->ListSensitivityLabels());
      ListLabels(labels);
      return 0;
    }

    // file
    string filePath;
    if (options.count("file")) {
      filePath = options["file"].as<string>();
    }

    if (filePath.empty()) {
      cout << options.help({ "" }) << endl;
      return 0;
    }

    ContentState contentState = ContentState::REST;
    if (options.count("contentState")) {
      string state = options["contentState"].as<string>();
      if (state == "motion") {
        contentState = ContentState::MOTION;
      } else if (state == "use") {
        contentState = ContentState::USE;
      } else if (state == "rest") {
        contentState = ContentState::REST;
      } else {
        cout << "ERROR: Invalid <contentState> value. Choose 'motion', 'use', or 'rest'" << endl;
        return -1;
      }
    }
    // All the rest of commands are file based. We need file handler.
    auto fileHandler = GetFileHandler(fileEngine, filePath, contentState);

    // getlabel
    if (options.count("getfilestatus")) {
      GetLabel(fileHandler);
      return 0;
    }

    auto method = options["auto"].as<bool>() ? AssignmentMethod::AUTO : options["privileged"].as<bool>() ?  AssignmentMethod::PRIVILEGED :
      AssignmentMethod::STANDARD;

    // setlabel
    if (options.count("setlabel")) {
      
      //Parse set label options
      const auto labelId = options["setlabel"].as<string>();
      string justificationMessage;
      string extendedKey;
      string extendedValue;
      vector<pair<string, string>> extendedProperties;

      if (options.count("justification")) {
        justificationMessage = options["justification"].as<string>();
      }

      if (options.count("extendedkey")) {
        if (options.count("extendedvalue")) {
          extendedKey = options["extendedkey"].as<string>();
          extendedValue = options["extendedvalue"].as<string>();
          pair<string, string> extendedProperty = pair<string, string>(extendedKey, extendedValue);
          extendedProperties.push_back(extendedProperty);
        } else {
          throw cxxopts::OptionException("Missing extendedvalue.");
        }
      }
      
      // Set the label on the file
      SetLabel(fileHandler, labelId, method, justificationMessage, extendedProperties);
      return 0;
    }

    // delete
    if (options.count("delete")) {

      string justificationMessage;
      if (options.count("justification")) {
        justificationMessage = options["justification"].as<string>();
      }

      // SetLabel without labelId delete the label
      SetLabel(fileHandler, "", method, justificationMessage, vector<pair<string, string>>());
      return 0;
    }

    // unprotect
    if (options.count("unprotect")) {
      Unprotect(fileHandler, filePath);
      return 0;
    }

    // protect
    if (options.count("protect")) {
      if (options.count("rights")) {
        ProtectWithCustomPermissions(fileHandler, options["protect"].as<string>(),
          options["rights"].as<string>());
        return 0;
      }

      // If protect option was given but no rights were provided throw exception
      throw cxxopts::OptionException("Missing rights for protection. use <rights>.");
    }

    //protect using template ID
    if (options.count("templateid")) {
      ProtectWithPermissions(fileHandler, ProtectionDescriptorBuilder::CreateFromTemplate(options["templateid"].as<string>()));
      return 0;
    }

    // default when there is a only file path - Show labels
    GetLabel(fileHandler);

  } catch (const cxxopts::OptionException& ex) {
    cout << "Error parsing options: " << ex.what() << endl;
    return -1;
  } catch (const std::exception& ex) {
    cout << "Something bad happend: " << ex.what() << "\nExiting." << endl;
    return -1;
  }

  return 0;
}

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[]) {
  std::vector<std::string> args;
  for (int i = 0; i < argc; ++i)
    args.push_back(ConvertWStringToString(argv[i]));
  
  std::unique_ptr<char*[]> ptr(new char*[argc + 1]);
  for (int i = 0; i < argc; ++i)
    ptr[i] = const_cast<char*>(args[i].c_str());
  ptr[argc] = nullptr;

  main_impl(argc, ptr.get());
}
#else
int main(int argc, char** argv) {
  main_impl(argc, argv);
}
#endif