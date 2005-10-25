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

#if defined(USE_PYTHIA)
#include "journal/firewall.h" // USES FIREWALL
#include "pythiautil/FireWallUtil.h" // USES FIREWALL
#else
#define FIREWALL assert
#endif

// ----------------------------------------------------------------------
/// Default constructor
cencalvm::extensions::cencalvmdb::CenCalVMDB::CenCalVMDB(void) :
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
// Query the database.
void
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
  spatialdata::geocoords::Converter::convert(&pCoords, numLocs, _pCS, pCSQuery);

  _pQuery->query(pVals, numVals, pCoords[0], pCoords[1], pCoords[2]);
  delete[] pCoords; pCoords = 0;
} // query

// version
// $Id$

// End of file 
