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

#Set domain variable
Logit 1 "Gettting Domain Variable..."
$domain = Get-MsolDomain | where {$_.Name -notlike "*mail*"}

#Set configuration directory location
Logit 1 "Settting path to HR export file..."
$dir = "C:\Configuration\HRExports"

#CSV HR export file to import in Azure Active Directory
Logit 1 "Import Users and assign License...."
(get-content "$($dir)\NewHire.csv") | foreach-object {$_ -replace "contoso.com" , $domain.Name} | Set-Content "$($dir)\NewHire.csv"import-csv "$($dir)\NewHire.csv" | foreach {New-Msoluser -userPrincipalName $_.UserPrincipalName -displayname $_.displayname -firstname $_.firstname -lastname $_.lastname -password $_.Password -mobilephone $phone -forcechangepassword $false -usagelocation "CH" | set-msoluserlicense -addlicenses "$($domain.name -replace ".onmicrosoft.com" ,":EMSPREMIUM")","$($domain.name -replace ".onmicrosoft.com" , ":ENTERPRISEPREMIUM")"}
