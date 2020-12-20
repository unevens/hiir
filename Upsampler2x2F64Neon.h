/*****************************************************************************

        Upsampler2x2F64Neon.h
        Author: Laurent de Soras, 2016
        Ported to double by Dario Mambro, 2020

Upsamples vectors of 4 double by a factor 2 the input signal, using the NEON
instruction set.

This object must be aligned on a 16-byte boundary!

Template parameters:
	- NC: number of coefficients, > 0

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (hiir_Upsampler2x2F64Neon_HEADER_INCLUDED)
#define hiir_Upsampler2x2F64Neon_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "hiir/def.h"
#include "hiir/StageDataNeonV2F64.h"

#include <arm_neon.h>

#include <array>



namespace hiir
{



template <int NC>
class Upsampler2x2F64Neon
{

	static_assert ((NC > 0), "Number of coefficient must be positive.");

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	typedef double DataType;
	static const int  _nbr_chn = 2;

	enum {         NBR_COEFS = NC };

	               Upsampler2x2F64Neon ();
	               Upsampler2x2F64Neon (const Upsampler2x2F64Neon <NC> &other) = default;
	               Upsampler2x2F64Neon (Upsampler2x2F64Neon <NC> &&other)      = default;
	               ~Upsampler2x2F64Neon ()                            = default;

	Upsampler2x2F64Neon <NC> &
	               operator = (const Upsampler2x2F64Neon <NC> &other) = default;
	Upsampler2x2F64Neon <NC> &
	               operator = (Upsampler2x2F64Neon <NC> &&other)      = default;

	void				set_coefs (const double coef_arr [NBR_COEFS]);
	hiir_FORCEINLINE void
	               process_sample (float64x2_t &out_0, float64x2_t &out_1, float64x2_t input);
	void				process_block (double out_ptr [], const double in_ptr [], long nbr_spl);
	void				clear_buffers ();




/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	typedef std::array <StageDataNeonV2F64, NBR_COEFS + 2> Filter;   // Stages 0 and 1 contain only input memories

	Filter         _filter; // Should be the first member (thus easier to align)



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool           operator == (const Upsampler2x2F64Neon <NC> &other) const = delete;
	bool           operator != (const Upsampler2x2F64Neon <NC> &other) const = delete;

}; // class Upsampler2x2F64Neon



}  // namespace hiir



#include "hiir/Upsampler2x2F64Neon.hpp"



#endif   // hiir_Upsampler2x2F64Neon_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
