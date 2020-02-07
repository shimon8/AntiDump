#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include<string.h>
unsigned char * key1;
unsigned char * key2;
unsigned char * key3;
unsigned char * key4;
void encrypt(unsigned long * v, unsigned long * k)
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

void ourEncryptionToFile(unsigned char* ptr, int length, char * nameWriteFile) {
	int modulo = length % 8;
	int rounds = length / 8;
	unsigned long * key = malloc(sizeof(unsigned long) * 4);
	unsigned char * keyTemp[4];
	keyTemp[0] = malloc(sizeof(unsigned long) + 1);
	keyTemp[1] = malloc(sizeof(unsigned long) + 1);
	keyTemp[2] = malloc(sizeof(unsigned long) + 1);
	keyTemp[3] = malloc(sizeof(unsigned long) + 1);
	strncpy_s(keyTemp[0], 5, key1, 4);
	strncpy_s(keyTemp[1], 5, key2, 4);
	strncpy_s(keyTemp[2], 5, key3, 4);
	strncpy_s(keyTemp[3], 5, key4, 4);
	for (size_t i = 0; i < 4; i++)
	{
		key[i] = keyTemp[i][0] + keyTemp[i][1] + keyTemp[i][2] + keyTemp[i][3];
	}

	for (int i = 0; i < rounds; i++)
	{

		encrypt((unsigned long*)(ptr + 8 * i), (unsigned long*)key);

	}
	FILE *f = fopen(nameWriteFile, "wb");
	fwrite(ptr, sizeof(char) *  (length - modulo), 1, f);
	fclose(f);

}
void orederkey(char ** argv) {

	key1 = argv[4];
	key2 = argv[5];
	key3 = argv[6];
	key4 = argv[7];


}
int main(int argc, char **argv)
{
	orederkey(argv);
	FILE *fileptr;
	unsigned char *buffer;
	long filelen;
	unsigned long lengthFile = atoi(argv[3]);
	char nameWriteFile[80];
	strcpy(nameWriteFile, argv[2]);
	fileptr = fopen(argv[1], "rb");  // Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file

	buffer = (unsigned char *)malloc((filelen + 1) * sizeof(char)); // Enough memory for file + \0
	fread(buffer, filelen, 1, fileptr); // Read in the entire file
	fclose(fileptr); // Close the file
	ourEncryptionToFile(buffer, lengthFile, nameWriteFile);
	return 0;
}