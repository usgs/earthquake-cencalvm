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

bin_PROGRAMS = cencalvmf77query

cencalvmf77query_SOURCES = \
	cencalvmf77query.f

cencalvmf77query_LDFLAGS =

INCLUDES +=

cencalvmf77query_LDADD = \
	$(top_builddir)/pkgs/query/liblang/libcencalvmquerylang.la

# version
# $Id$

# End of file 
