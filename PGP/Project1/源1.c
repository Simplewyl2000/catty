// openssl.cpp: 定义控制台应用程序的入口点。
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
{//一般没有指定长度的参数，默认都是16（AES_BLOCK_SIZE）个字节。输出数据的长度一般都是16字节的倍数，否则会出现数组越界访问。
	int y = len % 16;//y为除以16的余数
	//printf("%d\n", y);
	int x = len / 16;//x为倍数取整
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
	unsigned char in[100];//需要传输的信息
	unsigned char digest[21];//sha1摘要值,20字节，160比特
	unsigned char string[200] = { 0 };//连接后的数据
	unsigned char string2[300] = { 0 };

	unsigned char z[1024] = { 0 };//压缩后的数据
	unsigned char expand[1024] = { 0 };//解压后的数据
	unsigned long slen, zlen, exlen, plen;//原长度，压缩后的长度，解压后的长度,填充后的长度
	size_t n,crv_len;//传输信息的长度
	int ret, nid, size,i;
	unsigned int sig_len;
	COMP_CTX *ctx;
	BIO *berr;
	EC_KEY *key;
	//EC_POINT *pub_key;  //公钥，在ecc中，公钥就是一个曲线上的“点”
	//BIGNUM     *priv_key; //私钥，在ecc中，私钥是一个大数，通过这个大数和“基点”相乘能得到公钥
	EC_GROUP *group;//密钥生成参数
	EC_builtin_curve *curves;
	unsigned char* signature;
	printf("请输入你需要加密传输的信息：");
	scanf("%s", in);
	n = strlen((const char*)in);//输入信息的长度，以字节为单位
	SHA1(in, n, digest);
	digest[20] = '\0';
	printf("信息长度为：%d\n", n);
	printf("经过sha1后的摘要为:");
	for (i = 0; i < 20; i++)
	{
		printf("%x", digest[i]);
	}
	printf("\n");
	key = EC_KEY_new();//构造EC_KEY数据结构
	if (key == NULL)
	{
		printf("EC_KEY_new error!\n");
		return -1;
	}
	//获取实现的椭圆曲线个数
	crv_len = EC_get_builtin_curves(NULL, 0);
	curves = (EC_builtin_curve*)malloc(sizeof(EC_builtin_curve)*crv_len);

	EC_get_builtin_curves(curves, crv_len);//获取椭圆曲线列表
	nid = curves[10].nid;//选取一种椭圆曲线

	group = EC_GROUP_new_by_curve_name(nid);//根据选择的椭圆曲线生成密钥参数group
	if (group == NULL)
	{
		printf("EC_GROUP_new_by_curve_name err!\n");
		return -1;
	}

	ret = EC_KEY_set_group(key, group);// 设置密钥参数
	if (ret != 1)
	{
		printf("EC_KEY_set_group error!\n");
		return -1;
	}

	ret = EC_KEY_generate_key(key);//生成密钥
	if (ret != 1)
	{
		printf("EC_KEY_generate_key error!\n");
		return -1;
	}

	ret = EC_KEY_check_key(key);// 检查密钥
	if (ret != 1)
	{
		printf("check key error!\n");
		return -1;
	}

	// 获取密钥大小 
	size = ECDSA_size(key);
	//printf("size:%d\n", size);
	signature = (unsigned char*)malloc(size);
	ERR_load_crypto_strings(); 
	berr = BIO_new(BIO_s_file());

	// 签名数据， digest 中的数据是 sha1 摘要结果 
	ret = ECDSA_sign(0, digest, 20, signature, &sig_len, key);
	if (ret != 1)
	{
		ERR_print_errors(berr);
		printf("sign error!\n");
		return -1;
	}
	printf("ECC加密后的数据长度为：%d\n", sig_len);
	signature[sig_len] = '\0';
	printf("经过ECC加密后的信息为：");
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

	printf("连接后的信息为：");

	for (i = 0; i < (n+sig_len); i++)
	{
		printf("%x", string[i]);
	}
	printf("\n");
	//slen = strlen((const char*)string);
	slen = n + sig_len;
	printf("原串长度为:%ld\n", slen);



	
		 //zlen = sizeof(z);
		 //exlen = sizeof(expand);
		 

		 printf("字符串预计算长度为:%ld\n", compressBound(slen));
	     //压缩
		    compress(z, &zlen, string, slen);
			//zlen = strlen((const char*)z);
			//z[zlen] = '\0';
			printf("压缩后的信息为：");
			for (i = 0; i < zlen; ++i)
			{
				printf("%x", z[i]);
			}
			printf("\n");

	     printf("压缩后实际长度为:%ld\n", zlen);

		 padding(z,zlen);
		 plen = (zlen / 16 + 1) ;
		 printf("填充后的长度为：%d*16字节\n", plen);
		 printf("补零填充后的信息为：");
		 for (i = 0; i < plen*16; ++i)
		 {
			 printf("%x", z[i]);
		 }
		 //printf("%s", z);
		 printf("\n");


		 char userkey[AES_BLOCK_SIZE+1]="1234567890abcdef";//size为16个字节
		 AES_KEY Akey;
		 unsigned char* encrypt;
		 encrypt = (unsigned char*)malloc(sizeof(unsigned char)*(plen * 16 + 1)); 
		 unsigned char* plain;
		 plain = (unsigned char*)malloc(sizeof(unsigned char)*(plen * 16 + 1));
		 AES_set_encrypt_key((const unsigned char*)userkey, AES_BLOCK_SIZE * 8, &Akey);//设置加密密钥及其长度
		 int Alen=0;
		 printf("AES密钥为：");
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

		 printf("经过AES加密后的信息为：");
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
		 printf("经过RSA公钥加密后的信息为：");
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
		 printf("经过RSA公钥加密后的秘钥信息为：");
		 for (i = 0; i < aeskey_size; ++i)
		 {
			 printf("%x", common_key[i]);
		 }
		 printf("\n");


		 nexus(string2, common_key, aeskey_size);
		 nexus(string2 + aeskey_size, encrypt, plen * 16);
		 printf("连接后的信息为：");

		 for (i = 0; i < (plen*16 + aeskey_size); i++)
		 {
			 printf("%x", string2[i]);
		 }



		 printf("\n\n——————————————————当对方接受到后——————————————————\n\n");




		 ret = RSA_private_decrypt(128, common_key, common_keyd, rsaKey2, RSA_PKCS1_PADDING);
		 //ret = 15
		 printf("经过RSA私钥解密后的对称加密密钥信息为：");
		 for (i = 0; i < aeskey_size; ++i)
		 {
			 printf("%c", common_keyd[i]);
		 }
		 printf("\n");

















/*

		 ret = RSA_private_decrypt(128, aes, aesd, rsaKey, RSA_PKCS1_PADDING);
		 //ret = 15
		 printf("经过RSA私钥解密后的信息为：");
		 for (i = 0; i < plen * 16; ++i)
		 {
			 printf("%x", aesd[i]);
		 }
		 printf("\n");
*/


		 RSA_free(rsaKey2);
	    
		 AES_set_decrypt_key((const unsigned char*)userkey, AES_BLOCK_SIZE * 8, &Akey);//设置解密密钥及其长度
		 Alen = 0;
		 while (Alen < plen)
		 {
			 AES_decrypt(encrypt + Alen*AES_BLOCK_SIZE, plain + Alen*AES_BLOCK_SIZE, &Akey);
			 Alen ++;
		 }
		 plain[plen * 16] = '\0';
		 printf("经过AES解密后的信息为：");
		 for (i = 0; i < plen * 16; ++i)
		 {
			 printf("%x", plain[i]);
		 }
		 printf("\n");


		 unsigned int  pplen= plain[plen*16-1];
		 plain[pplen+1] = '\0';
		 printf("去除填充后的信息为：");
		 for (i = 0; i < pplen; ++i)
		 {
			 printf("%x", plain[i]);
		 }
		 printf("\n");

		 
		//解压缩
		     uncompress(expand, &exlen, plain, pplen);
			// expand[exlen] = '\0';
		     printf("解压出的长度为:%ld\n",exlen);
			 //printf("%s", expand);
			 printf("解压出的信息为:");
			 for (i = 0; i < exlen; ++i)
			 {
				 printf("%x", expand[i]);
			 }
			 printf("\n");
			 
			 unsigned char* last;
			 last = (unsigned char*)malloc(sig_len * sizeof(unsigned char));
			 nexus(last, expand, sig_len);
			 last[sig_len] = '\0';
			 printf("椭圆曲线签名后的摘要为:");
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

			 printf("明文为:");
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
		
		printf("摘要验证成功!\n");
	}
	

    return 0;
}

	