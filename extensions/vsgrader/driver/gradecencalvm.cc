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

// Application driver to limit gradient in Vs in etree database of central CA
// velocity model.

#include "cencalvm/vsgrader/VsGrader.h" // USES VMCreator
#include "cencalvm/storage/ErrorHandler.h" // USES VMCreator

#include <stdlib.h> // USES exit()
#include <unistd.h> // USES getopt()

#include <iostream> // USES std::cerr
#include <stdexcept> // USES std::exception
#include <assert.h> // USES assert()

// ----------------------------------------------------------------------
void
usage(void)
{ // usage
  std::cerr
    << "usage: gradecencalvm [-h] -p paramFile -i inFile -o outFile -t tmpFile [-l logFile]\n"
    << "  -p paramFile  Parameter file with list of grid input files\n"
    << "  -i inFile     Input Etree database file.\n"
    << "  -o outFile    Output Etree database file.\n"
    << "  -t tmpFile    Name of scratch file used in database construction.\n"
    << "  -h            Display usage and exit.\n"
    << "  -l logFile    Log file for warnings about data.\n"
    << "\n"
    << "Parameter file is list of grid input files, one per line.\n";
  exit(1);
} // usage

// ----------------------------------------------------------------------
void
parseArgs(std::string* pFilenameParams,
	  std::string* pFilenameIn,
	  std::string* pFilenameOut,
	  std::string* pFilenameTmp,
	  std::string* pFilenameLog,
	  int argc,
	  char** argv)
{ // parseArgs
  assert(0 != pFilenameParams);
  assert(0 != pFilenameIn);
  assert(0 != pFilenameOut);
  assert(0 != pFilenameTmp);
  assert(0 != pFilenameLog);

  extern char* optarg;

  int nparsed = 1;
  *pFilenameParams = "";
  *pFilenameIn = "";
  *pFilenameOut = "";
  *pFilenameTmp = "";
  *pFilenameLog = "";
  int c = EOF;
  while ( (c = getopt(argc, argv, "hi:l:o:p:t:") ) != EOF) {
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
	case 'p' : // process -p option
	  *pFilenameParams = optarg;
	  nparsed += 2;
	  break;
	case 't' : // process -t option
	  *pFilenameTmp = optarg;
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
      0 == pFilenameParams->length() ||
      0 == pFilenameIn->length() ||
      0 == pFilenameOut->length() ||
      0 == pFilenameTmp->length())
    usage();
} // parseArgs

// ----------------------------------------------------------------------
int
main(int argc,
     char* argv[])
{ // main
  std::string filenameParams = "";
  std::string filenameIn = "";
  std::string filenameOut = "";
  std::string filenameTmp = "";
  std::string filenameLog = "";
  
  parseArgs(&filenameParams, &filenameIn, &filenameOut, &filenameTmp,
	    &filenameLog, argc, argv);

  cencalvm::vsgrader::VsGrader grader;
  cencalvm::storage::ErrorHandler* pHandler = grader.errorHandler();

  if (filenameLog.length() > 0)
    pHandler->logFilename(filenameLog.c_str());

  grader.filenameParams(filenameParams.c_str());
  grader.filenameIn(filenameIn.c_str());
  grader.filenameOut(filenameOut.c_str());
  grader.filenameTmp(filenameTmp.c_str());
  grader.run();

  if (cencalvm::storage::ErrorHandler::OK != pHandler->status()) {
    std::cerr
      << "Error: "
      << pHandler->message() << std::endl;
    return 1;
  } // if

  return 0;
} // main

// version
// $Id$

// End of file 
