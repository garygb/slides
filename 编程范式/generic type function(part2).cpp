#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//3.stack的泛型
//3.1 int type stack(模拟C++中的class)
typedef struct {
	int* elems;
	int logicalLength;
	int allocLength;
} stack;

//构造函数(参数是C++中省略的this) 
void stackNew(stack* s) {
	s->allocLength = 5;
	s->logicalLength = 0;
	//malloc函数原型：extern void* malloc(unsigned int num_bytes);
	//比起C++的new更为原始, new int[5], 这种做法会隐式考虑数据类型（implicit data type），malloc不会 
	//free函数原型：void free(void *ptr); 
	s->elems = (int*)malloc(sizeof(int) * 5);
	//assert：macro,测试为false的时候，结束程序，并告诉你wehat line the program ended at，为true时什么都不做 
	//好处是及时制止潜在的错误，防止若干行之后对NULL进行解引用，产生seg fault(段错误,又叫bus error) 
	assert(s->elems != NULL); //如这里若s->elems == NULL, 则会提示你Line 22 出现了running time error：assertion failed. 
}
//析构函数 
void stackDispose(stack* s) {
	free(s->elems);
}
void stackExpend(stack* s) {
/*
	//记住原先的空间的地址 
	int* tempElems = s->elems;
	//开辟新空间 
	s->elems = (int*)malloc(sizeof(int) * s->allocLength * 2);
	//拷贝元素 
	for (int i = 0; i < s->allocLength; i++) {
		s->elems[i] = tempElems[i];
	}
	//释放原空间 
	free(tempElems);
	//更新allocLength
	s->allocLength *= 2; 
*/
//更好，更有效率的方法
	s->allocLength <<= 2;
	//realloc函数在发现后续空间足够的情况下会自动伸长至第二个参数自动的空间，
	//不够长时会重新分配空间，自动完成复值和对原数组的回收
	//返回的参数可能是原空间的首地址(长度足够的情况)，也可能是新申请空间的首地址 
	//分配失败返回NULL 
	//realloc(NULL, s->allocLength * sizeof(int)) 等价于 malloc(s->allocLength * sizeof(int)) 
	s->elems = (int*)realloc(s->elems, s->allocLength * sizeof(int));  
	assert(s->elems != NULL); 
}
void stackPush(stack* s, int value) {
	if (s->logicalLength == s->allocLength) {
		stackExpend(s);
	} 
	s->elems[s->logicalLength++] = value;
} 
int  stackPop(stack* s) {
	assert(s->allocLength != 0);
	return s->elems[--s->allocLength];
}
void stackDisplay(stack* s) {
	for (int i = 0; i < s->logicalLength; i++) {
		printf("%d ", s->elems[i]);
	}
}



//client
int main() {
	
	stack s;
	//C++中构造析构是编译器自动补上的 
	stackNew(&s);
	for (int i = 0; i < 2000; i++) {
		stackPush(&s, i);
	} 
	stackDisplay(&s);
	stackDispose(&s); 
	
	return 0;
}
