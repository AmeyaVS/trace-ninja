#README.md

##Instructions for building and using the trace tool.

**Note:** Requires [cmake](https://cmake.org/) and [boost](http://www.boost.org/) library to be installed in the system.

Follow these steps to build (test, and install wherever applicable).

```sh
$ cd <project folder containing CMakeLists.txt>
$ mkdir build && cd build
$ cmake ..
$ make -j<ncpus>
$ make (install/test)
```

* [libTracer](libTracer): Use this library for tracing application build with GCC's/Clang's _-finstrument-functions_ option.
* [tests](tests): Folder containing some simple exmaple test cases with C,C++,C++ templates etc.
* [traceParser](traceParser): Folder contains utility to parse the binary trace file and convert it to compatiable json format to be loaded on _chrome:tracing_ trace viewer tool.
* [nmParser](nmParser): This folder conatins the parser implementation for outputs from **_nm_** and **_/proc/[pid]/maps_** file (this folder will be deleted in subsequent update and completion of the trace tool.
