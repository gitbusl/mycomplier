#include <stdio.h>
#include <stdlib.h>

int main()
{
	int i=2;
	int j=1;
	i=i+2*3/j;	
	if(j>0){
		printf("hello");
	}
	else {
		printf("word\n");
	}
	while(j>0){
		printf("world\n");
		j=j-1;	
	}
	printf("Hello World\n");
	return 0;
}

