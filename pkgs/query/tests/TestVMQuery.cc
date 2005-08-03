// -*- C++ -*-
//
// ----------------------------------------------------------------------
//
//                           Brad T. Aagaard
//                        U.S. Geological Survey
//
// {LicenseText}
//
// ----------------------------------------------------------------------
//

#include "TestVMQuery.h" // Implementation of class methods

#include "cencalvm/query/VMQuery.h" // USES VMQuery
#include "cencalvm/average/Averager.h" // USES Averager
#include "cencalvm/storage/Geometry.h" // USES Averager
#include "cencalvm/storage/ErrorHandler.h" // USES ErrorHandler

extern "C" {
#include "etree.h"
}

// ----------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION( cencalvm::query::TestVMQuery );

// ----------------------------------------------------------------------
#include "data/TestVMQuery.dat"

// ----------------------------------------------------------------------
// Test constructor
void
cencalvm::query::TestVMQuery::testConstructor(void)
{ // testConstructor
  VMQuery query;
} // testConstructor

// ----------------------------------------------------------------------
// Test filename()
void
cencalvm::query::TestVMQuery::testFilename(void)
{ // testFilename
  VMQuery query;
  query.filename(_DBFILENAME);
  CPPUNIT_ASSERT(0 == strcmp(_DBFILENAME, query._filename.c_str()));
} // testFilename

// ----------------------------------------------------------------------
// Test open(), close()
void
cencalvm::query::TestVMQuery::testOpenClose(void)
{ // testOpenClose
  _createDB();  

  VMQuery query;
  query.filename(_DBFILENAME);
  query.open();
  CPPUNIT_ASSERT(0 != query._db);
  query.close();
  CPPUNIT_ASSERT(0 == query._db);
} // testOpenClose

// ----------------------------------------------------------------------
// Test queryType()
void
cencalvm::query::TestVMQuery::testQueryType(void)
{ // testQueryType
  VMQuery query;

  // default should be max
  CPPUNIT_ASSERT(&cencalvm::query::VMQuery::_queryMax == query._queryFn);

  cencalvm::query::VMQuery::QueryEnum queryType = 
    cencalvm::query::VMQuery::FIXEDRES;
  query.queryType(queryType);
  CPPUNIT_ASSERT(&cencalvm::query::VMQuery::_queryFixed == query._queryFn);

  queryType = cencalvm::query::VMQuery::AVGRES;
  query.queryType(queryType);
  CPPUNIT_ASSERT(&cencalvm::query::VMQuery::_queryAvg == query._queryFn);

  queryType = cencalvm::query::VMQuery::MAXRES;
  query.queryType(queryType);
  CPPUNIT_ASSERT(&cencalvm::query::VMQuery::_queryMax == query._queryFn);
} // testQueryType

// ----------------------------------------------------------------------
// Test queryVals()
void
cencalvm::query::TestVMQuery::testQueryVals(void)
{ // testQueryVals
  VMQuery query;

  // default should be all values
  const int defaultNumVals = 8;
  CPPUNIT_ASSERT(0 != query._pQueryVals);
  for (int i=0; i < defaultNumVals; ++i)
    CPPUNIT_ASSERT_EQUAL(i, query._pQueryVals[i]);


  const int numVals = 3;
  const char* pNames[] = { "Vp", "Zone", "Qs" };
  const int pVals[] = { 0, 7, 4 };
  query.queryVals(pNames, numVals);
  CPPUNIT_ASSERT_EQUAL(numVals, query._querySize);
  for (int i=0; i < numVals; ++i)
    CPPUNIT_ASSERT_EQUAL(pVals[i], query._pQueryVals[i]);
} // testQueryVals

// ----------------------------------------------------------------------
// Test cacheSize()
void
cencalvm::query::TestVMQuery::testCacheSize(void)
{ // testCacheSize
  VMQuery query;

  // default should be 128
  const int defaultSize = 128;
  CPPUNIT_ASSERT_EQUAL(defaultSize, query._cacheSize);

  const int cacheSize = 523;
  query.cacheSize(cacheSize);
  CPPUNIT_ASSERT_EQUAL(cacheSize, query._cacheSize);
} // testQueryVals

// ----------------------------------------------------------------------
// Test query() with max query
void 
cencalvm::query::TestVMQuery::testQueryMax(void)
{ // testQuerymax
  _createDB();

  VMQuery query;
  query.filename(_DBFILENAME);
  query.queryType(cencalvm::query::VMQuery::MAXRES);
  query.open();

  const cencalvm::storage::ErrorHandler* pHandler = query.errorHandler();

  const int numVals = 8;
  double* pVals = (numVals > 0) ? new double[numVals] : 0;

  double* pLonLatElev = 0;
  _dbLonLatElev(&pLonLatElev);
  const int numLocs = _NUMOCTANTSLEAF;
  for (int iLoc=0, i=0; iLoc < numLocs; ++iLoc, i+=3) {
    query.query(&pVals, numVals, 
		pLonLatElev[i  ], pLonLatElev[i+1], pLonLatElev[i+2]);
    
    const double tolerance = 1.0e-06;
    const double val = _OCTVALS[iLoc];
    for (int iVal=0; iVal < 6; ++iVal) {
      const double  valE = _RELPAY[iVal]*val;
      CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, pVals[iVal]/valE, tolerance);
    } // for
    int iVal = 6; // Block
    double valE = (iLoc < _NUMOCTANTSLEAF) ? 
      _RELPAY[iVal] :
      cencalvm::storage::INTERIORBLOCK;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, pVals[iVal]/valE, tolerance);
    iVal = 7; // Zone
    valE = (iLoc < _NUMOCTANTSLEAF) ? 
      _RELPAY[iVal] :
      cencalvm::storage::INTERIORZONE;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, pVals[iVal]/valE, tolerance);
  } // for

  query.close();

  CPPUNIT_ASSERT(cencalvm::storage::ErrorHandler::OK == pHandler->status());

  delete[] pLonLatElev; pLonLatElev = 0;
} // testQueryMax

// ----------------------------------------------------------------------
// Test query() with fixed query
void 
cencalvm::query::TestVMQuery::testQueryFixed(void)
{ // testQueryFixed
  _createDB();

  VMQuery query;
  query.filename(_DBFILENAME);
  query.queryType(cencalvm::query::VMQuery::FIXEDRES);
  query.open();

  cencalvm::storage::ErrorHandler* pHandler = query.errorHandler();
//   pHandler->logFilename("fixed.log");

  const int numVals = 8;
  double* pVals = (numVals > 0) ? new double[numVals] : 0;

  double* pLonLatElev = 0;
  _dbLonLatElev(&pLonLatElev);

  const int pOctIndices[] = { 3, 13, 17, 10, 12, 14, 15, 18 };
  const int numLocs = 8;
  for (int iLoc=0; iLoc < numLocs; ++iLoc) {
    const int iOctant = pOctIndices[iLoc];

    const int numOctCoords = 4;
    const int level = _COORDS[numOctCoords*iOctant+3];
    const double res = cencalvm::storage::Geometry::edgeLen(level) /
      cencalvm::storage::Geometry::vertExag();
    
    query.queryRes(res);
    query.query(&pVals, numVals, 
		pLonLatElev[3*iOctant  ], 
		pLonLatElev[3*iOctant+1],
		pLonLatElev[3*iOctant+2]);
    
    const double tolerance = 1.0e-06;
    const double val = _OCTVALS[iOctant];
    for (int iVal=0; iVal < 6; ++iVal) {
      const double  valE = _RELPAY[iVal]*val;
      CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, pVals[iVal]/valE, tolerance);
    } // for
    int iVal = 6; // Block
    double valE = (iOctant < _NUMOCTANTSLEAF) ? 
      _RELPAY[iVal] :
      cencalvm::storage::INTERIORBLOCK;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, pVals[iVal]/valE, tolerance);
    iVal = 7; // Zone
    valE = (iOctant < _NUMOCTANTSLEAF) ? 
      _RELPAY[iVal] :
      cencalvm::storage::INTERIORZONE;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, pVals[iVal]/valE, tolerance);
  } // for

  query.close();

  CPPUNIT_ASSERT(cencalvm::storage::ErrorHandler::OK == pHandler->status());

  delete[] pLonLatElev; pLonLatElev = 0;
} // testQueryFixed

// ----------------------------------------------------------------------
// Test query() with avg query
void 
cencalvm::query::TestVMQuery::testQueryAvg(void)
{ // testQueryAvg
  _createDB();

  VMQuery query;
  query.filename(_DBFILENAME);
  query.queryType(cencalvm::query::VMQuery::AVGRES);
  query.open();
  cencalvm::storage::ErrorHandler* pHandler = query.errorHandler();
  //  pHandler->logFilename("avg.log");
  
  const int numVals = 8;
  double* pVals = (numVals > 0) ? new double[numVals] : 0;

  double* pLonLatElev = 0;
  _dbLonLatElev(&pLonLatElev);

  const int numLocs = 3;
  const int pOctIndices[] = { 1, 7, 11 };
  const double periodMin[] = { 800.0, 1.0, 4000.0 };
  const double octValsAvg[] = { 5.79375, 9.9, 5.421875 };
  const bool isResLeaf[] = { false, true, false };


  for (int iLoc=0; iLoc < numLocs; ++iLoc) {
    const int iOctant = pOctIndices[iLoc];
    const double res = periodMin[iLoc];
    
    query.queryRes(res);
    query.query(&pVals, numVals, 
		pLonLatElev[3*iOctant  ], 
		pLonLatElev[3*iOctant+1],
		pLonLatElev[3*iOctant+2]);
    
    const double tolerance = 1.0e-06;
    const double val = octValsAvg[iLoc];
    for (int iVal=0; iVal < 6; ++iVal) {
      const double  valE = _RELPAY[iVal]*val;
      CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, pVals[iVal]/valE, tolerance);
    } // for
    int iVal = 6; // Block
    double valE = 
      isResLeaf[iLoc] ? _RELPAY[iVal] : cencalvm::storage::INTERIORBLOCK;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, pVals[iVal]/valE, tolerance);
    iVal = 7; // Zone
    valE = isResLeaf[iLoc] ? _RELPAY[iVal] : cencalvm::storage::INTERIORZONE;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, pVals[iVal]/valE, tolerance);
  } // for

  query.close();

  CPPUNIT_ASSERT(cencalvm::storage::ErrorHandler::OK == pHandler->status());

  delete[] pLonLatElev; pLonLatElev = 0;
} // testQueryAvg

// ----------------------------------------------------------------------
// Test errorHandler()
void
cencalvm::query::TestVMQuery::testErrorHandler(void)
{ // testErrorHandler
  VMQuery query;
  const cencalvm::storage::ErrorHandler* pHandler = query.errorHandler();
  CPPUNIT_ASSERT(0 != pHandler);
  CPPUNIT_ASSERT_EQUAL(cencalvm::storage::ErrorHandler::OK,
		       pHandler->status());
} // testErrorHandler

// ----------------------------------------------------------------------
// Create etree with desired number of octants.
void
cencalvm::query::TestVMQuery::_createDB(void) const
{ // _createDB
  const char* filenameTmp = "data/leaf.etree";

  etree_t* db = etree_open(filenameTmp, O_CREAT|O_RDWR|O_TRUNC, 0, 0, 3);
  CPPUNIT_ASSERT(0 != db);

  int err = etree_registerschema(db, cencalvm::storage::SCHEMA);
  CPPUNIT_ASSERT(0 == err);

  const int numOctants = _NUMOCTANTSLEAF;
  const int numCoords = 4;
  for (int iOctant=0; iOctant < numOctants; ++iOctant) {
    
    etree_addr_t addr;
    addr.level = _COORDS[numCoords*iOctant+3];
    addr.type = ETREE_LEAF;

    const etree_tick_t tickLen = 0x80000000 >> addr.level;
    addr.x = tickLen * _COORDS[numCoords*iOctant  ];
    addr.y = tickLen * _COORDS[numCoords*iOctant+1];
    addr.z = tickLen * _COORDS[numCoords*iOctant+2];
    addr.t = 0;

    const double val = _OCTVALS[iOctant];
    cencalvm::storage::PayloadStruct payload;
    int i=0;
    payload.Vp = _RELPAY[i++]*val;
    payload.Vs = _RELPAY[i++]*val;
    payload.Density = _RELPAY[i++]*val;
    payload.Qp = _RELPAY[i++]*val;
    payload.Qs = _RELPAY[i++]*val;
    payload.DepthFreeSurf = _RELPAY[i++]*val;
    payload.FaultBlock = int(_RELPAY[i++]);
    payload.Zone = int(_RELPAY[i++]);

    err = etree_insert(db, addr, &payload);
    CPPUNIT_ASSERT(0 == err);
  } // for

  err = etree_close(db);
  CPPUNIT_ASSERT(0 == err);

  cencalvm::average::Averager averager;
  averager.filenameIn(filenameTmp);
  averager.filenameOut(_DBFILENAME);
  averager.quiet(true);
  averager.average();  

  const cencalvm::storage::ErrorHandler* pHandler = averager.errorHandler();
  CPPUNIT_ASSERT(0 != pHandler);
  CPPUNIT_ASSERT_EQUAL(cencalvm::storage::ErrorHandler::OK,
		       pHandler->status());
} // _createDB

// ----------------------------------------------------------------------
// Get lon/lat/elev of octants in database.
void
cencalvm::query::TestVMQuery::_dbLonLatElev(double** ppCoords) const
{ // _dbLonLatElev
  assert(0 != ppCoords);
  const int numCoords = 3;
  const int numOctants = _NUMOCTANTS;
  delete[] *ppCoords; *ppCoords = new double[numCoords*numOctants];

  const int numOctCoords = 4;
  for (int iOctant=0, i=0; iOctant < numOctants; ++iOctant) {
    etree_addr_t addr;
    addr.level = _COORDS[numOctCoords*iOctant+3];
    addr.type = ETREE_LEAF;

    const etree_tick_t tickLen = 0x80000000 >> addr.level;
    addr.x = tickLen * _COORDS[numOctCoords*iOctant  ];
    addr.y = tickLen * _COORDS[numOctCoords*iOctant+1];
    addr.z = tickLen * _COORDS[numOctCoords*iOctant+2];

    double lon = 0;
    double lat = 0;
    double elev = 0;
    cencalvm::storage::ErrorHandler errHandler;
    cencalvm::storage::Geometry geom(errHandler);
    geom.addrToLonLatElev(&lon, &lat, &elev, &addr);

    (*ppCoords)[i++] = lon;
    (*ppCoords)[i++] = lat;
    (*ppCoords)[i++] = elev;    
  } // for
} // _dbLonLatElev

// version
// $Id$

// End of file 
