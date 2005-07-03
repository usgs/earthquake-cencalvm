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

/** @file tests/libtests/TestProjector.h
 *
 * @brief C++ TestProjector object
 *
 * C++ unit testing for TestProjector.
 */

#if !defined(cencalvm_storage_testprojector_h)
#define cencalvm_storage_testprojector_h

#include <cppunit/extensions/HelperMacros.h>

/// Namespace for spatialdata package
namespace cencalvm {
  namespace storage {
    class TestProjector;
  }; // storage
}; // cencalvm

/// C++ unit testing for Projector
class cencalvm::storage::TestProjector : public CppUnit::TestFixture
{ // class TestProjector

  // CPPUNIT TEST SUITE /////////////////////////////////////////////////
  CPPUNIT_TEST_SUITE( TestProjector );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testProject );
  CPPUNIT_TEST_SUITE_END();

  // PUBLIC METHODS /////////////////////////////////////////////////////
public :

  /// Test constructor
  void testConstructor(void);

  /// Test project()
  void testProject(void);

  // PRIVATE METHODS ////////////////////////////////////////////////////
private :

  static const double _XY[]; ///< Array of projected locations
  static const double _LONLAT[]; ///< Array of geographic locations
  static const int _NUMLOCS; ///< Number of locations

}; // class TestProjector

#endif // cencalvm_storgage_testprojector

// version
// $Id$

// End of file 
