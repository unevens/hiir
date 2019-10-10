/*****************************************************************************

        StageDataSseDouble.h
		Port of StageDataSse.h from float to double by Dario Mambro
        StageDataSse.h by Laurent de Soras, 2005

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if ! defined (hiir_StageDataSseDouble_HEADER_INCLUDED)
#define hiir_StageDataSseDouble_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <emmintrin.h>



namespace hiir
{



class StageDataSseDouble
{

public:

	union
	{
		__m128d         _coef2;     // Just to ensure alignement
		double          _coef [2];  
	};
	union
	{
		__m128d         _mem2;
		double          _mem [2];   // y of the stage
	};

}; // class StageDataSseDouble



}  // namespace hiir



#endif   // hiir_StageDataSseDouble_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
