#include <stdio.h>
#include <assert.h>

//int is_little_endian() {
//	int test_num = 1;
//	return *(char*)&test_num;
//} 

typedef unsigned pack_t;

//int xbyte(pack_t word, int bytenum) {
//	assert(bytenum >= 0 && bytenum < 4);
//	char byte = (word >> (byte << 3)) & 0xff;
//	return (int)byte;
//}

int xbyte(pack_t word, int bytenum) {
	assert(bytenum >= 0 && bytenum < 4);
	int num = (word << ((3 - bytenum) << 3));
	return num >> 24;
}

int main() {
	
//	printf("%d\n", is_little_endian());
	
	pack_t num = -1;
	printf("%d", xbyte(num, 2));
	
	return 0;
}
