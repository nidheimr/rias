# Rias

## Overview

Rias is a header only library that tracks allocations to help identify unhandled memory during development. It was written for educational purposes and is likely not very efficient.

## Usage

Rias is entirely one header file. To use it, you must simply include the header file into the file with your main function, and any file that uses stdlib memory allocation.

## Building (the test executable)

This project uses CMake.

Assuming you have CMake installed and some form of windows C compiler (such as MSVC), you can run the following commands inside the project directory:
1. `cmake -S . -B build`
2. `cmake --build build`

The executable will be placed inside `./bin/`.