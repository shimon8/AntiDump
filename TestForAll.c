




#include <stdio.h>

//endOfInclude
void	func1()
{
	//start1
		int i, f = 10;
		for (i = 9; i > 0; i--)
			f *= i;
		//printf("%i",f);
		char * name = "dan&shimon";
		//printf("hello world!!");
		//end1
}
void	func2()
{
	//start2
	char * s = "secret";
	char * s2 = malloc(sizeof(char) * 20);
		int i, f = 10;
		for (i = 9; i > 0; i--)
			f *= i;
		//printf("%c\n", s2[0]);
		strcopy(s2, 20, "hello func2!!", 13);
		printf(s2);
		//end2
}
void	func3()
{
	//start3

	char * s = "secret2";
	char * s2 = malloc(sizeof(char) * 20);
	int x = 12;
	int y = 24;
	int z = 58;
	int r = x * y * z;
	strcopy(s2, 20, "the number is: %i\n", 18);
	printf(s2, r);
	
	//end3
}
void main() {
	func1();
	func2();
	func3();
}