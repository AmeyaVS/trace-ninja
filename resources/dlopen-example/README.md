# README.md

This directory contains a sample example for usage of `dlopen` and `dlclose` operations for loading and unloading of shared library.

To build and run the example just type `make`.

## Observations

* Kindly observe the following file generated after the running the `make` command:

    1. map.0.log
    2. map.1.log
    3. map.2.log
    4. map.3.log
    5. map.4.log

* Better do a three-way compare of the files in sequence(i.e. diff between `map.0.log`, `map.1.log`, and `map.2.log`).

* For handling of dynamically loaded libraries we will need to wrap both `dlopen` and `dlclose` API's to intercept such calls.
* Probably we will also need to wrap the calls to `dlsym`, this will require more analysis.

**Note:**

* Sample `Makefile` referenced from [this](https://spin.atomicobject.com/2016/08/26/makefile-c-projects/) blog post.

