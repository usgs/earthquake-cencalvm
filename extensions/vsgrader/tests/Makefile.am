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

SUBDIRS = data

TESTS = testgrader

check_PROGRAMS = testgrader

testgrader_SOURCES = \
	TestVsGrader.cc \
	testgrader.cc

noinst_HEADERS = \
	TestVsGrader.h

testgrader_LDFLAGS =

INCLUDES +=

testgrader_LDADD = \
	-lcppunit -ldl \
	-letree \
	$(top_builddir)/extensions/vsgrader/lib/libcencalvmvsgrader.la \
	$(top_builddir)/pkgs/create/lib/libcencalvmcreate.la

# version
# $Id$

# End of file 
