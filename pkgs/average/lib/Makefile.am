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

subpackage = average
include $(top_srcdir)/pkgs/subpackage.am

lib_LTLIBRARIES = libcencalvmaverage.la

libcencalvmaverage_la_SOURCES = \
	Averager.cc \
	AvgEngine.cc

libcencalvmaverage_la_LDFLAGS =

INCLUDES +=

libcencalvmaverage_la_LIBADD = \
	-letree \
	$(top_builddir)/pkgs/storage/lib/libcencalvmstorage.la

subpkginclude_HEADERS = \
	Averager.icc \
	Averager.h

noinst_HEADERS = \
	AvgEngine.h

# export
clean-local: clean-subpkgincludeHEADERS
BUILT_SOURCES = export-subpkgincludeHEADERS
CLEANFILES = export-subpkgincludeHEADERS

# version
# $Id$

# End of file 
