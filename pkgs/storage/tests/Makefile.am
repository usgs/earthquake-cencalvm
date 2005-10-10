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

SUBDIRS = data

TESTS = teststorage

check_PROGRAMS = teststorage

teststorage_SOURCES = \
	TestErrorHandler.cc \
	TestGeometry.cc \
	TestProjector.cc \
	teststorage.cc

noinst_HEADERS = \
	TestErrorHandler.h \
	TestGeometry.h \
	TestProjector.h

teststorage_LDFLAGS =

INCLUDES +=

teststorage_LDADD = \
	-lcppunit -ldl \
	-lproj \
	-letree \
	$(top_builddir)/pkgs/storage/lib/libcencalvmstorage.la

# version
# $Id$

# End of file 
