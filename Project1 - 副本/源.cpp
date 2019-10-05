#define _CRT_SECURE_NO_DEPRECATE；
#define _CRT_SECURE_NO_WARNINGS；
#pragma warning(disable:4996)；

#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")

#define RSA_HEAD_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>

void RSA_Parameter(BIGNUM *p, BIGNUM *q, BIGNUM *n, BIGNUM *d, BIGNUM *e);
void RSA_Encrypt(const BIGNUM *Plain, BIGNUM *Cipher, const BIGNUM *n, const BIGNUM *e);
void RSA_Decipher(const BIGNUM *Cipher, BIGNUM *Plain, const BIGNUM *n, const BIGNUM *d);
void RSA_Decipher_CRT(const BIGNUM *Cipher, BIGNUM *Plain, const BIGNUM *p, const BIGNUM *q, const BIGNUM *d);

void ModRepeatSquare(BIGNUM *Result, const BIGNUM *b, const BIGNUM *n, const BIGNUM *m);
void CRT(BIGNUM *x, const BIGNUM *y, const BIGNUM *p, const BIGNUM *q, const BIGNUM *Power);
void Montgomerie(BIGNUM *x, const BIGNUM *y, const BIGNUM *Power, const BIGNUM *Mod);

void RSA_Encrypt_Montg(const BIGNUM *Plain, BIGNUM *Cipher, const BIGNUM *n, const BIGNUM *e);
void RSA_Decipher_Montg(const BIGNUM *Cipher, BIGNUM *Plain, const BIGNUM *n, const BIGNUM *d);


FILE *File_Key_n, *File_Key_e, *File_Key_d, *File_Plain, *File_Cipter;


int main()
{
	BIGNUM *p = BN_new();
	BIGNUM *q = BN_new();
	BIGNUM *n = BN_new();
	BIGNUM *d = BN_new();
	BIGNUM *e = BN_new();

	BIGNUM *Plain = BN_new();
	BIGNUM *Cipher = BN_new();

	clock_t Start;
	clock_t End;

	int j;
	int length;
	char *SPN_Key;

	File_Plain = fopen("SPN_K.txt", "r");
	fseek(File_Plain, 0, SEEK_END);
	length = ftell(File_Plain);
	//printf("Length:%d\n", length);
	SPN_Key = (char *)malloc(length * sizeof(char));
	fseek(File_Plain, 0, SEEK_SET);
	fgets(SPN_Key, length * sizeof(char), File_Plain);
	fclose(File_Plain);

	//printf("%s\n",SPN_Key);

start:
	printf("请选择以下功能（选择其它则退出）\n");
	printf("1.生成RSA算法参数\n");
	printf("2.RSA加密\n");
	printf("3.RSA解密\n");
	printf("4.中国剩余定理\n");
	printf("5.蒙哥马利\n");
	scanf("%d", &j);
	switch (j) {
	case 1:
	{
		Start = clock();

		RSA_Parameter(p, q, n, d, e);

		//test=(unsigned char*)malloc(BN_num_bytes(n)*sizeof(unsigned char));
		//printf("%d\n",BN_num_bytes(n));
		//i=BN_bn2bin(n,test);
		//printf("test:%s\n\n",test);
		//Cipter=fopen("Cipter.txt","w+");
		printf("私钥\n");
		printf("p:\t%s\n\n", BN_bn2hex(p));
		printf("q:\t%s\n\n", BN_bn2hex(q));
		printf("d:\t%s\n\n", BN_bn2hex(d));
		printf("公钥\n");
		printf("e:\t%s\n\n", BN_bn2hex(e));
		printf("n:\t%s\n\n", BN_bn2hex(n));

		End = clock();

		printf("参数生成时间为：%g ms\n", double(End - Start));
	}goto start;
	case 2:
	{

		Start = clock();

		BN_bin2bn((unsigned char *)SPN_Key, strlen(SPN_Key), Plain);	//将字符串转化为大数

		RSA_Encrypt(Plain, Cipher, n, e);
		printf("明文:\n");
		printf("%s\n", BN_bn2hex(Plain));
		printf("密文:\n");
		printf("%s\n", BN_bn2hex(Cipher));

		File_Cipter = fopen("SPN_K_Cipher.txt", "w+");
		fputs(BN_bn2hex(Cipher), File_Cipter);
		fclose(File_Cipter);

		End = clock();
		printf("加密时间为：%g ms\n", double(End - Start));

	}goto start;
	case 3:
	{
		Start = clock();

		RSA_Decipher(Cipher, Plain, n, d);
		BN_bn2bin(Plain, (unsigned char *)SPN_Key);
		printf("明文:\n");
		printf("%s\n", SPN_Key);

		End = clock();
		printf("解密时间为：%g ms\n", double(End - Start));

		File_Plain = fopen("SPN_K_Plain.txt", "w+");
		fputs(SPN_Key, File_Plain);
		fclose(File_Cipter);
	}goto start;
	case 4:
	{
		Start = clock();

		BN_bin2bn((unsigned char *)SPN_Key, strlen(SPN_Key), Plain);

		RSA_Encrypt_Montg(Plain, Cipher, n, e);
		printf("密文:\n");
		printf("%s\n", BN_bn2hex(Cipher));

		End = clock();
		printf("中国剩余定理加密时间为：%g ms\n", double(End - Start));

		Start = clock();

		RSA_Decipher_CRT(Cipher, Plain, p, q, d);
		BN_bn2bin(Plain, (unsigned char *)SPN_Key);
		printf("明文:\n");
		printf("%s\n", SPN_Key);

		End = clock();
		printf("中国剩余定理解密时间为：%g ms\n", double(End - Start));
	}goto start;
	case 5:
	{
		Start = clock();

		BN_bin2bn((unsigned char *)SPN_Key, strlen(SPN_Key), Plain);

		RSA_Encrypt_Montg(Plain, Cipher, n, e);
		printf("密文:\n");
		printf("%s\n", BN_bn2hex(Cipher));

		End = clock();
		printf("蒙哥马利加密时间为：%g ms\n", double(End - Start));

		Start = clock();
		RSA_Decipher_Montg(Cipher, Plain, n, d);
		BN_bn2bin(Plain, (unsigned char *)SPN_Key);
		printf("明文:\n");
		printf("%s\n", SPN_Key);

		End = clock();
		printf("蒙哥马利解密时间为：%g ms\n", double(End - Start));


	}goto start;
	default:
	{
		printf("正在准备退出\n");
		system("Pause");
		return 0;
	}
	goto start;
	}
}


void RSA_Parameter(BIGNUM *p, BIGNUM *q, BIGNUM *n, BIGNUM *d, BIGNUM *e) {

	BIGNUM *exp = BN_new();
	BIGNUM *p_CutOne = BN_new();
	BIGNUM *q_CutOne = BN_new();
	BIGNUM *GCD = BN_new();

	BN_CTX *ctx = BN_CTX_new();

	do
	{
		BN_generate_prime(p, 512, NULL, NULL, NULL, NULL, NULL);//生成一个512位的素数p
	} while (!BN_is_prime(p, NULL, NULL, NULL, NULL));

	do
	{
		BN_generate_prime(q, 512, NULL, NULL, NULL, NULL, NULL);//生成一个512位的素数q
	} while (!BN_is_prime(q, NULL, NULL, NULL, NULL));

	BN_mul(n, p, q, ctx);					//n=p*q
	BN_sub(p_CutOne, p, BN_value_one());    //p0=p-1
	BN_sub(q_CutOne, q, BN_value_one());    //q0=q-1
	BN_mul(exp, p_CutOne, q_CutOne, ctx);   //exp=q0*p0

	do
	{
		BN_rand_range(e, exp);
		BN_gcd(GCD, e, exp, ctx);
	} while (BN_cmp(GCD, BN_value_one()));  //取e

	while (!BN_mod_inverse(d, e, exp, ctx));    //取d

	BN_CTX_free(ctx);
	BN_free(GCD);
	BN_free(q_CutOne);
	BN_free(p_CutOne);
	BN_free(exp);
}

void RSA_Encrypt(const BIGNUM *Plain, BIGNUM *Cipher, const BIGNUM *n, const BIGNUM *e)
{
	ModRepeatSquare(Cipher, Plain, e, n);	//Plain^e % n = Cipher
}

void RSA_Decipher(const BIGNUM *Cipher, BIGNUM *Plain, const BIGNUM *n, const BIGNUM *d)
{
	ModRepeatSquare(Plain, Cipher, d, n);
}

void RSA_Decipher_CRT(const BIGNUM *Cipher, BIGNUM *Plain, const BIGNUM *p, const BIGNUM *q, const BIGNUM *d)
{
	CRT(Plain, Cipher, p, q, d);
}

void RSA_Encrypt_Montg(const BIGNUM *Plain, BIGNUM *Cipher, const BIGNUM *n, const BIGNUM *e)
{
	Montgomerie(Cipher, Plain, e, n);
}

void RSA_Decipher_Montg(const BIGNUM *Cipher, BIGNUM *Plain, const BIGNUM *n, const BIGNUM *d)
{
	Montgomerie(Plain, Cipher, d, n);
}


void ModRepeatSquare(BIGNUM *Result, const BIGNUM *b, const BIGNUM *Pow, const BIGNUM *Mod)
{
	BIGNUM *DivRes = BN_new();
	BIGNUM *Cup_b = BN_new();
	BIGNUM *Cup_Pow = BN_new();
	BIGNUM *a = BN_new();
	BIGNUM *a_new = BN_new();
	BIGNUM *b_sqr = BN_new();
	BIGNUM *Rem = BN_new();
	BIGNUM *Two = BN_new();

	BN_CTX *ctx = BN_CTX_new();

	BN_copy(Cup_b, b);
	BN_copy(Cup_Pow, Pow);

	BN_set_word(a_new, 1);
	BN_set_word(Two, 2);

	do {
		BN_div(DivRes, Rem, Cup_Pow, Two, ctx);
		if (BN_is_one(Rem))  BN_mod_mul(a, a_new, Cup_b, Mod, ctx);
		else BN_copy(a, a_new);

		BN_mod_sqr(b_sqr, Cup_b, Mod, ctx);

		BN_copy(a_new, a);
		BN_copy(Cup_Pow, DivRes);
		BN_copy(Cup_b, b_sqr);

	} while (!BN_is_zero(DivRes));

	BN_copy(Result, a);

	BN_CTX_free(ctx);
	BN_free(Two);
	BN_free(Rem);
	BN_free(b_sqr);
	BN_free(a_new);
	BN_free(a);
	BN_free(Cup_Pow);
	BN_free(Cup_b);
	BN_free(DivRes);
}

void CRT(BIGNUM *x, const BIGNUM *y, const BIGNUM *p, const BIGNUM *q, const BIGNUM *Pow)
{
	BIGNUM *Cp = BN_new();
	BIGNUM *Cq = BN_new();
	BIGNUM *c1 = BN_new();
	BIGNUM *c2 = BN_new();
	BIGNUM *Pow1 = BN_new();
	BIGNUM *Pow2 = BN_new();
	BIGNUM *x11 = BN_new();
	BIGNUM *x12 = BN_new();
	BIGNUM *x21 = BN_new();
	BIGNUM *x22 = BN_new();
	BIGNUM *p_CutOne = BN_new();
	BIGNUM *q_CutOne = BN_new();
	BIGNUM *Mod = BN_new();

	BN_CTX *ctx = BN_CTX_new();

	BN_mul(Mod, p, q, ctx);

	BN_sub(p_CutOne, p, BN_value_one());
	BN_sub(q_CutOne, q, BN_value_one());

	BN_nnmod(Pow1, Pow, p_CutOne, ctx);
	BN_nnmod(Pow2, Pow, q_CutOne, ctx);

	ModRepeatSquare(Cp, y, Pow1, p);
	ModRepeatSquare(Cq, y, Pow2, q);

	BN_mod_inverse(c1, q, p, ctx);
	BN_mod_inverse(c2, p, q, ctx);

	BN_mul(x11, Cp, c1, ctx);
	BN_mul(x12, x11, q, ctx);
	BN_mul(x21, Cq, c2, ctx);
	BN_mul(x22, x21, p, ctx);

	BN_mod_add(x, x12, x22, Mod, ctx);

	BN_CTX_free(ctx);

	BN_free(q_CutOne);
	BN_free(p_CutOne);
	BN_free(x22);
	BN_free(x21);
	BN_free(x12);
	BN_free(x11);
	BN_free(Pow2);
	BN_free(Pow1);
	BN_free(c2);
	BN_free(c1);
	BN_free(Cq);
	BN_free(Cp);

}

void Montgomerie(BIGNUM *Result, const BIGNUM *Base, const BIGNUM *Pow, const BIGNUM *Mod)
{
	BIGNUM *Res = BN_new();
	BIGNUM *Two = BN_new();
	BIGNUM *Zero = BN_new();
	BIGNUM *Cup = BN_new();
	BIGNUM *Pow_backup = BN_new();
	BN_CTX *ctx = BN_CTX_new();

	BN_one(Result);
	BN_set_word(Zero, 0);
	BN_set_word(Two, 2);
	BN_copy(Res, Base);
	BN_copy(Pow_backup, Pow);

	while (BN_cmp(Pow_backup, Zero) == 1)
	{
		BN_mod(Cup, Pow_backup, Two, ctx);

		if (BN_is_zero(Cup)) {
			BN_mod_sqr(Cup, Res, Mod, ctx);
			BN_copy(Res, Cup);

			BN_div(Cup, NULL, Pow_backup, Two, ctx);
			BN_copy(Pow_backup, Cup);

		}
		else {
			BN_mod_mul(Cup, Result, Res, Mod, ctx);
			BN_copy(Result, Cup);

			BN_sub(Cup, Pow_backup, BN_value_one());
			BN_copy(Pow_backup, Cup);
		}
	}

	BN_CTX_free(ctx);
	BN_free(Pow_backup);
	BN_free(Cup);
	BN_free(Zero);
	BN_free(Two);
	BN_free(Res);
}

