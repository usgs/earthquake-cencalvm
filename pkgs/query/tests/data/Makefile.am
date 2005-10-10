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

data_TMP = \
	leaf.etree \
	full.etree

noinst_HEADERS = \
	TestVMQuery.dat

# 'export' the input files by performing a mock install
export_datadir = $(top_builddir)/pkgs/query/tests/data

clean-tmp:
	for f in $(data_TMP); do $(RM) $(RM_FLAGS) $(export_datadir)/$$f; done

# export
clean-local: clean-tmp

# version
# $Id$

# End of file 
