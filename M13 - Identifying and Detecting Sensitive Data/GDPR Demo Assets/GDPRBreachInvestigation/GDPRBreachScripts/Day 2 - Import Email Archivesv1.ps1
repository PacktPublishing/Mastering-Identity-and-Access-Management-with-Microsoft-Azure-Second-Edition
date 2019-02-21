<#

This script has been developed for microsoftgdprscenarios.com.

The PowerShell cmdlets utilised within this script are sourced from:

Exchange Online cmdlets         https://technet.microsoft.com/en-us/library/jj200780(v=exchg.160).aspx

#>

# Collects the variables required to import the mail archives
Add-Type -AssemblyName Microsoft.VisualBasic
$GlobalAdminUsername = [Microsoft.VisualBasic.Interaction]::InputBox('Enter the Global Admin username', 'Global Admin username', "$env:UserName")
$GlobalAdminPassword = [Microsoft.VisualBasic.Interaction]::InputBox('Enter your Global Admins password', 'Global Admin password', "$env:UserName")
$SecurePassword = Convertto-SecureString –String $GlobalAdminPassword –AsPlainText –force
$UserCredential = new-object -typename System.Management.Automation.PSCredential -argumentlist $GlobalAdminUsername, $SecurePassword

# Strips the Azure AD Tenant Domain from the provided Global Administrator username variable
$TenantDomain = $GlobalAdminUsername.Replace("admin@","")

# Connects to the Azure AD Tenant (via Exchange Online PowerShell) with the provided variables
$Session = New-PSSession -ConfigurationName Microsoft.Exchange -ConnectionUri https://outlook.office365.com/powershell-liveid/ -Credential $UserCredential -Authentication Basic -AllowRedirection
Import-PSSession $Session

Write-Host "Importing demo mailbox data" -ForegroundColor Yellow
Write-Host 

# Imports the Demo PST content
New-MailboxImportRequest -Name "Admin Archive Import" -Mailbox "admin@$tenantdomain" -TargetRootFolder "/" -AzureBlobStorageAccountUri "https://gdprscenario.blob.core.windows.net/gdprscenario/Standard_PST.pst" -AzureSharedAccessSignatureToken "?sv=2016-05-31&ss=b&srt=co&sp=rl&se=2020-08-01T20:08:26Z&st=2017-06-01T12:08:26Z&spr=https&sig=lFq30GQ79c2CHtVr1jADUqN7l3UhvRWUmrJvGi83c9g%3D"
New-MailboxImportRequest -Name "Ellen.Adams Archive Import" -Mailbox "Ellen.Adams@$tenantdomain" -TargetRootFolder "/" -AzureBlobStorageAccountUri "https://gdprscenario.blob.core.windows.net/gdprscenario/Standard_PST.pst" -AzureSharedAccessSignatureToken "?sv=2016-05-31&ss=b&srt=co&sp=rl&se=2020-08-01T20:08:26Z&st=2017-06-01T12:08:26Z&spr=https&sig=lFq30GQ79c2CHtVr1jADUqN7l3UhvRWUmrJvGi83c9g%3D"
New-MailboxImportRequest -Name "Jeff.Simpson Archive Import" -Mailbox "Jeff.Simpson@$tenantdomain" -TargetRootFolder "/" -AzureBlobStorageAccountUri "https://gdprscenario.blob.core.windows.net/gdprscenario/Standard_PST.pst" -AzureSharedAccessSignatureToken "?sv=2016-05-31&ss=b&srt=co&sp=rl&se=2020-08-01T20:08:26Z&st=2017-06-01T12:08:26Z&spr=https&sig=lFq30GQ79c2CHtVr1jADUqN7l3UhvRWUmrJvGi83c9g%3D"
New-MailboxImportRequest -Name "Brian.Cox Archive Import" -Mailbox "Brian.Cox@$tenantdomain" -TargetRootFolder "/" -AzureBlobStorageAccountUri "https://gdprscenario.blob.core.windows.net/gdprscenario/LynneR_PST.pst" -AzureSharedAccessSignatureToken "?sv=2016-05-31&ss=b&srt=co&sp=rl&se=2020-08-01T20:08:26Z&st=2017-06-01T12:08:26Z&spr=https&sig=lFq30GQ79c2CHtVr1jADUqN7l3UhvRWUmrJvGi83c9g%3D"
New-MailboxImportRequest -Name "Doris.Sutton Archive Import" -Mailbox "Doris.Sutton@$tenantdomain" -TargetRootFolder "/" -AzureBlobStorageAccountUri "https://gdprscenario.blob.core.windows.net/gdprscenario/Standard_PST.pst" -AzureSharedAccessSignatureToken "?sv=2016-05-31&ss=b&srt=co&sp=rl&se=2020-08-01T20:08:26Z&st=2017-06-01T12:08:26Z&spr=https&sig=lFq30GQ79c2CHtVr1jADUqN7l3UhvRWUmrJvGi83c9g%3D"
New-MailboxImportRequest -Name "Don.Hall Archive Import" -Mailbox "Don.Hall@$tenantdomain" -TargetRootFolder "/" -AzureBlobStorageAccountUri "https://gdprscenario.blob.core.windows.net/gdprscenario/Standard_PST.pst" -AzureSharedAccessSignatureToken "?sv=2016-05-31&ss=b&srt=co&sp=rl&se=2020-08-01T20:08:26Z&st=2017-06-01T12:08:26Z&spr=https&sig=lFq30GQ79c2CHtVr1jADUqN7l3UhvRWUmrJvGi83c9g%3D"


# Removes the remote PowerShell session for Exchange Online
remove-pssession -Session (Get-PSSession)