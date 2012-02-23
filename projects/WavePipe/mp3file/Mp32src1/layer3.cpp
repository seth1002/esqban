/* layer3.cpp

	Implementation of the layer III decoder

   01/31/97 : Layer III routines adopted from the ISO MPEG Audio Subgroup
   Software Simulation Group's public c source for its MPEG audio decoder.
   These routines were in the file "decoder.c". Rearrangement of the routines
   as member functions of a layer III decoder object, and optimizations by
   Jeff Tsay (ctsay@pasteur.eecs.berkeley.edu).

   04/14/97 : Several performance improvements. Inverse IMDCT moved to
   an external source file. No huffman tables needed, so no need for
   initialization. Put get_side_info() in this source file, and made
   one function inline for better speed and elegance. Also added support
   for mono decoding of stereo streams as well as downmixing. Bug fix
   in dequantize_samples().

   06/26/97 : Added MPEG2 LSF capability and made a few minor speedups.
   The optimized reording function must be fixed, so right now the
   one from 1.81 is used. */

#include <math.h>

#include "all.h"
#include "l3type.h"
#include "ibitstr.h"
#include "obuffer.h"
#include "bit_res.h"
#include "header.h"
#include "synfilt.h"
#include "huffman.h"
#include "layer3.h"
#include "l3table.h"
#include "inv_mdct.h"

LayerIII_Decoder::LayerIII_Decoder(Ibitstream *stream0,
                                   Header *header0,
                                   SynthesisFilter *filtera,
                                   SynthesisFilter *filterb,
                 						  Obuffer *buffer0,
                                   enum e_channels which_ch0)
{
  stream         = stream0;
  header         = header0;
  filter1        = filtera;
  filter2        = filterb;
  buffer         = buffer0;
  which_channels = which_ch0;

  frame_start = 0;
  channels    = (header->mode() == single_channel) ? 1 : 2;
  max_gr      = (header->version() == MPEG1) ? 2 : 1;

  sfreq       =  header->sample_frequency() +
                 ((header->version() == MPEG1) ? 3 : 0);

  if (channels == 2) {

  	  switch (which_channels) {

     case left:
     case downmix:
     first_channel = last_channel = 0;
     break;

     case right:
     first_channel = last_channel = 1;
     break;

     case both:
	  default:
     first_channel  = 0;
     last_channel   = 1;
     break;
     }

  } else {

    first_channel = last_channel = 0;
  }

  for(int32 ch=0;ch<2;ch++)
     for (int32 j=0; j<576; j++)
   		prevblck[ch][j] = 0.0f;

  nonzero[0] = nonzero[1] = 576;

  br = new Bit_Reserve();
  si = new III_side_info_t;
}

LayerIII_Decoder::~LayerIII_Decoder()
{
   delete br;
   delete si;
}

void LayerIII_Decoder::seek_notify()
{
	frame_start = 0;


  for(int32 ch=0;ch<2;ch++)
	  for (int32 j=0; j<576; j++)
   	   prevblck[ch][j] = 0.0f;

   delete br;
   br = new Bit_Reserve;
}


bool LayerIII_Decoder::get_side_info()
// Reads the side info from the stream, assuming the entire
// frame has been read already.

// Mono   : 136 bits (= 17 bytes)
// Stereo : 256 bits (= 32 bytes)

{
	uint32 ch;
	int32 gr;

	if (header->version() == MPEG1) {

		si->main_data_begin = stream->get_bits(9);
		if (channels == 1)
			si->private_bits = stream->get_bits(5);
		else si->private_bits = stream->get_bits(3);

		for (ch=0; ch<channels; ch++) {
			si->ch[ch].scfsi[0] = stream->get_bits(1);
			si->ch[ch].scfsi[1] = stream->get_bits(1);
			si->ch[ch].scfsi[2] = stream->get_bits(1);
			si->ch[ch].scfsi[3] = stream->get_bits(1);
	   }

		for (gr=0; gr<2; gr++) {
			for (ch=0; ch<channels; ch++) {
				si->ch[ch].gr[gr].part2_3_length = stream->get_bits(12);
  				si->ch[ch].gr[gr].big_values = stream->get_bits(9);
				si->ch[ch].gr[gr].global_gain = stream->get_bits(8);
				si->ch[ch].gr[gr].scalefac_compress = stream->get_bits(4);
				si->ch[ch].gr[gr].window_switching_flag = stream->get_bits(1);
				if (si->ch[ch].gr[gr].window_switching_flag) {
					si->ch[ch].gr[gr].block_type       = stream->get_bits(2);
					si->ch[ch].gr[gr].mixed_block_flag = stream->get_bits(1);

					si->ch[ch].gr[gr].table_select[0]  = stream->get_bits(5);
					si->ch[ch].gr[gr].table_select[1]  = stream->get_bits(5);

					si->ch[ch].gr[gr].subblock_gain[0] = stream->get_bits(3);
					si->ch[ch].gr[gr].subblock_gain[1] = stream->get_bits(3);
					si->ch[ch].gr[gr].subblock_gain[2] = stream->get_bits(3);

					// Set region_count parameters since they are implicit in this case.

					if (si->ch[ch].gr[gr].block_type == 0) {
						//	 Side info bad: block_type == 0 in split block
						return false;
					} else if (si->ch[ch].gr[gr].block_type == 2
	  							  && si->ch[ch].gr[gr].mixed_block_flag == 0) {
						si->ch[ch].gr[gr].region0_count = 8;
               } else {
	               si->ch[ch].gr[gr].region0_count = 7;
               }
					si->ch[ch].gr[gr].region1_count = 20 -
							si->ch[ch].gr[gr].region0_count;
				} else {
					si->ch[ch].gr[gr].table_select[0] = stream->get_bits(5);
					si->ch[ch].gr[gr].table_select[1] = stream->get_bits(5);
					si->ch[ch].gr[gr].table_select[2] = stream->get_bits(5);
					si->ch[ch].gr[gr].region0_count = stream->get_bits(4);
					si->ch[ch].gr[gr].region1_count = stream->get_bits(3);
					si->ch[ch].gr[gr].block_type = 0;
				}
				si->ch[ch].gr[gr].preflag = stream->get_bits(1);
				si->ch[ch].gr[gr].scalefac_scale = stream->get_bits(1);
				si->ch[ch].gr[gr].count1table_select = stream->get_bits(1);
         }
      }

   } else {  	// MPEG-2 LSF

      si->main_data_begin = stream->get_bits(8);
      if (channels == 1)
      	si->private_bits = stream->get_bits(1);
      else si->private_bits = stream->get_bits(2);

      for (ch=0; ch<channels; ch++) {

          si->ch[ch].gr[0].part2_3_length = stream->get_bits(12);
          si->ch[ch].gr[0].big_values = stream->get_bits(9);
          si->ch[ch].gr[0].global_gain = stream->get_bits(8);
          si->ch[ch].gr[0].scalefac_compress = stream->get_bits(9);
          si->ch[ch].gr[0].window_switching_flag = stream->get_bits(1);

          if (si->ch[ch].gr[0].window_switching_flag) {

             si->ch[ch].gr[0].block_type = stream->get_bits(2);
             si->ch[ch].gr[0].mixed_block_flag = stream->get_bits(1);
             si->ch[ch].gr[0].table_select[0] = stream->get_bits(5);
             si->ch[ch].gr[0].table_select[1] = stream->get_bits(5);

             si->ch[ch].gr[0].subblock_gain[0] = stream->get_bits(3);
             si->ch[ch].gr[0].subblock_gain[1] = stream->get_bits(3);
             si->ch[ch].gr[0].subblock_gain[2] = stream->get_bits(3);

            // Set region_count parameters since they are implicit in this case.

             if (si->ch[ch].gr[0].block_type == 0) {
                // Side info bad: block_type == 0 in split block
                return false;
             } else if (si->ch[ch].gr[0].block_type == 2
                      && si->ch[ch].gr[0].mixed_block_flag == 0) {
             	 si->ch[ch].gr[0].region0_count = 8;
				 } else {
             	 si->ch[ch].gr[0].region0_count = 7;
                si->ch[ch].gr[0].region1_count = 20 -
                											si->ch[ch].gr[0].region0_count;
             }

          } else {
             si->ch[ch].gr[0].table_select[0] = stream->get_bits(5);
             si->ch[ch].gr[0].table_select[1] = stream->get_bits(5);
             si->ch[ch].gr[0].table_select[2] = stream->get_bits(5);
             si->ch[ch].gr[0].region0_count = stream->get_bits(4);
             si->ch[ch].gr[0].region1_count = stream->get_bits(3);
             si->ch[ch].gr[0].block_type = 0;
          }

          si->ch[ch].gr[0].scalefac_scale = stream->get_bits(1);
          si->ch[ch].gr[0].count1table_select = stream->get_bits(1);
      }   // for(ch=0; ch<channels; ch++)
   } // if (header->version() == MPEG1)
	return true;
}

struct {
	int32 l[5];
	int32 s[3];} sfbtable = {{0, 6, 11, 16, 21},
								    {0, 6, 12}};

void LayerIII_Decoder::get_scale_factors(uint32 ch, uint32 gr)
{
	int32 sfb, window;
	gr_info_s *gr_info = &(si->ch[ch].gr[gr]);
   int32 scale_comp   = gr_info->scalefac_compress;
   int32 length0      = slen[0][scale_comp];
   int32 length1      = slen[1][scale_comp];

	if (gr_info->window_switching_flag && (gr_info->block_type == 2)) {
		if (gr_info->mixed_block_flag) { // MIXED
			for (sfb = 0; sfb < 8; sfb++)
				scalefac[ch].l[sfb] = br->hgetbits(
					  slen[0][gr_info->scalefac_compress]);
			for (sfb = 3; sfb < 6; sfb++)
				for (window=0; window<3; window++)
					scalefac[ch].s[window][sfb] = br->hgetbits(
					  slen[0][gr_info->scalefac_compress]);
			for (sfb = 6; sfb < 12; sfb++)
				for (window=0; window<3; window++)
					scalefac[ch].s[window][sfb] = br->hgetbits(
					  slen[1][gr_info->scalefac_compress]);
			for (sfb=12,window=0; window<3; window++)
				scalefac[ch].s[window][sfb] = 0;

      } else {  // SHORT

         scalefac[ch].s[0][0]  = br->hgetbits(length0);
         scalefac[ch].s[1][0]  = br->hgetbits(length0);
         scalefac[ch].s[2][0]  = br->hgetbits(length0);
         scalefac[ch].s[0][1]  = br->hgetbits(length0);
         scalefac[ch].s[1][1]  = br->hgetbits(length0);
         scalefac[ch].s[2][1]  = br->hgetbits(length0);
         scalefac[ch].s[0][2]  = br->hgetbits(length0);
         scalefac[ch].s[1][2]  = br->hgetbits(length0);
         scalefac[ch].s[2][2]  = br->hgetbits(length0);
         scalefac[ch].s[0][3]  = br->hgetbits(length0);
         scalefac[ch].s[1][3]  = br->hgetbits(length0);
         scalefac[ch].s[2][3]  = br->hgetbits(length0);
         scalefac[ch].s[0][4]  = br->hgetbits(length0);
         scalefac[ch].s[1][4]  = br->hgetbits(length0);
         scalefac[ch].s[2][4]  = br->hgetbits(length0);
         scalefac[ch].s[0][5]  = br->hgetbits(length0);
         scalefac[ch].s[1][5]  = br->hgetbits(length0);
         scalefac[ch].s[2][5]  = br->hgetbits(length0);
         scalefac[ch].s[0][6]  = br->hgetbits(length1);
         scalefac[ch].s[1][6]  = br->hgetbits(length1);
         scalefac[ch].s[2][6]  = br->hgetbits(length1);
         scalefac[ch].s[0][7]  = br->hgetbits(length1);
         scalefac[ch].s[1][7]  = br->hgetbits(length1);
         scalefac[ch].s[2][7]  = br->hgetbits(length1);
         scalefac[ch].s[0][8]  = br->hgetbits(length1);
         scalefac[ch].s[1][8]  = br->hgetbits(length1);
         scalefac[ch].s[2][8]  = br->hgetbits(length1);
         scalefac[ch].s[0][9]  = br->hgetbits(length1);
         scalefac[ch].s[1][9]  = br->hgetbits(length1);
         scalefac[ch].s[2][9]  = br->hgetbits(length1);
         scalefac[ch].s[0][10] = br->hgetbits(length1);
         scalefac[ch].s[1][10] = br->hgetbits(length1);
         scalefac[ch].s[2][10] = br->hgetbits(length1);
         scalefac[ch].s[0][11] = br->hgetbits(length1);
         scalefac[ch].s[1][11] = br->hgetbits(length1);
         scalefac[ch].s[2][11] = br->hgetbits(length1);
         scalefac[ch].s[0][12] = 0;
			scalefac[ch].s[1][12] = 0;
			scalefac[ch].s[2][12] = 0;
		} // SHORT

	} else {   // LONG types 0,1,3

      if ((si->ch[ch].scfsi[0] == 0) || (gr == 0)) {
           scalefac[ch].l[0]  = br->hgetbits(length0);
           scalefac[ch].l[1]  = br->hgetbits(length0);
           scalefac[ch].l[2]  = br->hgetbits(length0);
           scalefac[ch].l[3]  = br->hgetbits(length0);
           scalefac[ch].l[4]  = br->hgetbits(length0);
           scalefac[ch].l[5]  = br->hgetbits(length0);
		}
      if ((si->ch[ch].scfsi[1] == 0) || (gr == 0)) {
           scalefac[ch].l[6]  = br->hgetbits(length0);
           scalefac[ch].l[7]  = br->hgetbits(length0);
           scalefac[ch].l[8]  = br->hgetbits(length0);
           scalefac[ch].l[9]  = br->hgetbits(length0);
           scalefac[ch].l[10] = br->hgetbits(length0);
		}
      if ((si->ch[ch].scfsi[2] == 0) || (gr == 0)) {
           scalefac[ch].l[11] = br->hgetbits(length1);
           scalefac[ch].l[12] = br->hgetbits(length1);
           scalefac[ch].l[13] = br->hgetbits(length1);
           scalefac[ch].l[14] = br->hgetbits(length1);
           scalefac[ch].l[15] = br->hgetbits(length1);
		}
      if ((si->ch[ch].scfsi[3] == 0) || (gr == 0)) {
           scalefac[ch].l[16] = br->hgetbits(length1);
           scalefac[ch].l[17] = br->hgetbits(length1);
           scalefac[ch].l[18] = br->hgetbits(length1);
           scalefac[ch].l[19] = br->hgetbits(length1);
           scalefac[ch].l[20] = br->hgetbits(length1);
		}

      scalefac[ch].l[21] = 0;
		scalefac[ch].l[22] = 0;
	}
}

uint32 nr_of_sfb_block[6][3][4] =
	{{{ 6, 5, 5, 5} , { 9, 9, 9, 9} , { 6, 9, 9, 9}},
    {{ 6, 5, 7, 3} , { 9, 9,12, 6} , { 6, 9,12, 6}},
    {{11,10, 0, 0} , {18,18, 0, 0} , {15,18, 0, 0}},
    {{ 7, 7, 7, 0} , {12,12,12, 0} , { 6,15,12, 0}},
    {{ 6, 6, 6, 3} , {12, 9, 9, 6} , { 6,12, 9, 6}},
    {{ 8, 8, 5, 0} , {15,12, 9, 0} , { 6,18, 9, 0}}};

uint32 scalefac_buffer[54];

void LayerIII_Decoder::get_LSF_scale_data(uint32 ch, uint32 gr)
{
	uint32 new_slen[4];
  	uint32 scalefac_comp, int_scalefac_comp;
   uint32 mode_ext = header->mode_extension();
	int32 m;
	int32 blocktypenumber, blocknumber;

	gr_info_s *gr_info = &(si->ch[ch].gr[gr]);

	scalefac_comp =  gr_info->scalefac_compress;

   if (gr_info->block_type == 2) {
   	if (gr_info->mixed_block_flag == 0)
      	blocktypenumber = 1;
      else if (gr_info->mixed_block_flag == 1)
			blocktypenumber = 2;
      else
      	blocktypenumber = 0;
   } else {
   	blocktypenumber = 0;
   }

   if(!(((mode_ext == 1) || (mode_ext == 3)) && (ch == 1))) {

		if(scalefac_comp < 400) {

			new_slen[0] = (scalefac_comp >> 4) / 5 ;
			new_slen[1] = (scalefac_comp >> 4) % 5 ;
			new_slen[2] = (scalefac_comp & 0xF) >> 2 ;
			new_slen[3] = (scalefac_comp & 3);
         si->ch[ch].gr[gr].preflag = 0;

         blocknumber = 0;

      } else if (scalefac_comp  < 500) {

			new_slen[0] = ((scalefac_comp - 400) >> 2) / 5 ;
			new_slen[1] = ((scalefac_comp - 400) >> 2) % 5 ;
			new_slen[2] = (scalefac_comp - 400 ) & 3 ;
			new_slen[3] = 0;
         si->ch[ch].gr[gr].preflag = 0;

         blocknumber = 1;

	   } else if (scalefac_comp < 512) {

			new_slen[0] = (scalefac_comp - 500 ) / 3 ;
			new_slen[1] = (scalefac_comp - 500)  % 3 ;
			new_slen[2] = 0;
			new_slen[3] = 0;
      	si->ch[ch].gr[gr].preflag = 1;

	      blocknumber = 2;
 	   }
   }

   if((((mode_ext == 1) || (mode_ext == 3)) && (ch == 1)))
   {
      int_scalefac_comp = scalefac_comp >> 1;

      if (int_scalefac_comp < 180)
      {
			new_slen[0] = int_scalefac_comp  / 36 ;
			new_slen[1] = (int_scalefac_comp % 36 ) / 6 ;
			new_slen[2] = (int_scalefac_comp % 36) % 6;
			new_slen[3] = 0;
         si->ch[ch].gr[gr].preflag = 0;
         blocknumber = 3;
      } else if (int_scalefac_comp < 244) {
			new_slen[0] = ((int_scalefac_comp - 180 )  & 0x3F) >> 4 ;
			new_slen[1] = ((int_scalefac_comp - 180) & 0xF) >> 2 ;
			new_slen[2] = (int_scalefac_comp - 180 ) & 3 ;
			new_slen[3] = 0;
         si->ch[ch].gr[gr].preflag = 0;
         blocknumber = 4;
      } else if (int_scalefac_comp < 255) {
			new_slen[0] = (int_scalefac_comp - 244 ) / 3 ;
			new_slen[1] = (int_scalefac_comp - 244 )  % 3 ;
			new_slen[2] = 0 ;
			new_slen[3] = 0;
         si->ch[ch].gr[gr].preflag = 0;
         blocknumber = 5;
      }
   }

   for (uint32 x=0; x<45; x++) // why 45, not 54?
   	scalefac_buffer[x] = 0;

   m = 0;
   for (uint32 i=0; i<4;i++) {
     	for (uint32 j = 0; j < nr_of_sfb_block[blocknumber][blocktypenumber][i];
      	 j++)
      {
        scalefac_buffer[m] = (new_slen[i] == 0) ? 0 :
        							  br->hgetbits(new_slen[i]);
        m++;

      } // for (unint32 j ...
   } // for (uint32 i ...
}

void LayerIII_Decoder::get_LSF_scale_factors(uint32 ch, uint32 gr)
{
	uint32 m = 0;
   uint32 sfb, window;
	gr_info_s *gr_info = &(si->ch[ch].gr[gr]);

   get_LSF_scale_data(ch, gr);

   if (gr_info->window_switching_flag && (gr_info->block_type == 2)) {
      if (gr_info->mixed_block_flag) { 	// MIXED
         for (sfb = 0; sfb < 8; sfb++)
         {
              scalefac[ch].l[sfb] = scalefac_buffer[m];
              m++;
         }
         for (sfb = 3; sfb < 12; sfb++) {
            for (window=0; window<3; window++)
            {
               scalefac[ch].s[window][sfb] = scalefac_buffer[m];
               m++;
            }
         }
         for (window=0; window<3; window++)
            scalefac[ch].s[window][12] = 0;

      } else {  // SHORT

           for (sfb = 0; sfb < 12; sfb++) {
               for (window=0; window<3; window++)
               {
                  scalefac[ch].s[window][sfb] = scalefac_buffer[m];
                  m++;
               }
           }

           for (window=0; window<3; window++)
               scalefac[ch].s[window][12] = 0;
      }
   } else {   // LONG types 0,1,3

      for (sfb = 0; sfb < 21; sfb++) {
          scalefac[ch].l[sfb] = scalefac_buffer[m];
          m++;
      }
      scalefac[ch].l[21] = 0; // Jeff
      scalefac[ch].l[22] = 0;
	}
}

void LayerIII_Decoder::huffman_decode(uint32 ch, uint32 gr)
{
	int32 x, y;
	int32 v, w;
   int32 part2_3_end = part2_start + si->ch[ch].gr[gr].part2_3_length;
   int32 num_bits;
	int32 region1Start;
	int32 region2Start;
   int32 index;
 	struct huffcodetab *h;

	// Find region boundary for short block case

	if ( (si->ch[ch].gr[gr].window_switching_flag) &&
		  (si->ch[ch].gr[gr].block_type == 2) ) {

		// Region2.

		region1Start = 36;  // sfb[9/3]*3=36
		region2Start = 576; // No Region2 for short block case

	} else {          // Find region boundary for long block case

		region1Start = sfBandIndex[sfreq].l[si->ch[ch].gr[gr].region0_count
                                             + 1];
		region2Start = sfBandIndex[sfreq].l[si->ch[ch].gr[gr].region0_count +
										   si->ch[ch].gr[gr].region1_count + 2]; /* MI */
   }

   index = 0;
	// Read bigvalues area
	for (uint32 i=0; i<(si->ch[ch].gr[gr].big_values<<1); i+=2) {
		if      (i<region1Start) h = &ht[si->ch[ch].gr[gr].table_select[0]];
		else if (i<region2Start) h = &ht[si->ch[ch].gr[gr].table_select[1]];
			  else                h = &ht[si->ch[ch].gr[gr].table_select[2]];

		huffman_decoder(h, &x, &y, &v, &w, br);

      is_1d[index++] = x;
      is_1d[index++] = y;
	}

	// Read count1 area
	h = &ht[si->ch[ch].gr[gr].count1table_select+32];
   num_bits = br->hsstell();

	while ((num_bits < part2_3_end) && (index < 576)) {

		huffman_decoder(h, &x, &y, &v, &w, br);

      is_1d[index++] = v;
      is_1d[index++] = w;
      is_1d[index++] = x;
      is_1d[index++] = y;
      num_bits = br->hsstell();
   }

	if (num_bits > part2_3_end) {
		br->rewindNbits(num_bits - part2_3_end);
      index-=4;
   }

   num_bits = br->hsstell();

	// Dismiss stuffing bits
	if (num_bits < part2_3_end)
   	br->hgetbits(part2_3_end - num_bits);

	// Zero out rest

   if (index < 576)
	   nonzero[ch] = index;
   else
   	nonzero[ch] = 576;

   // may not be necessary
   for (; index<576; index++)
   	is_1d[index] = 0;
}

void LayerIII_Decoder::dequantize_sample(real xr[SBLIMIT][SSLIMIT],
												     uint32 ch, uint32 gr)
{
	gr_info_s *gr_info = &(si->ch[ch].gr[gr]);
	int32  cb=0;
	int32  next_cb_boundary, cb_begin, cb_width;
	int32  index=0, t_index, j;
   real   g_gain;
   real  *xr_1d = &xr[0][0];

	// choose correct scalefactor band per block type, initalize boundary

	if (gr_info->window_switching_flag && (gr_info->block_type == 2) ) {
		if (gr_info->mixed_block_flag)
			next_cb_boundary=sfBandIndex[sfreq].l[1];  // LONG blocks: 0,1,3
		else {
         cb_width = sfBandIndex[sfreq].s[1];
		   next_cb_boundary = (cb_width << 2) - cb_width;
	 	   cb_begin = 0;
		}
	} else {
		next_cb_boundary=sfBandIndex[sfreq].l[1];  // LONG blocks: 0,1,3
   }

   // Compute overall (global) scaling.

	g_gain = (real) pow(2.0 , (0.25 * (gr_info->global_gain - 210.0)));

  	for (j=0; j<nonzero[ch]; j++) {
    	if (is_1d[j] == 0) {
			xr_1d[j] = 0.0f;
      } else {
         int32 abv = is_1d[j];
         if (is_1d[j] > 0)
	  		   xr_1d[j] = g_gain * t_43[abv];
         else
         	xr_1d[j] = -g_gain * t_43[-abv];
      }
   }

	// apply formula per block type

   for (j=0; j<nonzero[ch]; j++) {

		if (index == next_cb_boundary)  { /* Adjust critical band boundary */
      	if (gr_info->window_switching_flag && (gr_info->block_type == 2)) {
         	if (gr_info->mixed_block_flag)  {

            	if (index == sfBandIndex[sfreq].l[8])  {
                  next_cb_boundary = sfBandIndex[sfreq].s[4];
                  next_cb_boundary = (next_cb_boundary << 2) -
                    			           next_cb_boundary;
                  cb = 3;
                  cb_width = sfBandIndex[sfreq].s[4] -
                    			  sfBandIndex[sfreq].s[3];

                  cb_begin = sfBandIndex[sfreq].s[3];
                  cb_begin = (cb_begin << 2) - cb_begin;

               } else if (index < sfBandIndex[sfreq].l[8]) {

               	next_cb_boundary = sfBandIndex[sfreq].l[(++cb)+1];

               } else {

               	next_cb_boundary = sfBandIndex[sfreq].s[(++cb)+1];
                  next_cb_boundary = (next_cb_boundary << 2) -
                    				        next_cb_boundary;

                  cb_begin = sfBandIndex[sfreq].s[cb];
						cb_width = sfBandIndex[sfreq].s[cb+1] -
                             cb_begin;
                  cb_begin = (cb_begin << 2) - cb_begin;
               }

            } else  {

               next_cb_boundary = sfBandIndex[sfreq].s[(++cb)+1];
               next_cb_boundary = (next_cb_boundary << 2) -
                                  next_cb_boundary;

               cb_begin = sfBandIndex[sfreq].s[cb];
					cb_width = sfBandIndex[sfreq].s[cb+1] -
                          cb_begin;
               cb_begin = (cb_begin << 2) - cb_begin;
            }

         } else  { // long blocks

					next_cb_boundary = sfBandIndex[sfreq].l[(++cb)+1];

         }
      }

		// Do long/short dependent scaling operations

		if (gr_info->window_switching_flag &&
			 (((gr_info->block_type == 2) && (gr_info->mixed_block_flag == 0)) ||
			  ((gr_info->block_type == 2) && gr_info->mixed_block_flag && (j >= 36)) ))
      {

			t_index = (index - cb_begin) / cb_width;
/*            xr[sb][ss] *= pow(2.0, ((-2.0 * gr_info->subblock_gain[t_index])
                                    -(0.5 * (1.0 + gr_info->scalefac_scale)
                                      * scalefac[ch].s[t_index][cb]))); */
			uint32 idx = scalefac[ch].s[t_index][cb]
           				 << gr_info->scalefac_scale;
         idx += (gr_info->subblock_gain[t_index] << 2);

			xr_1d[j] *= two_to_negative_half_pow[idx];

		} else {   // LONG block types 0,1,3 & 1st 2 subbands of switched blocks
/*				xr[sb][ss] *= pow(2.0, -0.5 * (1.0+gr_info->scalefac_scale)
													 * (scalefac[ch].l[cb]
													 + gr_info->preflag * pretab[cb])); */
			uint32 idx = scalefac[ch].l[cb];

   		if (gr_info->preflag)
		   	idx += pretab[cb];

		   idx = idx << gr_info->scalefac_scale;
         xr_1d[j] *= two_to_negative_half_pow[idx];
		}
      index++;
	}

   for (j=nonzero[ch]; j<576; j++)
     	xr_1d[j] = 0.0f;

   return;
}

void LayerIII_Decoder::reorder(real xr[SBLIMIT][SSLIMIT], uint32 ch, uint32 gr)
{

	gr_info_s *gr_info = &(si->ch[ch].gr[gr]);
   uint32 freq, freq3;
   int32 index;
	int32 sfb, sfb_start, sfb_lines;
   int32 src_line, des_line;

   real *xr_1d = &xr[0][0];

	if (gr_info->window_switching_flag && (gr_info->block_type == 2)) {

      for(index=0; index<576; index++)
         out_1d[index] = 0.0f;

		if (gr_info->mixed_block_flag) {
			// NO REORDER FOR LOW 2 SUBBANDS
            for (index = 0; index < 36; index++)
               out_1d[index] = xr_1d[index];

			// REORDERING FOR REST SWITCHED SHORT
				for(sfb=3,sfb_start=sfBandIndex[sfreq].s[3],
	  				 sfb_lines=sfBandIndex[sfreq].s[4] - sfb_start;
	  				 sfb < 13; sfb++,sfb_start = sfBandIndex[sfreq].s[sfb],
				    (sfb_lines=sfBandIndex[sfreq].s[sfb+1] - sfb_start))
            {
					   int32 sfb_start3 = (sfb_start << 2) - sfb_start;

						for(freq=0, freq3=0; freq<sfb_lines;
                      freq++, freq3+=3) {

							src_line = sfb_start3 + freq;
							des_line = sfb_start3 + freq3;
							out_1d[des_line] = xr_1d[src_line];
							src_line += sfb_lines;
							des_line++;
							out_1d[des_line] = xr_1d[src_line];
							src_line += sfb_lines;
							des_line++;
							out_1d[des_line] = xr_1d[src_line];
					   }
            }

		} else {  // pure short
      	for(index=0;index<576;index++)
            out_1d[index] = xr_1d[reorder_table[sfreq][index]];
		}
	}
	else {   // long blocks
      for(index=0; index<576; index++)
      	out_1d[index] = xr_1d[index];
	}
}

void LayerIII_Decoder::i_stereo_k_values(uint32 is_pos, uint32 io_type,
												     uint32 i)
{
   if (is_pos == 0) {
      k[0][i] = 1.0f;
      k[1][i] = 1.0f;
   } else if (is_pos & 1) {
		k[0][i] = io[io_type][(is_pos + 1) >> 1];
      k[1][i] = 1.0f;
   } else {
      k[0][i] = 1.0f;
      k[1][i] = io[io_type][is_pos >> 1];
   }
}

void LayerIII_Decoder::stereo(uint32 gr)
{
   int32 sb, ss;

	if  (channels == 1) { // mono , bypass xr[0][][] to lr[0][][]

		for(sb=0;sb<SBLIMIT;sb++)
			for(ss=0;ss<SSLIMIT;ss+=3) {
				lr[0][sb][ss]   = ro[0][sb][ss];
            lr[0][sb][ss+1] = ro[0][sb][ss+1];
				lr[0][sb][ss+2] = ro[0][sb][ss+2];
         }

   } else {

	uint32 is_pos[576];
	real  is_ratio[576];
	gr_info_s *gr_info = &(si->ch[0].gr[gr]);
   uint32 mode_ext = header->mode_extension();
	int32 sfb;
	int32 i;
   int32 lines, temp, temp2;
	bool ms_stereo = (header->mode() == joint_stereo) && (mode_ext & 0x2);
	bool i_stereo  = (header->mode() == joint_stereo) && (mode_ext & 0x1);
   bool lsf = (header->version() == MPEG2_LSF);

	uint32 io_type = (gr_info->scalefac_compress & 1);

 	// initialization

   for (i=0; i<576; i++)
   	is_pos[i] = 7;

	if (i_stereo) {
   	if (gr_info->window_switching_flag && (gr_info->block_type == 2)) {
      	if (gr_info->mixed_block_flag) {

         	 int32 max_sfb = 0;

				 for (uint32 j=0; j<3; j++) {
            	int32 sfbcnt;
					sfbcnt = 2;
					for( sfb=12; sfb >=3; sfb-- ) {
               	i = sfBandIndex[sfreq].s[sfb];
						lines = sfBandIndex[sfreq].s[sfb+1] - i;
                  i = (i << 2) - i + (j+1) * lines - 1;

						while (lines > 0) {
                  	if (ro[1][ss_div[i]][ss_mod[i]] != 0.0f) {
                     	sfbcnt = sfb;
								sfb = -10;
								lines = -10;
							}

							lines--;
							i--;

						} // while (lines > 0)

					} // for (sfb=12 ...
					sfb = sfbcnt + 1;

					if (sfb > max_sfb)
						max_sfb = sfb;

					while(sfb < 12) {
               	temp = sfBandIndex[sfreq].s[sfb];
               	sb   = sfBandIndex[sfreq].s[sfb+1] - temp;
                  i    = (temp << 2) - temp + j * sb;

						for ( ; sb > 0; sb--) {
                  	is_pos[i] = scalefac[1].s[j][sfb];
							if (is_pos[i] != 7)
                     	if (lsf)
                           i_stereo_k_values(is_pos[i], io_type, i);
                        else
                        	is_ratio[i] = TAN12[is_pos[i]];

							i++;
						} // for (; sb>0...
						sfb++;
					} // while (sfb < 12)
					sfb = sfBandIndex[sfreq].s[10];
               sb  = sfBandIndex[sfreq].s[11] - sfb;
               sfb = (sfb << 2) - sfb + j * sb;
               temp  = sfBandIndex[sfreq].s[11];
               sb = sfBandIndex[sfreq].s[12] - temp;
               i = (temp << 2) - temp + j * sb;

					for (; sb > 0; sb--) {
               	is_pos[i] = is_pos[sfb];

		            if (lsf) {
		               k[0][i] = k[0][sfb];
				         k[1][i] = k[1][sfb];
		            } else {
     						is_ratio[i] = is_ratio[sfb];
                  }
						i++;
					} // for (; sb > 0 ...
				 }
				 if (max_sfb <= 3) {
                i = 2;
					 ss = 17;
					 sb = -1;
					 while (i >= 0) {
                	if (ro[1][i][ss] != 0.0f) {
                   	 sb = (i<<4) + (i<<1) + ss;
							 i = -1;
						} else {
                      ss--;
							 if (ss < 0) {
                         i--;
								 ss = 17;
							 }
						} // if (ro ...
					 } // while (i>=0)
					 i = 0;
					 while (sfBandIndex[sfreq].l[i] <= sb)
						 i++;
					 sfb = i;
					 i = sfBandIndex[sfreq].l[i];
					 for (; sfb<8; sfb++) {
                   sb = sfBandIndex[sfreq].l[sfb+1]-sfBandIndex[sfreq].l[sfb];
						 for (; sb>0; sb--) {
                      is_pos[i] = scalefac[1].l[sfb];
                   	 if (is_pos[i] != 7)
	                      if (lsf)
                           i_stereo_k_values(is_pos[i], io_type, i);
                         else
                        	is_ratio[i] = TAN12[is_pos[i]];
							 i++;
						 } // for (; sb>0 ...
					 } // for (; sfb<8 ...
				 } // for (j=0 ...
			} else { // if (gr_info->mixed_block_flag)
         	for (uint32 j=0; j<3; j++) {
            	int32 sfbcnt;
					sfbcnt = -1;
					for( sfb=12; sfb >=0; sfb-- )
					{
						temp = sfBandIndex[sfreq].s[sfb];
                  lines = sfBandIndex[sfreq].s[sfb+1] - temp;
                  i = (temp << 2) - temp + (j+1) * lines - 1;

						while (lines > 0) {
							if (ro[1][ss_div[i]][ss_mod[i]] != 0.0f) {
                     	sfbcnt = sfb;
								sfb = -10;
								lines = -10;
							}
							lines--;
							i--;
						} // while (lines > 0) */

					} // for (sfb=12 ...
					sfb = sfbcnt + 1;
					while(sfb<12) {
						temp = sfBandIndex[sfreq].s[sfb];
                  sb   = sfBandIndex[sfreq].s[sfb+1] - temp;
                  i    = (temp << 2) - temp + j * sb;
						for ( ; sb > 0; sb--) {
                  	is_pos[i] = scalefac[1].s[j][sfb];
							if (is_pos[i] != 7)
	                      if (lsf)
                           i_stereo_k_values(is_pos[i], io_type, i);
                         else
                        	is_ratio[i] = TAN12[is_pos[i]];
							i++;
						} // for (; sb>0 ...
						sfb++;
					} // while (sfb<12)

					temp = sfBandIndex[sfreq].s[10];
               temp2= sfBandIndex[sfreq].s[11];
               sb   = temp2 - temp;
               sfb  = (temp << 2) - temp + j * sb;
               sb   = sfBandIndex[sfreq].s[12] - temp2;
               i    = (temp2 << 2) - temp2 + j * sb;

					for (; sb>0; sb--) {
               	is_pos[i] = is_pos[sfb];

		            if (lsf) {
		               k[0][i] = k[0][sfb];
				         k[1][i] = k[1][sfb];
      		      } else {
               		is_ratio[i] = is_ratio[sfb];
                  }
						i++;
					} // for (; sb>0 ...
				} // for (sfb=12
			} // for (j=0 ...
		} else { // if (gr_info->window_switching_flag ...
      	i = 31;
			ss = 17;
			sb = 0;
			while (i >= 0) {
         	if (ro[1][i][ss] != 0.0f) {
            	sb = (i<<4) + (i<<1) + ss;
					i = -1;
				} else {
            	ss--;
					if (ss < 0) {
               	i--;
						ss = 17;
					}
				}
			}
			i = 0;
			while (sfBandIndex[sfreq].l[i] <= sb)
				i++;

			sfb = i;
			i = sfBandIndex[sfreq].l[i];
			for (; sfb<21; sfb++) {
         	sb = sfBandIndex[sfreq].l[sfb+1] - sfBandIndex[sfreq].l[sfb];
         	for (; sb > 0; sb--) {
            	is_pos[i] = scalefac[1].l[sfb];
					if (is_pos[i] != 7)
                  if (lsf)
                     i_stereo_k_values(is_pos[i], io_type, i);
                  else
                   	is_ratio[i] = TAN12[is_pos[i]];
					i++;
				}
			}
			sfb = sfBandIndex[sfreq].l[20];
			for (sb = 576 - sfBandIndex[sfreq].l[21]; (sb > 0) && (i<576); sb--)
			{
         	is_pos[i] = is_pos[sfb]; // error here : i >=576

            if (lsf) {
               k[0][i] = k[0][sfb];
		         k[1][i] = k[1][sfb];
            } else {
  					is_ratio[i] = is_ratio[sfb];
            }
				i++;
			} // if (gr_info->mixed_block_flag)
		} // if (gr_info->window_switching_flag ...
	} // if (i_stereo)

   	i = 0;
		for(sb=0;sb<SBLIMIT;sb++)
			for(ss=0;ss<SSLIMIT;ss++) {
				if (is_pos[i] == 7) {
					if (ms_stereo) {
						lr[0][sb][ss] = (ro[0][sb][ss]+ro[1][sb][ss]) * 0.707106781f;
						lr[1][sb][ss] = (ro[0][sb][ss]-ro[1][sb][ss]) * 0.707106781f;
					} else {
						lr[0][sb][ss] = ro[0][sb][ss];
						lr[1][sb][ss] = ro[1][sb][ss];
					}
				}
				else if (i_stereo) {

            	if (lsf) {
                  lr[0][sb][ss] = ro[0][sb][ss] * k[0][i];
                  lr[1][sb][ss] = ro[0][sb][ss] * k[1][i];
               } else {
               	lr[1][sb][ss] = ro[0][sb][ss] / (real) (1 + is_ratio[i]);
	  				   lr[0][sb][ss] = lr[1][sb][ss] * is_ratio[i];
               }
				}
/*				else {
					printf("Error in stereo processing\n");
				} */
            i++;
			}

    } // channels == 2
}

void LayerIII_Decoder::antialias(uint32 ch, uint32 gr)
{
	int32 sb18, ss, sb18lim;
   gr_info_s *gr_info = &(si->ch[ch].gr[gr]);

	// 31 alias-reduction operations between each pair of sub-bands
	// with 8 butterflies between each pair

	if  (gr_info->window_switching_flag && (gr_info->block_type == 2) &&
		 !gr_info->mixed_block_flag )
       return;

	if (gr_info->window_switching_flag && gr_info->mixed_block_flag &&
	    (gr_info->block_type == 2)) {
      sb18lim = 18;
	} else {
		sb18lim = 558;
   }

   for (sb18=0; sb18 < sb18lim; sb18+=18) {
      for (ss=0;ss<8;ss++) {
      	int32 src_idx1 = sb18 + 17 - ss;
         int32 src_idx2 = sb18 + 18 + ss;
      	real bu = out_1d[src_idx1];
			real bd = out_1d[src_idx2];
			out_1d[src_idx1] = (bu * cs[ss]) - (bd * ca[ss]);
			out_1d[src_idx2] = (bd * cs[ss]) + (bu * ca[ss]);
      }
   }
}

void LayerIII_Decoder::hybrid(uint32 ch, uint32 gr)
{
	real rawout[36];
   uint32 bt;
   int32 sb18;
   gr_info_s *gr_info = &(si->ch[ch].gr[gr]);
   real *tsOut;
   real *prvblk;

   for(sb18=0;sb18<576;sb18+=18) {

		bt = (gr_info->window_switching_flag && gr_info->mixed_block_flag &&
				 (sb18 < 36)) ? 0 : gr_info->block_type;

		tsOut = out_1d + sb18;

		inv_mdct(tsOut, rawout, bt);

		// overlap addition
		prvblk = &prevblck[ch][sb18];

	   tsOut[0]   = rawout[0]  + prvblk[0];
	   prvblk[0]  = rawout[18];
	   tsOut[1]   = rawout[1]  + prvblk[1];
	   prvblk[1]  = rawout[19];
	   tsOut[2]   = rawout[2]  + prvblk[2];
	   prvblk[2]  = rawout[20];
	   tsOut[3]   = rawout[3]  + prvblk[3];
	   prvblk[3]  = rawout[21];
	   tsOut[4]   = rawout[4]  + prvblk[4];
	   prvblk[4]  = rawout[22];
	   tsOut[5]   = rawout[5]  + prvblk[5];
	   prvblk[5]  = rawout[23];
	   tsOut[6]   = rawout[6]  + prvblk[6];
	   prvblk[6]  = rawout[24];
	   tsOut[7]   = rawout[7]  + prvblk[7];
	   prvblk[7]  = rawout[25];
	   tsOut[8]   = rawout[8]  + prvblk[8];
	   prvblk[8]  = rawout[26];
	   tsOut[9]   = rawout[9]  + prvblk[9];
	   prvblk[9]  = rawout[27];
   	tsOut[10]  = rawout[10] + prvblk[10];
   	prvblk[10] = rawout[28];
   	tsOut[11]  = rawout[11] + prvblk[11];
      prvblk[11] = rawout[29];
   	tsOut[12]  = rawout[12] + prvblk[12];
   	prvblk[12] = rawout[30];
   	tsOut[13]  = rawout[13] + prvblk[13];
   	prvblk[13] = rawout[31];
   	tsOut[14]  = rawout[14] + prvblk[14];
   	prvblk[14] = rawout[32];
   	tsOut[15]  = rawout[15] + prvblk[15];
   	prvblk[15] = rawout[33];
   	tsOut[16]  = rawout[16] + prvblk[16];
   	prvblk[16] = rawout[34];
   	tsOut[17]  = rawout[17] + prvblk[17];
   	prvblk[17] = rawout[35];
   }
}

void LayerIII_Decoder::do_downmix()
{
	for (uint32 sb=0; sb<SSLIMIT; sb++) {
   	for (uint32 ss=0; ss<SSLIMIT; ss+=3) {
      	lr[0][sb][ss]   = (lr[0][sb][ss]   + lr[1][sb][ss])   * 0.5f;
      	lr[0][sb][ss+1] = (lr[0][sb][ss+1] + lr[1][sb][ss+1]) * 0.5f;
      	lr[0][sb][ss+2] = (lr[0][sb][ss+2] + lr[1][sb][ss+2]) * 0.5f;
      }
   }
}

void LayerIII_Decoder::decode()
{
	 uint32 nSlots = header->slots();
    uint32 flush_main;
    uint32 gr, ch, ss, sb, sb18;
    int32 main_data_end;
	 int32 bytes_to_discard;
    int32 i;

	 get_side_info();

    for (i=0; i<nSlots; i++)
    	br->hputbuf(stream->get_bits(8));

    main_data_end = br->hsstell() >> 3; // of previous frame

    if (flush_main = (br->hsstell() & 7)) {
         br->hgetbits(8 - flush_main);
			main_data_end++;
	 }

	 bytes_to_discard = frame_start - main_data_end
							  - si->main_data_begin;

	 frame_start += nSlots;

    if (bytes_to_discard < 0)
			return;

	 if (main_data_end > 4096) {
			frame_start -= 4096;
			br->rewindNbytes(4096);
	 }

	 for (; bytes_to_discard > 0; bytes_to_discard--)
    		br->hgetbits(8);

	 for (gr=0;gr<max_gr;gr++) {

			for (ch=0; ch<channels; ch++) {
           part2_start = br->hsstell();

           if (header->version() == MPEG1)
				  get_scale_factors(ch, gr);
           else  // MPEG-2 LSF
              get_LSF_scale_factors(ch, gr);

			  huffman_decode(ch, gr);
			  dequantize_sample(ro[ch], ch, gr);
			}

         stereo(gr);

         if ((which_channels == downmix) && (channels > 1))
         	do_downmix();

         for (ch=first_channel; ch<=last_channel; ch++) {

         		reorder(lr[ch], ch, gr);
					antialias(ch, gr);
               hybrid(ch, gr);

					for (sb18=18;sb18<576;sb18+=36) // Frequency inversion
                   for (ss=1;ss<SSLIMIT;ss+=2)
                  	  out_1d[sb18 + ss] = -out_1d[sb18 + ss];

					if ((ch == 0) || (which_channels == right)) {
					  for (ss=0;ss<SSLIMIT;ss++) { // Polyphase synthesis
                  	sb = 0;
                 		for (sb18=0; sb18<576; sb18+=18) {
								 filter1->input_sample(out_1d[sb18+ss], sb);
                         sb++;
                     }
                    	filter1->calculate_pcm_samples(buffer);
					  }
					} else {
					  for (ss=0;ss<SSLIMIT;ss++) { // Polyphase synthesis
                  	sb = 0;
                 		for (sb18=0; sb18<576; sb18+=18) {
								 filter2->input_sample(out_1d[sb18+ss], sb);
                         sb++;
                     }
                    	filter2->calculate_pcm_samples(buffer);
					  }

               }
			}	// channels
	 }	// granule

  	 buffer->write_buffer(1);
}

