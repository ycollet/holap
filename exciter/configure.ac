#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([exciter],[0.1.0],[holborn@telefonica.net])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])


# Checks for programs.
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_RANLIB

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
AC_CHECK_LIB([pthread], [main])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([ladspa.h fcntl.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_MALLOC
AC_CHECK_FUNCS([bzero memset pow strstr strdup])

if test "x$prefix" == "xNONE"; then
prefix=/usr/local
fi

DATADIR="$prefix/share/${PACKAGE}"

AC_DEFINE_UNQUOTED(DATADIR,"$DATADIR",[DATADIR])

PKG_CHECK_MODULES(MODULE, dssi >= 0.4 liblo >= 0.12)

SSE=$(cat /proc/cpuinfo | grep sse)

if test -z "$SSE"; then
SSE=""
else
SSE="-msse -mfpmath=sse -ffast-math"
fi


CFLAGS="-Wall -pipe $SSE -O2 -Wall -ffunction-sections -fomit-frame-pointer $MODULE_CFLAGS"
LDFLAGS="$MODULE_LIBS" 


AC_CONFIG_FILES([Makefile
                 data/Makefile
                 src/Makefile])
AC_OUTPUT

