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

SUBDIRS = data

TESTS = testaverage

check_PROGRAMS = testaverage

testaverage_SOURCES = \
	TestAverager.cc \
	testaverage.cc

noinst_HEADERS = \
	TestAverager.h

testaverage_LDFLAGS =

INCLUDES +=

testaverage_LDADD = \
	-lcppunit -ldl \
	-letree \
	$(top_builddir)/pkgs/average/lib/libcencalvmaverage.la \
	$(top_builddir)/pkgs/storage/lib/libcencalvmstorage.la

# version
# $Id$

# End of file 
