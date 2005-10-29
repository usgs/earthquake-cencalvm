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

/** @file lib/VMQuery.h
 *
 * @brief C++ manager for querying the USGS central CA velocity
 * model (USER INTERFACE).
 *
 * The current defaults for queries are:
 * @li return all values in a query,
 * @li query at the maximum resolution of the model
 * @li 128 MB cache for queries
 *
 * The default behavior can be modified by calling the appropriate
 * class method, e.g., queryType(), cacheSize(), queryVals().
 *
 * The general order of use is:
 *
 * <ol>
 * <li> Create query object using cencalvm::query::VMQuery::VMQuery()
 * <li> Set filename of database using cencalvm::query::VMQuery::filename()
 * <li> Optionally, set cache size using cencalvm::query::VMQuery::cacheSize()
 * <li> Optionally, set values to return in query using 
 *   cencalvm::query::VMQuery::queryVals()
 * <li> Open database using cencalvm::query::VMQuery::open()
 * <li> Query database using cencalvm::query::VMQuery::query()
 * <li> Close database using cencalvm::query::VMQuery::close()
 * </ol>
 */

#if !defined(cencalvm_query_vmquery_h)
#define cencalvm_query_vmquery_h

#include "cencalvm/storage/etreefwd.h" // USES etree_t

#include <string> // USES std::string

namespace cencalvm {
  namespace query {
    class VMQuery;
    class TestVMQuery; // friend
  } // query
  namespace storage {
    class Geometry; // HOLDSA geometry
    class ErrorHandler; // HOLDA ErrorHandler
    struct PayloadStruct; // USES PayloadStruct
  } // storage
} // cencalvm

/// C++ manager for querying the USGS central CA velocity model (USER
/// INTERFACE).
class cencalvm::query::VMQuery
{ // class VMQuery
  friend class TestVMQuery; // unit testing

 public :
  // PUBLIC ENUM ////////////////////////////////////////////////////////

  /// Type of query
  enum QueryEnum { 
    MAXRES=0, ///< Query at maximum resolution
    FIXEDRES=1, ///< Query at fixed resolution
    WAVERES=2 ///< Query at resolution tuned to wavelength of shear waves
  };

 public :
  // PUBLIC METHODS /////////////////////////////////////////////////////

  /// Default constructor.
  VMQuery(void);
  
  /// Default destructor.
  ~VMQuery(void);
  
  /// Open the database and prepare for querying.
  void open(void);
  
  /// Close the database.
  void close(void);
  
  /** Set query type.
   *
   * @param queryType Set type of query
   */
  void queryType(const QueryEnum queryType);

  /** Set query resolution. Resolution is associated with vertical
   * resolution, which is 4 times greater than the horizontal
   * resolution.
   *
   * Meaning depends on type of query:
   *   @li MAXRES Resolution is not applicable
   *   @li FIXEDRES Query etree at level corresponding to resolution
   *   @li WAVERES Resolution corresponds to minimum period of waves;
   *     etree is queried at level corresponding to minimum wavelength
   *     for shear waves.
   *
   * @param res Resolution of query.
   */
  void queryRes(const double res);

  /** Set values to be returned by queries. Default is to return all
   * values in order they appear in PayloadStruct.
   *
   * @param names Names of values to be returned in queries
   * @param numVals Number of values to be returned in queries
   */
  void queryVals(const char** names,
		 const int numVals);

  /** Set the database filename.
   *
   * @param filename Name of database file
   */
  void filename(const char* filename);

  /** Set the database filename for the extended model.
   *
   * @param filename Name of database file for the extended model
   */
  void filenameExt(const char* filename);

  /** Set size of cache during queries.
   *
   * @param size Size of cache in MB
   */
  void cacheSize(const int size);

  /** Set size of cache during queries in the extended model.
   *
   * @param size Size of cache in MB for the extended model
   */
  void cacheSizeExt(const int size);

  /** Query the database.
   *
   * @warning Array for values to be returned must be allocated BEFORE
   * query.
   *
   * @note Longitude and latitude are given in degrees in the WGS84 datum.
   *
   * @note Elevation is given in meters with respect to mean sea level.
   *
   * @note Values will be returned either of the order they are shown
   * in cencalvm::storage::PayloadStruct or as specified with a call
   * to queryVals().
   *
   * @param ppVals Pointer to computed values (output from query)
   * @param numVals Number of values expected (size of array (preallocated))
   * @param lon Longitude of location for query in degrees
   * @param lat Latitude of location for query in degrees
   * @param elev Elevation of location wrt MSL in meters
   */
  void query(double** ppVals,
	     const int numVals,
	     const double lon,
	     const double lat,
	     const double elev);

  /** Get handle to error handler.
   *
   * @returns Pointer to Error handler
   */
  cencalvm::storage::ErrorHandler* errorHandler(void);

private :
  // PRIVATE METHODS ////////////////////////////////////////////////////

  /** Query database at maximum resolution possible.
   *
   * @param pPayload Pointer to database payload
   * @param pDB Database to search
   * @param lon Longitude of location for query in degrees
   * @param lat Latitude of location for query in degrees
   * @param elev Elevation of location for query in meters
   */
  void _queryMax(cencalvm::storage::PayloadStruct*,
		 etree_t* pDB,
		 const double lon,
		 const double lat,
		 const double elev);
  
  /** Query database at fixed resolution. Resolution is specified by
   * queryRes().
   *
   * @param pPayload Pointer to database payload
   * @param pDB Database to search
   * @param lon Longitude of location for query in degrees
   * @param lat Latitude of location for query in degrees
   * @param elev Elevation of location for query in meters
   */
  void _queryFixed(cencalvm::storage::PayloadStruct*,
		   etree_t* pDB,
		   const double lon,
		   const double lat,
		   const double elev);

  /** Query database at resolution specified by wavelength. Resolution
   * is specified by queryRes().
   *
   * @param pPayload Pointer to database payload
   * @param pDB Database to search
   * @param lon Longitude of location for query in degrees
   * @param lat Latitude of location for query in degrees
   * @param elev Elevation of location for query in meters
   */
  void _queryWave(cencalvm::storage::PayloadStruct*,
		  etree_t* pDB,
		  const double lon,
		  const double lat,
		  const double elev);

 private :
  // PRIVATE METHODS ////////////////////////////////////////////////////

  VMQuery(const VMQuery& q); ///< Not implemented
  const VMQuery& operator=(const VMQuery& q); ///< Not implemented
  
private :
 // PRIVATE MEMBERS ////////////////////////////////////////////////////
  
  double _queryRes; ///< Vertical resolution of query (if specified)

  etree_t* _db; ///< Database for detailed model
  etree_t* _dbExt; ///< Database for extended model
  std::string _filename; ///< Name of database file for detailed model
  std::string _filenameExt; ///< Name of database file for extended model

  int* _pQueryVals; ///< Address offsets in payload for query values
  int _querySize; ///< Number of values requested to be return in queries
  int _cacheSize; ///< Size of query cache for detailed model
  int _cacheSizeExt; ///< Size of query cache for extended model

  typedef void (cencalvm::query::VMQuery::*queryFn_t)
    (cencalvm::storage::PayloadStruct*, etree_t*, double, double, double);
  queryFn_t _queryFn; ///< Method to call for queries

  cencalvm::storage::Geometry* _pGeom; ///< Velocity model geometry
  cencalvm::storage::ErrorHandler* _pErrHandler; ///< Error handler

}; // class VMQuery 

#include "VMQuery.icc" // inline methods

#endif // cencalvm_query_vmquery_h

// version
// $Id$

// End of file 
