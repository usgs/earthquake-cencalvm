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

/** @file lib/Geometry.h
 *
 * @brief C++ manager for geometry of the USGS central CA velocity
 * model.
 */

#if !defined(cencalvm_storage_geometry_h)
#define cencalvm_storage_geometry_h

#include "etreefwd.h" // USES etree types

#include <math.h> // USES log()

namespace cencalvm {
  namespace storage {
    class Geometry;
    class Projector; // forward declaration (Geometry HOLDSA Projector)
    class TestGeometry; // friend
  }; // namespace storage
}; // namespace cencalvm

/// C++ manager for geometry of the USGS central CA velocity
/// model.
class cencalvm::storage::Geometry
{ // Geometry
  friend class TestGeometry;

public :
  // PUBLIC METHODS /////////////////////////////////////////////////////

  /// Constructor
  Geometry(void);

  /// Destructor
  ~Geometry(void);

  /** Map global coordinates xyz to etree address. 
   *
   * @warning Level in etree must have been set in address.
   *
   * @param pAddr Pointer to etree address
   * @param lon Longitude of location in degrees
   * @param lat Latitude of location in degrees
   * @param elev Elevation of location wrt MSL in meters
   */
  void lonLatElevToAddr(etree_addr_t* pAddr,
			const double lon,
			const double lat,
			const double elev);
  
  /** Get length of octant edge.
   *
   * @param level Level in etree
   */
  static double edgeLen(const etree_tick_t level);

  /** Get level in etree corresponding to resolution.
   *
   * @param res Spatial resolution
   */
  static int level(const double res);

  /** Get address of parent octant if it exists.
   *
   * @param pParentAddr Pointer to address of parent octant
   * @param childAddr Address of current octant
   *
   * @returns True if found parent, false otherwise
   */
  static bool findParent(etree_addr_t* pParentAddr,
			 const etree_addr_t& childAddr);

  /** Get vertical exaggeration.
   *
   * @returns Vertical exaggeration
   */
  static double vertExag(void);

 private :
  // PRIVATE METHODS ////////////////////////////////////////////////////

  Geometry(const Geometry& g); ///< Not implemented
  const Geometry& operator=(const Geometry& g); ///< Not implemented
  
private :
  // PRIVATE MEMBERS ////////////////////////////////////////////////////

  /// Projected X coordinate of NW corner of root octant
  const double _projXNWRoot;

  /// Projected Y coordinate of NW corner of root octant
  const double _projYNWRoot;

  /// Projector for converting from lon/lat/elev to projected coordinates
  Projector* _pProj;
  
  /// Length of root octant
  static const double _ROOTLEN;

  /// Azimuth of NW to NE edge of octants
  static const double _AZ;

  /// Vertical exaggeration of octants
  static const double _VERTEXAG;

  /// Projected X coordinate of NW corner of detailed region
  static const double _PROJXNW;

  /// Projected Y coordinate of NW cornder of detailed region
  static const double _PROJYNW;

  /// Distant to NW from detailed region to NW edge of root octant
  static const double _BUFFERNW;

  /// Distant to SW from detailed region to SW edge of root octant
  static const double _BUFFERSW;

  /// Elevation (wrt MSL) or top of root octant
  static const double _MAXELEV;

}; // Geometry

#include "Geometry.icc" // inline methods

#endif // cencalvm_storage_geometry_h

// version
// $Id$

// End of file 
