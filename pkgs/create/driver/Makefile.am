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

subpackage = create
include $(top_srcdir)/pkgs/subpackage.am

bin_PROGRAMS = gencencalvm

gencencalvm_SOURCES = \
	gencencalvm.cc

INCLUDES +=

gencencalvm_LDADD = \
	-letree \
	$(top_builddir)/pkgs/create/lib/libcencalvmcreate.la \
	$(top_builddir)/pkgs/storage/lib/libcencalvmstorage.la

# version
# $Id$

# End of file 
