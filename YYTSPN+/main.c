#define KEY 0x3A94D63F12345678
#define NUM_OF_ROLLKEY 9
#define WIDTH 1//16*1bits
#define ROTATE_RIGHT(x, s, n) ((x) >> (n)) | ((x) << ((s) - (n)))/*循环右移*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILENAME "5E153B27-EC28-4330-B24E-7D53E6CF583E.pdf"

char Sbox[] =
        {
                0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
                0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
                0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
                0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
                0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
                0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
                0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
                0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
                0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
                0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
                0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
                0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
                0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
                0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
                0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
                0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
        };

struct WORD1
{
    unsigned int s1:4;
    unsigned int s2:4;
    unsigned int s3:4;
    unsigned int s4:4;
    unsigned int s5:4;
    unsigned int s6:4;
    unsigned int s7:4;
    unsigned int s8:4;
};

struct WORD2
{
    unsigned int p1:1;
    unsigned int p2:1;
    unsigned int p3:1;
    unsigned int p4:1;
    unsigned int p5:1;
    unsigned int p6:1;
    unsigned int p7:1;
    unsigned int p8:1;
    unsigned int p9:1;
    unsigned int p10:1;
    unsigned int p11:1;
    unsigned int p12:1;
    unsigned int p13:1;
    unsigned int p14:1;
    unsigned int p15:1;
    unsigned int p16:1;
    unsigned int p17:1;
    unsigned int p18:1;
    unsigned int p19:1;
    unsigned int p20:1;
    unsigned int p21:1;
    unsigned int p22:1;
    unsigned int p23:1;
    unsigned int p24:1;
    unsigned int p25:1;
    unsigned int p26:1;
    unsigned int p27:1;
    unsigned int p28:1;
    unsigned int p29:1;
    unsigned int p30:1;
    unsigned int p31:1;
    unsigned int p32:1;
};

char re[5];

struct WORD2 pi_s(struct WORD1);/*代替密码*/
struct WORD2 pi_p(struct WORD2);/*置换密码*/
unsigned long encode(unsigned long,long long int);
int file_size(char* filename);

int main()
{
    FILE *fp = fopen(FILENAME, "rb");
    FILE *fp1 = fopen("y.dat", "wb");
    if (fp == NULL||fp1==NULL) {
        printf("can not Open the file.");
        exit(-1);
    }

    long long key;
    memset(&key,0,8);
    key = KEY;

    unsigned long x;
    unsigned long y;
    int filesize=file_size(FILENAME);
    int size=0;
    unsigned long iv=0x0;
    while(size<filesize)
    {
        fread(&x, sizeof(unsigned long), WIDTH, fp);
        //	printf("\nx : %x\n",x);
        //	if(feof(fp))
        //  		break;
        //	 printf("p：%x\n",x);
        if(size==0)
            x = x ^ iv;
        else
            x = x ^ y;
//	printf("异或后的p：%x\n",x);
        y = encode(x,key);
//	printf("c：%x\n",y);
        fwrite(&y, sizeof(unsigned long), WIDTH, fp1);
        //  printf("y : %x\n",y);
        size=size+4;
    }
    printf("已经使用SPN加密并保存到y.dat中。");
    fclose(fp);
    fclose(fp1);
    return 0;
}

int file_size(char* filename)
{
    FILE *fp=fopen(filename,"r");
    if(!fp) return -1;
    fseek(fp,0L,SEEK_END);
    int size=ftell(fp);
    fclose(fp);

    return size;
}

unsigned long encode(unsigned long x,long long int key)
{
    unsigned long y;//密文
    unsigned long rollkey[NUM_OF_ROLLKEY];/*生成轮密钥*/
    int i = 0;
    struct WORD2 w[NUM_OF_ROLLKEY-2];
    struct WORD1 u[NUM_OF_ROLLKEY-1];
    struct WORD2 v[NUM_OF_ROLLKEY-1];
    unsigned long temp;
    long tmp;
    int r = 1;

    /*密钥生成方案*/
    for( i = 0; i<NUM_OF_ROLLKEY ; i++)
    {
        rollkey[i] = ROTATE_RIGHT(key, 32, (8-i)*4) &0xff;
        //	printf("key[%d] : %4x\n",i,rollkey[i]);
    }
    rollkey[0]=key>>32;

    memcpy(&w[0], &x ,4);
    for(r = 1; r<=NUM_OF_ROLLKEY-2; r++)
    {
        memcpy(&temp,&w[r-1] ,4);
        temp = temp ^ rollkey[r-1];
        memcpy(&u[r],&temp ,4);
        v[r] = pi_s(u[r]);
        w[r] = pi_p(v[r]);
//		printf("%d : %4x %4x\n",r-1,w[r-1],rollkey[r-1]);
    }
    memcpy(&temp,&w[r-1] ,4);
    temp = temp ^ rollkey[r-1];
    memcpy(&u[r],&temp ,4);
    v[r] = pi_s(u[r]);
    memcpy(&temp,&v[r] ,4);
    y = temp ^ rollkey[NUM_OF_ROLLKEY-1];
    return y;
}

struct WORD2 pi_s(struct WORD1 a)
{

    struct WORD1 b;
    struct WORD2 c;
    unsigned char t=0;

    t=0;
    t=(a.s1)*16+(a.s2);
    t=Sbox[t];
    b.s1=t>>4&0x0f;
    b.s2=t&0x0f;

    t=0;
    t=(a.s3)*16+(a.s4);
    t=Sbox[t];
    b.s3=t>>4&0x0f;
    b.s4=t&0x0f;

    t=0;
    t=(a.s5)*16+(a.s6);
    t=Sbox[t];
    b.s5=t>>4&0x0f;
    b.s6=t&0x0f;

    t=0;
    t=(a.s7)*16+(a.s8);
    t=Sbox[t];
    b.s7=t>>4&0x0f;
    b.s8=t&0x0f;

    memcpy(&c,&b ,4);
    return c;
}

struct WORD2 pi_p(struct WORD2 a)
{
    struct WORD2 b;
    b.p1 = a.p16;
    b.p2 = a.p7;
    b.p3 = a.p20;
    b.p4 = a.p21;
    b.p5 = a.p29;
    b.p6 = a.p12;
    b.p7 = a.p28;
    b.p8 = a.p17;
    b.p9 = a.p1;
    b.p10 = a.p15;
    b.p11 = a.p23;
    b.p12 = a.p26;
    b.p13 = a.p5;
    b.p14 = a.p18;
    b.p15 = a.p31;
    b.p16 = a.p10;
    b.p17 = a.p2;
    b.p18 = a.p8;
    b.p19 = a.p24;
    b.p20 = a.p14;
    b.p21 = a.p32;
    b.p22 = a.p27;
    b.p23 = a.p3;
    b.p24 = a.p9;
    b.p25 = a.p19;
    b.p26 = a.p13;
    b.p27 = a.p30;
    b.p28 = a.p6;
    b.p29 = a.p22;
    b.p30 = a.p11;
    b.p31 = a.p4;
    b.p32 = a.p25;
    return b;
}
