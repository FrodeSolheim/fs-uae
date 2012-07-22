[Setup]
AppName=FS-UAE
AppVersion=1.3.19
DefaultDirName={localappdata}\FS-UAE
DefaultGroupName=FS-UAE
UninstallDisplayIcon={app}\FS-UAE.exe
OutputBaseFilename=fs-uae-1.3.19-setup
OutputDir=.
PrivilegesRequired=lowest
ShowLanguageDialog=no
DisableDirPage=yes
;DisableWelcomePage=yes
DisableReadyPage=yes
DisableStartupPrompt=yes
DisableProgramGroupPage=yes

[Files]
Source: "fs-uae-1.3.19-windows\*"; DestDir: "{app}"; Flags: recursesubdirs;
;Source: "oalinst.exe"; DestDir: "{app}";
;Source: "vcredist_x86.exe"; DestDir: "{app}";

[Icons]
Name: "{group}\FS-UAE Emulator"; Filename: "{app}\FS-UAE.exe"
Name: "{group}\FS-UAE Launcher"; Filename: "{app}\launcher\FS-UAE Launcher.exe"

[Run]
;Filename: "{app}\oalinst.exe"; Parameters: "/s"
;Filename: "{app}\vcredist_x86.exe"; Parameters: "/q"
