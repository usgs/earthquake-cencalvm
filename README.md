# cencalvm

[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/baagaard-usgs/cencalvm/blob/master/LICENSE)

## Description

This software package is used to create and query the seismic velocity
model(s) covering the greater San Francisco Bay area (central
California) developed by the United States Geological Survey.

Updates to the data in the velocity model (the etree database file)
are generally independent of updates to the querying software. This
means the version numbers are also independent of one another. Unless
the velocity model undergoes fundamental changes in the way the data
is stored, you can safely use updated versions of the etree database
without recompiling the query software, and vice versa.

## Installation instructions

See [INSTALL.md](INSTALL.md).
		
## Documentation

[Online Documentation](https://baagaard-usgs.github.io/cencalvm)

## Release Notes

See [CHANGES](CHANGES.md) for a complete list of changes for each
release.

## Author

* Brad Aagard, Earthquake Science Center, USGS, USA



 *
 * @section dirs Directories
 *
 * <ul>
 *
 * <li> \b doc Directory where the documentation for the query API
 * resides. The documentation is available online at
 * http://www.sf06simulation.org/geology/velocitymodel/querydoc/. You
 * can generate the documentation if you have doxygen installed by
 * running 'doxygen doxyconfig' in the doc directory. The routines
 * associated with the user interface are labeled as such in the html
 * documentation.
 *
 * </li>
 *
 * <li> \b libsrc Top-level directory for source files comprising
 * cencalvm library.
 *
 * </li>
 *
 * <ul>
 *
 * <li> \b average
 *
 * The average subpackage is used to populate the interior octants of
 * the etree with averages of the children. In general, it is for
 * internal use only.
 *
 * </li>
 *
 * <li> \b create
 *
 * The create subpackage is used to create the Etree database from
 * Earth Vision files. In general, it is for internal use only.
 * 
 * </li>
 *
 * <li> \b query
 *
 * The query subpackage is used to query the etree database holding
 * the velocity model. See \link libsrc/query/README
 * libsrc/query/README \endlink for additional information on the
 * query routines.
 *
 * </li>
 *
 * <li> \b storage
 *
 * The storage subpackage defines the geometry of the velocity model
 * and performs transformation of geographic coordinates to the Etree
 * ticks. This package is used by the other packages. You should never
 * need to call the routines in this subpackage directly.
 *
 * </li>
 *
 * </ul>
 *
 * <li> \b tests Unit testing of all routines.</li>
 *
 * <li> \b applications Source directory for applications (executables).</li>
 *
 * <li> \b examples Source directory for query examples.</li>
 *
 * </ul>
