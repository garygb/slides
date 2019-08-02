#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//3.stack�ķ���
//3.1 int type stack(ģ��C++�е�class)
typedef struct {
	int* elems;
	int logicalLength;
	int allocLength;
} stack;

//���캯��(������C++��ʡ�Ե�this) 
void stackNew(stack* s) {
	s->allocLength = 5;
	s->logicalLength = 0;
	//malloc����ԭ�ͣ�extern void* malloc(unsigned int num_bytes);
	//����C++��new��Ϊԭʼ, new int[5], ������������ʽ�����������ͣ�implicit data type����malloc���� 
	//free����ԭ�ͣ�void free(void *ptr); 
	s->elems = (int*)malloc(sizeof(int) * 5);
	//assert��macro,����Ϊfalse��ʱ�򣬽������򣬲�������wehat line the program ended at��Ϊtrueʱʲô������ 
	//�ô��Ǽ�ʱ��ֹǱ�ڵĴ��󣬷�ֹ������֮���NULL���н����ã�����seg fault(�δ���,�ֽ�bus error) 
	assert(s->elems != NULL); //��������s->elems == NULL, �����ʾ��Line 22 ������running time error��assertion failed. 
}
//�������� 
void stackDispose(stack* s) {
	free(s->elems);
}
void stackExpend(stack* s) {
/*
	//��סԭ�ȵĿռ�ĵ�ַ 
	int* tempElems = s->elems;
	//�����¿ռ� 
	s->elems = (int*)malloc(sizeof(int) * s->allocLength * 2);
	//����Ԫ�� 
	for (int i = 0; i < s->allocLength; i++) {
		s->elems[i] = tempElems[i];
	}
	//�ͷ�ԭ�ռ� 
	free(tempElems);
	//����allocLength
	s->allocLength *= 2; 
*/
//���ã�����Ч�ʵķ���
	s->allocLength <<= 2;
	//realloc�����ڷ��ֺ����ռ��㹻������»��Զ��쳤���ڶ��������Զ��Ŀռ䣬
	//������ʱ�����·���ռ䣬�Զ���ɸ�ֵ�Ͷ�ԭ����Ļ���
	//���صĲ���������ԭ�ռ���׵�ַ(�����㹻�����)��Ҳ������������ռ���׵�ַ 
	//����ʧ�ܷ���NULL 
	//realloc(NULL, s->allocLength * sizeof(int)) �ȼ��� malloc(s->allocLength * sizeof(int)) 
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
	//C++�й��������Ǳ������Զ����ϵ� 
	stackNew(&s);
	for (int i = 0; i < 2000; i++) {
		stackPush(&s, i);
	} 
	stackDisplay(&s);
	stackDispose(&s); 
	
	return 0;
}
