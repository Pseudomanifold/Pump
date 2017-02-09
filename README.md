[![Build Status](https://travis-ci.org/Submanifold/Pump.svg?branch=master)](https://travis-ci.org/Submanifold/Pump)

# Pump &mdash; A generic data flow program

`Pump` is a generic data flow program, meant to accommodate those who
often use multiple unrelated programs for *data wrangling*. Normally,
you would have to keep track manually about which program to call and
in what order.

With `Pump`, you just have to write a&nbsp;(simple) workflow file and
let the program run it for you. Not only does this make executing the
different scripts easier, it also gives you a better control over how
data sets are being created. You may even store the workflow file via
`git` or other version control systems, for example.

That way, you will *never* forget how you created that data set!

# Requirements

`Pump` is written in C++, following the recent C++11 standard, so you
definitely need a somewhat modern C++ compiler. Other than `CMake` as
a build system, `Pump` does not require any other dependencies. 

# Building

* Clone the repository
* Install [`CMake`](https://cmake.org) using a package manager of your
  choice
* Install a modern C++ compiler such as [`clang`](http://clang.llvm.org)
  if you do not have one already

In the cloned repository, issue the following commands to build `Pump`:

    $ mkdir build
    $ cd build
    $ cmake ../
    $ make

This should be sufficient to build `pump`, some related tools, and some
examples.

# Usage

Please consult [the wiki](https://github.com/Submanifold/Pump/wiki) for
usage information. You are welcome to extend the documentation, request
features, and so on&hellip;

# License

Pump uses the MIT license. Please see the file [`LICENSE.md`](LICENSE.md)
in the main directory of the repository for more details.
