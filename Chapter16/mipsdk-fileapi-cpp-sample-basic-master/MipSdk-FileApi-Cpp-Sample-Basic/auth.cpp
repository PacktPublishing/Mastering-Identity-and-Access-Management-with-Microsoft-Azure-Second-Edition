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
#include "utils.h"

#include <fstream>
#include <functional>
#include <memory>
#include <string>

using std::string;
using std::runtime_error;

namespace sample {
	namespace auth {

		//Simple, hard coded token example
		string AcquireToken()
		{
			string mToken = "your token here";
			return mToken;
		}

		//This function implements token acquisition in the application by calling an external Python script.
		//The Python script requires username, password, clientId, resource, and authority. 
		//Username, Password, and ClientId are provided by the user/developer		
		//Resource and Authority are provided as part of the OAuth2Challenge object that is passed in by the SDK to the AuthDelegate.
		string AcquireToken(
			const string& username,
			const string& password,
			const string& clientId,
			const string& resource,
			const string& authority) {

			string cmd = "python";
			if (sample::utils::FileExists("auth.py"))
				cmd += " auth.py -u ";
			else if (sample::utils::FileExists("samples/auth/auth.py"))
				cmd += " samples/auth/auth.py -u ";
			else
				throw runtime_error("Unable to find auth script.");

			cmd += username;
			cmd += " -p ";
			cmd += password;
			cmd += " -a ";
			cmd += authority;
			cmd += " -r ";
			cmd += resource;
			cmd += " -c ";
			cmd += (!clientId.empty() ? clientId : "6b069eef-9dde-4a29-b402-8ce866edc897");

			string result = sample::utils::Execute(cmd.c_str());
			if (result.empty())
				throw runtime_error("Failed to acquire token. Ensure Python is installed correctly.");

			return result;
		}
	}
}
