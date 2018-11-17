#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

union INST{
	unsigned short int WORD;
	unsigned char BYTE[2];
}CI;

int main(void){
	
	unsigned char a = 128;
	unsigned char b = 128;
	unsigned short int c = a + b;
	cout << (c>>8) << endl;
	
	return 0;
}

