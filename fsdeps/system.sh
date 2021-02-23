case "`uname`" in
    Linux*)  SYSTEM=Linux;;
    Darwin*) SYSTEM=macOS;;
    MINGW*)  SYSTEM=Windows;;
    *)       SYSTEM=Unknown;;
esac
