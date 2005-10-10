#!/bin/bash
#
# ======================================================================
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# {LicenseText}
#
# ======================================================================
#

# Remove autoconf stuff
autoconf_files="aclocal.m4 configure portinfo.in"
autoconf_dirs="autom4te.cache aux-config"
rm -r $autoconf_dirs
rm $autoconf_files
rm `find . -name Makefile.in`

# Remove emacs backup stuff
rm `find . -name "*~"`

# version
# $Id$

# End of file
