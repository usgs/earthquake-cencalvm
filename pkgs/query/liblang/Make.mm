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
PACKAGE = query

include local.def

PROJ_SAR = $(BLD_LIBDIR)/libcencalvmquerylang.$(EXT_SAR)

PROJ_SRCS = \
	cvmerror.cc \
	cvmquery.cc \
	f77vmerror.cc \
	f77vmquery.cc \

all: $(PROJ_SAR) export-package-headers

# ----------------------------------------------------------------------
export:: release-package-headers export-libraries

EXPORT_HEADERS = \
	cvmerror.h \
	cvmquery.h \

EXPORT_LIBS = $(PROJ_SAR)

# version
# $Id$

# End of file 
