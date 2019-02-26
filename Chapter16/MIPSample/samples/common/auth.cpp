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

#include "auth.h"

#include <fstream>
#include <functional>
#include <memory>
#include <string>

#include "string_utils.h"

#if defined(_WIN32) || defined(_WIN64)
  #define POPEN _popen
  #define PCLOSE _pclose
#else
  #define POPEN popen
  #define PCLOSE pclose
#endif

using std::function;
using std::ifstream;
using std::runtime_error;
using std::string;
using std::unique_ptr;

template<typename T>
using custom_unique_ptr = unique_ptr<T, function<void(T*)>>;

namespace {

string Execute(const char* cmd) {
    char buffer[128];
    string result = "";

  custom_unique_ptr<FILE> pipe(POPEN(cmd, "r"), [](FILE* f) { PCLOSE(f); });
  if (nullptr == pipe.get())
    throw runtime_error("popen() failed");

  while (!feof(pipe.get())) {
    if (fgets(buffer, 128, pipe.get()) != NULL)
      result += buffer;
  }

  return result;
}

bool FileExists(const char* path) {
  ifstream file(FILENAME_STRING(path));
  bool exists = file.good();
  if (exists)
    file.close();
  return exists;
}

} // namespace

namespace sample {
namespace auth {

// This function executes a Python script which acquires auth tokens directly via
// a simple http request. It is included in this sample as-is as a simple means
// of retrieving auth tokens and is not intended for use in production code.
// It will only work for tenants that support straightforward username/password
// http authentication.
//
// For proper auth integration, please use Azure Active Directory Authentication
// Library (ADAL), Active Directory v2 Libraries (MSAL), or other OAuth 2.0
// libraries:
//
// https://docs.microsoft.com/en-us/azure/active-directory/develop/active-directory-authentication-libraries
// https://docs.microsoft.com/en-us/azure/active-directory/develop/active-directory-v2-libraries
string AcquireToken(
    const string& userName,
    const string& password,
    const string& clientId,
    const string& resource,
    const string& authority,
    const string& workingDirectory) {
  string cmd = "python";
  string auth = "auth.py";
  auto fullAuthPath = workingDirectory + auth;
  if (FileExists(auth.c_str())) {
    cmd += " auth.py -u ";
  }
  else if (FileExists("samples/auth/auth.py")) {
    cmd += " samples/auth/auth.py -u ";
  }
  else if (FileExists(fullAuthPath.c_str())) {
    cmd += " ";
    cmd += fullAuthPath.c_str();
    cmd += " -u";
  }
  else {
    throw runtime_error("Unable to find auth script.");
  }

  // TODO: Client id from ADAL sample. Needs to be removed
  cmd += "\"" + userName + "\"";
  cmd += " -p ";
  cmd += "\"" + password + "\"";
  cmd += " -a ";
  cmd += authority;
  cmd += " -r ";
  cmd += resource;
  cmd += " -c ";
  cmd += (!clientId.empty() ? clientId : "6b069eef-9dde-4a29-b402-8ce866edc897");

  string result = Execute(cmd.c_str());
  if (result.empty())
    throw runtime_error("Failed to acquire token. Ensure Python is installed correctly.");

  return result;
}

} // namespace sample
} // namespace auth
