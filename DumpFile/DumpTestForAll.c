char * b = "OurStartOurStartOurStar1111111";
unsigned char * key1="qazx";
unsigned char * key2 = "swed"; 
unsigned char * key3 = "cvfr";
unsigned char * key4 = "4321";
 char * c = "OurEndOurEndOurEndOurEnd111111";





#include <stdio.h>

//endOfInclude

#include <stdlib.h>
#include <string.h>
#include "crypt.h"
//to encrypt the call

unsigned int encoder;

unsigned int decoder;
unsigned int _mask;

void ourDecreptionXor(unsigned char*	ptr, int length) {



	for (int i = 0; i < length; i++)
	{
                              int x=5;
		for (size_t j = 0; j < 3; j++)
		{
			ptr[i] ^= key1[j] ^ key2[j] ^ key3[j] ^ key4[j];

		}
		ptr[i] ^=x;
	}
}
void ourEncreptionXor(unsigned char*	ptr, int length) {


	for (int i = 0; i < length; i++)
	{
                              int x=5;
		for (size_t j = 0; j < 3; j++)
		{
			ptr[i] ^= key1[j] ^ key2[j] ^ key3[j] ^ key4[j];

		}
		ptr[i] ^=x;
	}
}
void strcopy(unsigned char * dest, unsigned long sizeDest, unsigned char * source, unsigned long sizeSource) //size of the dest
{

	unsigned long strLen = sizeSource;
	char * strDecrypt = malloc(strLen);
	strncpy_s(strDecrypt, strLen, source,strLen);
	if (strDecrypt[0] != source[0])
		strDecrypt[0] = source[0];
	ourDecreptionXor(strDecrypt, strLen);
	strncpy_s(dest, sizeDest, strDecrypt, strLen);
   
}
void encipher(unsigned long * v, unsigned long * k)
{
	register unsigned long  y = v[0], z = v[1], sum = 0, delta = 0x9E3779B9,
		a = k[0], b = k[1], c = k[2], d = k[3], n = 32;


	while (n-- > 0)
	{
		sum += delta;
		y += ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);
		z += (y << 4) + c ^ y + sum ^ (y >> 5) + d;
	}

	v[0] = y; v[1] = z;
	

}
void decipher(unsigned long * v, unsigned long * k)
{
	register unsigned long y = v[0], z = v[1],  //sum = 0xC6EF3720,

		delta = 0x9E3779B9, sum = delta << 5, a = k[0], b = k[1], c = k[2],
		d = k[3], n = 32;


	/* sum = delta<<5, in general sum = delta * n */

	while (n-- > 0)
	{
		z -= (y << 4) + c ^ y + sum ^ (y >> 5) + d;
		y -= (z << 4) + a ^ z + sum ^ (z >> 5) + b;
		sum -= delta;
	}

	v[0] = y; v[1] = z;

}
void ourEncreption(unsigned char*ptr, int length) {
	int modulo = length % 8;
	int rounds = length  / 8 ;
	unsigned long * key = malloc(sizeof(unsigned long) * 4);
	unsigned char * keyTemp[4];
	keyTemp[0] = malloc(sizeof(unsigned long) + 1);
	keyTemp[1] = malloc(sizeof(unsigned long) + 1);
	keyTemp[2] = malloc(sizeof(unsigned long) + 1);
	keyTemp[3] = malloc(sizeof(unsigned long) + 1);
	strcopy(keyTemp[0], 5, key1, 4);
	strcopy(keyTemp[1], 5, key2, 4);
	strcopy(keyTemp[2], 5, key3, 4);
	strcopy(keyTemp[3], 5, key4, 4);
	for (size_t i = 0; i < 4; i++)
	{
		key[i] = keyTemp[i][0] + keyTemp[i][1] + keyTemp[i][2] + keyTemp[i][3];
	}
	for (int i = 0; i < rounds; i++)
	{
		
		encipher((unsigned long*)(ptr + 8*i), (unsigned long*)key);

	}
}
void ourDecreption(unsigned char*	ptr, int length) {
	
	int modulo = length % 8;
	int rounds = length / 8 ;
	unsigned long * key = malloc(sizeof(unsigned long) * 4);
	unsigned char * keyTemp[4];
	keyTemp[0] = malloc(sizeof(unsigned long) + 1);
	keyTemp[1] = malloc(sizeof(unsigned long) + 1);
	keyTemp[2] = malloc(sizeof(unsigned long) + 1);
	keyTemp[3] = malloc(sizeof(unsigned long) + 1);
	strcopy(keyTemp[0], 5, key1, 4);
	strcopy(keyTemp[1], 5, key2, 4);
	strcopy(keyTemp[2], 5, key3, 4);
	strcopy(keyTemp[3], 5, key4, 4);
	for (size_t i = 0; i < 4; i++)
	{
		key[i] = keyTemp[i][0] + keyTemp[i][1] + keyTemp[i][2] + keyTemp[i][3];
	}

	for (int i = 0; i < rounds; i++)
	{
	
		decipher((unsigned long*)(ptr +  8*i), (unsigned long*) key);

	}
}
void	decode_code(unsigned char*	ptr, int length)
{
	SETRWX((unsigned int)ptr, length);
                   ourDecreption(ptr, length);
	SETROX((unsigned int)ptr, length);
}

/*	This function encodes back the previously decoded memory area 
 * 	in code section	
 */
void	encode_code(unsigned char* ptr, int length)
{
	SETRWX((unsigned int)ptr, length);
	ourEncreption(ptr, length);
	SETROX((unsigned int)ptr, length);
}
void	__attribute__((constructor))	prepare()
{


	/*	Prepare codec function addresses	*/
	_mask = (unsigned int)rand();				// Create random mask
	encoder= (unsigned int)encode_code ^ _mask;	// Encrypt "ecnode_code" address	
	decoder = (unsigned int)decode_code ^ _mask;// Encrypt "decode_code" address
	
}void	func1()
{
	//start1

unsigned int	addr1, len1;
	ASM("movl 	$enc_start1, %0\n\t"\
		"movl 	$enc_end1, %1\n\t"\
		: "=r"(addr1), "=r"(len1));
	len1 -= addr1;
                  ourCall(decoder,addr1, len1);//call to func decode_code
	ASM("enc_start1:\n\t");
	ASM(\
		"movl	$1112345789, %%ebx\n\t"\
		"movl	$1112345789, %%ebx\n\t"\
		"movl	$1112345789, %%ebx\n\t"\
		: : : "ebx");
			int i, f = 10;
		for (i = 9; i > 0; i--)
			f *= i;
		//printf("%i",f);
		char * name = "dan&shimon";
		//printf("hello world!!");
		//end1

	ASM(\
		"movl	$1212345789, %%ebx\n\t"\
		"movl	$1212345789, %%ebx\n\t"\
		"movl	$1212345789, %%ebx\n\t"\
		: : : "ebx");
	ASM("enc_end1:\n\t");
		
                 //call to func encode_code
                  ourCall(encoder
, addr1, len1);}
void	func2()
{
	//start2

	unsigned int	 addr2, len2;
	ASM("movl 	$enc_start2, %0\n\t"\
		"movl 	$enc_end2, %1\n\t"\
		: "=r"(addr2), "=r"(len2));
	len2 -= addr2;
                  ourCall(decoder, addr2, len2);//call to func decode_code
	ASM("enc_start2:\n\t");
	ASM(\
		"movl	$2112345789, %%ebx\n\t"\
		"movl	$2112345789, %%ebx\n\t"\
		"movl	$2112345789, %%ebx\n\t"\
		: : : "ebx");
char * b2 = "OurStartOurStartOurStar2222222";	char * s = "secret";
	char * s2 = malloc(sizeof(char) * 20);
		int i, f = 10;
		for (i = 9; i > 0; i--)
			f *= i;
		//printf("%c\n", s2[0]);
		strcopy(s2, 20, "hello func2!!", 13);
		printf(s2);
		//end2

char * c2 = "OurEndOurEndOurEndOurEnd222222";	
ASM(\
		"movl	$2212345789, %%ebx\n\t"\
		"movl	$2212345789, %%ebx\n\t"\
		"movl	$2212345789, %%ebx\n\t"\
		::: "ebx");
	ASM("enc_end2:\n\t");
                 //call to func encode_code
	                  ourCall(encoder
, addr2, len2);}
void	func3()
{
	//start3

	unsigned int	 addr3, len3;
	ASM("movl 	$enc_start3, %0\n\t"\
		"movl 	$enc_end3, %1\n\t"\
		: "=r"(addr3), "=r"(len3));
	len3 -= addr3;
                  
	                  ourCall(decoder, addr3, len3);//call to func decode_code
	/*	This label is a mark for our decoder and means the beginning of
	 * 	memory area to decode	*/
	ASM("enc_start3:\n\t");
	ASM(\
		"movl	$3112345789, %%ebx\n\t"\
		"movl	$3112345789, %%ebx\n\t"\
		"movl	$3112345789, %%ebx\n\t"\
		: : : "ebx");
char * b3 = "OurStartOurStartOurStar3333333";
	char * s = "secret2";
	char * s2 = malloc(sizeof(char) * 20);
	int x = 12;
	int y = 24;
	int z = 58;
	int r = x * y * z;
	strcopy(s2, 20, "the number is: %i\n", 18);
	printf(s2, r);
	
	//end3

char * c3 = "OurEndOurEndOurEndOurEnd333333";
	ASM(\
		"movl	$3212345789, %%ebx\n\t"\
		"movl	$3212345789, %%ebx\n\t"\
		"movl	$3212345789, %%ebx\n\t"\
	::: "ebx");
	ASM("enc_end3:\n\t");
                 //call to func encode_code
	                  ourCall(encoder, addr3, len3);}
void main() {
	func1();
	func2();
	func3();
}