#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;
#include "get_text.h"
#include "rand_util.h"

	
	char nobody = 0;
	char you = 1;
	char cpu = 2;
	
	
	char board[3][3] = 
	{{nobody,you,nobody},
	 {nobody,nobody,nobody},
	 {nobody,cpu,nobody}};

char check_win(void);
void cpu_tick(void);
void draw_board(void);
char draw_token(int x, int y);
void clear_board(void);

int main(void){
	
	bool gofirst;
	bool took = true;
	char won = 0;
	int spot;
	int row;
	int col;
	
	while(1){
		cout << "============New Game=============" << endl << endl;
		gofirst = yesno("Do you want to go first?");
		
		while(!won){
			if(gofirst){
				draw_board();
				while(took){
					cout << "Pick spot (1-9): ";
					cin >> spot;
					cout << endl << "Spot " << spot;
					spot--;
					col = spot % 3;
					row = (spot - col)/3;
					if(board[row][col] != nobody){
						cout << " is already ticked, pick another one" << endl;
					}else{
						cout << " ticked" << endl;
						board[row][col] = you;
						took = false;
					}
					cout << endl;
				}
				took = true;
				draw_board();
				won = check_win();
				if(won==1){
					cout << endl << "You win" << endl << endl;
				}else if(won==2){
					cout << endl << "Draw" << endl << endl;
				}
			}
			
			if(!won){
				gofirst = true;
				cout << endl << "CPU Thinking..." << endl;
				cpu_tick();
				won = check_win();
				if(won==1){
					cout << endl << "You Lose" << endl << endl;
				}else if(won==2){
					cout << endl << "Draw" << endl << endl;
				}else{
					cout << endl << "Your Turn" << endl << endl;
				}
			}	
		}
		draw_board();
		if(!yesno("Play again?")){
			return 0;
		}
		won = 0;
		took = true;
		clear_board();
		
		
	}
	
	
	
	return 0;
}

void clear_board(void){
	
	board[0][0] = board[1][0] = board[2][0] = board[0][1] = board[1][1] = board[2][1] = board[0][2] = board[1][2] = board[2][2] = nobody;	
	return;
	
}

char check_win(void){
	
	bool draw = true;
	
	if((board[0][0]==board[1][1])&&(board[1][1]==board[2][2])&&(board[0][0]!=nobody)){
		return 1;
	}else if((board[0][0]==board[0][1])&&(board[0][1]==board[0][2])&&(board[0][0]!=nobody)){
		return 1;
	}else if((board[0][0]==board[1][0])&&(board[1][0]==board[2][0])&&(board[0][0]!=nobody)){
		return 1;
	}else if((board[1][0]==board[1][1])&&(board[1][1]==board[1][2])&&(board[1][0]!=nobody)){
		return 1;
	}else if((board[2][0]==board[2][1])&&(board[2][1]==board[2][2])&&(board[2][0]!=nobody)){
		return 1;
	}else if((board[0][1]==board[1][1])&&(board[1][1]==board[2][1])&&(board[0][1]!=nobody)){
		return 1;
	}else if((board[0][2]==board[1][2])&&(board[1][2]==board[2][2])&&(board[0][2]!=nobody)){
		return 1;
	}else if((board[0][2]==board[1][1])&&(board[1][1]==board[2][0])&&(board[0][2]!=nobody)){
		return 1;
	}else{
		for(int i = 0;i < 3; i++){
			for(int k = 0; k < 3; k++){
				if(board[i][k] == nobody){
					draw = false;
					break;
				}
			}
		}
		if(draw){
			return 2;
		}
	}
	
	return 0;
}

void cpu_tick(void){
	int row;
	int col;
	int spt;
	bool tck = false;
	
	while(!tck){//cpu literally just picks random spot
		spt = i_rand(8);
		col = spt % 3;
		row = (spt - col)/3;
		if(board[row][col]==nobody){
			board[row][col] = cpu;
			tck = true;
		}
	}
	
	return;	
}

void draw_board(void){
	cout << endl;
	cout << " " << draw_token(0,0) << " | " << draw_token(1,0) << " | " << draw_token(2,0) << endl;
	cout << "-----------" << endl;
	cout << " " << draw_token(0,1) << " | " << draw_token(1,1) << " | " << draw_token(2,1) << endl;
	cout << "-----------" << endl;
	cout << " " << draw_token(0,2) << " | " << draw_token(1,2) << " | " << draw_token(2,2) << endl;
	cout << endl;
	return;
}

char draw_token(int x, int y){
	if(board[y][x] == nobody){
		return (char)(((y*3)+(x+1))+48);
	}else if(board[y][x] == you){
		return 'X';
	}else{
		return 'O';
	}
}


