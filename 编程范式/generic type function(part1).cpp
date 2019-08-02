#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//1.build-in type�ķ��� 
//1.1 int type version
void swap(int* ap, int* bp) {
	int temp = *ap;
	*ap = *bp;
	*bp = temp;
}

//1.2 generic pointer type version
/*void���ã�
	1.���ڷ���ֵ�ϣ������޷���ֵ��
	2.���ڲ������У��������贫�������
	3.�������е�void*����Ϊgeneric pointer
	**ע�⣺1.���ܶ���һ��������void���͵�(void*���͵ı����ǿ��Ա������)
			2.���ܶ�void*������(dereference),ԭ���ǻ�����֪����Ҫ��ȡ����byte���ڴ棨û��������Ϣ��			 
*/ 
/*
��C++��ģ�������������Ҫ��ÿһ�����Ͷ�ȥ�����ض��Ĵ��룬���ǲ���ȫ(û�м��)
ex:����һ��floatһ��double��ָ�룬size = 8; ����Ҳ�ܱ������ 
*/
void swap(void* vp1, void* vp2, int size) { 
//int size���ﲻ��Ҫ��const�޶�����const��Ҫ������sharing information��������������Ϣ�����ַ���������ǿ����������иı䣩 
	char buffer[size]; //C99�Ѿ�֧����running-timeȷ��size����������(��ջ��) 
	memcpy(buffer, vp1, size);
	memcpy(vp1, vp2, size);
	memcpy(vp2, buffer, size);
}

//2.array�ķ���
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
		//ע�⣬��ָ���ַ�����ʱ����Ҫһ�����������(������void*)��
		//������������ַ�ļӷ�ʱ����������֪����Ҫ����һ�������Ƕ��ٵ�scale factor
		//char* ת����void*���ض�->���ض�������Ȼ���е�(��������ת����Ϣ)��void*ת����char*����Ҫ��ʽ����ת��(����������Ϣ) 
		void* elemAddr = (char*)base + i * elemSize;
		if (memcmp(key,  elemAddr, elemSize) == 0) //memcmp���������strcmp���������ԱȽ������ֽڵ��ڴ棬���������ַ����ȽϺ���������һ��byte size���� 
			return elemAddr;	 
	}
	return NULL;
}

//����ȽϺ���ָ��cmpfun��version 
void* lsearch(void* key, void* base, int n, int elemSize, int (*cmpfun)(void*, void*)) {
	for (int i = 0; i < n; i++) {
		void* elemAddr = (char*)base + i * elemSize; 
		if (cmpfun(key, elemAddr) == 0) 
			return elemAddr;
	} 
	return NULL;
} 
//�Ƚ���intCmp
int intCmp(void* elem1, void* elem2) { 
//���������int*,Ҳ�ɱ�������Ϊint cmpfun(void*, void*)�Ĳ�������Ϊ������ʧ��һЩ��Ϣ���ǿ��Եģ�cast��  ??���ɣ��˱�������֧�� 
//����void*����int*����ȱ����һЩ��Ϣ�������ǲ��еġ��������ȡ���β���ȫһ�������� 
	int* ip1 = (int*)elem1;
	int* ip2 = (int*)elem2;
	return *ip1 - *ip2;
} 

//�Ƚ���StrCmp
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
	//ex2:˼��һ��swap(husband, wife, sizeof(char*))�ᷢ��ʲô��
	//�����Ѿ����ˣ����Ǳ�����ʲô������Ϊ���� 
	swap(&husband, &wife, sizeof(char*));
	printf("husband:%s  wife:%s\n", husband, wife);
	
	//���Է��͵�lsearch(����Ƚ���intCmp)
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
