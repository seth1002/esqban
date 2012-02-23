/* synfilt.cpp

	Synthesis Filter implementation */

/* -- 03/20/97 --
 *  compute_new_v() -- reoptimized with the assumption that constant offsets
 *    to memory are free.  Common subexpression were redone for better
 *    optimization.
 *  compute_pcm_samples() -- reoptimized with constant offsets.
 *
 *  -- Conrad Wei-Li Song (conradsong@mail.utexas.edu)
 */

/*
 *  @(#) synthesis_filter.cc 1.14, last edit: 6/21/94 11:22:20
 *  @(#) Copyright (C) 1993, 1994 Tobias Bading (bading@cs.tu-berlin.de)
 *  @(#) Berlin University of Technology
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 *  Changes from version 1.1 to 1.2:
 *    - compute_new_v() uses a 32 point fast cosine transform as described by
 *      Byeong Gi Lee in IEEE Transactions ASSP-32 Part 2, August 1984,
 *      "A New Algorithm to Compute the Discrete Cosine Transform"
 *      instead of the matrix-vector multiplication in V1.1
 *    - loop unrolling done in compute_pcm_samples()
 *    - if ULAW is defined, the synthesis filter does a downsampling
 *      to 8 kHz by dropping samples and ignoring subbands above 4 kHz
 */

#include <math.h>
#include "all.h"
#include "synfilt.h"

#define MY_PI 3.14159265358979323846

#include "scalfact.h"

#ifdef __WIN32__
#pragma warning (disable: 4244 4305)
#endif

static const real cos1_64  = 1.0 / (2.0 * cos(MY_PI        / 64.0));
static const real cos3_64  = 1.0 / (2.0 * cos(MY_PI * 3.0  / 64.0));
static const real cos5_64  = 1.0 / (2.0 * cos(MY_PI * 5.0  / 64.0));
static const real cos7_64  = 1.0 / (2.0 * cos(MY_PI * 7.0  / 64.0));
static const real cos9_64  = 1.0 / (2.0 * cos(MY_PI * 9.0  / 64.0));
static const real cos11_64 = 1.0 / (2.0 * cos(MY_PI * 11.0 / 64.0));
static const real cos13_64 = 1.0 / (2.0 * cos(MY_PI * 13.0 / 64.0));
static const real cos15_64 = 1.0 / (2.0 * cos(MY_PI * 15.0 / 64.0));
static const real cos17_64 = 1.0 / (2.0 * cos(MY_PI * 17.0 / 64.0));
static const real cos19_64 = 1.0 / (2.0 * cos(MY_PI * 19.0 / 64.0));
static const real cos21_64 = 1.0 / (2.0 * cos(MY_PI * 21.0 / 64.0));
static const real cos23_64 = 1.0 / (2.0 * cos(MY_PI * 23.0 / 64.0));
static const real cos25_64 = 1.0 / (2.0 * cos(MY_PI * 25.0 / 64.0));
static const real cos27_64 = 1.0 / (2.0 * cos(MY_PI * 27.0 / 64.0));
static const real cos29_64 = 1.0 / (2.0 * cos(MY_PI * 29.0 / 64.0));
static const real cos31_64 = 1.0 / (2.0 * cos(MY_PI * 31.0 / 64.0));
static const real cos1_32  = 1.0 / (2.0 * cos(MY_PI        / 32.0));
static const real cos3_32  = 1.0 / (2.0 * cos(MY_PI * 3.0  / 32.0));
static const real cos5_32  = 1.0 / (2.0 * cos(MY_PI * 5.0  / 32.0));
static const real cos7_32  = 1.0 / (2.0 * cos(MY_PI * 7.0  / 32.0));
static const real cos9_32  = 1.0 / (2.0 * cos(MY_PI * 9.0  / 32.0));
static const real cos11_32 = 1.0 / (2.0 * cos(MY_PI * 11.0 / 32.0));
static const real cos13_32 = 1.0 / (2.0 * cos(MY_PI * 13.0 / 32.0));
static const real cos15_32 = 1.0 / (2.0 * cos(MY_PI * 15.0 / 32.0));
static const real cos1_16  = 1.0 / (2.0 * cos(MY_PI        / 16.0));
static const real cos3_16  = 1.0 / (2.0 * cos(MY_PI * 3.0  / 16.0));
static const real cos5_16  = 1.0 / (2.0 * cos(MY_PI * 5.0  / 16.0));
static const real cos7_16  = 1.0 / (2.0 * cos(MY_PI * 7.0  / 16.0));
static const real cos1_8   = 1.0 / (2.0 * cos(MY_PI        / 8.0));
static const real cos3_8   = 1.0 / (2.0 * cos(MY_PI * 3.0  / 8.0));
static const real cos1_4   = 1.0 / (2.0 * cos(MY_PI / 4.0));


const real SynthesisFilter::d[512] = {
	// Note: These values are not in the same order
	// as in Annex 3-B.3 of the ISO/IEC DIS 11172-3
	0.000000000, -0.000442505,  0.003250122, -0.007003784,
	0.031082153, -0.078628540,  0.100311279, -0.572036743,
	1.144989014,  0.572036743,  0.100311279,  0.078628540,
	0.031082153,  0.007003784,  0.003250122,  0.000442505,
	-0.000015259, -0.000473022,  0.003326416, -0.007919312,
	0.030517578, -0.084182739,  0.090927124, -0.600219727,
	1.144287109,  0.543823242,  0.108856201,  0.073059082,
	0.031478882,  0.006118774,  0.003173828,  0.000396729,
  -0.000015259, -0.000534058,  0.003387451, -0.008865356,
	0.029785156, -0.089706421,  0.080688477, -0.628295898,
	1.142211914,  0.515609741,  0.116577148,  0.067520142,
	 0.031738281,  0.005294800,  0.003082275,  0.000366211,
	-0.000015259, -0.000579834,  0.003433228, -0.009841919,
	 0.028884888, -0.095169067,  0.069595337, -0.656219482,
	1.138763428,  0.487472534,  0.123474121,  0.061996460,
   0.031845093,  0.004486084,  0.002990723,  0.000320435,
  -0.000015259, -0.000625610,  0.003463745, -0.010848999,
	 0.027801514, -0.100540161,  0.057617188, -0.683914185,
	1.133926392,  0.459472656,  0.129577637,  0.056533813,
	0.031814575,  0.003723145,  0.002899170,  0.000289917,
  -0.000015259, -0.000686646,  0.003479004, -0.011886597,
	0.026535034, -0.105819702,  0.044784546, -0.711318970,
	1.127746582,  0.431655884,  0.134887695,  0.051132202,
	0.031661987,  0.003005981,  0.002792358,  0.000259399,
	-0.000015259, -0.000747681,  0.003479004, -0.012939453,
	0.025085449, -0.110946655,  0.031082153, -0.738372803,
   1.120223999,  0.404083252,  0.139450073,  0.045837402,
   0.031387329,  0.002334595,  0.002685547,  0.000244141,
  -0.000030518, -0.000808716,  0.003463745, -0.014022827,
   0.023422241, -0.115921021,  0.016510010, -0.765029907,
	1.111373901,  0.376800537,  0.143264771,  0.040634155,
   0.031005859,  0.001693726,  0.002578735,  0.000213623,
	-0.000030518, -0.000885010,  0.003417969, -0.015121460,
	0.021575928, -0.120697021,  0.001068115, -0.791213989,
   1.101211548,  0.349868774,  0.146362305,  0.035552979,
	0.030532837,  0.001098633,  0.002456665,  0.000198364,
	-0.000030518, -0.000961304,  0.003372192, -0.016235352,
   0.019531250, -0.125259399, -0.015228271, -0.816864014,
	 1.089782715,  0.323318481,  0.148773193,  0.030609131,
	0.029937744,  0.000549316,  0.002349854,  0.000167847,
	-0.000030518, -0.001037598,  0.003280640, -0.017349243,
	0.017257690, -0.129562378, -0.032379150, -0.841949463,
   1.077117920,  0.297210693,  0.150497437,  0.025817871,
   0.029281616,  0.000030518,  0.002243042,  0.000152588,
  -0.000045776, -0.001113892,  0.003173828, -0.018463135,
	0.014801025, -0.133590698, -0.050354004, -0.866363525,
	1.063217163,  0.271591187,  0.151596069,  0.021179199,
	0.028533936, -0.000442505,  0.002120972,  0.000137329,
	-0.000045776, -0.001205444,  0.003051758, -0.019577026,
	0.012115479, -0.137298584, -0.069168091, -0.890090942,
	 1.048156738,  0.246505737,  0.152069092,  0.016708374,
	0.027725220, -0.000869751,  0.002014160,  0.000122070,
  -0.000061035, -0.001296997,  0.002883911, -0.020690918,
   0.009231567, -0.140670776, -0.088775635, -0.913055420,
	 1.031936646,  0.221984863,  0.151962280,  0.012420654,
   0.026840210, -0.001266479,  0.001907349,  0.000106812,
  -0.000061035, -0.001388550,  0.002700806, -0.021789551,
	0.006134033, -0.143676758, -0.109161377, -0.935195923,
   1.014617920,  0.198059082,  0.151306152,  0.008316040,
	0.025909424, -0.001617432,  0.001785278,  0.000106812,
	-0.000076294, -0.001480103,  0.002487183, -0.022857666,
	 0.002822876, -0.146255493, -0.130310059, -0.956481934,
	0.996246338,  0.174789429,  0.150115967,  0.004394531,
   0.024932861, -0.001937866,  0.001693726,  0.000091553,
  -0.000076294, -0.001586914,  0.002227783, -0.023910522,
	-0.000686646, -0.148422241, -0.152206421, -0.976852417,
   0.976852417,  0.152206421,  0.148422241,  0.000686646,
	0.023910522, -0.002227783,  0.001586914,  0.000076294,
	-0.000091553, -0.001693726,  0.001937866, -0.024932861,
	-0.004394531, -0.150115967, -0.174789429, -0.996246338,
   0.956481934,  0.130310059,  0.146255493, -0.002822876,
   0.022857666, -0.002487183,  0.001480103,  0.000076294,
	-0.000106812, -0.001785278,  0.001617432, -0.025909424,
  -0.008316040, -0.151306152, -0.198059082, -1.014617920,
   0.935195923,  0.109161377,  0.143676758, -0.006134033,
   0.021789551, -0.002700806,  0.001388550,  0.000061035,
  -0.000106812, -0.001907349,  0.001266479, -0.026840210,
	-0.012420654, -0.151962280, -0.221984863, -1.031936646,
	0.913055420,  0.088775635,  0.140670776, -0.009231567,
	 0.020690918, -0.002883911,  0.001296997,  0.000061035,
	-0.000122070, -0.002014160,  0.000869751, -0.027725220,
  -0.016708374, -0.152069092, -0.246505737, -1.048156738,
   0.890090942,  0.069168091,  0.137298584, -0.012115479,
	0.019577026, -0.003051758,  0.001205444,  0.000045776,
	-0.000137329, -0.002120972,  0.000442505, -0.028533936,
  -0.021179199, -0.151596069, -0.271591187, -1.063217163,
   0.866363525,  0.050354004,  0.133590698, -0.014801025,
   0.018463135, -0.003173828,  0.001113892,  0.000045776,
  -0.000152588, -0.002243042, -0.000030518, -0.029281616,
  -0.025817871, -0.150497437, -0.297210693, -1.077117920,
	 0.841949463,  0.032379150,  0.129562378, -0.017257690,
	 0.017349243, -0.003280640,  0.001037598,  0.000030518,
  -0.000167847, -0.002349854, -0.000549316, -0.029937744,
  -0.030609131, -0.148773193, -0.323318481, -1.089782715,
	0.816864014,  0.015228271,  0.125259399, -0.019531250,
   0.016235352, -0.003372192,  0.000961304,  0.000030518,
	-0.000198364, -0.002456665, -0.001098633, -0.030532837,
	-0.035552979, -0.146362305, -0.349868774, -1.101211548,
	 0.791213989, -0.001068115,  0.120697021, -0.021575928,
	0.015121460, -0.003417969,  0.000885010,  0.000030518,
  -0.000213623, -0.002578735, -0.001693726, -0.031005859,
	-0.040634155, -0.143264771, -0.376800537, -1.111373901,
   0.765029907, -0.016510010,  0.115921021, -0.023422241,
   0.014022827, -0.003463745,  0.000808716,  0.000030518,
  -0.000244141, -0.002685547, -0.002334595, -0.031387329,
  -0.045837402, -0.139450073, -0.404083252, -1.120223999,
   0.738372803, -0.031082153,  0.110946655, -0.025085449,
	0.012939453, -0.003479004,  0.000747681,  0.000015259,
	-0.000259399, -0.002792358, -0.003005981, -0.031661987,
  -0.051132202, -0.134887695, -0.431655884, -1.127746582,
	0.711318970, -0.044784546,  0.105819702, -0.026535034,
   0.011886597, -0.003479004,  0.000686646,  0.000015259,
	-0.000289917, -0.002899170, -0.003723145, -0.031814575,
	-0.056533813, -0.129577637, -0.459472656, -1.133926392,
   0.683914185, -0.057617188,  0.100540161, -0.027801514,
	0.010848999, -0.003463745,  0.000625610,  0.000015259,
  -0.000320435, -0.002990723, -0.004486084, -0.031845093,
  -0.061996460, -0.123474121, -0.487472534, -1.138763428,
	0.656219482, -0.069595337,  0.095169067, -0.028884888,
	0.009841919, -0.003433228,  0.000579834,  0.000015259,
	-0.000366211, -0.003082275, -0.005294800, -0.031738281,
  -0.067520142, -0.116577148, -0.515609741, -1.142211914,
	0.628295898, -0.080688477,  0.089706421, -0.029785156,
	0.008865356, -0.003387451,  0.000534058,  0.000015259,
  -0.000396729, -0.003173828, -0.006118774, -0.031478882,
  -0.073059082, -0.108856201, -0.543823242, -1.144287109,
	0.600219727, -0.090927124,  0.084182739, -0.030517578,
	0.007919312, -0.003326416,  0.000473022,  0.000015259
};

SynthesisFilter::SynthesisFilter(uint32 channelnumber, real factor)
{
  channel = channelnumber;
  scalefactor = factor;

  reset();
}

void SynthesisFilter::reset()
{
  register real *floatp, *floatp2;

  // initialize v1[] and v2[]:
  for (floatp = v1 + 512, floatp2 = v2 + 512; floatp > v1; )
	 *--floatp = *--floatp2 = 0.0;

  // initialize samples[]:
  for (floatp = samples + 32; floatp > samples; )
	 *--floatp = 0.0;

  actual_v = v1;
  actual_write_pos = 15;
}

void SynthesisFilter::compute_new_v()
{
	real new_v[32];         // new V[0-15] and V[33-48] of Figure 3-A.2 in ISO DIS 11172-3
	real p[16];
	real pp[16];

	// compute new values via a fast cosine transform:
	{
	register real *x1 = samples;

	p[0] = x1[0] + x1[31];
	p[1] = x1[1] + x1[30];
	p[2] = x1[2] + x1[29];
	p[3] = x1[3] + x1[28];
	p[4] = x1[4] + x1[27];
	p[5] = x1[5] + x1[26];
	p[6] = x1[6] + x1[25];
	p[7] = x1[7] + x1[24];
	p[8] = x1[8] + x1[23];
	p[9] = x1[9] + x1[22];
	p[10] = x1[10] + x1[21];
	p[11] = x1[11] + x1[20];
	p[12] = x1[12] + x1[19];
	p[13] = x1[13] + x1[18];
	p[14] = x1[14] + x1[17];
	p[15] = x1[15] + x1[16];
	}

	{
	pp[0] = p[0] + p[15];
	pp[1] = p[1] + p[14];
	pp[2] = p[2] + p[13];
	pp[3] = p[3] + p[12];
	pp[4] = p[4] + p[11];
	pp[5] = p[5] + p[10];
	pp[6] = p[6] + p[9];
	pp[7] = p[7] + p[8];
	pp[8] = (p[0] - p[15]) * cos1_32;
	pp[9] = (p[1] - p[14]) * cos3_32;
	pp[10] = (p[2] - p[13]) * cos5_32;
	pp[11] = (p[3] - p[12]) * cos7_32;
	pp[12] = (p[4] - p[11]) * cos9_32;
	pp[13] = (p[5] - p[10]) * cos11_32;
	pp[14] = (p[6] - p[9]) * cos13_32;
	pp[15] = (p[7] - p[8]) * cos15_32;
	}

	{
	p[0] = pp[0] + pp[7];
	p[1] = pp[1] + pp[6];
	p[2] = pp[2] + pp[5];
	p[3] = pp[3] + pp[4];
	p[4] = (pp[0] - pp[7]) * cos1_16;
	p[5] = (pp[1] - pp[6]) * cos3_16;
	p[6] = (pp[2] - pp[5]) * cos5_16;
	p[7] = (pp[3] - pp[4]) * cos7_16;
	p[8] = pp[8] + pp[15];
	p[9] = pp[9] + pp[14];
	p[10] = pp[10] + pp[13];
	p[11] = pp[11] + pp[12];
	p[12] = (pp[8] - pp[15]) * cos1_16;
	p[13] = (pp[9] - pp[14]) * cos3_16;
	p[14] = (pp[10] - pp[13]) * cos5_16;
	p[15] = (pp[11] - pp[12]) * cos7_16;
	}

	{
	pp[0] = p[0] + p[3];
	pp[1] = p[1] + p[2];
	pp[2] = (p[0] - p[3]) * cos1_8;
	pp[3] = (p[1] - p[2]) * cos3_8;
	pp[4] = p[4] + p[7];
	pp[5] = p[5] + p[6];
	pp[6] = (p[4] - p[7]) * cos1_8;
	pp[7] = (p[5] - p[6]) * cos3_8;
	pp[8] = p[8] + p[11];
	pp[9] = p[9] + p[10];
	pp[10] = (p[8] - p[11]) * cos1_8;
	pp[11] = (p[9] - p[10]) * cos3_8;
	pp[12] = p[12] + p[15];
	pp[13] = p[13] + p[14];
	pp[14] = (p[12] - p[15]) * cos1_8;
	pp[15] = (p[13] - p[14]) * cos3_8;
	}

	{
	p[0] = pp[0] + pp[1];
	p[1] = (pp[0] - pp[1]) * cos1_4;
	p[2] = pp[2] + pp[3];
	p[3] = (pp[2] - pp[3]) * cos1_4;
	p[4] = pp[4] + pp[5];
	p[5] = (pp[4] - pp[5]) * cos1_4;
	p[6] = pp[6] + pp[7];
	p[7] = (pp[6] - pp[7]) * cos1_4;
	p[8] = pp[8] + pp[9];
	p[9] = (pp[8] - pp[9]) * cos1_4;
	p[10] = pp[10] + pp[11];
	p[11] = (pp[10] - pp[11]) * cos1_4;
	p[12] = pp[12] + pp[13];
	p[13] = (pp[12] - pp[13]) * cos1_4;
	p[14] = pp[14] + pp[15];
	p[15] = (pp[14] - pp[15]) * cos1_4;
	}

	{
	// this is pretty insane coding
	register real tmp1;
	new_v[36-17] = -(new_v[4] = (new_v[12] = p[7]) + p[5]) - p[6];
	new_v[44-17] = -p[6] - p[7] - p[4];
	new_v[6] = (new_v[10] = (new_v[14] = p[15]) + p[11]) + p[13];
	new_v[34-17] = -(new_v[2] = p[15] + p[13] + p[9]) - p[14];
	new_v[38-17] = (tmp1 = -p[14] - p[15] - p[10] - p[11]) - p[13];
	new_v[46-17] = -p[14] - p[15] - p[12] - p[8];
	new_v[42-17] = tmp1 - p[12];
	new_v[48-17] = -p[0];
	new_v[0] = p[1];
	new_v[40-17] = -(new_v[8] = p[3]) - p[2];
	}

	{
	register real *x1 = samples;

	p[0] = (x1[0] - x1[31]) * cos1_64;
	p[1] = (x1[1] - x1[30]) * cos3_64;
	p[2] = (x1[2] - x1[29]) * cos5_64;
	p[3] = (x1[3] - x1[28]) * cos7_64;
	p[4] = (x1[4] - x1[27]) * cos9_64;
	p[5] = (x1[5] - x1[26]) * cos11_64;
	p[6] = (x1[6] - x1[25]) * cos13_64;
	p[7] = (x1[7] - x1[24]) * cos15_64;
	p[8] = (x1[8] - x1[23]) * cos17_64;
	p[9] = (x1[9] - x1[22]) * cos19_64;
	p[10] = (x1[10] - x1[21]) * cos21_64;
	p[11] = (x1[11] - x1[20]) * cos23_64;
	p[12] = (x1[12] - x1[19]) * cos25_64;
	p[13] = (x1[13] - x1[18]) * cos27_64;
	p[14] = (x1[14] - x1[17]) * cos29_64;
	p[15] = (x1[15] - x1[16]) * cos31_64;
	}

	{
	pp[0] = p[0] + p[15];
	pp[1] = p[1] + p[14];
	pp[2] = p[2] + p[13];
	pp[3] = p[3] + p[12];
	pp[4] = p[4] + p[11];
	pp[5] = p[5] + p[10];
	pp[6] = p[6] + p[9];
	pp[7] = p[7] + p[8];
	pp[8] = (p[0] - p[15]) * cos1_32;
	pp[9] = (p[1] - p[14]) * cos3_32;
	pp[10] = (p[2] - p[13]) * cos5_32;
	pp[11] = (p[3] - p[12]) * cos7_32;
	pp[12] = (p[4] - p[11]) * cos9_32;
	pp[13] = (p[5] - p[10]) * cos11_32;
	pp[14] = (p[6] - p[9]) * cos13_32;
	pp[15] = (p[7] - p[8]) * cos15_32;
	}

	{
	p[0] = pp[0] + pp[7];
	p[1] = pp[1] + pp[6];
	p[2] = pp[2] + pp[5];
	p[3] = pp[3] + pp[4];
	p[4] = (pp[0] - pp[7]) * cos1_16;
	p[5] = (pp[1] - pp[6]) * cos3_16;
	p[6] = (pp[2] - pp[5]) * cos5_16;
	p[7] = (pp[3] - pp[4]) * cos7_16;
	p[8] = pp[8] + pp[15];
	p[9] = pp[9] + pp[14];
	p[10] = pp[10] + pp[13];
	p[11] = pp[11] + pp[12];
	p[12] = (pp[8] - pp[15]) * cos1_16;
	p[13] = (pp[9] - pp[14]) * cos3_16;
	p[14] = (pp[10] - pp[13]) * cos5_16;
	p[15] = (pp[11] - pp[12]) * cos7_16;
	}

	{
	pp[0] = p[0] + p[3];
	pp[1] = p[1] + p[2];
	pp[2] = (p[0] - p[3]) * cos1_8;
	pp[3] = (p[1] - p[2]) * cos3_8;
	pp[4] = p[4] + p[7];
	pp[5] = p[5] + p[6];
	pp[6] = (p[4] - p[7]) * cos1_8;
	pp[7] = (p[5] - p[6]) * cos3_8;
	pp[8] = p[8] + p[11];
	pp[9] = p[9] + p[10];
	pp[10] = (p[8] - p[11]) * cos1_8;
	pp[11] = (p[9] - p[10]) * cos3_8;
	pp[12] = p[12] + p[15];
	pp[13] = p[13] + p[14];
	pp[14] = (p[12] - p[15]) * cos1_8;
	pp[15] = (p[13] - p[14]) * cos3_8;
	}

	{
	p[0] = pp[0] + pp[1];
	p[1] = (pp[0] - pp[1]) * cos1_4;
	p[2] = pp[2] + pp[3];
	p[3] = (pp[2] - pp[3]) * cos1_4;
	p[4] = pp[4] + pp[5];
	p[5] = (pp[4] - pp[5]) * cos1_4;
	p[6] = pp[6] + pp[7];
	p[7] = (pp[6] - pp[7]) * cos1_4;
	p[8] = pp[8] + pp[9];
	p[9] = (pp[8] - pp[9]) * cos1_4;
	p[10] = pp[10] + pp[11];
	p[11] = (pp[10] - pp[11]) * cos1_4;
	p[12] = pp[12] + pp[13];
	p[13] = (pp[12] - pp[13]) * cos1_4;
	p[14] = pp[14] + pp[15];
	p[15] = (pp[14] - pp[15]) * cos1_4;
	}

	{
	// manually doing something that a compiler should handle sucks
	// coding like this is hard to read
	register real tmp1, tmp2;
	new_v[5] = (new_v[11] = (new_v[13] = (new_v[15] = p[15]) + p[7]) + p[11])
							+ p[5] + p[13];
	new_v[7] = (new_v[9] = p[15] + p[11] + p[3]) + p[13];
	new_v[33-17] = -(new_v[1] = (tmp1 = p[13] + p[15] + p[9]) + p[1]) - p[14];
	new_v[35-17] = -(new_v[3] = tmp1 + p[5] + p[7]) - p[6] - p[14];

	new_v[39-17] = (tmp1 = -p[10] - p[11] - p[14] - p[15])
									- p[13] - p[2] - p[3];
	new_v[37-17] = tmp1 - p[13] - p[5] - p[6] - p[7];
	new_v[41-17] = tmp1 - p[12] - p[2] - p[3];
	new_v[43-17] = tmp1 - p[12] - (tmp2 = p[4] + p[6] + p[7]);
	new_v[47-17] = (tmp1 = -p[8] - p[12] - p[14] - p[15]) - p[0];
	new_v[45-17] = tmp1 - tmp2;
	}

	{
	// insert V[0-15] (== new_v[0-15]) into actual v:
	register real *x1 = new_v;
	register real *x2 = actual_v + actual_write_pos;
	x2[0] = x1[0];
	x2[16] = x1[1];
	x2[32] = x1[2];
	x2[48] = x1[3];
	x2[64] = x1[4];
	x2[80] = x1[5];
	x2[96] = x1[6];
	x2[112] = x1[7];
	x2[128] = x1[8];
	x2[144] = x1[9];
	x2[160] = x1[10];
	x2[176] = x1[11];
	x2[192] = x1[12];
	x2[208] = x1[13];
	x2[224] = x1[14];
	x2[240] = x1[15];

	// V[16] is always 0.0:
	x2[256] = 0.0f;

	// insert V[17-31] (== -new_v[15-1]) into actual v:
	x2[272] = -x1[15];
	x2[288] = -x1[14];
	x2[304] = -x1[13];
	x2[320] = -x1[12];
	x2[336] = -x1[11];
	x2[352] = -x1[10];
	x2[368] = -x1[9];
	x2[384] = -x1[8];
	x2[400] = -x1[7];
	x2[416] = -x1[6];
	x2[432] = -x1[5];
	x2[448] = -x1[4];
	x2[464] = -x1[3];
	x2[480] = -x1[2];
	x2[496] = -x1[1];

	// insert V[32] (== -new_v[0]) into other v:
	x2 = (actual_v == v1 ? v2 : v1) + actual_write_pos;
	x2[0] = -x1[0];

	// insert V[33-48] (== new_v[16-31]) into other v:
	x2[16] = x1[16];
	x2[32] = x1[17];
	x2[48] = x1[18];
	x2[64] = x1[19];
	x2[80] = x1[20];
	x2[96] = x1[21];
	x2[112] = x1[22];
	x2[128] = x1[23];
	x2[144] = x1[24];
	x2[160] = x1[25];
	x2[176] = x1[26];
	x2[192] = x1[27];
	x2[208] = x1[28];
	x2[224] = x1[29];
	x2[240] = x1[30];
	x2[256] = x1[31];

	// insert V[49-63] (== new_v[30-16]) into other v:
	x2[272] = x1[30];
	x2[288] = x1[29];
	x2[304] = x1[28];
	x2[320] = x1[27];
	x2[336] = x1[26];
	x2[352] = x1[25];
	x2[368] = x1[24];
	x2[384] = x1[23];
	x2[400] = x1[22];
	x2[416] = x1[21];
	x2[432] = x1[20];
	x2[448] = x1[19];
	x2[464] = x1[18];
	x2[480] = x1[17];
	x2[496] = x1[16];
	}
}

void SynthesisFilter::compute_pcm_samples(Obuffer *buffer)
{
	// scoping variables makes it easier to optimize for the compiler
	register real *vp = actual_v;
	//real a[32];
	//real * pa=a;
	// switch depending on the value for actual_write_pos
	switch (actual_write_pos) {
		case 0: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[0] * dp[0]) +
					(vp[15] * dp[1]) +
					(vp[14] * dp[2]) +
					(vp[13] * dp[3]) +
					(vp[12] * dp[4]) +
					(vp[11] * dp[5]) +
					(vp[10] * dp[6]) +
					(vp[9] * dp[7]) +
					(vp[8] * dp[8]) +
					(vp[7] * dp[9]) +
					(vp[6] * dp[10]) +
					(vp[5] * dp[11]) +
					(vp[4] * dp[12]) +
					(vp[3] * dp[13]) +
					(vp[2] * dp[14]) +
					(vp[1] * dp[15])
					) /* * scalefactor */);
            buffer->append (channel, pcm_sample);
			} // for
		} break; // case 0:

		case 1: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[1] * dp[0]) +
					(vp[0] * dp[1]) +
					(vp[15] * dp[2]) +
					(vp[14] * dp[3]) +
					(vp[13] * dp[4]) +
					(vp[12] * dp[5]) +
					(vp[11] * dp[6]) +
					(vp[10] * dp[7]) +
					(vp[9] * dp[8]) +
					(vp[8] * dp[9]) +
					(vp[7] * dp[10]) +
					(vp[6] * dp[11]) +
					(vp[5] * dp[12]) +
					(vp[4] * dp[13]) +
					(vp[3] * dp[14]) +
					(vp[2] * dp[15])
					) /* * scalefactor */);

            buffer->append (channel, pcm_sample);
         } // for
		} break; // case 1:

		case 2: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[2] * dp[0]) +
					(vp[1] * dp[1]) +
					(vp[0] * dp[2]) +
					(vp[15] * dp[3]) +
					(vp[14] * dp[4]) +
					(vp[13] * dp[5]) +
					(vp[12] * dp[6]) +
					(vp[11] * dp[7]) +
					(vp[10] * dp[8]) +
					(vp[9] * dp[9]) +
					(vp[8] * dp[10]) +
					(vp[7] * dp[11]) +
					(vp[6] * dp[12]) +
					(vp[5] * dp[13]) +
					(vp[4] * dp[14]) +
					(vp[3] * dp[15])
					) /* * scalefactor */);

            buffer->append (channel, pcm_sample);
			} // for
		} break; // case 2:

		case 3: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[3] * dp[0]) +
					(vp[2] * dp[1]) +
					(vp[1] * dp[2]) +
					(vp[0] * dp[3]) +
					(vp[15] * dp[4]) +
					(vp[14] * dp[5]) +
					(vp[13] * dp[6]) +
					(vp[12] * dp[7]) +
					(vp[11] * dp[8]) +
					(vp[10] * dp[9]) +
					(vp[9] * dp[10]) +
					(vp[8] * dp[11]) +
					(vp[7] * dp[12]) +
					(vp[6] * dp[13]) +
					(vp[5] * dp[14]) +
					(vp[4] * dp[15])
					) /* * scalefactor */);

				buffer->append (channel, pcm_sample);
			} // for
		} break; // case 3:

		case 4: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[4] * dp[0]) +
					(vp[3] * dp[1]) +
					(vp[2] * dp[2]) +
					(vp[1] * dp[3]) +
					(vp[0] * dp[4]) +
					(vp[15] * dp[5]) +
					(vp[14] * dp[6]) +
					(vp[13] * dp[7]) +
					(vp[12] * dp[8]) +
					(vp[11] * dp[9]) +
					(vp[10] * dp[10]) +
					(vp[9] * dp[11]) +
					(vp[8] * dp[12]) +
					(vp[7] * dp[13]) +
					(vp[6] * dp[14]) +
					(vp[5] * dp[15])
					) /* * scalefactor */);

				buffer->append (channel, pcm_sample);
			} // for
		} break; // case 4:

		case 5: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[5] * dp[0]) +
					(vp[4] * dp[1]) +
					(vp[3] * dp[2]) +
					(vp[2] * dp[3]) +
					(vp[1] * dp[4]) +
					(vp[0] * dp[5]) +
					(vp[15] * dp[6]) +
					(vp[14] * dp[7]) +
					(vp[13] * dp[8]) +
					(vp[12] * dp[9]) +
					(vp[11] * dp[10]) +
					(vp[10] * dp[11]) +
					(vp[9] * dp[12]) +
					(vp[8] * dp[13]) +
					(vp[7] * dp[14]) +
					(vp[6] * dp[15])
					) /* * scalefactor */);

				buffer->append (channel, pcm_sample);
			} // for
		} break; // case 5:

		case 6: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[6] * dp[0]) +
					(vp[5] * dp[1]) +
					(vp[4] * dp[2]) +
					(vp[3] * dp[3]) +
					(vp[2] * dp[4]) +
					(vp[1] * dp[5]) +
					(vp[0] * dp[6]) +
					(vp[15] * dp[7]) +
					(vp[14] * dp[8]) +
					(vp[13] * dp[9]) +
					(vp[12] * dp[10]) +
					(vp[11] * dp[11]) +
					(vp[10] * dp[12]) +
					(vp[9] * dp[13]) +
					(vp[8] * dp[14]) +
					(vp[7] * dp[15])
					) /* * scalefactor */);

            buffer->append (channel, pcm_sample);
			} // for
		} break; // case 6:

		case 7: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[7] * dp[0]) +
					(vp[6] * dp[1]) +
					(vp[5] * dp[2]) +
					(vp[4] * dp[3]) +
					(vp[3] * dp[4]) +
					(vp[2] * dp[5]) +
					(vp[1] * dp[6]) +
					(vp[0] * dp[7]) +
					(vp[15] * dp[8]) +
					(vp[14] * dp[9]) +
					(vp[13] * dp[10]) +
					(vp[12] * dp[11]) +
					(vp[11] * dp[12]) +
					(vp[10] * dp[13]) +
					(vp[9] * dp[14]) +
					(vp[8] * dp[15])
					) /* * scalefactor */);

            buffer->append (channel, pcm_sample);
			} // for
		} break; // case 7:

		case 8: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[8] * dp[0]) +
					(vp[7] * dp[1]) +
					(vp[6] * dp[2]) +
					(vp[5] * dp[3]) +
					(vp[4] * dp[4]) +
					(vp[3] * dp[5]) +
					(vp[2] * dp[6]) +
					(vp[1] * dp[7]) +
					(vp[0] * dp[8]) +
					(vp[15] * dp[9]) +
					(vp[14] * dp[10]) +
					(vp[13] * dp[11]) +
					(vp[12] * dp[12]) +
					(vp[11] * dp[13]) +
					(vp[10] * dp[14]) +
					(vp[9] * dp[15])
					) /* * scalefactor */);

            buffer->append (channel, pcm_sample);
			} // for
		} break; // case 8:

		case 9: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[9] * dp[0]) +
					(vp[8] * dp[1]) +
					(vp[7] * dp[2]) +
					(vp[6] * dp[3]) +
					(vp[5] * dp[4]) +
					(vp[4] * dp[5]) +
					(vp[3] * dp[6]) +
					(vp[2] * dp[7]) +
					(vp[1] * dp[8]) +
					(vp[0] * dp[9]) +
					(vp[15] * dp[10]) +
					(vp[14] * dp[11]) +
					(vp[13] * dp[12]) +
					(vp[12] * dp[13]) +
					(vp[11] * dp[14]) +
					(vp[10] * dp[15])
					) /* * scalefactor */);

            buffer->append (channel, pcm_sample);
			} // for
		} break; // case 9:

		case 10: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[10] * dp[0]) +
					(vp[9] * dp[1]) +
					(vp[8] * dp[2]) +
					(vp[7] * dp[3]) +
					(vp[6] * dp[4]) +
					(vp[5] * dp[5]) +
					(vp[4] * dp[6]) +
					(vp[3] * dp[7]) +
					(vp[2] * dp[8]) +
					(vp[1] * dp[9]) +
					(vp[0] * dp[10]) +
					(vp[15] * dp[11]) +
					(vp[14] * dp[12]) +
					(vp[13] * dp[13]) +
					(vp[12] * dp[14]) +
					(vp[11] * dp[15])
					) /* * scalefactor */);

            buffer->append (channel, pcm_sample);
			} // for
		} break; // case 10:

		case 11: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[11] * dp[0]) +
					(vp[10] * dp[1]) +
					(vp[9] * dp[2]) +
					(vp[8] * dp[3]) +
					(vp[7] * dp[4]) +
					(vp[6] * dp[5]) +
					(vp[5] * dp[6]) +
					(vp[4] * dp[7]) +
					(vp[3] * dp[8]) +
					(vp[2] * dp[9]) +
					(vp[1] * dp[10]) +
					(vp[0] * dp[11]) +
					(vp[15] * dp[12]) +
					(vp[14] * dp[13]) +
					(vp[13] * dp[14]) +
					(vp[12] * dp[15])
					) /* * scalefactor */);

            buffer->append (channel, pcm_sample);
			} // for
		} break; // case 11:

		case 12: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[12] * dp[0]) +
					(vp[11] * dp[1]) +
					(vp[10] * dp[2]) +
					(vp[9] * dp[3]) +
					(vp[8] * dp[4]) +
					(vp[7] * dp[5]) +
					(vp[6] * dp[6]) +
					(vp[5] * dp[7]) +
					(vp[4] * dp[8]) +
					(vp[3] * dp[9]) +
					(vp[2] * dp[10]) +
					(vp[1] * dp[11]) +
					(vp[0] * dp[12]) +
					(vp[15] * dp[13]) +
					(vp[14] * dp[14]) +
					(vp[13] * dp[15])
					) /* * scalefactor */);

            buffer->append (channel, pcm_sample);
			} // for
		} break; // case 12:

		case 13: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[13] * dp[0]) +
					(vp[12] * dp[1]) +
					(vp[11] * dp[2]) +
					(vp[10] * dp[3]) +
					(vp[9] * dp[4]) +
					(vp[8] * dp[5]) +
					(vp[7] * dp[6]) +
					(vp[6] * dp[7]) +
					(vp[5] * dp[8]) +
					(vp[4] * dp[9]) +
					(vp[3] * dp[10]) +
					(vp[2] * dp[11]) +
					(vp[1] * dp[12]) +
					(vp[0] * dp[13]) +
					(vp[15] * dp[14]) +
					(vp[14] * dp[15])
					) /* * scalefactor */);

            buffer->append (channel, pcm_sample);
			} // for
		} break; // case 13:

		case 14: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[14] * dp[0]) +
					(vp[13] * dp[1]) +
					(vp[12] * dp[2]) +
					(vp[11] * dp[3]) +
					(vp[10] * dp[4]) +
					(vp[9] * dp[5]) +
					(vp[8] * dp[6]) +
					(vp[7] * dp[7]) +
					(vp[6] * dp[8]) +
					(vp[5] * dp[9]) +
					(vp[4] * dp[10]) +
					(vp[3] * dp[11]) +
					(vp[2] * dp[12]) +
					(vp[1] * dp[13]) +
					(vp[0] * dp[14]) +
					(vp[15] * dp[15])
					) /* * scalefactor */);

            buffer->append (channel, pcm_sample);
			} // for
		} break; // case 14:

		case 15: {
			// fat chance of having this loop unroll
			for( register const real *dp = d;
					 dp < d + (32 * 16);
					 dp += 16, vp += 16 ) {
				register real pcm_sample;

				pcm_sample = (real)(((vp[15] * dp[0]) +
					(vp[14] * dp[1]) +
					(vp[13] * dp[2]) +
					(vp[12] * dp[3]) +
					(vp[11] * dp[4]) +
					(vp[10] * dp[5]) +
					(vp[9] * dp[6]) +
					(vp[8] * dp[7]) +
					(vp[7] * dp[8]) +
					(vp[6] * dp[9]) +
					(vp[5] * dp[10]) +
					(vp[4] * dp[11]) +
					(vp[3] * dp[12]) +
					(vp[2] * dp[13]) +
					(vp[1] * dp[14]) +
					(vp[0] * dp[15])
					) /* * scalefactor */);

            buffer->append (channel, pcm_sample);
			} // for
		} break; // case 15:
	}; // switch (actual_write_pos)

	//buffer->appendblock (channel, a, 32);

}

void SynthesisFilter::calculate_pcm_samples(Obuffer *buffer)
{
	compute_new_v();
	compute_pcm_samples(buffer);

   actual_write_pos = (actual_write_pos + 1) & 0xf;
	actual_v = (actual_v == v1) ? v2 : v1;

	// initialize samples[]:
  for (register real *floatp = samples + 32; floatp > samples; )
	 *--floatp = 0.0f;
}
