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
  float DepthFreeSurf; ///< Depth wrt free surface (burial depth)
  int16_t FaultBlock; ///< Fault block identifier
  int16_t Zone; ///< Zone identifier
}; // struct PayloadStruct

namespace cencalvm {
  namespace storage {
    static const char* cencalvm::storage::SCHEMA = 
      "float32_t Vp; "
      "float32_t Vs; "
      "float32_t Density; "
      "float32_t Qp; "
      "float32_t Qs; "
      "float32_t DepthFreeSurf; "
      "int16_t FaultBlock; "
      "int16_t Zone; ";
  }; // namespace storage
}; // namespace cencalvm

#endif // cencalvm_storage_payload_h

// version
// $Id$

// End of file 
