/* ibitstr.cpp

	Input bitstream class implementation

   Changes by Jeff Tsay:

	01/26/96 : Modified for Windows file handles.

   11/29/96 : Added syncword detection for compatibility with streams produced
   by DALET. Changed at the request of Wilfried Solbach, thanks for the
   donation in advance! However Layer I MPP files playback jerkily.

   03/09/97 : Added a routine to read layer III side info. Also not mentioned
   previously are several routines that allow seeking in a stream.

   04/14/97 : Moved the side info routine to the layer III decoder object.
   Revamped the seeking mechanism and also included better syncword detection.
   Moved the original file reading mechanisms back for better
   portability, and used John Fehr's implementations of the seeking routines
	for non-Win32 file reading. */

/*
 *  @(#) ibitstream.cc 1.8, last edit: 6/15/94 16:51:45
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
 *    - third argument in open syscall added
 *    - minor bug in get_header() fixed
 */

#ifdef  __WIN32__
#include <windows.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#endif   // __WIN32__

#ifndef  GUI
#include <iostream.h>
#endif   // GUI

#include "all.h"
#include "ibitstr.h"
#include "header.h"

#ifdef  DAMN_INTEL_BYTE_ORDER
#define swap_int32(int_32) (((int_32) << 24) | (((int_32) << 8) & 0x00ff0000) | \
				               (((int_32) >> 8) & 0x0000ff00) | ((int_32) >> 24))
#endif  // DAMN_INTEL_BYTE_ORDER


Ibitstream::Ibitstream(const char *filename)
{

#ifdef __WIN32__

  SECURITY_ATTRIBUTES security;
  security.nLength              = sizeof(SECURITY_ATTRIBUTES);
  security.lpSecurityDescriptor = NULL;
  security.bInheritHandle       = FALSE;

  fd = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ,
                  &security, OPEN_EXISTING, NULL, NULL);

  if (fd == INVALID_HANDLE_VALUE) {
     char bad_file_msg[256];
     lstrcpy(bad_file_msg, "Error opening file: ");

#ifdef WIN32GUI
	  MessageBox(NULL, lstrcat(bad_file_msg, filename) , "Invalid handle", MB_OK);
#else
	cerr  << "Error opening file: " << filename << endl;
#endif // WIN32GUI

     return;
  }

#else
  // copy any arguments you need, open the file, and check for error

  if ((fd = open(filename, O_RDONLY)) == -1)
  {
#ifndef GUI
  		cerr << "Error opening file: " << filename << endl;
#endif
      return;
  }

#endif // __WIN32__

  wordpointer       = buffer;
  bitindex          = 0;

  // Seeking variables
  current_frame_number = -1;

#ifdef SEEK_STOP
  last_frame_number    = -1;
#endif

  copy = false;
}

Ibitstream::Ibitstream(const Ibitstream &s0)
{

  int32 i;

  fd = s0.fd;

  for (i=0; i<bufferintsize; i++)
  	   buffer[i] = s0.buffer[i];

  framesize = s0.framesize;

  wordpointer = buffer + (s0.wordpointer - s0.buffer);

  bitindex = s0.bitindex;
  syncword = s0.syncword;
  single_ch_mode = s0.single_ch_mode;

  current_frame_number = s0.current_frame_number;

#ifdef SEEK_STOP
  last_frame_number = s0.last_frame_number;
#endif

  copy = true;
}

// Dummy constructor
Ibitstream::Ibitstream()
{

  fd = 0;

  wordpointer       = buffer;
  bitindex          = 0;

  current_frame_number = -1;

#ifdef SEEK_STOP
  last_frame_number    = -1;
#endif

  copy = false;
}

Ibitstream::~Ibitstream()
{
  // close the file
#ifdef __WIN32__
  if (fd && !copy) CloseHandle(fd);
#else
  if (fd && !copy) close(fd);
#endif
}

Ibitstream &Ibitstream::operator = (const Ibitstream &s0)
{
  int32 i;

  fd = s0.fd;

  for (i=0; i<bufferintsize; i++)
  	   buffer[i] = s0.buffer[i];

  framesize = s0.framesize;

  wordpointer = buffer + (s0.wordpointer - s0.buffer);

  bitindex = s0.bitindex;
  syncword = s0.syncword;
  single_ch_mode = s0.single_ch_mode;

  current_frame_number = s0.current_frame_number;

#ifdef SEEK_STOP
  last_frame_number = s0.last_frame_number;
#endif

	copy = true;

	return *this;
}

bool Ibitstream::get_header (uint32 *headerstring, enum e_syncmode syncmode)
{
  bool result, sync;

  int32 numread;

  do {

	// Read 4 bytes from the file, placing the number of bytes actually
   // read in numread
#ifdef __WIN32__
   result = (bool) ReadFile(fd, (char *) headerstring, 4,
   							    (DWORD *) &numread, NULL);
#else
	if (fd == -1) return false; // check for error opening file
	result = (bool) ((numread=read(fd, (char*) headerstring, 4)) == 4);
#endif

   if (!(result && (numread == 4)))
      return false;

#ifdef DAMN_INTEL_BYTE_ORDER
	if (syncmode == INITIAL_SYNC) {
	   sync =  ((*headerstring & 0x0000F0FF) == 0x0000F0FF);
   } else {
   	sync =  ((*headerstring & 0x000CF8FF) == syncword) &&
             (((*headerstring & 0xC0000000) == 0xC0000000) == single_ch_mode);
   }
#else
	if (syncmode == INITIAL_SYNC) {
		sync =  ((*headerstring & 0xFFF00000) == 0xFFF00000);

	} else {
   	sync =  ((*headerstring & 0xFFF80C00) == syncword) &&
             (((*headerstring & 0x000000C0) == 0x000000C0) == single_ch_mode);
   }
#endif // DAMN_INTEL_BYTE_ORDER

   if (!sync)
 	// rewind 3 bytes in the file so we can try to sync again, if
   // successful set result to true
#ifdef __WIN32__
      result = (bool) SetFilePointer(fd, -3, NULL, FILE_CURRENT);
#else
		result = (bool) (lseek(fd, -3, SEEK_CUR) != -1);
#endif // __WIN32__

  } while (!sync && result);

  if (!result)
     return false;

#ifdef DAMN_INTEL_BYTE_ORDER
  uint32 header = *headerstring;
  *headerstring = swap_int32(header);
#endif // DAMN_INTEL_BYTE_ORDER

  current_frame_number++;

#ifdef SEEK_STOP
  if (last_frame_number < current_frame_number)
	  last_frame_number = current_frame_number;
#endif

  return true;
}

bool Ibitstream::read_frame(uint32 bytesize)
{

  int32 numread;

  // read bytesize bytes from the file, placing the number of bytes
  // actually read in numread and setting result to true if
  // successful
#ifdef __WIN32__
  bool result = ReadFile(fd, buffer, bytesize, (DWORD *) &numread, NULL);
#else
  bool result = (bool) ((numread=read(fd,buffer,bytesize)) != -1);
#endif // __WIN32__


/*  if (bytesize > (bufferintsize << 2))
  {
	 cerr << "Internal error: framelength > bufferlength?!\n";
	 exit (1);
  } */

  wordpointer = buffer;
  bitindex    = 0;
  framesize   = bytesize;

#ifdef DAMN_INTEL_BYTE_ORDER
  uint32 *wordp;
  for (wordp = buffer + ((bytesize - 1) >> 2); wordp >= buffer; --wordp)
  {

#ifdef INTEL386

  // inline 386 assembly doesn't work on Borland compiler

  __asm {
      mov eax, wordp
		mov ebx, [eax]
      bswap ebx
      mov [eax], ebx
  }

#else

	 uint32 word   = *wordp;
    *wordp = swap_int32(word);

#endif // INTEL386

  } // for (wordp ...

#endif // DAMN_INTEL_BYTE_ORDER

  return((bool) (result && (numread == framesize)));
}

uint32 Ibitstream::get_bits(uint32 number_of_bits)
{
  static uint32 bitmask[18] =
  {
	 0,	// dummy
	 0x00000001, 0x00000003, 0x00000007, 0x0000000F,
	 0x0000001F, 0x0000003F, 0x0000007F, 0x000000FF,
	 0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF,
	 0x00001FFF, 0x00003FFF, 0x00007FFF, 0x0000FFFF,
    0x0001FFFF
  };
  uint32 returnvalue;
  uint32 sum = bitindex + number_of_bits;

  if (sum <= 32)
  {
	 // all bits contained in *wordpointer
	 returnvalue = (*wordpointer >> (32 - sum)) & bitmask[number_of_bits];
	 if ((bitindex += number_of_bits) == 32)
	 {
		bitindex = 0;

/*		if ((char *)++wordpointer > (char *)buffer + framesize)
		{
	cerr << "Ibitstream::get_bits(): no more bits in buffer!\n";
	exit (1);
		} */
		wordpointer++; // added by me!
	 }
	 return returnvalue;
  }

#ifdef DAMN_INTEL_BYTE_ORDER
  *((int16 *)&returnvalue + 1) = *(int16 *)wordpointer;
  wordpointer++; // Added by me!
  *(int16 *)&returnvalue = *((int16 *)wordpointer + 1);
#else
  *(int16 *)&returnvalue = *((int16 *)wordpointer + 1);
  wordpointer++; // Added by me!
  *((int16 *)&returnvalue + 1) = *(int16 *)wordpointer;
#endif // DAMN_INTEL_BYTE_ORDER

  returnvalue >>= 48 - sum;	// returnvalue >>= 16 - (number_of_bits - (32 - bitindex))
  returnvalue &= bitmask[number_of_bits];
  bitindex = sum - 32;

  return returnvalue;
}

void Ibitstream::set_syncword(uint32 syncword0)
{

#ifdef DAMN_INTEL_BYTE_ORDER
   syncword = swap_int32(syncword0 & 0xFFFFFF3F);
#else
	syncword = syncword0 & 0xFFFFFF3F;
#endif // DAMN_INTEL_BYTE_ORDER

   single_ch_mode = ((syncword0 & 0x000000C0) == 0x000000C0);
}

uint32 Ibitstream::file_size() const
// return the file size of the file
{

#ifdef __WIN32__
	return GetFileSize(fd, NULL);
#else
	// hmm. I think this should work  -- John Fehr
	off_t o_off;
	off_t len;
	o_off = lseek(fd, 0, SEEK_CUR);
	len   = lseek(fd, 0, SEEK_END);
	lseek(fd, o_off, SEEK_SET);
	return len;
#endif // __WIN32__

}

#ifdef SEEK_STOP

bool Ibitstream::seek(int32 frame, int32 frame_size)
// set the file pointer to frame * (frame_size + 4) bytes after the
// beginning of the file and return true if successful
{
	current_frame_number = frame - 1;

#ifdef __WIN32__
	return(SetFilePointer(fd, frame * (frame_size + 4), NULL, FILE_BEGIN)
   	    != 0xFFFFFFFF);
#else

	return lseek(fd, frame * (frame_size + 4), SEEK_SET);
#endif // __WIN32__

}

bool Ibitstream::seek_pad(int32 frame, int32 base_frame_size,
                          Header *header, uint32 *offset)
{
	// base_frame_size is the frame size _without_ padding.

	Crc16 *crc = NULL;

	int32 total_frame_size = base_frame_size + 4;
	int32 diff;

	if (last_frame_number < frame) {
		diff = (last_frame_number >= 0) ?  offset[last_frame_number] : 0;

   	// set the file pointer to ((last_frame_number+1) * total_frame_size)
      // bytes after the beginning of the file
#ifdef __WIN32__
		SetFilePointer(fd, (last_frame_number + 1) * total_frame_size +
							diff, NULL, FILE_BEGIN);
#else
		lseek(fd, (last_frame_number + 1) * total_frame_size + diff, SEEK_SET);
#endif // __WIN32__

		current_frame_number = last_frame_number;

		do {
			if (!header->read_header(this, &crc)) // will increment last_frame_number
				return false;
		} while (last_frame_number < frame);

      return true;

	} else {
		diff = (frame > 0) ? offset[frame - 1] : 0;

   	// set the file pointer to (frame * total_frame_size  + diff) bytes
      // after the beginning of the file
#ifdef __WIN32__
		SetFilePointer(fd, frame * total_frame_size + diff, NULL, FILE_BEGIN);
#else
		lseek(fd, frame * total_frame_size + diff, SEEK_SET);
#endif // __WIN32__

		current_frame_number = frame - 1;

      return header->read_header(this, &crc);
	}
}

#endif // SEEK_STOP


