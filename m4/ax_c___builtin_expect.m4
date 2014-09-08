#####
#
# SYNOPSIS
#
#   AX_C___BUILTIN_EXPECT
#
# DESCRIPTION
#
#   AX_C___BUILTIN_EXPECT checks if the compiler allows __builtin_expect()
#   Defines HAVE___BUILTIN_EXPECT if it is allowed.
#
# LAST MODIFICATION
#
#   2008-02-29
#
# COPYLEFT
#
#   Copyright (c) 2008 Francesco Salvestrini <salvestrini@users.sourceforge.net>
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.

AC_DEFUN([AX_C___BUILTIN_EXPECT], [
  AC_MSG_CHECKING(if compiler allows __builtin_expect)

  AC_CACHE_VAL(ax_cv_c___builtin_expect,[
    AC_LANG_PUSH([C])
    AC_COMPILE_IFELSE([
      AC_LANG_PROGRAM([
      ],[
        if (__builtin_expect(1,1)) { }
      ])
    ],[
        ax_cv_c___builtin_expect=yes
    ],[
        ax_cv_c___builtin_expect=no
    ])
    AC_LANG_POP
  ])

  AS_IF([test "x$ax_cv_c___builtin_expect" != "xno"],[
    AC_DEFINE(HAVE___BUILTIN_EXPECT,,[Define if your compile allows __builtin_expect])
  ])

  AC_MSG_RESULT($ax_cv_c___builtin_expect)
])
