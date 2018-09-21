# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ----------------------------------------------------------------------

include $(top_srcdir)/check.am

SUBDIRS = data

TESTS = testcreate

check_PROGRAMS = testcreate

testcreate_SOURCES = \
	TestGridIngester.cc \
	TestVMCreator.cc \
	testcreate.cc

noinst_HEADERS = \
	TestGridIngester.h \
	TestVMCreator.h

testcreate_LDFLAGS =

testcreate_LDADD = \
	-lcppunit -ldl \
	-letree \
	$(top_builddir)/libsrc/cencalvm/libcencalvm.la


# End of file 
