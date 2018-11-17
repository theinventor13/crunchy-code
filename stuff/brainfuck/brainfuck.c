#include <stdio.h> 
#define MEMORY_SIZE 30000

int main(int argc, char * argv[]){
	
	//this is a very basic brainfuck interpreter
	if(argc != 2){ //we will only accept brainfuck source code passed in the first program arg
		printf("Usage: brainfuck \"brainfuck program\"\n"); 
		return 1;
	}
	
	char * program = argv[1]; //first thing in argv is program name (argv[0]), we want the first arg (argv[1])
	char cells[MEMORY_SIZE] = {0}; //memory array
	unsigned short int mp = 0; //memory pointer
	for(;mp < MEMORY_SIZE; mp++){ //zero out array
		cells[mp] = 0;
	}
	mp = 0;
	unsigned int i = 0; //instruction pointer
	unsigned int pc = 0; //parentheses counter
	
	while(program[i]){
		
		switch(program[i]){ 
		
			case '+': 
			    //increment value at current memory location
				cells[mp]++;
				break;
				
			case '-': 
				//decrement value at current memory location
				cells[mp]--;
				break;
				
			case '.': 
				//output value from current memory location to stdout
				if(cells[mp] == 10){
					printf("\n");
				}else{
					printf("%c",cells[mp]);
				}
				break;
				
			case ',': 
				//set value at current memory location to character read from stdin
				cells[mp] = getchar();
				fflush(stdin);
				if(cells[mp] == '\n'){
					cells[mp] = 10;
				}
				break;
				
			case '>': 
				//increment memory pointer
				mp++;
				if(mp > MEMORY_SIZE - 1){
					mp = 0;
				}
				break;
				
			case '<': 
				//decrement memory pointer
				mp--;
				if(mp < 0){
					mp = MEMORY_SIZE - 1;
				}
				break;
				
			case '[': 
			    //if value at current memory location is zero, jump to instruction directly after the matching closing square bracket (move forwards). 
			    //if value at current memory location is nonzero, step to next instruction
				if(!cells[mp]){
					while(program[i]){
						i++;
						if(program[i] == ']' && !pc){
							break;
						}else if(program[i] == '['){
							pc++;
						}else if(program[i] == ']'){
							pc--;
						}else if(!program[i]){
							return 1;
						} 
					}
				}
				break;
				
			case ']':
				//if value at current memory location is nonzero, jump to instruction directly after the matching opening square bracket (move backwards). 
			    //if value at current memory location is zero, step to next instruction
				if(cells[mp]){
					while(program[i]){
						i--;
						if(program[i] == '[' && !pc){
							break;
						}else if(program[i] == ']'){
							pc++;
						}else if(program[i] == '['){
							pc--;
						}else if(!program[i]){
							printf("Unbalanced Bracket\n");
							return 1;
						}
					}
				}
				break;
			

			default: 
				//ignore any non-instructions
				break;
		}
		
		i++; //increment instruction pointer
		
	}
	
	return 0;
	
}


	
	

