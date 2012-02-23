/* cep_obuffer.h

	Output buffer declarations for writing to a PCM wave file
   in Win32, written by Jeff Tsay (ctsay@pasteur.eecs.berkeley.edu). 

	CEP version just writes to memory, and lets us retrieve it at any time

*/

#ifndef CEP_OBUFFER_H
#define CEP_OBUFFER_H

#ifdef  __WIN32__
#define STRICT
#include <windows.h>

#include "all.h"
#include "header.h"
#include "args.h"
#include "obuffer.h"

class CEP_Obuffer : public Obuffer
{
private:

  uint32 bufferp[MAXCHANNELS];
  uint32 channels;
  uint32 data_size;

  BYTE *temp;

  
public:
	CEP_Obuffer(uint32 number_of_channels, MPEG_Args *maplay_args);
	~CEP_Obuffer();

  void	append(uint32 channel, real value);
  void  appendblock(uint32 channel, real * pvalues, int iCount);
  void	write_buffer(int32 fd);
  BYTE * get_buffer(int * piNumBytes);

};

Obuffer *create_CEP_Obuffer(MPEG_Args *maplay_args);

#endif // __WIN32__
#endif // CEP_OBUFFER_H
