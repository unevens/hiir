/*****************************************************************************

        Upsampler2x8Avx.hpp
        Ported  Upampler2x4Sse.hpp from SSE to AVX by Dario Mambro
        Upsampler2x4Sse.hpp by Laurent de Soras

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if ! defined (hiir_Upsampler2x8Avx_CODEHEADER_INCLUDED)
#define hiir_Upsampler2x8Avx_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "hiir/StageProc8Avx.h"

#include <cassert>



namespace hiir
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name: ctor
Throws: Nothing
==============================================================================
*/

template <int NC>
Upsampler2x8Avx <NC>::Upsampler2x8Avx ()
:	_filter ()
{
	for (int i = 0; i < NBR_COEFS + 2; ++i)
	{
		_mm256_store_ps (_filter [i]._coef, _mm256_setzero_ps ());
	}

	clear_buffers ();
}



/*
==============================================================================
Name: set_coefs
Description:
   Sets filter coefficients. Generate them with the PolyphaseIir2Designer
   class.
   Call this function before doing any processing.
Input parameters:
	- coef_arr: Array of coefficients. There should be as many coefficients as
      mentioned in the class template parameter.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Upsampler2x8Avx <NC>::set_coefs (const double coef_arr [NBR_COEFS])
{
	assert (coef_arr != 0);

	for (int i = 0; i < NBR_COEFS; ++i)
	{
		_mm256_store_ps (_filter [i + 2]._coef, _mm256_set1_ps (float (coef_arr [i])));
	}
}



/*
==============================================================================
Name: process_sample
Description:
	Upsamples (x2) the input vector, generating two output vectors.
Input parameters:
	- input: The input vector.
Output parameters:
	- out_0: First output vector.
	- out_1: Second output vector.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Upsampler2x8Avx <NC>::process_sample (__m256 &out_0, __m256 &out_1, __m256 input)
{
	__m256         even = input;
	__m256         odd  = input;
	StageProc8Avx <NBR_COEFS>::process_sample_pos (
		NBR_COEFS,
		even,
		odd,
		&_filter [0]
	);
	out_0 = even;
	out_1 = odd;
}



/*
==============================================================================
Name: process_block
Description:
	Upsamples (x2) the input vector block.
	Input and output blocks may overlap, see assert() for details.
Input parameters:
	- in_ptr: Input array, containing nbr_spl vector.
		No alignment constraint.
	- nbr_spl: Number of input vectors to process, > 0
Output parameters:
	- out_0_ptr: Output vector array, capacity: nbr_spl * 2 vectors.
		No alignment constraint.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Upsampler2x8Avx <NC>::process_block (float out_ptr [], const float in_ptr [], long nbr_spl)
{
	assert (out_ptr != 0);
	assert (in_ptr != 0);
	assert (out_ptr >= in_ptr + nbr_spl * 8 || in_ptr >= out_ptr + nbr_spl * 8);
	assert (nbr_spl > 0);

	long           pos = 0;
	do
	{
		__m256         dst_0;
		__m256         dst_1;
		const __m256   src = _mm256_loadu_ps (in_ptr + pos * 8);
		process_sample (dst_0, dst_1, src);
		_mm256_storeu_ps (out_ptr + pos * 16    , dst_0);
		_mm256_storeu_ps (out_ptr + pos * 16 + 8, dst_1);
		++ pos;
	}
	while (pos < nbr_spl);
}



/*
==============================================================================
Name: clear_buffers
Description:
	Clears filter memory, as if it processed silence since an infinite amount
	of time.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Upsampler2x8Avx <NC>::clear_buffers ()
{
	for (int i = 0; i < NBR_COEFS + 2; ++i)
	{
		_mm256_store_ps (_filter [i]._mem, _mm256_setzero_ps ());
	}
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace hiir



#endif   // hiir_Upsampler2x8Avx_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/