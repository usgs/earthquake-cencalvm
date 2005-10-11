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

#include "VMCreator.h" // implementation of class methods

#include "cencalvm/storage/Payload.h" // USES SCHEMA
#include "GridIngester.h" // USES GridIngester
#include "cencalvm/storage/ErrorHandler.h" // USES ErrorHandler

extern "C" {
#include "etree.h"
}

#include <fstream> // USES std::ifstream
#include <vector> // USES std::vector
#include <iostream> // USES std::cout

#include <time.h> // USES strftime(), gettimeofday(), localtime()
#include <unistd.h> // USES gethostname()

#include <sstream> // USES std::ostringstream
#include <assert.h> // USES assert()

// ----------------------------------------------------------------------
// Constructor
cencalvm::create::VMCreator::VMCreator(void) :
  _filenameParams(""),
  _filenameOut(""),
  _filenameTmp(""),
  _pErrHandler(new cencalvm::storage::ErrorHandler),
  _quiet(false)
{ // constructor
} // constructor

// ----------------------------------------------------------------------
// Destructor
cencalvm::create::VMCreator::~VMCreator(void)
{ // destructor
  delete _pErrHandler; _pErrHandler = 0;
} // destructor

// ----------------------------------------------------------------------
// Create and then pack the velocity model database.
void
cencalvm::create::VMCreator::run(void) const
{ // run
  assert(std::string("") != _filenameParams);
  assert(std::string("") != _filenameTmp);
  assert(std::string("") != _filenameOut);

  std::string* pGridFilenames = 0;
  int numGrids = 0;
  _readParams(&pGridFilenames, &numGrids);
  
  _createDB();
  _packDB();
} // run

// ----------------------------------------------------------------------
// Read parameter file.
void
cencalvm::create::VMCreator::_readParams(std::string** ppGridFilenames,
					 int* pNumGrids) const
{ // _readParams
  assert(0 != ppGridFilenames);
  assert(0 != pNumGrids);

  std::ifstream fin(_filenameParams.c_str());
  if (!fin.is_open()) {
    std::ostringstream msg;
    msg << "Could not open parameter file '" << _filenameParams
	<< "' for reading.";
    _pErrHandler->error(msg.str().c_str());
    return;
  } // if

  int numGrids = 0;
  int capacity = 16;
  std::vector<std::string> gridFilenames;
  gridFilenames.resize(capacity);
  while (!fin.eof() && fin.good()) {
    if (numGrids == capacity) {
      capacity *= 2;
      gridFilenames.resize(capacity);
    } // if
    assert(numGrids < capacity);
    std::string filename;
    fin >> filename;
    if (fin.good())
      gridFilenames[numGrids++] = filename;
  } // while

  fin.close();

  delete[] *ppGridFilenames; *ppGridFilenames = new std::string[numGrids];
  for (int iGrid=0; iGrid < numGrids; ++iGrid)
    (*ppGridFilenames)[iGrid] = gridFilenames[iGrid];
  *pNumGrids = numGrids;
} // _readParams

// ----------------------------------------------------------------------
// Create the velocity model database.
void
cencalvm::create::VMCreator::_createDB(void) const
{ // _createDB
  std::string* pGridFilenames = 0;
  int numGrids = 0;
  _readParams(&pGridFilenames, &numGrids);

  if (!_quiet)
    std::cout << "Preparing etree database for data..." << std::endl;
  const int cacheSize = 64;
  const int numDims = 3;
  const int payloadSize = sizeof(cencalvm::storage::PayloadStruct);
  etree_t* etreedb = etree_open(_filenameTmp.c_str(), 
				O_CREAT | O_TRUNC | O_RDWR, 
				cacheSize, payloadSize, numDims);
  if (0 == etreedb) {
    _pErrHandler->error("Could not create etree database.");
    return;
  } // if
  
  // Register schema
  if (0 != etree_registerschema(etreedb, cencalvm::storage::Payload::SCHEMA)) {
    _pErrHandler->error(etree_strerror(etree_errno(etreedb)));
    etree_close(etreedb);
    return;
  } // if
 
  // Create and then register metadata 
  const int maxLen = 128;
  char hostname[maxLen];
  gethostname(hostname, maxLen);
  time_t rawTime = time(0);
  const char* datetime = ctime(&rawTime);
  std::ostringstream metainfo;
  metainfo
    << "Central California Velocity Model\n"
    << "U.S. Geological Survey\n"
    << "created on: " << datetime
    << "host: "  << hostname;
  if (0 != etree_setappmeta(etreedb, metainfo.str().c_str())) {
    _pErrHandler->error(etree_strerror(etree_errno(etreedb)));
    etree_close(etreedb);
    return;
  } // if

  if (!_quiet)
    std::cout
      << "Finished preparing database. Starting processing of " << numGrids
      << " files." << std::endl;

  for (int iGrid=0; iGrid < numGrids; ++iGrid)
    GridIngester::addGrid(&etreedb, pGridFilenames[iGrid].c_str(),
			  *_pErrHandler, _quiet);
  
  if (0 != etree_close(etreedb))
    _pErrHandler->error("Could not close etree database.");
} // _createDB

// ----------------------------------------------------------------------
// Create packed etree database from unpacked etree database
void
cencalvm::create::VMCreator::_packDB(void) const
{ // _packDB
  if (!_quiet)
    std::cout << "Packing etree database..." << std::endl;

  const int cacheSize = 64;
  const int numDims = 3;
  const int payloadSize = sizeof(cencalvm::storage::PayloadStruct);
  etree_t* unpackeddb = etree_open(_filenameTmp.c_str(), O_RDONLY,
				   cacheSize, payloadSize, numDims);
  if (0 == unpackeddb) {
    _pErrHandler->error("Could not open unpacked etree database.");
    return;
  } // if
  
  etree_t* packeddb = etree_open(_filenameOut.c_str(),
				 O_CREAT | O_TRUNC | O_RDWR,
				 cacheSize, payloadSize, numDims);
  if (0 == packeddb) {
    _pErrHandler->error("Could not open packed etree database.");
    return;
  } // if
  
  if (0 != etree_registerschema(packeddb, cencalvm::storage::Payload::SCHEMA)) {
    _pErrHandler->error(etree_strerror(etree_errno(packeddb)));
    return;
  } // if
  
  char* appmeta = 0;
  if (0 != (appmeta = etree_getappmeta(unpackeddb)))
    if (0 != etree_setappmeta(packeddb, appmeta)) {
      _pErrHandler->error(etree_strerror(etree_errno(packeddb)));
      return;
    } // if
  free(appmeta);
  
  etree_addr_t addr;
  addr.x = 0;
  addr.y = 0;
  addr.z = 0;
  addr.level = 0;
  if (0 != etree_initcursor(unpackeddb, addr)) {
    _pErrHandler->error(etree_strerror(etree_errno(unpackeddb)));
    return;
  } // if
  
  if (0 != etree_beginappend(packeddb, 1)) {
    _pErrHandler->error(etree_strerror(etree_errno(packeddb)));
    return;
  } // if
  
  do {
    cencalvm::storage::PayloadStruct payload;
    
    if (0 != etree_getcursor(unpackeddb, &addr, "*", &payload)) {
      _pErrHandler->error(etree_strerror(etree_errno(unpackeddb)));
      return;
    } // if
    
    if (0 != etree_append(packeddb, addr, &payload)) {
      _pErrHandler->error(etree_strerror(etree_errno(packeddb)));
      return;
    } // if
  } while (0 == etree_advcursor(unpackeddb));
  
  if (0 != etree_endappend(packeddb)) {
    _pErrHandler->error(etree_strerror(etree_errno(packeddb)));
    return;
  } // if
  
  if (0 != etree_close(unpackeddb)) {
    _pErrHandler->error(etree_strerror(etree_errno(unpackeddb)));
    return;
  } // if
  
  if (0 != etree_close(packeddb)) {
    _pErrHandler->error(etree_strerror(etree_errno(packeddb)));
    return; 
  } // if

  if (!_quiet)
    std::cout << "Done packing etree database." << std::endl;
} // _packDB

// version
// $Id$

// End of file 
