/* crc32.c -- compute the CRC-32 of a data stream
 * Copyright (C) 1995-2002 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#include <io.h>
#include <fcntl.h>
#include <stdio.h>


#define ARRAYSIZE(arr)  (sizeof(arr) / sizeof(arr[0]))


#define NULL    0

typedef unsigned char           BYTE;
typedef unsigned int            UINT;
typedef unsigned long           ULONG;


ULONG crc32(ULONG crc, const BYTE *buf, UINT len);
void make_crc_table();
ULONG crc_table[256];






/*
 Generate a table for a byte-wise 32-bit CRC calculation on the polynomial:
 x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1.

 Polynomials over GF(2) are represented in binary, one bit per coefficient,
 with the lowest powers in the most significant bit.  Then adding polynomials
 is just exclusive-or, and multiplying a polynomial by x is a right shift by
 one.  If we call the above polynomial p, and represent a byte as the
 polynomial q, also with the lowest power in the most significant bit (so the
 byte 0xb1 is the polynomial x^7+x^3+x+1), then the CRC is (q*x^32) mod p,
 where a mod b means the remainder after dividing a by b.

 This calculation is done using the shift-register method of multiplying and
 taking the remainder.  The register is initialized to zero, and for each
 incoming bit, x^32 is added mod p to the register if the bit is a one (where
 x^32 mod p is p+x^32 = x^26+...+1), and the register is multiplied mod p by
 x (which is shifting right by one and adding x^32 mod p if the bit shifted
 out is a one).  We start with the highest power (least significant bit) of
 q and repeat for all eight bits of q.

 The table is simply the CRC of all possible eight bit values.  This is all
 the information needed to generate CRC's on data a byte at a time for all
 combinations of CRC register values and incoming bytes.
*/void make_crc_table()
{
       ULONG c;
       int n, k;
       ULONG poly;            /* polynomial exclusive-or pattern */
       /* terms of polynomial defining this crc (except x^32): */
       static const BYTE p[] = {0,1,2,4,5,7,8,10,11,12,16,22,23,26};

       /* make exclusive-or pattern from polynomial (0xedb88320L) */
       poly = 0L;
       for (n = 0; n < sizeof(p)/sizeof(BYTE); n++)
               poly |= 1L << (31 - p[n]);

       for (n = 0; n < 256; n++)
       {
               c = (ULONG)n;
               for (k = 0; k < 8; k++)
                       c = c & 1 ? poly ^ (c >> 1) : c >> 1;
               crc_table[n] = c;
       }
}


/* ========================================================================= */
#define DO1(buf) crc = crc_table[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);

/* ========================================================================= */
ULONG crc32(ULONG crc, const BYTE *buf, UINT len)
{
       if (buf == NULL) return 0L;
       crc = crc ^ 0xffffffffL;
       while (len >= 8)
       {
               DO8(buf);
               len -= 8;
       }
       if (len) do {
               DO1(buf);
       } while (--len);
       return crc ^ 0xffffffffL;
}

#define BUFF_SIZE 1024

int main(int argc, char **argv)
{
       if( -1 == setmode ( fileno ( stdin ), O_BINARY ) )
               return 0;

       make_crc_table();

       BYTE buff[BUFF_SIZE] = {0};
       UINT readBytes = 0;

       FILE *fp = stdin;

       if(argc == 2)
               fp = fopen(argv[1], "rb");

       if(NULL == fp)
               return false;

       ULONG crc = 0;

       do {
               readBytes = fread( buff, sizeof(BYTE), BUFF_SIZE, fp );
               crc = crc32(crc, buff, readBytes);
       } while(readBytes > 0);

       fclose(fp);

       printf("CRC32 checksum: %d\n", crc);

       printf("Press Enter to exit...");
       getchar();

       return 0;
}
