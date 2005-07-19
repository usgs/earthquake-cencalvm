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
PACKAGE = storage

include local.def

PROJ_SAR = $(BLD_LIBDIR)/libcencalvmstorage.$(EXT_SAR)

PROJ_SRCS = \
	ErrorHandler.cc \
	Geometry.cc \
	Projector.cc \

all: $(PROJ_SAR) export-package-headers

# ----------------------------------------------------------------------
export:: release-package-headers export-libraries

EXPORT_HEADERS = \
	ErrorHandler.h \
	ErrorHandler.icc \
	Geometry.h \
	Geometry.icc \
	Payload.h \
	etreefwd.h \

EXPORT_LIBS = $(PROJ_SAR)

# version
# $Id$

# End of file 
