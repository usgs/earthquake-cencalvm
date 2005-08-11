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

// Application demonstrating how to do queries from C++ code.

// Comment out the following line to use code that doesn't use all query values
#define ALLVALS


#include "cencalvm/query/VMQuery.h" // USES VMQuery
#include "cencalvm/storage/ErrorHandler.h" // USES ErrorHandler

#include <stdlib.h> // USES exit()
#include <unistd.h> // USES getopt()

#include <fstream> // USES std::ifstream, std::ofstream
#include <iomanip> // USES setw(), setiosflags(), resetiosflags()

#include <iostream> // USES std::cerr
#include <sstream> // USES std::ostringstream
#include <assert.h> // USES assert()

#include <vector> // USES std::vector
#include <string> // USES std::string

// ----------------------------------------------------------------------
// Dump usage to std::cout
void
usage(void)
{ // usage
  std::cerr
    << "usage: cencalvmcppquery [-h] -i fileIn -o fileOut -d dbfile [-l logfile]\n"
    << "  -i fileIn   File containing list of locations: 'lon lat elev'.\n"
    << "  -o fileOut  Output file with locations and material properties.\n"
    << "  -d dbfile   Etree database file to query.\n"
    << "  -h          Display usage and exit.\n"
    << "  -l logfile  Log file for warnings about no data for locations.\n";
} // usage

// ----------------------------------------------------------------------
// Parse command line arguments
void
parseArgs(std::string* pFilenameIn,
	  std::string* pFilenameOut,
	  std::string* pFilenameDB,
	  std::string* pFilenameLog,
	  int argc,
	  char** argv)
{ // parseArgs
  assert(0 != pFilenameIn);
  assert(0 != pFilenameOut);
  assert(0 != pFilenameDB);
  assert(0 != pFilenameLog);

  extern char* optarg;

  int nparsed = 1;
  *pFilenameIn = "";
  *pFilenameOut = "";
  *pFilenameDB = "";
  *pFilenameLog = "";
  int c = EOF;
  while ( (c = getopt(argc, argv, "hi:l:o:d:") ) != EOF) {
    switch (c)
      { // switch
	case 'i' : // process -i option
	  *pFilenameIn = optarg;
	  nparsed += 2;
	  break;
	case 'o' : // process -o option
	  *pFilenameOut = optarg;
	  nparsed += 2;
	  break;
	case 'd' : // process -d option
	  *pFilenameDB = optarg;
	  nparsed += 2;
	  break;
	case 'h' : // process -h option
	  nparsed += 1;
	  usage();
	  exit(0);
	  break;
	case 'l' : // process -l option
	  *pFilenameLog = optarg;
	  nparsed += 2;
	  break;
	default :
	  usage();
	} // switch
    } // while
  if (nparsed != argc || 
      0 == pFilenameIn->length() ||
      0 == pFilenameOut->length() ||
      0 == pFilenameDB->length()) {
    usage();
    exit(1);
  } // if
} // parseArgs

// ----------------------------------------------------------------------
// main
int
main(int argc,
     char* argv[])
{ // main
  std::string filenameIn = "";
  std::string filenameOut = "";
  std::string filenameDB = "";
  std::string filenameLog = "";
  
  // Parse command line arguments
  parseArgs(&filenameIn, &filenameOut, &filenameDB, &filenameLog, argc, argv);

  // Create query
  cencalvm::query::VMQuery query;

  // Get handle to error handler
  cencalvm::storage::ErrorHandler* pErrHandler = query.errorHandler();
  if (0 == pErrHandler) {
    std::cerr << "Could not get handle to error handler.\n";
    return 1;
  } // if
    
  // If log filename has been set, set log filename in error handler
  if (filenameLog.length() > 0)
    pErrHandler->logFilename(filenameLog.c_str());

  // Set database filename
  query.filename(filenameDB.c_str());
  if (cencalvm::storage::ErrorHandler::OK != pErrHandler->status()) {
    std::cerr << pErrHandler->message();
    return 1;
  } // if

  // Set values to be returned in queries (or not)
#if !defined(ALLVALS)
  const int numVals = 2;
  const char* pValNames[] = { "FaultBlock", "Zone" };
  query.queryVals(pValNames, numVals);
  if (cencalvm::storage::ErrorHandler::OK != pErrHandler->status()) {
    std::cerr << pErrHandler->message();
    return 1;
  } // if
#else
  const int numVals = 8;
#endif

  // Open database for querying
  query.open();
  if (cencalvm::storage::ErrorHandler::OK != pErrHandler->status()) {
    std::cerr << pErrHandler->message();
    return 1;
  } // if

  // Open input file to read locations
  std::ifstream fileIn(filenameIn.c_str());
  if (!fileIn.is_open()) {
    std::cerr << "Could not open file '" << filenameIn
	      << "' to read query locations.\n";
    return 1;
  } // if
  
  // Open output file to accept data
  std::ofstream fileOut(filenameOut.c_str());
  if (!fileOut.is_open()) {
    std::cerr << "Could not open file '" << filenameOut
	      << "' to receive query data.\n";
    return 1;
  } // if

  // Create array to hold values returned in queries
  double* pVals = (numVals > 0) ? new double[numVals] : 0;

  // Read location from input file
  double lon = 0.0;
  double lat = 0.0;
  double elev = 0.0;
  fileIn >> lon >> lat >> elev;

  // Continue operating on locations until end of file, reading fails,
  // or writing fails
  while (!fileIn.eof() && fileIn.good() && fileOut.good()) {
    // Query database
    query.query(&pVals, numVals, lon, lat, elev);

    // If query generated a warning or error, dump message to std::cerr
    if (cencalvm::storage::ErrorHandler::OK != pErrHandler->status()) {
      std::cerr << pErrHandler->message();
      // If query generated an error, then bail out, otherwise reset status
      if (cencalvm::storage::ErrorHandler::ERROR == pErrHandler->status())
	return 1;
      pErrHandler->resetStatus();
    } // if

    // Write values returned by query to output file
    fileOut
      << std::resetiosflags(std::ios::scientific)
      << std::setiosflags(std::ios::fixed)
      << std::setprecision(4)
      << std::setw(9) << lon
      << std::setw(8) << lat
      << std::setprecision(1) << std::setw(9) << elev;
#if !defined(ALLVALS)
    fileOut
      << std::setw(4) << int(pVals[0])
      << std::setw(4) << int(pVals[1])
      << "\n";
#else
    fileOut
      << std::setprecision(1)
      << std::setw(8) << pVals[0]
      << std::setw(8) << pVals[1]
      << std::setw(8) << pVals[2]
      << std::setw(9) << pVals[3]
      << std::setw(9) << pVals[4]
      << std::setw(9) << pVals[5]
      << std::setw(4) << int(pVals[6])
      << std::setw(4) << int(pVals[7])
      << "\n";
#endif
    
    // Read in next location from input file
    fileIn >> lon >> lat >> elev;
  } // while
  
  // Close database
  query.close();

  // Close input and output files
  fileIn.close();
  fileOut.close();

  // If an error was generated, write error message and bail out
  if (cencalvm::storage::ErrorHandler::OK != pErrHandler->status()) {
    std::cerr << pErrHandler->message();
    return 1;
  } // if

  return 0;
} // main

// version
// $Id$

// End of file 
