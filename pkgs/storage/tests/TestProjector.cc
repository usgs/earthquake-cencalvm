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

#include "TestProjector.h" // Implementation of class methods

#include "../lib/Projector.h" // USES Projector

// ----------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION( cencalvm::storage::TestProjector );

// ----------------------------------------------------------------------
#include "data/TestProjector.dat"

// ----------------------------------------------------------------------
// Test constructor
void
cencalvm::storage::TestProjector::testConstructor(void)
{ // testConstructor
  Projector proj;
} // testConstructor

// ----------------------------------------------------------------------
// Test project()
void
cencalvm::storage::TestProjector::testProject(void)
{ // testProject
  Projector proj;
  const int numLocs = _NUMLOCS;
  for (int iLoc=0; iLoc < numLocs; ++iLoc) {
    double x = 0;
    double y = 0;
    const double* pLonLat = &_LONLAT[2*iLoc];
    proj.project(&x, &y, pLonLat[0], pLonLat[1]);
    const double* pXY = &_XY[2*iLoc];
    const double tolerance = 1.0e-6;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(x/pXY[0], 1.0, tolerance);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(y/pXY[1], 1.0, tolerance);
  } // for
} // testProject

// version
// $Id$

// End of file 
