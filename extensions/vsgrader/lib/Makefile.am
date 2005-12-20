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

subpackage = vsgrader
include $(top_srcdir)/pkgs/subpackage.am

lib_LTLIBRARIES = libcencalvmvsgrader.la

libcencalvmvsgrader_la_SOURCES = \
	VsGrader.cc

libcencalvmvsgrader_la_LDFLAGS =

INCLUDES +=

libcencalvmvsgrader_la_LIBADD = \
	-letree \
	$(top_builddir)/pkgs/storage/lib/libcencalvmstorage.la

subpkginclude_HEADERS = \
	VsGrader.h \
	VsGrader.icc

noinst_HEADERS =

# export
clean-local: clean-subpkgincludeHEADERS
BUILT_SOURCES = export-subpkgincludeHEADERS
CLEANFILES = export-subpkgincludeHEADERS

# version
# $Id$

# End of file 
