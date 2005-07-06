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

PROJECT = cencalvm
PACKAGE = doc

all builddoc:
	doxygen doxyconfig

clean::
	$(RM) $(RMFLAGS) html

# version
# $Id$

#
# End of file
