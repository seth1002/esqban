#ifndef L3TABLE_H
#define L3TABLE_H

extern int32 pretab[22];
extern int32 slen[2][16];

struct SBI {
   int32 l[23];
   int32 s[14];
};
extern struct SBI sfBandIndex[6];

extern real  t_43[8192];

extern real  two_to_negative_half_pow[64];

extern real  io[2][32];
extern real  TAN12[16];
extern int32 ss_div[576];
extern int32 ss_mod[576];


extern int32 reorder_table[6][576]; 

extern real  cs[8];
extern real  ca[8];

#endif // L3TABLE_H
