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
PACKAGE = query/examples/cpp

include local.def

PROJ_SRCS = \
	cencalvmcppquery.cc \


PROJ_BIN = $(PROJ_BINDIR)/cencalvmcppquery

TEST_ARGS = -d data/800x800x200.etree -i ../data/sample.in -o test.out

# ----------------------------------------------------------------------
all: $(PROJ_BIN)

$(PROJ_BIN):: $(PROJ_OBJS)
	$(CXX) -o $@ $(LCXXFLAGS) $(PROJ_OBJS)

test:
	@LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(BLD_LIBDIR) $(PROJ_BIN) $(TEST_ARGS)

export:: export-binaries

tidy::
	BLD_ACTION="tidy" $(MM) recurse

clean::
	BLD_ACTION="clean" $(MM) recurse

EXPORT_BINS = $(PROJ_BIN)

# version
# $Id$

# End of file 
