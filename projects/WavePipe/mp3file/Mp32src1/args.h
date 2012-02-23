/* args.h

	An abstract class to pass values to maplay and other worker
   threads.

	From the Win32 API, we can only pass one 32-bit value to a thread. This will
   be a pointer to a object of this class. The Args class contains the seeking
   control variables needed for both MPEG and MCI files.

   Added stop and done variables and functions for terminating the
   threads from outside.

   This class should be used to pass arguments to maplay(),
   and to initialize the Obuffer's. */

#ifndef ARGS_H
#define ARGS_H

#ifdef __WIN32__
#define STRICT
#include <wtypes.h>
#include <windows.h>
enum e_output { O_WAVEMAPPER, O_DIRECTSOUND, O_WAVEFILE };
#endif // __WIN32__

#include "all.h"
#include "ibitstr.h"
#include "header.h"

#ifdef SEEK_STOP
#include "mutx_imp.h"
#endif

class Args
{
	public:

	Args()
   {

#ifdef SEEK_STOP
		mutex 			  = NULL;
   	stop             = false;
      done             = false;
      desired_position = 0;
      position_change  = false;
#endif // SEEK_STOP

#ifdef WIN32GUI
   	hWnd             = NULL;
#endif // WIN32GUI
   }

	virtual ~Args() {}

#ifdef SEEK_STOP
	_Mutex mutex;
   volatile bool  stop;
   volatile bool  done;
  	volatile int32 desired_position;
	volatile bool  position_change;
#endif // SEEK_STOP

#ifdef WIN32GUI
	HWND hWnd;
#endif // WIN32GUI

};

// A class to contain arguments for maplay.
class MPEG_Args : public Args
{
public:

	MPEG_Args()
	{
		stream = NULL;
		MPEGheader = NULL;
		which_c = both;
	   use_own_scalefactor = false;
   	scalefactor = 32768.0;

#ifdef __WIN32__
  		phwo = NULL;
      output_mode = O_WAVEMAPPER;
#endif // __WIN32__

      stdout_mode   = false;
      verbose_mode  = false;

#if defined(SPARC) || defined(HPUX)
   	use_speaker   = false;
      use_headphone = false;
      use_line_out  = false;
#endif // SPARC || HPUX

#if defined(SPARC) && defined(ULAW)
		force_amd     = false;
#endif // SPARC && ULAW

#if defined(SPARC) || defined(HPUX) || defined(AIX)
      volume = -1.0f;
#endif // SPARC || HPUX || AIX

#ifdef HPUX
		wait_if_busy = false;
#endif
  	}

	Ibitstream *stream;
	Header *MPEGheader;
	enum e_channels which_c;

   bool use_own_scalefactor;
   real scalefactor;

#ifdef __WIN32__
	HWAVEOUT *phwo;
   enum e_output output_mode;
	char output_filename[256];
#endif // __WIN32__

   bool stdout_mode;
   bool verbose_mode;

#if defined(SPARC) || defined(HPUX)
   bool use_speaker;
   bool use_headphone;
   bool use_line_out;
#endif // SPARC || HPUX

#if defined(SPARC) && defined(ULAW)
	bool force_amd;
#endif // SPARC && ULAW

#if defined(SPARC) || defined(HPUX) || defined(AIX)
   float volume;
#endif // SPARC || HPUX || AIX

#ifdef HPUX
	bool wait_if_busy;
#endif // HPUX

	~MPEG_Args() { }
};

#ifdef __WIN32__
// A class to hold the arguments for MCI playing.
class MCI_Args : public Args {

public:
	MCI_Args()
	{
		playing = false;
      CDMode  = false;
      final_track = false;
      length = 0;

      lstrcpy(this_track_str, "1");
      lstrcpy(next_track_str, "2");
	}

   ~MCI_Args() { }

   bool CDMode;
   bool final_track;
   char this_track_str[8];
   char next_track_str[8];
   int  length;

	volatile bool playing;
};
#endif // __WIN32__

#endif // ARGS_H
