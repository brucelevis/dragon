#ifndef _MY_RC4_H
#define _MY_RC4_H

/* 32 bit.  For Alpha, things may get weird */
#define RC4_32_INT unsigned int

typedef struct rc4_key
{      
	unsigned char state[256];       
	unsigned char x;        
	unsigned char y;
} RC4_KEY;

typedef void (* f_RC4_set_key)(RC4_KEY *key, int len, const unsigned char *data);
typedef void (* f_RC4_crypt)(unsigned char *data, int len, RC4_KEY* key);

#endif