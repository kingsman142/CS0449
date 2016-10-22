#include <stdio.h>

int main(){
	printf("int size: \t\t%ld\n", sizeof(int));
	printf("short size: \t\t%ld\n", sizeof(short));
	printf("long size: \t\t%ld\n", sizeof(long));
	printf("long long size: \t%ld\n", sizeof(long long));
	printf("unsigned int size: \t%ld\n", sizeof(unsigned int));
	printf("char size: \t\t%ld\n", sizeof(char));
	printf("float size: \t\t%ld\n", sizeof(float));
	printf("double size: \t\t%ld\n", sizeof(double));
	printf("long double size: \t%ld\n", sizeof(long double));	
	return 0;
}
