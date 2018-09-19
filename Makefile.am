# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ----------------------------------------------------------------------

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
	libsrc \
	applications \
	examples \
	docs

if ENABLE_SPATIAL
  SUBDIRS += extensions
endif
if ENABLE_VSGRADER
  SUBDIRS += extensions
endif

if ENABLE_TESTING
  SUBDIRS += tests
endif


EXTRA_DIST = \
	CHANGES.md \
	README.md \
	INSTALL.md \
	LICENSE.md \
	DISCLAIMER.md \
	code.json \
	docs/index.md \
	docs/query.md \
	docs/examples.md

DISTCHECK_CONFIGURE_FLAGS = \
	--enable-testing \
	CPPFLAGS="-I${ETREE_INCDIR} -I${PROJ4_INCDIR} -I${CPPUNIT_INCDIR}" \
	LDFLAGS="-L${ETREE_LIBDIR} -L${PROJ4_LIBDIR} -L${CPPUNIT_LIBDIR}"


# End of file 
