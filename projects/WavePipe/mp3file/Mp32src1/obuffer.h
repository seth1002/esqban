/* obuffer.h

   Declarations for output buffer, includes operating system
   implementation of the virtual Obuffer. Optional routines
   enabling seeks and stops added by Jeff Tsay. */

/*
 *  @(#) obuffer.h 1.8, last edit: 6/15/94 16:51:56
 *  @(#) Copyright (C) 1993, 1994 Tobias Bading (bading@cs.tu-berlin.de)
 *  @(#) Berlin University of Technology
 *
 *  Idea and first implementation for u-law output with fast downsampling by
 *  Jim Boucher (jboucher@flash.bu.edu)
 *
 *  LinuxObuffer class written by
 *  Louis P. Kruger (lpkruger@phoenix.princeton.edu)
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

#ifndef OBUFFER_H
#define OBUFFER_H

#include "all.h"
#include <stdio.h> // needed for definition of NULL
#include "args.h"

static const uint32 OBUFFERSIZE = 2 * 1152;	// max. 2 * 1152 samples per frame
static const uint32 MAXCHANNELS = 2;         // max. number of channels

// Abstract base class for audio output classes:
class Obuffer
{
public:

  virtual     ~Obuffer() {}		// dummy

#ifdef COOLPRO
  virtual void append(uint32 channel, real value) = 0;
			 // this function takes a 32 Bit float value from -1 to 1
#else
  virtual void append(uint32 channel, int16 value) = 0;
			 // this function takes a 32 Bit float value from -1 to 1
#endif
  virtual void write_buffer(int32 fd) = 0;
			 // this function should write the samples to the filedescriptor
			 // or directly to the audio hardware
#ifdef COOLPRO
  // OK - appendblock is not used, but it was a stab at speeding things up
  virtual void appendblock(uint32 channel, real * pvalues, int iCount) = 0;
			// this function takes an array of 32 bit float values from -1 to 1
  virtual BYTE * get_buffer(int * piNumBytes) = 0;
			// used in Cool Edit version
#endif

#ifdef SEEK_STOP
  virtual void clear_buffer() = 0;
			 // Clears all data in the buffer (for seeking)

  virtual void set_stop_flag() = 0;
			 // Notify the buffer that the user has stopped the stream
#endif // SEEK_STOP

};

Obuffer *create_obuffer(MPEG_Args *maplay_args);

#ifndef COOLPRO
#include "fileobuf.h"
#endif

#ifdef IRIX
#include "indigo_obuffer.h"
#endif

#ifdef SPARC
#include "sparc_obuffer.h"
#endif

#ifdef HPUX
#include "hpux_obuffer.h"
#endif

#if defined(LINUX) || defined(__FreeBSD__)
#include "linux_obuffer.h"
#endif

#ifdef NeXT
#include "NeXT_obuffer.h"
#endif

#ifdef AIX
#include "aix_obuffer.h"
#endif

#ifdef __WIN32__
#ifdef COOLPRO
#include "cep_obuffer.h"
#else
#include "mci_obuf.h"
#include "wavefile_obuffer.h"
#endif
// #include "ds_obuffer.h"
#endif

#ifdef BEOS
#include "beos_obuffer.h"
#endif

#ifdef OS2
#include "os2_obuffer.h"
#endif

#endif // OBUFFER_H
