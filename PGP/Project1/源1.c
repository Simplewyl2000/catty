// openssl.cpp: �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<openssl/ssl.h>
#include<openssl/ec.h>
#include<openssl/ecdsa.h>
#include<openssl/objects.h>
#include<openssl/err.h>
#include<openssl/sha.h>
#include<openssl/comp.h>
#include<openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/crypto.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <memory.h>
#include "zlib.h"

#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"zlibwapi.lib")
void nexus(unsigned char* string, unsigned char* source, unsigned int n)
{
	unsigned int i;
	for (i = 0; i < n; i++)
	{
		string[i] = source[i];
	}
	return;
}

void padding(unsigned char* z, int len)
{//һ��û��ָ�����ȵĲ�����Ĭ�϶���16��AES_BLOCK_SIZE�����ֽڡ�������ݵĳ���һ�㶼��16�ֽڵı�����������������Խ����ʡ�
	int y = len % 16;//yΪ����16������
	//printf("%d\n", y);
	int x = len / 16;//xΪ����ȡ��
	//printf("%d\n", x);
	unsigned char* temp;
	int i;
	int difference;
	difference = 16 - y;
	//printf("%d\n", difference);
	if (y != 0)
	{
		temp = (unsigned char*)malloc(sizeof(unsigned char)*(difference + 1));
		for (i = 0; i < (difference - 1); i++)
		{
			temp[i] = '0';
		}
		temp[difference - 1] = len;
		temp[difference] = '\0';
		nexus(z+len, temp, difference);
		/*strncpy(temp + difference, z, len);
		temp[difference+len] = '\0';
		strncpy(z, temp, 16*(x+1));*/
		z[16 * (x + 1)] = '\0';
		//printf("%s\n",z);

	}
	return;
}
int len_compute(unsigned char* a)
{
	int i = 0;
	while (a[i]!='\0')
	{
		i++;
	}
	return i;
}

int intercept(unsigned char* z)
{
	int len;
	unsigned int l;
	len = len_compute(z);
	printf("%d\n", len);
	l = (unsigned char)z[len - 1];
	printf("%d\n", l);
	z[l] = '\0';
	return l;
}

int main()
{
	unsigned char in[100];//��Ҫ�������Ϣ
	unsigned char digest[21];//sha1ժҪֵ,20�ֽڣ�160����
	unsigned char string[200] = { 0 };//���Ӻ������
	unsigned char string2[300] = { 0 };

	unsigned char z[1024] = { 0 };//ѹ���������
	unsigned char expand[1024] = { 0 };//��ѹ�������
	unsigned long slen, zlen, exlen, plen;//ԭ���ȣ�ѹ����ĳ��ȣ���ѹ��ĳ���,����ĳ���
	size_t n,crv_len;//������Ϣ�ĳ���
	int ret, nid, size,i;
	unsigned int sig_len;
	COMP_CTX *ctx;
	BIO *berr;
	EC_KEY *key;
	//EC_POINT *pub_key;  //��Կ����ecc�У���Կ����һ�������ϵġ��㡱
	//BIGNUM     *priv_key; //˽Կ����ecc�У�˽Կ��һ��������ͨ����������͡����㡱����ܵõ���Կ
	EC_GROUP *group;//��Կ���ɲ���
	EC_builtin_curve *curves;
	unsigned char* signature;
	printf("����������Ҫ���ܴ������Ϣ��");
	scanf("%s", in);
	n = strlen((const char*)in);//������Ϣ�ĳ��ȣ����ֽ�Ϊ��λ
	SHA1(in, n, digest);
	digest[20] = '\0';
	printf("��Ϣ����Ϊ��%d\n", n);
	printf("����sha1���ժҪΪ:");
	for (i = 0; i < 20; i++)
	{
		printf("%x", digest[i]);
	}
	printf("\n");
	key = EC_KEY_new();//����EC_KEY���ݽṹ
	if (key == NULL)
	{
		printf("EC_KEY_new error!\n");
		return -1;
	}
	//��ȡʵ�ֵ���Բ���߸���
	crv_len = EC_get_builtin_curves(NULL, 0);
	curves = (EC_builtin_curve*)malloc(sizeof(EC_builtin_curve)*crv_len);

	EC_get_builtin_curves(curves, crv_len);//��ȡ��Բ�����б�
	nid = curves[10].nid;//ѡȡһ����Բ����

	group = EC_GROUP_new_by_curve_name(nid);//����ѡ�����Բ����������Կ����group
	if (group == NULL)
	{
		printf("EC_GROUP_new_by_curve_name err!\n");
		return -1;
	}

	ret = EC_KEY_set_group(key, group);// ������Կ����
	if (ret != 1)
	{
		printf("EC_KEY_set_group error!\n");
		return -1;
	}

	ret = EC_KEY_generate_key(key);//������Կ
	if (ret != 1)
	{
		printf("EC_KEY_generate_key error!\n");
		return -1;
	}

	ret = EC_KEY_check_key(key);// �����Կ
	if (ret != 1)
	{
		printf("check key error!\n");
		return -1;
	}

	// ��ȡ��Կ��С 
	size = ECDSA_size(key);
	//printf("size:%d\n", size);
	signature = (unsigned char*)malloc(size);
	ERR_load_crypto_strings(); 
	berr = BIO_new(BIO_s_file());

	// ǩ�����ݣ� digest �е������� sha1 ժҪ��� 
	ret = ECDSA_sign(0, digest, 20, signature, &sig_len, key);
	if (ret != 1)
	{
		ERR_print_errors(berr);
		printf("sign error!\n");
		return -1;
	}
	printf("ECC���ܺ�����ݳ���Ϊ��%d\n", sig_len);
	signature[sig_len] = '\0';
	printf("����ECC���ܺ����ϢΪ��");
	for (i = 0; i < sig_len; i++)
	{
		printf("%x", signature[i]);
	}
	printf("\n");
	//string=(unsigned char*)malloc((n+sig_len+1)*sizeof(unsigned char));
	nexus(string+sig_len, (unsigned char*)in, n);
	/*for (i = 0; i <n; i++)
	{
		printf("%x", string[i]);
	}
	printf("\n");*/
	//string[n] = '\0';
	//strcat((char*)string, (const char*)signature);
	nexus(string, (unsigned char*)signature, sig_len);
	string[n + sig_len] = '\0';

	printf("���Ӻ����ϢΪ��");

	for (i = 0; i < (n+sig_len); i++)
	{
		printf("%x", string[i]);
	}
	printf("\n");
	//slen = strlen((const char*)string);
	slen = n + sig_len;
	printf("ԭ������Ϊ:%ld\n", slen);



	
		 //zlen = sizeof(z);
		 //exlen = sizeof(expand);
		 

		 printf("�ַ���Ԥ���㳤��Ϊ:%ld\n", compressBound(slen));
	     //ѹ��
		    compress(z, &zlen, string, slen);
			//zlen = strlen((const char*)z);
			//z[zlen] = '\0';
			printf("ѹ�������ϢΪ��");
			for (i = 0; i < zlen; ++i)
			{
				printf("%x", z[i]);
			}
			printf("\n");

	     printf("ѹ����ʵ�ʳ���Ϊ:%ld\n", zlen);

		 padding(z,zlen);
		 plen = (zlen / 16 + 1) ;
		 printf("����ĳ���Ϊ��%d*16�ֽ�\n", plen);
		 printf("�����������ϢΪ��");
		 for (i = 0; i < plen*16; ++i)
		 {
			 printf("%x", z[i]);
		 }
		 //printf("%s", z);
		 printf("\n");


		 char userkey[AES_BLOCK_SIZE+1]="1234567890abcdef";//sizeΪ16���ֽ�
		 AES_KEY Akey;
		 unsigned char* encrypt;
		 encrypt = (unsigned char*)malloc(sizeof(unsigned char)*(plen * 16 + 1)); 
		 unsigned char* plain;
		 plain = (unsigned char*)malloc(sizeof(unsigned char)*(plen * 16 + 1));
		 AES_set_encrypt_key((const unsigned char*)userkey, AES_BLOCK_SIZE * 8, &Akey);//���ü�����Կ���䳤��
		 int Alen=0;
		 printf("AES��ԿΪ��");
		 for (i = 0; i < 16; ++i)
		 {
			 printf("%c", userkey[i]);
		 }
		 printf("\n");

		 while (Alen<plen)
		 {
			 AES_encrypt(z + Alen* AES_BLOCK_SIZE, encrypt + Alen* AES_BLOCK_SIZE, &Akey);
			 Alen ++;
		 }
		 encrypt[plen * 16] = '\0';

		 printf("����AES���ܺ����ϢΪ��");
		 for (i = 0; i < plen * 16; ++i)
		 {
			 printf("%x", encrypt[i]);
		 }
		 printf("\n");



/*
		 RSA *rsaKey = RSA_generate_key(1024, 65537, NULL, NULL);

		 int keySize = RSA_size(rsaKey);

		 unsigned char aes[128];
		 unsigned char aesd[128];

		 ret = RSA_public_encrypt(plen * 16, encrypt, aes, rsaKey, RSA_PKCS1_PADDING);
		 //ret = 128
		 printf("����RSA��Կ���ܺ����ϢΪ��");
		 for (i = 0; i < plen * 16; ++i)
		 {
			 printf("%x", aes[i]);                                     
		 }
		 printf("\n");
		 
*/

		 RSA* rsaKey2 = RSA_generate_key(1024, 65537, NULL, NULL);
		 unsigned char common_key[128] = {0};
		 unsigned char common_keyd[128];
		 int aeskey_size = 0;
		 aeskey_size = strlen(userkey);
		 RSA_public_encrypt(aeskey_size, userkey, common_key, rsaKey2, RSA_PKCS1_PADDING);
		 printf("����RSA��Կ���ܺ����Կ��ϢΪ��");
		 for (i = 0; i < aeskey_size; ++i)
		 {
			 printf("%x", common_key[i]);
		 }
		 printf("\n");


		 nexus(string2, common_key, aeskey_size);
		 nexus(string2 + aeskey_size, encrypt, plen * 16);
		 printf("���Ӻ����ϢΪ��");

		 for (i = 0; i < (plen*16 + aeskey_size); i++)
		 {
			 printf("%x", string2[i]);
		 }



		 printf("\n\n���������������������������������������Է����ܵ��󡪡���������������������������������\n\n");




		 ret = RSA_private_decrypt(128, common_key, common_keyd, rsaKey2, RSA_PKCS1_PADDING);
		 //ret = 15
		 printf("����RSA˽Կ���ܺ�ĶԳƼ�����Կ��ϢΪ��");
		 for (i = 0; i < aeskey_size; ++i)
		 {
			 printf("%c", common_keyd[i]);
		 }
		 printf("\n");

















/*

		 ret = RSA_private_decrypt(128, aes, aesd, rsaKey, RSA_PKCS1_PADDING);
		 //ret = 15
		 printf("����RSA˽Կ���ܺ����ϢΪ��");
		 for (i = 0; i < plen * 16; ++i)
		 {
			 printf("%x", aesd[i]);
		 }
		 printf("\n");
*/


		 RSA_free(rsaKey2);
	    
		 AES_set_decrypt_key((const unsigned char*)userkey, AES_BLOCK_SIZE * 8, &Akey);//���ý�����Կ���䳤��
		 Alen = 0;
		 while (Alen < plen)
		 {
			 AES_decrypt(encrypt + Alen*AES_BLOCK_SIZE, plain + Alen*AES_BLOCK_SIZE, &Akey);
			 Alen ++;
		 }
		 plain[plen * 16] = '\0';
		 printf("����AES���ܺ����ϢΪ��");
		 for (i = 0; i < plen * 16; ++i)
		 {
			 printf("%x", plain[i]);
		 }
		 printf("\n");


		 unsigned int  pplen= plain[plen*16-1];
		 plain[pplen+1] = '\0';
		 printf("ȥ���������ϢΪ��");
		 for (i = 0; i < pplen; ++i)
		 {
			 printf("%x", plain[i]);
		 }
		 printf("\n");

		 
		//��ѹ��
		     uncompress(expand, &exlen, plain, pplen);
			// expand[exlen] = '\0';
		     printf("��ѹ���ĳ���Ϊ:%ld\n",exlen);
			 //printf("%s", expand);
			 printf("��ѹ������ϢΪ:");
			 for (i = 0; i < exlen; ++i)
			 {
				 printf("%x", expand[i]);
			 }
			 printf("\n");
			 
			 unsigned char* last;
			 last = (unsigned char*)malloc(sig_len * sizeof(unsigned char));
			 nexus(last, expand, sig_len);
			 last[sig_len] = '\0';
			 printf("��Բ����ǩ�����ժҪΪ:");
			 for (i = 0; i < sig_len ;++i)
			 {
				 printf("%x", last[i]);
			 }
			 printf("\n");

			 unsigned char* get;
			 int glen;
			 glen = exlen - sig_len;
			 get = (unsigned char*)malloc(sizeof(unsigned char)*(glen + 1));
			 strncpy((char*)get,(const char*)expand + sig_len, glen);
			 //nexus(get, expand + sig_len, glen);
			 get[glen] = '\0';

			 printf("����Ϊ:");
			 for (i = 0; i < glen; ++i)
			 {
				 printf("%c", get[i]);
			 }
			 printf("\n");

			 unsigned char dig[21];
			 SHA1(get, glen, dig);
			 dig[20] = '\0';
			 
	ret = ECDSA_verify(0, dig,20,last, sig_len, key);
	if (ret == 1)
	{
		
		printf("ժҪ��֤�ɹ�!\n");
	}
	

    return 0;
}

	