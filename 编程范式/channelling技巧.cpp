#include <stdio.h>

void DeclareAndInitArray() {
	int array[100];
	int i;
	for (i = 0; i < 100; i++) {
		array[i] = i;
	}
}

void PrintArray() {
	int array[100];
	int i;
	for (i = 0; i < 100; i++) {
		printf("%d ", array[i]);
	}
}

int main() {
	
	DeclareAndInitArray();
//	int i = printf("hello");
//	printf("%d\n", i);
	PrintArray();
	
	return 0;
} 
