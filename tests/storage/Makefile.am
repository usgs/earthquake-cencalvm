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

include $(top_srcdir)/check.am

SUBDIRS = data

TESTS = teststorage

check_PROGRAMS = teststorage

teststorage_SOURCES = \
	TestErrorHandler.cc \
	TestGeomCenCA.cc \
	TestGeometry.cc \
	TestProjector.cc \
	teststorage.cc

noinst_HEADERS = \
	TestErrorHandler.h \
	TestGeomCenCA.h \
	TestGeometry.h \
	TestProjector.h

teststorage_LDFLAGS =

teststorage_LDADD = \
	-lcppunit -ldl \
	-lproj \
	-letree \
	$(top_builddir)/libsrc/cencalvm/libcencalvm.la


# End of file 
