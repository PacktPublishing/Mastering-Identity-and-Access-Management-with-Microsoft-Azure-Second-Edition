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

#include "action.h"

#include "mip/common_types.h"
#include "mip/file/file_profile.h"
#include "mip/file/file_engine.h"
#include "mip/file/file_handler.h"
#include "mip/file/labeling_options.h"

#include "auth_delegate_impl.h"
#include "consent_delegate_impl.h"
#include "file_profile_observer_impl.h"
#include "file_handler_observer_impl.h"
#include "utils.h"

#include <iostream>
#include <future>

using std::cout;
using std::endl;

using mip::FileProfile;
using mip::FileEngine;
using mip::FileHandler;

namespace sample {
	namespace file {

		// Constructor accepts mip::ApplicationInfo object and uses it to initialize AuthDelegateImpl.
		// Specifically, AuthDelegateInfo uses mAppInfo.ApplicationId for AAD client_id value.		
		Action::Action(const mip::ApplicationInfo appInfo,
			const std::string& username,
			const std::string& password)
			: mAppInfo(appInfo),
			mUsername(username),
			mPassword(password) {
			mAuthDelegate = std::make_shared<sample::auth::AuthDelegateImpl>(mAppInfo, mUsername, mPassword);			
		}	

		// Method illustrates how to create a new mip::FileProfile using promise/future
		// Result is stored in private mProfile variable and referenced throughout lifetime of Action.
		void sample::file::Action::AddNewFileProfile()
		{
			// Initialize the FileProfile::Settings Object. Example below stores state data in /file_sample/ directory 
			// Accepts AuthDelegate, new ConsentDelegate, new FileProfile::Observer, and ApplicationInfo object as last parameters.
			FileProfile::Settings profileSettings("file_sample", true, mAuthDelegate, std::make_shared<sample::consent::ConsentDelegateImpl>(), std::make_shared<FileProfileObserver>(), mAppInfo);
			
			// Create promise and future for mip::FileProfile object.
			auto profilePromise = std::make_shared<std::promise<std::shared_ptr<FileProfile>>>();
			auto profileFuture = profilePromise->get_future();

			// Call static function LoadAsync providing the settings and promise. This will make the profile available to use.
			FileProfile::LoadAsync(profileSettings, profilePromise);
			
			// Get the future value and store in mProfile. mProfile is used throughout Action for profile operations.
			mProfile = profileFuture.get();
		}

		// Action::AddNewFileEngine adds an engine for a specific user. 		
		void Action::AddNewFileEngine()
		{
			// If mProfile hasn't been set, use AddNewFileProfile() to set it.
			if (!mProfile)
			{
				AddNewFileProfile();
			}			

			// FileEngine requires a FileEngine::Settings object. The first parameter is the user identity or engine ID. 
			FileEngine::Settings engineSettings(mip::Identity(mUsername), "");

			// Create promise and future for mip::FileEngine object
			auto enginePromise = std::make_shared<std::promise<std::shared_ptr<FileEngine>>>();
			auto engineFuture = enginePromise->get_future();

			// Engines are added to profiles. Call AddEngineAsync on mProfile, providing settings and promise
			// then get the future value and set in mEngine. mEngine will be used throughout Action for engine operations.
			mProfile->AddEngineAsync(engineSettings, enginePromise);
			mEngine = engineFuture.get();
		}

		// Creates a mip::FileHandler and returns to the caller. 
		// FileHandlers obtain a handle to a specific file, then perform any File API operations on the file.
		std::shared_ptr<mip::FileHandler> Action::CreateFileHandler(const std::string& filepath)
		{
			// Create promise/future for mip::FileHandler
			auto handlerPromise = std::make_shared<std::promise<std::shared_ptr<FileHandler>>>();
			auto handlerFuture = handlerPromise->get_future();

			// Use mEngine::CreateFileHandlerAsync to create the handler
			// Filepath, the mip::FileHandler::Observer implementation, and the promise are required. 
			// Event notification will be provided to the appropriate function in the observer.
			mEngine->CreateFileHandlerAsync(filepath, filepath, mip::ContentState::REST, false, std::static_pointer_cast<FileHandler::Observer>(std::make_shared<FileHandlerObserver>()), handlerPromise);

			// Get the value and store in a mip::FileHandler object.
			// auto resolves to std::shared_ptr<mip::FileHandler>
			auto handler = handlerFuture.get();

			// return the pointer to mip::FileHandler to the caller
			return handler;
		}


		// Function recursively lists all labels available for a user to	std::cout.
		void Action::ListLabels() {

			// If mEngine hasn't been set, call AddNewFileEngine() to load the engine.
			if (!mEngine) {			
				AddNewFileEngine();
			}

			// Use mip::FileEngine to list all labels
			auto labels = mEngine->ListSensitivityLabels();

			// Iterate through each label, first listing details
			for (const auto& label : labels) {
				cout << label->GetName() << " : " << label->GetId() << endl;
				
				// get all children for mip::Label and list details
				for (const auto& child : label->GetChildren()) {
					cout << "->  " << child->GetName() << " : " << child->GetId() << endl;
				}
			}
		}

		// Reads a label from the file at filepath, the displays.
		// Reading a label from a protected file will trigger consent flow, as implemented in mip::ConsentDelegate or derived classes.
		// In this sample, simple consent flow is implemented in consent_delegate_impl.h/cpp.
		void Action::ReadLabel(const std::string & filepath)
		{
			// Call private CreateFileHandler function, passing in file path. 
			// Returns a std::shared_ptr<mip::FileHandler> that will be used to read the label.
			auto handler = CreateFileHandler(filepath);
			
			// call mip::FileHandler::GetLabelAsync, passing in the promise.
			// The handler has the rest of the details it needs (file path and policy data via FileEngine) to display result.
			// handler->GetLabelAsync(labelPromise);
			auto label = handler->GetLabel();
			
			// Output results
			cout << "Name: " + label->GetLabel()->GetName() << endl;
			cout << "Id: " + label->GetLabel()->GetId() << endl;			
		}


		// Implements the code to assign a label to a file
		// Creates a file handler for filepath, sets the label with labelId, and writes the result to outputfile
		void Action::SetLabel(const std::string & filepath, const std::string& outputfile, const std::string & labelId)
		{
			// Call private CreateFileHandler function, passing in file path. 
			// Returns a std::shared_ptr<mip::FileHandler> that will be used to read the label.
			auto handler = CreateFileHandler(filepath);

			// Labeling requires a mip::LabelingOptions object. 
			// Review API ref for more details. The sample implies that the file was labeled manually by a user.
			mip::LabelingOptions labelingOptions(mip::AssignmentMethod::PRIVILEGED, mip::ActionSource::MANUAL);
			
			// use the mip::FileHandler to set label with labelId and labelOptions created above
			handler->SetLabel(labelId, labelingOptions);

			// Changes to the file held by mip::FileHandler aren't committed until CommitAsync is called.						
			// Call Action::CommitChanges to write changes. Commit logic is implemented there.
			bool result = CommitChanges(handler, outputfile);
			
			// Write result to console.
			if (result) {
				cout << "Labeled: " + outputfile << endl;
			}
			else {
				cout << "Failed to label: " + outputfile << endl;
			}
		}
	
		// Implements code to commit changes made via mip::FileHandler
		// Accepts pointer to the mip::FileHandler and output file path
		bool Action::CommitChanges(const std::shared_ptr<mip::FileHandler>& fileHandler, const std::string& outputFile)
		{
			// CommitAsync is implemented similar to other async patterns via promise/future
			// In this instance, rather than a mip related object, we create the promise for a bool
			// The result provided will be true if the file was written, false if it failed.
			auto commitPromise = std::make_shared<std::promise<bool>>();
			auto commitFuture = commitPromise->get_future();
			
			// Commit changes to file referenced by fileHandler, writing to output file.
			fileHandler->CommitAsync(outputFile, commitPromise);

			// Get value from future and return to caller. Will be true if operation succeeded, false otherwise.
			return commitFuture.get(); 
		}				
	}
}
