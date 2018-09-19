# Installation instructions

See [Online Documentation](https://baagaard-usgs.github.io/cencalvm) for a description
of the velocity model querying software and how to use it.

## Software Prerequisites

### gzip software

Version 1.3.3 is known to work

Gzip is required to uncompress the database file and the software
tar ball. See [Known Issues] for more information.

### Euclid - Etree database library

[http://www.cs.cmu.edu/~euclid/euclid3-latest.tar.gz](http://www.cs.cmu.edu/~euclid/euclid3-latest.tar.gz)

1. Download the euclid source code from
[http://www.cs.cmu.edu/~euclid](http://www.cs.cmu.edu/~euclid) or use
the link above.

2. Edit the Makefile.

  Go to the libsrc directory. Edit the Makefile to suit your
  system. Some example Makefiles can be found on the [USGS EHZ ftp
  server](ftp://ehzftp.wr.usgs.gov/baagaard/cencalvm/euclid3/Makefiles). These
  examples assume the environment variable `ETREE_LIBDIR` has been set
  (see below).

  Note: The `-DALIGNMENT` flag appears to be required on all big
  endian systems. The code will compile without it, but you will get
  core dumps, segmentation faults, and/or bus errors if you don't use
  it.

3. Build the library.

  Type `make` and copy the library and all the header files to your
  favorite local install directory unless they are put there in the
  Makefile.

4. Set environment variables.

If the libraries and header files are not installed in system
directories, set the environment variables `ETREE_LIBDIR`,
`ETREE_INCDIR`, and `LD_LIBRARY_PATH` (or the equivalent) accordingly
(you probably want to do this in your `.bashrc` or `.cshrc` file or a
file that is included by them so that these are set automatically
every time you log in). If `libetree.so` is in `/home/tools/etree/lib`
and header files are in `/home/tools/etree/include`, then:

  * bash shell users

```
export ETREE_LIBDIR=/home/tools/etree/libsrc
export ETREE_INCDIR=/home/tools/etree/libsrc
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${ETREE_LIBDIR}
```

  * tcsh shell users

```
setenv ETREE_LIBDIR /home/tools/etree/libsrc
setenv ETREE_INCDIR /home/tools/etree/libsrc
setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${ETREE_LIBDIR}
```

### Proj.4 - Cartographic projections library

[http://trac.osgeo.org/proj](http://trac.osgeo.org/proj)
Versions 4.4.9 and later are known to work

1. Download the Proj.4 source code from [http://trac.osgeo.org/proj](http://trac.osgeo.org/proj)

2. Run configure.  Helpful arguments include `--prefix=INSTALL_DIR`.

3. Run `make` and then `make install`.

Although not required, it is useful to download the NAD datum
corrections, so that you can use proj4 to convert b/t NAD27 and
NAD83/WGS84 datums. See the Proj.4 installation instructions for
details as the procedure has changed multiple times over the past
several years.

If the libraries and files are not installed in system
libraries, set the environment variables `PROJ4_LIBDIR` and
`PROJ4_INCDIR` accordingly (you probably want to do this in your
`.bashrc` or `.cshrc` file or a file that is included by them so that
these are set automatically every time you log in). If `libproj.so`
is in `/home/tools/proj4/lib` and header files are in
`/home/tools/proj4/include`, then:

  * bash shell users

```
export PROJ4_LIBDIR=/usr/local/proj4/lib
export PROJ4_INCDIR=/usr/local/proj4/include
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${PROJ4_LIBDIR}
```

  * tcsh shell users

```
setenv PROJ4_LIBDIR /usr/local/proj4/lib
setenv PROJ4_INCDIR /usr/local/proj4/include
setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${PROJ4_LIBDIR}
```

### Install unit testing software (OPTIONAL)

#### cppunit - C++ unit testing library

[http://dev-www.libreoffice.org/src/cppunit-1.14.0.tar.gz](http://dev-www.libreoffice.org/src/cppunit-1.14.0.tar.gz)
versions 1.10.2 are known to work

This library is used in the unit testing of the code. It is only
used for testing and skipping this step will not affect the
functionality of the velocity model software.

1. Download the cppunit source code from
[https://freedesktop.org/wiki/Software/cppunit/](https://freedesktop.org/wiki/Software/cppunit/)
via the link above, or install the binary package for your operating
system (most Linux distributions have cppunit packages).

2. Run `configure` and then `make` and `make install`.

  * If the libraries and files are not installed in system libraries,
    set the environment variables `CPPUNIT_LIBDIR` and `CPPUNIT_INCDIR`
    accordingly, in the same manner as you did for the required
    software.

## cencalvm - USGS Central CA velocity model query software
[https://github.com/baagaard-usgs/cencalvm](https://github.com/baagaard-usgs/cencalvm)

1. Download the source code for the latest release from [GitHub]([https://github.com/baagaard-usgs/cencalvm](https://github.com/baagaard-usgs/cencalvm/releases)).

1. Run `configure` and then `make` and `make install`.

Use `configure --help` to see all of the arguments configure
accepts. After `make install`, the header files, libraries, and
executables can be found under the `PREFIX` directory (defined by
`--prefix=PREFIX` when running configure).

  * Euclid and Proj.4 not in default paths

    Configure will fail if it cannot find the header files and
    libraries for Euclid and Proj4. You will need to set `CPPFLAGS` and
    `LDFLAGS` when running configure. For example if you have defined
    the environment variables as suggested above, you can simply run
    configure using

```
configure \
    CPPFLAGS="-I${ETREE_INCDIR} -I${PROJ4_INCDIR}" \
    LDFLAGS="-L${ETREE_LIBDIR} -L${PROJ4_LIBDIR}"
```

  * Changing the compilers.

    You can change the compilers configure will use by setting `CC` (C),
    `CXX` (C++), and `FC` (Fortran) when running configure. For example,
    to build using the Sunpro compilers

```
configure FC=f95 CC=cc
CXX=CC
```

    These compilers must be binary compatible! The libraries built by
    the C++ compiler must be able to be used by the C and Fortran
    compilers. For example, you CANNOT mix compilers from gcc-5.x and
    gcc-6.x.

## Run unit tests (OPTIONAL)

If you have cppunit and enabled testing (`--enable-testing`), then
you can run `make check` to run the unit tests.

## Download the velocity model(s) from
[ftp://ehzftp.wr.usgs.gov/baagaard/cencalvm/database](ftp://ehzftp.wr.usgs.gov/baagaard/cencalvm/database)

In the current configuration, the detailed model is required while the
extended (regional) model is optional. Use of the extended model is
disabled by default. See the
[online documentation](https://baagaard-usgs.github.io/cencalvm/query.md)
for details on how to enable use of the extended model.

Download the `MD5SUM` files and check to the integrity of the
downloaded file by running `md5sum -c MD5SUMS_GZIPPED` for the
detailed model and `md5sum -c MD5SUMS_GZIPPEDEXT` for the extended
model. After gunzipping the file, you can again check the integrity
of the file by running `md5sum -c MD5SUMS` for the detailed model
and `md5sum -c MD5SUMSEXT` for the extended model. Each time you
should get output reporting that the file is OK.

## Run examples (OPTIONAL)

Run the examples for the languages you are interested in. The examples
are in the `examples/query` directory. See the
[online documentation](https://baagaard-usgs.github.io/cencalvm/examples.md) for more information.

Set environment variables to find libraries and executables that are
in `PREFIX`. If the libraries and executables are not installed in
system libraries, set the environment variables `LD_LIBRARY_PATH` and
`PATH` (or their equivalent) accordingly. You will probably want to do
this in your `.bashrc` or `.cshrc` file or a file that is included by
them so that these are set automatically every time you log in.

  * bash shell users

```
export CENCALVM_DIR=/home/johndoe/cencalvm
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${CENCALVM_DIR}/lib
export PATH=${PATH}:${CENCALVM_DIR}/bin
```

  * tcsh shell users
```
setenv CENCALVM_DIR /home/johndoe/cencalvm
setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${CENCALVM_DIR}/lib
setenv PATH ${PATH}:${CENCALVM_DIR}/bin
```

# Known Issues

* Uncompressing the database file gives a 'File too large' error.

  Older versions of gzip may not be able to uncompress the large etree
  database file with is 7.5 GB uncompressed. The workaround is to get
  gzip version 1.3.3 or later.

* Configure warning 'WARNING: etree.h: present but cannot be compiled'.

  The Sunpro compiler chokes on the btree.h header file because it
  contains an empty declaration. The workaround is to change line 39
  of btree.h from

```
typedef struct btree_t {} btree_t;
```

to

```
typedef struct btree_t { int a; } btree_t;
```


# Getting Help

If you run into problems installing this software, please send an
email to Brad Aagaard and include (1) `config.log` from the top level
directory (generated when running configure), (2) a copy of the output
showing the error (i.e., dump the output from the command to a file),
and (3) a complete description of the problem.
