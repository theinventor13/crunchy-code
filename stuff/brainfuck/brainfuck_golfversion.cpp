#include <iostream>
#include <string>



int main(void){

	std::string pg = ",[>+<-]>.";
	char * cells = new char[65536];
	unsigned short int cc = 1;
	unsigned int i = 0;
	for(;cc > 0;cc++){
		cells[cc] = 0;
	}
	while(1){
		if(pg[i] == '+'){
			cells[cc]++;
		}else if(pg[i] == '-'){
			cells[cc]--;
		}else if(pg[i] == '.'){
			std::cout << cells[cc];
		}else if(pg[i] == ','){
			std::cin >> cells[cc];
		}else if(pg[i] == '>'){
			cc++;
		}else if(pg[i] == '<'){
			cc--;
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
		if(i == pg.length()){
			return 0;
		}
	}
}


	
	

