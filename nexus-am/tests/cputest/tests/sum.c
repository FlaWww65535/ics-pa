#include "trap.h"

int main() {
	int i = 1;
	volatile int sum = 0;
	while(i <= 1) {
		sum += i;
		i ++;
	}

	nemu_assert(sum == 1);

	return 0;
}
