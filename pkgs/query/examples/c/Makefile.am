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

# ----------------------------------------------------------------------

VELMODEL = USGSBayAreaVM-05.0.1.etree

testmaxres:
	cencalvmcquery \
		-d $(VELMODEL) \
		-i ../data/sample-05.0.x.in \
		-o test_maxres.out \
		-l test.log

testfixedres:
	cencalvmcquery \
		-d $(VELMODEL) \
		-i ../data/sample-05.0.x.in \
		-o test_fixedres.out \
		-t fixedres \
		-r 200.0 \
		-l test.log

testwaveres:
	cencalvmcquery \
		-d $(VELMODEL) \
		-i ../data/sample-05.0.x.in \
		-o test_waveres.out \
		-t waveres \
		-r 0.2 \
		-l test.log

CLEANFILES = \
	test_maxres.out \
	test_fixedres.out \
	test_waveres.out \
	test.log

# version
# $Id$

# End of file 
