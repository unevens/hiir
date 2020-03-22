/*****************************************************************************

        Downsampler2xSseOld.hpp
        Author: Laurent de Soras, 2005

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if defined (hiir_Downsampler2xSseOld_CURRENT_CODEHEADER)
	#error Recursive inclusion of Downsampler2xSseOld code header.
#endif
#define hiir_Downsampler2xSseOld_CURRENT_CODEHEADER

#if ! defined (hiir_Downsampler2xSseOld_CODEHEADER_INCLUDED)
#define hiir_Downsampler2xSseOld_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "hiir/StageProcSseV4.h"

#include <xmmintrin.h>

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
Downsampler2xSseOld <NC>::Downsampler2xSseOld ()
:	_filter ()
{
	for (int i = 0; i < NBR_STAGES + 1; ++i)
	{
		_mm_store_ps (_filter [i]._coef, _mm_setzero_ps ());
	}
	if ((NBR_COEFS & 1) != 0)
	{
		const int      pos = (NBR_COEFS ^ 1) & (STAGE_WIDTH - 1);
		_filter [NBR_STAGES]._coef [pos] = 1;
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
void	Downsampler2xSseOld <NC>::set_coefs (const double coef_arr [])
{
	assert (coef_arr != nullptr);

	for (int i = 0; i < NBR_COEFS; ++i)
	{
		const int      stage = (i / STAGE_WIDTH) + 1;
		const int      pos   = (i ^ 1) & (STAGE_WIDTH - 1);
		_filter [stage]._coef [pos] = DataType (coef_arr [i]);
	}
}



/*
==============================================================================
Name: process_sample
Description:
	Downsamples (x2) one pair of samples, to generate one output sample.
Input parameters:
	- in_ptr: pointer on the two samples to decimate
Returns: Samplerate-reduced sample.
Throws: Nothing
==============================================================================
*/

template <int NC>
float	Downsampler2xSseOld <NC>::process_sample (const float in_ptr [2])
{
	assert (in_ptr != nullptr);

	// Combines two input samples and two mid-processing data
	const __m128   spl_in  = _mm_loadu_ps (in_ptr);
	const __m128   spl_mid = _mm_load_ps (_filter [NBR_STAGES]._mem);
	__m128         y       = _mm_shuffle_ps (spl_in, spl_mid, 0x44);

	__m128         mem     = _mm_load_ps (_filter [0]._mem);

	// Processes each stage
	StageProcSseV4 <NBR_STAGES>::process_sample_pos (&_filter [0], y, mem);

	_mm_store_ps (_filter [NBR_STAGES]._mem, y);

	// Averages both paths and outputs the result
	const __m128   dup_y = y;
	y = _mm_shuffle_ps (y, y, 0x80);
	y = _mm_add_ps (y, dup_y);
	y = _mm_shuffle_ps (y, y, 3);
	y = _mm_mul_ss (y, _mm_set_ss (0.5f));

	return _mm_cvtss_f32 (y);
}



/*
==============================================================================
Name: process_block
Description:
	Downsamples (x2) a block of samples.
	Input and output blocks may overlap, see assert() for details.
Input parameters:
	- in_ptr: Input array, containing nbr_spl * 2 samples.
	- nbr_spl: Number of samples to output, > 0
Output parameters:
	- out_ptr: Array for the output samples, capacity: nbr_spl samples.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Downsampler2xSseOld <NC>::process_block (float out_ptr [], const float in_ptr [], long nbr_spl)
{
	assert (in_ptr  != nullptr);
	assert (out_ptr != nullptr);
	assert (out_ptr <= in_ptr || out_ptr >= in_ptr + nbr_spl * 2);
	assert (nbr_spl > 0);

	long           pos = 0;
	do
	{
		out_ptr [pos] = process_sample (in_ptr + pos * 2);
		++ pos;
	}
	while (pos < nbr_spl);
}



/*
==============================================================================
Name: process_sample_split
Description:
	Split (spectrum-wise) in half a pair of samples. The lower part of the
	spectrum is a classic downsampling, equivalent to the output of
	process_sample().
	The higher part is the complementary signal: original filter response
	is flipped from left to right, becoming a high-pass filter with the same
	cutoff frequency. This signal is then critically sampled (decimation by 2),
	flipping the spectrum: Fs/4...Fs/2 becomes Fs/4...0.
Input parameters:
	- in_ptr: pointer on the pair of input samples
Output parameters:
	- low: output sample, lower part of the spectrum (downsampling)
	- high: output sample, higher part of the spectrum.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Downsampler2xSseOld <NC>::process_sample_split (float &low, float &high, const float in_ptr [2])
{
	assert (in_ptr != nullptr);

	// Combines two input samples and two mid-processing data
	const __m128   spl_in  = _mm_loadu_ps (in_ptr);
	const __m128   spl_mid = _mm_load_ps (_filter [NBR_STAGES]._mem);
	__m128         y       = _mm_shuffle_ps (spl_in, spl_mid, 0x44);

	__m128         mem     = _mm_load_ps (_filter [0]._mem);

	// Processes each stage
	StageProcSseV4 <NBR_STAGES>::process_sample_pos (&_filter [0], y, mem);

	_mm_store_ps (_filter [NBR_STAGES]._mem, y);

	// Outputs the result
	__m128         dup_y = y;
	y   = _mm_shuffle_ps (y, y, 0x80);
	y   = _mm_add_ps (y, dup_y);
	y   = _mm_shuffle_ps (y, y, 3);
	y   = _mm_mul_ss (y, _mm_set_ss (0.5f));
	low = _mm_cvtss_f32 (y);

	dup_y = _mm_shuffle_ps (dup_y, dup_y, 3);
	dup_y = _mm_sub_ps (dup_y, y);
	high  = _mm_cvtss_f32 (dup_y);
}



/*
==============================================================================
Name: process_block_split
Description:
	Split (spectrum-wise) in half a block of samples. The lower part of the
	spectrum is a classic downsampling, equivalent to the output of
	process_block().
	The higher part is the complementary signal: original filter response
	is flipped from left to right, becoming a high-pass filter with the same
	cutoff frequency. This signal is then critically sampled (decimation by 2),
	flipping the spectrum: Fs/4...Fs/2 becomes Fs/4...0.
	Input and output blocks may overlap, see assert() for details.
Input parameters:
	- in_ptr: Input array, containing nbr_spl * 2 samples.
	- nbr_spl: Number of samples for each output, > 0
Output parameters:
	- out_l_ptr: Array for the output samples, lower part of the spectrum
		(downsampling). Capacity: nbr_spl samples.
	- out_h_ptr: Array for the output samples, higher part of the spectrum.
		Capacity: nbr_spl samples.
Throws: Nothing
==============================================================================
*/

template <int NC>
void	Downsampler2xSseOld <NC>::process_block_split (float out_l_ptr [], float out_h_ptr [], const float in_ptr [], long nbr_spl)
{
	assert (in_ptr    != nullptr);
	assert (out_l_ptr != nullptr);
	assert (out_l_ptr <= in_ptr || out_l_ptr >= in_ptr + nbr_spl * 2);
	assert (out_h_ptr != nullptr);
	assert (out_h_ptr <= in_ptr || out_h_ptr >= in_ptr + nbr_spl * 2);
	assert (out_h_ptr != out_l_ptr);
	assert (nbr_spl > 0);

	long           pos = 0;
	do
	{
		process_sample_split (out_l_ptr [pos], out_h_ptr [pos], in_ptr + pos * 2);
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
void	Downsampler2xSseOld <NC>::clear_buffers ()
{
	for (int i = 0; i < NBR_STAGES + 1; ++i)
	{
		_mm_store_ps (_filter [i]._mem, _mm_setzero_ps ());
	}
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace hiir



#endif   // hiir_Downsampler2xSseOld_CODEHEADER_INCLUDED

#undef hiir_Downsampler2xSseOld_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
