# This installs two files, app.exe and logo.ico, creates a start menu shortcut, builds an uninstaller, and
# adds uninstall information to the registry for Add/Remove Programs
 
# To get started, put this script into a folder with the two files (app.exe, logo.ico, and license.rtf -
# You'll have to create these yourself) and run makensis on it
 
# If you change the names "app.exe", "logo.ico", or "license.rtf" you should do a search and replace - they
# show up in a few places.
# All the other settings can be tweaked by editing the !defines at the top of this script
!define APPNAME "QMentat"
!define COMPANYNAME "Michel Dusseault"
!define DESCRIPTION "Mental Arithmetic Trainer"
# These three must be integers
!define VERSIONMAJOR 0
!define VERSIONMINOR 9
!define VERSIONBUILD 0
# These will be displayed by the "Click here for support information" link in "Add/Remove Programs"
# It is possible to use "mailto:" links in here to open the email client
!define HELPURL "http://realgrep.github.io/QMentat/" # "Support Information" link
#!define UPDATEURL "http://..." # "Product Updates" link
!define ABOUTURL "http://realgrep.github.io/QMentat/" # "Publisher" link
# This is the size (in kB) of all the files copied into "Program Files"
# QMentat.ico:			67
# bin/QMentat.exe:		393
#"share\QMentat\LICENSE":	33
#"share\QMentat\QMentat.png": 	16
#"share\QMentat\QMentat.qch": 	25
#"share\QMentat\QMentat.qhc": 	40
#"bin\mpir.dll": 		437
#"bin\QtCLucene4.dll": 		921
#"bin\QtCore4.dll": 		2,542
#"bin\QtGui4.dll": 		8,375
#"bin\QtHelp4.dll": 		424
#"bin\QtNetwork4.dll": 		1,028
#"bin\QtSql4.dll": 		197
#                      Total:   14,498
!define INSTALLSIZE 14498
 
RequestExecutionLevel admin ;Require admin rights on NT6+ (When UAC is turned on)
 
InstallDir "$PROGRAMFILES\${APPNAME}\${APPNAME}"
 
# rtf or txt file - remember if it is txt, it must be in the DOS text format (\r\n)
#LicenseData "license.rtf"
# This will be in the installer/uninstaller's title bar
#Name "${COMPANYNAME} - ${APPNAME}"
Name "${APPNAME}"
Icon "QMentat.ico"
outFile "QMentat-installer_${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}.exe"
 
!include LogicLib.nsh
 
# Just three pages - license agreement, install location, and installation
#page license
page directory
Page instfiles
 
!macro VerifyUserIsAdmin
UserInfo::GetAccountType
pop $0
${If} $0 != "admin" ;Require admin rights on NT4+
        messageBox mb_iconstop "Administrator rights required!"
        setErrorLevel 740 ;ERROR_ELEVATION_REQUIRED
        quit
${EndIf}
!macroend
 
function .onInit
	setShellVarContext all
	!insertmacro VerifyUserIsAdmin
functionEnd
 
section "install"
	# Files for the install directory - to build the installer, these should be in the same directory as the install script (this file)
	setOutPath $INSTDIR
	# Files added here should be removed by the uninstaller (see section "uninstall")
	file "QMentat.ico"
	setOutPath $INSTDIR\bin
	file "bin\QMentat.exe"
	file "bin\mpir.dll"
	file "bin\QtCLucene4.dll"
	file "bin\QtCore4.dll"
	file "bin\QtGui4.dll"
	file "bin\QtHelp4.dll"
	file "bin\QtNetwork4.dll"
	file "bin\QtSql4.dll"


	setOutPath $INSTDIR\share\QMentat
	file "share\QMentat\LICENSE"
	file "share\QMentat\QMentat.png"
	file "share\QMentat\QMentat.qch"
	file "share\QMentat\QMentat.qhc"
 
	# Uninstaller - See function un.onInit and section "uninstall" for configuration
	writeUninstaller "$INSTDIR\uninstall.exe"
 
	# Start Menu
	createDirectory "$SMPROGRAMS\${APPNAME}"
	createShortCut "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\bin\QMentat.exe" "" "$INSTDIR\QMentat.ico"
 
	# Registry information for add/remove programs
#	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayName" "${COMPANYNAME} - ${APPNAME} - ${DESCRIPTION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayName" "${APPNAME} - ${DESCRIPTION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayIcon" "$\"$INSTDIR\QMentat.ico$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "Publisher" "$\"${COMPANYNAME}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "HelpLink" "$\"${HELPURL}$\""
#	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "URLUpdateInfo" "$\"${UPDATEURL}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "URLInfoAbout" "$\"${ABOUTURL}$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayVersion" "$\"${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}$\""
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "VersionMajor" ${VERSIONMAJOR}
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "VersionMinor" ${VERSIONMINOR}
	# There is no option for modifying or repairing the install
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoRepair" 1
	# Set the INSTALLSIZE constant (!defined at the top of this script) so Add/Remove Programs can accurately report the size
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "EstimatedSize" ${INSTALLSIZE}
sectionEnd
 
# Uninstaller
 
function un.onInit
	SetShellVarContext all
 
	#Verify the uninstaller - last chance to back out
	MessageBox MB_OKCANCEL "Permanantly remove ${APPNAME}?" IDOK next
		Abort
	next:
	!insertmacro VerifyUserIsAdmin
functionEnd
 
section "uninstall"
 
	# Remove Start Menu launcher
	delete "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk"
	# Try to remove the Start Menu folder - this will only happen if it is empty
	rmDir "$SMPROGRAMS\${APPNAME}"
 
	# Remove files
	delete $INSTDIR\QMentat.ico
	delete $INSTDIR\bin\QMentat.exe
	delete $INSTDIR\bin\mpir.dll
	delete $INSTDIR\bin\QtCLucene4.dll
	delete $INSTDIR\bin\QtCore4.dll
	delete $INSTDIR\bin\QtGui4.dll
	delete $INSTDIR\bin\QtHelp4.dll
	delete $INSTDIR\bin\QtNetwork4.dll
	delete $INSTDIR\bin\QtSql4.dll
	delete $INSTDIR\share\QMentat\LICENSE
	delete $INSTDIR\share\QMentat\QMentat.png
	delete $INSTDIR\share\QMentat\QMentat.qch
	delete $INSTDIR\share\QMentat\QMentat.qhc
	rmDir "$INSTDIR\bin"
	rmDir "$INSTDIR\share\QMentat"
	rmDir "$INSTDIR\share"
 
	# Always delete uninstaller as the last action
	delete $INSTDIR\uninstall.exe
 
	# Try to remove the install directory - this will only happen if it is empty

	rmDir $INSTDIR
 
	# Remove uninstaller information from the registry
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}"
sectionEnd
