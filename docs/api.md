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
