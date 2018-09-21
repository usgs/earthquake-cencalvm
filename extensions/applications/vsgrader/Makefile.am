# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ----------------------------------------------------------------------

bin_PROGRAMS = gradecencalvm

gradecencalvm_SOURCES = \
	VsGrader.cc \
	gradecencalvm.cc

AM_CPPFLAGS = -I$(top_srcdir)/libsrc

gradecencalvm_LDADD = \
	-letree \
	$(top_builddir)/libsrc/cencalvm/libcencalvm.la


noinst_HEADERS = \
	VsGrader.h \
	VsGrader.icc


# End of file 
