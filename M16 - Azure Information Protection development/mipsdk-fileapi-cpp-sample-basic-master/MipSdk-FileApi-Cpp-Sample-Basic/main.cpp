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

#include "action.h"
#include "mip/common_types.h"
#include "utils.h"

using std::make_shared;
using std::shared_ptr;
using std::string;

using std::cout;
using std::cin;
using std::endl;

using sample::file::Action;

int main()
{
	//local variables to store target file and the label that will be applied to the file.
	string filePath;
	string labelToApply;		

	// Create the mip::ApplicationInfo object. 
	// Client ID should be the client ID registered in Azure AD for your custom application.
	// Friendly Name should be the name of the application as it should appear in reports.
	mip::ApplicationInfo appInfo{"f2808158-d40c-4588-8b7d-17460668639c", "MipSdkFileApiCppSampleBasic" };
	
	// All actions for this tutorial project are implemented in samples::file::Action
	// Source files are Action.h/cpp.
	// "File" was chosen because this example is specifically for the MIP SDK File API. 
	// Action's constructor takes in the mip::ApplicationInfo object and uses the client ID for auth.
	// Username and password are required in this sample as the oauth2 token is obtained via Python script and basic auth.
	Action action = Action(appInfo, "don.hall@inovitdemos.ch","Kins!1318");

	// Call action.ListLabels() to display all available labels, then pause.
	action.ListLabels();	
	system("pause");
	
	// Prompt the user to copy the Label ID from a displayed label. This will be stored then applied later to a file.
	cout << "Copy a label ID from above to apply to a new file." << endl;
	cout << "Label ID: ";
	cin >> labelToApply;
	
	// Prompt the user to enter a file. A labeled copy of this file will be created.
	cout << "Enter a file path for the input file: ";
	cin >> filePath;
	
	// Show action plan
	cout << "Applying Label ID " + labelToApply + " to " + filePath << endl;
	
	// Use GetOutputFileNameModified from utils to generate the output file name.
	string outputFileName = sample::utils::GetOutputFileNameModified(filePath, "_modified");

	// Call Action.SetLabel, providing the file to label, generated output name, the label ID, and a justification. 
	// Justification is required only when downgrading the label. Justification examples will be shown in later samples.
	action.SetLabel(filePath, outputFileName, labelToApply);
	
	system("pause");
	cout << endl << endl;

	// Read the label from the labeled file. 
	action.ReadLabel(outputFileName);			
	
	system("pause");
		
	return 0;
}


