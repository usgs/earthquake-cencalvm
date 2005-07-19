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

/** @file lib/ErrorHandler.h
 *
 * @brief C++ error handler associated with the USGS central CA velocity
 * model.
 */

#if !defined(cencalvm_storage_errorhandler_h)
#define cencalvm_storage_errorhandler_h

#include <string> // HASA std::string
#include <iosfwd> // HOLDSA std::ostream

namespace cencalvm {
  namespace storage {
    class ErrorHandler;
    class TestErrorHandler; // friend
  }; // namespace storage
}; // namespace cencalvm

/// C++ error handler associated with the USGS central CA velocity model.
class cencalvm::storage::ErrorHandler
{ // Geometry
  friend class TestErrorHandler;

public :
  // PUBLIC ENUMS ///////////////////////////////////////////////////////

  /// Enumerated type for error status.
  enum StatusEnum { 
    OK, ///< No errors
    WARNING, ///< Non-fatal error
    ERROR ///< Fatal error
  };

public :
  // PUBLIC METHODS /////////////////////////////////////////////////////

  /// Constructor
  ErrorHandler(void);

  /// Destructor
  ~ErrorHandler(void);

  /** Set filename for logging.
   *
   * @param filename Name of file
   */
  void logFilename(const char* filename);

  /** Turn logging on/off.
   *
   * @param turnOn True to turn on logging, false to turn logging off
   */
  void loggingOn(const bool turnOn =true);

  /// Reset error status and message.
  void resetStatus(void);

  /** Get status.
   *
   * @returns Status of errors
   */
  StatusEnum status(void) const;

  /** Get warning/error message.
   *
   * @returns Warning/error message
   */
  const char* message(void) const;

  /** Set status to error and store error message.
   *
   * @param msg Error message
   */
  void error(const char* msg);
  
  /** Set status to warning and store warning message.
   *
   * @param msg Warning message
   */
  void warning(const char* msg);
  
  /** Write message to log file.
   *
   * @param msg Message to write to log file
   */
  void log(const char* msg);
  
 private :
  // PRIVATE METHODS ////////////////////////////////////////////////////

  ErrorHandler(const ErrorHandler& h); ///< Not implemented
  const ErrorHandler& operator=(const ErrorHandler& h); ///< Not implemented
  
private :
  // PRIVATE MEMBERS ////////////////////////////////////////////////////

  std::string _message; ///< Message associated with error/warning
  std::string _logFilename; ///< Name of log file
  std::ofstream* _pLogFile; ///< Pointer to log file
  StatusEnum _status; ///< Error status

  static const char* _NULLFILE; ///< Name of null device

}; // ErrorHandler

#include "ErrorHandler.icc" // inline methods

#endif // cencalvm_storage_errorhandler_h

// version
// $Id$

// End of file 
