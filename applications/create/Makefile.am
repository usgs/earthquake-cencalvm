# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ----------------------------------------------------------------------

bin_PROGRAMS = \
	cencalvmgen \
	cencalvmpack

cencalvmgen_SOURCES = \
	cencalvmgen.cc

AM_CPPFLAGS = -I$(top_srcdir)/libsrc

cencalvmgen_LDADD = \
	-letree \
	$(top_builddir)/libsrc/cencalvm/libcencalvm.la

cencalvmpack_SOURCES = \
	cencalvmpack.cc

cencalvmpack_LDADD = \
	-letree \
	$(top_builddir)/libsrc/cencalvm/libcencalvm.la


# End of file 
