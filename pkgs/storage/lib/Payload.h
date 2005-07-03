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

#if !defined(cencalvm_storage_payload_h)
#define cencalvm_storage_payload_h

namespace cencalvm {
  namespace storage {
    struct PayloadStruct;
  }; // namespace storage
}; // namespace cencalvm

struct cencalvm::storage::PayloadStruct {
  float Vp;
  float Vs;
  float Density;
  float Qp;
  float Qs;
  float DepthFreeSurf;
  short FaultBlock;
  short Zone;
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
