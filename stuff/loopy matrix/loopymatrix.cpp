#include <iostream>
using namespace std;

void loopy(int x, int y);

int main(void){
	loopy(9,10);
	return 0;
}

void loopy(int y, int x){
	if(!x||!y){
		cout << "Matrice is either one or zero dimensional" << endl;
		return;
	}
	bool check = false;
	int iter[2] = {0,0};
	int lims[2] = {x,y};
	int cells = x * y;
	int count = 1; 
	int updown = 1;
	int flipmode = 0;
	int * matrix = calloc(cells,sizeof(int));
	if(matrix == NULL){
		cout << "Allocation error" << endl;
		return;
	}
	while(count<=cells){//no explanation 4u, my method is a secret
	//unless you can read this and understand it
	//good luck
		matrix[(iter[1]*x)+iter[0]] = count++;	
		if((iter[check]+updown==lims[check]||iter[check]+updown<0)||(matrix[((iter[1]+((check==true)*updown))*x)+iter[0]+((check==false)*updown)]!=0)){
			check ^= true;
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
				cout << " ";
			}
			cout << matrix[(i*x)+k];
			cout << " ";		 
		}
		cout << endl;
	}
	free(matrix);
	return;
}
