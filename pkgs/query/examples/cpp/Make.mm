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

TEST_ARGS = \
	-d USGSBayAreaVM-05.0.1.etree \
	-i ../data/sample-05.0.x.in \
	-o test.out \
	-l test.log

TEST_ARGS_FIXEDRES = \
	-d USGSBayAreaVM-05.0.1.etree \
	-i ../data/sample-05.0.x.in \
	-o test_fixedres.out \
	-l test.log \
	-t fixedres \
	-r 200

TEST_ARGS_WAVERES = \
	-d USGSBayAreaVM-05.0.1.etree \
	-i ../data/sample-05.0.x.in \
	-o test_waveres.out \
	-l test.log \
	-t waveres \
	-r 0.2

# ----------------------------------------------------------------------
all: $(PROJ_BIN)

$(PROJ_BIN):: product_dirs $(PROJ_OBJS)
	$(CXX) -o $@ $(LCXXFLAGS) $(PROJ_OBJS)

test:
	@LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(BLD_LIBDIR) $(PROJ_BIN) $(TEST_ARGS)

testfixedres:
	@LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(BLD_LIBDIR) $(PROJ_BIN) $(TEST_ARGS_FIXEDRES)

testwaveres:
	@LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(BLD_LIBDIR) $(PROJ_BIN) $(TEST_ARGS_WAVERES)

export:: export-binaries

tidy::
	BLD_ACTION="tidy" $(MM) recurse

clean::
	BLD_ACTION="clean" $(MM) recurse

EXPORT_BINS = $(PROJ_BIN)

# version
# $Id$

# End of file 
