// -*- C++ -*-
//
// ======================================================================
//
//                           Brad T. Aagaard
//                        U.S. Geological Survey
//
// {LicenseText}
//
// ======================================================================
//

/** @file lib/GridIngester.h
 *
 * @brief C++ object for reading input grids containing velocity model
 * data and inserting them into the database.
 */

#if !defined(cencalvm_create_gridingester_h)
#define cencalvm_create_gridingester_h

#include "cencalvm/storage/etreefwd.h" // USES etree_t

namespace cencalvm {
  namespace create {
    class GridIngester;
  }; // namespace create
  namespace storage {
    class ErrorHandler; // USES ErrorHandler
  }; // namespace storage
}; // namespace cencalvm

/// C++ object for reading input grids containing velocity model data
/// and inserting them into the database.
class cencalvm::create::GridIngester
{ // GridIngester

public :
  // PUBLIC METHODS /////////////////////////////////////////////////////

  /** Add gridded data to database.
   *
   * @param pDB Pointer to database
   * @param filename Filename of gridded data
   * @param errHandler Error handler
   */
  static void addGrid(etree_t** pDB,
		      const char* filename,
		      cencalvm::storage::ErrorHandler& errHandler);
}; // GridIngester

#endif // cencalvm_create_gridingester  

// version
// $Id$

// End of file 
