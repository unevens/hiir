# HIIR

This repository hosts a fork of the [HIIR library by Laurent De Soras](http://ldesoras.free.fr/prod.html), *"an oversampling and Hilbert transform library in C++"*, with some new files that **add support for double precision floating point numbers and AVX instructions**, making the library able to work with:

- 8 interleaved channels of single precision floating point data (using AVX instructions), see `Upsampler2x8xAvx.hpp`.
- 4 interleaved channels of double precision floating point data (using AVX instructions), see `Upsampler2x4xAvxDouble.hpp`.
- 2 interleaved channels of double precision floating point data (using SSE2 instructions), see `Upsampler2x2xSseDouble.hpp`.

The original HIIR library is already able to work with:

- 4 interleaved channels of single precision floating point data (using SSE or Neon instructions), see `Upsampler2x4xSse.hpp` and `Upsampler2x4xNeon.hpp`.

This fork of the library is also header-only, and does not contain the `test` folder.

For usage instructions see the original library readme: `readme.txt`.
