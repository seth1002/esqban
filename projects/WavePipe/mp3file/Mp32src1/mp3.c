/*  @(#) mp3.c, last edit: 4/9/1998
 *  @(#) Copyright (C) 1998 Syntrillium Software (www.syntrillium.com)
 
	Cool Edit (and Cool Edit Pro) MP3 file reader based on MAPLAY by Jeff Tsay

	A bunch of convoluted code was added to this file filter in order to allow
	for the choice of volume level during decode, and format to decode to. 
	That's right, you can decode to 16-bit by truncation, or by dithering, or
	keep an entire 32-bit float for each sample for use in Cool Edit Pro.

    Versions Cool Edit that do not support 32-bit will still say "32-bit" when
	the file is loaded, but rest assured, it's only 16-bit.

	Most of the files for MAPLAY were left untouched, and many were not needed
	for this port, so only the files needed for the port are included here.

    Search for MAPLAY 1.2 or later for original source (and if the source gets
	updated, it should be trivial to add it to this build).  Very few of the 
	other files were modified, and when they were, they were bracketed by 
	"#ifdef COOLPRO" for easy location and such.

	The Obuffer class was modified to take in floats instead of ints for
	reading in with greater accuracy.  Since MPEG (which is similar to JPEG
	in this respect) doesn't try to encode the exact sample values, but 
	instead is psychoacoustic in nature, higher quality can be gained by
	encoding from a 32-bit source (I haven't found an encoder that can take
	floating point samples yet) and then decoding back to 32-bit.  Nothing
	in MPEG says it should be limited to 16-bit I believe (but don't quote
	me on this... I do not know much about the internals of the MPEG format).

	Some links to the binaries of ports of maplay are on Jeff's homepage at:
	http://www-inst.eecs.berkeley.edu/~ctsay/mp2win32.html


  Believe it or not, the speed is much faster than original ISO MPEG code,
  but still slower than Fraunhoffer.  Their Windows ACM codec is about
  twice as fast for reading.  But the good news is it that it's still fast
  enough for real time previewing (auto-play) on pretty much all Pentiums.

  If you purchase an ACM codec for writing Layer 3 MPEG, use the updated
  ACM Waveform format type found at http://www.syntrillium.com which allows
  for saving any ACM format as raw data thus allowing the saving directly
  to .mp3 files if you have the codec which can write MPEG .wav files.


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


*/


#include <windows.h>
#include <math.h>
#include "filters.h"
#include "ceglue.h"
#include "resource.h"


// Hey!  Someone want a _real_ challenge?  Implement Layer 3 encoding as well!
    
typedef struct output_tag  // any special vars associated with output file
{	short nFile;         
	long lSize;
} MYOUTPUT;

typedef struct input_tag // any special vars associated with input file
{	short nFile;
	long lSize;
	long lSamprate;
	DWORD dwFormat;
	BOOL bOpenedMPEG;
	MPEG_INFO mpi;
	double fAttenuation;
	float fScaleBy;
	WORD wBitsPerSample;
	BOOL bDither;
	HANDLE h32bit;
	short * pi32bit;
} MYINPUT;

HANDLE myInstance=NULL;

BOOL WINAPI DllMain (HANDLE hModule, DWORD fdwReason, LPVOID lpReserved)
{
   switch (fdwReason)
   {
      case DLL_PROCESS_ATTACH:
        
		 myInstance=hModule;
    
         break;
 
      case DLL_THREAD_ATTACH:
         break;
 
      case DLL_THREAD_DETACH:
         break;
 
      case DLL_PROCESS_DETACH:
         myInstance=NULL;
         break;
   }
 
   return TRUE;   
}

// Fill COOLQUERY structure with information regarding this file filter

__declspec(dllexport) short FAR PASCAL QueryCoolFilter(COOLQUERY far * cq)
{   lstrcpy(cq->szName,"MPEG 3");         
	lstrcpy(cq->szCopyright,"MPEG (Layer 1,2,3) Decoder (GNU)");
	lstrcpy(cq->szExt,"MP3");
	lstrcpy(cq->szExt2,"MP2");
	lstrcpy(cq->szExt3,"MP1");
	cq->lChunkSize=16384; 
	cq->dwFlags=QF_RATEADJUSTABLE|QF_CANLOAD|QF_CANDO32BITFLOATS|QF_HASOPTIONSBOX;
	cq->Stereo8=0; 
	cq->Stereo16=R_32075|R_44100|R_48000;
	cq->Stereo24=0;
	cq->Stereo32=R_32075|R_44100|R_48000;
	cq->Mono8=0; 
	cq->Mono16=R_32075|R_44100|R_48000;
	cq->Mono24=0;
	cq->Mono32=R_32075|R_44100|R_48000;
	cq->Quad32=0;
	cq->Quad16=0;
	cq->Quad8=0;
	return C_VALIDLIBRARY;
}

// Just our little way of not including stdio.h to do float to string cvting
char * ftos(char *tmp, double d)
{	double dd;
	double mplier;
	short maxdecimal=9;
	char * otemp;
	
	otemp=tmp;
	
	if (d==0)
	{ *tmp='0';
	  tmp++;
	  *tmp=0x00;
	  return otemp;
	} 
	
	if (d<0)
	{	*tmp='-';
		tmp++;
		d=-d;
	}
	d=d+(1.0/65535.0/32768.0);
	dd=d;         
	mplier=1.0;
	while (dd>=1.0)
	{	dd/=10.0;
		mplier*=10.0;	
	}   
	mplier/=10.0;             
	while (mplier>=1.0)
	{	*tmp='0'+(short)(d/mplier);
		tmp++;
		d=d-mplier*(short)(d/mplier);
		mplier/=10.0;	
	}
    if (d!=0.0)
    {	short digit;
    	*tmp='.';
    	tmp++;
    	while ((d!=0.0) && (maxdecimal!=0))
    	{	d*=10.0;
    		digit=(short)d;
    		d-=(double)digit;
    		*tmp='0'+digit;
    		tmp++;
    		maxdecimal--;    	    	
    	}             
    	while (*(tmp-1)=='0')
    		tmp--;
    	if (*(tmp-1)=='.')
    		tmp--;
    }
                  
    *tmp=0x00;   
    
    return otemp;
}                                  

__declspec(dllexport) long FAR PASCAL FilterGetFileSize(HANDLE hInput)
{	
	long lSize=0L;
	if (hInput)
	{       MYINPUT *mi;
		mi=(MYINPUT *)GlobalLock(hInput);
		lSize=mi->lSize;
		GlobalUnlock(hInput);
	}
    return lSize;
}

__declspec(dllexport) BOOL FAR PASCAL FilterGetActualFormat(HANDLE hInput,DWORD dwOptions,long * plSamprate, WORD * pwChannels, WORD * pwBitsPerSample)
{   
	if (hInput)
	{	MYINPUT *mi;
		mi=(MYINPUT *)GlobalLock(hInput);
		
		*pwBitsPerSample=HIWORD(dwOptions)&0x00FF;

		GlobalUnlock(hInput);
	}
    return TRUE;
}


__declspec(dllexport) BOOL FAR PASCAL FilterUnderstandsFormat(LPSTR filename)
{	
	if ((lstrlen(filename)>4) && 
		((strcmpi(filename+lstrlen(filename)-4,".mp3")==0) ||
		 (strcmpi(filename+lstrlen(filename)-4,".mp2")==0) ||
		 (strcmpi(filename+lstrlen(filename)-4,".mp1")==0)		 
		 ))
	{	
  		return TRUE;
  	}
  	return FALSE;
}


__declspec(dllexport) BOOL FAR PASCAL DIALOGMsgProc(HWND hWndDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{	switch(Message)
	{	case WM_INITDIALOG:
		{	 WORD wBitsPerSample;
			 double fAttenuation;
			 BOOL bDither;
			 char tmp[30];

         	 wBitsPerSample=HIWORD(lParam)&0x00FF;
			 bDither=(lParam&0x01000000)?TRUE:FALSE;
			 fAttenuation=(double)((DWORD)lParam&0xffff)/100.0-200.0;
             if (fAttenuation>200.0)
				 fAttenuation=0.0;

			 //wsprintf(tmp," Read MPEG As %d-bit ",wBitsPerSample);
			 //SetDlgItemText(hWndDlg,IDC_TEXT1,tmp);
             
			 if (wBitsPerSample==16)
	         {	CheckDlgButton(hWndDlg,IDC_RADIO1,TRUE);
	         }
	         else if (wBitsPerSample==32)
	         {  CheckDlgButton(hWndDlg,IDC_RADIO2,TRUE);
	         }
	         else
	         {	CheckDlgButton(hWndDlg,IDC_RADIO1,TRUE);
	         }
			 
			 CheckDlgButton(hWndDlg,IDC_CHECK1,bDither);
	         ftos(tmp,fAttenuation);
			 SetDlgItemText(hWndDlg,IDC_EDIT1,tmp);
			 EnableWindow(GetDlgItem(hWndDlg,IDC_CHECK1),IsDlgButtonChecked(hWndDlg,IDC_RADIO1)?TRUE:FALSE);
         }         
         break; 

		case WM_CLOSE:         
			 PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
			 break; 

		case WM_COMMAND:
		{	WORD wNotifyCode=HIWORD(wParam);
			wParam=(WPARAM)LOWORD(wParam);
			switch(wParam)
			{	
				case IDC_RADIO1:
				case IDC_RADIO2:
					EnableWindow(GetDlgItem(hWndDlg,IDC_CHECK1),IsDlgButtonChecked(hWndDlg,IDC_RADIO1)?TRUE:FALSE);
					break;
				
				case IDOK:
				{	long nDialogReturn=0;
            		char tmp[30];
					
					if (IsDlgButtonChecked(hWndDlg,IDC_RADIO2))
					{	nDialogReturn=32L<<16;
					}
					else
					{	nDialogReturn=16L<<16;
					}

					GetDlgItemText(hWndDlg,IDC_EDIT1,tmp,29);
					nDialogReturn|=(WORD)max(0,min(40000,(atof(tmp)+200.0)*100.0));
					if (IsDlgButtonChecked(hWndDlg,IDC_CHECK1))
						nDialogReturn|=0x01000000;

					EndDialog(hWndDlg, nDialogReturn);
				}
				break;

				case IDCANCEL:
					 EndDialog(hWndDlg, FALSE);
					 break;
			}
			break;
		}
		default:
			return FALSE;
	}
	return TRUE;
} 

__declspec(dllexport) DWORD FAR PASCAL FilterGetOptions(HWND hWnd, HINSTANCE hInst, long lSamprate, WORD wChannels, WORD wBitsPerSample, DWORD dwOptions) // return 0 if no options box
{	
	long nDialogReturn=0L;
	FARPROC lpfnDIALOGMsgProc;
	lpfnDIALOGMsgProc = GetProcAddress(hInst,(LPCSTR)MAKELONG(20,0));	
			
	if (dwOptions==0)
	{	
		nDialogReturn=GetPrivateProfileInt("MPEG Filter","InputFormat",0,"coolrw.ini");
		if (nDialogReturn==0)
			nDialogReturn=(WORD)max(0,min(40000,(100.0+200.0)*100.0));
	}
	else
		nDialogReturn=dwOptions;
	
	// These were to force the dialog to have _only_ 16 or 32... now we let user choose
	//nDialogReturn=(nDialogReturn&0xFF00FFFF)|(((DWORD)wBitsPerSample)<<8);	
	//nDialogReturn|=MAKELONG(nDialogReturn&0x0FF,wBitsPerSample);
	
	nDialogReturn = (long)DialogBoxParam((HINSTANCE)hInst,(LPCSTR)MAKEINTRESOURCE(IDD_COMPRESSION), (HWND)hWnd, (DLGPROC)lpfnDIALOGMsgProc,nDialogReturn);
	
	if (nDialogReturn)
	{	char m[30];
		wsprintf(m,"%d",nDialogReturn);
		WritePrivateProfileString("MPEG Filter","InputFormat",m,"coolrw.ini");
	}

	return nDialogReturn;
}          

__declspec(dllexport) DWORD FAR PASCAL FilterSetOptions2(HANDLE hInput, DWORD dwOptions, long * plSamprate, WORD * pwChannels, WORD * pwBitsPerSample)
{	
	DWORD dwOldOptions;
	MYINPUT *mi;
	mi=(MYINPUT *)GlobalLock(hInput);
		
	dwOldOptions=mi->dwFormat;

	if (!dwOptions)
	{	dwOptions=(DWORD)GetPrivateProfileInt("MPEG Filter","InputFormat",0,"coolrw.ini");
		if (!dwOptions)
			dwOptions=(WORD)max(0,min(40000,(200.0)*100.0))|(16L<<16);
	}
					
	mi->wBitsPerSample=HIWORD(dwOptions)&0x00FF;
	mi->fAttenuation=(double)((DWORD)dwOptions&0xffff)/100.0-200.0;
	mi->bDither=(dwOptions&0x01000000)?TRUE:FALSE;
			
    mi->dwFormat=dwOptions;

	if ((mi->fAttenuation<-199.0) || (mi->fAttenuation>200.0))
		mi->fAttenuation=0;

	if (mi->wBitsPerSample==16)
	{	*pwBitsPerSample=16;
		if (!mi->h32bit)
		{	mi->h32bit=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,32768);
			mi->pi32bit=(short *)GlobalLock(mi->h32bit);
		}
	}
	else
	{	*pwBitsPerSample=32;		
	}
	mi->lSize=(int)(mi->mpi.fLengthInMilliseconds*(*plSamprate)/1000.0)*(*pwChannels)*(*pwBitsPerSample/8);
	mi->fScaleBy=(float)(32768.0*(pow(10.0,mi->fAttenuation/20.0)));
	
    GlobalUnlock(hInput);
    
    return dwOldOptions;
        
}

// Dummy, but must be here... FilterSetOptions2 is actually called instead if it exists
__declspec(dllexport) DWORD FAR PASCAL FilterSetOptions(HANDLE hInput, DWORD dwOptions, long lSamprate, WORD wChannels, WORD wBitsPerSample)
{	
	DWORD dwOldOptions;
	MYINPUT *mi;
	mi=(MYINPUT *)GlobalLock(hInput);
	
	dwOldOptions=mi->dwFormat;

    mi->dwFormat=dwOptions;
	mi->wBitsPerSample=HIWORD(dwOptions)&0x00FF;
	mi->fAttenuation=(double)((DWORD)dwOptions&0xffff)/100.0-200.0;
	mi->bDither=(dwOptions&0x01000000)?TRUE:FALSE;
	
	if ((mi->fAttenuation<-199.0) || (mi->fAttenuation>200.0))
		mi->fAttenuation=0;

	if (mi->wBitsPerSample==16)
	{	if (!mi->h32bit)
		{	mi->h32bit=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,32768);
			mi->pi32bit=(short *)GlobalLock(mi->h32bit);
		}
	}

	mi->lSize=(int)(mi->mpi.fLengthInMilliseconds*(lSamprate)/1000.0)*(wChannels)*(wBitsPerSample/8);
	mi->fScaleBy=(float)(32768.0*(pow(10.0,mi->fAttenuation/20.0)));

    GlobalUnlock(hInput);
    
    return dwOldOptions;
}


__declspec(dllexport) DWORD FAR PASCAL FilterOptions(HANDLE hInput)
{   MYINPUT *mi;
	DWORD dwFormat;
	if (!hInput) return 0;
	mi=(MYINPUT *)GlobalLock(hInput);
		
	dwFormat=(DWORD)mi->dwFormat;

    GlobalUnlock(hInput);
    return dwFormat;
}


__declspec(dllexport) DWORD FAR PASCAL FilterOptionsString(HANDLE hInput, LPSTR szString)
{   MYINPUT *mi;
	DWORD dwFormat;
	
	if (!hInput) return 0;
	mi=(MYINPUT *)GlobalLock(hInput);
    dwFormat=(DWORD)mi->dwFormat;

	wsprintf(szString,"%s Layer %d\r\n(%s)",mi->mpi.szVersion,mi->mpi.iLayer,mi->mpi.szBitrate);
	
    GlobalUnlock(hInput);
    return dwFormat;
}


__declspec(dllexport) DWORD FAR PASCAL FilterCanWriteSpecial(HANDLE hOutput, DWORD idType)
{
 return 0L;
}

__declspec(dllexport) DWORD FAR PASCAL FilterCanReadSpecial(HANDLE hInput, DWORD idType)
{
 return 0L;
}
					 
__declspec(dllexport) DWORD FAR PASCAL FilterWriteSpecial(HANDLE hOutput, DWORD idType, unsigned char far *buf, DWORD dwSize)
{
 return 0L;
}

__declspec(dllexport) DWORD FAR PASCAL FilterReadSpecial(HANDLE hOutput, DWORD idType, unsigned char far *buf, DWORD dwSize)
{
 return 0L;
}



__declspec(dllexport) HANDLE FAR PASCAL OpenFilterOutput(LPSTR lpstrFilename,long lSamprate,WORD wBitsPerSample,WORD wChannels,long lSize,long far *lpChunkSize,DWORD dwOptions)
{   HANDLE hOutput;
	short nFile;
	nFile=_lcreat(lpstrFilename,0);
    if (nFile==-1) return NULL;
    hOutput=GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE,sizeof(MYOUTPUT));
    if (hOutput)
    {   MYOUTPUT *mo;
		mo=(MYOUTPUT *)GlobalLock(hOutput);
		mo->nFile=nFile;
		mo->lSize=lSize;
		GlobalUnlock(hOutput);
    }
    *lpChunkSize=16384;
    return hOutput;
}

__declspec(dllexport) void FAR PASCAL CloseFilterOutput(HANDLE hOutput)
{   if (hOutput)
	{       MYOUTPUT *mo;
		mo=(MYOUTPUT *)GlobalLock(hOutput);
		if (mo->nFile!=-1)
		{       _lclose(mo->nFile);
			mo->nFile=-1;
		}

		GlobalUnlock(hOutput);
		GlobalFree(hOutput);
	}
}

__declspec(dllexport) DWORD FAR PASCAL WriteFilterOutput(HANDLE hOutput, unsigned char far *buf, long lBytes)
{       DWORD written=0L;
	if (hOutput)
	{       MYOUTPUT *mo;
		mo=(MYOUTPUT *)GlobalLock(hOutput);
		written=(DWORD)_lwrite(mo->nFile,buf,(UINT)lBytes);
		GlobalUnlock(hOutput);
	}
	return written; 
}

DWORD valJ=524309;			
DWORD valI=12345; // reset to time at startup

double fRandom(void) // 0 to 1
{	DWORDLONG dwl;	
	dwl=UInt32x32To64(valJ,valI);
	dwl++;
	valI=(DWORD)(dwl&0x1FFFFF);	
	return (double)valI/2097152.0;
}

// This is the 32-bit to 16-bit converter from Cool Edit, but with only the code
// for the straight triangular dithering.  For higher quality, read in the file
// as 32-bit, then use Cool Edit Pro's dithering options for going to 16-bit.

void HfCvt32To16InMem(short *piDest, float *pfSource, long dwSourceSamples, BOOL bStereo, double fDitherAmount)
{	long t;
	double fFilteredError=0;
	double fSourceBeforeNoise;
	double qq=1.0;
	double fMultiplier=0.5;
	double fNoise;
	double fSource;
	double fSqrtDitherAmount;
	long quant;
	static double fError=0;
	static double fError1=0;
	static double fError2=0;
	static double fError3=0;
	static double fError4=0;
	static double fError5=0;
	static double fErrorR=0;
	static double fErrorR1=0;
	static double fErrorR2=0;
	static double fErrorR3=0;
	static double fErrorR4=0;
	static double fErrorR5=0;
		  		 
	{   
		if (fDitherAmount>0)
			fSqrtDitherAmount=sqrt(fDitherAmount);
		else
			fSqrtDitherAmount=0;
		
		if (bStereo)
	 	{	
			for (t=0;t<dwSourceSamples;t+=2)			
			{	
				fFilteredError=0;
				
				fError4=fError3;
				fError3=fError2;
				fError2=fError1;
				fError1=fFilteredError;
				
				fSource=*pfSource;
				pfSource++;
				
				// Don't Shift out 8 bits
				fSource=fSource-fFilteredError; //*fDitherMultiplier;					
				
				fNoise=fDitherAmount*(fRandom()+fRandom()-1.0);				
				
				fSourceBeforeNoise=fSource;

				// Add Noise (v)
				fSource+=fNoise; //*fDitherMultiplier; //*2.0;
				
				// Quantization
				quant=(long)(fSource+500000+0.5)-500000; // floor function, values -1000 to +1000 OK

				// Error for input to H(z)
				fError=(double)quant-fSourceBeforeNoise;

				if (quant>=-32768)
				{	if (quant<=32767)
						*piDest=(short)quant;
					else
						*piDest=32767;
				}
				else
					*piDest=-32768;
				piDest++;

				fFilteredError=0;
				
				fErrorR4=fErrorR3;
				fErrorR3=fErrorR2;
				fErrorR2=fErrorR1;
				fErrorR1=fFilteredError;
				
				fSource=*pfSource;
				pfSource++;

				// Don't Shift out 8 bits
				fSource=fSource-fFilteredError; //*fDitherMultiplier;					
				
				fNoise=fDitherAmount*(fRandom()+fRandom()-1.0);
				
				fSourceBeforeNoise=fSource;

				// Add Noise (v)
				fSource+=fNoise; //*fDitherMultiplier; //*2.0;
				
				// Quantization
				quant=(long)(fSource+500000+0.5)-500000; // floor function, values -1000 to +1000 OK

				// Error for input to H(z)
				fErrorR=(double)quant-fSourceBeforeNoise;

				if (quant>=-32768)
				{	if (quant<=32767)
						*piDest=(short)quant;
					else
						*piDest=32767;
				}
				else
					*piDest=-32768;
				piDest++;
			}		
	 	}       
	 	else
	 	{   
			for (t=0;t<dwSourceSamples;t++)
			{	fFilteredError=0;
							
				fError5=fError4;
				fError4=fError3;
				fError3=fError2;
				fError2=fError1;
				fError1=fFilteredError;

				fSource=*pfSource;
				pfSource++;
				
				// Don't Shift out 8 bits
				fSource=fSource-fFilteredError; //*fDitherMultiplier;					
				// Triangular Dither Noise
				
				fNoise=fDitherAmount*(fRandom()+fRandom()-1.0);					
				
				fSourceBeforeNoise=fSource;

				// Add Noise (v)
				fSource+=fNoise; //*fDitherMultiplier; //*2.0;
				
				// Quantization
				quant=(long)(fSource+500000+0.5)-500000; // floor function, values -1000 to +1000 OK

				// Error for input to H(z)
				fError=(double)quant-fSourceBeforeNoise;

				if (quant>=-32768)
				{	if (quant<=32767)
						*piDest=(short)quant;
					else
						*piDest=32767;
				}
				else
					*piDest=-32768;
				piDest++;

			}	
		} 
	}
}


__declspec(dllexport) void FAR PASCAL GetSuggestedSampleType(long far *lSugSamprate,WORD far *wSugBitsPerSample, WORD far *wSugChannels)
{   // return 0 for don't care
 	*lSugSamprate=0;
	{	int n=GetPrivateProfileInt("MPEG Filter","InputFormat",0,"coolrw.ini");
		*wSugBitsPerSample=HIWORD(n)&0x00FF;
		if ((*wSugBitsPerSample!=16) && (*wSugBitsPerSample!=32))
			*wSugBitsPerSample=16;
	}
 	*wSugChannels=0;
}


// return handle that will be passed in to close, and write routines
__declspec(dllexport) HANDLE FAR PASCAL OpenFilterInput( LPSTR lpstrFilename,
											long *lSamprate,
											WORD *wBitsPerSample,
											WORD *wChannels,
											HWND hWnd,
											long *lChunkSize)
{	HANDLE hInput;
	short source; 
	long fullsize;
	DWORD dwOptions;
	
	dwOptions=0;
	valI=GetTickCount()%50000;

	source=_lopen(lpstrFilename,OF_READ);
    if (source==-1) return NULL;     
    
    fullsize=_llseek(source,0L,2);
	_lclose(source);
	
    hInput=GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE|GMEM_ZEROINIT,sizeof(MYINPUT));
    if (hInput)
    {   MYINPUT *mi;
		
		mi=(MYINPUT *)GlobalLock(hInput);

		mi->wBitsPerSample=32; 
		mi->fAttenuation=0; 
		mi->dwFormat=0;			// Force asking of format, and default to 32-bit No attenuation
		mi->fScaleBy=32768.0f;
		mi->bDither=FALSE;
	
		// return 0 for cool to inquire user for sample rates (we aren't doing that though)
		*lSamprate=44100;
		*wBitsPerSample=32;
		*wChannels=2;
		*lChunkSize=16384;

		mi->bOpenedMPEG=MPEG_Open(lpstrFilename);
		if (!mi->bOpenedMPEG)
		{	GlobalUnlock(hInput);
     		GlobalFree(hInput);
     		return NULL;
		}
		
		MPEG_GetInfo(&mi->mpi);

		*lSamprate=mi->mpi.iSamplerate;
		*wChannels=mi->mpi.iChannels;

		{	int n=GetPrivateProfileInt("MPEG Filter","InputFormat",0,"coolrw.ini");
			*wBitsPerSample=HIWORD(n)&0x00FF;
			if ((*wBitsPerSample!=16) && (*wBitsPerSample!=32))
				*wBitsPerSample=16;
		}

		mi->wBitsPerSample=*wBitsPerSample;
		
		mi->lSize=(int)(mi->mpi.fLengthInMilliseconds*(*lSamprate)/1000.0)*(*wChannels)*(*wBitsPerSample/8);

		GlobalUnlock(hInput);
    }
    
    return hInput;
}

__declspec(dllexport) void FAR PASCAL CloseFilterInput(HANDLE hInput)
{   if (hInput)  
	{       MYINPUT *mi;
		mi=(MYINPUT *)GlobalLock(hInput);
		
		if (mi->bOpenedMPEG)
			MPEG_Close();
    	
		if (mi->h32bit)
		{	GlobalUnlock(mi->h32bit);
			GlobalFree(mi->h32bit);
		}

		GlobalUnlock(hInput);
		GlobalFree(hInput);
	}
}



__declspec(dllexport) DWORD FAR PASCAL ReadFilterInput(HANDLE hInput, unsigned char far *bufout, long lBytes)
{       DWORD read=0L;
	if (hInput)
	{   MYINPUT *mi;
		float fScaleBy;

		mi=(MYINPUT *)GlobalLock(hInput);
		fScaleBy=mi->fScaleBy;

		if (mi->wBitsPerSample==32)
		{	// just scale
			read=(DWORD)MPEG_Read(bufout);
		
			{	int iNum=read/4;
				float * pBuf=(float *)bufout;
				while (iNum--)
				{	*pBuf++*=fScaleBy;					
				}
			}
			
		}
		else if (mi->wBitsPerSample==16)
		{	// Convert to 16-bit with scaling and dithering
			int iNum;
			float * pfBuf;
			short * piBuf;

			pfBuf=(float *)mi->pi32bit;
			piBuf=(short *)bufout;

			read=(DWORD)MPEG_Read((BYTE *)mi->pi32bit);						
			iNum=read/4;
			// Dither it to 16-bit

			if (mi->bDither)
			{	// Scale it first
				{	int iNum=read/4;
					float * pBuf=pfBuf;
					while (iNum--)
					{	*pBuf++*=fScaleBy;					
					}
				}

				// Dither and Clip
				HfCvt32To16InMem(piBuf, pfBuf, iNum,
					(mi->mpi.iChannels==2)?TRUE:FALSE, 1.0);
			}
			else 
			{	// Scale, Truncate, and Clip
				float f;
				while (iNum--)
				{	f=*pfBuf++*fScaleBy;
					if (f>=(float)(-32767.0))
					{	if (f<=(float)32767.0)
							*piBuf++=(short)f;
						else
							*piBuf++=32767;
					}
					else
						*piBuf++=-32767;
				}
			}
			read/=2;
		}
		GlobalUnlock(hInput);
	}
	return read;
}
