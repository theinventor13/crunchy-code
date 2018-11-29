#include <stdio.h>
#include <stdbool.h>
#define buffersize 2048

void remove_newline(char * remove){
	size_t iter = 0;
	while(remove[iter] != 0){
		if(remove[iter] == '\n'){
			remove[iter] = 0;
		}
		iter++;
	}
}

int main(void){
	char buffer[buffersize];
	size_t groupbytes = 2;
	size_t linebytes = 16;
	FILE * handle;
	while(1){
		printf("file to open: ");
		fgets(buffer, buffersize, stdin);
		remove_newline(buffer);
		handle = fopen(buffer, "r");
		if(handle == NULL){
			printf("couldnt open file: %s\n", buffer);
			bool exit = false;
			do{
				printf("exit?: ");
				fgets(buffer, buffersize, stdin);	
				switch(buffer[0]){
					case 'y':
						return 1;
					case 'n':
						exit = true;
						break;
					default:
						break;	
				}
			}while(!exit);
		}else{
			printf("\nfile %s: \n", buffer);
			rewind(handle);
			size_t count = 0;
			size_t lines = 0;
			char cc = 0;
			while((cc = getc(handle)) != EOF){
				if(count == 0){
					printf("  offset:");
					for(size_t iter = 0; iter < linebytes; iter++){
						printf(" %02u", iter);
					}	
				}
				if((count % linebytes == 0) || (count == 0)){
					printf("\n%08x: ", lines * linebytes);
					lines++;
				}
				printf("%02hhx", cc);
				count++;
				if(count % (1 << groupbytes) == 0){
					printf(" ");
				}else{
					printf("-");
				}				
			}
			printf("\n%u bytes loaded\n\n", count);
		}
			
	}
	return 0;
}
