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

SUBDIRS = data

TESTS = testquery

check_PROGRAMS = testquery

testquery_SOURCES = \
	TestVMQuery.cc \
	testquery.cc

noinst_HEADERS = \
	TestVMQuery.h

testquery_LDFLAGS =

INCLUDES +=

testquery_LDADD = \
	-lcppunit -ldl \
	-letree \
	$(top_builddir)/pkgs/query/lib/libcencalvmquery.la \
	$(top_builddir)/pkgs/average/lib/libcencalvmaverage.la \
	$(top_builddir)/pkgs/storage/lib/libcencalvmstorage.la

# version
# $Id$

# End of file 
