# -*- Makefile -*-
#
# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# {LicenseText}
#
# ----------------------------------------------------------------------
#

PROJECT = cencalvm
PACKAGE = storage/tests

include local.def

PROJ_SRCS = \
	TestGeometry.cc \
	TestProjector.cc \
	teststorage.cc \


RECURSE_DIRS = \
	data \

PROJ_BIN = $(PROJ_BINDIR)/teststorage

# ----------------------------------------------------------------------
all: $(PROJ_BIN)

$(PROJ_BIN):: $(PROJ_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(LCXXFLAGS) $(PROJ_OBJS)

test:
	@LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(BLD_LIBDIR) $(PROJ_BIN)

export:: export-binaries

tidy::
	BLD_ACTION="tidy" $(MM) recurse

clean::
	BLD_ACTION="clean" $(MM) recurse

EXPORT_BINS = $(PROJ_BIN)

# version
# $Id$

# End of file 
