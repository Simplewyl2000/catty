#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Nr 16//轮数
int allround_key[128*(Nr+1)+1]={0};//轮密钥
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
int AES[256]={
        99,124,119,123,242,107,111,197,48,1,103,43,254,215,171,118,
        202,130,201,125,250,89,71,240,173,212,162,175,156,164,114,192,
        183,253,147,38,54,63,247,204,52,165,229,241,113,216,49,21,
        4,199,35,195,24,150,5,154,7,18,128,226,235,39,178,117,
        9,131,44,26,27,110,90,160,82,59,214,179,41,227,47,132,
        83,209,0,237,32,252,177,91,106,203,190,57,74,76,88,207,
        208,239,170,251,67,77,51,133,69,249,2,127,80,60,159,168,
        81,163,64,143,146,157,56,245,188,182,218,33,16,255,243,210,
        205,12,19,236,95,151,68,23,196,167,126,61,100,93,25,115,
        96,129,79,220,34,42,144,136,70,238,184,20,222,94,11,219,
        224,50,58,10,73,6,36,92,194,211,172,98,145,149,228,121,
        231,200,55,109,141,213,78,169,108,86,244,234,101,122,174,8,
        186,120,37,46,28,166,180,198,232,221,116,31,75,189,139,138,
        112,62,181,102,72,3,246,14,97,53,87,185,134,193,29,158,
        225,248,152,17,105,217,142,148,155,30,135,233,206,85,40,223,
        140,161,137,13,191,230,66,104,65,153,45,15,176,84,187,22};
int AES_Reserve[256]={
        82,9,106,213,48,54,165,56,191,64,163,158,129,243,215,251,
        124,227,57,130,155,47,255,135,52,142,67,68,196,222,233,203,
        84,123,148,50,166,194,35,61,238,76,149,11,66,250,195,78,
        8,46,161,102,40,217,36,178,118,91,162,73,109,139,209,37,
        114,248,246,100,134,104,152,22,212,164,92,204,93,101,182,146,
        108,112,72,80,253,237,185,218,94,21,70,87,167,141,157,132,
        144,216,171,0,140,188,211,10,247,228,88,5,184,179,69,6,
        208,44,30,143,202,63,15,2,193,175,189,3,1,19,138,107,
        58,145,17,65,79,103,220,234,151,242,207,206,240,180,230,115,
        150,172,116,34,231,173,53,133,226,249,55,232,28,117,223,110,
        71,241,26,113,29,41,197,137,111,183,98,14,170,24,190,27,
        252,86,62,75,198,210,121,32,154,219,192,254,120,205,90,244,
        31,221,168,51,136,7,199,49,177,18,16,89,39,128,236,95,
        96,81,127,169,25,181,74,13,45,229,122,159,147,201,156,239,
        160,24,59,77,174,42,245,176,200,235,187,60,131,83,153,97,
        23,43,4,126,186,119,214,38,225,105,20,99,85,33,12,125};
int padding(char* z,int len)
{//一般没有指定长度的参数，默认都是16（AES_BLOCK_SIZE）个字节。输出数据的长度一般都是16字节的倍数，否则会出现数组越界访问。
    int y = len % 16;//y为除以16的余数
    //printf("%d\n",y);
    int x = len / 16;//x为倍数取整
    //printf("%d\n",x);
    char* temp;
    int i;
    int gap;
    int result;
    gap = 16 - y;
    //printf("%d\n",gap);
    if (y!=0)
    {
        temp = (char*)malloc(sizeof(char)*(gap + 1));
        for (i = 0; i < gap; i++)
        {
            temp[i] = '0';
        }
        temp[gap] = '\0';
        strcat(z, temp);
        z[16*(x+1)] = '\0';

    }
    result=16*(x+1);
    return result;
}
void  Eight_Bit_Padding(char* string)//将二进制字符串填充到八位
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

void int_strncpy(int* g,int* s,int size)
{
    int i;
    for(i=0;i<size;i++)
        g[i]=s[i];
    return;
}

int* Character_to_Binary(char* clear_text)//2个字节的明文
{ int* binary;
    binary=(int*)malloc(17*sizeof(int));
    char string1[9];
    char string2[9];
    char* string;
    string=(char*)malloc(17*sizeof(char));
    int a,b,i;
    a=(unsigned char)clear_text[0];//得到十进制ascll
    a=AES[a];
    b=(unsigned char)clear_text[1];
    b=AES[b];
    itoa(a,string1,2);
    Eight_Bit_Padding(string1);
    itoa(b,string2,2);
    Eight_Bit_Padding(string2);
    strncpy(string,string1,8);
    strncpy(string+8,string2,8);
    string[16]='\0';
    for(i=0;i<16;i++)
    {
        binary[i]=(unsigned char)string[i]-48;
    }
    return binary;
}

int* Character_to_Binary_strengthen(char* clear_text)//16字节的明文
{
    int * binary;
    int i;
    int * temp;
    binary=(int*)malloc(129*sizeof(int));
    for(i=0;i<8;i++)
    {
        temp=Character_to_Binary(clear_text+2*i);
        int_strncpy(binary+16*i,temp,16);
    }
    binary[128]='\0';
    return binary;
}

void Xor(int* binary,int round)//128位异或
{
    int i;
    for(i=0;i<128;i++)
    {
        binary[i]=binary[i]^allround_key[128*round+i];
    }
    return;
}


void Xorcbc(int* binary,int* cbc)//128位异或
{
    int i;
    for(i=0;i<128;i++)
    {
        binary[i]=binary[i]^cbc[i];
    }
    return;
}

int isEqual(int *g,int* s)//比较四个字节
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

void sub(int* g)//增强S盒，避免循环移位造成线性
{
    if(isEqual(g,s0)==1)
    {int_strncpy(g,s1,4);return;}
    if(isEqual(g,s1)==1)
    {int_strncpy(g,s6,4);return;}
    if(isEqual(g,s2)==1)
    {int_strncpy(g,s15,4);return;}
    if(isEqual(g,s3)==1)
    {int_strncpy(g,s8,4);return;}
    if(isEqual(g,s4)==1)
    {int_strncpy(g,s5,4);return;}
    if(isEqual(g,s5)==1)
    {int_strncpy(g,s7,4);return;}
    if(isEqual(g,s6)==1)
    {int_strncpy(g,s2,4);return;}
    if(isEqual(g,s7)==1)
    {int_strncpy(g,s10,4);return;}
    if(isEqual(g,s8)==1)
    {int_strncpy(g,s13,4);return;}
    if(isEqual(g,s9)==1)
    {int_strncpy(g,s12,4);return;}
    if(isEqual(g,s10)==1)
    {int_strncpy(g,s0,4);return;}
    if(isEqual(g,s11)==1)
    {int_strncpy(g,s4,4);return;}
    if(isEqual(g,s12)==1)
    {int_strncpy(g,s14,4);return;}
    if(isEqual(g,s13)==1)
    {int_strncpy(g,s3,4);return;}
    if(isEqual(g,s14)==1)
    {int_strncpy(g,s11,4);return;}
    if(isEqual(g,s15)==1)
    {int_strncpy(g,s9,4);return;}
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
    sub(g1);
    sub(g2);
    sub(g3);
    sub(g4);
    int_strncpy(binary,g1,4);
    int_strncpy(binary+4,g2,4);
    int_strncpy(binary+8,g3,4);
    int_strncpy(binary+12,g4,4);
    return;
}

void Sbox_strengthen(int *binary)
{
    int i;
    for(i=0;i<8;i++)
        Sbox(binary+16*i);
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

void Pbox_strengthen(int* binary)
{
    int i;
    for(i=0;i<8;i++)
        Pbox(binary+16*i);
    return;
}


char b_to_c(int* g)//将二进制数组转换为对应的字符
{
    int decimal;
    char b;
    decimal=g[0]*128+g[1]*64+g[2]*32+g[3]*16+g[4]*8+g[5]*4+g[6]*2+g[7];//8位二进制转换为十进制
    decimal=AES_Reserve[decimal];
    b=(char)decimal;
    return b;
}

void Binary_to_Character(int* binary,char *string)
{
    int g1[9];
    int g2[9];
    int_strncpy(g1,binary,8);
    int_strncpy(g2,binary+8,8);
    string[0]=b_to_c(g1);
    string[1]=b_to_c(g2);
    return;
}

char* Binary_to_Character_strengthen(int* binary)//
{   char* string;
    string=(char*)malloc(17*sizeof(char));
    int i;
    for(i=0;i<8;i++)
    {
        Binary_to_Character(binary+16*i,string+2*i);
    }
    string[16]='\0';
    return string;
}


void AES_strengthen(int *binary)
{
    char* string;
    string=(char*)malloc(17*sizeof(char));
    string=Binary_to_Character_strengthen(binary);
    string[16]='\0';
    unsigned int temp;
    int i;
    for(i=0;i<16;i++)
    {
        temp=(unsigned char)string[i];
        temp=AES[temp];
        string[i]=(char)temp;
    }
    binary=Character_to_Binary_strengthen(string);
    return;
}
void AES_strengthen_Reserve(int *binary)
{
    char* string;
    string=(char*)malloc(17*sizeof(char));
    string=Binary_to_Character_strengthen(binary);
    string[16]='\0';
    unsigned int temp;
    int i;
    for(i=0;i<16;i++)
    {
        temp=(unsigned char)string[i];
        temp=AES_Reserve[temp];
        string[i]=(char)temp;
    }
    binary=Character_to_Binary_strengthen(string);
    return;
}
char* Spn_Encryption_Strengthen(char* clear_text,int* lcbc)//对16个字节进行操作
{   int* binary;
    int i;
    binary=Character_to_Binary_strengthen(clear_text);//将16个字节转换为128位二进制整数数组
    Xorcbc(binary,lcbc);
    for(i=0;i<(Nr-1);i++)
    {
        AES_strengthen(binary);
        Xor(binary,i);
        Sbox_strengthen(binary);
        Pbox_strengthen(binary);

    }
    AES_strengthen(binary);
    Xor(binary,Nr-1);
    Sbox_strengthen(binary);
    Xor(binary,Nr);
    int_strncpy(lcbc,binary,128);
    char* string;
    string=Binary_to_Character_strengthen(binary);
    return string;
}

void sub_reserve(int* g)
{
    if(isEqual(g,s0)==1)
    {int_strncpy(g,s10,4);return;}
    if(isEqual(g,s1)==1)
    {int_strncpy(g,s0,4);return;}
    if(isEqual(g,s2)==1)
    {int_strncpy(g,s6,4);return;}
    if(isEqual(g,s3)==1)
    {int_strncpy(g,s13,4);return;}
    if(isEqual(g,s4)==1)
    {int_strncpy(g,s11,4);return;}
    if(isEqual(g,s5)==1)
    {int_strncpy(g,s4,4);return;}
    if(isEqual(g,s6)==1)
    {int_strncpy(g,s1,4);return;}
    if(isEqual(g,s7)==1)
    {int_strncpy(g,s5,4);return;}
    if(isEqual(g,s8)==1)
    {int_strncpy(g,s3,4);return;}
    if(isEqual(g,s9)==1)
    {int_strncpy(g,s15,4);return;}
    if(isEqual(g,s10)==1)
    {int_strncpy(g,s7,4);return;}
    if(isEqual(g,s11)==1)
    {int_strncpy(g,s14,4);return;}
    if(isEqual(g,s12)==1)
    {int_strncpy(g,s9,4);return;}
    if(isEqual(g,s13)==1)
    {int_strncpy(g,s8,4);return;}
    if(isEqual(g,s14)==1)
    {int_strncpy(g,s12,4);return;}
    if(isEqual(g,s15)==1)
    {int_strncpy(g,s2,4);return;}
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
    sub_reserve(g1);
    sub_reserve(g2);
    sub_reserve(g3);
    sub_reserve(g4);
    int_strncpy(binary,g1,4);
    int_strncpy(binary+4,g2,4);
    int_strncpy(binary+8,g3,4);
    int_strncpy(binary+12,g4,4);
    return;
}

void Sbox_strengthen_Reserve(int *binary)
{
    int i;
    for(i=0;i<8;i++)
        Sbox_Reserve(binary+16*i);
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

void Pbox_strengthen_Reserve(int* binary)
{
    int i;
    for(i=0;i<8;i++)
        Pbox_Reserve(binary+16*i);
    return;
}



char* Spn_Decryption_strengthen(char* cipher_text,int* lcbc,int* ncbc)//对两个字符进行操作
{ int* binary;
    int i;
    binary=Character_to_Binary_strengthen(cipher_text);//将两个字符转换为16位二进制整数数组

    int_strncpy(ncbc,binary,128);
    Xor(binary,Nr);
    Sbox_strengthen_Reserve(binary);
    Xor(binary,Nr-1);
    AES_strengthen(binary);
    for(i=Nr-2;i>=0;i--)
    {
        Pbox_strengthen_Reserve(binary);
        Sbox_strengthen_Reserve(binary);
        Xor(binary,i);
        AES_strengthen_Reserve(binary);
    }
    Xorcbc(binary,lcbc);
    int_strncpy(lcbc,ncbc,128);
    char* string;
    string=Binary_to_Character_strengthen(binary);
    return string;
}
void key_generation(void)
{   char source[16*(Nr+1)+1]="在系统学习红树林相关理论知识后，我们来到山口红树林自然保护区参观，并采访了保护区的护林员莫积瑞。这次近距离接触，让我们在红树林研究中心所学到的理论知识变成实际。死板的图片变成眼前鲜活的生命，我们心中对大自然这片绿色的馈赠亦充满感激。这次实践让我们真正明白，保护红树林从来都不只是一句空话，在自然面前我们是那么渺小，也许我们能做的微乎其微，但点点萤火终将";
    int i;
    for(i=0;i<(Nr+1);i++)
    {
        int_strncpy(allround_key+128*i,Character_to_Binary_strengthen(source+16*i),128);
    }
    return;
}


void strgentherSPN(char* filename){
    char* input;//文件读入的需要加密的明文
    char* output;//输入到文件中的加密完成的密文4
    char* decrypt;
    int lcbc[128]={0};
    int ncbc[128];
    int binary[128];
    memset(binary,0,128);

    char clear_text[17];//每次进行加密的16个字符明文
    char cipher_text[17];//每次进行加密后的16个字符密文
    int len;//输入的明文长度（字符数)
    int group_len;//分组后的组数（每组两个字符）
    int i;//分组加密计数

    FILE *pFile=fopen(filename,"r");
    fseek(pFile,0,SEEK_END);
    len=ftell(pFile);
    printf("%d\n",len);
    input=(char*)malloc(sizeof(char)*(len+1));
    output=(char*)malloc(sizeof(char)*(len+1));
    decrypt=(char*)malloc(sizeof(char)*(len+1));
    rewind(pFile);
    fread(input,1,len,pFile); //读文件
    input[len]='\0';
    printf("%s\n",input);  //显示读到的数据
    fclose(pFile);
    if(len%16!=0)
    {
        len=padding(input,len);
    }
    group_len=len/16;

    key_generation();
    for(i=0;i<group_len;i++)
    {
        strncpy(clear_text,input+16*i,16);
        clear_text[16]='\0';
        strncpy(cipher_text,Spn_Encryption_Strengthen(clear_text,lcbc),16);
        cipher_text[16]='\0';
        strncpy(output+16*i,cipher_text,16);
    }
    printf("%s\n",output);

    memset(lcbc,0,128*sizeof(int));

    for(i=0;i<group_len;i++)
    {
        strncpy(clear_text,output+16*i,16);
        clear_text[16]='\0';
        strncpy(cipher_text,Spn_Decryption_strengthen(clear_text,lcbc,ncbc),16);
        cipher_text[16]='\0';
        strncpy(decrypt+16*i,cipher_text,16);
    }
    decrypt[len]='\0';
    printf("%s",decrypt);


}


int main()//加强版spn分组长度为128比特，即16个字节
{
    char filename[100];
    memset(filename, 0, sizeof(char)*100);

    int choice;
    printf("Choose one mode\n");
    printf("1.One file \t2.Batch of file\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            printf("Filename please(no more than 100 words):");
            scanf("%s",filename);
            strgentherSPN(filename);
            break;
        case 2:
            break;


    }

    return 0;

}

