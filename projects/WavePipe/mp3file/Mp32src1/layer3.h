/* layer3.h

   Declarations for the Layer III decoder object */

#ifndef LAYERIII_H
#define LAYERIII_H

#include "all.h"
#include "l3type.h"
#include "ibitstr.h"
#include "obuffer.h"
#include "bit_res.h"
#include "header.h"
#include "synfilt.h"

#define SSLIMIT 18
#define SBLIMIT 32

/* Size of the table of whole numbers raised to 4/3 power.
   This may be adjusted for performance without any problems. */
#define POW_TABLE_LIMIT 512

class LayerIII_Decoder {

public:

	LayerIII_Decoder(Ibitstream *stream0, Header *header0,
   	           	  SynthesisFilter *filtera, SynthesisFilter *filterb,
                    Obuffer *buffer0, enum e_channels which_ch0);

   ~LayerIII_Decoder();

   // Notify decoder that a seek is being made
   void seek_notify();

   // Decode one frame, filling the buffer with the output samples
	void decode();

private:

	// private data

	int32 is_1d[SBLIMIT*SSLIMIT];
   real  ro[2][SBLIMIT][SSLIMIT];
   real  lr[2][SBLIMIT][SSLIMIT];
	real	out_1d[SBLIMIT*SSLIMIT];
   real  prevblck[2][SBLIMIT*SSLIMIT];
   real  k[2][SBLIMIT*SSLIMIT];
   int32 nonzero[2];

	Ibitstream *stream;
   Header *header;
   SynthesisFilter *filter1, *filter2;
   Obuffer *buffer;
   enum e_channels which_channels;
   Bit_Reserve *br;
   III_side_info_t *si;
   III_scalefac_t scalefac;

   uint32 max_gr;
   int32  frame_start;
   int32  part2_start;
   uint32 channels;
   uint32 first_channel;
   uint32 last_channel;
   uint32 sfreq;

   // private member functions

	bool get_side_info();
	void get_scale_factors(uint32 ch, uint32 gr);
   void get_LSF_scale_data(uint32 ch, uint32 gr);
   void get_LSF_scale_factors(uint32 ch, uint32 gr);
   void huffman_decode(uint32 ch, uint32 gr);
   void i_stereo_k_values(uint32 is_pos, uint32 io_type, uint32 i);
	void dequantize_sample(real xr[SBLIMIT][SSLIMIT], uint32 ch, uint32 gr);
	void reorder(real xr[SBLIMIT][SSLIMIT], uint32 ch, uint32 gr);
	void stereo(uint32 gr);
	void antialias(uint32 ch, uint32 gr);
	void hybrid(uint32 ch, uint32 gr);
   void do_downmix();

};

#endif
