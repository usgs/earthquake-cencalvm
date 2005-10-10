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

subpackage = query
include $(top_srcdir)/pkgs/subpackage.am

bin_PROGRAMS = cencalvmcppquery

cencalvmcppquery_SOURCES = \
	cencalvmcppquery.cc

cencalvmcppquery_LDFLAGS =

INCLUDES +=

cencalvmcppquery_LDADD = \
	$(top_builddir)/pkgs/query/lib/libcencalvmquery.la \
	$(top_builddir)/pkgs/storage/lib/libcencalvmstorage.la

# version
# $Id$

# End of file 
