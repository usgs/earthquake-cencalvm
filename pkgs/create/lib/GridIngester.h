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

#if !defined(cencalvm_create_gridingester_h)
#define cencalvm_create_gridingester_h

#include "cencalvm/storage/etreefwd.h" // USES etree_t

namespace cencalvm {
  namespace create {
    class GridIngester;
  }; // namespace create
}; // namespace cencalvm

class cencalvm::create::GridIngester
{ // GridIngester

public :
  // PUBLIC METHODS /////////////////////////////////////////////////////

  /** Add gridded data to database.
   *
   * @param pDB Pointer to database
   * @param filename Filename of gridded data
   */
  static void addGrid(etree_t** pDB,
		      const char* filename);
}; // GridIngester

#endif // cencalvm_create_gridingester  

// version
// $Id$

// End of file 
