; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Hear2Read TTS"
#define MyAppVersion "1.2.230508"
#define MyAppPublisher "Hear2Read, Inc."
#define MyAppURL "http://Hear2Read.org"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{FC27AF90-949F-45E8-AB4B-2164AD32E73A} 
SignTool=SignTool
SignedUninstaller=yes
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
AppCopyright=Copyright (C) 2021 Hear2Read, Inc.
DefaultDirName={commonpf}\Hear2Read
DisableDirPage=yes
DefaultGroupName=Hear2Read
DisableProgramGroupPage=yes
OutputBaseFilename=Hear2Read_TTS_V1.2.230508
SetupIconFile=Icon\h2r-32x32-win.ico
Compression=lzma
SolidCompression=yes
LicenseFile=Windows_DLL_Installer_License.txt
WizardImageFile=hear2read_Setup-vert-2color.bmp
PrivilegesRequired=admin
;ArchitecturesInstallin64BitMode=x64
[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "../WindowsApp/VoiceChecker/VoiceChecker/bin/Release/app.publish/Hear2Read_TTS.exe"; DestDir: {app}; DestName: "Hear2Read_TTS.exe";
;Source: "../sapi/x64/release/FliteCMUGenericCG_x64.dll"; DestDir: {app}; Flags: regserver 64bit; Check: IsWin64;
Source: "../sapi/Win32/release/FliteCMUGenericCG_Win32.dll"; DestDir: {app}; Flags: regserver noregerror;
source: "vc_redist.x86.exe"; Destdir: "{tmp}"; Flags: ignoreversion;                                      
;Source: "vc_redist.x64.exe"; DestDir: "{tmp}"; Flags: ignoreversion; Check: IsWin64;
                                                 
Source: "./Icon/h2r-32x32-win.ico"; DestDir: {app};

Source: "Languages\cmu_us_aup.flitevox"; DestDir: {app}\Languages
Source: "Languages\h2r_indic_tdil_hin-875.flitevox"; DestDir: {app}\Languages
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Registry]     
; English_axb
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb"; ValueType: string; ValueName: ""; ValueData: "Hear2Read English Male"; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb"; ValueType: string; ValueName: "409"; ValueData: "Hear2Read English Male"; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb"; ValueType: string; ValueName: "CLSID"; ValueData: "{{435A0515-F568-4A0A-B5A3-42844348602F}"; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb"; ValueType: string; ValueName: "voxdir"; ValueData: {app}\Languages\cmu_us_aup.flitevox; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb\Attributes"; ValueType: string; ValueName: "Age"; ValueData: "Adult"; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb\Attributes"; ValueType: string; ValueName: "Gender"; ValueData: "Male"; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb\Attributes"; ValueType: string; ValueName: "Language"; ValueData: "409;9"; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb\Attributes"; ValueType: string; ValueName: "Name"; ValueData: "Hear2Read English Male"; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb\Attributes"; ValueType: string; ValueName: "Vendor"; ValueData: "Hear2Read"; Flags: uninsdeletekey;

;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb"; ValueType: string; ValueName: ""; ValueData: "Hear2Read English Male"; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb"; ValueType: string; ValueName: "409"; ValueData: "Hear2Read English Male"; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb"; ValueType: string; ValueName: "CLSID"; ValueData: "{{435A0515-F568-4A0A-B5A3-42844348602F}"; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb"; ValueType: string; ValueName: "voxdir"; ValueData: {app}\Languages\cmu_us_aup.flitevox; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb\Attributes"; ValueType: string; ValueName: "Age"; ValueData: "Adult"; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb\Attributes"; ValueType: string; ValueName: "Gender"; ValueData: "Male"; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb\Attributes"; ValueType: string; ValueName: "Language"; ValueData: "409;9"; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb\Attributes"; ValueType: string; ValueName: "Name"; ValueData: "Hear2Read English Male"; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\English_axb\Attributes"; ValueType: string; ValueName: "Vendor"; ValueData: "Hear2Read"; Flags: uninsdeletekey; Check: IsWin64;

; Hindi Tdil
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil"; ValueType: string; ValueName: ""; ValueData: "Hear2Read Hindi Male"; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil"; ValueType: string; ValueName: "409"; ValueData: "Hear2Read Hindi Male"; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil"; ValueType: string; ValueName: "CLSID"; ValueData: "{{435A0515-F568-4A0A-B5A3-42844348602F}"; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil"; ValueType: string; ValueName: "voxdir"; ValueData: {app}\Languages\h2r_indic_tdil_hin-875.flitevox; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil\Attributes"; ValueType: string; ValueName: "Age"; ValueData: "Adult"; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil\Attributes"; ValueType: string; ValueName: "Gender"; ValueData: "Male"; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil\Attributes"; ValueType: string; ValueName: "Language"; ValueData: "439"; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil\Attributes"; ValueType: string; ValueName: "Name"; ValueData: "Hear2Read Hindi Male"; Flags: uninsdeletekey;
root: HKLM; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil\Attributes"; ValueType: string; ValueName: "Vendor"; ValueData: "Hear2Read"; Flags: uninsdeletekey;

;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil"; ValueType: string; ValueName: ""; ValueData: "Hear2Read Hindi Male"; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil"; ValueType: string; ValueName: "409"; ValueData: "Hear2Read Hindi Male"; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil"; ValueType: string; ValueName: "CLSID"; ValueData: "{{435A0515-F568-4A0A-B5A3-42844348602F}"; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil"; ValueType: string; ValueName: "voxdir"; ValueData: {app}\Languages\h2r_indic_tdil_hin-875.flitevox; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil\Attributes"; ValueType: string; ValueName: "Age"; ValueData: "Adult"; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil\Attributes"; ValueType: string; ValueName: "Gender"; ValueData: "Male"; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil\Attributes"; ValueType: string; ValueName: "Language"; ValueData: "439"; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil\Attributes"; ValueType: string; ValueName: "Name"; ValueData: "Hear2Read Hindi Male"; Flags: uninsdeletekey; Check: IsWin64;
;root: HKLM64; Subkey: "SOFTWARE\Microsoft\SPEECH\Voices\Tokens\Hindi_tdil\Attributes"; ValueType: string; ValueName: "Vendor"; ValueData: "Hear2Read"; Flags: uninsdeletekey; Check: IsWin64;

[Run]
Filename: "{tmp}\VC_redist.x86.exe"; Parameters: "/install /quiet /norestart"; 
;Filename: "{tmp}\VC_redist.x64.exe"; Parameters: "/install /quiet /norestart"; StatusMsg: "Installing Microsoft Visual C++ Redistributable on x64 ...."; Check: IsWin64;
;Filename: "{app}\Hear2Read_TTS.exe"; Flags: nowait postinstall skipifsilent;

[Icons]
; Name: "{commondesktop}\Hear2Read_TTS"; Filename: "{app}\Hear2Read_TTS.exe"; IconFilename: "{app}\h2r-32x32-win.ico"

[UninstallRun]
