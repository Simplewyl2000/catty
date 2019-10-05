// rainbow.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include "pch.h"
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
#include <openssl/aes.h>
#include <openssl/crypto.h>
#include <time.h>

#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
#define T 100000//���ɲʺ�������Ŀ
typedef struct rainbowlist
{
	struct rainbowlist* pre;
	unsigned char plain[7];//�ʺ������׶�
	unsigned char rainbow[7];//�ʺ�����ĩβ
	struct rainbowlist* next;
}L;


unsigned char compute(unsigned char sum)//����R����ֵ
{
	unsigned int y;
	unsigned char final;
	y = sum % 36;
	if (y >= 0 && y < 10)
	{
		final = y % 10 + 48;
	}
	if (y >= 10 && y < 36)
	{
		final = y % 26 + 97;
	}
	/*if (y >= 36 && y < 62)
	{
		final = y % 26 + 65;
	}*/
	return final;
}



void R(unsigned char* rainbow, unsigned char* hash)//Լ����
{
	unsigned char temp[7];
	int i;
	temp[0] = hash[0] + hash[19] + hash[6] + hash[13];
	temp[1] = hash[1] + hash[18] + hash[7] + hash[12];
	temp[2] = hash[2] + hash[17] + hash[8];
	temp[3] = hash[3] + hash[16] + hash[9];
	temp[4] = hash[4] + hash[15] + hash[10];
	temp[5] = hash[5] + hash[14] + hash[11];
	for (i = 0; i < 6; i++)
	{
		rainbow[i] = compute(temp[i]);
	}
	rainbow[6] = '\0';
	return;
}

unsigned char rand_generate(unsigned int l, unsigned int h)//���������,��ΧΪl-h
{
	unsigned char t;
	t = rand() % (h - l + 1) + l;
	return t;
}

unsigned char rain_generate(void)//���ĵ��ַ��������
{
	unsigned char t;
	unsigned int a, b;
	a = rand_generate(1, 2);
	if (a == 1)
	{
		t = rand_generate(48, 57);
	}
	if (a == 2)
	{
		t = rand_generate(97, 122);
	}
	/*if (a==3)
	{
		t = rand_generate(65, 90);
	}*/
	return t;
}


void node_generate(unsigned char* plain, unsigned char* rainbow)//��㣨������������
{
	unsigned char digest[21];
	size_t n;//������Ϣ�ĳ���
	for (size_t i = 0; i < 6; i++)//�����������
	{
		plain[i] = rain_generate();
	}
	plain[6] = '\0';
	//printf("%s\n", plain);
	n = strlen((const char*)plain);//������Ϣ�ĳ��ȣ����ֽ�Ϊ��λ
	//���ɵĲʺ�������Ϊ3
	SHA1(plain, n, digest);
	digest[20] = '\0';
	//printf("%s\n", digest);
	R(rainbow, digest);
	//printf("%s\n", rainbow);
	SHA1(rainbow, n, digest);
	digest[20] = '\0';
	//printf("%s\n", digest);
	R(rainbow, digest);
	//printf("%s\n", rainbow);
	SHA1(rainbow, n, digest);
	digest[20] = '\0';
	//printf("%s\n", digest);
	R(rainbow, digest);
	//printf("%s\n", rainbow);
	return;
}
void rainbowlist_generate(struct rainbowlist* first)//����Ϊ�ʺ���������
{
	int i;
	first->pre = NULL;
	first->next = NULL;
	struct  rainbowlist* pos = first;
	node_generate(first->plain, first->rainbow);
	for (i = 0; i < T; i++)
	{
		struct  rainbowlist* newprior = (struct rainbowlist*)malloc(sizeof(struct rainbowlist));
		newprior->pre = pos;
		newprior->next = NULL;
		pos->next = newprior;
		node_generate(newprior->plain, newprior->rainbow);
		pos = newprior;
	}
	return;
}

struct rainbowlist* hit(unsigned char* rainbow, struct rainbowlist* first)//�жϲʺ����д����ĩβֵ�Ƿ����
{
	struct rainbowlist* pos = first;
	do
	{
		if (!strcmp((const char*)rainbow, (const char*)pos->rainbow))
		{
			printf("����\n");
			return pos;
		}
		else
		{
			pos = pos->next;
		}
	} while (pos->next != NULL);
	printf("������\n");
	return NULL;
}

void search(unsigned char* hash, struct rainbowlist* first)
{
	unsigned char temp[7];
	unsigned char digest[21];
	struct rainbowlist* pos;
	R(temp, hash);//����Ҫ��ѯ�Ĺ�ϣֵ����Լ��
	pos = hit(temp, first);
	if (pos != NULL)//�ʺ���������
	{
		printf("λ�ڲʺ���������\n");
		SHA1(pos->plain, 6, digest);
		digest[20] = '\0';
		R(temp, digest);
		SHA1(temp, 6, digest);
		digest[20] = '\0';
		R(temp, digest);
		printf("%s\n", temp);
		return;
	}
	SHA1(temp, 6, digest);
	digest[20] = '\0';
	R(temp, digest);
	pos = hit(temp, first);
	if (pos != NULL)//�ʺ����ڶ���
	{
		printf("λ�ڲʺ����ڶ���\n");
		SHA1(pos->plain, 6, digest);
		digest[20] = '\0';
		R(temp, digest);
		printf("%s\n", temp);
		return;
	}
	SHA1(temp, 6, digest);
	digest[20] = '\0';
	R(temp, digest);
	pos = hit(temp, first);
	if (pos != NULL)//�ʺ�����һ��
	{
		printf("λ�ڲʺ�����һ��\n");
		printf("%s\n", pos->plain);
		return;
	}
	printf("Not Find!\n");
	return;
}


void insertCertainRainbow(struct rainbowlist* first) {
	char certainRainbow[7] = "l4zbw7\0";
	unsigned char digest[21];
	SHA1(certainRainbow, 6, digest);
	digest[20] = '\0';
	R(certainRainbow, digest);
	SHA1(certainRainbow, 6, digest);
	digest[20] = '\0';
	R(certainRainbow, digest);


	struct rainbowlist* temp = NULL;
	struct rainbowlist* list = (struct rainbowlist*)malloc(sizeof(struct rainbowlist));

	strcpy(list->plain, "rmi435");
	strcpy(list->rainbow, certainRainbow);

	

//	for (int i = 0; i < 100; i++)
//		temp = temp->next;


	list->next = first->next;
	list->pre =  first;
	first->next->pre = list;
	first->next = list;
	

}




int main()
{
	srand(time(0));
	unsigned char a;
	unsigned char store[20];
	
	struct rainbowlist* first = (struct rainbowlist*)malloc(sizeof(struct rainbowlist));//��ʼ���׽��
	first->pre = NULL;
	first->next = NULL;

	rainbowlist_generate(first);

	unsigned char test[7] = "rmi435";
	unsigned char rainbow[7];
	unsigned char digest[21];
	SHA1(test, 6, digest);
	insertCertainRainbow(first);
	search(digest, first);
	
	return 0;
}


/*
Сд��ĸ��acsll�뷶ΧΪ97-122 26��
��д��ĸ��ascll�뷶ΧΪ65-90 26��
���ֵ�ascll�뷶ΧΪ48-57 10��
��Ƶ�Լ�����ĺ���ֵ�ķ�Χ��Ҫ����������Χ֮��
�����ܵظ������ķ�Χ
20���ֽڷֳ�3*4+4*2ת��Ϊ6���ֽ�
�ʺ�����������Ϊ3
*/