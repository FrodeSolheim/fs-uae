[Setup]
AppName=FS-UAE
AppVersion=9.8.7
AppVerName=FS-UAE 9.8.7
DefaultDirName={localappdata}\FS-UAE
DefaultGroupName=FS-UAE
UninstallDisplayIcon={app}\FS-UAE.exe
OutputBaseFilename=fs-uae-9.8.7-setup
OutputDir=.
PrivilegesRequired=lowest
ShowLanguageDialog=no
DisableDirPage=yes
; DisableWelcomePage=yes
DisableReadyPage=yes
DisableStartupPrompt=yes
DisableProgramGroupPage=yes

[Files]
Source: "fs-uae-9.8.7-windows\*"; DestDir: "{app}"; Flags: recursesubdirs replacesameversion;

[Icons]
Name: "{group}\FS-UAE Emulator"; Filename: "{app}\FS-UAE.exe"
Name: "{group}\FS-UAE Launcher"; Filename: "{app}\launcher\FS-UAE Launcher.exe"

[Registry]
Root: HKCU; Subkey: "SOFTWARE\Classes\Applications\FS-UAE.exe\shell\open"; ValueType: string; ValueName: "FriendlyAppName"; ValueData: "FS-UAE Emulator"; Flags: uninsdeletekey

Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1"; ValueType: string; ValueName: ""; ValueData: "FS-UAE Config File"; Flags: uninsdeletekey
Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1"; ValueType: string; ValueName: "FriendlyTypeName"; ValueData: "FS-UAE Config File"; Flags: uninsdeletekey
Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1\shell\open"; ValueType: string; ValueName: "FriendlyAppName"; ValueData: "FS-UAE Emulator"; Flags: uninsdeletekey
Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\FS-UAE.exe"" ""%1"""; Flags: uninsdeletekey
; Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\FS-UAE.exe,2"; Flags: uninsdeletekey

Root: HKCU; Subkey: "SOFTWARE\Classes\.fs-uae"; ValueType: string; ValueName: ""; ValueData: "Fengestad.FS-UAE.Config.1"; Flags: uninsdeletekey
Root: HKCU; Subkey: "SOFTWARE\Classes\.fs-uae\OpenWithProgids"; ValueType: binary; ValueName: "Fengestad.FS-UAE.Config.1"; ValueData: ""; Flags: uninsdeletekey
