# -*- Makefile -*-
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

subpackage = query
include $(top_srcdir)/pkgs/subpackage.am

lib_LTLIBRARIES = libcencalvmquerylang.la

libcencalvmquerylang_la_SOURCES = \
	cvmerror.cc \
	cvmquery.cc \
	f77vmerror.cc \
	f77vmquery.cc

libcencalvmquerylang_la_LDFLAGS =

libcencalvmquerylang_la_LIBADD = \
	$(top_builddir)/pkgs/query/lib/libcencalvmquery.la \
	$(top_builddir)/pkgs/storage/lib/libcencalvmstorage.la

subpkginclude_HEADERS = \
	cvmerror.h \
	cvmquery.h

noinst_HEADERS = \
	f77vmerror.h \
	f77vmquery.h

# export
clean-local: clean-subpkgincludeHEADERS
BUILT_SOURCES = export-subpkgincludeHEADERS
CLEANFILES = export-subpkgincludeHEADERS

# version
# $Id$

# End of file 
