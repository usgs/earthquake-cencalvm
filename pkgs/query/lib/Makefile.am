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

lib_LTLIBRARIES = libcencalvmquery.la

libcencalvmquery_la_SOURCES = \
	VMQuery.cc

libcencalvmquery_la_LDFLAGS =

INCLUDES +=

libcencalvmquery_la_LIBADD = \
	-letree

subpkginclude_HEADERS = \
	VMQuery.h \
	VMQuery.icc

# export
clean-local: clean-subpkgincludeHEADERS
BUILT_SOURCES = export-subpkgincludeHEADERS
CLEANFILES = export-subpkgincludeHEADERS

# version
# $Id$

# End of file 
