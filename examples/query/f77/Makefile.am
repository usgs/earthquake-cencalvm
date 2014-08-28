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

# Link using C++ compiler to make sure we satisfy all of the C++
# symbols in the C++ query library which this program uses.
# This may only be required by the Sun compiler and appears to break
# linking with the Intel compiler, so will comment it out.
#F77LD = @CXX@

# Use Fortran 90 compiler to build Fortran 77 code.
F77=$(FC)
FFLAGS=$(FCFLAGS)

bin_PROGRAMS = cencalvmf77query

cencalvmf77query_SOURCES = \
	cencalvmf77query.f

cencalvmf77query_LDFLAGS =

AM_CPPFLAGS = -I$(top_srcdir)/libsrc

cencalvmf77query_LDADD = \
	$(top_builddir)/libsrc/cencalvm/libcencalvm.la
	$(FCLIBS)

CLEANFILES = \
	test_maxres.out \
	test_fixedres.out \
	test_waveres.out \
	test.log \
	testext_maxres.out \
	testext_fixedres.out \
	testext_waveres.out


# End of file 
