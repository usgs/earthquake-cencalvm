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

#include "GridIngester.h" // implementation of class methods

#include "cencalvm/storage/Payload.h" // USES PayloadStruct
#include "cencalvm/storage/Geometry.h" // USES Geometry

extern "C" {
#include "etree.h"
}

#include <fstream> // USES std::ifstream
#include <iostream> // USES std::cout
#include <sstream> // USES std::ostringstream
#include <stdexcept> // USES std::runtime_error
#include <assert.h> // USES assert()

// ----------------------------------------------------------------------
// Add gridded data to database.
void
cencalvm::create::GridIngester::addGrid(etree_t** pDB,
					const char* filename)
{ // addGrid
  assert(0 != pDB);
  
  std::ifstream fin(filename);
  if (!fin.is_open()) {
    std::ostringstream msg;
    msg << "Could not open grid file '" << filename
	<< "' for reading.";
    throw std::runtime_error(msg.str());
  } // if

  cencalvm::storage::Geometry vmgeom;

  std::cout << "Beginning processing of '" << filename << "'..." << std::endl;

  double resHoriz = 0.0;
  double resVert = 0.0;
  fin >> resHoriz >> resVert;
  if (0.0 == resHoriz ||
      0.0 == resVert) {
    std::ostringstream msg;
    msg << "Could not read horizontal and vertical resolution in '"
	<< filename << "'.";
    std::runtime_error(msg.str());
  } // if
  
  const double tolerance = 1.0e-6;
  const double vertExag = resHoriz / resVert;
  const double vertExagE = vmgeom.vertExag();
  if (fabs(1.0 - vertExag/vertExagE) > tolerance) {
    std::ostringstream msg;
    msg << "Vertical exaggeration of " << vertExag << " in '" << filename
	<< "' does not match velocity model vertical exaggeration of "
	<< vertExagE << ".";
    throw std::runtime_error(msg.str());
  } // if

  const etree_tick_t level = vmgeom.level(resHoriz);
  const double edgeLen = vmgeom.edgeLen(level);
  if ( fabs(1.0 - resHoriz/edgeLen) > tolerance) {
    std::ostringstream msg;
    msg << "Horizontal resolution of " << resHoriz << " in '" << filename
	<< "' does not fit resolution of nearest level in database of "
	<< edgeLen << ".";
    throw std::runtime_error(msg.str());
  } // if    

  int numAdded = 0;
  int numIgnored = 0;
  try {
    while (!fin.eof()) {
      double lon = 0.0;
      double lat = 0.0;
      double elev = 0.0;
      cencalvm::storage::PayloadStruct payload;
      fin
	>> lon
	>> lat
	>> elev
	>> payload.Vp
	>> payload.Vs
	>> payload.Density
	>> payload.Qp
	>> payload.Qs
	>> payload.DepthFreeSurf
	>> payload.FaultBlock
	>> payload.Zone;
      if (fin.eof())
	break;
      if (!fin.good())
	throw std::runtime_error("Couldn't parse line.");
      if (payload.FaultBlock > 0 && payload.Zone > 0) {
	etree_addr_t addr;
	addr.level = level;
	addr.type = ETREE_LEAF;
	vmgeom.lonLatElevToAddr(&addr, lon, lat, elev);
	if (0 != etree_insert(*pDB, addr, &payload))
	  throw std::runtime_error(etree_strerror(etree_errno(*pDB)));
	numAdded++;
      } else
	numIgnored++;
    } // while
  }
  catch (std::exception& err) {
    std::ostringstream msg;
    msg << "Caught exception while reading grid from '" << filename << "'.\n"
	<< "Successfully added " << numAdded << " points and ignore "
	<< numIgnored << " others.\n"
	<< err.what();
    throw std::runtime_error(msg.str());
  } // catch
  catch (...) {
    std::ostringstream msg;
    msg << "Caught exception while reading grid from '" << filename << "'.\n"
	<< "Successfully added " << numAdded << " points and ignore "
	<< numIgnored << " others.\n";
    throw std::runtime_error(msg.str());
  } // catch
   
  fin.close();

  std::cout << "Done procesing '" << filename << "'."
	    << "  # points added: " << numAdded
	    << ",  # points ignored: " << numIgnored
	    << std::endl;
} // addGrid

