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

#include <portinfo>

#include <sys/types.h> // USES size_t

#include "f77vmquery.h"

extern "C" {
#include "cvmquery.h"
}

#include <sstream> // USES std::istringstream
#include <string.h> // USE strncpy

#include <iostream>

// ----------------------------------------------------------------------
// Create velocity model query object.
void
cencalvm_createquery_f(size_t* handle)
{ // createquery
  *handle = (size_t) cencalvm_createQuery();
} // createquery

// ----------------------------------------------------------------------
// Destroy velocity model query object.
void
cencalvm_destroyquery_f(size_t* handleAddr,
			int* ok)
{ // destroyQuery
  *ok = cencalvm_destroyQuery((void*) *handleAddr);
} // destroyQuery

// ----------------------------------------------------------------------
// Open database for querying.
void
cencalvm_open_f(size_t* handleAddr,
		int* ok)
{ // open
  *ok = cencalvm_open((void*) *handleAddr);
} // open
  
// ----------------------------------------------------------------------
// Close the database.
void
cencalvm_close_f(size_t* handleAddr,
		 int* ok)
{ // close
  *ok = cencalvm_close((void*) *handleAddr);
} // close
  
// ----------------------------------------------------------------------
// Set query type.
void
cencalvm_querytype_f(size_t* handleAddr,
		     const int* queryType,
		     int* ok)
{ // queryType
  *ok = cencalvm_queryType((void*) *handleAddr, *queryType);
} // queryType

// ----------------------------------------------------------------------
// Set query resolution.
void
cencalvm_queryres_f(size_t* handleAddr,
		    const double* res,
		    int* ok)
{ // queryRes
  *ok = cencalvm_queryRes((void*) *handleAddr, *res);
} // queryRes

// ----------------------------------------------------------------------
// Set the database filename.
void
cencalvm_filename_f(size_t* handleAddr,
		    const char* filename,
		    int* ok,
		    const int len)
{ // filename
  std::istringstream sin(filename);
  std::string cfilename;
  sin >> cfilename;
  *ok = cencalvm_filename((void*) *handleAddr, cfilename.c_str());
} // filename

// ----------------------------------------------------------------------
// Set size of cache during queries.
void
cencalvm_cachesize_f(size_t* handleAddr,
		     const int* size,
		     int* ok)
{ // cacheSize
  *ok = cencalvm_cacheSize((void*) *handleAddr, *size);
} // cacheSize

#include <iostream>
// ----------------------------------------------------------------------
// Query the database.
void
cencalvm_query_f(size_t* handleAddr,
		 double* pVals,
		 const int* numVals,
		 const double* lon,
		 const double* lat,
		 const double* elev,
		 int *ok)
{ // query
  *ok = cencalvm_query((void*) *handleAddr, &pVals, *numVals, 
		       *lon, *lat, *elev);
} // query


// version
// $Id$

// End of file
