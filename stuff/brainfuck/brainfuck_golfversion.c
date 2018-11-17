#include <stdio.h>
int main(int argc, char * argv[]){
	if(argc!=2){
		return 1;
	}
	char *pg = argv[1];
	//char pg[] = ",>,<[>[>+>+<<-]>>[-<<+>>]<<<-]>>";
	char cells[65536];
	unsigned short int cc = 1;
	unsigned int i = 0;
	for(;cc > 0;cc++){
		cells[cc] = 0;
	}
	while(1){
		switch(pg[i]){
		case '+':
			cells[cc]++;
			break;
		case '-':
			cells[cc]--;
			break;
		case '.':
			printf("%c",cells[cc]);
			break;
		case ',':
			scanf("%1s",cells+cc);
			break;
		case '>':
			cc++;
			break;
		case '<':
			cc--;
			break;
		case '[':
			if(!cells[cc]){
				while(pg[i] != ']'){
					i++;
				}
			}
			break;
		default:
			if(cells[cc]){
				while(pg[i] != '['){
					i--;
				}
			}
			break;
		}
		i++;
		if(!pg[i]){
			return 0;
		}
	}
	return 0;
}


	
	

