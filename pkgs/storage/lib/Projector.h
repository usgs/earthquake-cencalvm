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

/** @file lib/Projector.h
 *
 * @brief C++ manager for projecting to/from 3-D geologic
 * model.
 */

#if !defined(cencalvm_storage_projector_h)
#define cencalvm_storage_projector_h

#include "proj4fwd.h" // HOLDSA projPJ

namespace cencalvm {
  namespace storage {
    class Projector;
  }; // namespace storage
}; // namespace cencalvm

/// C++ manager for projecting to/from 3-D geologic model.
class cencalvm::storage::Projector
{ // Projector

public :
  // PUBLIC METHODS /////////////////////////////////////////////////////

  /// Constructor
  Projector(void);

  /// Destructor
  ~Projector(void);

  /** Apply projection to geographic coordinates.
   *
   * @param pX Pointer to projected X coordinate
   * @param pY Pointer to projected Y coordinate
   * @param lon Longitude of location in degrees
   * @param lat Latitude of location in degrees
   */
  void project(double* pX,
	       double* pY,
	       const double lon,
	       const double lat) const;

private :
  // PRIVATE MEMBERS ////////////////////////////////////////////////////

  /// Handle to Proj4 projection
  projPJ _pProj; // Pointer to projection

  static const double _MERIDIAN; ///< Longitude of central meridian for proj
  static const double _LAT; ///< Latitude of origin for projection
  static const double _SCALE; ///< Scale factor for projection
  static const char* _PROJECTION; ///< Name of projection
  static const char* _DATUM; ///< Name of horizontal datum
  static const char* _UNITS; ///< Name of units used in projection

}; // Projector

#endif // cencalvm_storage_projector_h

// version
// $Id$

// End of file 
