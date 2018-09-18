# CHANGES

List of changes to query software for each release.

## Version 1.0.0, 2018-09-18

* Migrated from private subversion repository to GitHub.

* Fortran bindings are now optional and turned off by
  default. Configure with `--enable-fortran` to build the Fortran
  bindings.


## Version 0.9.0, 2014/08/28

* Improved code layout. Updates for automake 1.14.1.


## Version 0.8.1, 2011/09/28

* Switched to using Fortran 90 compiler to build Fortran code to
 insure name mangling macros match compiler.

 
## Version 0.8.0, 2011/05/26

* Updated documentation with availability of elevation query value.


## Version 0.7.0, 2010/11/18

* Switched from Pyrex/Pyrexembed to SWIG for extension (use as a
spatialdatabase).


## Version 0.6.6, 2010/01/18

* Added missing include directives to fix compilation errors with
recent gcc versions.


## Version 0.6.5, 2008/05/14

* Improved Makefiles so that files are not removed by 'make clean'
when the building in the source tree. When building outside the source
tree, data files copied from the source tree are removed from the
build tree.

* Switched from linking Fortran example query code with C++ compiler
to linking with Fortran compiler. This appears to fix build problems
with the Intel compiler, but may required adding C++ libraries to the
link line for the Sun compiler (by passing arguments to configure).


## Version 0.6.4, 2008/04/10

* Fixed bug for finding elevation in extended model (cannot allow use
of interior octants). Bug affected squashing as well as queries for
elevation of ground surface.


## Version 0.6.3, 2008/04/02

*  Fixed bug for finding elevation in extended model (found flag
always false). Bug affected squashing as well as queries for
elevation of ground surface. Added sample data files for USGS Bay
Area Velocity Model 08.2.0.


## Version 0.6.2, 2008/04/01

* Fixed bugs related to setting squashing flag in C and C++ examples
(it was always turned on). Fixed bug in determining elevation using
depth from free surface (limited to querying for elevation; squashing
was not affected).


## Version 0.6.1, 2008/03/31

* Added C and Fortran bindings to squash().


## Version 0.6.0, 2008/02/06

* Added ability to query for elevation of topography/bathymetry and
squash topography/bathymetry to retain low-velocity material when
using the velocity model in models with a flat ground surface. Added
extension to the query routines for Python to allow seamless
integration into PyLith via CIG's spatialdata software. These new
features can be used with any release of the velocity model.


## Version 0.5.0, 2007/02/21

* Simplified code layout. Condensed multiple libraries into one main
library. No changes were made to the API, so the only changes users
will need to make is to adjust their linking to simply use
"-lcenvalvm".


## Version 0.4.1, 2006/03/21

* Restructured geometry routines in order to generalize geometry
interface. Etree databases with different geometries (different
locations) can now use the same query routines. The default geometry
is the one for central CA; this can be changed via
VMQuery::geometry().

* Improved compliance with C++ standards which eliminated compiler
warnings for some compilers.

* Miscellaneous minor improvements in documentation.


## Version 0.4.0, 2005/11/05

* Restructured query routines, adding the ability to query the
regional model, if desired.

* Updated sample input and output data files to be compatible with
release 05.1.0 of the USGS Bay Area Velocity Model, including the
abilty to use the regional model, if desired.

* Created extension of VMQuery to be compatible with CIG spatialdata
package. This will only be of use in CIG code. Building this extension
is turned off by default.

* Fixed bug in query routines where projection routine was called
twice. Second, superfluous call was removed. This should speed up
queries a very slight amount.


## Version 0.3.0, 2005/10/07

* Changed examples to include FIXEDRES and WAVERES query
types. Improved instructions for the examples.

* Turned on averaging of water points but do not allow mixing of
averaging of water and solid material points in averaging by giving
solid material points precedence (if attempting to mix the two, ignore
the water values).

* Fixed FIXEDRES and WAVERES queries so averaged values aren't used
when query wants finer resolution than available. For example, if you
want values at 200m resolution and a search says only values at 800m
are available, then this location is a "void" at 200m resolution so
"NODATA" should be returned instead of the values at 800m. Previous
versions would return values at the 800m resolution.

* Switched to GNU build system for easier installtion and
portability. Users now run configure to generate Makefiles instead of
creating them by hand.


## Version 0.2.2, 2005/08/10

* Fixed bug where inclusion of assert header file was missing in
several source files. This resulted in compiler errors in some cases.


## Version 0.2.1, 2005/08/03

* Fixed bug in creation routine where Vs for water (-999) was
scaled. 'No data' values should not be scaled. The query routine was
adjusted to undo this scaling, so 'no data' values will now be
consistent.

* Fixed bug in MAXRES query routine. Interior octants were returned in
search of etree. Now if an interior octant is returned, the values are
set to 'no data'. This only occurred when querying the etree at
locations above topography and the water surface.

* Added 'no data' and water points to examples for illustration of
those cases.

* Added ability to make creation/averaging routines run quietly (no
progress reports).

* Fixed bug in cencalvmcquery where filenames were not initialized to
empty string. This created filenames from garbage and log files tended
to created by default which is not the desired behavior.


## Version 0.2.0, 2005/07/29

* Added error handler to allow queries to continue even when values
cannnot be found for locations. Coordinates of locations where data
was not found can be written to a log file. Values for locations not
found are set to predefined value (see \link libsrc/query/README
libsrc/query/README \endlink for details).

* Fixed bugs in C and C++ examples associated with setting values to
be returned in a query.

* Fixed documentation for units of cache size. Units are MB.

* Added average package for creating database with spatial averaging.

* Finished implementing routines and unit testing of queries at fixed
resolution or resolution based on wavelength of shear wave.


## Version 0.1.1, 2005/07/18

* Fixed errors in documentation and updated Make.mm files in examples
directory to reflect name of database in release pre1.0.


## Version 0.1.0, 2005/07/09 

original version
