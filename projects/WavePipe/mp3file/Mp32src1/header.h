/* header.h

	Declarations for MPEG header class

	A few layer III, MPEG-2 LSF, and seeking modifications made by Jeff Tsay.

   Last modified : 04/19/97 */

/*
 *  @(#) header.h 1.7, last edit: 6/15/94 16:55:33
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

#ifndef HEADER_H
#define HEADER_H

#include "all.h"
#include "ibitstr.h"
#include "crc.h"

enum e_version { MPEG2_LSF, MPEG1 };
enum e_mode { stereo, joint_stereo, dual_channel, single_channel };
enum e_sample_frequency { fourtyfour_point_one, fourtyeight, thirtytwo };

// Class for extraction information from a frame header:
class Header
{

private:

  static const uint32	frequencies[2][4];
  uint32		h_layer, h_protection_bit, h_bitrate_index,
				h_padding_bit, h_mode_extension;
  e_version h_version;
  e_mode		h_mode;
  e_sample_frequency	h_sample_frequency;
  uint32		h_number_of_subbands, h_intensity_stereo_bound;
  bool		h_copyright, h_original;
  bool      initial_sync;
  Crc16		*crc;
  uint32    *offset;
  uint16		checksum;
  uint32    framesize;
  uint32    nSlots;

public:

  Header();
  Header(const Header &h0);

  ~Header();

  Header& operator = (const Header &h0);

  bool read_header(Ibitstream *stream, Crc16 **crc);
  // read a 32-bit header from the bitstream

  // functions to query header contents:
  e_version version()		 const { return h_version; }
  uint32		layer()         const { return h_layer; }
  uint32		bitrate_index() const { return h_bitrate_index; }

  e_sample_frequency	sample_frequency() const
  {
     return h_sample_frequency;
  }

  uint32		frequency() const
  {
     return frequencies[h_version][h_sample_frequency];
  }

  e_mode		mode()         const { return h_mode; }
  bool		checksums()    const { return (bool) !h_protection_bit; }
  bool		copyright()    const { return h_copyright; }
  bool		original()     const { return h_original; }

  bool		checksum_ok () const { return (bool) (checksum == crc->checksum ()); }
			// compares computed checksum with stream checksum

  // Seeking and layer III stuff
  bool      padding()        const { return (bool) h_padding_bit; }
  uint32    slots()          const { return nSlots; }
  uint32		mode_extension() const { return h_mode_extension; }
  uint32		calculate_framesize(); // made public

  // functions which return header informations as strings:
  const char *	layer_string() const;
  const char *	bitrate_string() const;
  const char *	sample_frequency_string() const;
  const char *	mode_string() const;
  const char * version_string() const;

  uint32		number_of_subbands() const
  // returns the number of subbands in the current frame
  {
     return h_number_of_subbands;
  }

  uint32		intensity_stereo_bound() const
  // (Layer II joint stereo only)
  // returns the number of subbands which are in stereo mode,
  // subbands above that limit are in intensity stereo mode
  {
     return h_intensity_stereo_bound;
  }

  // Scrolling stuff

#ifdef SEEK_STOP
  bool      stream_seek(Ibitstream *stream, uint32 seek_pos);
#endif // SEEK_STOP

  uint32    max_number_of_frames(Ibitstream *stream) const;
  uint32    min_number_of_frames(Ibitstream *stream) const;

  real      ms_per_frame() const;
  				// milliseconds per frame, for time display

  real      total_ms(Ibitstream *stream) const;

};

#endif
