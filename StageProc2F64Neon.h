/*****************************************************************************

        StageProc2F64Neon.h
        Author: Laurent de Soras, 2016
        Ported to double by Dario Mambro, 2020

Template parameters:
	- REMAINING: Number of remaining coefficients to process, >= 0

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (hiir_StageProc2F64Neon_HEADER_INCLUDED)
#define hiir_StageProc2F64Neon_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "hiir/def.h"



namespace hiir
{



class StageDataNeonV4;

template <int REMAINING>
class StageProc2F64Neon
{
	static_assert (REMAINING >= 0, "REMAINING must be >= 0.");

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	static hiir_FORCEINLINE void
	               process_sample_pos (const int nbr_coefs, float64x2_t &spl_0, float64x2_t &spl_1, StageDataNeonV2F64 *stage_arr);
	static hiir_FORCEINLINE void
	               process_sample_neg (const int nbr_coefs, float64x2_t &spl_0, float64x2_t &spl_1, StageDataNeonV2F64 *stage_arr);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                   StageProc2F64Neon ()                                        = delete;
	               StageProc2F64Neon (const StageProc2F64Neon <REMAINING> &other) = delete;
	               StageProc2F64Neon (StageProc2F64Neon <REMAINING> &&other)      = delete;
	               ~StageProc2F64Neon ()                                       = delete;
	StageProc2F64Neon <REMAINING> &
	               operator = (const StageProc2F64Neon <REMAINING> &other)     = delete;
	StageProc2F64Neon <REMAINING> &
	               operator = (StageProc2F64Neon <REMAINING> &&other)          = delete;
	bool           operator == (const StageProc2F64Neon <REMAINING> &other) const = delete;
	bool           operator != (const StageProc2F64Neon <REMAINING> &other) const = delete;

}; // class StageProc2F64Neon



}  // namespace hiir



#include "hiir/StageProc2F64Neon.hpp"



#endif   // hiir_StageProc2F64Neon_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
