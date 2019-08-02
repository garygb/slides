#include <stdio.h>
typedef int float_bits;

float_bits bit_pattern(float f) {
	return *(int*)&f;
}

int isNaN(float_bits fb) {
	printf("%x\n", fb);
	printf("%x\n", (fb & 0x7f800000));
	if (((fb & 0x7f800000) == 0x7f800000 ) && (fb & 0x007fffff)) { //指数部分全为1且尾数部分不全为0 
		return 1;
	} else {
		return 0;
	}
}

float float_negate(float_bits fb) {
	if (isNaN(fb)) {
		return *(float*)&fb;
	} else {
		int temp = fb ^ 0x80000000;
		return *(float*)(&temp);
	}
}

int main() {
	int a = 0x7f800700; //bit pattern of NaN
	float f1 = *(float*)&a;
	float f2 = 5.3;
	
	printf("%f", float_negate(bit_pattern(f1)));
	printf("%f", float_negate(bit_pattern(f2)));
	
	return 0;
} 
