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

#include "VMQuery.h" // implementation of class methods

#include "cencalvm/storage/Payload.h" // USES PayloadStruct

extern "C" {
#include "etree.h"
}

#include <sstream> // USES std::ostringstream
#include <stdexcept> // USES std::runtime_error
#include <strings.h> // USES strcasecmp()

// ----------------------------------------------------------------------
/// Default constructor
cencalvm::query::VMQuery::VMQuery(void) :
  _queryRes(0),
  _db(0),
  _filename(""),
  _pQueryVals(0),
  _querySize(0),
  _cacheSize(128),
  _queryFn(&cencalvm::query::VMQuery::_queryMax)
{ // constructor
  const int querySize = 8;
  _pQueryVals = (querySize > 0) ? new int[querySize] : 0;
  for (int i=0; i < querySize; ++i)
    _pQueryVals[i] = i;
  _querySize = querySize;
} // constructor
  
// ----------------------------------------------------------------------
// Default destructor.
cencalvm::query::VMQuery::~VMQuery(void)
{ // destructor
  delete[] _pQueryVals; _pQueryVals = 0;
} // destructor
  
// ----------------------------------------------------------------------
// Open the database and prepare for querying.
void
cencalvm::query::VMQuery::open(void)
{ // open
  if (0 != _db) // database is already open
    return;
  
  assert(_cacheSize > 0);
  _db = etree_open(_filename, O_RDONLY, _cacheSize, 0, 0);
  if (0 == _db) {
    std::ostringstream msg;
    msg << "Could not open the etree database '" << _filename
	<< "' for querying.";
    throw std::runtime_error(msg.str());
  } // if
} // open
  
// ----------------------------------------------------------------------
// Close the database.
void
cencalvm::query::VMQuery::close(void)
{ // close
  if (0 != _db && 0 != etree_close(_db)) {
    std::ostringstream msg;
    msg << "Could not close the etree database '" << _filename << "'.";
    throw std::runtime_error(msg.str());
  } // if
  _db = 0;
} // close
  
// ----------------------------------------------------------------------
// Set query type.
void
cencalvm::query::VMQuery::queryType(const QueryEnum queryType)
{ // queryType
  switch (queryType)
    { // switch
    case MAXRES :
      _queryFn = _queryMax;
      break;
    case FIXEDRES :
      _queryFn = _queryFixed;
      break;
    case AVGRES :
      _queryFn = _queryAvg;
      break;
    default :
      throw std::runtime_error("Could not find query function for "
			       "requested query type.");
    } // switch
} // queryType

// ----------------------------------------------------------------------
// Set values to be returned by queries.
void
cencalvm::query::VMQuery::queryVals(const char** names,
				    const int numVals)
{ // queryVals
  assert(0 != names);

  if (numVals > 0) {
    delete[] _pQueryVals; 
    _pQueryVals = (numVals > 0) ? new int[numVals] : 0;
    for (int iVal=0; iVal < numVals; ++iVal) {
      if (0 == strcasecmp("Vp", names[iVal]))
	_pQueryVals[iVal] = 0;
      else if (0 == strcasecmp("Vs", names[iVal]))
	_pQueryVals[iVal] = 1;
      else if (0 == strcasecmp("Density", names[iVal]))
	_pQueryVals[iVal] = 2;
      else if (0 == strcasecmp("Qp", names[iVal]))
	_pQueryVals[iVal] = 3;
      else if (0 == strcasecmp("Qs", names[iVal]))
	_pQueryVals[iVal] = 4;
      else if (0 == strcasecmp("DepthFreeSurf", names[iVal]))
	_pQueryVals[iVal] = 5;
      else if (0 == strcasecmp("FaultBlock", names[iVal]))
	_pQueryVals[iVal] = 6;
      else if (0 == strcasecmp("Zone", names[iVal]))
	_pQueryVals[iVal] = 7;
      else {
	std::ostringstream msg;
	msg << "Value name '" << names[iVal] << "' does not match any "
	    << "of the values in the velocity database.\n"
	    << "Values in database are:\n"
	    << "  Vp, Vs, Density, Qp, Qs, DepthFreeSurf, FaultBlock, Zone.";
	throw std::runtime_error(msg.str());
      } // else
    } // for
  } // if
} // queryVals

// ----------------------------------------------------------------------
// Query the database.
void
cencalvm::query::VMQuery::query(double** pVals,
				const int numVals,
				const double lon,
				const double lat,
				const double elev)
{ // query
  assert(0 != _queryFn);
  _queryFn(pVals, numVals, lon, lat, elev);
} // query

// ----------------------------------------------------------------------
// Query database at maximum resolution possible.
void
cencalvm::query::VMQuery::_queryMax(double** pVals,
				    const int numVals,
				    const double lon,
				    const double lat,
				    const double elev)
{ // _queryMax
} // _queryMax

// ----------------------------------------------------------------------
// Query database at fixed resolution. Resolution is specified by queryRes().
void
cencalvm::query::VMQuery::_queryFixed(double** pVals,
				      const int numVals,
				      const double lon,
				      const double lat,
				      const double elev)
{ // _queryFixed
} // _queryFixed

// ----------------------------------------------------------------------
// Query database at resolution specified by wavelength. Resolution
// is specified by queryRes().
void
cencalvm::query::VMQuery::_queryAvg(double** pVals,
				    const int numVals,
				    const double lon,
				    const double lat,
				    const double elev)
{ // _queryAvg
} // _queryAvg


// version
// $Id$

// End of file 
