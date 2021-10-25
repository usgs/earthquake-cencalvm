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

#include "Projector.h" // implementation of class methods

#include "ErrorHandler.h" // HOLDSA ErrorHandler

#include <stdexcept> // USES std::runtime_error
#include <sstream> // USES std::ostringstream
#include <iomanip> // USES setw(), setiosflags(), resetiosflags()
#include <assert.h> // USES assert()

// ----------------------------------------------------------------------
// @warning These values must be compatible with the 3-D geologic model!!!
const char* cencalvm::storage::Projector::_PROJECTION = "tmerc";
const double cencalvm::storage::Projector::_MERIDIAN = -123.0;
const double cencalvm::storage::Projector::_LAT = 35.0;
const double cencalvm::storage::Projector::_SCALE = 0.9996;
const char* cencalvm::storage::Projector::_DATUM = "NAD83";
const char* cencalvm::storage::Projector::_UNITS = "m";

// ----------------------------------------------------------------------
cencalvm::storage::Projector::Projector(void) :
  _pProj(0)
{ // constructor
  std::ostringstream args;
  args
    << "+proj=" << _PROJECTION
    << " +lon_0=" << _MERIDIAN
    << " +lat_0=" << _LAT
    << " +k=" << _SCALE
    << " +datum=" << _DATUM
    << " +units=" << _UNITS;
  
  proj_destroy(_pProj);
  _pProj = proj_create(NULL, args.str().c_str());
  if (!_pProj) {
    std::ostringstream msg;
    msg << "Error while initializing projection:\n"
	<< "  " << proj_errno_string(proj_errno(_pProj)) << "\n"
	<< "Projection parameters:\n"
	<< "  " << args.str();
    throw std::runtime_error(msg.str());
  } // if
} // constructor

// ----------------------------------------------------------------------
cencalvm::storage::Projector::~Projector(void)
{ // destructor
  proj_destroy(_pProj); _pProj = NULL;
} // destructor

// ----------------------------------------------------------------------
void
cencalvm::storage::Projector::project(double* pX,
				      double* pY,
				      const double lon,
				      const double lat) const
{ // project
  assert(0 != pX);
  assert(0 != pY);
  assert(0 != _pProj);

  PJ_COORD src = proj_coord(proj_torad(lon), proj_torad(lat), 0.0, 0.0);
  PJ_COORD dest = proj_trans(_pProj, PJ_FWD, src);
  if (proj_errno(_pProj)) {
    std::ostringstream msg;
    msg << "Error while projecting location.\n"
	<< "  " << proj_errno_string(proj_errno(_pProj)) << "\n"
	<< "  longitude: " << lon << "\n"
	<< "  latitude: " << lat << "\n";
    throw std::runtime_error(msg.str());
  } // if
  *pX = dest.xyzt.x;
  *pY = dest.xyzt.y;
} // project

// ----------------------------------------------------------------------
// Apply inverse projection.
void
cencalvm::storage::Projector::invProject(double* pLon,
					 double* pLat,
					 const double x,
					 const double y) const
{ // invProject
  assert(0 != pLon);
  assert(0 != pLat);
  assert(0 != _pProj);

  PJ_COORD src = proj_coord(x, y, 0.0, 0.0);
  PJ_COORD dest = proj_trans(_pProj, PJ_INV, src);
  if (proj_errno(_pProj)) {
    std::ostringstream msg;
    msg << "Error while inverse projecting location.\n"
	<< "  " << proj_errno_string(proj_errno(_pProj)) << "\n"
	<< "  x: " << x << "\n"
	<< "  y: " << y << "\n";
    throw std::runtime_error(msg.str());
  } // if
  *pLon = proj_todeg(dest.xyzt.x);
  *pLat = proj_todeg(dest.xyzt.y);
} // invProject

// End of file 
