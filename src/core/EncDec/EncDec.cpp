//#include "../../engine/include/engine.h"
#include "EncDec.h"
#include <string.h>

CEncrypt::CEncrypt()
{
	/*
	m_fDES_random_key  = ::DES_random_key  ;
	m_fDES_set_key  = ::DES_set_key  ;
	m_fDES_encrypt1 = ::DES_encrypt1 ;
	m_fDES_encrypt3 = ::DES_encrypt3 ;
	m_fDES_decrypt3 = ::DES_decrypt3 ;

	m_fRC5_32_decrypt = ::RC5_32_decrypt ;
	m_fRC5_32_encrypt = ::RC5_32_encrypt ;
	m_fRC5_32_set_key = ::RC5_32_set_key ;

	m_fidea_set_encrypt_key = ::idea_set_encrypt_key  ;
	m_fidea_set_decrypt_key = ::idea_set_decrypt_key  ;
	m_fidea_encrypt = ::idea_encrypt;

	m_fCAST_set_key = ::CAST_set_key;
	m_fCAST_encrypt = ::CAST_encrypt;
	m_fCAST_decrypt = ::CAST_decrypt;
	*/

	bzero(&key_des, sizeof(key_des));
	bzero(&key_rc5, sizeof(key_rc5));
	bzero(&key_rc4_encrypt, sizeof(key_rc4_encrypt));
	bzero(&key_rc4_decrypt, sizeof(key_rc4_decrypt));
	haveKey_des = false;
	haveKey_rc5 = false;
	haveKey_rc4 = false;
	//key_des = new DES_key_schedule;
	//key_rc5 = new RC5_32_KEY;
	method = ENCDEC_NONE;
	enc_mask = dec_mask = 0;
}

/*
CEncrypt::~CEncrypt()
{
	//if (key_des) delete key_des; 
	//if (key_rc5) delete key_rc5; 
}
*/

void CEncrypt::DES_set_key(const_DES_cblock *key, DES_key_schedule *schedule)
{
	//m_fDES_set_key(key,schedule);
	::DES_set_key(key, schedule);
}

void CEncrypt::DES_random_key(DES_cblock *ret)
{
	//m_fDES_random_key(ret);
	::DES_random_key(ret);
}

void CEncrypt::DES_encrypt1(DES_LONG *data, DES_key_schedule *ks, int enc)
{
	//m_fDES_encrypt1(data,ks,&MyDES_SPtrans,enc);
	::DES_encrypt1(data, ks, &MyDES_SPtrans, enc);
}

void CEncrypt::DES_encrypt3(DES_LONG *data, DES_key_schedule *ks1,
			DES_key_schedule *ks2, DES_key_schedule *ks3)
{
	//m_fDES_encrypt3(data,ks1,ks2,ks3,&MyDES_SPtrans);
	::DES_encrypt3(data, ks1, ks2, ks3, &MyDES_SPtrans);
}

void CEncrypt::DES_decrypt3(DES_LONG *data, DES_key_schedule *ks1,
			DES_key_schedule *ks2, DES_key_schedule *ks3)
{
	//m_fDES_decrypt3(data,ks1,ks2,ks3,&MyDES_SPtrans);
	::DES_decrypt3(data, ks1, ks2, ks3, &MyDES_SPtrans);
}

void CEncrypt::RC5_32_set_key(RC5_32_KEY *key, int len, const unsigned char *data,
	int rounds)
{
	//m_fRC5_32_set_key(key,len,data,rounds);
	::RC5_32_set_key(key,len,data,rounds);
}

void CEncrypt::RC5_32_encrypt(RC5_32_INT*d, RC5_32_KEY *key)
{
	//m_fRC5_32_encrypt(d,key);
	::RC5_32_encrypt(d,key);
}

void CEncrypt::RC5_32_decrypt(RC5_32_INT*d, RC5_32_KEY *key)
{
	//m_fRC5_32_decrypt(d,key);
	::RC5_32_decrypt(d,key);
}

void CEncrypt::idea_set_encrypt_key(const unsigned char *key, IDEA_KEY_SCHEDULE *ks)
{
	//m_fidea_set_encrypt_key(key,ks);
	::idea_set_encrypt_key(key,ks);
}

void CEncrypt::idea_set_decrypt_key(IDEA_KEY_SCHEDULE *ek, IDEA_KEY_SCHEDULE *dk)
{
	//m_fidea_set_decrypt_key(ek,dk);
	::idea_set_decrypt_key(ek,dk);
}

void CEncrypt::idea_encrypt(unsigned long *d, IDEA_KEY_SCHEDULE *key)
{
	//m_fidea_encrypt(d,key);
	::idea_encrypt(d,key);
}

void CEncrypt::CAST_set_key(CAST_KEY *key, int len, const unsigned char *data)
{
	//m_fCAST_set_key(key,len,data);
	::CAST_set_key(key,len,data);
}

void CEncrypt::CAST_encrypt(CAST_LONG *data, CAST_KEY *key)
{
	//m_fCAST_encrypt(data,key);
	::CAST_encrypt(data,key);
}

void CEncrypt::CAST_decrypt(CAST_LONG *data, CAST_KEY *key)
{
	//m_fCAST_decrypt(data,key);
	::CAST_decrypt(data,key);
}

void CEncrypt::RC4_set_key(RC4_KEY *key, int len, const unsigned char *data)
{
	::RC4_set_key(key, len, data);
}

void CEncrypt::RC4_encrypt(unsigned char *data, int len, RC4_KEY* key)
{
	::RC4_crypt(data, len, key);
}

void CEncrypt::RC4_decrypt(unsigned char *data, int len, RC4_KEY* key)
{
	::RC4_crypt(data, len, key);
}

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
int CEncrypt::encdec_des(unsigned char *data, unsigned int nLen, bool enc)
{
	if ((0==data)||(!haveKey_des)) return -1;

	unsigned int offset = 0;
	while (offset<=nLen-8)
	{
		if (0x80000000 & (enc?enc_mask:dec_mask))
			DES_encrypt1((DES_LONG*)(data+offset), &key_des, enc);
		offset += 8;
		if (enc)
			enc_mask = ROTATE_LEFT(enc_mask, 1);
		else
			dec_mask = ROTATE_LEFT(dec_mask, 1);
	}

	return nLen-offset;
}

int CEncrypt::encdec_rc5(unsigned char *data, unsigned int nLen, bool enc)
{
	if ((0==data)||(!haveKey_rc5)) return -1;

	unsigned int offset = 0;
	while (offset<=nLen-8)
	{
		RC5_32_INT d[2];
		if (0x80000000 & (enc?enc_mask:dec_mask))
		{
			memcpy(d, data+offset, sizeof(d));
			if (enc)
				RC5_32_encrypt(d, &key_rc5);
			else
				RC5_32_decrypt(d, &key_rc5);
			memcpy(data+offset, d, sizeof(d));
		}
		offset += sizeof(d);//不为8则出错
		if (enc)
			enc_mask = ROTATE_LEFT(enc_mask, 1);
		else
			dec_mask = ROTATE_LEFT(dec_mask, 1);
	}

	return nLen-offset;
}

int CEncrypt::encdec_rc4(unsigned char *data, unsigned int nLen, bool enc)
{
	if ((0==data)||(!haveKey_rc4)) return -1;

	unsigned int offset = 0;
	while (offset<=nLen-8)
	{
		if (0x80000000 & (enc?enc_mask:dec_mask))
		{
			if (enc)
				RC4_encrypt(data+offset, 8, &key_rc4_encrypt);
			else
				RC4_decrypt(data+offset, 8, &key_rc4_decrypt);
		}
		offset += 8;
		if (enc)
			enc_mask = ROTATE_LEFT(enc_mask, 1);
		else
			dec_mask = ROTATE_LEFT(dec_mask, 1);
	}

	return nLen-offset;
}


void CEncrypt::random_key_des(DES_cblock *ret)
{
	::DES_random_key(ret);
}

void CEncrypt::set_key_des(const_DES_cblock *key)
{
	::DES_set_key(key, &key_des);
	haveKey_des = true;
}

void CEncrypt::set_key_rc5(const unsigned char *data, int nLen, int rounds = RC5_8_ROUNDS)
{
	//Fir::logger->debug("rc5_key:%s", data);
	::RC5_32_set_key(&key_rc5, nLen, data, rounds);
	//Fir::logger->debug("value:%d %d %d %d", key_rc5.data[0], key_rc5[1], key_rc5[2], key_rc5[3]);
	haveKey_rc5 = true;
} 

void CEncrypt::set_key_rc4(const unsigned char *data, int nLen)
{
	::RC4_set_key(&key_rc4_encrypt, nLen, data);
	::RC4_set_key(&key_rc4_decrypt, nLen, data);
	haveKey_rc4 = true;
}

int CEncrypt::encdec(void *data, unsigned int nLen, bool enc)
{
	switch (method)
	{
	case ENCDEC_NONE:
		return -1;
	case ENCDEC_DES:
		return encdec_des((unsigned char *)data, nLen, enc);
	case ENCDEC_RC5:
		return encdec_rc5((unsigned char *)data, nLen, enc);
	case ENCDEC_RC4:
		return encdec_rc4((unsigned char *)data, nLen, enc);
	default:
		break;
	}

	return -2;
}

void CEncrypt::setEncMethod(encMethod m)
{
	method = m;
	enc_mask = dec_mask = 0xffffffff;
}

CEncrypt::encMethod CEncrypt::getEncMethod() const
{
	return method;
}

void CEncrypt::setEncMask(unsigned int m)
{
	enc_mask = m;
}

void CEncrypt::setDecMask(unsigned int m)
{
	dec_mask = m;
}
