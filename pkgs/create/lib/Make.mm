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
PACKAGE = create

include local.def

PROJ_SAR = $(BLD_LIBDIR)/libcencalvmcreate.$(EXT_SAR)

PROJ_SRCS = \
	VMCreator.cc \
	GridIngester.cc \

all: $(PROJ_SAR) export-package-headers

# ----------------------------------------------------------------------
export:: release-package-headers export-libraries

EXPORT_HEADERS = \
	VMCreator.h \
	VMCreator.icc \

EXPORT_LIBS = $(PROJ_SAR)

# version
# $Id$

# End of file 
