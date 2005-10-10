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
	tmp.etree

noinst_HEADERS = \
	TestVMCreator.dat \
	$(data_DATA)

# 'export' the input files by performing a mock install
export_datadir = $(top_builddir)/pkgs/create/tests/data
export-data: $(data_DATA)
	for f in $(data_DATA); do $(install_sh_DATA) $(srcdir)/$$f $(export_datadir); done

clean-data:
	for f in $(data_DATA); do $(RM) $(RM_FLAGS) $(export_datadir)/$$f; done

clean-tmp:
	for f in $(data_TMP); do $(RM) $(RM_FLAGS) $(export_datadir)/$$f; done

# export
clean-local: clean-data clean-tmp
BUILT_SOURCES = export-data

# version
# $Id$

# End of file 
