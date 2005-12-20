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

#include "VsGrader.h" // implementation of class methods

#include "cencalvm/storage/Payload.h" // USES SCHEMA
#include "cencalvm/storage/Geometry.h" // USES Geometry
#include "cencalvm/storage/ErrorHandler.h" // USES ErrorHandler
#include "cencalvm/create/VMCreator.h" // USES VMCreator
#include "cencalvm/storage/Projector.h" // USES Projector

extern "C" {
#include "etree.h"
}

#include <fstream> // USES std::ifstream

#include <sstream> // USES std::ostringstream
#include <assert.h> // USES assert()
#include <iostream> // USES std::cout

// ----------------------------------------------------------------------
const double cencalvm::vsgrader::VsGrader::_AZIMUTHLEN = -36.3620 * M_PI/180.0;
const double cencalvm::vsgrader::VsGrader::_NODATAVAL = 1.0e+10;

// ----------------------------------------------------------------------
// Constructor
cencalvm::vsgrader::VsGrader::VsGrader(void) :
  _gradientMaxVs(0),
  _swcornerLon(0),
  _swcornerLat(0),
  _swcornerElev(0),
  _swcornerX(0),
  _swcornerY(0),
  _domainLen(0),
  _domainWidth(0),
  _domainHt(0),
  _resVert(1.0),
  _filenameIn(""),
  _filenameOut(""),
  _filenameTmp(""),
  _filenameParams(""),
  _pErrHandler(new cencalvm::storage::ErrorHandler),
  _pGeom(new cencalvm::storage::Geometry(*_pErrHandler)),
  _pProj(new cencalvm::storage::Projector(*_pErrHandler)),
  _quiet(false)
{ // constructor
} // constructor

// ----------------------------------------------------------------------
// Destructor
cencalvm::vsgrader::VsGrader::~VsGrader(void)
{ // destructor
  delete _pGeom; _pGeom = 0;
  delete _pProj; _pProj = 0;
  delete _pErrHandler; _pErrHandler = 0;
} // destructor

// ----------------------------------------------------------------------
// Create Etree database with maximum gradient.
void
cencalvm::vsgrader::VsGrader::run(void)
{ // run
  _readParams();
  if (cencalvm::storage::ErrorHandler::OK != _pErrHandler->status())
    return;

  _initialize();
  if (cencalvm::storage::ErrorHandler::OK != _pErrHandler->status())
    return;

  _extract();
  if (cencalvm::storage::ErrorHandler::OK != _pErrHandler->status())
    return;

  _pack();
  if (cencalvm::storage::ErrorHandler::OK != _pErrHandler->status())
    return;

  _grade();
  if (cencalvm::storage::ErrorHandler::OK != _pErrHandler->status())
    return;
} // run

// ----------------------------------------------------------------------
// Read parameters from file.
void
cencalvm::vsgrader::VsGrader::_readParams(void)
{ // _readParams
  assert(0 != _pErrHandler);

  if (!_quiet)
    std::cout << "Reading parameters from '" << _filenameParams << "'..." 
	      << std::endl;

  std::ifstream filein(_filenameParams.c_str());
  if (!filein.is_open() || !filein.good()) {
    std::ostringstream msg;
    msg << "Could not open file '" << _filenameParams
	<< "' to read VsGrader parameters.";
    _pErrHandler->error(msg.str().c_str());
    return;
  } // if

  std::string name;
  filein >> name;

  if (0 == strcasecmp(name.c_str(), "VsGrader")) {
    std::string token;
    const int maxIgnore = 128;
    filein.ignore(maxIgnore, '{');

    filein >> token;
    while(filein.good() && token != "}") {
      if (0 == strcasecmp(token.c_str(), "swcorner-lon")) {
	filein.ignore(maxIgnore, '=');
	filein >> _swcornerLon;
      } else if (0 == strcasecmp(token.c_str(), "swcorner-lat")) {
	filein.ignore(maxIgnore, '=');
	filein >> _swcornerLat;
      } else if (0 == strcasecmp(token.c_str(), "swcorner-elev")) {
	filein.ignore(maxIgnore, '=');
	filein >> _swcornerElev;
      } else if (0 == strcasecmp(token.c_str(), "domain-length")) {
	filein.ignore(maxIgnore, '=');
	filein >> _domainLen;
      } else if (0 == strcasecmp(token.c_str(), "domain-width")) {
	filein.ignore(maxIgnore, '=');
	filein >> _domainWidth;
      } else if (0 == strcasecmp(token.c_str(), "domain-height")) {
	filein.ignore(maxIgnore, '=');
	filein >> _domainHt;
      } else if (0 == strcasecmp(token.c_str(), "resolution-vert")) {
	filein.ignore(maxIgnore, '=');
	filein >> _resVert;
      } else if (0 == strcasecmp(token.c_str(), "vs-gradient-max")) {
	filein.ignore(maxIgnore, '=');
	filein >> _gradientMaxVs;
      } else {
	std::ostringstream msg;
	msg << "Could not parse '" << token << "' into a parameter setting.";
	_pErrHandler->error(msg.str().c_str());
	return;
      } // else
      
      filein >> token;
    } // while
    if (!filein.good()) {
	_pErrHandler->error("I/O error while parsing settings.");
	return;
    } // if
  } else {
    std::ostringstream msg;
    msg << "Could not parse '" << name << "' into 'VsGrader'.";
    _pErrHandler->error(msg.str().c_str());
    return;
  } // else

  filein.close();
} // _readParams

// ----------------------------------------------------------------------
// Initialize.
void
cencalvm::vsgrader::VsGrader::_initialize(void)
{ // _initialize
  assert(0 != _pProj);

  if (!_quiet)
    std::cout << "Initializing grader..." << std::endl;

  _pProj->project(&_swcornerX, &_swcornerY, _swcornerLon, _swcornerLat);
} // _initialize

// ----------------------------------------------------------------------
// Extract database.
void
cencalvm::vsgrader::VsGrader::_extract(void) const
{ // _extract
  assert(0 != _pGeom);
  assert(0 != _pErrHandler);

  if (!_quiet)
    std::cout << "Extracting data from '" << _filenameIn << "'..."
	      << std::endl;

  const int cacheSize = 512;
  const int numDims = 3;
  const int payloadSize = sizeof(cencalvm::storage::PayloadStruct);
  etree_t* dbOrig = etree_open(_filenameIn.c_str(), O_RDONLY,
			       cacheSize, payloadSize, numDims);
  if (0 == dbOrig) {
    _pErrHandler->error("Could not open source etree database.");
    return;
  } // if
  
  etree_t* dbNew = etree_open(_filenameTmp.c_str(),
			      O_CREAT | O_TRUNC | O_RDWR,
			      cacheSize, payloadSize, numDims);
  if (0 == dbNew) {
    _pErrHandler->error("Could not open new etree database.");
    return;
  } // if
  
  if (0 != etree_registerschema(dbNew, cencalvm::storage::Payload::SCHEMA)) {
    _pErrHandler->error(etree_strerror(etree_errno(dbNew)));
    return;
  } // if
  
  char* appmeta = 0;
  if (0 != (appmeta = etree_getappmeta(dbOrig)))
    if (0 != etree_setappmeta(dbNew, appmeta)) {
      _pErrHandler->error(etree_strerror(etree_errno(dbNew)));
      return;
    } // if
  free(appmeta);

  const double resHoriz = _resVert * _pGeom->vertExag();
  const int numLen = int(1 + _domainLen / resHoriz);
  const int numWidth = int(1 + _domainWidth / resHoriz);
  const int numHt = int(1 + _domainHt / _resVert);
  etree_addr_t addr;
  addr.type = ETREE_LEAF;
  addr.level = cencalvm::storage::Geometry::level(resHoriz);
  cencalvm::storage::PayloadStruct payload;
  for (int iLen=0; iLen < numLen; ++iLen) {
    for (int iWidth=0; iWidth < numWidth; ++iWidth) {
      double lon = 0;
      double lat = 0;
      _indexToLonLat(&lon, &lat, iLen, iWidth);
      for (int iHt=0; iHt < numHt; ++iHt) {
	const double elev = _indexToElev(iHt);
	_pGeom->lonLatElevToAddr(&addr, lon, lat, elev);

	etree_addr_t resAddr;
	int err = etree_search(dbOrig, addr, &resAddr, "*", &payload);
	if (0 != err ||
	    (ETREE_INTERIOR == resAddr.type && addr.level > resAddr.level)) {
	  payload.Vp = _NODATAVAL;
	  payload.Vs = _NODATAVAL;
	  payload.Density = _NODATAVAL;
	  payload.Qp = _NODATAVAL;
	  payload.Qs = _NODATAVAL;
	  payload.DepthFreeSurf = cencalvm::storage::Payload::NODATAVAL;
	  payload.FaultBlock = cencalvm::storage::Payload::NODATABLOCK;
	  payload.Zone = cencalvm::storage::Payload::NODATAZONE;
	} // if

	// Append data to extracted database
	if (0 != etree_insert(dbNew, addr, &payload)) {
	  _pErrHandler->error(etree_strerror(etree_errno(dbNew)));
	  return;
	} // if
      } // for
    } // for
  } // for
  
  if (0 != etree_close(dbOrig)) {
    _pErrHandler->error(etree_strerror(etree_errno(dbOrig)));
    return;
  } // if
  
  if (0 != etree_close(dbNew)) {
    _pErrHandler->error(etree_strerror(etree_errno(dbNew)));
    return; 
  } // if

  if (!_quiet)
    std::cout << "Done extracting." << std::endl;
} // _extract

// ----------------------------------------------------------------------
// Pack database.
void
cencalvm::vsgrader::VsGrader::_pack(void) const
{ // _pack
  if (!_quiet)
    std::cout << "Creating packed database..." << std::endl;

  cencalvm::create::VMCreator creator;
  creator.filenameTmp(_filenameTmp.c_str());
  creator.filenameOut(_filenameOut.c_str());
  creator.quiet(_quiet);
  creator.packDB();

  if (!_quiet)
    std::cout << "Done packing database..." << std::endl;
} // _pack

// ----------------------------------------------------------------------
// Limit gradient in database.
void
cencalvm::vsgrader::VsGrader::_grade(void) const
{ // _grade
  assert(0 != _pGeom);
  assert(0 != _pErrHandler);

  if (!_quiet)
    std::cout << "Starting grading of database." << std::endl;

  // Open database for reading/writing
  const int cacheSize = 512;
  const int numDims = 3;
  const int payloadSize = sizeof(cencalvm::storage::PayloadStruct);
  etree_t* db = etree_open(_filenameOut.c_str(), O_RDWR,
			       cacheSize, payloadSize, numDims);
  if (0 == db) {
    _pErrHandler->error("Could not open etree database.");
    return;
  } // if

  const double resHoriz = _resVert * _pGeom->vertExag();
  const int numLen = int(1 + _domainLen / resHoriz);
  const int numWidth = int(1 + _domainWidth / resHoriz);
  const int numHt = int(1 + _domainHt / _resVert);
  
  int dataLen = 0;
  cencalvm::storage::PayloadStruct* pData = 0;
  bool* pIsChanged = 0;

  // Limit gradient in vertical direction
  if (!_quiet)
    std::cout << "Grading database in vertical direction." << std::endl;
  dataLen = (numHt > 0) ? numHt : 0;
  pData = (dataLen > 0) ? new cencalvm::storage::PayloadStruct[dataLen] : 0;
  pIsChanged = (dataLen > 0) ? new bool[dataLen] : 0;
  for (int iLen=0; iLen < numLen; ++iLen)
    for (int iWidth=0; iWidth < numWidth; ++iWidth) {
      double lon = 0;
      double lat = 0;
      _indexToLonLat(&lon, &lat, iLen, iWidth);
      for (int iHt=0; iHt < numHt; ++iHt) {
	const double elev = _indexToElev(iHt);
	_pullData(&pData[iHt], lon, lat, elev, db);
      } // for
      const double maxdiff = _gradientMaxVs * _resVert;
      _limitDiff(&pData, &pIsChanged, dataLen, maxdiff);
      
      for (int iHt=0; iHt < numHt; ++iHt)
	if (pIsChanged[iHt]) {
	  const double elev = _indexToElev(iHt);
	  _pushData(db, pData[iHt], lon, lat, elev);
	} // for
    } // for
  delete[] pData; pData = 0;
  delete[] pIsChanged; pIsChanged = 0;
  dataLen = 0;

  // Limit gradient in length direction
  if (!_quiet)
    std::cout << "Grading database in length direction." << std::endl;
  dataLen = (numLen > 0) ? numLen : 0;
  pData = (dataLen > 0) ? new cencalvm::storage::PayloadStruct[dataLen] : 0;
  pIsChanged = (dataLen > 0) ? new bool[dataLen] : 0;
  for (int iHt=0; iHt < numHt; ++iHt) {
    const double elev = _indexToElev(iHt);
    double lon = 0;
    double lat = 0;
    for (int iWidth=0; iWidth < numWidth; ++iWidth) {
      for (int iLen=0; iLen < numLen; ++iLen) {
	_indexToLonLat(&lon, &lat, iLen, iWidth);
	_pullData(&pData[iLen], lon, lat, elev, db);
      } // for
      const double maxdiff = _gradientMaxVs * resHoriz;
      _limitDiff(&pData, &pIsChanged, dataLen, maxdiff);
      
      for (int iLen=0; iLen < numLen; ++iLen)
	if (pIsChanged[iLen]) {
	  _indexToLonLat(&lon, &lat, iLen, iWidth);
	  _pushData(db, pData[iLen], lon, lat, elev);
	} // for
    } // for
  } // for
  delete[] pData; pData = 0;
  delete[] pIsChanged; pIsChanged = 0;
  dataLen = 0;
      
  // Limit gradient in width direction
  if (!_quiet)
    std::cout << "Grading database in width direction." << std::endl;
  dataLen = (numWidth > 0) ? numWidth : 0;
  pData = (dataLen > 0) ? new cencalvm::storage::PayloadStruct[dataLen] : 0;
  pIsChanged = (dataLen > 0) ? new bool[dataLen] : 0;
  for (int iHt=0; iHt < numHt; ++iHt) {
    const double elev = _indexToElev(iHt);
    double lon = 0;
    double lat = 0;
    for (int iLen=0; iLen < numLen; ++iLen) {
      for (int iWidth=0; iWidth < numWidth; ++iWidth) {
	_indexToLonLat(&lon, &lat, iLen, iWidth);
	_pullData(&pData[iWidth], lon, lat, elev, db);
      } // for
      const double maxdiff = _gradientMaxVs * resHoriz;
      _limitDiff(&pData, &pIsChanged, dataLen, maxdiff);
      
      for (int iWidth=0; iWidth < numWidth; ++iWidth)
	if (pIsChanged[iWidth]) {
	  _indexToLonLat(&lon, &lat, iLen, iWidth);
	  _pushData(db, pData[iWidth], lon, lat, elev);
	} // for
    } // for
  } // for
  delete[] pData; pData = 0;
  delete[] pIsChanged; pIsChanged = 0;
  dataLen = 0;

  // close database
  if (0 != etree_close(db)) {
    _pErrHandler->error(etree_strerror(etree_errno(db)));
    return; 
  } // if

  if (!_quiet)
    std::cout << "Done grading database." << std::endl;
} // _grade

// ----------------------------------------------------------------------
// Compute longitude and latitude given length and width
void
cencalvm::vsgrader::VsGrader::_indexToLonLat(double* pLon,
					     double* pLat,
					     const int indexL,
					     const int indexW) const
{ // _indexToLonLat
  assert(0 != _pGeom);
  assert(0 != _pProj);

  const double azR = _AZIMUTHLEN;
  const double resHoriz = _resVert * _pGeom->vertExag();
  const double l = indexL * resHoriz;
  const double w = indexW * resHoriz;
  const double x = _swcornerX + l * sin(azR) + w * cos(azR);
  const double y = _swcornerY + l * cos(azR) - w * sin(azR);
  _pProj->invProject(pLon, pLat, x, y);
} // _indexToLonLat

// ----------------------------------------------------------------------
// Get data from database. Payload is returned through the parameters.
void
cencalvm::vsgrader::VsGrader::_pullData(cencalvm::storage::PayloadStruct* pPayload,
					const double lon,
					const double lat,
					const double elev,
					etree_t* pDB) const
{ // _pullData
  assert(0 != pPayload);
  assert(0 != pDB);
  assert(0 != _pGeom);
  assert(0 != _pErrHandler);

  etree_addr_t addr;
  addr.type = ETREE_LEAF;

  const double resHoriz = _resVert * _pGeom->vertExag();
  addr.level = _pGeom->level(resHoriz);

  _pGeom->lonLatElevToAddr(&addr, lon, lat, elev);
  
  etree_addr_t resAddr;
  int err = etree_search(pDB, addr, &resAddr, "*", pPayload);
  if (0 != err) {
    _pErrHandler->error("Could not find octant in database.\n");
    return;
  } // if
} // _pullData

// ----------------------------------------------------------------------
// Update data in database.
void
cencalvm::vsgrader::VsGrader::_pushData(etree_t* pDB,
			      const cencalvm::storage::PayloadStruct& payload,
					const double lon,
					const double lat,
					const double elev) const
{ // _pushData
  assert(0 != pDB);
  assert(0 != _pGeom);
  assert(0 != _pErrHandler);

  etree_addr_t addr;
  addr.type = ETREE_LEAF;

  const double resHoriz = _resVert * _pGeom->vertExag();
  addr.level = _pGeom->level(resHoriz);

  _pGeom->lonLatElevToAddr(&addr, lon, lat, elev);
  int err = etree_update(pDB, addr, &payload);
  if (0 != err) {
    _pErrHandler->error("Could not update octant in database.\n");
    return;
  } // if
} // _pushData

// ----------------------------------------------------------------------
// Limit maximum difference in Vs. Other material properties are
void
cencalvm::vsgrader::VsGrader::_limitDiff(cencalvm::storage::PayloadStruct** ppData,
					 bool** ppIsChanged,
					 const int dataLen,
					 const double maxdiff) const
{ // _limitDiff
  assert(0 != ppData);
  assert(0 != ppIsChanged);

  for (int i=0; i < dataLen; ++i)
    (*ppIsChanged)[i] = false;

  for (int i=1; i < dataLen; ++i)
    if ((*ppData)[i].Vs - (*ppData)[i-1].Vs > maxdiff) {
      const double vs = (*ppData)[i-1].Vs + maxdiff;
      cencalvm::storage::PayloadStruct* pPayload = (*ppData)+i;
      const double scale = vs / pPayload->Vs;
      pPayload->Vp *= scale;
      pPayload->Vs = vs;
      pPayload->Density *= scale;
      pPayload->Qp *= scale;
      pPayload->Qs *= scale;
      (*ppIsChanged)[i] = true;
    } // if
  for (int i=dataLen-2; i >= 0; --i)
    if ((*ppData)[i].Vs - (*ppData)[i+1].Vs > maxdiff) {
      const double vs = (*ppData)[i+1].Vs + maxdiff;
      cencalvm::storage::PayloadStruct* pPayload = (*ppData)+i;
      const double scale = vs / pPayload->Vs;
      pPayload->Vp *= scale;
      pPayload->Vs = vs;
      pPayload->Density *= scale;
      pPayload->Qp *= scale;
      pPayload->Qs *= scale;
      (*ppIsChanged)[i] = true;
    } // if
} // _limitDiff

// version
// $Id$

// End of file 
