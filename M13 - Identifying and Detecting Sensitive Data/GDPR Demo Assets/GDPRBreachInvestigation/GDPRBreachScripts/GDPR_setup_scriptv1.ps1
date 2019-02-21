
<#

This script has been developed for microsoftgdprscenarios.com.

The PowerShell cmdlets utilised within this script are sourced from:
Azure Active Directory v2.0 PowerShell Module - General Availability Release (2.0.0.115)    https://www.powershellgallery.com/packages/AzureAD/2.0.0.115
Office 365 Security and Compliance Center cmdlets                                           https://technet.microsoft.com/en-us/library/mt587093(v=exchg.160).aspx
Administering the Azure Rights Management service by using Windows PowerShell               https://docs.microsoft.com/en-us/information-protection/deploy-use/administer-powershell 
Exchange Online cmdlets                                                                     https://technet.microsoft.com/en-us/library/jj200780(v=exchg.160).aspx

#>

Write-Host "Collecting Azure AD tenant credentials and Microsoft Partner information" -ForegroundColor Yellow
Write-Host 

# Collects of the variables used throughout the script
Add-Type -AssemblyName Microsoft.VisualBasic
$GlobalAdminUsername = [Microsoft.VisualBasic.Interaction]::InputBox('Please enter the Tenant Global Administrator username', 'Global Admin username', "$env:UserName")
$GlobalAdminPassword = [Microsoft.VisualBasic.Interaction]::InputBox('Please enter the Tenant Global Adminstrator password', 'Global Admin password', "$env:UserName")
$MSPartnerCountry = [Microsoft.VisualBasic.Interaction]::InputBox('Please enter your Country', 'Country', "$env:UserName")
$MSPartnerOrganization = [Microsoft.VisualBasic.Interaction]::InputBox('Please enter your Microsoft Partner Organization Name', 'Microsoft Partner Organization', "$env:UserName")
$SecurePassword = Convertto-SecureString –String $GlobalAdminPassword –AsPlainText –force
$UserCredential = new-object -typename System.Management.Automation.PSCredential -argumentlist $GlobalAdminUsername, $SecurePassword

# Removes the Azure AD Tenant Domain from the Global Administrator username variable
$TenantDomain =$GlobalAdminUsername.Replace("admin@","")


#######################################################################################################################################################################################################################


Write-Host "Starting PowerShell session recording" -ForegroundColor Yellow
Write-Host 

Start-Transcript -path "gdprdeploymentoutput.txt" -Append 


#######################################################################################################################################################################################################################


Write-Host "Creating the RMS/AIP Templates" -ForegroundColor Yellow
Write-Host 


# Connects to the Azure AD RMS Service
Connect-AadrmService -Credential $UserCredential
 

#  Creation of the "Personal Data" RMS Template
$names = @{}
$names[1033] = "Personal Data"
$descriptions = @{}
$descriptions[1033] = "This data includes personally identifiable information (PII) that could cause damage to the business if shared with unauthorized people"
$r1 = New-AadrmRightsDefinition -EmailAddress hrcompensation@$TenantDomain -Rights "VIEW","PRINT","EXTRACT","REPLY","REPLYALL","FORWARD","OWNER","EDIT","OBJMODEL","VIEWRIGHTSDATA","EXPORT","EDITRIGHTSDATA","DOCEDIT"
Add-AadrmTemplate -Names $names -Descriptions $Descriptions -LicenseValidityDuration 7 -RightsDefinitions $r1 -Status Published


#  Creation of the "Health Information" RMS Template
$names = @{}
$names[1033] = "Health Information"
$descriptions = @{}
$descriptions[1033] = "This PII data contains Health Information that could cause damage to the business if shared with unauthorized people"
$r1 = New-AadrmRightsDefinition -EmailAddress hrcompensation@$TenantDomain -Rights "VIEW","PRINT","EXTRACT","REPLY","REPLYALL","FORWARD","OWNER","EDIT","OBJMODEL","VIEWRIGHTSDATA","EXPORT","EDITRIGHTSDATA","DOCEDIT"
Add-AadrmTemplate -Names $names -Descriptions $Descriptions -LicenseValidityDuration 7 -RightsDefinitions $r1 -Status Published

#  Creation of the "Salary Information" RMS Template
$names = @{}
$names[1033] = "Salary Information"
$descriptions = @{}
$descriptions[1033] = "This PII data contains sensative employee salary information that could cause damage to the business if shared with unauthorized people"
$r1 = New-AadrmRightsDefinition -EmailAddress hrcompensation@$TenantDomain -Rights "VIEW","PRINT","EXTRACT","REPLY","REPLYALL","FORWARD","OWNER","EDIT","OBJMODEL","VIEWRIGHTSDATA","EXPORT","EDITRIGHTSDATA","DOCEDIT"
Add-AadrmTemplate -Names $names -Descriptions $Descriptions -LicenseValidityDuration 7 -RightsDefinitions $r1 -Status Published

#  Creation of the "Contact Information" RMS Template
$names = @{}
$names[1033] = "Contact Information"
$descriptions = @{}
$descriptions[1033] = "This PII data contains contact information that could cause damage to the business if shared with unauthorized people"
Add-AadrmTemplate -Names $names -Descriptions $Descriptions -LicenseValidityDuration 7 -Status Published

#  Creation of the "Bank Account Information" RMS Template
$names = @{}
$names[1033] = "Bank Account Information"
$descriptions = @{}
$descriptions[1033] = "This PII data contains bank account information that could cause damage to the business if shared with unauthorized people"
$r1 = New-AadrmRightsDefinition -EmailAddress hrcompensation@$TenantDomain -Rights "VIEW","PRINT","EXTRACT","REPLY","REPLYALL","FORWARD","OWNER","EDIT","OBJMODEL","VIEWRIGHTSDATA","EXPORT","EDITRIGHTSDATA","DOCEDIT"
Add-AadrmTemplate -Names $names -Descriptions $Descriptions -LicenseValidityDuration 7 -RightsDefinitions $r1 -Status Published


#######################################################################################################################################################################################################################


Write-Host "Deleting Exchange Online DLP Policies" -ForegroundColor Yellow
Write-Host 


# Connects to the Exchange Online remote PowerShell
$Session = New-PSSession -ConfigurationName Microsoft.Exchange -ConnectionUri https://outlook.office365.com/powershell-liveid/ -Credential $UserCredential -Authentication Basic -AllowRedirection
Start-Sleep -seconds 10
Import-PSSession $Session
Start-Sleep -seconds 15


# Deletes the existing Exchange Online DLP policies
Remove-DlpPolicy -Identity "INOVITDEMOS Patent DLP" -Confirm:$false
Remove-DlpPolicy -Identity "PCI-CreditCard" -Confirm:$false


#######################################################################################################################################################################################################################


Write-Host "Updating Exchange Online Permissions" -ForegroundColor Yellow
Write-Host 


# Adds the Mailbox Import Export role to the global admin account
New-ManagementRoleAssignment –Role "Mailbox Import Export" –User "admin@$TenantDomain"


Write-Host "Mailbox Import Export Role Assignment" -ForegroundColor Yellow
Write-Host 

$loopnum = 0
$val = $false
$maxloops = 12
$secondstowait = 5
$user=$GlobalAdminUsername.Split("@")[0]
do{
    $loopnum ++
    write-host "Checking $loopnum of $maxloops"
    $roleexist = Get-ManagementRoleAssignment -Role "Mailbox Import Export"
    if ($roleexist.roleassignee -contains $user) {
        write-host "Permissions have now been applied for $GlobalAdminUsername"
        $val = $True
        } else {write-host "Applying permissions for $GlobalAdminUsername"}
        if ($loopnum -eq $maxloops){
            write-host "Max number of tries attempted, aborting."
            $val = $true
        }
    start-sleep $secondstowait
} while( $val -ne $true)

Add-RoleGroupMember "Organization Management" -Member "admin@$TenantDomain"

#######################################################################################################################################################################################################################


# Sends the email with Microsoft Partner name and Country
$EmailRecipient = "admin@MicrosoftGDPRScenarios.com"  
$EmailSender = $GlobalAdminUsername
$Date = Get-Date -format "dd/MM/yyyy HH:mm"
$EmailSubject = "$Date, $MSPartnerCountry, $MSPartnerOrganization"
$EmailBody = "A Microsoft Demo tenant has just started deploying the GDPR content pack at, $date <br> <br> The Country location is, $MSPartnerCountry <br> <br> The Microsoft Partner Organization is,  $MSPartnerOrganization" 
$smtpServer = "smtp.office365.com"
send-mailmessage -to $EmailRecipient -SmtpServer $smtpServer -Port "587" -Credential $UserCredential -UseSsl -subject "$EmailSubject" -body "$EmailBody" -from "$EmailSender" -BodyAsHtml


#######################################################################################################################################################################################################################


Write-Host "Enable Audit Log recording for User and Admin activites and tenant mailboxes" -ForegroundColor Yellow
Write-Host 


# Enables the Admin and User Audit logging
Set-AdminAuditLogConfig -UnifiedAuditLogIngestionEnabled $true

# Enables mailbox auditing on all accounts in the Tenant
Get-Mailbox -ResultSize Unlimited -Filter {RecipientTypeDetails -eq "UserMailbox"} | Set-Mailbox -AuditEnabled $true

# Configures the Owner actions to audit for all Exchange Online mailboxes, capturing the mailbox logins and Soft and Hard deletes
Get-Mailbox -ResultSize Unlimited -Filter {RecipientTypeDetails -eq "UserMailbox"} | Set-Mailbox -AuditOwner MailboxLogin,HardDelete,SoftDelete


#######################################################################################################################################################################################################################


Write-Host "Creating Advanced Threat Protection policies" -ForegroundColor Yellow
Write-Host 

# Creates the Safelinks Policy
New-SafeLinksPolicy -Name "INOVITDEMOS Safe Links Policy" -AdminDisplayName  "The INOVITDEMOS organization-wide Safe Links Policy" -DoNotAllowClickThrough $true -DoNotTrackUserClicks $false -Enabled $true -IsEnabled $true -ScanUrls $true -TrackClicks $true
# Creates the Safelinks Rule
New-SafeLinksRule -Name "INOVITDEMOS Safe Links Policy" -SafeLinksPolicy "INOVITDEMOS Safe Links Policy" -RecipientDomainIs $TenantDomain -Enabled $true
# Creates the SafeAttachments Policy
New-SafeAttachmentPolicy -Name "INOVITDEMOS Safe Attachments Policy" -Action Replace -ActionOnError $true -AdminDisplayName "The INOVITDEMOS organization-wide Safe Attachments Policy" -Redirect $true -RedirectAddress $GlobalAdminUsername -enable $true
# Creates the SafeAttachments Rule
New-SafeAttachmentRule -Name "INOVITDEMOS Safe Attachments Policy" -SafeattachmentPolicy "INOVITDEMOS Safe Attachments Policy" -RecipientDomainIs $TenantDomain -Enabled $true


#######################################################################################################################################################################################################################


Write-Host "Configuring Exchange Online IRM" -ForegroundColor Yellow
Write-Host 


# Specifies the location of the Azure Information Protection tenant key, according to according to where your organization's tenant was created. North America set with the Demos.Microsoft.com tenants
Set-IRMConfiguration -RMSOnlineKeySharingLocation "https://sp-rms.eu.aadrm.com/TenantManagement/ServicePartner.svc"

# Import configuration data from the Azure Rights Management service to Exchange Online, in the form of the trusted publishing domain (TPD). This includes the Azure Information Protection tenant key and Azure Rights Management templates:
Import-RMSTrustedPublishingDomain -RMSOnline -name "RMS Online"

# Enables the Azure Rights Management functionality so that IRM features are available for Exchange Online
Set-IRMConfiguration -InternalLicensingEnabled $true

# Refreshes the RMS templates for Exchange Online
Import-RMSTrustedPublishingDomain -Name "RMS Online - 1" -RefreshTemplates -RMSOnline


#######################################################################################################################################################################################################################


# Remove the Exchange Online remote PowerShell session 
remove-pssession -Session (Get-PSSession)

# Connects to Security and Compliance remote PowerShell session
$Session = New-PSSession -ConfigurationName Microsoft.Exchange -ConnectionUri https://ps.compliance.protection.outlook.com/powershell-liveid/ -Credential $UserCredential -Authentication Basic -AllowRedirection
Start-Sleep -seconds 5
Import-PSSession $Session
Start-Sleep -seconds 5


#######################################################################################################################################################################################################################


Write-Host "Creating Data Loss Protection policies" -ForegroundColor Yellow
Write-Host 


# Creates the Credit Card DLP Policy
New-DlpCompliancePolicy -Name "Credit Card Number Data" -Comment "DLP Compliance Policy for the protection of Credit Card Data" –ExchangeLocation All -SharePointLocation All -OneDriveLocation All -Mode Enable 

# Creates the HR Personal Data DLP Policy
New-DlpCompliancePolicy -Name "HR Personal Data" -Comment "DLP Compliance Policy for the protection of HR Personal Data" –ExchangeLocation All -SharePointLocation All -OneDriveLocation All -Mode Enable

# Creates the corresponding DLP rules for the DLP Credit Card Policy
New-DlpComplianceRule -Name "Low volume of Credit Card Number Data" -Policy "Credit Card Number Data" -AccessScope NotInOrganization -BlockAccess $true -Comment "DLP Compliance Rule for the protection of Credit Card Number Information" -ContentContainsSensitiveInformation @{Name="50842eb7-edc8-4019-85dd-5a5c1f2bb085"; minCount="1"; maxcount="4"} -GenerateIncidentReport SiteAdmin, $GlobalAdminUsername -IncidentReportContent All -NotifyAllowOverride withjustification -NotifyUser LastModifier, Owner, SiteAdmin, $GlobalAdminUsername -ReportSeverityLevel Low -Disabled $false
New-DlpComplianceRule -Name "High volume of Credit Card Number Data" -Policy "Credit Card Number Data" -AccessScope NotInOrganization -BlockAccess $true -Comment "DLP Compliance Rule for the protection of Credit Card Number Information" -ContentContainsSensitiveInformation @{Name="50842eb7-edc8-4019-85dd-5a5c1f2bb085"; minCount="5"} -GenerateIncidentReport SiteAdmin, $GlobalAdminUsername -IncidentReportContent All -NotifyAllowOverride withjustification -NotifyUser LastModifier, Owner, SiteAdmin, $GlobalAdminUsername -ReportSeverityLevel High -Disabled $false

# Creates the corresponding DLP rules for the HR Personal Data Policy
New-DlpComplianceRule -Name "HR Personal Data" -Policy "HR Personal Data" -AccessScope NotInOrganization -BlockAccess $true -Comment "DLP Compliance Rule for the protection of HR Personal Data" -ContentContainsSensitiveInformation @{Name="e7dc4711-11b7-4cb0-b88b-2c394a771f0e"; minCount="1"; minconfidence="85"; maxconfidence="100"}, @{Name="a2ce32a8-f935-4bb6-8e96-2a5157672e2c"; minCount="1"; minconfidence="75"; maxconfidence="100"}, @{Name="3192014e-2a16-44e9-aa69-4b20375c9a78";minCount="1"; minconfidence="85"; maxconfidence="100"}, @{Name="9a5445ad-406e-43eb-8bd7-cac17ab6d0e4"; minCount="1"; minconfidence="85"; maxconfidence="100"}, @{Name="16c07343-c26f-49d2-a987-3daf717e94cc"; minCount="1"; minconfidence="75"; maxconfidence="100"}, @{Name="2e3da144-d42b-47ed-b123-fbf78604e52c"; minCount="1"; minconfidence="75"; maxconfidence="100"}, @{Name="03937FB5-D2B6-4487-B61F-0F8BFF7C3517"; minCount="1"; minconfidence="85"; maxconfidence="100"}, @{Name="ba4e7456-55a9-4d89-9140-c33673553526"; minCount="1"; minconfidence="75"; maxconfidence="100"}, @{Name="178ec42a-18b4-47cc-85c7-d62c92fd67f8"; minCount="1"; minconfidence="75"; maxconfidence="100"}, @{Name="3008b884-8c8c-4cd8-a289-99f34fc7ff5d"; minCount="1"; minconfidence="75"; maxconfidence="100"}, @{Name="d1685ac3-1d3a-40f8-8198-32ef5669c7a5"; minCount="1"; minconfidence="75"; maxconfidence="100"}, @{Name="cb353f78-2b72-4c3c-8827-92ebe4f69fdf"; minCount="1"; minconfidence="75"; maxconfidence="100"}, @{Name="0e9b3178-9678-47dd-a509-37222ca96b42"; minCount="1"; minconfidence="85"; maxconfidence="100"} -GenerateIncidentReport SiteAdmin, $GlobalAdminUsername -IncidentReportContent All -NotifyAllowOverride withjustification -NotifyPolicyTipCustomText "This has been classified as Personal Data by HR." -NotifyUser LastModifier, Owner, SiteAdmin, $GlobalAdminUsername -ReportSeverityLevel High -Disabled $false


#######################################################################################################################################################################################################################


Write-Host "Creating Content Search entries" -ForegroundColor Yellow
Write-Host 

# Creates the Content Search Items - using the SensativeType searches only view SharePointOnline/OneDrive for Business
New-ComplianceSearch -Name "Credit Card Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"Credit Card Number`""
New-ComplianceSearch -Name "U.K. Drivers License Number Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"U.K. Driver's License Number`""
New-ComplianceSearch -Name "U.K. National Health Service Number Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"U.K. National Health Service Number`""
New-ComplianceSearch -Name "U.S. / U.K. Passport Number Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"U.S. / U.K. Passport Number`""
New-ComplianceSearch -Name "U.S. Bank Account Number Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"U.S. Bank Account Number`""
New-ComplianceSearch -Name "U.S. Drivers License Number Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"U.S. Driver's License Number`""
New-ComplianceSearch -Name "U.S. Social Security Number (SSN) Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"U.S. Social Security Number (SSN)`""
New-ComplianceSearch -Name "German Drivers License Number Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"German Driver's License Number`""
New-ComplianceSearch -Name "German Identity Card Number Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"German Identity Card Number`""
New-ComplianceSearch -Name "German Passport Number Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"German Passport Number`""
New-ComplianceSearch -Name "EU Debit Card Number Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"EU Debit Card Number`""
New-ComplianceSearch -Name "French Drivers License Number Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"France Driver's License Number`""
New-ComplianceSearch -Name "French National ID Card (CNI) Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"France National ID Card (CNI)`""
New-ComplianceSearch -Name "French Passport Number Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"France Passport Number`""
New-ComplianceSearch -Name "French Social Security Number (INSEE) Information Search" -SharePointLocation All -ContentMatchQuery "SensitiveType:`"France Social Security Number (INSEE)`""

# Starts each of the Content Search Items
Start-ComplianceSearch -identity "Credit Card Information Search"
Start-ComplianceSearch -identity "U.K. Drivers License Number Information Search"
Start-ComplianceSearch -identity "U.K. National Health Service Number Information Search"
Start-ComplianceSearch -identity "U.S. / U.K. Passport Number Information Search"
Start-ComplianceSearch -identity "U.S. Bank Account Number Information Search"
Start-ComplianceSearch -identity "U.S. Drivers License Number Information Search"
Start-ComplianceSearch -identity "U.S. Social Security Number (SSN) Information Search"
Start-ComplianceSearch -identity "German Drivers License Number Information Search"
Start-ComplianceSearch -identity "German Identity Card Number Information Search"
Start-ComplianceSearch -identity "German Passport Number Information Search"
Start-ComplianceSearch -identity "EU Debit Card Number Information Search"
Start-ComplianceSearch -identity "French Drivers License Number Information Search"
Start-ComplianceSearch -identity "French National ID Card (CNI) Information Search"
Start-ComplianceSearch -identity "French Passport Number Information Search"
Start-ComplianceSearch -identity "French Social Security Number (INSEE) Information Search"


#######################################################################################################################################################################################################################


Write-Host "Updating Security and Compliance permissions" -ForegroundColor Yellow
Write-Host 

# Updates eDiscovery Admin role for global admin 
update-eDiscoveryCaseAdmin -user admin@$TenantDomain

# Updates Organization Management role group membership
Add-RoleGroupMember "OrganizationManagement" -member Don.Hall@$TenantDomain
Add-RoleGroupMember "OrganizationManagement" -member Ellen.Adams@$TenantDomain
Add-RoleGroupMember "OrganizationManagement" -member Doris.Sutton@$TenantDomain
Add-RoleGroupMember "OrganizationManagement" -member Jeff.Simpson@$TenantDomain

# Adds the eDiscovery Admin roles
Add-eDiscoveryCaseAdmin -user Don.Hall@$TenantDomain
Add-eDiscoveryCaseAdmin -user Ellen.Adams@$TenantDomain
Add-eDiscoveryCaseAdmin -user Doris.Sutton@$TenantDomain
Add-eDiscoveryCaseAdmin -user Jeff.Simpson@$TenantDomain
Add-RoleGroupMember "eDiscoveryManager" -Member admin@$TenantDomain
Add-RoleGroupMember "eDiscoveryManager" -Member Don.Hall@$TenantDomain
Add-RoleGroupMember "eDiscoveryManager" -Member Ellen.Adams@$TenantDomain

# Updates the Supervisory Review role group membership
Add-RoleGroupMember "Supervisory Review" -member admin@$TenantDomain
Add-RoleGroupMember "Supervisory Review" -member Don.Hall@$TenantDomain
Add-RoleGroupMember "Supervisory Review" -member Ellen.Adams@$TenantDomain
Add-RoleGroupMember "Supervisory Review" -member Doris.Sutton@$TenantDomain
Add-RoleGroupMember "Supervisory Review" -member Jeff.Simpson@$TenantDomain

# Updates the Service Assurance role group membership
Add-RoleGroupMember "Service Assurance User" -member admin@$TenantDomain
Add-RoleGroupMember "Service Assurance User" -member Don.Hall@$TenantDomain
Add-RoleGroupMember "Service Assurance User" -member Ellen.Adams@$TenantDomain
Add-RoleGroupMember "Service Assurance User" -member Doris.Sutton@$TenantDomain
Add-RoleGroupMember "Service Assurance User" -member Jeff.Simpson@$TenantDomain

# Updates the Compliance Administrator role group membership
Add-RoleGroupMember "ComplianceAdministrator" -member admin@$TenantDomain
Add-RoleGroupMember "ComplianceAdministrator" -member Don.Hall@$TenantDomain
Add-RoleGroupMember "ComplianceAdministrator" -member Ellen.Adams@$TenantDomain
Add-RoleGroupMember "ComplianceAdministrator" -member Jeff.Simpson@$TenantDomain


#######################################################################################################################################################################################################################


Write-Host "Creating Compliance Cases" -ForegroundColor Yellow
Write-Host 

New-ComplianceCase -name "INOVITDEMOS Data Breach Investigation" 
New-ComplianceCase -name "INOVITDEMOS Health Information Search"


#######################################################################################################################################################################################################################


Write-Host "Assigning Compliance Case Member" -ForegroundColor Yellow
Write-Host 

Add-ComplianceCaseMember -Case "INOVITDEMOS Data Breach Investigation" -Member Ellen.Adams@$TenantDomain
Add-ComplianceCaseMember -Case "INOVITDEMOS Health Information Search" -Member Ellen.Adams@$TenantDomain

#######################################################################################################################################################################################################################


Write-Host "Creating Retention Policy" -ForegroundColor Yellow
Write-Host 

New-RetentionCompliancePolicy -Name "Compliance and Regulation Hold" -comment "This data should not be deleted due to compliance and regulatory requirements.  This policy is governed by the Legal and Compliance Team." -OneDriveLocation "All" -SharePointLocation "All" -ExchangeLocation "All" -ModernGroupLocation "All" -RestrictiveRetention $False -Enabled $True
New-RetentionComplianceRule -Name "Compliance and Regulation Hold" -Policy "Compliance and Regulation Hold" -RetentionComplianceAction Keep -RetentionDuration 2555 -ExpirationDateOption CreationAgeInDays
New-ComplianceTag -Name "Compliance and Regulation Hold Policy" -Comment "This data should not be deleted due to compliance and regulatory requirements.  This policy is governed by the Legal and Compliance Team." -RetentionAction Keep -RetentionDuration 2555 -RetentionType CreationAgeInDays
Set-RetentionComplianceRule -Identity "Compliance and Regulation Hold" -ApplyComplianceTag "Compliance and Regulation Hold Policy" -ContentMatchQuery "Financial Reporting OR Executive Board OR Regulatory Reporting OR AGM OR Composition with Creditors OR Shareholders' agreement OR Without prejudice OR Uberrima fides OR Arbitration" -RetentionComplianceAction Keep -RetentionDuration 2555 -ExpirationDateOption CreationAgeInDays


#######################################################################################################################################################################################################################


Write-Host "Updating Azure AD attributes and permissions" -ForegroundColor Yellow
Write-Host 

# Connects to Azure AD with the provided variables
Connect-AzureAD -Credential $UserCredential

# Updates the GDPR Personas with updated information
Set-AzureADUser -ObjectId Don.Hall@$TenantDomain -Jobtitle "Data Protection Officer" -Department "Auditing and Compliance Department"
Set-AzureADUser -ObjectId Ellen.Adams@$TenantDomain -Jobtitle "HR Special Investigations Officer" -Department "HR Special Investigations Department"
Set-AzureADUser -ObjectId Jeff.Simpson@$TenantDomain -Jobtitle "IT Support and Security Officer" -Department "Information Technology Department"
Set-AzureADUser -ObjectId Brian.Cox@$TenantDomain -Jobtitle "Sales Executive" -Department "Sales Department"
Set-AzureADUser -ObjectId Doris.Sutton@$TenantDomain -Jobtitle Lawyer -Department "Legal Department"

# Adds Jeff.Simpson to the Company Administrator (Global Admin) role
$roleMember = Get-AzureADUser -ObjectId "Jeff.Simpson@$tenantdomain"
$role = Get-AzureADDirectoryRole | Where-Object {$_.displayName -eq 'Company Administrator'}
Add-AzureADDirectoryRoleMember -ObjectId $role.ObjectId -RefObjectId $roleMember.ObjectId

# Adds Doris.Sutton to the Company Administrator (Global Admin) role for Secure Score acces
$roleMemberGrady = Get-AzureADUser -ObjectId "Doris.Sutton@$tenantdomain"
$roleGrady = Get-AzureADDirectoryRole | Where-Object {$_.displayName -eq 'Company Administrator'}
Add-AzureADDirectoryRoleMember -ObjectId $roleGrady.ObjectId -RefObjectId $roleMemberGrady.ObjectId


#######################################################################################################################################################################################################################


Write-Host "Creating MDM security group and assigning members" -ForegroundColor Yellow
Write-Host 

New-AzureADGroup -DisplayName "Mobile Salesforce" -Description "Mobile Salesforce" -MailEnabled $false -SecurityEnabled $true -MailNickName "NotSet"
$mobilesalesforceuser = Get-AzureADUser -ObjectId "Brian.Cox@$tenantdomain"
$mobilesalesforcegroup = Get-AzureADGroup | Where-Object {$_.Description -eq 'Mobile Salesforce'} 
Add-AzureADGroupMember -ObjectId $mobilesalesforcegroup.ObjectId -RefObjectId $mobilesalesforceuser.ObjectId


#######################################################################################################################################################################################################################


Write-Host "The Deployment Script has finished" -ForegroundColor Yellow
Write-Host

# Removes the remote PowerShell session for Security and Compliance
remove-pssession -Session (Get-PSSession)

Stop-Transcript 