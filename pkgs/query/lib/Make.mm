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

PROJ_SAR = $(BLD_LIBDIR)/libcencalvmquery.$(EXT_SAR)

PROJ_SRCS = \
	VMQuery.cc \

all: $(PROJ_SAR) export-package-headers

# ----------------------------------------------------------------------
export:: release-package-headers export-libraries

EXPORT_HEADERS = \
	VMQuery.h \

EXPORT_LIBS = $(PROJ_SAR)

# version
# $Id$

# End of file 
