#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Nr 4//轮数
int allround_key[16*(Nr+1)+1];//轮密钥
int s0[]={0,0,0,0};
int s1[]={0,0,0,1};
int s2[]={0,0,1,0};
int s3[]={0,0,1,1};
int s4[]={0,1,0,0};
int s5[]={0,1,0,1};
int s6[]={0,1,1,0};
int s7[]={0,1,1,1};
int s8[]={1,0,0,0};
int s9[]={1,0,0,1};
int s10[]={1,0,1,0};
int s11[]={1,0,1,1};
int s12[]={1,1,0,0};
int s13[]={1,1,0,1};
int s14[]={1,1,1,0};
int s15[]={1,1,1,1};
int key[33]={0,0,1,1,1,0,1,0,1,0,0,1,0,1,0,0,1,1,0,1,0,1,1,0,0,0,1,1,1,1,1,1};

void  paddingTo8(char* string)
{
    int len;
    int gap;
    int i;
    char* temp;
    temp=(char*)malloc(9*sizeof(char));
    len=strlen(string);
    if(len!=8)
    {
        gap=8-len;
        for(i=0;i<gap;i++)
            temp[i]='0';
        temp[gap]='\0';
        strncpy(temp+gap,string,len);
        temp[8]='\0';
        strncpy(string,temp,8);
        string[8]='\0';
    }
    return;
}

int* char2bin(char* textBuffer)
{ int* binary;
    binary=(int*)malloc(16*sizeof(int));
    char string1[9];
    char string2[9];
    char* string;
    string=(char*)malloc(17*sizeof(char));
    int a,b,i;
    a=(unsigned char)textBuffer[0];
    b=(unsigned char)textBuffer[1];
    itoa(a,string1,2);
    paddingTo8(string1);
    itoa(b,string2,2);
    paddingTo8(string2);
    strncpy(string,string1,8);
    strncpy(string+8,string2,8);
    string[16]='\0';
    for(i=0;i<16;i++)
    {
        binary[i]=(unsigned char)string[i]-48;
    }
    return binary;
}

void Xor(int* binary,int round)
{
    int i;
    for(i=0;i<16;i++)
    {
        binary[i]=binary[i]^allround_key[16*round+i];
    }
    return;
}

int isEqual(int *g,int* s)
{
    int i;
    int flag=1;
    for(i=0;i<4;i++)
    {
        if(g[i]!=s[i])
        {
            flag=0;
            break;
        }
    }
    return flag;
}

void int_strncpy(int* g,int* s,int size)
{
    int i;
    for(i=0;i<size;i++)
        g[i]=s[i];
    return;
}
void substitution(int* g)
{
    if(isEqual(g,s0)==1)
    {int_strncpy(g,s14,4);return;}
    if(isEqual(g,s1)==1)
    {int_strncpy(g,s4,4);return;}
    if(isEqual(g,s2)==1)
    {int_strncpy(g,s13,4);return;}
    if(isEqual(g,s3)==1)
    {int_strncpy(g,s1,4);return;}
    if(isEqual(g,s4)==1)
    {int_strncpy(g,s2,4);return;}
    if(isEqual(g,s5)==1)
    {int_strncpy(g,s15,4);return;}
    if(isEqual(g,s6)==1)
    {int_strncpy(g,s11,4);return;}
    if(isEqual(g,s7)==1)
    {int_strncpy(g,s8,4);return;}
    if(isEqual(g,s8)==1)
    {int_strncpy(g,s3,4);return;}
    if(isEqual(g,s9)==1)
    {int_strncpy(g,s10,4);return;}
    if(isEqual(g,s10)==1)
    {int_strncpy(g,s6,4);return;}
    if(isEqual(g,s11)==1)
    {int_strncpy(g,s12,4);return;}
    if(isEqual(g,s12)==1)
    {int_strncpy(g,s5,4);return;}
    if(isEqual(g,s13)==1)
    {int_strncpy(g,s9,4);return;}
    if(isEqual(g,s14)==1)
    {int_strncpy(g,s0,4);return;}
    if(isEqual(g,s15)==1)
    {int_strncpy(g,s7,4);return;}
}
void Sbox(int* binary)
{
    int g1[5];
    int g2[5];
    int g3[5];
    int g4[5];
    int_strncpy(g1,binary,4);
    int_strncpy(g2,binary+4,4);
    int_strncpy(g3,binary+8,4);
    int_strncpy(g4,binary+12,4);
    substitution(g1);
    substitution(g2);
    substitution(g3);
    substitution(g4);
    int_strncpy(binary,g1,4);
    int_strncpy(binary+4,g2,4);
    int_strncpy(binary+8,g3,4);
    int_strncpy(binary+12,g4,4);
    return;
}

void Pbox(int* binary)
{
    int temp[17];
    int_strncpy(temp,binary,16);
    binary[0]=temp[0];
    binary[1]=temp[4];
    binary[2]=temp[8];
    binary[3]=temp[12];
    binary[4]=temp[1];
    binary[5]=temp[5];
    binary[6]=temp[9];
    binary[7]=temp[13];
    binary[8]=temp[2];
    binary[9]=temp[6];
    binary[10]=temp[10];
    binary[11]=temp[14];
    binary[12]=temp[3];
    binary[13]=temp[7];
    binary[14]=temp[11];
    binary[15]=temp[15];
    return;
}

void Roundkey_Generation(int key[])//密钥生成算法
{
    int oneround_key[17];
    int i,j,k;
    for(i=0;i<=Nr;i++)
    {
        for(j=0;j<=15;j++)
        {
            oneround_key[j]=key[4*i+j];
        }
        for(k=16*i;k<16*(i+1);k++)
        {
            allround_key[k]=oneround_key[k%16];
        }
    }
    return;
}

char b_to_c(int* g)//将八位二进制数组转换为对应的字符
{
    int decimal;
    char b;
    decimal=g[0]*128+g[1]*64+g[2]*32+g[3]*16+g[4]*8+g[5]*4+g[6]*2+g[7];//8位二进制转换为十进制
    b=(char)decimal;
    return b;
}
char* bin2char(int* binary)
{   char* string;
    string=(char*)malloc(2*sizeof(char));
    int g1[9];
    int g2[9];
    int_strncpy(g1,binary,8);
    int_strncpy(g2,binary+8,8);
    string[0]=b_to_c(g1);
    string[1]=b_to_c(g2);
    string[2]='\0';
    return string;
}
char* Spn_Encryption(char* textBuffer)//对两个字符进行操作
{ int* binary;
    int i;
    binary=char2bin(textBuffer);//将两个字符转换为16位二进制整数数组
    for(i=0;i<(Nr-1);i++)
    {
        Xor(binary,i);
        Sbox(binary);
        Pbox(binary);
    }
    Xor(binary,Nr-1);
    Sbox(binary);
    Xor(binary,Nr);
    char* string;
    string=(char*)malloc(2*sizeof(char));
    string=bin2char(binary);
    return string;
}
void substitutionReverse(int* g)
{
    if(isEqual(g,s0)==1)
    {int_strncpy(g,s14,4);return;}
    if(isEqual(g,s1)==1)
    {int_strncpy(g,s3,4);return;}
    if(isEqual(g,s2)==1)
    {int_strncpy(g,s4,4);return;}
    if(isEqual(g,s3)==1)
    {int_strncpy(g,s8,4);return;}
    if(isEqual(g,s4)==1)
    {int_strncpy(g,s1,4);return;}
    if(isEqual(g,s5)==1)
    {int_strncpy(g,s12,4);return;}
    if(isEqual(g,s6)==1)
    {int_strncpy(g,s10,4);return;}
    if(isEqual(g,s7)==1)
    {int_strncpy(g,s15,4);return;}
    if(isEqual(g,s8)==1)
    {int_strncpy(g,s7,4);return;}
    if(isEqual(g,s9)==1)
    {int_strncpy(g,s13,4);return;}
    if(isEqual(g,s10)==1)
    {int_strncpy(g,s9,4);return;}
    if(isEqual(g,s11)==1)
    {int_strncpy(g,s6,4);return;}
    if(isEqual(g,s12)==1)
    {int_strncpy(g,s11,4);return;}
    if(isEqual(g,s13)==1)
    {int_strncpy(g,s2,4);return;}
    if(isEqual(g,s14)==1)
    {int_strncpy(g,s0,4);return;}
    if(isEqual(g,s15)==1)
    {int_strncpy(g,s5,4);return;}
}

void Sbox_Reserve(int* binary)
{
    int g1[5];
    int g2[5];
    int g3[5];
    int g4[5];
    int_strncpy(g1,binary,4);
    int_strncpy(g2,binary+4,4);
    int_strncpy(g3,binary+8,4);
    int_strncpy(g4,binary+12,4);
    substitutionReverse(g1);
    substitutionReverse(g2);
    substitutionReverse(g3);
    substitutionReverse(g4);
    int_strncpy(binary,g1,4);
    int_strncpy(binary+4,g2,4);
    int_strncpy(binary+8,g3,4);
    int_strncpy(binary+12,g4,4);
    return;
}

void Pbox_Reserve(int* binary)
{
    int temp[17];
    int_strncpy(temp,binary,16);
    binary[0]=temp[0];
    binary[1]=temp[4];
    binary[2]=temp[8];
    binary[3]=temp[12];
    binary[4]=temp[1];
    binary[5]=temp[5];
    binary[6]=temp[9];
    binary[7]=temp[13];
    binary[8]=temp[2];
    binary[9]=temp[6];
    binary[10]=temp[10];
    binary[11]=temp[14];
    binary[12]=temp[3];
    binary[13]=temp[7];
    binary[14]=temp[11];
    binary[15]=temp[15];
    return;
}

char* Spn_Decryption(char* cipher_text)//对两个字符进行操作
{ int* binary;
    int i;
    binary=char2bin(cipher_text);//将两个字符转换为16位二进制整数数组
    Xor(binary,Nr);
    Sbox_Reserve(binary);
    Xor(binary,Nr-1);
    for(i=Nr-2;i>=0;i--)
    {
        Pbox_Reserve(binary);
        Sbox_Reserve(binary);
        Xor(binary,i);
    }
    char* string;
    string=(char*)malloc(2*sizeof(char));
    string=bin2char(binary);
    return string;
}



char* encrypt(char* input){
    int len;
    int groups;
    char textBuffer[3];//暂存每次SPN需要的两个字节
    char cipher_text[3];//暂存每次SPN输出的两个字节
    char*output = (char*)malloc(100* sizeof(char));//密文
    len=strlen(input);
    if(len%2!=0)
    { char padding[2]={0, 0};
        strcat(input,padding);
        len++;
    }
    groups=len/2;
    Roundkey_Generation(key);
    for(int i=0;i<groups;i++)
    {
        strncpy(textBuffer,input+2*i,2);
        textBuffer[2]='\0';
        strncpy(cipher_text,Spn_Encryption(textBuffer),2);
        cipher_text[2]='\0';
        strncpy(output+2*i,cipher_text,2);
    }
    return output;

}

char* decryptcip(char* input){
    int len;
    int groups;
    char textBuffer[3];//暂存每次SPN需要的两个字节
    char cipher_text[3];//暂存每次SPN输出的两个字节
    char*output = (char*)malloc(100* sizeof(char));//密文
    len=strlen(input);
    if(len%2!=0)
    { char padding[2]={0, 0};
        strcat(input,padding);
        len++;
    }
    groups=len/2;
    Roundkey_Generation(key);
    for(int i=0;i<groups;i++)
    {
        strncpy(textBuffer,input+2*i,2);
        textBuffer[2]='\0';
        strncpy(cipher_text,Spn_Decryption(textBuffer),2);
        cipher_text[2]='\0';
        strncpy(output+2*i,cipher_text,2);
    }
    return output;



}


char * enandde(){


}





int main()
{   char input[100];//明文
    char* encryption;
    char* decryption;


    int choice;
    printf("Choose to encrypt or decrypt:\n");
    printf("1.Encrypt\t2.Decrypt\t3.Encrypt and Decrypt\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            printf("Input the plaintext:\n");
            scanf("%s",input);
            encryption = encrypt(input);
            printf("%s\n",encryption);
            break;

        case 2:
            printf("Input the ciphertext:\n");
            scanf("%s",input);
            decryption = decryptcip(input);
            printf("%s\n",decryption);
            break;

        case 3:
            printf("Input the plaintext:\n");
            scanf("%s",input);
            encryption = encrypt(input);
            printf("%s \n",encryption);
            decryption = decryptcip(encryption);
            printf("%s \n",decryption);


    }

    return 0;
}
