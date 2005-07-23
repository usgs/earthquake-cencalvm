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
const char* cencalvm::average::TestAverager::_DBFILENAMEIN = "in.etree";
const char* cencalvm::average::TestAverager::_DBFILENAMEOUT = "out.etree";
const int cencalvm::average::TestAverager::_LEVEL = 2;
const etree_tick_t cencalvm::average::TestAverager::_XTICK = 2;
const etree_tick_t cencalvm::average::TestAverager::_YTICK = 0;
const etree_tick_t cencalvm::average::TestAverager::_ZTICK = 2;
const double cencalvm::average::TestAverager::_VALS[] = {
  10.0, 1.0, 0.1, 0.01, 0.001, 100.0, 1.0, 1.0
};

// ----------------------------------------------------------------------
// Test constructor
void
cencalvm::average::TestAverager::testConstructor(void)
{ // testConstructor
  Averager averager;
} // testConstructor

// ----------------------------------------------------------------------
// Test fillOctants() with 1 octant
void
cencalvm::average::TestAverager::testFillOctants1(void)
{ // testFillOctants1
  const int numOctants = 1;
  const double val = _createDB(numOctants);

  Averager averager;
  averager.filenameIn(_DBFILENAMEIN);
  averager.filenameOut(_DBFILENAMEOUT);
  averager.average();

  const cencalvm::storage::ErrorHandler* pHandler = averager.errorHandler();
  if (cencalvm::storage::ErrorHandler::OK != pHandler->status()) {
    std::cerr << pHandler->message() << std::endl;
    CPPUNIT_ASSERT(false);
  } // if

  

} // testFillOctants1

// ----------------------------------------------------------------------
// Create etree with desired number of octants.
double
cencalvm::average::TestAverager::_createDB(const int size) const
{ // _createDB
  etree_t* db = etree_open(_DBFILENAMEIN, O_CREAT|O_RDWR|O_TRUNC, 0, 0, 3);
  CPPUNIT_ASSERT(0 != db);

  int err = etree_registerschema(db, cencalvm::storage::SCHEMA);
  CPPUNIT_ASSERT(0 == err);

  double sum = 0.0;
  for (int iOctant=0; iOctant < size; ++iOctant) {
    etree_addr_t addr;

    const etree_tick_t tickLen = 0x80000000 >> _LEVEL;

    addr.x = tickLen*(_XTICK + iOctant % 2);
    addr.y = tickLen*(_YTICK + (iOctant / 2) % 2);
    addr.z = tickLen*(_ZTICK + iOctant / 4);
    addr.level = _LEVEL;
    addr.type = ETREE_LEAF;

    std::cout << "CREATEDB: "
	      << addr.x << " "
	      << addr.y << " "
	      << addr.z << " "
	      << addr.level
	      << std::endl;

    cencalvm::storage::PayloadStruct payload;
    int i=0;
    const double val = 1.0+iOctant;
    sum += val;
    payload.Vp = _VALS[i++]*val;
    payload.Vs = _VALS[i++]*val;
    payload.Density = _VALS[i++]*val;
    payload.Qp = _VALS[i++]*val;
    payload.Qs = _VALS[i++]*val;
    payload.DepthFreeSurf = _VALS[i++]*val;
    payload.FaultBlock = int(_VALS[i++]);
    payload.Zone = int(_VALS[i++]);
    err = etree_insert(db, addr, &payload);
    CPPUNIT_ASSERT(0 == err);
  } // for

  err = etree_close(db);
  CPPUNIT_ASSERT(0 == err);

  return sum / size;
} // _createDB

// version
// $Id$

// End of file 
