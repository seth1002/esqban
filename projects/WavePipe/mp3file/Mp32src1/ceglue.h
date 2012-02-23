BOOL MPEG_Open(LPSTR szName);
int MPEG_Read(BYTE * pBuffer);
void MPEG_Close();

typedef struct mpeg_info_t
{
	int iSamplerate;
	int iChannels;
	int iStereoMode;
	int iLayer;
	int iVersion;
	int iBitrateIndex;
	int iFrameSize;
	double fLengthInMilliseconds;
	BOOL bIsChecksummed;
	BOOL bIsCopyrighted;
	BOOL bIsOriginal;
	char szBitrate[48];
	char szVersion[16];

} MPEG_INFO;

BOOL MPEG_GetInfo(MPEG_INFO * mpi);
