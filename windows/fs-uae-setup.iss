[Setup]
AppName=FS-UAE
AppVersion=2.3.3
AppVerName=FS-UAE & FS-UAE Launcher 2.3.3
DefaultDirName={localappdata}\FS-UAE
DefaultGroupName=FS-UAE
UninstallDisplayIcon={app}\Emulator\FS-UAE.exe
OutputBaseFilename=fs-uae-suite_2.3.3_windows_setup
OutputDir=.
PrivilegesRequired=lowest
ShowLanguageDialog=no
DisableDirPage=yes
; DisableWelcomePage=yes
DisableReadyPage=yes
DisableStartupPrompt=yes
DisableProgramGroupPage=yes

[Files]
Source: "fs-uae-emulator_2.3.3_windows\*"; DestDir: "{app}\Emulator\"; Flags: recursesubdirs ignoreversion;
Source: "fs-uae-launcher_2.3.3_windows\*"; DestDir: "{app}\Launcher\"; Flags: recursesubdirs ignoreversion;
Source: "fs-uae-game-center_2.3.3_windows\*"; DestDir: "{app}\Game Center\"; Flags: recursesubdirs ignoreversion;

[installDelete]
Type: filesandordirs; Name: "{app}\Emulator"
Type: filesandordirs; Name: "{app}\Launcher"
Type: filesandordirs; Name: "{app}\Game Center"

Type: files; Name: "{userprograms}\FS-UAE\FS-UAE Emulator.lnk"
Type: files; Name: "{app}\CAPSImg.dll"
Type: files; Name: "{app}\COPYING.txt"
Type: files; Name: "{app}\example.conf"
Type: files; Name: "{app}\freetype6.dll"
Type: files; Name: "{app}\fs-uae.exe"
Type: files; Name: "{app}\fs-uae.log"
Type: files; Name: "{app}\libfreetype-6.dll"
Type: files; Name: "{app}\libgcc_s_dw2-1.dll"
Type: files; Name: "{app}\intl.dll"
Type: files; Name: "{app}\libglib-2.0-0.dll"
Type: files; Name: "{app}\libgthread-2.0-0.dll"
Type: files; Name: "{app}\libiconv-2.dll"
Type: files; Name: "{app}\libintl-8.dll"
Type: files; Name: "{app}\libpng14-14.dll"
Type: files; Name: "{app}\libpng15-15.dll"
Type: files; Name: "{app}\libstdc++-6.dll"
Type: files; Name: "{app}\OpenAL32.dll"
Type: files; Name: "{app}\README.txt"
Type: files; Name: "{app}\SDL.dll"
Type: files; Name: "{app}\stdout.txt"
Type: files; Name: "{app}\zlib1.dll"
Type: filesandordirs; Name: "{app}\share"
Type: filesandordirs; Name: "{app}\licenses"

[Icons]
;Name: "{group}\FS-UAE Emulator"; Filename: "{app}\FS-UAE.exe"
Name: "{group}\FS-UAE Launcher"; Filename: "{app}\Launcher\FS-UAE Launcher.exe"
Name: "{group}\FS-UAE Game Center"; Filename: "{app}\Game Center\FS-UAE Game Center.exe"

[Registry]
Root: HKCU; Subkey: "SOFTWARE\Classes\Applications\FS-UAE.exe\shell\open"; ValueType: string; ValueName: "FriendlyAppName"; ValueData: "FS-UAE Emulator"; Flags: uninsdeletekey

Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1"; ValueType: string; ValueName: ""; ValueData: "FS-UAE Config File"; Flags: uninsdeletekey
Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1"; ValueType: string; ValueName: "FriendlyTypeName"; ValueData: "FS-UAE Config File"; Flags: uninsdeletekey
Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1\shell\open"; ValueType: string; ValueName: "FriendlyAppName"; ValueData: "FS-UAE Emulator"; Flags: uninsdeletekey
Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\Emulator\FS-UAE.exe"" ""%1"""; Flags: uninsdeletekey
; Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\FS-UAE.exe,2"; Flags: uninsdeletekey

Root: HKCU; Subkey: "SOFTWARE\Classes\.fs-uae"; ValueType: string; ValueName: ""; ValueData: "Fengestad.FS-UAE.Config.1"; Flags: uninsdeletekey
Root: HKCU; Subkey: "SOFTWARE\Classes\.fs-uae\OpenWithProgids"; ValueType: binary; ValueName: "Fengestad.FS-UAE.Config.1"; ValueData: ""; Flags: uninsdeletekey
