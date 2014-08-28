# -*- Makefile -*-
#
# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ----------------------------------------------------------------------
#

include $(top_srcdir)/check.am

SUBDIRS = data

TESTS = testaverage

check_PROGRAMS = testaverage

testaverage_SOURCES = \
	TestAverager.cc \
	testaverage.cc

noinst_HEADERS = \
	TestAverager.h

testaverage_LDFLAGS =

testaverage_LDADD = \
	-lcppunit -ldl \
	-letree \
	$(top_builddir)/libsrc/cencalvm/libcencalvm.la


# End of file 
