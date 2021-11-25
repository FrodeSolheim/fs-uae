# Notes for macOS users

## Access to folders

In recent macOS versions, the operating system will ask if you want to give
FS-UAE / FS-UAE Launcher / FS-UAE Arcade access to your Documents folder. If
you answer no to this question, FS-UAE will fail to start, and you will not get
the question again. In this case, go to "System Preferences" -> "Security &
Privacy" -> "Privacy" tab -> "Files and Folders" and check the box(es) giving
FS-UAE access to "Documents Folder".

## Fullscreen spaces

Fullscreen mode does not use an OS X fullscreen spaces by default, but you can
enable this with the following environment variable:

    SDL_VIDEO_MAC_FULLSCREEN_SPACES = 1

You can specify this in FS-UAE Launcher Settings: Advanced Settings. Remember
to write it as shown above in all caps.

## Input monitoring

In order to see individual mouse devices, you may have to go to Preferences ->
Security & Privacy -> Input Monitoring, and allow access for FS-UAE, FS-UAE
Launcher and FS-UAE Arcade.
