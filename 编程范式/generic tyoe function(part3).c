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
//Ϊ��ȷ�����������ڿ��ٿռ��ÿһ����Ա��ָ���ʱ��Ҳ���ã���Ҫ�޸�stackNew���� 
void stackNew(stack* s, int elemSize, void (*freefunc)(void*)) { //��������freefunction��ʱ����NULL���� 
	assert(elemSize > 0); //���뱣֤elemSize����0 
	s->logicalLength = 0;
	s->allocLength = 4;
	s->elemSize = elemSize;
	s->elems = malloc(elemSize * 4);
	assert(s->elems != NULL);
	s->freefn = freefunc; 
}
//��ǿ�����������(�����������ٿռ��ÿһ����Ա��ָ��)
void stackDispose(stack* s) {
	if (s->freefn != NULL) { //freefnΪNULL����ζ�Ų���Ҫ����Ĵ��� 
		int i;
		for (i = 0; i < s->logicalLength; i++) {
			s->freefn((char*)s->elems + i * s->elemSize);
		}
	}
	free(s->elems);
}
//˼��һ��Ϊʲô����stackPop��ʱ������ռ䣿 
//��Ϊpop֮��ʵ�����ǽ���ָ�����ռ��ʹ��Ȩ�������û�����Ȼ���Բ��ܽ����ͷŵ� 
void stringFree(void* elem) {
	free(*(char**)elem);
}

//��static���Σ���������Ͳ��ܱ�����.o�ļ�������֮��ʹ��(�ڲ�����)
//��û��static��ȫ�ֺ����ǿ��Ա�����.o�ļ���ʹ�õģ� 
//����ĳ����������������֣���ϣ�������˵ĺ������ͻ(���ͳ����к���̫����) 
static void stackExpend(stack* s) { 
//Ϊ��ִ��Ч�ʣ�������stackShrink����(malloc,realloc�ڶ����ҿռ䣬ʵ�ʵ�ʱ��ɱ���ֹO(sizeof array),����O(sizeof heap)) 
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

//��stack s��pop����Ԫ��д��elemAddrΪ�׵�ַ��λ�� 
void stackPop(stack* s, void* elemAddr) {
	assert(s->logicalLength != 0); //��ֹ��һ����ջ����pop���� 
	void* source = (char*)s->elems + (--s->logicalLength) * s->elemSize;
	memcpy(elemAddr, source, s->elemSize);
}

//client
int main() {
//1.ÿ��Ԫ��Ϊint��ͨ��ջ 
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
//2.ÿ��Ԫ��Ϊchar*(string)��ͨ��ջ 
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
