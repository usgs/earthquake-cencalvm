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
	test.log

noinst_HEADERS = \
	TestProjector.dat

# 'export' the input files by performing a mock install
export_datadir = $(top_builddir)/pkgs/storage/tests/data

clean-tmp:
	for f in $(data_TMP); do $(RM) $(RM_FLAGS) $(export_datadir)/$$f; done

# export
clean-local: clean-tmp

# version
# $Id$

# End of file 
