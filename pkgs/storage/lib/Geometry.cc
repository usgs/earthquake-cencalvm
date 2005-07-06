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

#include "Geometry.h" // implementation of class methods

#include "Projector.h"

extern "C" {
#include "etree.h"
}

#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <iomanip> // USES setw(), setiosflags(), resetiosflags()
#include <assert.h> // USES assert()

// ----------------------------------------------------------------------
// @warning If you change these values, you MUST regenerate the Etree
// database!!!!
const double cencalvm::storage::Geometry::_ROOTLEN = 1638400.0;
const double cencalvm::storage::Geometry::_AZ = 53.638;
const double cencalvm::storage::Geometry::_VERTEXAG = 4.0;
const double cencalvm::storage::Geometry::_PROJXNW = -74937.817;
const double cencalvm::storage::Geometry::_PROJYNW = 380186.124;
const double cencalvm::storage::Geometry::_BUFFERNW = 409600.0;
const double cencalvm::storage::Geometry::_BUFFERSW = 204800.0;
const double cencalvm::storage::Geometry::_MAXELEV = 12800.0;

// ----------------------------------------------------------------------
cencalvm::storage::Geometry::Geometry(void) :
  _projXNWRoot(_PROJXNW 
	       - _BUFFERNW*cos(_AZ*M_PI/180.0)
	       - _BUFFERSW*sin(_AZ*M_PI/180.0)),
  _projYNWRoot(_PROJYNW
	       + _BUFFERNW*sin(_AZ*M_PI/180.0)
	       - _BUFFERSW*cos(_AZ*M_PI/180.0)),
  _pProj(new Projector)
{ // constructor
} // constructor

// ----------------------------------------------------------------------
cencalvm::storage::Geometry::~Geometry(void)
{ // destructor
  delete _pProj; _pProj = 0;
} // destructor

// ----------------------------------------------------------------------
// Map global coordinates xyz to etree address. 
void
cencalvm::storage::Geometry::lonLatElevToAddr(etree_addr_t* pAddr,
					      const double lon,
					      const double lat,
					      const double elev)
{ // lonLatElevToAddr
  assert(0 != _pProj);
  assert(0 != pAddr);
  assert(0 <= pAddr->level && 32 > pAddr->level);

  // Convert lon, lat, elev to projected coordinates
  double x = 0;
  double y = 0;
  _pProj->project(&x, &y, lon, lat);
  
  // convert projected coordinates to coordinates along root octant
  const double azR = _AZ * M_PI / 180.0;
  const double p = 
    (x - _projXNWRoot) * cos(azR) - (y - _projYNWRoot) * sin(azR);
  const double q = 
    (x - _projXNWRoot) * sin(azR) + (y - _projYNWRoot) * cos(azR);
  const double r = (elev-_MAXELEV)*_VERTEXAG + _ROOTLEN;

  if (p < 0.0 || p > _ROOTLEN ||
      q < 0.0 || q > _ROOTLEN ||
      r < 0.0 || r > _ROOTLEN) {
    std::ostringstream msg;
    msg
      << std::resetiosflags(std::ios::fixed)
      << std::setiosflags(std::ios::scientific)
      << std::setprecision(6)
      << "Location (" << lon << ", " << lat << ", " << elev << ")\n"
      << "not in domain. Coordinates relative to root octant are:\n"
      << "  p: " << p/_ROOTLEN << "\n"
      << "  q: " << q/_ROOTLEN << "\n"
      << "  r: " << r/_ROOTLEN << "\n";
    throw std::runtime_error(msg.str());
  } // if

  const double res = _ROOTLEN / ((etree_tick_t) 1 << pAddr->level);
  const etree_tick_t tickLen = 0x80000000 >> pAddr->level;

  pAddr->x = tickLen*etree_tick_t(p / res);
  pAddr->y = tickLen*etree_tick_t(q / res);
  pAddr->z = tickLen*etree_tick_t(r / res);
} // lonLatElevToAddr
  
// ----------------------------------------------------------------------
// Get address of parent octant if it exists.
bool
cencalvm::storage::Geometry::findParent(etree_addr_t* pParentAddr,
					const etree_addr_t& childAddr)
{ // findParent
  assert(0 != pParentAddr);
  
  if (childAddr.level == 0)
    return false;

  const etree_tick_t leftmost_one = ~(~((etree_tick_t)0) >> 1);
  const int parentLevel = childAddr.level - 1;
  etree_tick_t mask = ((signed) leftmost_one) >> parentLevel;
  
  pParentAddr->x     = childAddr.x & mask;
  pParentAddr->y     = childAddr.y & mask;
  pParentAddr->z     = childAddr.z & mask;
  pParentAddr->level = parentLevel;
  pParentAddr->t     = 0;
  pParentAddr->type  = ETREE_INTERIOR;

  return true;
} // findParent

// version
// $Id$

// End of file 
