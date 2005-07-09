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

#if !defined(cencalvm_f77vmquery_h)
#define cencalvm_f77vmquery_h

// ----------------------------------------------------------------------
#define cencalvm_createquery_f \
  FORTRAN_NAME_(cencalvm_createquery_f, CENCALVM_CREATEQUERY_F)
/** Create velocity model query object.
 *
 * Create handle to velocity model query object. If creation fails
 * address of handle is set to 0.
 *
 * @param handleAddr Address of handle to VMQuery object.
 */
extern "C"
void cencalvm_createquery_f(size_t* handleAddr);

// ----------------------------------------------------------------------
#define cencalvm_destroyquery_f \
  FORTRAN_NAME_(cencalvm_destroyquery_f, CENCALVM_DESTROYQUERY_F)
/** Destroy velocity model query object.
 *
 * Calls ~VMQuery().
 *
 * @param handleAddr Addreee of handle to VMQuery object
 * @param ok set to 1 on success, 0 on error
 */
extern "C"
void cencalvm_destroyquery_f(size_t* handleAddr,
			     int* ok);

// ----------------------------------------------------------------------
#define cencalvm_open_f \
  FORTRAN_NAME_(cencalvm_open_f, CENCALVM_OPEN_F)
/** Open database for querying.
 *
 * @param handleAddr Addreee of handle to VMQuery object
 * @param ok set to 1 on success, 0 on error
 */
extern "C"
void cencalvm_open_f(size_t* handleAddr,
		     int* ok);
  
// ----------------------------------------------------------------------
#define cencalvm_close_f \
  FORTRAN_NAME_(cencalvm_close_f, CENCALVM_CLOSE_F)
/** Close the database.
 *
 * @param handleAddr Addreee of handle to VMQuery object
 * @param ok set to 1 on success, 0 on error
 */
extern "C"
void cencalvm_close_f(size_t* handleAddr,
		      int* ok);

// ----------------------------------------------------------------------
#define cencalvm_querytype_f \
  FORTRAN_NAME_(cencalvm_querytype_f, CENCALVM_QUERYTYPE_F)
/** Set query type.
 *
 * Queries other than maximum resolution require an etree database
 * that has been spatially averaged. This fills in parent octants with
 * the average of its children. 
 *
 * @param handleAddr Addreee of handle to VMQuery object
 * @param queryType Type of query
 *   @li =0  Query at maximum resolution
 *   @li =1  Query at fixed resolution
 *   @li =2  Query at resolution tuned to wavelength of shear waves
 * @param ok set to 1 on success, 0 on error
 */
extern "C"
void cencalvm_querytype_f(size_t* handleAddr,
			  const int* queryType,
			  int* ok);

// ----------------------------------------------------------------------
#define cencalvm_queryres_f \
  FORTRAN_NAME_(cencalvm_queryres_f, CENCALVM_QUERYRES_F)
/** Set query resolution.
 *
 * Meaning depends on type of query:
 *   @li MAXRES Resolution is not applicable
 *   @li FIXEDRES Query etree at level corresponding to resolution
 *   @li AVGRES Resolution corresponds to minimum period of waves;
 *     etree is queried at lavel corresponding to minimum wavelength
 *     for shear waves
 *
 * @param handleAddr Addreee of handle to VMQuery object
 * @param queryRes Resolution of query.
 * @param ok set to 1 on success, 0 on error
 */
extern "C"
void cencalvm_queryres_f(size_t* handleAddr,
			 const double* res,
			 int* ok);

// ----------------------------------------------------------------------
#define cencalvm_filename_f \
  FORTRAN_NAME_(cencalvm_filename_f, CENCALVM_FILENAME_F)
/** Set the database filename.
 *
 * @param handleAddr Addreee of handle to VMQuery object
 * @param filename Name of database file
 * @param len Length of string (IMPLICIT IN FORTRAN)
 * @param ok set to 1 on success, 0 on error
 */
extern "C"
void cencalvm_filename_f(size_t* handleAddrVal,
			 const char* filename,
			 int* ok,
			 const int len);

// ----------------------------------------------------------------------
#define cencalvm_cachesize_f \
  FORTRAN_NAME_(cencalvm_cachesize_f, CENCALVM_CACHESIZE_F)
/** Set size of cache during queries.
 *
 * @param handleAddr Addreee of handle to VMQuery object
 * @param size Size of cache in ??
 * @param ok set to 1 on success, 0 on error
 */
extern "C"
void cencalvm_cachesize_f(size_t* handleAddr,
			  const int* size,
			  int* ok);

// ----------------------------------------------------------------------
#define cencalvm_query_f \
  FORTRAN_NAME_(cencalvm_query_f, CENCALVM_QUERY_F)
/** Query the database.
 *
 * @pre Must call Open() before Query()
 *
 * @param handleAddr Addreee of handle to VMQuery object
 * @param pVals Pointer to computed values (output from query)
 * @param numVals Number of values expected (size of array (preallocated))
 * @param lon Longitude of location for query in degrees
 * @param lat Latitude of location for query in degrees
 * @param elev Elevation of location wrt MSL in meters
 * @param ok set to 1 on success, 0 on error
 */
extern "C"
void cencalvm_query_f(size_t* handleAddr,
		      double* pVals,
		      const int* numVals,
		      const double* lon,
		      const double* lat,
		      const double* elev,
		      int* ok);

#endif // cencalvm77vmquery_h

// version
// $Id$

// End of file
