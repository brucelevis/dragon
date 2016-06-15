#include "myrc4.h"

void swap_byte(unsigned char *a, unsigned char *b)
{
	unsigned char x;
	x=*a;*a=*b;*b=x;
}

void RC4_set_key(RC4_KEY *key, int len, const unsigned char *data)
{
	unsigned char index1;
	unsigned char index2;
	unsigned char* state;
	short counter;

	state = &(key->state[0]);

	for(counter = 0; counter < 256; counter++)
		state[counter] = (unsigned char)counter;

	key->x = 0;
	key->y = 0;
	index1 = 0;
	index2 = 0;

	for(counter = 0; counter < 256; counter++)
	{
		index2 = (data[index1] + state[counter] + index2) % 256;
		swap_byte(&state[counter], &state[index2]);
		index1 = (index1 + 1) % len;
	}
}

void RC4_crypt(unsigned char *data, int len, RC4_KEY* key)
{
	unsigned char x;
	unsigned char y;
	unsigned char* state;
	unsigned char xorIndex;

	x = key->x;
	y = key->y;
	state = &(key->state[0]);

	for(int i = 0; i < len; i++)
	{
		x = (x + 1) % 256;
		y = (state[x] + y) % 256;
		swap_byte(&state[x], &state[y]);
		xorIndex = (state[x] + state[y]) % 256;
		data[i] ^= state[xorIndex];
	}
	key->x = x;
	key->y = y;
}