/*  @(#) cep_obuffer.cpp, last edit: 4/9/1998
 *  @(#) Copyright (C) 1998 Syntrillium Software (www.syntrillium.com)
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
 *

	
	The only output buffer thing we need for Cool Edit integeration


*/

#ifdef  __WIN32__
#include <windows.h>

#ifndef WIN32GUI
#include <iostream.h>
#endif

#include "header.h"
#include "args.h"
#include "obuffer.h"
#include "CEP_Obuffer.h"


#define MAXFRAMESFORWRITE 12

CEP_Obuffer::CEP_Obuffer(uint32 number_of_channels,
										     MPEG_Args *maplay_args)
{
  channels  = number_of_channels;
  data_size = channels * OBUFFERSIZE * 2; // double to make for floats

  if (maplay_args->MPEGheader->version() == MPEG2_LSF)
     data_size >>= 1;

  if (maplay_args->MPEGheader->layer() == 1)
  	  data_size /= 3;

  temp = new BYTE[data_size*MAXFRAMESFORWRITE];

  uint32 i;
  for(i=0; i<channels; i++)
	   bufferp[i] = i << 2;	// corrected, was i * channels
}

void CEP_Obuffer::append (uint32 channel, real value)
{
  // Need to break up the 32-bit integer into 2 8-bit bytes.
  // (ignore the first two bytes - either 0x0000 or 0xffff)
  // Note that Intel byte order is backwards!!!

	float * pTemp=(float *)(temp+bufferp[channel]);
	*pTemp=(float)(value);
	
  //temp[bufferp[channel]]   = (BYTE) (value & 0xff);
  //temp[bufferp[channel]+1] = (BYTE) (value >> 8);

  bufferp[channel] += channels << 2;	// byte advancing

  return;
}

void CEP_Obuffer::appendblock (uint32 channel, real * pvalues, int iCount)
{
  // Need to break up the 32-bit integer into 2 8-bit bytes.
  // (ignore the first two bytes - either 0x0000 or 0xffff)
  // Note that Intel byte order is backwards!!!
	float * pTemp=(float *)(temp+bufferp[channel]);
	int iIncrement=(channels<<2)/4;
	
	bufferp[channel]+=(channels<<2)*iCount;

	while (iCount--)
	{	*pTemp=*pvalues++;
		pTemp+=iIncrement;
	}
	
	return;
}

void CEP_Obuffer::write_buffer(int32 fd)
{ 
	// we really don't need to do anything but reset here
  // Reset buffer pointers
  //uint32 i;
  //for(i=0; i<channels; i++)
  // 	bufferp[i] = i * channels;

  return;
}

BYTE * CEP_Obuffer::get_buffer(int * piNumBytes)
{
	*piNumBytes=bufferp[channels-1]-((channels-1)<<2);
	
	// And reset for next time through
	uint32 i;
    for(i=0; i<channels; i++)
	   bufferp[i] = i << 2;	// corrected, was i * channels

	return temp;
}

CEP_Obuffer::~CEP_Obuffer()
{
	// Mark the current chunk as dirty and flush it
   // Free the buffer memory
 	delete [] temp;
}

Obuffer *create_CEP_Obuffer(MPEG_Args *maplay_args)
{
	Obuffer *buffer;

	enum e_mode mode = maplay_args->MPEGheader->mode();
	enum e_channels which_channels = maplay_args->which_c;

	if ((mode == single_channel) || (which_channels != both))
		buffer = new CEP_Obuffer(1, maplay_args);	// mono
	else
		buffer = new CEP_Obuffer(2, maplay_args);	// stereo

	return(buffer);
}

#endif // __WIN32__
