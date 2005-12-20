// -*- C++ -*-
//
// ======================================================================
//
//                           Brad T. Aagaard
//                        U.S. Geological Survey
//
// {LicenseText}
//
// ======================================================================
//

#include "Averager.h" // implementation of class methods

#include "cencalvm/storage/Payload.h" // USES PayloadStruct
#include "cencalvm/storage/ErrorHandler.h" // USES ErrorHandler
#include "AvgEngine.h" // USES AvgEngine

extern "C" {
#include "etree.h"
}

#include <sstream> // USES std::ostringstream
#include <iomanip> // USES setw(), setiosflags(), resetiosflags()
#include <assert.h> // USES assert()

// ----------------------------------------------------------------------
// Default constructor
cencalvm::average::Averager::Averager(void) :
  _dbIn(0),
  _dbAvg(0),
  _filenameIn(""),
  _filenameOut(""),
  _pErrHandler(new cencalvm::storage::ErrorHandler),
  _quiet(false)
{ // constructor
} // constructor
  
// ----------------------------------------------------------------------
// Default destructor.
cencalvm::average::Averager::~Averager(void)
{ // destructor
  delete _pErrHandler; _pErrHandler = 0;
  if (0 != _dbIn) {
    etree_close(_dbIn);
    _dbIn = 0;
  } // if
  if (0 != _dbAvg) {
    etree_close(_dbAvg);
    _dbAvg = 0;
  } // if
} // destructor

// ----------------------------------------------------------------------
// Spatially average etree database by filling in etree octants with
// average of their children.
void
cencalvm::average::Averager::average(void)
{ // average
  assert(0 != _pErrHandler);


  // Open input database
  const int cacheSize = 512;
  const int numDims = 3;
  const int payloadSize = sizeof(cencalvm::storage::PayloadStruct);
  _dbIn = etree_open(_filenameIn.c_str(), O_RDONLY,
		     cacheSize, payloadSize, numDims);
  if (0 == _dbIn) {
    std::ostringstream msg;
    msg
      << "Could not open etree database '" << _filenameIn
      << "' for averaging.";
    _pErrHandler->error(msg.str().c_str());
    return;
  } // if

  // Check compatibility of database
  if (etree_getpayloadsize(_dbIn) != payloadSize) {
    std::ostringstream msg;
    msg
      << "Payload size of input etree database '" << _filenameIn
      << "' doesn't match the expected payload size.\n"
      << "Expected payload size is " << payloadSize
      << ", and database payload size is " << etree_getpayloadsize(_dbIn)
      << ".";
    _pErrHandler->error(msg.str().c_str());
    return;
  } // if
  const char* schema = etree_getschema(_dbIn);
  if (0 != strcmp(schema, cencalvm::storage::Payload::SCHEMA)) {
    std::ostringstream msg;
    msg
      << "Schema of input etree database '" << _filenameIn
      << "' doesn't match the expected schema.\n"
      << "Expected schema is\n'" << cencalvm::storage::Payload::SCHEMA
      << "'\nand database schema is\n'" << schema << "'";
    _pErrHandler->error(msg.str().c_str());
    return;
  } // if

  // Open avg database for output
  _dbAvg = etree_open(_filenameOut.c_str(),
		      O_CREAT|O_RDWR|O_TRUNC, cacheSize, payloadSize, numDims);
  if (0 == _dbAvg) {
    std::ostringstream msg;
    msg
      << "Could not open etree database '" << _filenameOut
      << "' for output of averaged etree database .";
    _pErrHandler->error(msg.str().c_str());
    return;
  } // if

  // Register schema in output database
  if (0 != etree_registerschema(_dbAvg, cencalvm::storage::Payload::SCHEMA)) {
    _pErrHandler->error(etree_strerror(etree_errno(_dbAvg)));
    etree_close(_dbAvg);
    return;
  } // if

  // Set database metadata if input etree has metadata
  char* appmeta = etree_getappmeta(_dbIn);
  if (0 != appmeta) {
    const int maxLen = 128;
    char hostname[maxLen];
    gethostname(hostname, maxLen);
    time_t rawTime = time(0);
    const char* datetime = ctime(&rawTime);
    std::ostringstream metainfo;
    metainfo
      << appmeta << "\n"
      << "spatially averaged on: " << datetime
      << "host: "  << hostname;
    if (0 != etree_setappmeta(_dbAvg, metainfo.str().c_str())) {
      _pErrHandler->error(etree_strerror(etree_errno(_dbAvg)));
      etree_close(_dbAvg);
      return;
    } // if
  } // if

  AvgEngine engine(_dbAvg, _dbIn, *_pErrHandler);
  engine.fillOctants();
  if (!_quiet)
    engine.printOctantInfo();

  etree_close(_dbIn); _dbIn = 0;
  etree_close(_dbAvg); _dbAvg = 0;
} // average
  
// version
// $Id$

// End of file 
