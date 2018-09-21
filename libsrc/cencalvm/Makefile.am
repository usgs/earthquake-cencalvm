# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# ----------------------------------------------------------------------

SUBDIRS = \
	storage \
	create \
	average \
	query

lib_LTLIBRARIES = libcencalvm.la

libcencalvm_la_SOURCES = \
	storage/ErrorHandler.cc \
	storage/GeomCenCA.cc \
	storage/Geometry.cc \
	storage/Payload.cc \
	storage/Projector.cc \
	create/VMCreator.cc \
	create/GridIngester.cc \
	average/Averager.cc \
	average/AvgEngine.cc \
	query/VMQuery.cc \
	query/cvmerror.cc \
	query/cvmquery.cc

if ENABLE_FORTRAN
libcencalvm_la_SOURCES += \
	query/f77vmerror.cc \
	query/f77vmquery.cc
endif

libcencalvm_la_LDFLAGS =

AM_CPPFLAGS = -I$(top_srcdir)/libsrc

libcencalvm_la_LIBADD = \
	-lproj \
	-letree


# End of file 
