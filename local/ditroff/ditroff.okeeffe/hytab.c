#ifndef lint
static char sccsid[] = "@(#)hytab.c	1.1 (CWI) 85/07/17";
#endif lint

/*
 * Hyphenation digram tables
 */

/*
 * These table are for the ORIGINAL dialect
 */

char	bxh[1][13] = {
	0060,0000,0040,0000,0040,0000,0000,0040,0000,0000,0040,0000,0040
};

char	hxx[26][13] = {
	0006,0042,0041,0123,0021,0024,0063,0042,0002,0043,0021,0001,0022,
	0140,0000,0200,0003,0260,0006,0000,0160,0007,0000,0140,0000,0320,
	0220,0000,0160,0005,0240,0010,0000,0100,0006,0000,0200,0000,0320,
	0240,0000,0120,0003,0140,0000,0000,0240,0010,0000,0220,0000,0160,
	0042,0023,0041,0040,0040,0022,0043,0041,0030,0064,0021,0000,0041,
	0100,0000,0140,0000,0220,0006,0000,0140,0003,0000,0200,0000,0000,
	0200,0000,0120,0002,0220,0010,0000,0160,0006,0000,0140,0000,0320,
	0020,0000,0020,0000,0020,0000,0000,0020,0000,0000,0020,0000,0000,
	0043,0163,0065,0044,0022,0043,0104,0042,0061,0146,0061,0000,0007,
	0100,0000,0140,0000,0040,0000,0000,0100,0000,0000,0120,0000,0000,
	0140,0000,0040,0011,0060,0004,0001,0120,0003,0000,0140,0000,0040,
	0200,0000,0100,0000,0140,0000,0000,0140,0000,0000,0140,0000,0240,
	0200,0000,0140,0000,0160,0000,0000,0220,0000,0000,0140,0000,0240,
	0200,0000,0140,0000,0160,0000,0000,0220,0000,0000,0060,0000,0240,
	0021,0043,0041,0121,0040,0023,0042,0003,0142,0042,0061,0001,0022,
	0120,0000,0140,0010,0140,0010,0000,0140,0002,0000,0120,0000,0120,
	0000,0000,0000,0000,0360,0000,0000,0000,0000,0000,0160,0000,0000,
	0100,0000,0040,0005,0120,0000,0000,0100,0000,0000,0060,0000,0140,
	0140,0040,0100,0001,0240,0041,0000,0242,0000,0002,0140,0000,0100,
	0240,0000,0120,0002,0200,0000,0000,0320,0007,0000,0240,0000,0340,
	0101,0021,0041,0020,0040,0005,0042,0121,0002,0021,0201,0000,0020,
	0160,0000,0100,0000,0140,0000,0000,0160,0006,0000,0220,0000,0140,
	0140,0000,0020,0001,0020,0000,0000,0100,0001,0000,0300,0000,0000,
	0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,
	0106,0041,0040,0147,0040,0000,0063,0041,0001,0102,0160,0002,0002,
	0300,0000,0040,0017,0140,0017,0000,0240,0000,0000,0140,0000,0120,
};

char	bxxh[26][13] = {
	0005,0150,0153,0062,0062,0246,0152,0127,0146,0203,0310,0017,0206,
	0100,0000,0120,0000,0140,0000,0000,0100,0000,0000,0120,0000,0060,
	0100,0000,0040,0000,0060,0000,0000,0060,0000,0000,0220,0000,0040,
	0100,0000,0120,0000,0200,0000,0000,0100,0000,0000,0140,0000,0060,
	0043,0142,0046,0140,0062,0147,0210,0131,0046,0106,0246,0017,0111,
	0060,0000,0020,0000,0060,0000,0000,0040,0000,0000,0100,0000,0000,
	0060,0000,0040,0000,0040,0000,0000,0040,0000,0000,0100,0000,0040,
	0100,0000,0100,0000,0100,0000,0000,0040,0000,0000,0100,0000,0140,
	0066,0045,0145,0140,0000,0070,0377,0030,0130,0103,0003,0017,0006,
	0040,0000,0040,0000,0020,0000,0000,0040,0000,0000,0100,0000,0000,
	0200,0000,0020,0000,0140,0000,0000,0120,0000,0000,0120,0000,0040,
	0120,0000,0040,0000,0060,0000,0000,0060,0000,0000,0160,0000,0040,
	0120,0000,0040,0000,0120,0000,0000,0040,0000,0000,0160,0000,0040,
	0120,0000,0020,0000,0140,0000,0000,0120,0000,0000,0140,0000,0040,
	0051,0126,0150,0140,0060,0210,0146,0006,0006,0165,0003,0017,0244,
	0120,0000,0040,0000,0160,0000,0000,0140,0000,0000,0060,0000,0140,
	0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,
	0140,0000,0140,0000,0060,0000,0000,0100,0000,0000,0140,0000,0020,
	0120,0000,0020,0000,0060,0000,0000,0060,0000,0000,0060,0000,0040,
	0140,0000,0020,0000,0100,0000,0000,0140,0000,0000,0140,0000,0020,
	0070,0125,0051,0162,0120,0105,0126,0104,0006,0044,0000,0017,0052,
	0140,0000,0020,0000,0140,0000,0000,0060,0000,0000,0060,0000,0040,
	0020,0000,0000,0000,0020,0000,0000,0000,0000,0000,0000,0000,0060,
	0140,0000,0160,0000,0200,0000,0000,0140,0000,0000,0000,0000,0240,
	0065,0042,0060,0200,0000,0210,0222,0146,0006,0204,0220,0012,0003,
	0240,0000,0020,0000,0120,0000,0000,0200,0000,0000,0200,0000,0240,
};

char	xhx[26][13] = {
	0032,0146,0042,0107,0076,0102,0042,0146,0202,0050,0006,0000,0051,
	0036,0377,0057,0013,0057,0366,0377,0057,0001,0377,0057,0000,0040,
	0037,0377,0020,0000,0100,0022,0377,0057,0362,0116,0100,0000,0017,
	0057,0377,0057,0031,0137,0363,0377,0037,0362,0270,0077,0000,0117,
	0074,0142,0012,0236,0076,0125,0063,0165,0341,0046,0047,0000,0024,
	0020,0017,0075,0377,0040,0001,0377,0017,0001,0204,0020,0000,0040,
	0057,0017,0057,0340,0140,0362,0314,0117,0003,0302,0100,0000,0057,
	0057,0357,0077,0017,0100,0366,0314,0057,0342,0346,0037,0000,0060,
	0252,0145,0072,0157,0377,0165,0063,0066,0164,0050,0363,0000,0362,
	0000,0000,0020,0000,0020,0000,0000,0017,0000,0000,0020,0000,0000,
	0117,0017,0237,0377,0200,0354,0125,0110,0004,0257,0000,0000,0300,
	0057,0367,0054,0357,0157,0216,0314,0114,0217,0353,0053,0000,0057,
	0077,0213,0077,0077,0177,0317,0377,0114,0377,0352,0077,0000,0076,
	0077,0213,0077,0077,0157,0177,0377,0054,0377,0352,0117,0000,0075,
	0125,0230,0065,0216,0057,0066,0063,0047,0345,0126,0011,0000,0033,
	0057,0377,0051,0360,0120,0361,0273,0056,0001,0256,0057,0000,0060,
	0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,0000,
	0076,0310,0056,0310,0137,0174,0273,0055,0335,0266,0033,0000,0155,
	0077,0157,0057,0360,0057,0063,0042,0024,0077,0206,0020,0000,0040,
	0057,0037,0077,0360,0100,0365,0377,0037,0362,0176,0050,0000,0026,
	0167,0146,0042,0112,0077,0110,0062,0254,0366,0052,0377,0000,0163,
	0060,0000,0040,0000,0120,0000,0377,0060,0012,0000,0037,0000,0257,
	0037,0232,0157,0361,0040,0003,0125,0010,0001,0256,0000,0000,0340,
	0377,0377,0377,0377,0377,0377,0377,0377,0377,0377,0377,0017,0277,
	0253,0315,0257,0216,0377,0206,0146,0306,0371,0126,0232,0000,0004,
	0057,0012,0100,0360,0160,0360,0000,0040,0000,0017,0157,0000,0176,
};

char	xxh[26][13] = {
	0045,0150,0154,0162,0042,0246,0210,0147,0152,0103,0230,0017,0206,
	0100,0000,0040,0000,0140,0000,0000,0100,0000,0021,0120,0017,0060,
	0100,0000,0040,0002,0140,0320,0000,0060,0000,0001,0220,0017,0040,
	0100,0001,0120,0001,0241,0000,0000,0100,0000,0020,0140,0017,0060,
	0023,0162,0046,0142,0022,0207,0210,0131,0052,0106,0250,0017,0110,
	0060,0000,0042,0000,0160,0000,0000,0040,0000,0212,0100,0017,0000,
	0140,0000,0040,0002,0140,0000,0000,0120,0000,0040,0120,0017,0040,
	0100,0000,0100,0000,0140,0001,0021,0140,0000,0046,0100,0017,0140,
	0066,0045,0025,0201,0020,0130,0146,0030,0130,0103,0025,0017,0006,
	0100,0000,0040,0000,0020,0000,0000,0040,0000,0000,0200,0017,0000,
	0200,0000,0020,0001,0140,0000,0000,0140,0000,0000,0120,0017,0040,
	0120,0026,0042,0020,0140,0161,0042,0143,0000,0022,0162,0017,0040,
	0121,0042,0060,0020,0140,0200,0000,0123,0000,0021,0220,0017,0041,
	0121,0042,0060,0120,0140,0200,0000,0123,0000,0021,0160,0017,0041,
	0051,0126,0150,0141,0060,0210,0146,0066,0026,0165,0026,0017,0247,
	0120,0000,0040,0003,0160,0000,0000,0140,0000,0021,0100,0017,0140,
	0000,0000,0000,0000,0200,0000,0000,0000,0000,0000,0000,0017,0000,
	0141,0023,0122,0040,0160,0143,0042,0142,0000,0047,0143,0017,0020,
	0120,0000,0040,0006,0140,0060,0000,0141,0000,0026,0100,0017,0040,
	0140,0000,0020,0007,0100,0000,0000,0140,0000,0001,0140,0017,0020,
	0110,0125,0051,0162,0120,0125,0127,0104,0006,0104,0000,0017,0052,
	0140,0000,0040,0000,0160,0000,0000,0140,0000,0000,0060,0017,0000,
	0040,0005,0020,0000,0040,0313,0231,0030,0000,0140,0000,0017,0056,
	0140,0000,0160,0000,0200,0000,0000,0140,0000,0000,0000,0017,0240,
	0065,0042,0060,0040,0000,0206,0231,0146,0006,0224,0220,0017,0004,
	0240,0000,0020,0000,0140,0000,0000,0220,0000,0000,0200,0017,0141,
};

/*
 * These tables are for the DUTCH dialect
 */

char translate[26] =
{   /* a */ 29,  /* b */ 44,  /* c */ 40,  /* d */ 45,  /* e */ 30,
    /* f */ 46,  /* g */ 47,  /* h */ 42,  /* i */ 31,  /* j */ 41,
    /* k */ 43,  /* l */ 48,  /* m */ 49,  /* n */ 50,  /* o */ 33,
    /* p */ 51,  /* q */ 39,  /* r */ 52,  /* s */ 53,  /* t */ 54,
    /* u */ 32,  /* v */ 55,  /* w */ 56,  /* x */ 57,  /* y */ 38,
    /* z */ 58
};

char comprimation[15][14] = {

/*       a    e    i    u    o                        y              j    h*/

/*aa*/   0,   0,   1,   7,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,
/*ee*/   0,   0,   0,  12,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*oo*/   0,   0,   5,   0,   0,   0,   0,   0,   0,   6,   0,   0,   0,   0,
/*a */  26,   8,   9,  10,   0,   0,   0,   0,   0,  11,   0,   0,   0,   0,
/*e */  -1,  27,  13,  34,   0,   0,   0,   0,   0,  15,   0,   0,   0,   0,
/*i */   0,  35,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  17,   0,
/*u */   0,   0,  21,  25,   0,   0,   0,   0,   0,  22,   0,   0,   0,   0,
/*o */   0,  36,  18,  37,  28,   0,   0,   0,   0,  20,   0,   0,   0,   0,
/*eu*/   0,   0,  14,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*ie*/   0,   0,   0,  16,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*oe*/   0,   0,   3,  24,   0,   0,   0,   0,   0,   4,   0,   0,   0,   0,
/*ou*/   0,  19,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*  */   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*q */   0,   0,   0,  60,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
/*c */   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  61

} ;


char consonant[23][23] = {

/*      q  c  j  h  k  b  d  f  g  l  m  n  p  r  s  t  v  w  x  z  - qu ch*/

/* q*/ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 5,-1,-1,
/* c*/ -1,-1,-1,-1,-4,-1,-1,-1,-1, 3,-1,-1,-1, 3,-1,-1,-1,-1,-1,-1, 5,-1,-1,
/* j*/ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 5,-1,-1,
/* h*/ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2,-2,-1,-1,-1,-1,-1,-1,-1,-1, 5,-1,-1,
/* k*/ -1,-1,-1,-1,-1,-1,-1,-1,-1, 3,-1, 5,-1, 3,-5,-1,-1, 3,-1,-1, 5,-1,-1,
/* b*/ -1,-1,-1,-1,-1,-1,-1,-1,-1, 3,-1,-1,-1, 3,-1,-1,-1,-1,-1,-1, 5,-1,-1,
/* d*/ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 3,-5,-1,-1, 5,-1,-1, 5,-1,-1,
/* f*/ -1,-1,-1,-1,-1,-1,-4,-1,-1, 5,-1,-1,-1, 5,-5,-4,-1,-1,-1,-1, 5,-1,-1,
/* g*/ -1,-1,-1, 2,-1,-1,-4,-1,-1, 5,-1,-1,-1, 5,-5,-1,-1,-1,-1,-1, 5,-1,-1,
/* l*/ -1,-1,-1,-1,-4,-1,-4,-4,-4,-1,-4,-1,-4,-1,-4,-1,-1,-1,-1,-1, 5,-1,-1,
/* m*/ -1,-1,-1,-1,-1,-1,-4,-1,-1,-1,-1,-1,-1,-1,-2,-1,-1,-1,-1,-1, 5,-1,-1,
/* n*/ -1,-1,-1,-1,-4,-1,-4,-1,-4,-1,-1,-1,-1,-1,-4,-4,-1,-1,-1,-1, 5,-1,-1,
/* p*/ -1,-1, 2, 5,-1,-1,-1,-1,-1, 3,-1,-1,-1, 3,-5,-1,-1,-1,-1,-1, 5,-1,-1,
/* r*/ -1,-1,-1,-1,-4,-1,-4,-1,-4,-1,-4,-4,-4,-1,-5,-1,-1,-1,-1,-1, 5,-1,-1,
/* s*/ -1,-1,-1,-5,-1,-1,-1,-1,-1, 5, 5, 5, 5,-1,-1, 5,-1,-1,-1,-1, 5, 5, 3,
/* t*/ -1,-1,-5,-5,-1,-1,-1,-1,-1,-1,-1,-1,-1, 5,-4,-1,-1,-5,-1,-1, 5,-1,-1,
/* v*/ -1,-1,-1,-1,-1,-1,-1,-1,-1, 3,-1,-1,-1, 3,-1,-1,-1,-1,-1,-1, 5,-1,-1,
/* w*/ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 3,-1,-1,-1,-1,-1,-1, 5,-1,-1,
/* x*/ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 5,-1,-1,
/* z*/ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 3,-1,-1, 5,-1,-1,
/* -*/  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
/*qu*/ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 5,-1,-1,
/*ch*/ -1,-1,-1,-1,-1,-1,-1,-1,-1, 3,-1,-1,-1, 3,-1,-4,-1,-1,-1,-1, 5,-1,-1
} ;


char prefix[7][3] = {
	55, 30, 50,        /*'ven'*/
	54, 30, 52,        /*'ter'*/
	50, 33, 47,        /*'nog'*/
	45, 30, 52,        /*'der'*/
	55, 30, 52,        /*'ver'*/
	55, 29, 50,        /*'van'*/
	42, 35, 52         /*'hier'*/
} ;
 
