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

subpackage = storage
include $(top_srcdir)/pkgs/subpackage.am

lib_LTLIBRARIES = libcencalvmstorage.la

libcencalvmstorage_la_SOURCES = \
	ErrorHandler.cc \
	GeomCenCA.cc \
	Geometry.cc \
	Payload.cc \
	Projector.cc

libcencalvmstorage_la_LDFLAGS =

INCLUDES +=

libcencalvmstorage_la_LIBADD = \
	-lproj \
	-letree

subpkginclude_HEADERS = \
	ErrorHandler.h \
	ErrorHandler.icc \
	GeomCenCA.h \
	GeomCenCA.icc \
	Geometry.h \
	Payload.h \
	Projector.h \
	etreefwd.h \
	proj4fwd.h

noinst_HEADERS =

# export
clean-local: clean-subpkgincludeHEADERS
BUILT_SOURCES = export-subpkgincludeHEADERS
CLEANFILES = export-subpkgincludeHEADERS

# version
# $Id$

# End of file 
