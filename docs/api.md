# cencalvm API

The best way to understand the API is to browse the source code. The
header files are all documented using Doxygen conventions.

## Directory structure

### Query library `libsrc/cencalvm`

* **average**

   The average subpackage is used to populate the interior octants of
   the etree with averages of the children. In general, it is for
   internal use only.

* **create**

  The create subpackage is used to create the Etree database from
  EarthVision files. In general, it is for internal use only.

* **query**

  The query subpackage is used to query the etree database holding the
  velocity model. See [Interface for querying the USGS central CA
  velocity model](query.md) for more information.

* **storage**

  The storage subpackage defines the geometry of the velocity model
  and performs transformation of geographic coordinates to the Etree
  ticks. This package is used by the other packages. You should never
  need to call the routines in this subpackage directly.

### Unit testing `tests`

### Applications `applications`

## Query API

### Setting the query type

When the query type is set to `FIXEDRES`, values at the requested
resolution will be used even if the database contains values at a
higher resolution. The requested resolution is set with
`cencalvm::query::VMQuery::queryRes()` in C++, `cencalvm_queryRes()`
in C, and `cencalvm_queryres_f()` in Fortran.

When the query type is set to `WAVERES`, the requested resolution is
the wavelength for shear waves at a given minimum period. The
requested resolution is set by passing the minimum period as an
argument to `cencalvm::query::VMQuery::queryRes()` in C++,
`cencalvm_queryRes()` in C, and `cencalvm_queryres_f()` in Fortran.

#### Squashing topography/bathymetry

Use `cencalvm::query:::VMQuery::squash()` to turn on squashing of
topography. Squashing, by default, is limited to elevations above -2
km. That is, the geometry of the model above an elevation of -2 km (2
km below sea level) is moved up/down so that that ground surface is at
sea level. Below an elevation of -2 km, the geometry of the seismic
velocity model is retained. The elevation for this extent of squashing
can be set using the second argument in
`cencalvm::query::VMQuery::squash()`.

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
