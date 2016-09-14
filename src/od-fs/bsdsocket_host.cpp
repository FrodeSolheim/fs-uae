#ifdef _WIN32
#include "../od-win32/bsdsock.cpp"
HWND hAmigaWnd = 0;
#else
#include "bsdsocket_posix.cpp"
#endif
