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

/** @file lib/Payload.h
 *
 * @brief Definition of structure of velocity model data.
 */

#if !defined(cencalvm_storage_payload_h)
#define cencalvm_storage_payload_h

namespace cencalvm {
  namespace storage {
    struct PayloadStruct;
  }; // namespace storage
}; // namespace cencalvm

/// Data stored in velocity model database
struct cencalvm::storage::PayloadStruct {
  float Vp; ///< P wave speed in m/s
  float Vs; ///< S wave speed in m/s
  float Density; ///< Density in kg/m^3
  float Qp; ///< Q for P waves
  float Qs; ///< Q for S waves
  float DepthFreeSurf; ///< Depth wrt free surface (burial depth) in m

  /** Fault block identifier.
   *
   * @li Set to fault block for location in Earth Vision model
   * @li Set to 0 for locations outside the domain.
   * @li Set to -999 for interior octants that contain values
   *  that are the average of its children.
   */
  int16_t FaultBlock;

  /** Zone identifier. 
   *
   * @li Set to zone for location in Earth Vision model
   * @li Set to 0 for locations outside the domain.
   * @li Set to -999 for interior octants that contain values that are the
   *  averages of its children.
   */
  int16_t Zone;
}; // struct PayloadStruct

namespace cencalvm {
  namespace storage {
    /// Database schema
    static const char* cencalvm::storage::SCHEMA = 
      "float32_t Vp; "
      "float32_t Vs; "
      "float32_t Density; "
      "float32_t Qp; "
      "float32_t Qs; "
      "float32_t DepthFreeSurf; "
      "int16_t FaultBlock; "
      "int16_t Zone;";
    /// Database values for no data
    static const float cencalvm::storage::NODATAVAL = -999.0;
    static const short cencalvm::storage::NODATABLOCK = 0;
    static const short cencalvm::storage::NODATAZONE = 0;

    /// Database values for interior (average) octants
    static const short cencalvm::storage::INTERIORBLOCK = -999;
    static const short cencalvm::storage::INTERIORZONE = -999;
  }; // namespace storage
}; // namespace cencalvm

#endif // cencalvm_storage_payload_h

// version
// $Id$

// End of file 
