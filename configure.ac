#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([quicksend], [0.1.0], [dg.briseno@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/receiver.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  src/Makefile
  lib/Makefile
])

# Checks for programs.
AC_PROG_CC
gl_EARLY
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([sys/socket.h argp.h netinet/in.h arpa/inet.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_OFF_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
# For gnulib
gl_INIT

# Checks for library functions.
AC_CHECK_FUNCS([socket strerror strtol inet_ntoa])

AC_OUTPUT
