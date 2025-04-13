# Rias

## Overview

Rias is a header only library that tracks allocations to help identify unhandled memory during development. It was written for educational purposes and is likely not very efficient.

## Usage

Rias is entirely one header file. To use it, you must simply include the header file into the file with your main function and define `RIAS_IMPLEMENTATION`.
Its also wise to include Rias into all files that use stdlib allocation functions but do not define `RIAS_IMPLEMENTATION` again.

By default, Rias hijacks the main function and the stdlib memory allocation functions then dumps all leaks at the end of the program to stdout. To disable any of these features define the following:
- `RIAS_DONT_HIJACK_MAIN` -> you must manually call `rias_begin()` and `rias_end()` at the start and end of your main function respectively.
- `RIAS_DONT_WRAP_STDLIB` -> you must manually call `rias_<allocation function>()` and provide arguments that are usually provided by macros.
- `RIAS_DONT_DUMP_ON_EXIT` -> you must manually call `rias_end()` when you want to dump all detected leaks.

## Quirks

- If the program is exited using stdlib's exit() function then you must manually dump using `rias_end()`.
- The header file must be included in every source file that uses memory allocation functions.

## Building (the test executable)

This project uses CMake.

Assuming you have CMake installed and some form of windows C compiler (such as MSVC), you can run the following commands inside the project directory:
1. `cmake -S . -B build`
2. `cmake --build build`

The executable will be placed inside `./bin/`.