/*
 *  @(#) synthesis_filter.h 1.8, last edit: 6/15/94 16:52:00
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

#ifndef SYNTHESIS_FILTER_H
#define SYNTHESIS_FILTER_H

#include "all.h"
#include "obuffer.h"


// A class for the synthesis filter bank:
// This class does a fast downsampling from 32, 44.1 or 48 kHz to 8 kHz, if ULAW is defined.
// Frequencies above 4 kHz are removed by ignoring higher subbands.
class SynthesisFilter
{
  static const real d[512];
  real v1[512], v2[512];
  real *actual_v;			// v1 or v2
  uint32 actual_write_pos;		// 0-15

  real	samples[32];			// 32 new subband samples

  uint32 channel;
  real	scalefactor;

  void compute_new_v();
  void compute_pcm_samples(Obuffer *buffer);

  // Clips the pcm sample so it will fit in 16 bits
#ifdef TRUNC_ROUNDING

  inline int16 clip(real sample)
  {
  	return ((sample > 32767.0f) ? (int16) 32767 :
           ((sample < -32768.0f) ? (int16) -32768 :
           ((sample < 0.0f) ? -((int16) (-sample + 0.5f))
           					    :   (int16) ( sample + 0.5f)) ));
  }

#else

  inline int16 clip(real sample)
  {
  	return ((sample > 32767.0f) ? 32767 :
           ((sample < -32768.0f) ? -32768 :
			  (int16) sample));
  }
#endif // TRUNC_ROUNDING

public:
  SynthesisFilter(uint32 channelnumber, real scalefactor = 32768.0);
	  // the scalefactor scales the calculated float pcm samples to short values
	  // (raw pcm samples are in [-1.0, 1.0], if no violations occur)

  void	input_sample(real sample, uint32 subbandnumber)
  {
	 samples[subbandnumber] = sample;
  };

  void	calculate_pcm_samples(Obuffer *buffer);
	// calculate 32 PCM samples and put the into the Obuffer-object

  void   reset();
  // reset the synthesis filter
};

#endif
