/*
 *  @(#) ceglue.cpp, last edit: 4/9/1998
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
 * Basically we are taking what we need from mp2win.cpp and doing a 
 * non-independent-thread (non Windows GUI) version of maplay.
 *
 *
 */

#define  STRICT
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <shlobj.h>

#include "all.h"
#include "crc.h"
#include "ibitstr.h"
#include "header.h"
#include "args.h"


#include "subband.h"
#include "sublay1.h"
#include "sublay2.h"
#include "synfilt.h"
#include "ibitstr.h"
#include "obuffer.h"
#include "layer3.h"

extern "C"
{
#include "ceglue.h"
}

#include <math.h>

#define NUM_RFL_ENTRIES 4
#define REMOTE_THRESH   5


BOOL MPEG = FALSE;
enum e_channels mode = both;
// Dummy CRC pointer
Crc16 *crc;
SynthesisFilter *filter1 = NULL, *filter2 = NULL;
Obuffer *buffer = NULL;
LayerIII_Decoder *l3decoder = NULL;
enum e_mode actualmode;


uint32 layer;
// These arguments should not change while decoding
Ibitstream *stream = NULL;
Header *header     = NULL;
enum e_channels which_channels;
BOOL bComplete=FALSE;

MPEG_Args _maplay_args;
MPEG_Args *maplay_args = &_maplay_args;

void maplay_Exit(uint32 returncode)
{

  delete buffer;
  buffer = NULL;

  delete filter1;
  filter1 = NULL;
  delete filter2;
  filter2 = NULL;

  delete l3decoder;
  l3decoder = NULL;
}


BOOL CPPMPEG_GetInfo(MPEG_INFO * mpi)
{
	if (!MPEG)
		return FALSE;
	if (!header)
		return FALSE;
	
	mpi->iLayer			=(int)header->layer();
	mpi->iVersion		=(int)header->version();
	mpi->iStereoMode	=(int)header->mode();
	mpi->iSamplerate	=(int)header->frequency();
	mpi->iChannels		=(header->mode()==single_channel)?1:2;
	mpi->bIsChecksummed	=(BOOL)header->checksums();
	mpi->bIsCopyrighted	=(BOOL)header->copyright();
	mpi->bIsOriginal	=(BOOL)header->original();
	mpi->iBitrateIndex	=(int)header->bitrate_index();
	mpi->iFrameSize		=(int)header->calculate_framesize();
	mpi->fLengthInMilliseconds=header->total_ms(stream);

	lstrcpy(mpi->szBitrate,header->bitrate_string());
	lstrcpy(mpi->szVersion,header->version_string());

	return TRUE;
}

// header_copy.total_ms(&stream_copy) in CPPMPEG_GetInfo
BOOL CPPMPEG_Open(LPSTR szName)
{
	MPEG = TRUE;
	bComplete=FALSE;
	real own_scalefactor = 32768.0f;
	//args->mutex = CreateMutex(NULL, FALSE, "m");
	stream      = new Ibitstream(szName);
	header = new Header;

   
	if (!header->read_header(stream, &crc)){
		return FALSE;
	}


	// AND GET THINGS STARTED!
	crc         = NULL;
	which_channels = both;	// or left, right, downmix
	bComplete=FALSE;

   // get info from header of first frame:
   layer = header->layer();
   if ((actualmode=header->mode())== single_channel)
	   which_channels = left;

   // create filter(s):
   {
      real scalefactor = own_scalefactor; // insert any scal factor here

		filter1 = new SynthesisFilter(0, scalefactor);

	   if ((actualmode != single_channel) && (which_channels == both))
		   filter2 = new SynthesisFilter(1, scalefactor);
   }


   // set up args for buffer class to use
   maplay_args->MPEGheader=header;
   maplay_args->which_c=which_channels;


   // create buffer, and check to see if created ok:
   buffer=create_CEP_Obuffer(maplay_args);

   if (buffer == NULL) {
		maplay_Exit(0);
      return 1;
   }

  // Layer III : initialize decoder

  	if (layer == 3)
      l3decoder = new LayerIII_Decoder(stream, header, filter1,
      											filter2, buffer, which_channels);

	return TRUE;
}

// Returns number of bytes, or zero for complete
int CPPMPEG_Read(BYTE * pBuffer)
{
	bool read_ready = false, write_ready = false;
   	int iRead;

	if (bComplete)
		return 0;
	// is there a change in important parameters?
	// (bitrate switching is allowed)
	if (header->layer() != layer)
	{
		// layer switching is allowed
	  if (header->layer() == 3) {
         l3decoder = new LayerIII_Decoder(stream, header,
           											filter1, filter2,
                                          buffer, which_channels);
      } else if (layer == 3) {
      	delete l3decoder;
         l3decoder = NULL;
      }

		layer = header->layer();
	 }

	 if (layer != 3) {

	    Subband *subbands[32];
	    uint32 num_subbands = header->number_of_subbands();
       uint32 i;
       actualmode = header->mode();

		 // create subband objects:
		 if (layer == 1)
		 {
			if (actualmode == single_channel)
				for (i = 0; i < num_subbands; ++i)
				  subbands[i] = new SubbandLayer1(i);
			else if (actualmode == joint_stereo) {
				for (i = 0; i < header->intensity_stereo_bound(); ++i)
				  subbands[i] = new SubbandLayer1Stereo(i);
				for (; i < num_subbands; ++i)
				  subbands[i] = new SubbandLayer1IntensityStereo(i);
			} else {
				for (i = 0; i < num_subbands; ++i)
				  subbands[i] = new SubbandLayer1Stereo(i);
	      }

		 } else { // Layer II
			if (actualmode == single_channel)
				for (i = 0; i < num_subbands; ++i)
		   	  subbands[i] = new SubbandLayer2(i);
			else if (actualmode == joint_stereo)
			{
				for (i = 0; i < header->intensity_stereo_bound(); ++i)
			 	    subbands[i] = new SubbandLayer2Stereo(i);
				for (; i < num_subbands; ++i)
				    subbands[i] = new SubbandLayer2IntensityStereo(i);
			} else {
				for (i = 0; i < num_subbands; ++i)
				    subbands[i] = new SubbandLayer2Stereo(i);
         }
	 	 }

  	    // start to read audio data:
	    for (i = 0; i < num_subbands; ++i)
	       subbands[i]->read_allocation(stream, header, crc);

		 if (layer == 2)
			for (i = 0; i < num_subbands; ++i)
				((SubbandLayer2 *)subbands[i])->read_scalefactor_selection(stream,
            																		     crc);

		 if (!crc || header->checksum_ok())
		 {
			// no checksums or checksum ok, continue reading from stream:
			for (i = 0; i < num_subbands; ++i)
				subbands[i]->read_scalefactor(stream, header);

			do
			{
				for (i = 0; i < num_subbands; ++i)
					read_ready = subbands[i]->read_sampledata(stream);

				do
				{
					for (i = 0; i < num_subbands; ++i)
						write_ready = subbands[i]->put_next_sample(which_channels,
   	                                                       filter1, filter2);

					filter1->calculate_pcm_samples(buffer);
					if ((which_channels == both) && (actualmode!= single_channel))
               	filter2->calculate_pcm_samples(buffer);
				} while (!write_ready);
			} while (!read_ready);

	      buffer->write_buffer(1);

		 } // checksum ok
	 // Jeff : Don't let user know if crc violated.
//	    else
		// Sh*t! Wrong crc checksum in frame!
//		cerr << "WARNING: frame contains wrong crc checksum! (throwing frame away)\n";

       for (i = 0; i < num_subbands; ++i)
			delete subbands[i];

    } else {  // Layer III
	   l3decoder->decode();
    }

   if (!header->read_header(stream, &crc))
	   bComplete=TRUE;

	BYTE * pBuf=buffer->get_buffer(&iRead);
    CopyMemory(pBuffer,pBuf,iRead);

	return iRead;
}

void CPPMPEG_Close()
{	
	maplay_Exit(0);

	if (MPEG) 
	{
   		delete stream;
		stream = NULL;

		delete header;
		header = NULL;
	}
}

// To interface with the Cool Edit File Filters API, we make "C" callable functions

extern "C" {

	void MPEG_Close()
	{
		CPPMPEG_Close();
	}

	int MPEG_Read(BYTE * pBuffer)
	{
		return CPPMPEG_Read(pBuffer);
	}

	BOOL MPEG_Open(LPSTR szName)
	{
		return CPPMPEG_Open(szName);
	}

	BOOL MPEG_GetInfo(MPEG_INFO * mpi)
	{
		return CPPMPEG_GetInfo(mpi);
	}

}