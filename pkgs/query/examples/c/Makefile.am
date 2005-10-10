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

bin_PROGRAMS = cencalvmcquery

cencalvmcquery_SOURCES = \
	cencalvmcquery.c

cencalvmcquery_LDFLAGS =

INCLUDES +=

cencalvmcquery_LDADD = \
	$(top_builddir)/pkgs/query/liblang/libcencalvmquerylang.la

# version
# $Id$

# End of file 
