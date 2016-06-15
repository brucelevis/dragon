#ifndef _ENCDEC_H
#define _ENCDEC_H

#include "myrc4.h"
#include "myrc5.h"
#include "mydes.h"
#include "mycast.h"
#include "myidea.h"
#include "md5ex.h"


extern void DES_random_key(DES_cblock *ret);
extern void DES_set_key(const_DES_cblock *key, DES_key_schedule *schedule);
extern void DES_encrypt1(DES_LONG *data, DES_key_schedule *ks,t_DES_SPtrans * sp, int enc);
extern void DES_encrypt3(DES_LONG *data, DES_key_schedule *ks1,
			DES_key_schedule *ks2, DES_key_schedule *ks3,t_DES_SPtrans * sp);
extern void DES_decrypt3(DES_LONG *data, DES_key_schedule *ks1,
			DES_key_schedule *ks2, DES_key_schedule *ks3,t_DES_SPtrans * sp);

extern void RC5_32_set_key(RC5_32_KEY *key, int len, const unsigned char *data,
	int rounds);
extern void RC5_32_encrypt(RC5_32_INT *d, RC5_32_KEY *key);
extern void RC5_32_decrypt(RC5_32_INT *d, RC5_32_KEY *key);

extern void idea_set_encrypt_key(const unsigned char *key, IDEA_KEY_SCHEDULE *ks);
extern void idea_set_decrypt_key(IDEA_KEY_SCHEDULE *ek, IDEA_KEY_SCHEDULE *dk);
extern void idea_encrypt(unsigned long *d, IDEA_KEY_SCHEDULE *key);

extern void CAST_set_key(CAST_KEY *key, int len, const unsigned char *data);
extern void CAST_encrypt(CAST_LONG *data, CAST_KEY *key);
extern void CAST_decrypt(CAST_LONG *data, CAST_KEY *key);

extern void RC4_set_key(RC4_KEY *key, int len, const unsigned char *data);
extern void RC4_crypt(unsigned char *data, int len, RC4_KEY* key);

/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
/*-------------------------------------------------------*/
class CEncrypt
{
public:
	CEncrypt();
	enum encMethod
	{
		ENCDEC_NONE,
		ENCDEC_DES,
		ENCDEC_RC5,
		ENCDEC_RC4,
	};
	void random_key_des(DES_cblock *ret);
	void set_key_des(const_DES_cblock *key);
	void set_key_rc5(const unsigned char *data, int nLen, int rounds);
	void set_key_rc4(const unsigned char *data, int nLen);
	int encdec(void *data, unsigned int nLen, bool enc);

	void setEncMethod(encMethod method);
	encMethod getEncMethod() const;

	void setEncMask(unsigned int m);
	void setDecMask(unsigned int m);
private:
	/*
	f_DES_random_key m_fDES_random_key;
	f_DES_set_key m_fDES_set_key;
	f_DES_encrypt1 m_fDES_encrypt1;
	f_DES_encrypt3 m_fDES_encrypt3;
	f_DES_decrypt3 m_fDES_decrypt3;

	f_RC5_32_decrypt m_fRC5_32_decrypt;
	f_RC5_32_encrypt m_fRC5_32_encrypt;
	f_RC5_32_set_key m_fRC5_32_set_key;

	f_idea_set_encrypt_key m_fidea_set_encrypt_key;
	f_idea_set_decrypt_key m_fidea_set_decrypt_key;
	f_idea_encrypt			m_fidea_encrypt;

	f_CAST_set_key			m_fCAST_set_key;
	f_CAST_encrypt			m_fCAST_encrypt;
	f_CAST_decrypt			m_fCAST_decrypt;
	*/

	void DES_random_key(DES_cblock *ret);
	void DES_set_key(const_DES_cblock *key, DES_key_schedule *schedule);
	void DES_encrypt1(DES_LONG *data, DES_key_schedule *ks, int enc);
	void DES_encrypt3(DES_LONG *data, DES_key_schedule *ks1,
				DES_key_schedule *ks2, DES_key_schedule *ks3);
	void DES_decrypt3(DES_LONG *data, DES_key_schedule *ks1,
				DES_key_schedule *ks2, DES_key_schedule *ks3);

	void RC5_32_set_key(RC5_32_KEY *key, int len, const unsigned char *data,
		int rounds);
	void RC5_32_encrypt(RC5_32_INT *d, RC5_32_KEY *key);
	void RC5_32_decrypt(RC5_32_INT *d, RC5_32_KEY *key);

	void idea_set_encrypt_key(const unsigned char *key, IDEA_KEY_SCHEDULE *ks);
	void idea_set_decrypt_key(IDEA_KEY_SCHEDULE *ek, IDEA_KEY_SCHEDULE *dk);
	void idea_encrypt(unsigned long *d, IDEA_KEY_SCHEDULE *key);

	void CAST_set_key(CAST_KEY *key, int len, const unsigned char *data);
	void CAST_encrypt(CAST_LONG *data, CAST_KEY *key);
	void CAST_decrypt(CAST_LONG *data, CAST_KEY *key);

	void RC4_set_key(RC4_KEY *key, int len, const unsigned char *data);
	void RC4_encrypt(unsigned char *data, int len, RC4_KEY* key);
	void RC4_decrypt(unsigned char *data, int len, RC4_KEY* key);

	int encdec_des(unsigned char *data, unsigned int nLen, bool enc);
	int encdec_rc5(unsigned char *data, unsigned int nLen, bool enc);
	int encdec_rc4(unsigned char *data, unsigned int nLen, bool enc);

	DES_key_schedule key_des;
	RC5_32_KEY key_rc5;
	RC4_KEY key_rc4_encrypt;
	RC4_KEY key_rc4_decrypt;
	bool haveKey_des;
	bool haveKey_rc5;
	bool haveKey_rc4;

	encMethod method;

	unsigned int enc_mask;
	unsigned int dec_mask;
};

#endif

