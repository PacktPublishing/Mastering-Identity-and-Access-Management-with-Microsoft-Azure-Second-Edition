#Implement log function
function logit($level, $message)
{
	switch($level)
	{
	        1 {$prefix = "INFO: "}        
		2 {$prefix = "WARNING: "}        
		3 {$prefix = "ERROR: "}        
		default {$prefix = "UNKNOWN"}
	}    write-host $prefix$message
}

#Connect to the Microsoft Online Service
Logit 1 "Getting Microsoft Azure Active Directory Credentials...."
Connect-MsolService

#Add some Organizational Groups
Logit 1 "Creating some Organizational Groups....."
New-MsolGroup -DisplayName "Sales" -Description "Sales"
New-MsolGroup -DisplayName "Accounting" -Description "Accounting"
New-MsolGroup -DisplayName "HR" -Description "Human Resources"