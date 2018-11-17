#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

string program;

bool brainfuck(string pg, int mem_len = 30000);

int main(void){

	bool yup = brainfuck(",[>+<-]>.");
	return 0;
}

bool brainfuck(string pg, int mem_len = 30000){
	char * cells = new char[mem_len];//cell array
	const int p_len = pg.length();//program length
	int i = 0;//slut counter, used by every loop in the function
	int cc = 0;//current cell
	int open = 0;//count bracket layers
	bool done = false;//check whether program is finished running
	
	//////////CHECK INPUT PROGRAM FOR SYNTAX ERRORS/////////
	for(i = 0; i < p_len;i++){
		if(pg[i] == '['){
			open++;
		}else if(pg[i] == ']'){
			if(!open){
				cout << endl << "ERROR: Closing bracket without opening bracket at instruction " << i+1 << endl;
				return false;
			}else{
				open--;
			}
		}else if((pg[i] != '+')&&(pg[i] != '-')&&(pg[i] != '<')&&(pg[i] != '>')&&(pg[i] != '.')&&(pg[i] != ',')){
			cout << endl << "ERROR: Invalid character" << endl;
			return false;
		}
		cout << pg[i];
	}
	cout << endl;
	if(open){
		cout << "ERROR: Missing closing bracket at end" << endl;
		return false;
	}
	////////END CHECK INPUT PROGRAM FOR SYNTAX ERRORS////////
	
	//////////CLEAN CELL ARRAY (BLEACH DAT HEAP)////////////
	mem_len--;
	for(i = 0;i <= mem_len;i++){
		cells[i] = 0;//"cause i forgot how to calloc();"
	}
	////////END CLEAN CELL ARRAY (BLEACH DAT HEAP)//////////
	
	/////////////////////RUN PROGRAM////////////////////////
	i = 0;//"You're nothing, and you'll ALWAYS be nothing!"
	
	while(!done){
		if(pg[i] == '+'){
			cells[cc]++;
		}else if(pg[i] == '-'){
			cells[cc]--;
		}else if(pg[i] == '.'){
			cout << cells[cc];
		}else if(pg[i] == ','){
			cin >> cells[cc];
		}else if(pg[i] == '>'){
			if(cc < mem_len){
				cc++;
			}else{
				cc = 0;
			}
		}else if(pg[i] == '<'){
			if(cc > 0){
				cc--;
			}else{
				cc = mem_len;
			}
		}else if(pg[i] == '['){
			if(!cells[cc]){
				while(pg[i] != ']'){
					i++;
				}
			}
		}else{
			if(cells[cc]){
				while(pg[i] != '['){
					i--;
				}
			}
		}
		i++;
		if(i == p_len){
			done = true;
		}
	}
	///////////////////END RUN PROGRAM//////////////////////
	//add memory dump feature
	return true;
}
