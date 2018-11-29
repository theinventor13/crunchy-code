#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#define tests 10
typedef unsigned long long int uint64;
typedef long long int int64;

uint64 fibbonacci_iterative(uint64 to);
uint64 fibbonacci_recursive(uint64 to);
LONGLONG codetimer(void);
int main(void){
	size_t iterations = 1000000;
	int64 nanoseconds;	
	uint64 accumulate = 0;
	uint64 val = 1;
	size_t iter = 0;
	
	iter = 0;
	codetimer();
	for(; iter < iterations; iter++){
		fibbonacci_iterative(1000);
	}
	nanoseconds = codetimer();
	printf("'fibbonacci_iterative' nanoseconds: %u\n", nanoseconds / iterations);
	
	iter = 0;
	codetimer();
	for(;iter < iterations; iter++){
		fibbonacci_recursive(1000);
	}
	nanoseconds = codetimer();
	printf("'fibbonacci_recursive' nanoseconds: %u\n", nanoseconds / iterations);

	
	return 0;
}

LONGLONG codetimer(void){
	static LARGE_INTEGER start, end, frequency;
	static bool startstop = false;
	switch(startstop){
		case false:
			QueryPerformanceFrequency(&frequency);
			QueryPerformanceCounter(&start);
			startstop = true;
			return 0;
		case true:
			QueryPerformanceCounter(&end);
			startstop = false;
			return ((end.QuadPart - start.QuadPart) * 1000000000) / frequency.QuadPart;
			break;
	}
}

uint64 fibbonacci_recursive(uint64 to){
	static uint64 one = 1;
	static uint64 two = 1;
	static temp;
	if(to == 1){
		return two;
		one = 1;
		two = 1;
	}else{
		temp = one + two;
		two = one;
		one = temp;
		fibbonacci_recursive(--to);
	}
}

uint64 fibbonacci_iterative(uint64 to){
	uint64 one = 1;
	uint64 two = 1;
	uint64 temp = 1;
	
	if(to < 2){
		return 1;
	}else{
		to -= 2;
		while(to > 0){
			temp = one + two;
			two = one;
			one = temp;
			to--;
		}
	}
	return one;
}

