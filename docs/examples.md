# Examples of Velocity Model Queries

The `examples` directory contains sample query applications, input
files, and output files. When querying with the provided input files,
you can compare your output to the provided output files to make sure
the example ran properly.

The `cpp`, `c`, and `f77` directories contain examples that illustrate
how to query the database from C++, C, and Fortran, respectively.

## Directories

* **data** Sample input and output data files.

* **cpp** Examples of how to use the query routines from C++.

* **c** Example of how to use the query routines from C.

* **f77** Example of how to use the query routines from Fortran 77.

## Sample data files

The `data` directory contains sample input and output data files used in
the examples. The input files are "*.in" and the output files are
"*.out". The examples use the input files in this directory as
input. The output files are provided so that you can compare the
output from the examples against these data files that are
considered "correct."

### Files related to querying the detailed model

```
sample-05.0.0.out               Output for MAXRES query of 05.0.0
sample-05.0.1_fixedres.out      Output for FIXEDRES query of 05.0.1
sample-05.0.1.out               Output for MAXRES query of 05.0.1
sample-05.0.1_waveres.out       Output for WAVERES query of 05.0.1
sample-05.0.x.in                Input for version 05.0.x
sample-05.1.0_fixedres.out      Output for FIXEDRES query of 05.1.0
sample-05.1.0_maxres.out        Output for MAXRES query of 05.1.0
sample-05.1.0_waveres.out       Output for WAVERES query of 05.1.0
sample-05.1.x.in                Input for version 05.1.x
sample-08.2.0_fixedres.out      Output for FIXEDRES query of 08.2.0
sample-08.2.0_maxres.out        Output for MEXRES query of 08.2.0
sample-08.2.0_waveres.out       Output for WAVERES query of 08.2.0
sample-08.3.0_fixedres.out      Output for FIXEDRES query of 08.3.0
sample-08.3.0_maxres.out        Output for MAXRES query of 08.3.0
sample-08.3.0_waveres.out       Output for WAVERES query of 08.3.0
```

### Files related to querying the regional model

```
sample_ext-05.1.0_fixedres.out  Output for FIXEDRES query of 05.1.0
sample_ext-05.1.0_maxres.out    Output for MAXRES query of 05.1.0
sample_ext-05.1.0_waveres.out   Output for WAVERES query of 05.1.0
sample_ext-05.1.x.in            Input for version 05.1.x
sample_ext-08.2.0_fixedres.out  Output for FIXEDRES query of 08.2.0
sample_ext-08.2.0_maxres.out    Output for MAXRES query of 08.2.0
sample_ext-08.2.0_waveres.out   Output for WAVERES query of 08.2.0
sample_ext-08.3.0_fixedres.out  Output for FIXEDRES query of 08.3.0
sample_ext-08.3.0_maxres.out    Output for MAXRES query of 08.3.0
sample_ext-08.3.0_waveres.out   Output for WAVERES query of 08.3.0
```

## Instructions that apply to ALL examples

Create a symbolic link from the database file to the example
directory. For example, if the database is
`/home/data/USGSBayAreaVM-08.3.0.etree` and you want to run the C++
example in the cpp directory, type

```
$ cd cpp
$ ln -s /home/data/USGSBayAreaVM-08.3.0.etree .
```

The examples are setup to dump warnings to the screen, so when it
encounters a point in air where there is no data it will dump a
warning to the screen that can be ignored; the material property
values will be -999 for this point in the output file. If a filename
is given for the log file, then this point will appear in the log file
as well.

## C and C++ examples

The instructions are identical for the C and C++ examples.

1. Run `make` to build the example query application.

2. Run `make testmaxres` to run the sample MAXRES query. The output
   `test_maxres.out` should be identical to the corresponding
   `sample-VERSION.out` file in the data directory.

3. Run `make testfixedres` to run the sample FIXEDRES query. The
   output `test_fixedres.out` should be identical to the corresponding
   `sample-VERSION_fixedres.out` file in the data directory.

4. Run `make testwaveres` to run the sample WAVERES query. The output
   `test_waveres.out` should be identical to the corresponding
   `sample-VERSION_waveres.out` file in the data directory.

5. Analogous tests using both the detailed and extended (regional)
models can be run via `make testextmaxres`, `make testextfixedres`,
and `make testextwaveres`. The output should be identical to the
sample_ext* files in the data directory.

## Fortran 77 example.

1. Edit the source file `cencalvmf77query.f` to select the type of
query that you want. You may need to adjust the name of the output
filename (see line 54), the parameters (see line 68), and whether to
use the extended (regional) model (see line 132).

2. Run `make` and then `./cencalvmf77query`.

  * For the sample MAXRES query. The output `test_maxres.out` should
    be identical to the corresponding `sample-VERSION.out` file in the
    data directory.

  * For the sample FIXEDRES query. The output `test_fixedres.out`
    should be identical to the corresponding
    `sample-VERSION_fixedres.out` file in the data directory.

  * For the sample WAVERES query. The output `test_waveres.out` should
    be identical to the corresponding `sample-VERSION_waveres.out`
    file in the data directory.

3. Similarly, for queries using the extended (regional) model compare
the output to the files beginning with 'sample_ext' in the data
directory.
