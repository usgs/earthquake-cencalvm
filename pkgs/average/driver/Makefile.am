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

subpackage = average
include $(top_srcdir)/pkgs/subpackage.am

bin_PROGRAMS = avgcencalvm

avgcencalvm_SOURCES = \
	avgcencalvm.cc

INCLUDES += 

avgcencalvm_LDADD = \
	$(top_builddir)/pkgs/average/lib/libcencalvmaverage.la \
	$(top_builddir)/pkgs/storage/lib/libcencalvmstorage.la

# version
# $Id$

# End of file 
