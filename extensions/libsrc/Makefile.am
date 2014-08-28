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

SUBDIRS = 

lib_LTLIBRARIES = libcencalvmextras.la

libcencalvmextras_la_SOURCES = 

libcencalvmextras_la_LDFLAGS =

AM_CPPFLAGS = -I$(top_srcdir)/libsrc

libcencalvmextras_la_LIBADD = \
	-lproj \
	-letree \
	$(top_builddir)/libsrc/cencalvm/libcencalvm.la


if ENABLE_SPATIAL
  SUBDIRS += cencalvmdb

  libcencalvmextras_la_SOURCES += \
	cencalvmdb/CenCalVMDB.cc

  libcencalvmextras_la_LIBADD += -lspatialdata
endif


# End of file 
