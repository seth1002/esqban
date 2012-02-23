/* inv_mdct.cpp
 *
 *	Inverse Discrete Cosine Transform for hybrid
 * synthesis in MPEG Audio Layer III. Based on the public_c source,
 * but almost completely redone by Jeff Tsay and Francois-Raymond Boyer.
 *
 * Tomilla's 9 point IDCT algorithm has been replaced with Boyer's, which
 * is faster. The final output multiplications required by Lee's FDCT
 * have been folded into the multiplication by the window.
 *
 * Last modified : 08/02/97
 */

#include "all.h"
#include "inv_mdct.h"

/*
 * This uses Byeong Gi Lee's Fast Cosine Transform algorithm to
 * decompose the 36 point and 12 point IDCT's into 9 point and 3
 * point IDCT's, respectively. Then the 9 point IDCT is computed
 * by Francois-Raymond Boyer's algorithm. See his comments before
 * the first 9 point IDCT. The 3 point IDCT is already efficient
 * to implement. -- Jeff Tsay. 
 */

#ifdef COOLPRO
#pragma optimize("",off)	// If full optimizations are done,
#pragma optimize("t",on)	// VC 5.0 generates bad code
#endif

real win[4][36] =
{
 { -1.6141214951E-02f, -5.3603178919E-02f, -1.0070713296E-01f, -1.6280817573E-01f,
   -4.9999999679E-01f, -3.8388735032E-01f, -6.2061144372E-01f, -1.1659756083E+00f,
   -3.8720752656E+00f, -4.2256286556E+00f, -1.5195289984E+00f, -9.7416483388E-01f,
   -7.3744074053E-01f, -1.2071067773E+00f, -5.1636156596E-01f, -4.5426052317E-01f,
   -4.0715656898E-01f, -3.6969460527E-01f, -3.3876269197E-01f, -3.1242222492E-01f,
   -2.8939587111E-01f, -2.6880081906E-01f, -5.0000000266E-01f, -2.3251417468E-01f,
   -2.1596714708E-01f, -2.0004979098E-01f, -1.8449493497E-01f, -1.6905846094E-01f,
   -1.5350360518E-01f, -1.3758624925E-01f, -1.2103922149E-01f, -2.0710679058E-01f,
   -8.4752577594E-02f, -6.4157525656E-02f, -4.1131172614E-02f, -1.4790705759E-02f },

 { -1.6141214951E-02f, -5.3603178919E-02f, -1.0070713296E-01f, -1.6280817573E-01f,
   -4.9999999679E-01f, -3.8388735032E-01f, -6.2061144372E-01f, -1.1659756083E+00f,
   -3.8720752656E+00f, -4.2256286556E+00f, -1.5195289984E+00f, -9.7416483388E-01f,
   -7.3744074053E-01f, -1.2071067773E+00f, -5.1636156596E-01f, -4.5426052317E-01f,
   -4.0715656898E-01f, -3.6969460527E-01f, -3.3908542600E-01f, -3.1511810350E-01f,
   -2.9642226150E-01f, -2.8184548650E-01f, -5.4119610000E-01f, -2.6213228100E-01f, 
   -2.5387916537E-01f, -2.3296291359E-01f, -1.9852728987E-01f, -1.5233534808E-01f, 
   -9.6496400054E-02f, -3.3423828516E-02f, 0.0000000000E+00f, 0.0000000000E+00f,
   0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f },

 { -4.8300800645E-02f, -1.5715656932E-01f, -2.8325045177E-01f, -4.2953747763E-01f, 
   -1.2071067795E+00f, -8.2426483178E-01f, -1.1451749106E+00f, -1.7695290101E+00f,
   -4.5470225061E+00f, -3.4890531002E+00f, -7.3296292804E-01f, -1.5076514758E-01f, 
   0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f,
   0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f,
   0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f,
   0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f, 
   0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f,
   0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f },

 { 0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f, 0.0000000000E+00f,
   0.0000000000E+00f, 0.0000000000E+00f, -1.5076513660E-01f, -7.3296291107E-01f,
   -3.4890530566E+00f, -4.5470224727E+00f, -1.7695290031E+00f, -1.1451749092E+00f,
   -8.3137738100E-01f, -1.3065629650E+00f, -5.4142014250E-01f, -4.6528974900E-01f,
   -4.1066990750E-01f, -3.7004680800E-01f, -3.3876269197E-01f, -3.1242222492E-01f,
   -2.8939587111E-01f, -2.6880081906E-01f, -5.0000000266E-01f, -2.3251417468E-01f,
   -2.1596714708E-01f, -2.0004979098E-01f, -1.8449493497E-01f, -1.6905846094E-01f,
   -1.5350360518E-01f, -1.3758624925E-01f, -1.2103922149E-01f, -2.0710679058E-01f,
   -8.4752577594E-02f, -6.4157525656E-02f, -4.1131172614E-02f, -1.4790705759E-02f }
};

void inv_mdct(real *in, real *out, int32 block_type)
{
	 real    tmp[18];
	 real    *win_bt;
    int32   i;

	 if(block_type == 2){

	   for(int32 p=0;p<36;p+=9) {
	   	out[p]   = out[p+1] = out[p+2] = out[p+3] =
	      out[p+4] = out[p+5] = out[p+6] = out[p+7] =
	      out[p+8] = 0.0f;
	   }

    	int32 six_i = 0;

	   for(i=0;i<3;i++)
   	{

      	// 12 point IMDCT

         // Begin 12 point IDCT

   		// Input aliasing for 12 pt IDCT
	   	in[15+i] += in[12+i]; in[12+i] += in[9+i]; in[9+i]  +=  in[6+i];
   		in[6+i]  += in[3+i];  in[3+i]  += in[0+i];

	   	// Input aliasing on odd indices (for 6 point IDCT)
   		in[15+i] += in[9+i];  in[9+i]  += in[3+i];

	   	// 3 point IDCT on even indices

         {
	  	   real 	pp1, pp2, sum;
			pp2 = in[12+i] * 0.500000000f;
	   	pp1 = in[ 6+i] * 0.866025403f;
	   	sum = in[0+i] + pp2;
	   	tmp[1] = in[0+i] - in[12+i];
	   	tmp[0] = sum + pp1;
	   	tmp[2] = sum - pp1;

      	// End 3 point IDCT on even indices

	   	// 3 point IDCT on odd indices (for 6 point IDCT)

			pp2 = in[15+i] * 0.500000000f;
   		pp1 = in[ 9+i] * 0.866025403f;
	   	sum = in[ 3+i] + pp2;
	   	tmp[4] = in[3+i] - in[15+i];
	   	tmp[5] = sum + pp1;
	   	tmp[3] = sum - pp1;
         }

   	   // End 3 point IDCT on odd indices

   		// Twiddle factors on odd indices (for 6 point IDCT)

   		tmp[3] *= 1.931851653f;
   		tmp[4] *= 0.707106781f;
   		tmp[5] *= 0.517638090f;

	   	// Output butterflies on 2 3 point IDCT's (for 6 point IDCT)

         {
   		real save = tmp[0];
   		tmp[0] += tmp[5];
   		tmp[5] = save - tmp[5];
	   	save = tmp[1];
	   	tmp[1] += tmp[4];
	   	tmp[4] = save - tmp[4];
	   	save = tmp[2];
	   	tmp[2] += tmp[3];
	   	tmp[3] = save - tmp[3];
         }

   		// End 6 point IDCT

	   	// Twiddle factors on indices (for 12 point IDCT)

	   	tmp[0]  *=  0.504314480f;
	   	tmp[1]  *=  0.541196100f;
	   	tmp[2]  *=  0.630236207f;
	   	tmp[3]  *=  0.821339815f;
	   	tmp[4]  *=  1.306562965f;
	   	tmp[5]  *=  3.830648788f;

      	// End 12 point IDCT

	   	// Shift to 12 point modified IDCT, multiply by window type 2
	   	tmp[8]  = -tmp[0] * 0.793353340f;
	   	tmp[9]  = -tmp[0] * 0.608761429f;
	   	tmp[7]  = -tmp[1] * 0.923879532f;
	   	tmp[10] = -tmp[1] * 0.382683432f;
	   	tmp[6]  = -tmp[2] * 0.991444861f;
	   	tmp[11] = -tmp[2] * 0.130526192f;

	   	tmp[0]  =  tmp[3];
	   	tmp[1]  =  tmp[4] * 0.382683432f;
	   	tmp[2]  =  tmp[5] * 0.608761429f;

	   	tmp[3]  = -tmp[5] * 0.793353340f;
   		tmp[4]  = -tmp[4] * 0.923879532f;
	   	tmp[5]  = -tmp[0] * 0.991444861f;

	   	tmp[0] *= 0.130526192f;

   		out[six_i + 6]  += tmp[0];
			out[six_i + 7]  += tmp[1];
	   	out[six_i + 8]  += tmp[2];
			out[six_i + 9]  += tmp[3];
   		out[six_i + 10] += tmp[4];
			out[six_i + 11] += tmp[5];
	   	out[six_i + 12] += tmp[6];
			out[six_i + 13] += tmp[7];
	   	out[six_i + 14] += tmp[8];
			out[six_i + 15] += tmp[9];
	   	out[six_i + 16] += tmp[10];
			out[six_i + 17] += tmp[11];

   		six_i += 6;
   	}

	 } else {


   // 36 point IDCT

   // input aliasing for 36 point IDCT
   in[17]+=in[16]; in[16]+=in[15]; in[15]+=in[14]; in[14]+=in[13];
   in[13]+=in[12]; in[12]+=in[11]; in[11]+=in[10]; in[10]+=in[9];
   in[9] +=in[8];  in[8] +=in[7];  in[7] +=in[6];  in[6] +=in[5];
   in[5] +=in[4];  in[4] +=in[3];  in[3] +=in[2];  in[2] +=in[1];
   in[1] +=in[0];

   // 18 point IDCT for odd indices

   // input aliasing for 18 point IDCT
   in[17]+=in[15]; in[15]+=in[13]; in[13]+=in[11]; in[11]+=in[9];
   in[9] +=in[7];  in[7] +=in[5];  in[5] +=in[3];  in[3] +=in[1];

{
   real tmp0,tmp1,tmp2,tmp3,tmp4,tmp0_,tmp1_,tmp2_,tmp3_;
   real tmp0o,tmp1o,tmp2o,tmp3o,tmp4o,tmp0_o,tmp1_o,tmp2_o,tmp3_o;

// Fast 9 Point Inverse Discrete Cosine Transform
//
// By  Francois-Raymond Boyer
//         mailto:boyerf@iro.umontreal.ca
//         http://www.iro.umontreal.ca/~boyerf
//
// The code has been optimized for Intel processors
//  (takes a lot of time to convert float to and from iternal FPU representation)
//
// It is a simple "factorization" of the IDCT matrix.

   // 9 point IDCT on even indices
   {
// 5 points on odd indices (not realy an IDCT)
   real i0 = in[0]+in[0];
   real i0p12 = i0 + in[12];

   tmp0 = i0p12 + in[4]*1.8793852415718f  + in[8]*1.532088886238f   + in[16]*0.34729635533386f;
   tmp1 = i0    + in[4]                   - in[8] - in[12] - in[12] - in[16];
   tmp2 = i0p12 - in[4]*0.34729635533386f - in[8]*1.8793852415718f  + in[16]*1.532088886238f;
   tmp3 = i0p12 - in[4]*1.532088886238f   + in[8]*0.34729635533386f - in[16]*1.8793852415718f;
   tmp4 = in[0] - in[4]                   + in[8] - in[12]          + in[16];

// 4 points on even indices
   real i6_ = in[6]*1.732050808f;		// Sqrt[3]

   tmp0_ = in[2]*1.9696155060244f  + i6_ + in[10]*1.2855752193731f  + in[14]*0.68404028665134f;
   tmp1_ = (in[2]                        - in[10]                   - in[14])*1.732050808f;
   tmp2_ = in[2]*1.2855752193731f  - i6_ - in[10]*0.68404028665134f + in[14]*1.9696155060244f;
   tmp3_ = in[2]*0.68404028665134f - i6_ + in[10]*1.9696155060244f  - in[14]*1.2855752193731f;
   }

   // 9 point IDCT on odd indices
   {
// 5 points on odd indices (not realy an IDCT)
   real i0 = in[0+1]+in[0+1];
   real i0p12 = i0 + in[12+1];

   tmp0o = i0p12   + in[4+1]*1.8793852415718f  + in[8+1]*1.532088886238f       + in[16+1]*0.34729635533386f;
   tmp1o = i0      + in[4+1]                   - in[8+1] - in[12+1] - in[12+1] - in[16+1];
   tmp2o = i0p12   - in[4+1]*0.34729635533386f - in[8+1]*1.8793852415718f      + in[16+1]*1.532088886238f;
   tmp3o = i0p12   - in[4+1]*1.532088886238f   + in[8+1]*0.34729635533386f     - in[16+1]*1.8793852415718f;
   tmp4o = (in[0+1] - in[4+1]                   + in[8+1] - in[12+1]            + in[16+1])*0.707106781f; // Twiddled

// 4 points on even indices
   real i6_ = in[6+1]*1.732050808f;		// Sqrt[3]

   tmp0_o = in[2+1]*1.9696155060244f  + i6_ + in[10+1]*1.2855752193731f  + in[14+1]*0.68404028665134f;
   tmp1_o = (in[2+1]                        - in[10+1]                   - in[14+1])*1.732050808f;
   tmp2_o = in[2+1]*1.2855752193731f  - i6_ - in[10+1]*0.68404028665134f + in[14+1]*1.9696155060244f;
   tmp3_o = in[2+1]*0.68404028665134f - i6_ + in[10+1]*1.9696155060244f  - in[14+1]*1.2855752193731f;
   }

   // Twiddle factors on odd indices
   // and
   // Butterflies on 9 point IDCT's
   // and
   // twiddle factors for 36 point IDCT

   real e, o;
   e = tmp0 + tmp0_; o = (tmp0o + tmp0_o)*0.501909918f; tmp[0] = e + o;    tmp[17] = e - o;
   e = tmp1 + tmp1_; o = (tmp1o + tmp1_o)*0.517638090f; tmp[1] = e + o;    tmp[16] = e - o;
   e = tmp2 + tmp2_; o = (tmp2o + tmp2_o)*0.551688959f; tmp[2] = e + o;    tmp[15] = e - o;
   e = tmp3 + tmp3_; o = (tmp3o + tmp3_o)*0.610387294f; tmp[3] = e + o;    tmp[14] = e - o;
   tmp[4] = tmp4 + tmp4o; tmp[13] = tmp4 - tmp4o;
   e = tmp3 - tmp3_; o = (tmp3o - tmp3_o)*0.871723397f; tmp[5] = e + o;    tmp[12] = e - o;
   e = tmp2 - tmp2_; o = (tmp2o - tmp2_o)*1.183100792f; tmp[6] = e + o;    tmp[11] = e - o;
   e = tmp1 - tmp1_; o = (tmp1o - tmp1_o)*1.931851653f; tmp[7] = e + o;    tmp[10] = e - o;
   e = tmp0 - tmp0_; o = (tmp0o - tmp0_o)*5.736856623f; tmp[8] = e + o;    tmp[9] =  e - o;
   }

   // end 36 point IDCT */

	// shift to modified IDCT
   win_bt = win[block_type];

	out[0] =-tmp[9]  * win_bt[0];
   out[1] =-tmp[10] * win_bt[1];
	out[2] =-tmp[11] * win_bt[2];
   out[3] =-tmp[12] * win_bt[3];
   out[4] =-tmp[13] * win_bt[4];
	out[5] =-tmp[14] * win_bt[5];
	out[6] =-tmp[15] * win_bt[6];
	out[7] =-tmp[16] * win_bt[7];
	out[8] =-tmp[17] * win_bt[8];

   out[9] = tmp[17] * win_bt[9];
   out[10]= tmp[16] * win_bt[10];
	out[11]= tmp[15] * win_bt[11];
	out[12]= tmp[14] * win_bt[12];
	out[13]= tmp[13] * win_bt[13];
	out[14]= tmp[12] * win_bt[14];
   out[15]= tmp[11] * win_bt[15];
	out[16]= tmp[10] * win_bt[16];
	out[17]= tmp[9]  * win_bt[17];
	out[18]= tmp[8]  * win_bt[18];
   out[19]= tmp[7]  * win_bt[19];
	out[20]= tmp[6]  * win_bt[20];
   out[21]= tmp[5]  * win_bt[21];
	out[22]= tmp[4]  * win_bt[22];
	out[23]= tmp[3]  * win_bt[23];
 	out[24]= tmp[2]  * win_bt[24];
   out[25]= tmp[1]  * win_bt[25];
	out[26]= tmp[0]  * win_bt[26];

   out[27]= tmp[0]  * win_bt[27];
	out[28]= tmp[1]  * win_bt[28];
	out[29]= tmp[2]  * win_bt[29];
	out[30]= tmp[3]  * win_bt[30];
	out[31]= tmp[4]  * win_bt[31];
   out[32]= tmp[5]  * win_bt[32];
	out[33]= tmp[6]  * win_bt[33];
	out[34]= tmp[7]  * win_bt[34];
	out[35]= tmp[8]  * win_bt[35];
	}
}

#ifdef COOLPRO
#pragma optimize("",on)	 // Restore optimizations for VC 5.0
#endif