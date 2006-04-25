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

data_DATA = \
	one.dat \
	paramfile.txt

data_TMP = \
	one.etree \
	two.etree \
	tmp.etree

noinst_HEADERS = \
	TestVMCreator.dat \
	TestGridIngester.dat \
	$(data_DATA)

# 'export' the input files by performing a mock install
export_datadir = $(top_builddir)/pkgs/create/tests/data
export-data: $(data_DATA)
	for f in $(data_DATA); do $(install_sh_DATA) $(srcdir)/$$f $(export_datadir); done

# export
BUILT_SOURCES = export-data

CLEANFILES = \
	$(export_datadir)/$(data_TMP) \
	$(export_datadir)/$(data_DATA)

# version
# $Id$

# End of file 
