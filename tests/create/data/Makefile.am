# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ----------------------------------------------------------------------

noinst_DATA = \
	one.dat \
	paramfile.txt

data_TMP = \
	one.etree \
	two.etree \
	tmp.etree

noinst_HEADERS = \
	TestVMCreator.dat \
	TestGridIngester.dat

# 'export' the input files by performing a mock install
export_datadir = $(top_builddir)/tests/create/data
export-data: $(data_DATA)
	if [ "X$(top_srcdir)" != "X$(top_builddir)" ]; then for f in $(noinst_DATA); do $(install_sh_DATA) $(srcdir)/$$f $(export_datadir); done; fi

clean-data:
	if [ "X$(top_srcdir)" != "X$(top_builddir)" ]; then for f in $(noinst_DATA); do $(RM) $(RM_FLAGS) $(export_datadir)/$$f; done; fi

# export
clean-local: clean-data
BUILT_SOURCES = export-data
CLEANFILES = \
	$(export_datadir)/$(data_TMP)


# End of file 
