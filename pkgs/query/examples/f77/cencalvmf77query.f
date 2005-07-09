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
	parameter(filenameIn='../data/sample.in')
	integer unitIn
	parameter(unitIn=10)

        character*128 filenameOut
	parameter(filenameOut='test.out')
	integer unitOut
	parameter(unitOut=11)

        character*128 filenameDB
	parameter(filenameDB='data/800x800x200.etree')

	integer numVals
	parameter(numVals=8)
	real*8 vals(numVals)

	real*8 lon, lat, elev

	integer ok

c       ****************************************************************
c       Size of query MUST match sizeof(void*) in C
c       ****************************************************************
	integer*8 query

	ok = 1

	call cencalvm_createquery_f(query)
	if (query.eq.0) goto 999

	call cencalvm_filename_f(query, filenameDB, ok)
	if (ok.eq.0) goto 999

	call cencalvm_open_f(query, ok)
	if(ok.eq.0) goto 999

	open(unitIn,file=filenameIn,status='old',err=30)
	open(unitOut,file=filenameOut,status='new')

 10	read(unitIn,*,err=30) lon,lat,elev

	call cencalvm_query_f(query,vals,numVals,lon,lat,elev,ok)
	if(ok.eq.0) goto 999

	write(unitOut,20) lon,lat,elev,vals(1),vals(2),vals(3),
       1 vals(4),vals(5),vals(6),int(vals(7)),int(vals(8))
 20	format(f9.4,f8.4,f9.1,f8.1,f8.1,f8.1,f7.1,f7.1,f9.1,i4,i4)

	goto 10

 30	call cencalvm_close_f(query, ok)
	if(ok.eq.0) goto 999
	call cencalvm_destroyquery_f(query, ok)
	if(ok.eq.0) goto 999

	close(unitIn)
	close(unitOut)
	goto 100
 666	write(6,*) 'Error reading input file.'
	goto 999
 777	write(6,*) 'Error opening input file.'
	goto 999
 888	write(6,*) 'Error opening output file.'
 999	write(6,*) 'ERROR'
 100	return
	end

c version
c $Id$

c End of file
