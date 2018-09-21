# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ----------------------------------------------------------------------

include $(top_srcdir)/check.am

SUBDIRS = data

TESTS = testquery

check_PROGRAMS = testquery

testquery_SOURCES = \
	TestVMQuery.cc \
	testquery.cc

noinst_HEADERS = \
	TestVMQuery.h

testquery_LDFLAGS =

testquery_LDADD = \
	-lcppunit -ldl \
	-letree \
	$(top_builddir)/libsrc/cencalvm/libcencalvm.la


# End of file 
