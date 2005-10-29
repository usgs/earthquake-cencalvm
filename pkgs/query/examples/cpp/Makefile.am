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

# ----------------------------------------------------------------------

VELMODEL = USGSBayAreaVM-05.1.0.etree
VELMODELEXT = USGSBayAreaVMExt-05.1.0.etree

testmaxres:
	cencalvmcppquery \
		-d $(VELMODEL) \
		-i ../data/sample-05.1.x.in \
		-o test_maxres.out \
		-l test.log

testfixedres:
	cencalvmcppquery \
		-d $(VELMODEL) \
		-i ../data/sample-05.1.x.in \
		-o test_fixedres.out \
		-t fixedres \
		-r 200.0 \
		-l test.log

testwaveres:
	cencalvmcppquery \
		-d $(VELMODEL) \
		-i ../data/sample-05.1.x.in \
		-o test_waveres.out \
		-t waveres \
		-r 0.2 \
		-l test.log

testextmaxres:
	cencalvmcppquery \
		-d $(VELMODEL) \
		-e $(VELMODELEXT) \
		-i ../data/sample_ext-05.1.x.in \
		-o testext_maxres.out \
		-l test.log

testextfixedres:
	cencalvmcppquery \
		-d $(VELMODEL) \
		-e $(VELMODELEXT) \
		-i ../data/sample_ext-05.1.x.in \
		-o testext_fixedres.out \
		-t fixedres \
		-r 200.0 \
		-l test.log

testextwaveres:
	cencalvmcppquery \
		-d $(VELMODEL) \
		-e $(VELMODELEXT) \
		-i ../data/sample_ext-05.1.x.in \
		-t waveres \
		-r 0.2 \
		-o testext_waveres.out \
		-l test.log

CLEANFILES = \
	test_maxres.out \
	test_fixedres.out \
	test_waveres.out \
	test.log \
	testext_maxres.out \
	testext_fixedres.out \
	testext_waveres.out

# version
# $Id$

# End of file 
