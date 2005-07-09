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

extern "C" {
#include "cvmquery.h"
}

#include "cencalvm/query/VMQuery.h" // USES VMQuery

#include <stdexcept> // USES std::exception
#include <iostream> // USES std::cerr

// ----------------------------------------------------------------------
static const int VM_OK = 0;
static const int VM_ERROR = 1;

// ----------------------------------------------------------------------
// Create velocity model query object.
void*
cencalvm_createQuery(void)
{ // createQuery
  void* handle = (void*) new cencalvm::query::VMQuery::VMQuery;
  return handle;
} // createQuery

// ----------------------------------------------------------------------
// Destroy velocity model query object.
int
cencalvm_destroyQuery(void* handle)
{ // destroyQuery
  cencalvm::query::VMQuery* pQuery = (cencalvm::query::VMQuery*) handle;

  delete pQuery;

  return VM_OK;
} // destroyQuery

// ----------------------------------------------------------------------
// Open database for querying.
int
cencalvm_open(void* handle)
{ // open
  if (0 == handle) {
    std::cerr << "Null handle for query manager in call to open()."
	      << std::endl;
    return VM_ERROR;
  } // if

  try {
    cencalvm::query::VMQuery* pQuery = (cencalvm::query::VMQuery*) handle;
    pQuery->open();
  } // try
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return VM_ERROR;
  } // catch
  catch (...) {
    std::cerr << "Unknown error occurred while opening etree database."
	      << std::endl;
    return VM_ERROR;
  } // catch

  return VM_OK;
} // open
  
// ----------------------------------------------------------------------
// Close the database.
int
cencalvm_close(void* handle)
{ // close
  if (0 == handle) {
    std::cerr << "Null handle for query manager in call to close()."
	      << std::endl;
    return VM_ERROR;
  } // if

  try {
    cencalvm::query::VMQuery* pQuery = (cencalvm::query::VMQuery*) handle;
    pQuery->close();
  } // try
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return VM_ERROR;
  } // catch
  catch (...) {
    std::cerr << "Unknown error occurred while closing etree database."
	      << std::endl;
    return VM_ERROR;
  } // catch

  return VM_OK;
} // close
  
// ----------------------------------------------------------------------
// Set query type.
int
cencalvm_queryType(void* handle,
		   const int queryType)
{ // queryType
  if (0 == handle) {
    std::cerr << "Null handle for query manager in call to queryType()."
	      << std::endl;
    return VM_ERROR;
  } // if

  try {
    cencalvm::query::VMQuery* pQuery = (cencalvm::query::VMQuery*) handle;
    cencalvm::query::VMQuery::QueryEnum queryEnum = 
      cencalvm::query::VMQuery::QueryEnum(queryType);
    pQuery->queryType(queryEnum);
  } // try
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return VM_ERROR;
  } // catch
  catch (...) {
    std::cerr << "Unknown error occurred while setting query type."
	      << std::endl;
    return VM_ERROR;
  } // catch

  return VM_OK;
} // queryType

// ----------------------------------------------------------------------
// Set query resolution.
int
cencalvm_queryRes(void* handle,
		  const double res)
{ // queryRes
  if (0 == handle) {
    std::cerr << "Null handle for query manager in call to queryRes()."
	      << std::endl;
    return VM_ERROR;
  } // if

  try {
    cencalvm::query::VMQuery* pQuery = (cencalvm::query::VMQuery*) handle;
    pQuery->queryRes(res);
  } // try
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return VM_ERROR;
  } // catch
  catch (...) {
    std::cerr << "Unknown error occurred while setting query resolution."
	      << std::endl;
    return VM_ERROR;
  } // catch

  return VM_OK;
} // queryRes

// ----------------------------------------------------------------------
// Set values to be returned by queries. Default is to return all
int 
cencalvm_queryVals(void* handle,
		   const char** names,
		   const int numVals)
{ // queryVals
  if (0 == handle) {
    std::cerr << "Null handle for query manager in call to queryVals()."
	      << std::endl;
    return VM_ERROR;
  } // if

  try {
    cencalvm::query::VMQuery* pQuery = (cencalvm::query::VMQuery*) handle;
    pQuery->queryVals(names, numVals);
  } // try
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return VM_ERROR;
  } // catch
  catch (...) {
    std::cerr << "Unknown error occurred while setting query values."
	      << std::endl;
    return VM_ERROR;
  } // catch

  return VM_OK;
} // queryVals

// ----------------------------------------------------------------------
// Set the database filename.
int
cencalvm_filename(void* handle,
		   const char* filename)
{ // filename
  if (0 == handle) {
    std::cerr << "Null handle for query manager in call to filename()."
	      << std::endl;
    return VM_ERROR;
  } // if

  try {
    cencalvm::query::VMQuery* pQuery = (cencalvm::query::VMQuery*) handle;
    pQuery->filename(filename);
  } // try
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return VM_ERROR;
  } // catch
  catch (...) {
    std::cerr << "Unknown error occurred while setting etree database "
	      << "filename."
	      << std::endl;
    return VM_ERROR;
  } // catch

  return VM_OK;
} // filename

// ----------------------------------------------------------------------
// Set size of cache during queries.
int
cencalvm_cacheSize(void* handle,
		   const int size)
{ // cacheSize
  if (0 == handle) {
    std::cerr << "Null handle for query manager in call to cacheSize()."
	      << std::endl;
    return VM_ERROR;
  } // if

  try {
    cencalvm::query::VMQuery* pQuery = (cencalvm::query::VMQuery*) handle;
    pQuery->cacheSize(size);
  } // try
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return VM_ERROR;
  } // catch
  catch (...) {
    std::cerr << "Unknown error occurred while cache size."
	      << std::endl;
    return VM_ERROR;
  } // catch

  return VM_OK;
} // cacheSize

// ----------------------------------------------------------------------
// Query the database.
int
cencalvm_query(void* handle,
	       double** ppVals,
	       const int numVals,
	       const double lon,
	       const double lat,
	       const double elev)
{ // query
  if (0 == handle) {
    std::cerr << "Null handle for query manager in call to query()."
	      << std::endl;
    return VM_ERROR;
  } // if

  try {
    cencalvm::query::VMQuery* pQuery = (cencalvm::query::VMQuery*) handle;
    pQuery->query(ppVals, numVals, lon, lat, elev);
  } // try
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return VM_ERROR;
  } // catch
  catch (...) {
    std::cerr << "Unknown error occurred while querying etree database."
	      << std::endl;
    return VM_ERROR;
  } // catch

  return VM_OK;
} // query

// version
// $Id$

// End of file
