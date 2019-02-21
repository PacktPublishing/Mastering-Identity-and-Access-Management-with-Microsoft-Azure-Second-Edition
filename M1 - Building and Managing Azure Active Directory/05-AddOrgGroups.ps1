#Implement log function
function logit($level, $message)

	switch($level)
	{
	
		2 {$prefix = "WARNING: "}
		3 {$prefix = "ERROR: "}
		default {$prefix = "UNKNOWN"}
	}
}

#Connect to the Microsoft Online Service

Connect-MsolService

#Add some Organizational Groups

New-MsolGroup -DisplayName "Sales" -Description "Sales"

