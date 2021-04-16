/*****************************************************************************

        StageProc2F64Neon.hpp
        Author: Laurent de Soras, 2016
        Ported to double by Dario Mambro, 2020

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if ! defined (hiir_StageProc2F64Neon_CODEHEADER_INCLUDED)
#define hiir_StageProc2F64Neon_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "hiir/StageDataNeonV2F64.h"



namespace hiir
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <>
inline void	StageProc2F64Neon <1>::process_sample_pos (const int nbr_coefs, float64x2_t &spl_0, float64x2_t &spl_1, StageDataNeonV2F64 *stage_arr)
{
	const int      cnt = nbr_coefs + 2 - 1;

	const float64x2_t tmp_0 = vmlaq_f64 (
		        vld1q_f64 (stage_arr [cnt - 2]._mem ),
		spl_0 - vld1q_f64 (stage_arr [cnt    ]._mem ),
		        vld1q_f64 (stage_arr [cnt    ]._coef)
	);

	vst1q_f64 (stage_arr [cnt - 2]._mem, spl_0);
	vst1q_f64 (stage_arr [cnt - 1]._mem, spl_1);
	vst1q_f64 (stage_arr [cnt    ]._mem, tmp_0);

	spl_0 = tmp_0;
}



template <>
inline void	StageProc2F64Neon <0>::process_sample_pos (const int nbr_coefs, float64x2_t &spl_0, float64x2_t &spl_1, StageDataNeonV2F64 *stage_arr)
{
	const int      cnt = nbr_coefs + 2;

	vst1q_f64 (stage_arr [cnt - 2]._mem, spl_0);
	vst1q_f64 (stage_arr [cnt - 1]._mem, spl_1);
}



template <int REMAINING>
void	StageProc2F64Neon <REMAINING>::process_sample_pos (const int nbr_coefs, float64x2_t &spl_0, float64x2_t &spl_1, StageDataNeonV2F64 *stage_arr)
{
	const int      cnt = nbr_coefs + 2 - REMAINING;

	const float64x2_t tmp_0 = vmlaq_f64 (
		        vld1q_f64 (stage_arr [cnt - 2]._mem ),
		spl_0 - vld1q_f64 (stage_arr [cnt    ]._mem ),
		        vld1q_f64 (stage_arr [cnt    ]._coef)
	);
	const float64x2_t tmp_1 = vmlaq_f64 (
		        vld1q_f64 (stage_arr [cnt - 1]._mem ),
		spl_1 - vld1q_f64 (stage_arr [cnt + 1]._mem ),
		        vld1q_f64 (stage_arr [cnt + 1]._coef)
	);

	vst1q_f64 (stage_arr [cnt - 2]._mem, spl_0);
	vst1q_f64 (stage_arr [cnt - 1]._mem, spl_1);

	spl_0 = tmp_0;
	spl_1 = tmp_1;

	StageProc2F64Neon <REMAINING - 2>::process_sample_pos (
		nbr_coefs,
		spl_0,
		spl_1,
		stage_arr
	);
}



template <>
inline void	StageProc2F64Neon <1>::process_sample_neg (const int nbr_coefs, float64x2_t &spl_0, float64x2_t &spl_1, StageDataNeonV2F64 *stage_arr)
{
	const int      cnt = nbr_coefs + 2 - 1;

	float64x2_t tmp_0 = spl_0;
	tmp_0 += vld1q_f64 (stage_arr [cnt    ]._mem );
	tmp_0 *= vld1q_f64 (stage_arr [cnt    ]._coef);
	tmp_0 -= vld1q_f64 (stage_arr [cnt - 2]._mem );

	vst1q_f64 (stage_arr [cnt - 2]._mem, spl_0);
	vst1q_f64 (stage_arr [cnt - 1]._mem, spl_1);
	vst1q_f64 (stage_arr [cnt    ]._mem, tmp_0);

	spl_0 = tmp_0;
}

template <>
inline void	StageProc2F64Neon <0>::process_sample_neg (const int nbr_coefs, float64x2_t &spl_0, float64x2_t &spl_1, StageDataNeonV2F64 *stage_arr)
{
	const int      cnt = nbr_coefs + 2;

	vst1q_f64 (stage_arr [cnt - 2]._mem, spl_0);
	vst1q_f64 (stage_arr [cnt - 1]._mem, spl_1);
}

template <int REMAINING>
void	StageProc2F64Neon <REMAINING>::process_sample_neg (const int nbr_coefs, float64x2_t &spl_0, float64x2_t &spl_1, StageDataNeonV2F64 *stage_arr)
{
	const int      cnt = nbr_coefs + 2 - REMAINING;

	float64x2_t tmp_0 = spl_0;
	tmp_0 += vld1q_f64 (stage_arr [cnt    ]._mem );
	tmp_0 *= vld1q_f64 (stage_arr [cnt    ]._coef);
	tmp_0 -= vld1q_f64 (stage_arr [cnt - 2]._mem );

	float64x2_t tmp_1 = spl_1;
	tmp_1 += vld1q_f64 (stage_arr [cnt + 1]._mem );
	tmp_1 *= vld1q_f64 (stage_arr [cnt + 1]._coef);
	tmp_1 -= vld1q_f64 (stage_arr [cnt - 1]._mem );

	vst1q_f64 (stage_arr [cnt - 2]._mem, spl_0);
	vst1q_f64 (stage_arr [cnt - 1]._mem, spl_1);

	spl_0 = tmp_0;
	spl_1 = tmp_1;

	StageProc2F64Neon <REMAINING - 2>::process_sample_neg (
		nbr_coefs,
		spl_0,
		spl_1,
		stage_arr
	);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace hiir



#endif   // hiir_StageProc2F64Neon_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
