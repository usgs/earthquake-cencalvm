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
  CPPUNIT_ASSERT(0 == strcmp(filename, creator._filenameParams));
} // testFilenameParams

// ----------------------------------------------------------------------
// Test filenameOut()
void
cencalvm::create::TestVMCreator::testFilenameOut(void)
{ // testFilenameOut
  const char* filename = "xyz";

  VMCreator creator;
  creator.filenameOut(filename);
  CPPUNIT_ASSERT(0 == strcmp(filename, creator._filenameOut));
} // testFilenameOut

// ----------------------------------------------------------------------
// Test filenameTmp()
void
cencalvm::create::TestVMCreator::testFilenameTmp(void)
{ // testFilenameTmp
  const char* filename = "lmn";

  VMCreator creator;
  creator.filenameTmp(filename);
  CPPUNIT_ASSERT(0 == strcmp(filename, creator._filenameTmp));
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

  CPPUNIT_ASSERT(false);
} // testRun

// version
// $Id$

// End of file 
