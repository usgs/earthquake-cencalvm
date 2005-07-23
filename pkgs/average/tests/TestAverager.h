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

/** @file tests/TestAverager.h
 *
 * @brief C++ TestAverager object
 *
 * C++ unit testing for TestAverager.
 */

#if !defined(cencalvm_storage_testvmquery_h)
#define cencalvm_storage_testvmquery_h

#include <cppunit/extensions/HelperMacros.h>
#include "cencalvm/storage/etreefwd.h" // HOLDSA etree_t

namespace cencalvm {
  namespace average {
    class TestAverager;
  }; // query
}; // cencalvm

/// C++ unit testing for Averager
class cencalvm::average::TestAverager : public CppUnit::TestFixture
{ // class TestAverager

  // CPPUNIT TEST SUITE /////////////////////////////////////////////////
  CPPUNIT_TEST_SUITE( TestAverager );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testFillOctants1 );
  CPPUNIT_TEST_SUITE_END();

  // PUBLIC METHODS /////////////////////////////////////////////////////
public :

  /// Test constructor
  void testConstructor(void);

  /// Test fillOctants() with 1 octant
  void testFillOctants1(void);

  /// Test fillOctants() with 2 octants
  void testFillOctants2(void);

  /// Test fillOctants() with 3 octants
  void testFillOctants3(void);

  /// Test fillOctants() with 4 octants
  void testFillOctants4(void);

  /// Test fillOctants() with 5 octants
  void testFillOctants5(void);

  /// Test fillOctants() with 6 octants
  void testFillOctants6(void);

  /// Test fillOctants() with 7 octants
  void testFillOctants7(void);

  /// Test fillOctants() with 8 octants
  void testFillOctants8(void);

  // PRIVATE MEMBERS ////////////////////////////////////////////////////
private :

  /** Create etree with desired number of octants.
   *
   * @param size Number of octants in etree
   *
   * @returns Average value of octant.
   */
  double _createDB(const int size) const;

  // PRIVATE METHODS ////////////////////////////////////////////////////
private :

  static const int _LEVEL; ///< Level of octants in database
  static const etree_tick_t _XTICK; ///< x tick of first octant
  static const etree_tick_t _YTICK; ///< y tick of first octant
  static const etree_tick_t _ZTICK; ///< z tick of first octant
  static const char* _DBFILENAMEIN; ///< Filename of input etree database
  static const char* _DBFILENAMEOUT; ///< Filename of output etree database
  static const double _VALS[]; ///< Relative values of payload

}; // class TestAverager

#endif // cencalvm_query_testvmquery

// version
// $Id$

// End of file 
