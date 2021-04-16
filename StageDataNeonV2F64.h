/*****************************************************************************

        StageDataNeonV2F64.h
        Author: Laurent de Soras, 2020
        Ported to double by Dario Mambro, 2020

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (hiir_StageDataNeonV2F64_HEADER_INCLUDED)
#define hiir_StageDataNeonV2F64_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <arm_neon.h>



namespace hiir
{



class StageDataNeonV2F64
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	union
	{
		__attribute__ ((aligned (32))) float64x2_t
		               _coef2;
		__attribute__ ((aligned (32))) double
		               _coef [2];
	};
	union
	{
		__attribute__ ((aligned (32))) float64x2_t
		               _mem2;
		__attribute__ ((aligned (32))) double
		               _mem [2];   // y of the stage
	};



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

}; // class StageDataNeonV2F64



}  // namespace hiir



//#include "hiir/StageDataNeonV2F64.hpp"



#endif   // hiir_StageDataNeonV2F64_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
