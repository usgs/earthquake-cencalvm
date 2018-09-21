# Interface for querying USGS central CA velocity model

This software was designed with the assumption that users will want
to query the velocity model from their own code. As a result, an
API is provided for the query routines. Examples of how to use the
query routines from C++, C, and Fortran are provided in
examples/query with sample input and output in
examples/query/data. Any of these examples could be used to get
material property values for locations, but performance will be
improved if you use the API because the data won't have to be
written to and then read from a file.


## Database

By default the query software will return all values stored in the
Etree database in the following order:

* longitude (WGS84)

* latitude (WGS84)

* elevation (m)

* Vp (m/s)

* Vs (m/s)

* density (kg/m**3)

* Qp

* Qs

* Depth from free surface (m) of centroid of database cell

* Fault block id

* Zone id

* Elevation (m) of ground surface at longitude/latitude of centroid of
  database cell

Notes:

1. Values are not interpolated from the Etree database. That is,
queries for any point within a grid cell in the database will return
the values for the grid cell that correspond to its centroid.

2. The elevation for a given longitude/latitude can differ between
database cells at different resolutions because the centroid will
shift horizontally as the resolution changes.

## Units

SI units are for all values in the database and its interface.

## Enabling use of the extended (regional) model

Use of the extended model is disabled by default. To enable use of the
extended model in queries simply set the filename of the extended
model using `cencalvm::query::VMQuery::filenameExt()` in C++,
`cencalvm_filenameExt()` in C, or `cencalvm_filenameext_f()` in
Fortran. You can also change the size of the cache used in queries of
the extended model using `cencalvm::query::VMQuery::cacheSizeExt()` in
C++, `cencalvm_cacheSizeExt()` in C, or `cencalvm_cachesizeext_f()` in
Fortran.

## Applications

### cencalvminfo

This application dumps the metadata, database schema, and number of
octants in an etree database to stdout.

### cencalvmquery

This application uses a list of points as input, queries the database
at each point, and writes out the database values to a file.

```
usage: cencalvmquery [-h] -i fileIn -o fileOut -d dbfile
       [-l logfile] [-t queryType] [-r res] [-e dbextfile]
       [-c cacheSize] [-s squashLimit]

  -h            Display usage and exit.
  -i fileIn     File containing list of locations: 'lon lat elev'.
  -o fileOut    Output file with locations and material properties.
  -d dbfile     Etree database file to query.
  -l logfile    Log file for warnings about no data for locations.
  -t queryType  Type of query {'maxres', 'fixedres', 'waveres'}
  -r res        Resolution for query (not needed for maxres queries)
  -e dbextfile  Etree extended database file to query.
  -c cacheSize  Size of cache in MB to use in query
  -s squashLim  Turn on squashing of topography and set limit
```
Arguments in square brackets are optional.

#### Input file

Columns are:

```
0: longitude (WGS84)
1: latitude (WGS84)
2: elevation (m) with respect to mean sea level
```

Example input file

```
-122.7655 38.9735   1187.5
-122.3684 38.6190     12.5
-122.8102 38.5669     37.5
-123.3883 37.9286  -2475.0
```

#### Output file

Each line of the output file will have the following values:
```
 0: longitude (WGS84)
 1: latitude (WGS84)
 2: elevation (m)
 3: Vp (m/s)
 4: Vs (m/s)
 5: density (kg/m**3)
 6: Qp
 7: Qs
 8: Depth from free surface (m) of centroid of database cell
 9: Fault block id
10: Zone id
11: Elevation (m) of ground surface at longitude/latitude of centroid of
    database cell
```

Example output file

```
-122.76550 38.97350   1187.5  1880.0   530.0  2040.0     67.0     34.0     67.9    9   30   1255.4
-122.36840 38.61900     12.5  2950.0  1360.0  2660.0    200.0    100.0    208.0    9   11    220.5
-122.81020 38.56690     37.5  1220.0   120.0  1830.0     26.0     13.0     15.3   13   31     52.8
-123.38830 37.92860  -2475.0  5560.0  3330.0  2670.0    709.0    355.0   2352.0   25   20   -123.0
```


## Querying routines

Documentation for the query routines for each language are contained
in the header files. Pay special attention to the default query
parameters.

### Query types

The etree database is a fully populated etree, meaning data is
stored at the leaves of the tree as well as in the interior. The
different levels in the etree correspond to different spatial
resolutions. The lower an octant is in the etree, the smaller the
spatial volume associated with the octant.  In other words, values
at the different levels of the etree correspond to material
properties at different spatial resolutions, for example the P wave
speed on a scale of 25 meters versus the P wave speed on a scale of
400 meters. The values at the interior octants at each level are
the arithmetic means of the values of its children. The queries for
the elevation of topography and bathymetry are computed indirectly
using the the depth from the free surface and the coordinates of
the octant returned in a search. Thus, the resolution depends on
the depth of the point used in the query and the type of query
(`MAXRES`, `FIXEDRES`, or `WAVERES`).

Because averaging fault block and zone values is not meaningful, the
fault block and zone values are set to -999 in the interior octants.

In the database, the value for Vs in water is -999. In order to
prevent nonphysical values for solid material near the solid/water
interface, mixing of solid and water material properties is not
permitted and solid material properties take precedence. In other
words, if an interior octant has both children with water properties
and children wich solid material properties, the solid material
properties will be averaged and the water properties will be ignored.

### Setting the query type

The query routine assigns values from the Etree database for a given
location depending on the query type.

Resolution in the discussion that follows refers to the vertical
resolution which is finer than the horizontal resolution.

#### MAXRES query

When the query type is set to `MAXRES`, then the query routines will
use values at the maximum resolution available. The query values will
be set to -999.0 if the etree was not populated with data at that
location even if averaged values are available; this happens when one
queries at locations above topography or the water surface. If you
want averaged values, use one of the other query types.

#### FIXEDRES query

When the query type is set to `FIXEDRES`, values at the requested
resolution will be used even if the database contains values at a
higher resolution. This prevents aliasing the model for queries at a
uniform spatial resolution. The requested resolution is set with
`cencalvm::query::VMQuery::queryRes()` in C++, `cencalvm_queryRes()` in C,
and `cencalvm_queryres_f()` in Fortran.

#### WAVERES query

When the query type is set to `WAVERES`, the requested resolution is
the wavelength for shear waves at a given minimum period. The query
routine first finds the maximum resolution available and traverses up
the tree until the resolution is no greater than the requested
resolution. This prevents aliasing the model for queries at variable
spatial resolution but uniform resolution with respect to the
wavelength of shear waves at a given period. The requested resolution
is set by passing the minimum period as an argument to
`cencalvm::query::VMQuery::queryRes()` in C++, `cencalvm_queryRes()`
in C, and `cencalvm_queryres_f()` in Fortran.

#### Squashing topography/bathymetry

There are two settings that permit the velocity structure to be
adjusted so that the top surface is aligned with sea level. The
default query behavior does NOT use squashing. Use
`cencalvm::query:::VMQuery::squash()` to turn on squashing of
topography. Squashing, by default, is limited to elevations above -2
km. That is, the geometry of the model above an elevation of -2 km (2
km below sea level) is moved up/down so that that ground surface is at
sea level. Below an elevation of -2 km, the geometry of the seismic
velocity model is retained. The elevation for this extent of squashing
can be set using the second argument in
`cencalvm::query::VMQuery::squash()`.

The elevation of the ground surface used in squashing topography is
found by performaing a `MAXRES` query for the elevation of topography at
the location of the velocity model query. Thus, the resolution of the
elevation used to squash topography is coarser for points deep in the
model (provided the location lies above the depth extent of
squashing).

## Fortran 77 notes

### `cencalvm_createquery_f()`

The argument to `cencalvm_createquery_f()` is used to store the
address of the handle to the underlying C++ object. It is
ABSOLUTELY CRITICAL that the number of bytes associated with this
variable match the size of a C/C++ pointer on the system where the
routines are compiled. On 32-bit and 64-bit systems, the variables
will generally be a 4 byte integer (integer*4) and an 8 byte
integer (integer*8), respectively.

### No Fortran 77 binding for `cencalvm_queryVals()`

Because arrays of strings cannot be passed from Fortran to C++ in a
standard manner, there is no Fortran 77 binding for
`cencalvm_queryVals()`. Instead, you must accept the default behavior
in which all values are returned.
