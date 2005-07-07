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

extern "C" {
#include "etree.h"
}

#include <fstream> // USES std::ifstream
#include <vector> // USES std::vector
#include <iostream> // USES std::cout

#include <time.h> // USES strftime(), gettimeofday(), localtime()
#include <unistd.h> // USES gethostname()

#include <sstream> // USES std::ostringstream
#include <stdexcept> // USES std::runtime_error
#include <assert.h> // USES assert()

// ----------------------------------------------------------------------
// Constructor
cencalvm::create::VMCreator::VMCreator(void) :
  _filenameParams(""),
  _filenameOut(""),
  _filenameTmp("")
{ // constructor
} // constructor

// ----------------------------------------------------------------------
// Destructor
cencalvm::create::VMCreator::~VMCreator(void)
{ // destructor
} // destructor

// ----------------------------------------------------------------------
// Create and then pack the velocity model database.
void
cencalvm::create::VMCreator::run(void) const
{ // run
  assert(0 != strcmp("", _filenameParams));
  assert(0 != strcmp("", _filenameTmp));
  assert(0 != strcmp("", _filenameOut));

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

  std::ifstream fin(_filenameParams);
  if (!fin.is_open()) {
    std::ostringstream msg;
    msg << "Could not open parameter file '" << _filenameParams
	<< "' for reading.";
    throw std::runtime_error(msg.str());
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

  std::cout << "Preparing etree database for data..." << std::endl;
  etree_t* etreedb = etree_open(_filenameTmp, O_CREAT | O_TRUNC | O_RDWR, 
				0, 0, 3);
  if (0 == etreedb)
    throw std::runtime_error("Could not create etree database.");
  
  // Register schema
  if (0 != etree_registerschema(etreedb, cencalvm::storage::SCHEMA))
    throw std::runtime_error(etree_strerror(etree_errno(etreedb)));
 
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
  if (0 != etree_setappmeta(etreedb, metainfo.str().c_str()))
    throw std::runtime_error(etree_strerror(etree_errno(etreedb)));

  std::cout
    << "Finished preparing database. Starting processing of " << numGrids
    << " files." << std::endl;
  try {
    for (int iGrid=0; iGrid < numGrids; ++iGrid)
      GridIngester::addGrid(&etreedb, pGridFilenames[iGrid].c_str());
  } // try
  catch (std::exception& err) {
    if (0 != etree_close(etreedb))
      throw std::runtime_error("Could not close etree database.");
    throw;
  } // catch
  catch (...) {
    if (0 != etree_close(etreedb))
      throw std::runtime_error("Could not close etree database.");
    throw;
  } // catch

  if (0 != etree_close(etreedb))
    throw std::runtime_error("Could not close etree database.");       
} // _createDB

// ----------------------------------------------------------------------
// Create packed etree database from unpacked etree database
void
cencalvm::create::VMCreator::_packDB(void) const
{ // _packDB
  etree_t* unpackeddb = etree_open(_filenameTmp, O_RDONLY, 0, 0, 0);
  if (0 == unpackeddb)
    throw std::runtime_error("Could not open unpacked etree database.");
  
  etree_t* packeddb = etree_open(_filenameOut, O_CREAT | O_TRUNC | O_RDWR,
				 0, 0, 3);
  if (0 == packeddb)
    throw std::runtime_error("Could not open packed etree database.");
  
  if (0 != etree_registerschema(packeddb, cencalvm::storage::SCHEMA))
    throw std::runtime_error(etree_strerror(etree_errno(packeddb)));
  
  char* appmeta = 0;
  if (0 != (appmeta = etree_getappmeta(unpackeddb)))
    if (0 != etree_setappmeta(packeddb, appmeta))
      throw std::runtime_error(etree_strerror(etree_errno(packeddb)));
  free(appmeta);
  
  etree_addr_t addr;
  addr.x = 0;
  addr.y = 0;
  addr.z = 0;
  addr.level = 0;
  if (0 != etree_initcursor(unpackeddb, addr))
    throw std::runtime_error(etree_strerror(etree_errno(unpackeddb)));
  
  if (0 != etree_beginappend(packeddb, 1))
    throw std::runtime_error(etree_strerror(etree_errno(packeddb)));
  
  do {
    cencalvm::storage::PayloadStruct payload;
    
    if (0 != etree_getcursor(unpackeddb, &addr, "*", &payload))
      throw std::runtime_error(etree_strerror(etree_errno(unpackeddb)));
    
    if (0 != etree_append(packeddb, addr, &payload))
      throw std::runtime_error(etree_strerror(etree_errno(packeddb)));
  } while (0 == etree_advcursor(unpackeddb));
  
  if (0 != etree_endappend(packeddb))
    throw std::runtime_error(etree_strerror(etree_errno(packeddb)));
  
  if (0 != etree_close(unpackeddb))
    throw std::runtime_error(etree_strerror(etree_errno(unpackeddb)));
  
  if (0 != etree_close(packeddb))
    throw std::runtime_error(etree_strerror(etree_errno(packeddb)));
} // _packDB

// version
// $Id$

// End of file 
