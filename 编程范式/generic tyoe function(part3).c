#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//3.2 generic type stack
typedef struct {
	void* elems; //lost type information(size) 
	int elemSize;
	int logicalLength;
	int allocLength;
	void (*freefn)(void*);
} stack;
//为了确保析构函数在开辟空间的每一个成员是指针的时候也适用，需要修改stackNew函数 
void stackNew(stack* s, int elemSize, void (*freefunc)(void*)) { //无需特殊freefunction的时候传入NULL即可 
	assert(elemSize > 0); //必须保证elemSize大于0 
	s->logicalLength = 0;
	s->allocLength = 4;
	s->elemSize = elemSize;
	s->elems = malloc(elemSize * 4);
	assert(s->elems != NULL);
	s->freefn = freefunc; 
}
//加强版的析构函数(可以析构开辟空间的每一个成员是指针)
void stackDispose(stack* s) {
	if (s->freefn != NULL) { //freefn为NULL，意味着不需要特殊的处理 
		int i;
		for (i = 0; i < s->logicalLength; i++) {
			s->freefn((char*)s->elems + i * s->elemSize);
		}
	}
	free(s->elems);
}
//思考一下为什么不在stackPop的时候清理空间？ 
//因为pop之后实际上是将所指的这块空间的使用权交给了用户，当然电脑不能将它释放掉 
void stringFree(void* elem) {
	free(*(char**)elem);
}

//有static修饰，这个函数就不能被其他.o文件在链接之后使用(内部修饰)
//（没有static的全局函数是可以被其他.o文件所使用的） 
//用于某个函数名经常会出现，不希望与他人的函数相冲突(大型程序中函数太多了) 
static void stackExpend(stack* s) { 
//为了执行效率，不考虑stackShrink操作(malloc,realloc在堆中找空间，实际的时间成本不止O(sizeof array),而是O(sizeof heap)) 
	s->allocLength <<= 1;
	s->elems = realloc(s->elems, s->elemSize * s->allocLength);
	assert(s->elems != NULL);
}

void stackPush(stack* s, void* elemAddr) {
	if (s->logicalLength == s->allocLength) {
		stackExpend(s);
	}
	void* target = (char*)s->elems + s->elemSize * s->logicalLength;
	memcpy(target, elemAddr, s->elemSize);
	s->logicalLength++;
}

//将stack s中pop出的元素写在elemAddr为首地址的位置 
void stackPop(stack* s, void* elemAddr) {
	assert(s->logicalLength != 0); //防止对一个空栈进行pop操作 
	void* source = (char*)s->elems + (--s->logicalLength) * s->elemSize;
	memcpy(elemAddr, source, s->elemSize);
}

//client
int main() {
//1.每个元素为int的通用栈 
	stack s;
	stackNew(&s, sizeof(int), 0);
	int i; 
	for (i = 0; i < 20; i++) {
		stackPush(&s, &i);
	}
	int top;
	stackPop(&s, &top);
	printf("The pop elem is %d.\n", top);
	stackDispose(&s);
//2.每个元素为char*(string)的通用栈 
	const char* friends[] = {"Al", "Bob", "Carl"};
	stack stringStack;
	stackNew(&stringStack, sizeof(char*), stringFree);
	int j;
	char* arr[3];
	for (j = 0; j < 3; j++) {
		char* copy = strdup(friends[j]);
		stackPush(&stringStack, &copy);
	}
	char* name;
	for (j = 0; j < 3; j++) {
		stackPop(&stringStack, &name);
		printf("%s\n", name);
	}
	stackDispose(&stringStack);
	
	return 0;
}
