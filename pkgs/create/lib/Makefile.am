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

subpackage = create
include $(top_srcdir)/pkgs/subpackage.am

lib_LTLIBRARIES = libcencalvmcreate.la

libcencalvmcreate_la_SOURCES = \
	VMCreator.cc \
	GridIngester.cc

libcencalvmcreate_la_LDFLAGS =

INCLUDES +=

libcencalvmcreate_la_LIBADD = \
	-letree \
	$(top_builddir)/pkgs/storage/lib/libcencalvmstorage.la

subpkginclude_HEADERS = \
	VMCreator.h \
	VMCreator.icc \
	GridIngester.h \
	GridIngester.icc

# export
clean-local: clean-subpkgincludeHEADERS
BUILT_SOURCES = export-subpkgincludeHEADERS
CLEANFILES = export-subpkgincludeHEADERS

# version
# $Id$

# End of file 
