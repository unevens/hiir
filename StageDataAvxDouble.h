/*****************************************************************************

        StageDataAvxDouble.h
		Port of StageDataSse.h from float to double by Dario Mambro
        StageDataSse.h by Laurent de Soras, 2005

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if ! defined (hiir_StageDataAvxDouble_HEADER_INCLUDED)
#define hiir_StageDataAvxDouble_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma once
	#pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <immintrin.h> 



namespace hiir
{



class StageDataAvxDouble
{

public:

	union
	{
		__m256d         _coef4;     // Just to ensure alignement
		double          _coef [4];  
	};
	union
	{
		__m256d         _mem4;
		double          _mem [4];   // y of the stage
	};

}; // class StageDataAvxDouble



}  // namespace hiir



#endif   // hiir_StageDataAvxDouble_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
