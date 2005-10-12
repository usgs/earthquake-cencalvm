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

SUBDIRS = pkgs

if ENABLE_DOCUMENTATION
  SUBDIRS += doc
endif

DISTCHECK_CONFIGURE_FLAGS = \
	--enable-testing \
	--enable-documentation \
	CPPFLAGS="-I${ETREE_INCDIR} -I${PROJ4_INCDIR} -I${CPPUNIT_INCDIR}" \
	LDFLAGS="-L${ETREE_LIBDIR} -L${PROJ4_LIBDIR} -L${CPPUNIT_LIBDIR}"

# version
# $Id$

# End of file 