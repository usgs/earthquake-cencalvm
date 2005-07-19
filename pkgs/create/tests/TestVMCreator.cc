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

#include "TestVMCreator.h" // Implementation of class methods

#include "cencalvm/create/VMCreator.h" // USES VMCreator
#include "cencalvm/storage/Geometry.h" // USES Geometry
#include "cencalvm/storage/ErrorHandler.h" // USES ErrorHandler

extern "C" {
#include "etree.h"
}

#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream

// ----------------------------------------------------------------------
CPPUNIT_TEST_SUITE_REGISTRATION( cencalvm::create::TestVMCreator );

// ----------------------------------------------------------------------
#include "data/TestVMCreator.dat"

// ----------------------------------------------------------------------
// Test constructor
void
cencalvm::create::TestVMCreator::testConstructor(void)
{ // testConstructor
  VMCreator creator;
} // testConstructor

// ----------------------------------------------------------------------
// Test filenameParams()
void
cencalvm::create::TestVMCreator::testFilenameParams(void)
{ // testFilenameParams
  const char* filename = "abc";

  VMCreator creator;
  creator.filenameParams(filename);
  CPPUNIT_ASSERT_EQUAL(std::string(filename), creator._filenameParams);
} // testFilenameParams

// ----------------------------------------------------------------------
// Test filenameOut()
void
cencalvm::create::TestVMCreator::testFilenameOut(void)
{ // testFilenameOut
  const char* filename = "xyz";

  VMCreator creator;
  creator.filenameOut(filename);
  CPPUNIT_ASSERT_EQUAL(std::string(filename), creator._filenameOut);
} // testFilenameOut

// ----------------------------------------------------------------------
// Test filenameTmp()
void
cencalvm::create::TestVMCreator::testFilenameTmp(void)
{ // testFilenameTmp
  const char* filename = "lmn";

  VMCreator creator;
  creator.filenameTmp(filename);
  CPPUNIT_ASSERT_EQUAL(std::string(filename), creator._filenameTmp);
} // testFilenameTmp

// ----------------------------------------------------------------------
// Test run()
void 
cencalvm::create::TestVMCreator::testRun(void)
{ // testRun
  const char* filenameParams = "data/paramfile.txt";
  const char* filenameOut = "data/one.etree";
  const char* filenameTmp = "data/tmp.etree";

  VMCreator creator;
  creator.filenameParams(filenameParams);
  creator.filenameOut(filenameOut);
  creator.filenameTmp(filenameTmp);
  creator.run();


  const double p = 409612.5;
  const double q = 204812.5;
  const double r = 1587187.5;
  const etree_tick_t level = 3;
  const double res = cencalvm::storage::Geometry::edgeLen(level);
  const etree_tick_t tickLen = 0x80000000 >> level;
  etree_addr_t addr;
  addr.x = tickLen*int(p / res);
  addr.y = tickLen*int(q / res);
  addr.z = tickLen*int(r / res);
  addr.level = level;

  etree_t* db = etree_open(filenameOut, O_RDONLY, 0, 0, 0);
  if (0 == db) {
    std::ostringstream msg;
    msg << "Could not open etree database '" << filenameOut << "'.";
    throw std::runtime_error(msg.str());
  } // if

  etree_addr_t resaddr;
  cencalvm::storage::PayloadStruct payload;
  if (0 != etree_search(db, addr, &resaddr, "*", &payload))
    throw std::runtime_error(etree_strerror(etree_errno(db)));
  CPPUNIT_ASSERT_EQUAL(addr.x, resaddr.x);
  CPPUNIT_ASSERT_EQUAL(addr.y, resaddr.y);
  CPPUNIT_ASSERT_EQUAL(addr.z, resaddr.z);
  CPPUNIT_ASSERT_EQUAL(addr.level, resaddr.level);

  const double tolerance = 1.0e-06;
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, _PAYLOAD.Vp/payload.Vp, tolerance);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, _PAYLOAD.Vs/payload.Vs, tolerance);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, _PAYLOAD.Density/payload.Density, 
			       tolerance);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, _PAYLOAD.Qp/payload.Qp, tolerance);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, _PAYLOAD.Qs/payload.Qs, tolerance);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, 
			       _PAYLOAD.DepthFreeSurf/payload.DepthFreeSurf,
			       tolerance);
  CPPUNIT_ASSERT_EQUAL(_PAYLOAD.FaultBlock, payload.FaultBlock);
  CPPUNIT_ASSERT_EQUAL(_PAYLOAD.Zone, payload.Zone);
} // testRun

// ----------------------------------------------------------------------
// Test errorHandler()
void
cencalvm::create::TestVMCreator::testErrorHandler(void)
{ // testErrorHandler
  VMCreator creator;
  const cencalvm::storage::ErrorHandler* pHandler = creator.errorHandler();
  CPPUNIT_ASSERT(0 != pHandler);
  CPPUNIT_ASSERT_EQUAL(cencalvm::storage::ErrorHandler::OK,
		       pHandler->status());
} // testErrorHandler

// version
// $Id$

// End of file 
