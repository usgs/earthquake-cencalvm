# -*- autoconf -*-
#
# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# <LicenseText>
#
# ----------------------------------------------------------------------
#

AC_PREREQ(2.59)
AC_INIT([cencalvm], [0.4.1], [baagaard@usgs.gov])
AC_CONFIG_HEADER([portinfo])
AC_CONFIG_AUX_DIR([./aux-config])
AM_INIT_AUTOMAKE([foreign])

# ----------------------------------------------------------------------
# TESTING w/cppunit
AC_ARG_ENABLE([testing],
	[  --enable-testing        Enable unit testing with cppunit (requires cppunit) [[default=no]]],
	[enable_testing=yes],
	[enable_testing=no])

# SPATIAL w/spatialdata
AC_ARG_ENABLE([spatial],
	[  --enable-spatial        Enable use of spatialdata package (requires spatialdata) [[default=no]]],
	[enable_spatial=yes],
	[enable_spatial=no])

# DOCUMENTATION w/doxygen
AC_ARG_ENABLE([documentation],
	[  --enable-documentation  Enable buuilding html documentation (requires doxygen) [[default=no]]],
	[enable_documentation=yes],
	[enable_documentation=no])
# ----------------------------------------------------------------------

AC_PROG_CXX
AC_PROG_CC
AC_PROG_FC
AC_FC_WRAPPERS
AC_FC_LIBRARY_LDFLAGS
AC_PROG_LIBTOOL
AC_PROG_INSTALL

# ----------------------------------------------------------------------
# ETREE
AC_CHECK_LIB(etree, etree_open, [
  AC_CHECK_HEADER([etree.h], [], [
    AC_MSG_ERROR([Etree header not found; try CPPFLAGS="-I<Etree include dir>"])
  ])
],[
  AC_MSG_ERROR([Etree library not found; try LDFLAGS="-L<Etree lib dir>"])
])

# PROJ4
AC_CHECK_LIB(proj, pj_init_plus, [
  AC_CHECK_HEADER([proj_api.h], [], [
    AC_MSG_ERROR([Proj4 header not found; try CPPFLAGS="-I<Proj4 include dir>"])
  ])
],[
  AC_MSG_ERROR([Proj4 library not found; try LDFLAGS="-L<Proj4 lib dir>"])
])

# CPPUNIT
AM_CONDITIONAL([ENABLE_TESTING], [test "$enable_testing" = yes])
if test "$enable_testing" = "yes" ; then
  AC_LANG(C++)
  AC_CHECK_HEADER([cppunit/TestRunner.h], [], [
    AC_MSG_ERROR([CppUnit header not found; try CPPFLAGS="-I<CppUnit include dir>"])
  ])
  AC_MSG_CHECKING([for CppUnit::TestRunner in -lcppunit])
  AC_REQUIRE_CPP
  AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([[#include <cppunit/TestRunner.h>]],
	             [[CppUnit::TestRunner runner;]])],
    [AC_MSG_RESULT(yes)],
    [AC_MSG_RESULT(no)
     AC_MSG_ERROR([CppUnit library not found; try LDFLAGS="-L<CppUnit lib dir>"])
  ])
fi

# SPATIALDATA
AM_CONDITIONAL([ENABLE_SPATIAL], [test "$enable_spatial" = yes])
if test "$enable_spatial" = "yes" ; then
  AC_LANG(C++)
  AC_CHECK_HEADER([spatialdata/spatialdb/SpatialDB.hh], [], [
    AC_MSG_ERROR([SpatialDB header not found; try CPPFLAGS="-I<spatialdata include dir>"])
  ])
  AC_MSG_CHECKING([for spatialdata::spatialdb::SimpleDB in -lspatialdataspatialdb])
  AC_REQUIRE_CPP
  AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([[#include <spatialdata/spatialdb/SpatialDB.hh>]
	[#include <spatialdata/spatialdb/SimpleDB.hh>]],
	             [[spatialdata::spatialdb::SimpleDB db;]])],
    [AC_MSG_RESULT(yes)],
    [AC_MSG_RESULT(no)
     AC_MSG_ERROR([spatialdata library not found; try LDFLAGS="-L<spatialdata lib dir>"])
  ])
fi

# DOXYGEN
AM_CONDITIONAL([ENABLE_DOCUMENTATION], [test "$enable_documentation" = yes])
if test "$enable_documentation" = "yes" ; then
  AC_CHECK_PROG([have_doxygen],
    [doxygen],
    [yes],
    [no])
  if test "$have_doxygen" = "no"; then
    AC_MSG_ERROR([Doxygen not found in current path.])
  fi
fi

# ----------------------------------------------------------------------
AC_CONFIG_FILES([Makefile
	pkgs/Makefile
	pkgs/storage/Makefile
	pkgs/storage/lib/Makefile
	pkgs/storage/tests/Makefile
	pkgs/storage/tests/data/Makefile
	pkgs/create/Makefile
	pkgs/create/lib/Makefile
	pkgs/create/driver/Makefile
	pkgs/create/tests/Makefile
	pkgs/create/tests/data/Makefile
	pkgs/average/Makefile
	pkgs/average/lib/Makefile
	pkgs/average/driver/Makefile
	pkgs/average/tests/Makefile
	pkgs/average/tests/data/Makefile
	pkgs/query/Makefile
	pkgs/query/lib/Makefile
	pkgs/query/tests/Makefile
	pkgs/query/tests/data/Makefile
	pkgs/query/liblang/Makefile
	pkgs/query/apps/Makefile
	pkgs/query/apps/cencalvminfo/Makefile
	pkgs/query/examples/Makefile
	pkgs/query/examples/cpp/Makefile
	pkgs/query/examples/c/Makefile
	pkgs/query/examples/f77/Makefile
	pkgs/query/examples/data/Makefile
	extensions/Makefile
	extensions/cencalvmdb/Makefile
	extensions/cencalvmdb/lib/Makefile
	extensions/vsgrader/Makefile
	extensions/vsgrader/lib/Makefile
	extensions/vsgrader/driver/Makefile
	extensions/vsgrader/tests/Makefile
	extensions/vsgrader/tests/data/Makefile
	doc/Makefile])

AC_OUTPUT


# version
# $Id$

# End of file 
