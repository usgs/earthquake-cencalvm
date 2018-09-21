# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ----------------------------------------------------------------------

include $(top_srcdir)/check.am

SUBDIRS = data

TESTS = testgrader

check_PROGRAMS = testgrader

testgrader_SOURCES = \
	TestVsGrader.cc \
	testgrader.cc

noinst_HEADERS = \
	TestVsGrader.h

testgrader_LDFLAGS =

AM_CPPFLAGS += -I$(top_srcdir)/extensions/applications/vsgrader

testgrader_LDADD = \
	-lcppunit -ldl \
	-letree \
	$(top_builddir)/libsrc/libcencalvm.la


# End of file 
