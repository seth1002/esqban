/*	header.cpp

	Implementation of MPEG header class

	A few layer III, MPEG-2 LSF, and seeking modifications made by
   Jeff Tsay. MPEG-2 LSF is now supported.

   Last modified : 06/04/97 */

   /*
 *  @(#) header.cc 1.8, last edit: 6/15/94 16:51:44
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
 *    - iostreams manipulator calls like "cerr << setw (2) << ..." replaced by
 *      "cerr.width (2); ..." due to problems with older GNU C++ releases.
 *    - syncword recognition slightly changed
 */

#ifndef GUI
#include <iostream.h>
#endif  // GUI

#include "header.h"

const uint32 Header::frequencies[2][4] =
{{22050, 24000, 16000, 1},
 {44100, 48000, 32000, 1}};

Header::Header()
{
  	framesize = 0;
   nSlots    = 0;
  	crc       = NULL;
   offset    = NULL;
   initial_sync = false;
}

Header::Header(const Header &h0)
{
  int32 entries;
  int32 i;

  h_layer          = h0.h_layer;
  h_protection_bit = h0.h_protection_bit;
  h_bitrate_index  = h0.h_bitrate_index;
  h_padding_bit    = h0.h_padding_bit;
  h_mode_extension = h0.h_mode_extension;
  h_version        = h0.h_version;
  h_mode           = h0.h_mode;

  h_sample_frequency   = h0.h_sample_frequency;
  h_number_of_subbands = h0.h_number_of_subbands;
  h_intensity_stereo_bound = h0.h_intensity_stereo_bound;

  h_copyright  = h0.h_copyright;
  h_original   = h0.h_original;
  initial_sync = h0.initial_sync;

  crc       = h0.crc;

  if (h0.offset) {

     entries   = sizeof(h0.offset) / sizeof(uint32);
     offset    = new uint32 [entries];

     for (i=0; i<entries; i++)
       	offset[i] = h0.offset[i];

  } else {
     offset = NULL;
  }

  checksum  = h0.checksum;
  framesize = h0.framesize;
  nSlots    = h0.nSlots;
}

Header::~Header()
{
	if (offset != NULL) delete [] offset;
}

Header& Header::operator = (const Header &h0)
{
  int32 entries;
  int32 i;

  h_layer          = h0.h_layer;
  h_protection_bit = h0.h_protection_bit;
  h_bitrate_index  = h0.h_bitrate_index;
  h_padding_bit    = h0.h_padding_bit;
  h_mode_extension = h0.h_mode_extension;
  h_version        = h0.h_version;
  h_mode           = h0.h_mode;

  h_sample_frequency   = h0.h_sample_frequency;
  h_number_of_subbands = h0.h_number_of_subbands;
  h_intensity_stereo_bound = h0.h_intensity_stereo_bound;

  h_copyright  = h0.h_copyright;
  h_original   = h0.h_original;
  initial_sync = h0.initial_sync;

  crc       = h0.crc;

  if (h0.offset) {

	  entries   = sizeof(h0.offset) / sizeof(uint32);

     if (offset != NULL)
        delete [] offset;

	  offset    = new uint32 [entries];

  	  for (i=0; i<entries; i++)
   		offset[i] = h0.offset[i];

  } else {
     offset = NULL;
  }

  checksum  = h0.checksum;
  framesize = h0.framesize;
  nSlots    = h0.nSlots;

  return *this;
}

bool Header::read_header(Ibitstream *stream, Crc16 **crcp)
{
  uint32 headerstring, channel_bitrate;

  if (!initial_sync) {

  	if (!stream->get_header(&headerstring, INITIAL_SYNC))
	 	return false;

  	h_version = (e_version) ((headerstring >> 19) & 1);

   if ((h_sample_frequency = (e_sample_frequency)
                             ((headerstring >> 10) & 3)) == 3)
   {
#ifdef WIN32GUI
		 MessageBox(NULL, "Unknown sample frequency in header",
	    				"Stream not supported", MB_ICONSTOP | MB_OK);
#else
		 cerr << "Unknown sample frequency!" << endl;
#endif
	    return false;
   }

   stream->set_syncword(headerstring & 0xFFF80CC0);

   initial_sync = true;

  } else {

	  	if (!stream->get_header(&headerstring, STRICT_SYNC))
		 	return false;
  } // initial_sync

/*  if ((h_layer = (headerstring >> 17) & 3) == 0)
  {
	 cerr << "unknown layer identifier found!\n";
	 exit (1);
  }
  h_layer = 4 - h_layer;		// now 1 means Layer I and 3 means Layer III   */

  h_layer   = 4 - (headerstring >> 17) & 3;

  h_protection_bit = (headerstring >> 16) & 1;


/*  if ((h_bitrate_index = (headerstring >> 12) & 0xF) == 15)
  {
	 cerr << "unknown bitrate index found!\n";
	 exit (1);
  }           */
/*  if (!h_bitrate_index)
  {
	 cerr << "free format not yet implemented!\n";
	 exit (1);
  } */

  h_bitrate_index  = (headerstring >> 12) & 0xF;

  h_padding_bit = (headerstring >> 9) & 1;
  h_mode        = (e_mode)((headerstring >> 6) & 3);

/*  if (h_layer == 2)
	 // testing validity of mode and bitrate:
	 if ((((h_bitrate_index >= 1 && h_bitrate_index <= 3) || h_bitrate_index == 5) &&
	 h_mode != single_channel) ||
	(h_bitrate_index >= 11 && h_mode == single_channel))
	 {
		cerr << "illegal combination of mode and bitrate in a layer II stream:\n"
			"  mode: " << mode_string ()
		<< "\n  bitrate: " << bitrate_string () << '\n';
		exit (1);
	 } */

  h_mode_extension = (headerstring >> 4) & 3;

  if (h_mode == joint_stereo)
	 h_intensity_stereo_bound = (h_mode_extension << 2) + 4;
  else
	 h_intensity_stereo_bound = 0;		// should never be used

  h_copyright = (bool) ((headerstring >> 3) & 1);
  h_original  = (bool) ((headerstring >> 2) & 1);

  // calculate number of subbands:
  if (h_layer == 1)
	 h_number_of_subbands = 32;
  else
  {
	 channel_bitrate = h_bitrate_index;

	 // calculate bitrate per channel:
	 if (h_mode != single_channel)
		if (channel_bitrate == 4)
			channel_bitrate = 1;
		else
			channel_bitrate -= 4;

	 if ((channel_bitrate == 1) || (channel_bitrate == 2))
		if (h_sample_frequency == thirtytwo)
			h_number_of_subbands = 12;
		else
			h_number_of_subbands = 8;
	 else
		if ((h_sample_frequency == fourtyeight) || ((channel_bitrate >= 3) &&
      													  	  (channel_bitrate <= 5)))
			h_number_of_subbands = 27;
		else
			h_number_of_subbands = 30;
  }

  if (h_intensity_stereo_bound > h_number_of_subbands)
     h_intensity_stereo_bound = h_number_of_subbands;


  // calculate framesize and nSlots
	calculate_framesize();

  // read framedata:
  if (stream->read_frame(framesize) == false)
     return false;

  if (!h_protection_bit)
  {
	 // frame contains a crc checksum
	 checksum = (uint16) stream->get_bits(16);
	 if (!crc)
       crc = new Crc16;
	 crc->add_bits(headerstring, 16);
	 *crcp = crc;
  }
  else
	 *crcp = NULL;

#ifdef SEEK_STOP
  if (h_sample_frequency == fourtyfour_point_one) {

	 if (!offset) {

    	uint32 max = max_number_of_frames(stream);

		offset = new uint32[max];

      for(uint32 i=0; i<max; i++)
      	offset[i] = 0;
    }

    {
     int32 cf = stream->current_frame();
     int32 lf = stream->last_frame();
	  if ((cf > 0) && (cf == lf)) {
		   offset[cf] = offset[cf-1] + h_padding_bit;
     } else {
  	      offset[0] = h_padding_bit;
     }
    }
  }
#endif // SEEK_STOP

  return true;
}

uint32 Header::calculate_framesize()
// calculates framesize in bytes excluding header size
{
  static const int32 bitrates[2][3][16] = {
  {{0 /*free format*/, 32000, 48000, 56000, 64000, 80000, 96000,
    112000, 128000, 144000, 160000, 176000, 192000 ,224000, 256000, 0},
   {0 /*free format*/, 8000, 16000, 24000, 32000, 40000, 48000,
    56000, 64000, 80000, 96000, 112000, 128000, 144000, 160000, 0},
   {0 /*free format*/, 8000, 16000, 24000, 32000, 40000, 48000,
    56000, 64000, 80000, 96000, 112000, 128000, 144000, 160000, 0}},
  {{0 /*free format*/, 32000, 64000, 96000, 128000, 160000, 192000,
	 224000, 256000, 288000, 320000, 352000, 384000, 416000, 448000, 0},
	{0 /*free format*/, 32000, 48000, 56000, 64000, 80000, 96000,
	 112000, 128000, 160000, 192000, 224000, 256000, 320000, 384000, 0},
	{0 /*free format*/, 32000, 40000, 48000, 56000, 64000, 80000,
	 96000, 112000, 128000, 160000, 192000, 224000, 256000, 320000, 0}}
  };

  if (h_layer == 1) {
	 framesize = (12 * bitrates[h_version][0][h_bitrate_index]) /
                 frequencies[h_version][h_sample_frequency];

	 if (h_padding_bit) framesize++;

	 framesize <<= 2;		// one slot is 4 bytes long

    nSlots = 0;

  } else {

	 framesize = (144 * bitrates[h_version][h_layer - 1][h_bitrate_index]) /
                 frequencies[h_version][h_sample_frequency];

	 if (h_version == MPEG2_LSF)
    	framesize >>= 1;

	 if (h_padding_bit) framesize++;

	 // Layer III slots
	 if (h_layer == 3) {

       if (h_version == MPEG1) {

			 nSlots = framesize - ((h_mode == single_channel) ? 17 : 32) // side info size
									  -  (h_protection_bit ? 0 : 2) 		       // CRC size
									  - 4; 								             // header size
       } else {  // MPEG-2 LSF
        	 nSlots = framesize - ((h_mode == single_channel) ?  9 : 17) // side info size
						   		  -  (h_protection_bit ? 0 : 2) 		       // CRC size
									  - 4; 								             // header size
       }
	 } else {
    	 nSlots = 0;
    }
  }

  framesize -= 4;             // subtract header size

  return framesize;
}

const char *Header::layer_string() const
{
  switch (h_layer)
  {
	 case 1:
		return "I";
	 case 2:
		return "II";
	 case 3:
		return "III";
  }
  return NULL;			// dummy
}


const char *Header::bitrate_string() const
{

  static const char *bitrate_str[2][3][16] = {
  {{"free format", "32 kbit/s", "48 kbit/s", "56 kbit/s", "64 kbit/s",
    "80 kbit/s", "96 kbit/s", "112 kbit/s", "128 kbit/s", "144 kbit/s",
    "160 kbit/s", "176 kbit/s", "192 kbit/s", "224 kbit/s", "256 kbit/s",
    "forbidden"},
   {"free format", "8 kbit/s", "16 kbit/s", "24 kbit/s", "32 kbit/s",
    "40 kbit/s", "48 kbit/s", "56 kbit/s", "64 kbit/s", "80 kbit/s",
    "96 kbit/s", "112 kbit/s", "128 kbit/s", "144 kbit/s", "160 kbit/s",
    "forbidden"},
   {"free format", "8 kbit/s", "16 kbit/s", "24 kbit/s", "32 kbit/s",
    "40 kbit/s", "48 kbit/s", "56 kbit/s", "64 kbit/s", "80 kbit/s",
    "96 kbit/s", "112 kbit/s", "128 kbit/s", "144 kbit/s", "160 kbit/s",
    "forbidden"}},
  {{"free format", "32 kbit/s", "64 kbit/s", "96 kbit/s", "128 kbit/s",
    "160 kbit/s", "192 kbit/s", "224 kbit/s", "256 kbit/s", "288 kbit/s",
    "320 kbit/s", "352 kbit/s", "384 kbit/s", "416 kbit/s", "448 kbit/s",
    "forbidden"},
	{"free format", "32 kbit/s", "48 kbit/s", "56 kbit/s", "64 kbit/s",
    "80 kbit/s", "96 kbit/s", "112 kbit/s", "128 kbit/s", "160 kbit/s",
    "192 kbit/s", "224 kbit/s", "256 kbit/s", "320 kbit/s", "384 kbit/s",
    "forbidden"},
	{"free format", "32 kbit/s", "40 kbit/s", "48 kbit/s", "56 kbit/s",
    "64 kbit/s", "80 kbit/s" , "96 kbit/s", "112 kbit/s", "128 kbit/s",
    "160 kbit/s", "192 kbit/s", "224 kbit/s", "256 kbit/s", "320 kbit/s",
    "forbidden"}}
  };

  return bitrate_str[h_version][h_layer - 1][h_bitrate_index];
}

const char *Header::sample_frequency_string() const
{
  switch (h_sample_frequency)
  {
	 case thirtytwo:
    	if (h_version == MPEG1)
			return "32 kHz";
      else
      	return "16 kHz";
	 case fourtyfour_point_one:
    	if (h_version == MPEG1)
			return "44.1 kHz";
      else
      	return "22.05 kHz";
	 case fourtyeight:
    	if (h_version == MPEG1)
			return "48 kHz";
      else
      	return "24 kHz";
  }
  return(NULL);			// dummy
}

const char *Header::mode_string() const
{
  switch (h_mode)
  {
	 case stereo:
		return "Stereo";
	 case joint_stereo:
		return "Joint stereo";
	 case dual_channel:
		return "Dual channel";
	 case single_channel:
		return "Single channel";
  }
  return NULL;			// dummy
}

const char *Header::version_string() const
{
 	switch (h_version)
   {
   	case MPEG1:
        return "MPEG-1";
      case MPEG2_LSF:
        return "MPEG-2 LSF";
   }
   return(NULL);
}

#ifdef SEEK_STOP
// Stream searching routines
bool Header::stream_seek(Ibitstream *stream, uint32 seek_pos)
{
   return((h_sample_frequency == fourtyfour_point_one) ?
          stream->seek_pad(seek_pos, framesize - h_padding_bit,
                           this, offset) :
     		 stream->seek(seek_pos, framesize));
}
#endif

uint32 Header::max_number_of_frames(Ibitstream *stream) const
// Returns the maximum number of frames in the stream
{
	return(stream->file_size() / (framesize + 4 - h_padding_bit));
}

uint32 Header::min_number_of_frames(Ibitstream *stream) const
// Returns the minimum number of frames in the stream
{
   return(stream->file_size() / (framesize + 5 - h_padding_bit));
}

real Header::ms_per_frame() const
{
	static real ms_per_frame_array[3][3] = {{8.707483f,  8.0f, 12.0f},
												       {26.12245f, 24.0f, 36.0f},
   	                                     {26.12245f, 24.0f, 36.0f}};

	return ms_per_frame_array[h_layer-1][h_sample_frequency];
}

real Header::total_ms(Ibitstream *stream) const
{
	return(max_number_of_frames(stream) * ms_per_frame());
}

