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

package = cencalvm

pkgpyexec_LTLIBRARIES = cencalvmmodule.la

cencalvmmodule_la_LDFLAGS = -module -avoid-version \
	$(AM_LDFLAGS) $(PYTHON_LA_LDFLAGS)

cencalvmmodule_la_SOURCES = cencalvm.pyxe

nodist_cencalvmmodule_la_SOURCES = \
	cencalvm.c cencalvm_embed.cpp cencalvm_embed.h

cencalvmmodule_la_LIBADD = \
	$(top_builddir)/extensions/libsrc/libcencalvmextras.la \
	$(top_builddir)/libsrc/libcencalvm.la
#if NO_UNDEFINED
#cencalvmmodule_la_LIBADD += \
#	$(PYTHON_BLDLIBRARY) $(PYTHON_LIBS) $(PYTHON_SYSLIBS)
#endif

INCLUDES = -I$(top_builddir)/include \
	-I$(top_srcdir)/extensions/libsrc/cencalvmdb \
	-I$(PYTHON_INCDIR)

cencalvm.pyx cencalvm_embed.cpp  cencalvm_embed.h: cencalvm.pyxe
	pyrexembed cencalvm.pyxe
cencalvm.pyxe: $(srcdir)/cencalvm.pyxe.src
	cp $(srcdir)/cencalvm.pyxe.src $@
cencalvm_embed.cpp: cencalvm_embed.h
cencalvm_embed.h: cencalvm.pyx

.pyx.c:
	pyrexc $<

CLEANFILES = cencalvm.pyx cencalvm.c *_embed.*

# End of file 
