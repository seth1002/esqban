# include <stdio.h>
# include <stdlib.h>
# include <malloc.h>
#define MAXC 1001
#define MAXPP 500

struct vbr {
  int brc;
  char c[MAXC];
};

char pe[MAXPP+1][MAXPP+1];
char he[MAXPP+1][MAXPP+1];
struct vbr xg[MAXPP+1];
struct vbr pt[MAXPP];

int_dod (struct vbr *d, struct vbr *s) {
  int i;

  d->brc = s->brc;
  for (i = 0; i < MAXC; i++) d->c[i] = 0;
  for (i = 0; i < s->brc; i++) d->c[i] = s->c[i];
}

/*** ucitavanje broja ***/
int intsc (struct vbr *x) {
  int i, j;
  char ch[100];

  printf ("? ");
  scanf ("%s", ch);
  i = strlen(ch)-1;
  j = 0;
  while ((ch[i] < '0') || (ch[i] > '9')) i--;
  while ((i >= 0) && (ch[i] >= '0') && (ch[i] <= '9'))
    x->c[j++] = ch[i--]-'0';
  x->brc = j;
}

intpr (struct vbr *x) {
  int i;

  for (i = x->brc-1; i >= 0; i--)
    printf("%d", x->c[i]);
}

intprln (struct vbr *x) {
  int i;

  for (i = x->brc-1; i >= 0; i--)
    printf("%d", x->c[i]);
  printf("\n");
}


/*** generisanje slucajne cifre ***/
intrand1 (int n) {
  long m;

  m = (long) n * rand();
  m = m / RAND_MAX;
  return ((int)m);
}

/*** generisanje slucajnog broja sa n cifara ***/
int intrand (int n, struct vbr *x) {
  int i;

  while ((i = intrand1(9)) == 0);
  x->c[n-1] = i;
  for (i = n-2; i >= 0; i--) x->c[i] = intrand1(9);
  x->brc = n;
}


/*** uvecavanje broja za 1 ***/
int intinc (struct vbr *x) {
  int i, j, k;

  for (i = 0, k = 1; k && (i < x->brc); i++) {
    k = x->c[i]+k;
    x->c[i] = k % 10;
    k = k /10;
  }

  if (k) {
    x->c[i] = k;
    x->brc++;
  }
}

/*** umanjivanje broja za 1 ***/
int intdec(struct vbr *x) {
  int i, j, k;

  for (i = 0, k = 1; k && (i < x->brc); i++) {
    k = x->c[i] - k;
    if (k < 0)
      k = 1;
    else
      k = 0;
  }

  if (k) return (-1);

  for (i = 0, k = 1; k && (i < x->brc); i++) {
    k = x->c[i] - k;
    if (k < 0) {
      x->c[i] = k+10;
      k = 1;
    } else {
      x->c[i] = k;
      k = 0;
    }
  }
}

    
int sab (struct vbr *s1, struct vbr *s2, struct vbr *z) {
/***
s1 i s2 su nizovi pomo{\'c}u kojih su reprezentovani sabirci
z je niz za reprezentaciju zbira
***/
  int i, j, k, l;

  k = 0;
  l = min(s1->brc, s2->brc);
  for (i = 0, k = 0; i < l; i++) {
    k = s1->c[i]+s2->c[i]+k;
    z->c[i] = k % 10;
    k = k / 10;
  }

  if (s1->brc > s2->brc)
    for (; i < s1->brc; i++) {
      k = s1->c[i]+k;
      z->c[i] = k % 10;
      k = k / 10;
    }
   else
    for (; i < s2->brc; i++) {
      k = s2->c[i]+k;
      z->c[i] = k % 10;
      k = k / 10;
    }

  if (k) 
    z->c[i++] = k;
  z->brc = i;
}

int oduz (struct vbr *u1, struct vbr *u2, struct vbr *r) {
/***
u1 i u2 su nizovi pomo{\'c}u kojih su reprezentovani redom
umanjenik i umanjilac,
r je niz za reprezentaciju razlike
rezultat funkcije je:
- nula (0), ako je oduzimanje uspe{\vs}no izvedeno,
- minus jedan (-1), ako oduzimanje nije uspe{\vs}no izvedeno.
***/
  int i, j, k, l;

  if (u1->brc < u2->brc) return(-1);
  k = 0;
  l = u2->brc;
  for (i = 0, k = 0; i < l; i++) {
    k = u1->c[i]-u2->c[i]-k;
    if (k < 0) {
      r->c[i] = k + 10;
      k = 1;
    } else {
      r->c[i] = k;
      k = 0;
    }
  }

  for (; i < u1->brc; i++) {
    k = u1->c[i]-k;
    if (k < 0) {
      r->c[i] = k+10;
      k = 1;
    } else {
      r->c[i] = k;
      k = 0;
    }
  }

  if (k) return(-1);
  while ((i>0) && !(r->c[--i]));
  r->brc = i+1;
  return(0);
}


int mnoz (struct vbr *m1, struct vbr *m2, struct vbr *r) {
/*** 
  m1, m2 su redom mno{\vz}enik i mno{\vz}ilac
  r je proizvod brojeva m1 i m2
  rezultat funkcije je
  0 - proizvod je u dozvoljenom opsegu
  -1 - proizvod je van dozvoljenog opsega (ima vi{\vs}e od MAXC
cifara)
***/
  int i, j, k;
  short m[2*(MAXC+1)];

  for (i = 0, k = 0; i < m1->brc+m2->brc-1; i++) {
    for (j = max(0, i-m2->brc+1); j <= min(i, m1->brc-1); j++)
      k += m1->c[j]*m2->c[i-j];
    m[i] = k % 10;
    k = k / 10;
  }

  while (k) {
    m[i++] = k % 10;
    k = k / 10;
  }
  if (i > MAXC)
    return (-1);
  else {
    r->brc = i;
    for (i--; i >= 0; i--) r->c[i] = m[i];
    return (0);
  }
}


int mnoz1 (struct vbr *m1, struct vbr *m2, struct vbr *r) {
  int i, j, k;
  short m[2*(MAXC+1)];

  for (i = 0; i <= m1->brc+m2->brc-1; m[i++]=0);
  for (i = 0; i < m1->brc; i++)
    for (j = 0; j < m2->brc; j++)
      m[i+j] += m1->c[i]*m2->c[j];
  for (i = 0, k = 0; i <= m1->brc+m2->brc-2; i++) {
    k += m[i];
    m[i] = k % 10;
    k = k / 10;
  }

  while (k) {
    m[i++] = k % 10;
    k = k / 10;
  }
  if (i > MAXC)
    return (-1);
  else {
    r->brc = i;
    for (i--; i >= 0; i--) r->c[i] = m[i];
    return (0);
  }
}


/*** mnozenje sa 2 na n ***/
int shiftl (int n, struct vbr *a) {
  int i, j, k;

  for (i = 0; i < n; i++) {
    for (j = 0, k = 0; j < a->brc; j++) {
      k = 2*a->c[j]+k;
      a->c[j] = k % 10;
      k = k / 10;
    }
    if (k) {
      a->c[j] = k;
      a->brc++;
    }
  }
}

/*** deljenje sa 2 na n ***/
int shiftr (int n, struct vbr *a) {
  int i, j, k;

  for (i = 0; i < n; i++) {
    j = a->brc-1;
    for (k = 0; j >= 0; j--) {
      k = k*10+a->c[j];
      a->c[j] = k / 2;
      k = k % 2;
    }
    if ((a->brc > 1) && (a->c[a->brc-1] == 0)) (a->brc)--;
    if ((a->brc == 1) && (a->c[0] == 0)) break;
  }
}


/*** rusko mnozenje ***/
int mnozr (struct vbr *m1, struct vbr *m2, struct vbr *r) {
  int i, j, k;
  struct vbr m3, m4;

  j = m1->brc+m2->brc;
  m3.brc = m1->brc; m4.brc = m2->brc;
  for (j = 0; j < m1->brc; j++) m3.c[j] = m1->c[j];
  for (j = 0; j < m2->brc; j++) m4.c[j] = m2->c[j];
  r->brc = 1; r->c[0] = 0;
  while ((m3.brc > 1) || (m3.c[0] != 0)) {
    if (m3.c[0] % 2) sab (&m4, r, r);
    shiftr (1, &m3);
    shiftl (1, &m4);
  }
}



/*** poredjenje dva broja 1 - prvi veci, -1 - drugi veci, 0 -
jednaki ***/
int intcmp(struct vbr *s1, struct vbr *s2) {
  int i, j, k, l, m;

  if (s1->brc < s2->brc) return(-1);
  k = 0;
  l = s2->brc;
  for (i = k = m = 0; i < l; i++) {
    k = s1->c[i]-s2->c[i]-k;
    if (k) m++;
    if (k < 0) 
      k = 1;
    else 
      k = 0;
  }

  if (!m && (s1->brc == s2->brc)) return (0);
  for (; (i < s1->brc) && k; i++) {
    k = s1->c[i]-k;
    if (k < 0) 
      k = 1;
    else
      k = 0;
  }

  if (k) return(-1);
  return(1);
}


int podr (struct vbr *d1, struct vbr *d2, 
          struct vbr *k, struct vbr *r) {
  int i, j;
  struct vbr m2, m3;

  i = d1->brc;
  r->brc = d1->brc; m2.brc = d2->brc;
  for (i = 0; i < d1->brc; i++) r->c[i] = d1->c[i];
  for (i = 0; i < d2->brc; i++) m2.c[i] = d2->c[i];
  m3.brc = 1; m3.c[0] = 1;

  i = 0;
  while (intcmp(r, &m2) > 0) {
    i++;
    shiftl (1, &m2);
    shiftl (1, &m3);
  }
  
  k->brc = 1;
  k->c[0] = 0;
  while (i >= 0) {
    i--;
    if (intcmp(r, &m2) >= 0) {
      oduz (r, &m2, r);
      sab (&m3, k, k);
    }
    shiftr (1, &m2);
    shiftr (1, &m3);
  }
}


mnozc (struct vbr *a, int b, struct vbr *c) {
/***
  a - polazni broj
  b - cifra kojom mnozimo
  c -proizvod
***/
  int i, j;

  j = 0;
  for (i = 0; i < a->brc; i++) {
    j = b*a->c[i]+j;
    c->c[i] = j%10;
    j = j/10;
  }

  c->brc = a->brc;
  while (j) {
    c->c[c->brc] = j%10;
    c->brc++;
    j = j / 10;
  }
}

podc (struct vbr *a, int b, struct vbr *c) {
/***
  a - polazni broj
  b - cifra kojom delimo
  c - dobijeni koli{\vc}nik
***/
  struct vbr m1;
  int i, j, k;

  j = a->brc-1; k = 0;
  m1.brc = 1;
  m1.c[0] = 0;
  for (i = j; i >= 0; i--) {
    k = 10*k+a->c[i];
    if ((m1.brc == 1) && (m1.c[0] == 0))
      m1.c[0] = k/b;
    else {
      shift10l (1, &m1);
      m1.c[0] = k/b;
    }
    k = k % b;
  }

  c->brc = m1.brc;
  for (i = 0; i < m1.brc; i++) c->c[i] = m1.c[i];
  int_dod (c, &m1);
  for (i = c->brc-1; (i > 0) && (c->c[i]==0); i--);
  c->brc = i+1;
  return (k);
}

/*** mnozenje sa 10 na n ***/
int shift10l (int n, struct vbr *a) {
  int i, j;

  if (n <= 0) return (0);
  if (a->brc+n > MAXC) return (-1);
  for (i = a->brc; i >= 0; i--)
    a->c[i+n] = a->c[i];
  for (i = 0; i < n; i++) a->c[i] = 0;
  a->brc += n;
}

/*** deljenje sa 10 na n ***/
int shift10r (int n, struct vbr *a) {
  int i, j;

  if (a->brc <= n) {
     a->brc = 1;
     a->c[0] = 0;
     return (0);
  }
  for (i = 0; i < a->brc-n; i++) a->c[i] = a->c[i+n];
  a->brc -= n;
}


int oduzl (int p, struct vbr *s1, struct vbr *s2, struct vbr *r)
{
  int i, j, k, l, l1, l2;

  if (s1->brc-p < s2->brc) return(-1);
  k = 0;
  l1 = s2->brc;
  l2 = s1->brc-s2->brc-p;
  for (i = 0; i < l2; i++) r->c[i] = s1->c[i];
  for (i = 0, k = 0; i < l1; i++) {
    k = s1->c[l2+i]-s2->c[i]-k;
    if (k < 0) {
      r->c[l2+i] = k+10;
      k = 1;
    } else {
      r->c[l2+i] = k;
      k = 0;
    }
  }

  for (; l2+i < s1->brc; i++) {
    k = s1->c[l2+i]-k;
    if (k < 0) {
      r->c[l2+i] = k+10;
      k = 1;
    } else {
      r->c[l2+i] = k;
      k = 0;
    }
  }
  
  if (k) return(-1);
  for (r->brc = s1->brc; (r->brc > 1) && (r->c[r->brc-1] == 0); r->brc--);
  return(1);
}



podk (struct vbr *a, struct vbr *b, 
      struct vbr *c, struct vbr *d) {
  struct vbr m1, m2, m3, m4;
  int i, j, k, k1, l, l1, l2;


  l = b->brc;
  k1 = 10/(b->c[l-1]+1);
  mnozc (a, k1, &m1);
  mnozc (b, k1, &m2);
  if (m1.brc == a->brc) 
    m1.c[m1.brc++] = 0;

  j = m1.brc;
  c->brc = 1; c->c[0] = 0;
  while (j > m2.brc) {
    if (m1.c[l1=m1.brc-1] == m2.c[l2=m2.brc-1])
      k = 9;
    else
      k = (10*m1.c[l1]+m1.c[l1-1])/m2.c[l2];
    while (m2.c[l2-1]*k > (m1.c[l1]*10+m1.c[l1-1]-k*m2.c[l2])*10+
m1.c[l1-2]) k--;

    if (k > 0) {
      mnozc (&m2, k, &m3);
      if (oduzl ((m2.brc+1)-m3.brc, &m1, &m3, &m4) == -1) {
        k--;
        if (k > 0) {
          mnozc (&m2, k, &m3);
          oduzl ((m2.brc+1)-m3.brc, &m1, &m3, &m1);
        }
      } else
        int_dod (&m1, &m4);
    }
    shift10l (1, c);
    c->c[0] = k;
    m1.brc = --j;
  }

  for (i = c->brc; (i > 1) && (c->c[i-1] == 0); i--);
  c->brc = i;
  int_dod (d, &m1);
  podc (d, k1, d);
}


/*** mnozenje sa 10 na n 
int shift10l (int n, struct vbr *a) {
  int i, j;

  if ((n < 0) || (a->brc+n > MAXC)) return (-1);
  for (i = a->brc-1; i >= 0; i--)
    a->c[i+n] = a->c[i];
  for (i = 0; i < n; i++) a->c[i] = 0;
  a->brc += n;
}
***/

/*** deljenje sa 10 na n 
int shift10r (int n, struct vbr *a) {
  int i, j;

  if (a->brc <= n) {
     a->brc = 1;
     a->c[0] = 0;
     return (0);
  }
  for (i = n; i < a->brc) a->c[i-n] = a[i];
  a->brc -= n;
}
***/

int rec (struct vbr *x, struct vbr *y) {
  int i, j, k;
  struct vbr m1, m2, m3, m4;

  if (x->brc == 1)
    if (x->c[0] == 1) {
      y->brc = 2; y->c[0] = 0; y->c[1] = 1;
    } else {
      y->brc = 1; y->c[0] = 10 / x->c[0];
    }
  else {
    m1.brc = j = x->brc/2;
    for (i = 0; i < x->brc/2; i++) 
      m1.c[i] = x->c[x->brc-j+i];
    rec (&m1, &m2);
    int_dod (&m3, &m2);
    shift10l (3*m1.brc-1, &m3);
    if (x->brc & 1) shift10l (1, &m3);
    shiftl (1, &m3);
    mnoz (&m2, &m2, &m4);
    mnoz (&m4, x, &m4);
    oduz (&m3, &m4, &m3);
    y->brc = j = x->brc;
    for (i = 0; i < j; i++)
      y->c[i] = m3.c[m3.brc-x->brc+i];
    mnoz (x, y, &m4);
    m3.brc = 1; m3.c[0] = 1;
    shift10l (2*x->brc-1, &m3);
    if (intcmp(&m3, &m4) > 0) {
      oduz (&m3, &m4, &m3);
      m1.brc = 3; m1.c[0] = m1.c[1] = 0; m1.c[2] = 1;
      shift10l (2, x);
      while (intcmp (&m3, x) >= 0) {
        sab (y, &m1, y);
        oduz (&m3, x, &m3);
      }
      shift10r (1, x);
      shift10r (1, &m1);
      while (intcmp (&m3, x) >= 0) {
        sab (y, &m1, y);
        oduz (&m3, x, &m3);
      }
      shift10r (1, x);
      shift10r (1, &m1);
      while (intcmp (&m3, x) >= 0) {
        sab (y, &m1, y);
        oduz (&m3, x, &m3);
      }
    } else {
      oduz (&m4, &m3, &m3);
      m1.brc = 3; m1.c[0] = m1.c[1] = 0; m1.c[2] = 1;
      shift10l (2, x);
      while (intcmp (&m3, x) >= 0) {
        oduz (y, &m1, y);
        oduz (&m3, x, &m3);
      }
      shift10r (1, x);
      shift10r (1, &m1);
      while (intcmp (&m3, x) >= 0) {
        oduz (y, &m1, y);
        oduz (&m3, x, &m3);
      }
      shift10r (1, x);
      shift10r (1, &m1);
      while (intcmp (&m3, x) >= 0) {
        oduz (y, &m1, y);
        oduz (&m3, x, &m3);
      }
      if ((m3.brc != 1) || (m3.c[0] != 0)) {
        oduz (y, &m1, y);
        oduz (&m3, x, &m3);
      }
    }
  }
}


int nzd (struct vbr *x1, struct vbr *x2, struct vbr *r) {
  struct vbr m1, m2, m3;

  int_dod (&m1, x1);
  int_dod (&m2, x2);
  while ((m2.brc > 1) || (m2.c[0] != 0)) {
    podr (&m1, &m2, &m3, r);
    int_dod (&m1, &m2);
    int_dod (&m2, r);
  }

  int_dod (r, &m1);
}


int nzdb (struct vbr *a, struct vbr *b, struct vbr *c) {
  struct vbr m1, m2, m3;
  int i, j, k;

  int_dod (&m1, a);
  int_dod (&m2, b);

  i = 0;
  while (!(m1.c[0] & 1) && !(m2.c[0] & 1)) {
    i++;
    shiftr (1, &m1);
    shiftr (1, &m2);
  }

  if (m1.c[0] & 1) {
    k = -1;
    int_dod (&m3, &m2);
  } else {
    k = 1;
    int_dod (&m3, &m1);
  }

  while (!(m3.c[0] & 1)) shiftr (1, &m3);
  if (k == 1)
    int_dod (&m1, &m3);
  else
    int_dod (&m2, &m3);

  while (k = intcmp(&m1, &m2)) {
    if (k == 1)
      oduz (&m1, &m2, &m3);
    else
      oduz (&m2, &m1, &m3);

    while (!(m3.c[0] & 1)) shiftr(1, &m3);

    if (k == 1)
      int_dod (&m1, &m3);
    else
      int_dod (&m2, &m3);
  }

  int_dod (c, &m3);
  shiftl (i, c);
}

kv_koren (struct vbr *a, struct vbr *b) {
  int i, j;
  struct vbr m1, m2, m3, m4;

  int_dod (&m2, a);
  shiftr (1, &m2);

  do {
    int_dod (&m1, &m2);
    podr (a, &m1, &m3, &m4);
    sab (&m2, &m3, &m2);
    shiftr (1, &m2);
  } while (intcmp (&m1, &m2) > 0);

  int_dod (b, &m1);
}



int fakt_pq (struct vbr *a, struct vbr *b, struct vbr *c) {
  int i, j;
  struct vbr p, q, a2, p2mq2;

  kv_koren(a, &p);
  int_dod(&a2, a);
  shiftr (1, &a2);
  mnoz(&p, &p, &p2mq2);
  q.brc = 1; q.c[0] = 0;

  while (intcmp(&a2, &p) > 0) {
    switch (intcmp(&p2mq2, a)) {
      case 1:
        oduz (&p2mq2, &q, &p2mq2);
        oduz (&p2mq2, &q, &p2mq2);
        intdec (&p2mq2);
        intinc (&q);
        break;

      case 0:
        sab (&p, &q, b);
        oduz (&p, &q, c);
        return (0);

     case -1:
       sab (&p2mq2, &p, &p2mq2);
       sab (&p2mq2, &p, &p2mq2);
       intinc (&p2mq2);
       intinc (&p);
       break;
    }
  }
  return (-1);
}

int fakt_pol (int n, int m, struct vbr *x, struct vbr *y) {
  int i, j, k;
  struct vbr m0, m1, m2, m3, m4;

  for (i = 0; i < n; i++) {
    intrand (x->brc, &m1);
    int_dod (&m0, &m1);
    for (j = k = 2; j < m; j++) {
      nzd(x, &m1, &m2);
      if ((m2.brc > 1) || (m2.c[0] != 1)) break;
      mnoz (&m1, &m1, &m3);
      intinc(&m3);
      podr (&m3, x, &m2, &m3);
      int_dod(&m1, &m3);
      if (intcmp(&m1, &m0) > 0) 
        oduz (&m1, &m0, &m4);
      else
        oduz (&m0, &m1, &m4);
      nzd (&m4, x, &m2);
      if ((m2.brc > 1) || (m2.c[0] != 1))
        break;
      if (j == k) {
        int_dod (&m0, &m1);
        k *= 2;
      }
    }

    if (j < m) break;
  }

  if (i < n) {
    int_dod (y, &m2);
    return(0);
  }

  return(1);
}

int prost (long n) {
/*** 
funkcija proverava da li je broj n prost;
rezultat je:
1, ako broj n jeste prost
0, ako broj n nije prost
***/
  long i;

  for (i = 2; i*i <= n; i++)
    if (n%i == 0) return(0);
  return(1);
}

void broj_u_niz (long n, struct vbr *a) {
/***
Broj n se predstavlja pomocu strukture a
***/
  int i;

  if (!n) {
    a->brc = 1;
    a->c[1] = 0;
    return;
  }
  for (i = 0; n; i++) {
    a->c[i] = n%10;
    n = n / 10;
  }
  a->brc = i;
}

gen_proste (int pn, struct vbr *pt) {
  int i;
  long j;

  for (i = 0, j = 2; (i < pn) && (j > 0); j++)
    if (prost(j)) 
      broj_u_niz (j, &pt[i++]);
}


int razlozi (int pn, struct vbr *pt, char pe[], struct vbr
*a) {
  int i, j, k;
  struct vbr m1, m2, m3;

  int_dod (&m1, a);
  for (i = 0; i < pn; i++) pe[i] = 0;

  while ((m1.brc > 1) || (m1.c[0] > 1)) {
    for (i = 0; i < pn; i++) {
      podr (&m1, &pt[i], &m2, &m3);
      if ((m3.brc == 1) && (m3.c[0] == 0)) {
     pe[i] += 1;
     int_dod (&m1, &m2);
     break;
      }
    }
    if (i == pn) break;
  }

  if ((m1.brc == 1) && (m1.c[0] == 1)) i = 1; else i = 0;
  return (i);
}


int resis (int en, char pe[][MAXPP+1], char he[][MAXPP+1]) {
  char c;
  int i, j, k, l;

  for (i = 0; i <= en; i++)
    for (j = 0; j <= en; j++)
      he[i][j] = (i==j);

  for (i = 0; i < en; i++) {
    for (j = i; (j <= en) && !(pe[j][i]%2); j++);
    if (j > en) continue;
    if (j != i)
      for (k = 0; k <= en; k++) {
        c = pe[j][k]; pe[j][k] = pe[i][k]; pe[i][k] = c;
        c = he[j][k]; he[j][k] = he[i][k]; he[i][k] = c;
      }
    for (j = i+1; j <= en; j++)
      if (pe[j][i]%2) {
        for (k = 0; k <=en; k++) {
          pe[j][k] = pe[j][k]+pe[i][k];
          he[j][k] = (he[j][k]+he[i][k])%2;
        }
      }
  }
}

izaberi_p_ove (int bp, struct vbr *a) {
  int i, j, k, n;
  struct vbr m1, m2, m3, m4, m5;

  gen_proste (bp, pt);
  
  i = 0;
  m1.brc = m1.c[0] = 1;
  while (i < bp+1) {
    mnoz (a, &m1, &m2);
    kv_koren (&m2, &m3);
    intinc (&m3);
    mnoz (&m3, &m3, &m4);
    podr (&m4, a, &m2, &m5);
    if (razlozi(bp, pt, pe[i], &m5)) {
      intprln(&m3);
      int_dod (&xg[i], &m3);
      i++;
    }
    intinc(&m1);
  }

  getch();

  k = resis (bp, pe, he);
  printf ("\n");
  for (i = 0; i <= bp; i++) {
    for (j = 0; j < bp; j++) 
      printf ("%2d", pe[i][j]);
    printf ("     ");
    for (j = 0; j <= bp; j++) 
      printf ("%2d", he[i][j]);
    printf ("\n");
  }
  
  for (k = 0; k <= bp; k++) {
    for (j = 0; j < bp; j++)
      if (pe[k][j]%2) break;
    if (j == bp) {
      m1.brc = m1.c[0] = 1;
      for (i = 0; i <= bp; i++)
        if (he[k][i]) {
          mnoz (&m1, &xg[i], &m2);
          podr (&m2, a, &m3, &m1);
        }
      m2.brc = m2.c[0] = 1;
      for (i = 0; i <= bp; i++)
        for (j = 0; j < pe[k][i]/2; j++) {
          mnoz (&m2, &pt[i], &m3);
          podr (&m3, a, &m4, &m2);
/***          intas (m2, m3); ***/
        }
      intprln(&m1);
      intprln(&m2);
      if (intcmp(&m1, &m2)) {
        if (intcmp(&m1, &m2) > 0)
          oduz(&m1, &m2, &m3);
        else
          oduz(&m2, &m1, &m3);
        nzd (&m3, a, &m1);
        intprln (&m1);
        if ((m1.brc != 1) || (m1.c[0] != 1)) break;
      }
    }
  }

  if (k == bp) 
    printf("Nisam uspeo!\n");
}


main0 () {
  char ch;
  int i, j, k;
  struct vbr a, b, c, d;

  printf ("1 - shiftovanje\n");
  printf ("2 - mnozenje\n");
  printf ("3 - deljenje\n");
  printf ("4 - nzd\n");
  printf ("5 - fakt\n");
  printf ("6 - rec\n");
  printf ("7 - generisanje prostih\n");
  printf ("8 - korenovanje\n");
  printf ("9 - verizni\n");
  printf ("A - faktprizacija kvadratima\n");
  printf ("B - Polard-Rho faktprizacija\n");
  while (((ch = getch()) != 'q') && (ch != 'Q')) {
    switch (ch) {
      case '1':
        printf ("koliko mesta: ");
        scanf ("%d", &k);
        intsc (&a);
        shiftl (k, &a);
        intprln (&a);
        break;

      case '2':
        intsc (&a);
        intsc (&b);
        mnoz (&a, &b, &c);
        intprln (&c);
        mnoz1 (&a, &b, &c);
        intprln (&c);
        mnozr (&a, &b, &c);
        intprln (&c);
        break;

      case '3':
        intsc (&a);
        intsc (&b);
        podr (&a, &b, &c, &d);
        intprln (&c);
        intprln (&d);
        podk (&a, &b, &c, &d);
        intprln (&c);
        intprln (&d);
        break;

      case '4':
        intsc (&a);
        intsc (&b);
	nzd (&a, &b, &c);
	nzdb (&a, &b, &d);
	intprln (&c);
	intprln (&d);
        break;

/***
      case '5':
        intsc (a);
        if (intpol (10, 10, a, b))
          printf ("Prost!\n");
        else
          intprln (b);
        break;
***/

      case '5':
        intsc (&a);
        if (fakt_pq (&a, &b, &c))
          printf ("Prost!\n");
        else {
          intprln (&b);
          intprln (&c);
        }
        break;

      case '6':
        intsc (&a);
        rec (&a, &b);
        intprln (&b);
        break;

      case '7':
        printf ("koliko prostih: ");
        scanf ("%d", &k);
        gen_proste (k, pt);
        for (i = 0; i < k; i++)
          intprln (&pt[i]);
        break;

      case '8':
        intsc (&a);
        kv_koren (&a, &b);
        intprln (&b);
        break;

      case '9':
        printf ("Koliko p-ova: ");
        scanf ("%d", &k);
        intsc (&a);
        izaberi_p_ove (k, &a);
/***
        intprln (b);
***/
        break;

      case 'A':
      case 'a':
        intsc (&a);
        fakt_pq (&a, &b, &c);
        intprln (&b);
        intprln (&c);
        break;

      case 'B':
      case 'b':
        intsc (&a);
        fakt_pol (10, 10, &a, &b);
        intprln (&b);
        break;
    }

    printf ("1 - shiftovanje\n");
    printf ("2 - mnozenje\n");
    printf ("3 - deljenje\n");
    printf ("4 - nzd\n");
    printf ("5 - fakt\n");
    printf ("6 - rec\n");
    printf ("7 - generisanje prostih\n");
    printf ("8 - korenovanje\n");
    printf ("9 - verizni\n");
    printf ("A - faktprizacija kvadratima\n");
    printf ("B - Polard-Rho faktprizacija\n");
  }
}



#define MAX 1000

void main(void)
{
	int i;
	struct vbr f[MAX];

	f[0].brc	= 1;
	f[0].c[0]	= 0;
	f[1].brc	= 1;
	f[1].c[0]	= 1;

	printf("0\t"); intprln(&f[0]);
	printf("1\t"); intprln(&f[1]);

	for(i=2; i<MAX-1; i++)
	{
		sab(&f[i-1], &f[i-2], &f[i]);
		printf("%d\t", i); intprln(&f[i]);
	}
}

