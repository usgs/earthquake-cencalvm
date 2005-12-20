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

subpackage = vsgrader
include $(top_srcdir)/pkgs/subpackage.am

bin_PROGRAMS = gradecencalvm

gradecencalvm_SOURCES = \
	gradecencalvm.cc

INCLUDES +=

gradecencalvm_LDADD = \
	-letree \
	$(top_builddir)/extensions/vsgrader/lib/libcencalvmvsgrader.la \
	$(top_builddir)/pkgs/create/lib/libcencalvmcreate.la \
	$(top_builddir)/pkgs/storage/lib/libcencalvmstorage.la

# version
# $Id$

# End of file 
