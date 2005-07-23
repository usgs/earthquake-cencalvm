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

#include "TestAverager.h" // Implementation of class methods

#include "cencalvm/average/Averager.h" // USES Averager

#include "cencalvm/storage/Payload.h" // USES Payload
#include "cencalvm/storage/ErrorHandler.h" // USES ErrorHandler

extern "C" {
#include "etree.h" // USES etree
}

#include <iostream>
// ----------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION( cencalvm::average::TestAverager );

// ----------------------------------------------------------------------
#include "data/TestAverager.dat"

// ----------------------------------------------------------------------
// Test constructor
void
cencalvm::average::TestAverager::testConstructor(void)
{ // testConstructor
  Averager averager;
} // testConstructor

// ----------------------------------------------------------------------
// Test fillOctants()
void
cencalvm::average::TestAverager::testFillOctants(void)
{ // testFillOctants

  _createDB();

  Averager averager;
  averager.filenameIn(_DBFILENAMEIN);
  averager.filenameOut(_DBFILENAMEOUT);
  averager.average();

  const cencalvm::storage::ErrorHandler* pHandler = averager.errorHandler();
  if (cencalvm::storage::ErrorHandler::OK != pHandler->status()) {
    std::cerr << pHandler->message() << std::endl;
    CPPUNIT_ASSERT(false);
  } // if

  

} // testFillOctants

// ----------------------------------------------------------------------
// Create etree with desired number of octants.
void
cencalvm::average::TestAverager::_createDB(void) const
{ // _createDB
  etree_t* db = etree_open(_DBFILENAMEIN, O_CREAT|O_RDWR|O_TRUNC, 0, 0, 3);
  CPPUNIT_ASSERT(0 != db);

  int err = etree_registerschema(db, cencalvm::storage::SCHEMA);
  CPPUNIT_ASSERT(0 == err);

  const int numOctants = _NUMOCTANTSIN;
  for (int iOctant=0; iOctant < numOctants; ++iOctant) {
    
    etree_addr_t addr;
    addr.level = _LEVELS[iOctant];
    addr.type = ETREE_LEAF;

    const etree_tick_t tickLen = 0x80000000 >> addr.level;
    const int numCoords = 3;
    addr.x = tickLen * _COORDS[numCoords*iOctant  ];
    addr.y = tickLen * _COORDS[numCoords*iOctant+1];
    addr.z = tickLen * _COORDS[numCoords*iOctant+2];

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

    std::cout << "CREATEDB: "
	      << addr.x << " "
	      << addr.y << " "
	      << addr.z << " "
	      << addr.level << " "
	      << val
	      << std::endl;

    err = etree_insert(db, addr, &payload);
    CPPUNIT_ASSERT(0 == err);
  } // for

  err = etree_close(db);
  CPPUNIT_ASSERT(0 == err);
} // _createDB

// version
// $Id$

// End of file 
