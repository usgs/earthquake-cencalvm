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
#include "cencalvm/storage/ErrorHandler.h" // USES ErrorHandler

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
  query.filename(_FILENAME);
  CPPUNIT_ASSERT(0 == strcmp(_FILENAME, query._filename.c_str()));
} // testFilename

// ----------------------------------------------------------------------
// Test open(), close()
void
cencalvm::query::TestVMQuery::testOpenClose(void)
{ // testOpenClose
  VMQuery query;
  query.filename(_FILENAME);
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
  VMQuery query;
  query.filename(_FILENAME);
  query.queryType(cencalvm::query::VMQuery::MAXRES);
  query.open();

  const int numVals = 8;
  double* pVals = (numVals > 0) ? new double[numVals] : 0;
  query.query(&pVals, numVals, _LONLATELEV[0], _LONLATELEV[1], _LONLATELEV[2]);

  const double tolerance = 1.0e-6;
  for (int i=0; i < numVals; ++i)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, pVals[i]/_VALUESMAX[i], tolerance);

  query.close();
} // testQueryMax

// ----------------------------------------------------------------------
// Test query() with fixed query
void 
cencalvm::query::TestVMQuery::testQueryFixed(void)
{ // testQueryFixed
  VMQuery query;
  query.filename(_FILENAME);
  query.queryType(cencalvm::query::VMQuery::FIXEDRES);
  query.open();

  CPPUNIT_ASSERT(false);

  query.close();
} // testQueryFixed

// ----------------------------------------------------------------------
// Test query() with avg query
void 
cencalvm::query::TestVMQuery::testQueryAvg(void)
{ // testQueryAvg
  VMQuery query;
  query.filename(_FILENAME);
  query.queryType(cencalvm::query::VMQuery::AVGRES);
  query.open();

  CPPUNIT_ASSERT(false);

  query.close();
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

// version
// $Id$

// End of file 
