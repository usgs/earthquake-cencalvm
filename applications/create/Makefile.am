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

bin_PROGRAMS = \
	gencencalvm \
	packcencalvm

gencencalvm_SOURCES = \
	gencencalvm.cc

AM_CPPFLAGS = -I$(top_srcdir)/libsrc

gencencalvm_LDADD = \
	-letree \
	$(top_builddir)/libsrc/cencalvm/libcencalvm.la

packcencalvm_SOURCES = \
	packcencalvm.cc

packcencalvm_LDADD = \
	-letree \
	$(top_builddir)/libsrc/cencalvm/libcencalvm.la


# End of file 
