c -*- Fortran -*-
c
c ======================================================================
c
c                           Brad T. Aagaard
c                        U.S. Geological Survey
c
c {LicenseText}
c
c ======================================================================
c

c Application demonstrating how to do queries from C++ code.
c

c Switch used to select whether all values or selected ones are queried.

c 
	program cencalvmf77query
	implicit none

	character*128 filenameIn
	parameter(filenameIn='../data/sample-05.0.0.in')
	integer unitIn
	parameter(unitIn=10)

        character*64 filenameOut
	parameter(filenameOut='test.out')
	integer unitOut
	parameter(unitOut=11)

        character*64 filenameDB
	parameter(filenameDB='data/ver-05.0.0/USGSBayAreaVM-05.0.0.etree')

        character*64 filenameLog
	parameter(filenameLog='test.log')

	character*256 errorMsg

	integer numVals
	parameter(numVals=8)
	real*8 vals(numVals)

	real*8 lon, lat, elev

	integer ok

c       ****************************************************************
c       Size of query and errHandler MUST match sizeof(void*) in C
c       ****************************************************************
	integer*8 query
	integer*8 errHandler

	ok = 0

c       Create query
	call cencalvm_createquery_f(query)
	if (query.eq.0) goto 999

c       Get handle to error handler
	call cencalvm_errorhandler_f(query, errHandler)
	if (errHandler.eq.0) goto 999

c       Set log file in error handler
	call cencalvm_error_logfilename_f(errHandler, filenameLog, ok)
	if (ok.ne.0) goto 999

c       Set database filename
	call cencalvm_filename_f(query, filenameDB, ok)
	if (ok.ne.0) goto 998

c       Open database for querying
	call cencalvm_open_f(query, ok)
	if(ok.ne.0) goto 998

c       Open input file to read locations
	open(unitIn,file=filenameIn,status='old')

c       Open output file to accept data
	open(unitOut,file=filenameOut,status='new')

c       Read location from input file
 10	read(unitIn,*,end=30) lon,lat,elev

c       Query database
	call cencalvm_query_f(query,vals,numVals,lon,lat,elev,ok)

	if(ok.ne.0) then
	   call cencalvm_error_message_f(errHandler, errorMsg, ok)
	   write(6,*) errorMsg
c          If query generated an error, then bail out, otherwise reset status
	   if (ok.eq.2) goto 999
	   call cencalvm_error_resetstatus_f(errHandler, ok)
	   if (ok.ne.0) goto 998
	endif

c       Write values returned by query to output file
	write(unitOut,20) lon,lat,elev,vals(1),vals(2),vals(3),
       1 vals(4),vals(5),vals(6),int(vals(7)),int(vals(8))
 20	format(f9.4,f8.4,f9.1,f8.1,f8.1,f8.1,f7.1,f7.1,f9.1,i4,i4)

	goto 10

c       Close database
 30	call cencalvm_close_f(query, ok)
	if(ok.ne.0) goto 998

c       Close input and output files
	close(unitIn)
	close(unitOut)

c       Destroy query handle
	call cencalvm_destroyquery_f(query, ok)
	if(ok.ne.0) goto 999

	goto 100
 666	write(6,*) 'Error reading input file.'
	goto 999
 777	write(6,*) 'Error opening input file.'
	goto 999
 888	write(6,*) 'Error opening output file.'
	goto 999
 998	call cencalvm_error_message_f(errHandler, errorMsg, ok)
	write(6,*) errorMsg
	goto 999
 999	write(6,*) 'ERROR'
 100	return
	end

c version
c $Id$

c End of file
