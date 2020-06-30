; Script generated by the Inno Script Studio Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "ImgArchive"
#define MyAppVersion "0.9"
#define MyAppPublisher "IDK Software Ltd"
#define MyAppURL "http://www.idk-software.com/"
#define MyAppExeName "iaarc.exe"
#define AppConfig "{commonappdata}\IDK-Software\ImgArchive\config"
#define AppHooks "{commonappdata}\IDK-Software\ImgArchive\hooks"
#define AppHome "{commonappdata}\IDK-Software\ImgArchive"
#define ReleasePath "C:\development\imgarch_root\ImgArchive\Release"
#define ReleasePath64 "C:\development\imgarch_root\ImgArchive\x64\Release"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{82C69D0D-39D7-4B73-8079-0A119C825290}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
; Changes Environment 
;    ChangesEnvironment=true
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\IDK-Software\imgarchive
DefaultGroupName={#MyAppName}
LicenseFile={#ReleasePath}\licence.txt
OutputDir=C:\development\imgarch_root\ImgArchive\Release
OutputBaseFilename=iasetup
SetupIconFile={#ReleasePath}\app.ico
Compression=lzma
SolidCompression=yes
; "ArchitecturesInstallIn64BitMode=x64" requests that the install be
; done in "64-bit mode" on x64, meaning it should use the native
; 64-bit Program Files directory and the 64-bit view of the registry.
; On all other architectures it will install in "32-bit mode".
ArchitecturesInstallIn64BitMode=x64
; Note: We don't set ProcessorsAllowed because we want this
; installation to run on all architectures (including Itanium,
; since it's capable of running 32-bit code too).


[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: modifypath; Description:{cm:AppAddPath}; 
[Files]
; Install MyProg-x64.exe if running in 64-bit mode (x64; see above),
; MyProg.exe otherwise.
Source: {#ReleasePath64}\iaarc.exe; DestDir: "{app}"; Flags: ignoreversion; Check: Is64BitInstallMode
Source: {#ReleasePath64}\iaadmin.exe; DestDir: "{app}"; Flags: ignoreversion; Check: Is64BitInstallMode
Source: {#ReleasePath}\iaarc.exe; DestDir: "{app}"; Flags: ignoreversion; Check: not Is64BitInstallMode
Source: {#ReleasePath}\iaadmin.exe; DestDir: "{app}"; Flags: ignoreversion; Check: not Is64BitInstallMode
;Source: {#ReleasePath}\dlls_x86\msvcp120.dll; DestDir: "{app}"; Flags: ignoreversion
;Source: {#ReleasePath}\dlls_x86\msvcr120.dll; DestDir: "{app}"; Flags: ignoreversion

; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: "defaultFiles\config\config.dat"; DestDir: "{#AppConfig}"; DestName: "config.dat"
Source: "defaultFiles\config\ext.dat"; DestDir: "{#AppConfig}"; DestName: "ext.dat"
Source: "defaultFiles\config\mirror.dat"; DestDir: "{#AppConfig}"; DestName: "mirror.dat"
Source: "defaultFiles\config\view.dat"; DestDir: "{#AppConfig}"; DestName: "view.dat"
Source: "defaultFiles\README.TXT"; DestDir: "{app}"; DestName: "README.TXT"

Source: "defaultFiles\hooks\view-preview1.py"; DestDir: "{#AppHooks}"; DestName: "view-preview1.py"
Source: "defaultFiles\hooks\view-preview2.py"; DestDir: "{#AppHooks}"; DestName: "view-preview2.py"
Source: "defaultFiles\hooks\view-preview3.py"; DestDir: "{#AppHooks}"; DestName: "view-preview3.py"
Source: "defaultFiles\hooks\view-thumbnail.py"; DestDir: "{#AppHooks}"; DestName: "view-thumbnail.py"

[Icons]
;Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
;Name: "{group}\"; Filename: "{%SystemRoot}\System32\cmd.exe"
;Name: "{commondesktop}\{#MyAppName}"; Filename: "cmd.exe {app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
; Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
Filename: "{app}\README.TXT"; Description: "View the README file"; Flags: postinstall shellexec skipifsilent

[Dirs]
Name: "{#AppHome}\logs"
Name: "{#AppHome}\dups"
Name: "{#AppHome}\history"
Name: "{#AppHome}\hooks"
Name: "{#AppHome}\sqldb"
Name: "{#AppHome}\tools"
Name: "{#AppHome}\templates"
Name: "{#AppHome}\backups" 
Name: "{#AppHome}\derivative"
Name: "{#AppHome}\master"
Name: "{#AppHome}\pi"
Name: "{#AppHome}\web"
Name: "{#AppHome}\tmp"
Name: "{#AppHome}\web"
Name: "{%USERPROFILE}\ImgArchive\Workspace"
Name: "{%USERPROFILE}\ImgArchive\Pictures"

[Registry]
Root: "HKLM"; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}"; 
Root: "HKLM"; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: string; ValueName: "IMGARCHIVE_HOME"; ValueData: "{#AppHome}"; Flags: createvalueifdoesntexist preservestringtype
Root: "HKLM"; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: string; ValueName: "IMGA_WORKSPACE"; ValueData: "{%USERPROFILE}\ImgArchive\Workspace"; Flags: createvalueifdoesntexist preservestringtype
Root: "HKLM"; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: string; ValueName: "IMGA_PICTURES"; ValueData: "{%USERPROFILE}\ImgArchive\Pictures"; Flags: createvalueifdoesntexist preservestringtype

   
[CustomMessages]
AppAddPath=Add application directory to your environmental path (required)

;[Code]
;const
;    ModPathName = 'modifypath';
;    ModPathType = 'system';

;function ModPathDir(): TArrayOfString;
;begin
;    setArrayLength(Result, 1)
;    Result[0] := ExpandConstant('{app}');
;end;
;#include "modpath.iss"
