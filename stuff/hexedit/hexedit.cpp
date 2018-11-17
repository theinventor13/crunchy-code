#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define BF_BLOCK 1000//1k buffer block

	void doline(signed char * buffer);
    unsigned int i = 0;//counter
	unsigned int j = 0;//counter
	unsigned int l = 0;//counter
	unsigned int c = 0;//counter
    unsigned int cs = 4;//chunk display size
    unsigned int cpl = 4;//chunks per line
    unsigned int bs = BF_BLOCK;
        
    
int main(void/*char * fn*/){
    //fn = filename
    char *fn = "shit.txt";
    FILE *hex;//file pointer
    hex = fopen(fn, "r");//load file pointer
    signed char *buffer = (signed char *) malloc(BF_BLOCK);
	
    
    while(i < bs){
        
        if(i >= bs){
            //realloc
        }
        buffer[i] = (signed char)getc(hex);//load file into buffer
        if(buffer[i] == -1){
            break;
        }else{
            i++;
        }
    }
    
    printf("file size: %i\n", i);
	while(j<i){
		doline(buffer);
	}

    return 0;
    
}

void doline(signed char *buffer){
	
	signed char next;
	for(l = 0;l < cpl;l++){
		for(c = 0;c < cs;c++){
			next = buffer[j];
			
			if(next == -1){
				printf("EOF");
				return;
			}else if(next==0){
				printf("--");
			}else if(next<17){
				printf("0%x",next);
			}else{
				printf("%x",next);
			}
			j++;
		}
		printf("  ");
	}
	printf("\n");
	
}
