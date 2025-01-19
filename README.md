# Lucy's Useful C Library

I often do projects in pure C, and I find myself needing some of the same
tools over and over again, such as needing a dynamically sized list. So, I
created this little library to have one place for all of those things without
needing to manage individual things across multiple projects. Feel free to use
this if you find it useful, just give me credit (see [LICENSE](./LICENSE))!

## Build

This library uses CMake, so you can just do a simple:

```sh
cd liblucu
mkdir build
cd build
cmake ..
cmake --build .
```

## Documentation

CMake is configured to build documentation with Doxygen. Just use your build
system:

```sh
# in build directory:
cmake --build . -t docs
```

## Tests

Running tests requires installing [Criterion](https://github.com/Snaipe/Criterion).
Then you can just use your build system to run the tests:

```sh
# in build directory:
cmake --build . -t test
```

## Install

You can also install with CMake:

```sh
# in build directory
sudo cmake --install .
```
