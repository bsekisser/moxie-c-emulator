# Process this file with autoconf to produce a configure script.

AC_INIT([moxie-c-emulator],[0.1],[squirmyworms@embarqmail.com])

AC_CONFIG_MACRO_DIR([m4])

AM_MAINTAINER_MODE

AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/run_moxie.c])
AC_CONFIG_HEADERS([src/include/config.h])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
LT_INIT

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/libmoxie/Makefile])

AC_OUTPUT

