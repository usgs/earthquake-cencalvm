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
PACKAGE = average

include local.def

PROJ_SAR = $(BLD_LIBDIR)/libcencalvmavg.$(EXT_SAR)

PROJ_SRCS = \
	Averager.cc \
	AvgEngine.cc \

all: $(PROJ_SAR) export-package-headers

# ----------------------------------------------------------------------
export:: release-package-headers export-libraries

EXPORT_HEADERS = \
	Averager.icc \
	Averager.h \

EXPORT_LIBS = $(PROJ_SAR)

# version
# $Id$

# End of file 
