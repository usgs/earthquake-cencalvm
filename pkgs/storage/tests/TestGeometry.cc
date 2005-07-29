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

#include "TestGeometry.h" // Implementation of class methods

#include "cencalvm/storage/Geometry.h" // USES Geometry
#include "cencalvm/storage/ErrorHandler.h" // USES Geometry

extern "C" {
#include "etree.h"
}

// ----------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION( cencalvm::storage::TestGeometry );

// ----------------------------------------------------------------------
// Test constructor
void
cencalvm::storage::TestGeometry::testConstructor(void)
{ // testConstructor
  ErrorHandler errHandler;
  Geometry geom(errHandler);
  CPPUNIT_ASSERT(ErrorHandler::OK == errHandler.status());
} // testConstructor

// ----------------------------------------------------------------------
// Test lonLatElevToAddr()
void 
cencalvm::storage::TestGeometry::testAddress(void)
{ // testAddress
  ErrorHandler errHandler;
  Geometry geom(errHandler);

  const double lon = -123.8584929;
  const double lat = 38.424179;
  const double elev = 0.0;
  const double p = Geometry::_BUFFERNW+0.1;
  const double q = Geometry::_BUFFERSW+0.1;
  const double r = Geometry::_ROOTLEN - Geometry::_MAXELEV;

  const int numLevels = 6;
  for (int iLevel=0; iLevel < numLevels; ++iLevel) {
    const double res = Geometry::_ROOTLEN / ((etree_tick_t) 1 << iLevel);
    const etree_tick_t tickLen = 0x80000000 >> iLevel;

    etree_tick_t x = tickLen*int(p / res);
    etree_tick_t y = tickLen*int(q / res);
    etree_tick_t z = tickLen*int(r / res);
    
    etree_addr_t addr;
    addr.level = iLevel;
    geom.lonLatElevToAddr(&addr, lon, lat, elev);

    CPPUNIT_ASSERT_EQUAL(x, addr.x);
    CPPUNIT_ASSERT_EQUAL(y, addr.y);
    CPPUNIT_ASSERT_EQUAL(z, addr.z);
  } // for

  CPPUNIT_ASSERT(ErrorHandler::OK == errHandler.status());
} // testAddress

// ----------------------------------------------------------------------
// Test addrToLonLatElev()
void 
cencalvm::storage::TestGeometry::testInvAddress(void)
{ // testInvAddress
  ErrorHandler errHandler;
  Geometry geom(errHandler);

  const int level = 6;
  const double res = Geometry::_ROOTLEN / ((etree_tick_t) 1 << level);
  const etree_tick_t tickLen = 0x80000000 >> level;

  const double lonE = -123.653262;
  const double latE = 38.400998;
  const double elevE = 0.5*res/Geometry::_VERTEXAG;

  const double p = Geometry::_BUFFERNW + 0.5*res;
  const double q = Geometry::_BUFFERSW + 0.5*res;
  const double r = Geometry::_ROOTLEN +
    (0.5*res / Geometry::_VERTEXAG - Geometry::_MAXELEV) * Geometry::_VERTEXAG;

  etree_addr_t addr;
  addr.level = level;
  addr.x = tickLen*int(p / res);
  addr.y = tickLen*int(q / res);
  addr.z = tickLen*int(r / res);
    
  double lon = 0;
  double lat = 0;
  double elev = 0;
  geom.addrToLonLatElev(&lon, &lat, &elev, &addr);
  
  const double tolerance = 1.0e-6;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, lon/lonE, tolerance);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, lat/latE, tolerance);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, elev/elevE, tolerance);

  CPPUNIT_ASSERT(ErrorHandler::OK == errHandler.status());

  // double check by going lon/lat/elev back to addr
  etree_addr_t addrCheck;
  addrCheck.level = level;
  geom.lonLatElevToAddr(&addrCheck, lon, lat, elev);
  CPPUNIT_ASSERT_EQUAL(addr.x, addrCheck.x);
  CPPUNIT_ASSERT_EQUAL(addr.y, addrCheck.y);
  CPPUNIT_ASSERT_EQUAL(addr.z, addrCheck.z);

  CPPUNIT_ASSERT(ErrorHandler::OK == errHandler.status());
} // testInvAddress

// ----------------------------------------------------------------------
// Test edgeLen()
void 
cencalvm::storage::TestGeometry::testEdgeLen(void)
{ // testEdgeLen
  const double rootLen = Geometry::_ROOTLEN;
  const int numLevels = 19;
  double len = rootLen;
  for (int iLevel=0; iLevel < numLevels; ++iLevel, len /= 2.0)
    CPPUNIT_ASSERT_EQUAL(len, Geometry::edgeLen(iLevel));
} // testEdgeLen

// ----------------------------------------------------------------------
// Test level()
void 
cencalvm::storage::TestGeometry::testLevel(void)
{ // testLevel
  const double rootLen = Geometry::_ROOTLEN;
  const int numLevels = 19;
  double len = rootLen;
  for (int iLevel=0; iLevel < numLevels; ++iLevel, len /= 2.0) {
    CPPUNIT_ASSERT_EQUAL(iLevel, Geometry::level(len));
  } // for
} // testLevel

// ----------------------------------------------------------------------
// Test findAncestor()
void 
cencalvm::storage::TestGeometry::testFindAncestor(void)
{ // testFindAncestor

  const int numTests = 4;
  const int pLevelsChild[] = { 2, 3, 4, 4 };
  const int pAddrChild[] = { 0, 0, 0,
			     1, 0, 2,
			     53, 88, 19,
			     53, 88, 19 };
  const int pAddrParent[] = { 0, 0, 0,
			      0, 0, 1,
			      26, 44, 9,
                              13, 22, 4 };
  const int pLevelsParent[] = { 1, 2, 3, 2 };

  for (int iTest=0, i=0; iTest < numTests; ++iTest, i+=3) {
    etree_addr_t child;
    child.level = pLevelsChild[iTest];
    etree_tick_t tickLen = 0x80000000 >> child.level;
    child.x = pAddrChild[i  ]*tickLen;
    child.y = pAddrChild[i+1]*tickLen;
    child.z = pAddrChild[i+2]*tickLen;

    etree_addr_t parent;
    etree_tick_t parentLevel = pLevelsParent[iTest];
    tickLen = 0x80000000 >> parentLevel;
    etree_tick_t x = pAddrParent[i  ]*tickLen;
    etree_tick_t  y = pAddrParent[i+1]*tickLen;
    etree_tick_t  z = pAddrParent[i+2]*tickLen;
    Geometry::findAncestor(&parent, child, parentLevel);
    CPPUNIT_ASSERT_EQUAL(x, parent.x);
    CPPUNIT_ASSERT_EQUAL(y, parent.y);
    CPPUNIT_ASSERT_EQUAL(z, parent.z);
    CPPUNIT_ASSERT_EQUAL((int) parentLevel, parent.level);
  } // for
} // testFindAncestor

// version
// $Id$

// End of file 
