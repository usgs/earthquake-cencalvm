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
 * @brief C++ manager for querying the USGS central CA velocity model.
 */

#if !defined(cencalvm_query_vmquery_h)
#define cencalvm_query_vmquery_h

#include "cencalvm/storage/etreefwd.h" // USES etree_t

namespace cencalvm {
  namespace query {
    class VMQuery;
  }; // query
}; // cencalvm

/// C++ manager for querying the USGS central CA velocity model.
class cencalvm::query::VMQuery
{ // class VMQuery

 public :
  // PUBLIC ENUM ////////////////////////////////////////////////////////

  /// Type of query
  enum QueryEnum { MAXRES=0, FIXEDRES=1, AVGRES=2 };

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

  /** Set size of cache during queries.
   *
   * @param size Size of cache in ??
   */
  void cacheSize(const int size);

  /** Query the database.
   *
   * @pre Must call Open() before Query()
   *
   * @param pVals Pointer to computed values (output from query)
   * @param numVals Number of values expected (size of pVals array)
   * @param lon Longitude of location for query in degrees
   * @param lat Latitude of location for query in degrees
   * @param elev Elevation of location for query in meters
   */
  void query(double** pVals,
	     const int numVals,
	     const double lon,
	     const double lat,
	     const double elev);

private :
  // PRIVATE METHODS ////////////////////////////////////////////////////

  /** Query database at maximum resolution possible.
   *
   * @param pVals Pointer to computed values (output from query)
   * @param numVals Number of values expected (size of pVals array)
   * @param lon Longitude of location for query in degrees
   * @param lat Latitude of location for query in degrees
   * @param elev Elevation of location for query in meters
   */
  void _queryMax(double** pVals,
		 const int numVals,
		 const double lon,
		 const double lat,
		 const double elev);

  /** Query database at fixed resolution. Resolution is specified by
   * queryRes().
   *
   * @param pVals Pointer to computed values (output from query)
   * @param numVals Number of values expected (size of pVals array)
   * @param lon Longitude of location for query in degrees
   * @param lat Latitude of location for query in degrees
   * @param elev Elevation of location for query in meters
   */
  void _queryFixed(double** pVals,
		 const int numVals,
		 const double lon,
		 const double lat,
		 const double elev);

  /** Query database at resolution specified by wavelength. Resolution
   * is specified by queryRes().
   *
   * @param pVals Pointer to computed values (output from query)
   * @param numVals Number of values expected (size of pVals array)
   * @param lon Longitude of location for query in degrees
   * @param lat Latitude of location for query in degrees
   * @param elev Elevation of location for query in meters
   */
  void _queryAvg(double** pVals,
		 const int numVals,
		 const double lon,
		 const double lat,
		 const double elev);

 private :
  // PRIVATE METHODS ////////////////////////////////////////////////////

  VMQuery(const VMQuery& q); ///< Not implemented
  const VMQuery& operator=(const VMQuery& q); ///< Not implemented
  
private :
 // PRIVATE MEMBERS ////////////////////////////////////////////////////
  
  double _queryRes; ///< Resolution of query (if specified)

  etree_t* _db; ///< Database
  const char* _filename; ///< Name of database file

  int* _pQueryVals; ///< Indices of values to be returned in queries
  int _querySize; ///< Number of values requested to be return in queries

  int _cacheSize; ///< Size of query cache

  typedef void (*cencalvm::query::VMQuery::queryFn_t)(double**, int, double, double, double);
  queryFn_t _queryFn; ///< Method to call for queries

}; // class VMQuery 

#include "VMQuery.icc" // inline methods

#endif // cencalvm_query_vmquery_h

// version
// $Id$

// End of file 
