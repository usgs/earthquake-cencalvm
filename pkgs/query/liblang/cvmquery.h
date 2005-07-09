/*  -*- C -*-  */
/*
 * ======================================================================
 *
 *                           Brad T. Aagaard
 *                        U.S. Geological Survey
 *
 * {LicenseText}
 *
 * ======================================================================
 */

/** @file lib/cvmquery.h
 *
 * @brief C interface definitions for querying the USGS central CA velocity
 * model (USER INTERFACE).
 *
 * These are the routines that are used to query the USGS central CA
 * velocity model from C. See f77vmquery.h for the routines to query
 * the velocity model using Fortran via Fortran 77. If you are using
 * C++, you should use the VMQuery object directly.
 */

#if !defined(cencalvm_cvmquery_h)
#define cencalvm_cvmquery_h

/** Create velocity model query object.
 *
 * Calls VMQuery().
 *
 * @returns pointer to VMQuery object
 */
void* cencalvm_createQuery(void);

/** Destroy velocity model query object.
 *
 * Calls ~VMQuery().
 *
 * @param handle Pointer to query
 *
 * @returns 1 on success, 0 on error
 */
int cencalvm_destroyQuery(void* handle);

/** Open database for querying.
 *
 * @param handle Pointer to query
 *
 * @returns 1 on success, 0 on error
 */
int cencalvm_open(void* handle);
  
/** Close the database.
 *
 * @param handle Pointer to query
 *
 * @returns 1 on success, 0 on error
 */
int cencalvm_close(void* handle);
  
/** Set query type.
 *
 * Queries other than maximum resolution require an etree database
 * that has been spatially averaged. This fills in parent octants with
 * the average of its children. 
 *
 * @param handle Pointer to query
 * @param queryType Type of query
 *   @li =0  Query at maximum resolution
 *   @li =1  Query at fixed resolution
 *   @li =2  Query at resolution tuned to wavelength of shear waves
 *
 * @returns 1 on success, 0 on error
 */
int cencalvm_queryType(void* handle,
		       const int queryType);

/** Set query resolution.
 *
 * Meaning depends on type of query:
 *   @li MAXRES Resolution is not applicable
 *   @li FIXEDRES Query etree at level corresponding to resolution
 *   @li AVGRES Resolution corresponds to minimum period of waves;
 *     etree is queried at lavel corresponding to minimum wavelength
 *     for shear waves
 *
 * @param handle Pointer to query
 * @param queryRes Resolution of query.
 *
 * @returns 1 on success, 0 on error
 */
int cencalvm_queryRes(void* handle,
		      const double res);

/** Set values to be returned by queries. Default is to return all
 * values in order they appear in PayloadStruct.
 *
 * @param handle Pointer to query
 * @param names Names of values to be returned in queries
 * @param numVals Number of values to be returned in queries
 *
 * @returns 1 on success, 0 on error
 */
int cencalvm_queryVals(void* handle,
		       const char** names,
		       const int numVals);

/** Set the database filename.
 *
 * @param handle Pointer to query
 * @param filename Name of database file
 *
 * @returns 1 on success, 0 on error
 */
int cencalvm_filename(void* handle,
		      const char* filename);

/** Set size of cache during queries.
 *
 * @param handle Pointer to query
 * @param size Size of cache in ??
 *
 * @returns 1 on success, 0 on error
 */
int cencalvm_cacheSize(void* handle,
		       const int size);

/** Query the database.
 *
 * @warning Array for values to be returned must be allocated BEFORE
 * query.
 *
 * @pre Must call Open() before Query()
 *
 * @param handle Pointer to query
 * @param ppVals Pointer to array holding computed values (output from query)
 * @param numVals Number of values expected (size of array (preallocated))
 * @param lon Longitude of location for query in degrees
 * @param lat Latitude of location for query in degrees
 * @param elev Elevation of location wrt MSL in meters
 *
 * @returns 1 on success, 0 on error
 */
int cencalvm_query(void* handle,
		   double** ppVals,
		   const int numVals,
		   const double lon,
		   const double lat,
		   const double elev);

#endif /* cencalvm_cvmquery_h */

/* version */
/* $Id$ */

/* End of file  */
