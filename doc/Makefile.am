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

noinst_HEADERS = \
	doxyconfig

export-data:
	doxygen $(top_srcdir)/doc/doxyconfig

clean-data:
	rm -r html

BUILT_SOURCES = export-data
clean-local: clean-data

# version
# $Id$

# End of file 
