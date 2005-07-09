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

/* Application demonstrating how to do queries from C++ code.
 */

/* Switch used to select whether all values or selected ones are queried.
 */
#define ALLVALS


#include "cencalvm/query/cvmquery.h"

#include <stdlib.h> /* USES exit() */
#include <unistd.h> /* USES getopt() */
#include <stdio.h> /* USES fopen(), fclose(), fprintf() */
#include <string.h> /* USES strcpy() */

#include <assert.h> /* USES assert() */

/* ------------------------------------------------------------------- */
void
usage(void)
{ /* usage */
  fprintf(stderr,
	  "usage: cencalvmcppquery [-h] -i fileIn -o fileOut -d dbfile\n"
	  "  -i fileIn   File containing list of locations: 'lon lat elev'.\n"
	  "  -o fileOut  Output file with locations and material properties.\n"
	  "  -d dbfile   Etree database file to query.\n"
	  "  -h          Display usage and exit.\n");
} /* usage */

/* ------------------------------------------------------------------- */
void
parseArgs(char* filenameIn,
	  char* filenameOut,
	  char* filenameDB,
	  int argc,
	  char** argv)
{ // parseArgs
  extern char* optarg;

  int nparsed = 1;
  int c = EOF;
  while ( (c = getopt(argc, argv, "hi:o:d:") ) != EOF) {
    switch (c)
      { /* switch */
      case 'i' : /* process -i option */
	strcpy(filenameIn, optarg);
	nparsed += 2;
	break;
      case 'o' : /* process -o option */
	strcpy(filenameOut, optarg);
	nparsed += 2;
	break;
      case 'd' : /* process -d option */
	strcpy(filenameDB, optarg);
	nparsed += 2;
	break;
      case 'h' : /* process -h option */
	nparsed += 1;
	usage();
	exit(0);
	break;
      default :
	usage();
      } /* switch */
  } /* while */
  if (nparsed != argc || 
      0 == strlen(filenameIn) ||
      0 == strlen(filenameOut) ||
      0 == strlen(filenameDB)) {
    usage();
    exit(1);
  } /* if */
} /* parseArgs */

/* ------------------------------------------------------------------- */
int
main(int argc,
     char* argv[])
{ /* main */
  char filenameIn[256];
  char filenameOut[256];
  char filenameDB[256];
  
  parseArgs(filenameIn, filenameOut, filenameDB, argc, argv);

  void* query = cencalvm_createQuery();
  if (0 == query)
    return 1;

  if (0 != cencalvm_filename(query, filenameDB))
    return 1;

#if !defined(ALLVALS)
  int numVals = 3;
  char* pValNames = { "FaultBlock", "Zone" };
  if (0 != cencalvm_queryVals(query, pValNames, numVals))
    return 1;
#else
  int numVals = 8;
#endif

  if (0 != cencalvm_open(query))
    return 1;
  
  FILE* fileIn = fopen(filenameIn, "r");
  if (0 == fileIn) {
    fprintf(stderr, "Could not open file '%s' to read query locations.\n",
	    filenameIn);
    return 1;
  } /* if */
  
  FILE* fileOut = fopen(filenameOut, "w");
  if (0 == fileOut) {
    fprintf(stderr, "Could not open file '%s' to write query data.\n",
	    filenameIn);
    return 1;
  } /* if */

  double* pVals = (double*) malloc(sizeof(double)*numVals);
  double lon = 0.0;
  double lat = 0.0;
  double elev = 0.0;
  fscanf(fileIn, "%lf %lf %lf", &lon, &lat, &elev);
  while (!feof(fileIn)) {
    if (0 != cencalvm_query(query, &pVals, numVals, lon, lat, elev))
      return 1;

    fprintf(fileOut, "%9.4f%8.4f%9.1f", lon, lat, elev);
#if !defined(ALLVALS)
    fprintf(fileOut, "%4d%4d\n", int(pVals[0]), int(pVals[1]));
#else
    fprintf(fileOut, "%8.1f%8.1f%8.1f%7.1f%7.1f%9.1f%4d%4d\n",
	    pVals[0], pVals[1], pVals[2], pVals[3], pVals[4], pVals[5],
	    (int) pVals[6], (int) pVals[7]);
#endif
      
    fscanf(fileIn, "%lf %lf %lf", &lon, &lat, &elev);
  } /* while */

  if (0 != cencalvm_close(query))
    return 1;

  if (0 != cencalvm_destroyQuery(query))
    return 1;

  fclose(fileIn);
  fclose(fileOut);

  return 0;
} // main

// version
// $Id$

// End of file 
