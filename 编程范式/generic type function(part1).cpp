#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//1.build-in type的泛型 
//1.1 int type version
void swap(int* ap, int* bp) {
	int temp = *ap;
	*ap = *bp;
	*bp = temp;
}

//1.2 generic pointer type version
/*void作用：
	1.放在返回值上，声明无返回值；
	2.放在参数表中，声明无需传入参数；
	3.参数表中的void*，作为generic pointer
	**注意：1.不能定义一个变量是void类型的(void*类型的变量是可以被定义的)
			2.不能对void*解引用(dereference),原因是机器不知道需要提取多少byte的内存（没有类型信息）			 
*/ 
/*
与C++的模板比起来，不需要对每一种类型都去生成特定的代码，但是不安全(没有检查)
ex:传入一个float一个double的指针，size = 8; 这样也能被编译过 
*/
void swap(void* vp1, void* vp2, int size) { 
//int size这里不需要加const限定符，const主要被用于sharing information（变量保存了信息，如字符串，对象，强调不对其进行改变） 
	char buffer[size]; //C99已经支持了running-time确定size的数组声明(在栈区) 
	memcpy(buffer, vp1, size);
	memcpy(vp1, vp2, size);
	memcpy(vp2, buffer, size);
}

//2.array的泛型
//2.1 int type version 
int lsearch(int key, int array[], int size) { 
	for (int i = 0; i < size; i++) {
		if (array[i] == key) {
			return i;
		}
	}
	return -1;
}
//2.2 generic type version
void* lsearch(void* key, void* base, int n, int elemSize) {
	for (int i = 0; i < n; i++) {
		//注意，在指针地址运算的时候，需要一个具体的类型(不能用void*)，
		//这样，在做地址的加法时，编译器才知道需要乘上一个具体是多少的scale factor
		//char* 转换到void*（特定->不特定）是自然进行的(无需额外的转换信息)，void*转换到char*才需要显式类型转换(加入类型信息) 
		void* elemAddr = (char*)base + i * elemSize;
		if (memcmp(key,  elemAddr, elemSize) == 0) //memcmp函数相对于strcmp函数，可以比较任意字节的内存，因此相对于字符串比较函数，多了一个byte size参数 
			return elemAddr;	 
	}
	return NULL;
}

//传入比较函数指针cmpfun的version 
void* lsearch(void* key, void* base, int n, int elemSize, int (*cmpfun)(void*, void*)) {
	for (int i = 0; i < n; i++) {
		void* elemAddr = (char*)base + i * elemSize; 
		if (cmpfun(key, elemAddr) == 0) 
			return elemAddr;
	} 
	return NULL;
} 
//比较器intCmp
int intCmp(void* elem1, void* elem2) { 
//这里如果是int*,也可被传入作为int cmpfun(void*, void*)的参数，因为这样损失了一些信息，是可以的（cast）  ??存疑，此编译器不支持 
//但是void*传入int*，则缺少了一些信息，这样是不行的。这里则采取和形参完全一样的类型 
	int* ip1 = (int*)elem1;
	int* ip2 = (int*)elem2;
	return *ip1 - *ip2;
} 

//比较器StrCmp
int StrCmp(void* vp1, void* vp2) {
	char* s1 = *(char**)vp1;
	char* s2 = *(char**)vp2;
	return strcmp(s1, s2);
} 


//client
int main() {
	
	//Non-pointer data type
	int x = 17, y = 37;
	swap(&x, &y, sizeof(int));
	printf("x = %d, y = %d\n", x, y);
	
	//data type is already pointer
	char* husband = strdup("Fred");
	char* wife = strdup("Wilma");
	//ex2:思考一下swap(husband, wife, sizeof(char*))会发生什么，
	//尽管已经错了，但是编译器什么都不会为你检查 
	swap(&husband, &wife, sizeof(char*));
	printf("husband:%s  wife:%s\n", husband, wife);
	
	//测试泛型的lsearch(传入比较器intCmp)
	//non-pointer array elem data type 
	int array[] = {4,2,3,7,11,6};
	int size = 6;
	int number = 7;
	int* found = (int*)lsearch(&number, array, size, sizeof(int), intCmp);
	if (found == NULL) {
		printf("Not found.\n");
	} else {
		printf("Found.\n");
	}
	//array elem data type is already pointer
	char* notes[] = {"Ab", "F#", "B", "Eb", "D"};
	char* favoriteNote = "Eb";
	char** found2 = (char**)lsearch(&favoriteNote, notes, 5, sizeof(char*), StrCmp);
	if (found == NULL) {
		printf("Not found.\n");
	}
	printf("found:%s\n", *found2);
	return 0;
}
