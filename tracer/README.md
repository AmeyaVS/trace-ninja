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

##Setup

Install the libTracer library and traceParser utility and following steps will
be required for setting up the environment variable:

```sh
export TRACER=$HOME/apps/tracer
export PATH=$PATH:$TRACER/bin
```

After setting up the environment variable in **_~/.bashrc_** restart your
terminal session.

Check if you are able to execute _traceParser_ utility from the terminal.

```sh
$ traceParser -h
```


##Using the tracing library.

* First build your application in debug mode with "**_-finstrument-functions_**" compiler flags to GCC/Clang.
* Then execute your application:

    ```sh
    $ LD_PRELOAD=$TRACER/lib/libTracer.so ./<your_application> <your_application_args>
    ```

* The trace library will create log file of the name: **trace.<pid>.bin** and
  **map.<pid>.log**.
* Use the [symbol.sh](tests/symbols.sh) as template to dump nm utility output to
  list all symbols in the application libraries and the application itself.

    ```sh
    $ nm -o -C --line-numbers <your application executables/library files> >> <symbol filename>.symb
    ```

* Use the traceParsre utility to convert binary traces to JSON format to be
  comsumbed by Chrome about:tracing trace viewer:

    ```sh
    $ traceParser -t <path to trace file> -s <symbol file generated in previous step> -o <output JSON file name>
    ```

* Viewing the traces:
    - Open Chrome/Chromium browser.
    - In the navigation bar enter the text: _**about:tracing**_.
    - Click on the load button and navigate to the folder where the parsed JSON
      file kept.
    - Wait for the rendering to complete to show the results.
