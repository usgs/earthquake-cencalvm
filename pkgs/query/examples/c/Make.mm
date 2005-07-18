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
PACKAGE = query/examples/c

include local.def

PROJ_SRCS = \
	cencalvmcquery.c \


PROJ_BIN = $(PROJ_BINDIR)/cencalvmcquery

TEST_ARGS = -d data/cencalvm-pre1.0.etree -i ../data/sample.in -o test.out

# ----------------------------------------------------------------------
all: $(PROJ_BIN)

$(PROJ_BIN):: product_dirs $(PROJ_OBJS)
	$(CC) -o $@ $(LCXXFLAGS) $(PROJ_OBJS)

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
