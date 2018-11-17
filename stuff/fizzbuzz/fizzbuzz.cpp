#include <iostream>
using namespace std;

void fizzbuzz(void);

int main(void){
	
	fizzbuzz();
	
	return 0;

}

void fizzbuzz(void){
	unsigned int three = 0;
	unsigned int five = 0;
	bool na;
	
	for(unsigned int i = 1; i <= 90; i++){
	three++;
	five++;	
	if(three==3){
		cout << "fizz";
		three = 0;
		na = false;
	}
	if(five==5){
		cout << "buzz";
		five = 0;
		na = false;
	}
	cout << "(" << i << ")" << endl;
	}
	
	
	return;
}


