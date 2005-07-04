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

#include "cencalvm/storage/Geometry.h" // USES Projector

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
  Geometry geom;
} // testConstructor

// ----------------------------------------------------------------------
// Test lonLatElevToAddr()
void 
cencalvm::storage::TestGeometry::testAddress(void)
{ // testAddress
  Geometry geom;

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
} // testAddress

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
// Test findParent()
void 
cencalvm::storage::TestGeometry::testFindParent(void)
{ // testFindParent

  const int numTests = 3;
  const int pLevelsChild[] = { 2, 3, 4 };
  const int pAddrChild[] = { 0, 0, 0,
			     1, 0, 2,
			     53, 88, 19 };
  const int pAddrParent[] = { 0, 0, 0,
			      0, 0, 1,
			      26, 44, 9 };

  for (int iTest=0, i=0; iTest < numTests; ++iTest, i+=3) {
    etree_addr_t child;
    child.level = pLevelsChild[iTest];
    etree_tick_t tickLen = 0x80000000 >> child.level;
    child.x = pAddrChild[i  ]*tickLen;
    child.y = pAddrChild[i+1]*tickLen;
    child.z = pAddrChild[i+2]*tickLen;

    etree_addr_t parent;
    etree_tick_t parentLevel = pLevelsChild[iTest] - 1;
    tickLen = 0x80000000 >> parentLevel;
    etree_tick_t x = pAddrParent[i  ]*tickLen;
    etree_tick_t  y = pAddrParent[i+1]*tickLen;
    etree_tick_t  z = pAddrParent[i+2]*tickLen;
    bool hasParent = Geometry::findParent(&parent, child);
    CPPUNIT_ASSERT(hasParent);
    CPPUNIT_ASSERT_EQUAL(x, parent.x);
    CPPUNIT_ASSERT_EQUAL(y, parent.y);
    CPPUNIT_ASSERT_EQUAL(z, parent.z);
    CPPUNIT_ASSERT_EQUAL((int) parentLevel, parent.level);
  } // for
} // testFindParent

// version
// $Id$

// End of file 
