// -*- C++ -*-
//
// ----------------------------------------------------------------------
//
//                           Brad T. Aagaard
//                        U.S. Geological Survey
//
// <LicenseText>
//
// ----------------------------------------------------------------------
//

#include <portinfo>

#include "spatialdata/spatialdb/SpatialDB.h" // ISA SpatialDB object
#include "CenCalVMDB.h" // Implementation of class methods

#include "cencalvm/query/VMQuery.h" // USES VMQuery
#include "spatialdata/geocoords/CoordSys.h" // USES CoordSys
#include "spatialdata/geocoords/CSGeo.h" // USES CoordSys
#include "spatialdata/geocoords/Converter.h" // USES Converter
#include "cencalvm/storage/ErrorHandler.h" // USES ErrorHandler
#include "cencalvm/storage/Payload.h" // USES Payload::NODATAVAL

#include <stdexcept> // USES std::runtime_error

#if defined(USE_PYTHIA)
#include "journal/firewall.h" // USES FIREWALL
#include "pythiautil/FireWallUtil.h" // USES FIREWALL
#else
#define FIREWALL assert
#endif

// ----------------------------------------------------------------------
/// Default constructor
cencalvm::extensions::cencalvmdb::CenCalVMDB::CenCalVMDB(void) :
  _minVs(0),
  _vsVal(-1),
  _pQuery(new cencalvm::query::VMQuery),
  _pCS(new spatialdata::geocoords::CSGeo)
{ // constructor
  _pCS->ellipsoid("WGS84");
  _pCS->datumHoriz("WGS84");
  _pCS->datumVert("mean sea level");
  _pCS->isGeocentric(false);
  _pCS->toMeters(1.0);
  _pCS->initialize();
} // constructor

// ----------------------------------------------------------------------
/// Default destructor
cencalvm::extensions::cencalvmdb::CenCalVMDB::~CenCalVMDB(void)
{ // destructor
  delete _pQuery; _pQuery = 0;
  delete _pCS; _pCS = 0;
} // destructor

// ----------------------------------------------------------------------
// Open the database and prepare for querying.
void
cencalvm::extensions::cencalvmdb::CenCalVMDB::open(void)
{ // open
  FIREWALL(0 != _pQuery);

  _pQuery->open();
  if (storage::ErrorHandler::ERROR == _pQuery->errorHandler()->status())
    throw std::runtime_error(_pQuery->errorHandler()->message());
} // open

// ----------------------------------------------------------------------
// Close the database.
void
cencalvm::extensions::cencalvmdb::CenCalVMDB::close(void)
{ // close
  FIREWALL(0 != _pQuery);

  _pQuery->close(); 
  if (storage::ErrorHandler::ERROR == _pQuery->errorHandler()->status())
    throw std::runtime_error(_pQuery->errorHandler()->message());
} // close

// ----------------------------------------------------------------------
// Set query type.
void
cencalvm::extensions::cencalvmdb::CenCalVMDB::queryType(const query::VMQuery::QueryEnum queryType)
{ // queryType
  FIREWALL(0 != _pQuery);

  _pQuery->queryType(queryType);
  if (storage::ErrorHandler::ERROR == _pQuery->errorHandler()->status())
    throw std::runtime_error(_pQuery->errorHandler()->message());
} // queryType

// ----------------------------------------------------------------------
// Set values to be returned by queries.
void
cencalvm::extensions::cencalvmdb::CenCalVMDB::queryVals(const char** names,
							const int numVals)
{ // queryVals
  FIREWALL(0 != _pQuery);

  _pQuery->queryVals(names, numVals);
  if (storage::ErrorHandler::ERROR == _pQuery->errorHandler()->status())
    throw std::runtime_error(_pQuery->errorHandler()->message());

  _vsVal = -1;
  for (int i=0; i < numVals; ++i)
    if (0 == strcasecmp(names[i], "Vs")) {
      _vsVal = i;
      break;
    } // if
} // queryVals

// ----------------------------------------------------------------------
// Query the database.
int
cencalvm::extensions::cencalvmdb::CenCalVMDB::query(double** pVals,
					const int numVals,
					const double x,
					const double y,
					const double z,
			      const spatialdata::geocoords::CoordSys* pCSQuery)
{ // query
  FIREWALL(0 != _pQuery);

  double* pCoords = new double[3];
  const int numLocs = 1;
  pCoords[0] = x;
  pCoords[1] = y;
  pCoords[2] = z;

  spatialdata::geocoords::Converter::convert(&pCoords, numLocs, 
					     _pCS, pCSQuery);

  /** :KLUDGE:
   * Prevent elevations from being deeper than 45.0 km.
   */
  if (pCoords[2] < -44.95e+3)
    pCoords[2] = -44.95e+3;

  int err = 0;
  cencalvm::storage::ErrorHandler* pErrHandler = _pQuery->errorHandler();
  _pQuery->query(pVals, numVals, pCoords[0], pCoords[1], pCoords[2]);
  if (storage::ErrorHandler::ERROR == pErrHandler->status())
    throw std::runtime_error(pErrHandler->message());

  if (_vsVal >= 0) {
    double* pVs = &(*pVals)[_vsVal];
    int iter = 1;
    while (*pVs < 0.0) {
      const int maxIter = 8;
      const double elevDiff = 25.0;
      pErrHandler->resetStatus();
      const double newElev = pCoords[2] - iter*elevDiff;
      _pQuery->query(pVals, numVals, pCoords[0], pCoords[1], newElev);
      if (iter < maxIter)
	++iter;
      else
	break;
    } // while
    if (0.0 < *pVs && *pVs < _minVs)
      *pVs = _minVs;
  } // if

  if (cencalvm::storage::ErrorHandler::WARNING == pErrHandler->status()) {
    err = 1;
    pErrHandler->resetStatus();
  } // if
  
  delete[] pCoords; pCoords = 0;

  return err;
} // query

// version
// $Id$

// End of file 
