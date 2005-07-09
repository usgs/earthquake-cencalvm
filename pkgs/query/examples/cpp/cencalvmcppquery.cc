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

// Switch used to select whether all values or selected ones are queried.
#define ALLVALS


#include "cencalvm/query/VMQuery.h"

#include <stdlib.h> // USES exit()
#include <unistd.h> // USES getopt()

#include <fstream> // USES std::ifstream, std::ofstream
#include <iomanip> // USES setw(), setiosflags(), resetiosflags()

#include <iostream> // USES std::cerr
#include <sstream> // USES std::ostringstream
#include <stdexcept> // USES std::exception

#include <vector> // USES std::vector
#include <string> // USES std::string

// ----------------------------------------------------------------------
void
usage(void)
{ // usage
  std::cerr
    << "usage: cencalvmcppquery [-h] -i fileIn -o fileOut -d dbfile\n"
    << "  -i fileIn   File containing list of locations: 'lon lat elev'.\n"
    << "  -o fileOut  Output file with locations and material properties.\n"
    << "  -d dbfile   Etree database file to query.\n"
    << "  -h          Display usage and exit.\n";
} // usage

// ----------------------------------------------------------------------
void
parseArgs(std::string* pFilenameIn,
	  std::string* pFilenameOut,
	  std::string* pFilenameDB,
	  int argc,
	  char** argv)
{ // parseArgs
  assert(0 != pFilenameIn);
  assert(0 != pFilenameOut);
  assert(0 != pFilenameDB);

  extern char* optarg;

  int nparsed = 1;
  *pFilenameIn = "";
  *pFilenameOut = "";
  *pFilenameDB = "";
  int c = EOF;
  while ( (c = getopt(argc, argv, "hi:o:d:") ) != EOF) {
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
int
main(int argc,
     char* argv[])
{ // main
  std::string filenameIn = "";
  std::string filenameOut = "";
  std::string filenameDB = "";
  
  parseArgs(&filenameIn, &filenameOut, &filenameDB, argc, argv);

  try {
    cencalvm::query::VMQuery query;
    query.filename(filenameDB.c_str());

#if !defined(ALLVALS)
    const int numVals = 3;
    const char* pValNames = { "FaultBlock", "Zone" };
    query.queryVals(pValNames, numVals);
#else
    const int numVals = 8;
#endif

    query.open();

    std::ifstream fileIn(filenameIn.c_str());
    if (!fileIn.is_open()) {
      std::ostringstream msg;
      msg << "Could not open file '" << filenameIn
	  << "' to read query locations.";
      throw std::runtime_error(msg.str());
    } // if
    
    std::ofstream fileOut(filenameOut.c_str());
    if (!fileOut.is_open()) {
      std::ostringstream msg;
      msg << "Could not open file '" << filenameOut
	  << "' to receive query data.";
      throw std::runtime_error(msg.str());
    } // if

    double* pVals = (numVals > 0) ? new double[numVals] : 0;
    double lon = 0.0;
    double lat = 0.0;
    double elev = 0.0;
    fileIn >> lon >> lat >> elev;
    while (!fileIn.eof() && fileIn.good() && fileOut.good()) {
      query.query(&pVals, numVals, lon, lat, elev);
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
	<< std::setw(7) << pVals[3]
	<< std::setw(7) << pVals[4]
	<< std::setw(9) << pVals[5]
	<< std::setw(4) << int(pVals[6])
	<< std::setw(4) << int(pVals[7])
	<< "\n";
#endif
      
      fileIn >> lon >> lat >> elev;
    } // while

    query.close();
    fileIn.close();
    fileOut.close();
  } // try
  catch (const std::exception& err) {
    std::cerr << "Error occurred while querying etree database.\n"
	      << "Error message:\n"
	      << err.what()
	      << std::endl;
    return -1;
  } // catch
  catch (...) {
    std::cerr << "Unknown error occurred while querying etree database."
	      << std::endl;
    return -1;
  } // catch

  return 0;
} // main

// version
// $Id$

// End of file 
