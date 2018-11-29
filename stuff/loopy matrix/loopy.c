#include <stdio.h>
#include <malloc.h>
#include "sti.h"

void loopy(int y, int x){
	if(!x||!y){
		printf("Matrice is either one or zero dimensional");
		return;
	}
	int iter[2] = {0,0};
	int lims[2] = {x,y};
	int cells = x * y;
	int check = 0;
	int count = 1; 
	int updown = 1;
	int flipmode = 0;
	int * matrix = (int *)calloc(cells,sizeof(int));
	if(matrix == NULL){
		printf("Allocation error");
		return;
	}
	while(count<=cells){//no explanation 4u, my method is a secret
	//unless you can read this and understand it
	//good luck
		matrix[(iter[1]*x)+iter[0]] = count++;	
		if((iter[check]+updown==lims[check]||iter[check]+updown<0)||(matrix[((iter[1]+((check==1)*updown))*x)+iter[0]+((check==0)*updown)]!=0)){
			check ^= 1;
			if(++flipmode == 2){
				flipmode = 0;
				updown *= -1;
			}
		}
		iter[check] += updown;	
	}
	
	for(int i = 0;i < y;i++){
		for(int k = 0;k < x;k++){
			if(matrix[(i*x)+k] < 10){
				printf(" ");
			}
			printf("%i",matrix[(i*x)+k]);
			printf(" ");		 
		}
		printf("\n");
	}
	free(matrix);
	return;
}

int main(int argc, char * argv[]){
	/*
	if(argc!=3){
		printf("usage: ./loopy [rows] [columns]\n");
	}else{
		int r = sti(argv[1]);
		int c = sti(argv[2]);
		loopy(r,c);
	}
	*/
	//Uncomment this to use without command line arguments
	loopy(10,7); 
	return 0;
}


