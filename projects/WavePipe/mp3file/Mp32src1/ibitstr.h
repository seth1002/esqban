/*  ibitstr.cpp
 *
 *  Input bitstream class declarations
 */

/*
 *  @(#) ibitstream.h 1.5, last edit: 6/15/94 16:55:34
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
 *  Changes made by Jeff Tsay :
 *  04/14/97 : Added function prototypes for new syncing and seeking
 *  mechanisms. Also made this file portable.
 */

#ifndef BITSTREAM_H
#define BITSTREAM_H

#ifdef __WIN32__
#define STRICT
#include <wtypes.h>
#endif // __WIN32__

#include "all.h"

enum e_syncmode { INITIAL_SYNC, STRICT_SYNC };

class Header;	// forward declaration so we can use the type

const uint32 bufferintsize = 433;
	// max. 1730 bytes per frame: 144 * 384kbit/s / 32000 Hz + 2 Bytes CRC

// Class to extract bitstrings from files:
class Ibitstream
{

private:

#ifdef __WIN32__
  HANDLE fd;
#else
  int fd;
#endif

  uint32	buffer[bufferintsize];
  uint32	framesize;		// number of valid bytes in buffer
  uint32	*wordpointer;	// position of next unsigned int for get_bits()
  uint32	bitindex;		// number (0-31, from MSB to LSB) of next bit for get_bits()
  uint32 syncword;
  bool   single_ch_mode;

  int32  current_frame_number;

#ifdef SEEK_STOP
  int32  last_frame_number;
#endif

  bool   copy;          // true if this is a copy (don't close file handle)

public:

  Ibitstream(const char *filename);
  Ibitstream(const Ibitstream &s0);
  Ibitstream();

  Ibitstream &operator = (const Ibitstream &s0);

  ~Ibitstream();

  bool	get_header(uint32 *headerstring , enum e_syncmode syncmode);
		// get next 32 bits from bitstream in an unsigned int,
		// returned value False => end of stream
  bool	read_frame(uint32 bytesize);
		// fill buffer with data from bitstream, returned value False => end of stream
  uint32	get_bits(uint32 number_of_bits);
		// read bits (1 <= number_of_bits <= 16) from buffer into the lower bits
		// of an unsigned int. The LSB contains the latest read bit of the stream.

  void   set_syncword(uint32 syncword0);
  		// Set the word we want to sync the header to, in
      // Big-Endian byte order

  int32  current_frame() const { return(current_frame_number); }

  uint32 file_size() const;
		// Returns the size, in bytes, of the input file.

  // Stream searching routines (Jeff Tsay)

#ifdef SEEK_STOP

  int32  last_frame() const { return(last_frame_number); }

  bool   seek(int32 frames, int32 frame_size);
		// Seeks to frames

  bool   seek_pad(int32 frames, int32 frame_size,
  						Header *header, uint32 *offset);
		// Seeks frames for 44.1 or 22.05 kHz (padded) files

#endif

};

#endif
