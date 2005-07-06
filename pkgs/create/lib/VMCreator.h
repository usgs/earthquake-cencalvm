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

/** @file lib/VMCreator.h
 *
 * @brief C++ manager for creating the velocity model
 * database.
 */

#if !defined(cencalvm_create_vmcreator_h)
#define cencalvm_create_vmcreator_h

#include <string> // USES std::string

namespace cencalvm {
  namespace create {
    class VMCreator;
    class TestVMCreator; // friend
  }; // namespace create
}; // namespace cencalvm

/// C++ manager for creating the velocity model database.
class cencalvm::create::VMCreator
{ // VMCreator
  friend class TestVMCreator;

public :
  // PUBLIC METHODS /////////////////////////////////////////////////////

  /// Constructor
  VMCreator(void);

  /// Destructor
  ~VMCreator(void);

  /** Set filename for parameter file.
   *
   * @param filename Name of file
   */
  void filenameParams(const char* filename);

  /** Set filename for output file.
   *
   * @param filename Name of file
   */
  void filenameOut(const char* filename);

  /** Set filename for temporary (scratch) file.
   *
   * @param filename Name of file
   */
  void filenameTmp(const char* filename);

  /// Create the velocity model database.
  void run(void) const;

private :
  // PRIVATE METHODS ////////////////////////////////////////////////////

  /** Read parameter file.
   *
   * @param ppGridFilenames Pointer to array of input grid filenames
   * @param pNumGrids Pointer to number of input grids
   */
  void _readParams(std::string** pGridFilenames,
		   int* pNumGrids) const;

  /// Create the unpacked etree database.
  void _createDB(void) const;

  /// Create packed etree database from unpacked etree database.
  void _packDB(void) const;

 private :
  // PRIVATE METHODS ////////////////////////////////////////////////////

  VMCreator(const VMCreator& c); ///< Not implemented
  const VMCreator& operator=(const VMCreator& c); ///< Not implemented
  
private :
  // PRIVATE MEMBERS ////////////////////////////////////////////////////

  const char* _filenameParams; ///< Filename for parameter file
  const char* _filenameOut; ///< Filename for output file
  const char* _filenameTmp; ///< Filename for temporary file

}; // VMCreator

#include "VMCreator.icc" // inline methods

#endif // cencalvm_create_vmcreator_h



// version
// $Id$

// End of file 
