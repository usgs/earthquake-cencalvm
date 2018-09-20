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

bin_PROGRAMS = cencalvminfo cencalvmisosurface cencalvmquery

AM_CPPFLAGS = -I$(top_srcdir)/libsrc

cencalvminfo_SOURCES = cencalvminfo.cc
cencalvminfo_LDADD = -letree


cencalvmisosurface_SOURCES = cencalvmisosurface.cc
cencalvmisosurface_LDADD = $(top_builddir)/libsrc/cencalvm/libcencalvm.la

cencalvmquery_SOURCES = cencalvmquery.cc
cencalvmquery_LDADD = $(top_builddir)/libsrc/cencalvm/libcencalvm.la


# End of file 
