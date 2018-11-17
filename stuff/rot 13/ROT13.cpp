#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

unsigned int ROT13(ifstream& infile, ofstream& outfile);
int main(void){
	ifstream input;
	ofstream output;
	string fn;
	
	while(1){
		cout << "Enter input filename: ";
		cin >> fn;
		input.open(fn);
		if(input.is_open()){
			break;
		}else{
			cout << fn << " could not be opened, try again" << endl;
		}
	}
	
	while(1){
		cout << "Enter output filename: ";
		cin >> fn;
		output.open(fn);
		if(output.is_open()){
			break;
		}else{
			cout << fn << " could not be opened, try again" << endl;
		}
	}
	
	ROT13(input, output);

	return 0;
}

unsigned int ROT13(ifstream& infile, ofstream& outfile){
	if(infile.is_open()&&outfile.is_open()){
		char getch;
		unsigned int len = 0;
		while(!infile.eof()){
			infile.get(getch);
			if(isalpha(getch)){
				if(isupper(getch)){
					getch += (getch < 78)?13:-13;
				}else{
					getch += (getch < 110)?13:-13;
				}
			}
			cout << getch;
			outfile << getch;
			len++;
		}
		infile.close();
		outfile.close();
		return;	
	}else{
		cout << "One or both of the provided file streams are invalid" << endl;
		return;
	}
	
}
