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
#include "cencalvm/storage/Geometry.h" // USES Geometry
#include "cencalvm/storage/ErrorHandler.h" // USES ErrorHandler

extern "C" {
#include "etree.h"
}

#include <sstream> // USES std::ostringstream
#include <iomanip> // USES setw(), setiosflags(), resetiosflags()
#include <strings.h> // USES strcasecmp()

// ----------------------------------------------------------------------
const double cencalvm::query::VMQuery::_NODATAVAL = -999.0;

// ----------------------------------------------------------------------
/// Default constructor
cencalvm::query::VMQuery::VMQuery(void) :
  _queryRes(0),
  _db(0),
  _filename(""),
  _pQueryVals(0),
  _querySize(0),
  _cacheSize(128),
  _queryFn(&cencalvm::query::VMQuery::_queryMax),
  _pErrHandler(new cencalvm::storage::ErrorHandler),
  _pGeom(new cencalvm::storage::Geometry(*_pErrHandler))
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
  delete _pGeom; _pGeom = 0;
  delete _pErrHandler; _pErrHandler = 0;
  if (0 != _db)
    close();
} // destructor

// ----------------------------------------------------------------------
// Open the database and prepare for querying.
void
cencalvm::query::VMQuery::open(void)
{ // open
  if (0 != _db) // database is already open
    return;
  
  assert(_cacheSize > 0);
  _db = etree_open(_filename.c_str(), O_RDONLY, _cacheSize, 0, 0);
  if (0 == _db) {
    std::ostringstream msg;
    msg << "Could not open the etree database '" << _filename
	<< "' for querying.";
    _pErrHandler->error(msg.str().c_str());
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
    _pErrHandler->error(msg.str().c_str());
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
      _queryFn = &cencalvm::query::VMQuery::_queryMax;
      break;
    case FIXEDRES :
      _queryFn = &cencalvm::query::VMQuery::_queryFixed;
      break;
    case AVGRES :
      _queryFn = &cencalvm::query::VMQuery::_queryAvg;
      break;
    default :
      _pErrHandler->error("Could not find query function for requested "
			 "query type.");
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
    _querySize = numVals;
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
	_pErrHandler->error(msg.str().c_str());
      } // else
    } // for
  } // if
} // queryVals

// ----------------------------------------------------------------------
// Query the database.
void
cencalvm::query::VMQuery::query(double** ppVals,
				const int numVals,
				const double lon,
				const double lat,
				const double elev)
{ // query
  assert(0 != _queryFn);
  assert(0 != ppVals);
  assert(numVals == _querySize);

  cencalvm::storage::PayloadStruct payload;
  (this->*_queryFn)(&payload, lon, lat, elev);

  for (int i=0; i < _querySize; ++i) {
    switch (_pQueryVals[i])
      { // switch
      case 0 :
	(*ppVals)[i] = payload.Vp;
	break;
      case 1 :
	(*ppVals)[i] = payload.Vs;
	break;
      case 2 :
	(*ppVals)[i] = payload.Density;
	break;
      case 3 :
	(*ppVals)[i] = payload.Qp;
	break;
      case 4 :
	(*ppVals)[i] = payload.Qs;
	break;
      case 5 :
	(*ppVals)[i] = payload.DepthFreeSurf;
	break;
      case 6 :
	(*ppVals)[i] = payload.FaultBlock;
	break;
      case 7 :
	(*ppVals)[i] = payload.Zone;
	break;
      default :
	_pErrHandler->error("Could not parse requested query value.");
      } // switch
  } // for
} // query

// ----------------------------------------------------------------------
// Query database at maximum resolution possible.
void
cencalvm::query::VMQuery::_queryMax(cencalvm::storage::PayloadStruct* pPayload,
				    const double lon,
				    const double lat,
				    const double elev)
{ // _queryMax
  assert(0 != pPayload);

  etree_addr_t addr;
  addr.level = ETREE_MAXLEVEL;
  addr.type = ETREE_LEAF;
  _pGeom->lonLatElevToAddr(&addr, lon, lat, elev);
  if (_pErrHandler->status() != cencalvm::storage::ErrorHandler::OK)
    return;

  etree_addr_t resAddr;
  if (0 != etree_search(_db, addr, &resAddr, "*", pPayload)) {
    std::ostringstream msg;
    msg
      << std::resetiosflags(std::ios::fixed)
      << std::setiosflags(std::ios::scientific)
      << std::setprecision(6)
      << lon << ", " << lat << ", " << elev << ", No data\n";
    _pErrHandler->log(msg.str().c_str());
    std::ostringstream warning;
    warning
      << "WARNING: No data for "
      << std::resetiosflags(std::ios::fixed)
      << std::setiosflags(std::ios::scientific)
      << std::setprecision(6)
      << lon << ", " << lat << ", " << elev << ".\n";
    _pErrHandler->warning(warning.str().c_str());
    pPayload->Vp = _NODATAVAL;
    pPayload->Vs = _NODATAVAL;
    pPayload->Density = _NODATAVAL;
    pPayload->Qp = _NODATAVAL;
    pPayload->Qs = _NODATAVAL;
    pPayload->DepthFreeSurf = _NODATAVAL;
    pPayload->FaultBlock = 0;
    pPayload->Zone = 0;
    return;
  } // if
} // _queryMax

// ----------------------------------------------------------------------
// Query database at fixed resolution. Resolution is specified by queryRes().
void
cencalvm::query::VMQuery::_queryFixed(cencalvm::storage::PayloadStruct* pPayload,
				      const double lon,
				      const double lat,
				      const double elev)
{ // _queryFixed
  assert(0 != pPayload);

  etree_addr_t addr;
  const double vertExag = cencalvm::storage::Geometry::vertExag();
  addr.level = _pGeom->level(vertExag * _queryRes);
  addr.type = ETREE_LEAF;
  _pGeom->lonLatElevToAddr(&addr, lon, lat, elev);
  if (_pErrHandler->status() != cencalvm::storage::ErrorHandler::OK)
    return;

  etree_addr_t resAddr;
  if (0 != etree_search(_db, addr, &resAddr, "*", pPayload)) {
    std::ostringstream msg;
    msg
      << std::resetiosflags(std::ios::fixed)
      << std::setiosflags(std::ios::scientific)
      << std::setprecision(6)
      << lon << ", " << lat << ", " << elev << ", No data\n";
    _pErrHandler->log(msg.str().c_str());
    std::ostringstream warning;
    warning
      << "WARNING: No data for "
      << std::resetiosflags(std::ios::fixed)
      << std::setiosflags(std::ios::scientific)
      << std::setprecision(6)
      << lon << ", " << lat << ", " << elev << ".\n";
    _pErrHandler->warning(warning.str().c_str());
    pPayload->Vp = _NODATAVAL;
    pPayload->Vs = _NODATAVAL;
    pPayload->Density = _NODATAVAL;
    pPayload->Qp = _NODATAVAL;
    pPayload->Qs = _NODATAVAL;
    pPayload->DepthFreeSurf = _NODATAVAL;
    pPayload->FaultBlock = 0;
    pPayload->Zone = 0;
    return;
  } // if
} // _queryFixed

// ----------------------------------------------------------------------
// Query database at resolution specified by wavelength. Resolution
// is specified by queryRes().
void
cencalvm::query::VMQuery::_queryAvg(cencalvm::storage::PayloadStruct* pPayload,
				    const double lon,
				    const double lat,
				    const double elev)
{ // _queryAvg
  etree_addr_t addr;

  addr.level = ETREE_MAXLEVEL;
  addr.type = ETREE_LEAF;
  _pGeom->lonLatElevToAddr(&addr, lon, lat, elev);
  if (_pErrHandler->status() != cencalvm::storage::ErrorHandler::OK)
    return;

  etree_addr_t resAddr;
  if (0 != etree_search(_db, addr, &resAddr, "*", pPayload)) {
    std::ostringstream msg;
    msg
      << std::resetiosflags(std::ios::fixed)
      << std::setiosflags(std::ios::scientific)
      << std::setprecision(6)
      << lon << ", " << lat << ", " << elev << ", No data\n";
    _pErrHandler->log(msg.str().c_str());
    std::ostringstream warning;
    warning
      << "WARNING: No data for "
      << std::resetiosflags(std::ios::fixed)
      << std::setiosflags(std::ios::scientific)
      << std::setprecision(6)
      << lon << ", " << lat << ", " << elev << ".\n";
    _pErrHandler->warning(warning.str().c_str());
    pPayload->Vp = _NODATAVAL;
    pPayload->Vs = _NODATAVAL;
    pPayload->Density = _NODATAVAL;
    pPayload->Qp = _NODATAVAL;
    pPayload->Qs = _NODATAVAL;
    pPayload->DepthFreeSurf = _NODATAVAL;
    pPayload->FaultBlock = 0;
    pPayload->Zone = 0;
    return;
  } // if
  
  const double vertExag = cencalvm::storage::Geometry::vertExag();
  const double minPeriod = vertExag * _queryRes;
  cencalvm::storage::PayloadStruct childPayload = *pPayload;
  while (pPayload->Vs > 0.0 && 
	 _pGeom->edgeLen(resAddr.level) / pPayload->Vs < minPeriod) {
    childPayload = *pPayload;
    etree_addr_t parentAddr;
    _pGeom->findAncestor(&parentAddr, resAddr, resAddr.level-1);
    if (0 != etree_search(_db, parentAddr, &resAddr, "*", pPayload)) {
      std::ostringstream msg;
      msg
	<< std::resetiosflags(std::ios::fixed)
	<< std::setiosflags(std::ios::scientific)
	<< std::setprecision(6)
	<< lon << ", " << lat << ", " << elev << ", No parent\n";
      _pErrHandler->log(msg.str().c_str());
      char buf[ETREE_MAXBUF];
      msg
	<< "Could not find parent octant " << 
	etree_straddr(_db, buf, parentAddr)
	<< "of child octant " << etree_straddr(_db, buf, resAddr) << "\n"
	<< "for location " << lon << ", " << lat << ", " << elev
	<< ". Using values from child octant.";
      _pErrHandler->warning(msg.str().c_str());
      etree_search(_db, addr, &resAddr, "*", pPayload);
      return;
    } // if
  } // while
  
  // Values returned are averages between values in payload that first
  // meets the wavelength criteria and the child that didn't. This
  // helps reduce the bias towards faster wave speeds. Fault block
  // values are taken from parent which is already in pPayload.
  pPayload->Vp = 0.5 * (childPayload.Vp + pPayload->Vp);
  pPayload->Vs = 0.5 * (childPayload.Vs + pPayload->Vs);
  pPayload->Density = 0.5 * (childPayload.Density + pPayload->Density);
  pPayload->Qp = 0.5 * (childPayload.Qp + pPayload->Qp);
  pPayload->Qs = 0.5 * (childPayload.Qs + pPayload->Qs);
  pPayload->DepthFreeSurf = 
    0.5 * (childPayload.DepthFreeSurf + pPayload->DepthFreeSurf);
} // _queryAvg

// version
// $Id$

// End of file 
