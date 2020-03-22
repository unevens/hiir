# HIIR

This used to be a fork of the [HIIR library by Laurent De Soras](http://ldesoras.free.fr/prod.html), *"an oversampling and Hilbert transform library in C++"*, with some new files that add support for double precision floating point numbers and AVX instructions, making the library able to work with:

- 8 interleaved channels of single precision floating point data (using AVX instructions).
- 4 interleaved channels of double precision floating point data (using AVX instructions).
- 2 interleaved channels of double precision floating point data (using SSE2 instructions).

The original HIIR library is already able to work with:

- 4 interleaved channels of single precision floating point data (using SSE or Neon instructions), see `Upsampler2x4xSse.hpp` and `Upsampler2x4xNeon.hpp`.

As of March 2020 this functionality has been merged in the official release of HIIR, version 1.30, which also supports 

- 16 interleaved channels of single precision floating point data (using AVX512 instructions).
- 8 interleaved channels of double precision floating point data (using AVX512 instructions).

This repository has been updated to version 1.30, but is header-only, and does not contain the `test` folder.

For usage instructions see the original library readme: `readme.txt`.
