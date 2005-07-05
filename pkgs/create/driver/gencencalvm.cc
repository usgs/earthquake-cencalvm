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

// Application driver to generate etree database of central CA
// velocity model.

#include "cencalvm/create/VMCreator.h" // USES VMCreator

#include <stdlib.h> // USES exit()
#include <unistd.h> // USES getopt()

#include <iostream> // USES std::cerr
#include <stdexcept> // USES std::exception

// ----------------------------------------------------------------------
void
usage(void)
{ // usage
  std::cerr
    << "usage: gencencalvm [-h] -i paramFile -o outFile -t tmpFile\n"
    << "  -i paramFile  Parameter file with list of grid input files\n"
    << "  -o outFile    Etree database file created.\n"
    << "  -t tmpFile    Name of scratch file used in database construction.\n"
    << "  -h            Display usage and exit.\n"
    << "\n"
    << "Parameter file is list of grid input files, one per line.\n";
  exit(1);
} // usage

// ----------------------------------------------------------------------
void
parseArgs(std::string* pFilenameParams,
	  std::string* pFilenameOut,
	  std::string* pFilenameTmp,
	  int argc,
	  char** argv)
{ // parseArgs
  assert(0 != pFilenameParams);
  assert(0 != pFilenameOut);
  assert(0 != pFilenameTmp);

  extern char* optarg;

  int nparsed = 1;
  *pFilenameParams = "";
  *pFilenameOut = "";
  *pFilenameTmp = "";
  int c = EOF;
  while ( (c = getopt(argc, argv, "hi:o:t:") ) != EOF) {
    switch (c)
      { // switch
	case 'i' : // process -i option
	  *pFilenameParams = optarg;
	  nparsed += 2;
	  break;
	case 'o' : // process -o option
	  *pFilenameOut = optarg;
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
	default :
	  usage();
	} // switch
    } // while
  if (nparsed != argc || 
      0 == pFilenameParams->length() ||
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
  std::string filenameOut = "";
  std::string filenameTmp = "";
  
  parseArgs(&filenameParams, &filenameOut, &filenameTmp, argc, argv);

  try {
    cencalvm::create::VMCreator creator;
    creator.filenameParams(filenameParams.c_str());
    creator.filenameOut(filenameOut.c_str());
    creator.filenameTmp(filenameTmp.c_str());
    creator.run();
  } // try
  catch (const std::exception& err) {
    std::cerr << "Error occurred while generating etree database.\n"
	      << "Error message:\n"
	      << err.what()
	      << std::endl;
    return -1;
  } // catch
  catch (...) {
    std::cerr << "Unknown error occurred while generating etree database."
	      << std::endl;
    return -1;
  } // catch

  return 0;
} // main

// version
// $Id$

// End of file 
