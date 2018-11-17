#include <iostream>
#include <string>
using namespace std;
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "rand_util.h"
#include "gettext.h"

enum hand{
	rock,
	paper,
	scissor
};

int getsbeatby[3] = 
{hand::scissor,
hand::rock,
hand::paper};
				


int main(void){

	int cpu_choice;
	int ur_choice;
	int wins = 0;
	bool gtg = false;
	string in;
	while(1){
		cpu_choice = i_rand(3);
		while(!gtg){
			cout << "rock, paper, or scissors?: ";
			cin >> in;
			gtg = true;
			if(check_text(in, "rock")){
				ur_choice = hand::rock;
			}else if(check_text(in, "paper")){
				ur_choice = hand::paper;
			}else if(check_text(in, "scissors")||check_text(in, "scissor")){
				ur_choice = hand::scissor;
			}else{
				cout << "'" << in << "'' is not a choice you stupid fuck, try again" << endl;
				gtg = false;
			}
		}
		gtg = false;
		
		cout << "CPU picked ";
		switch(cpu_choice){
			case hand::rock:
				cout << "Rock";
				break;
			case hand::paper:
				cout << "Paper";
				break;
			case hand::scissor:
				cout << "Scissor";
				break;
		}
		cout << endl;
		if(ur_choice == cpu_choice){
			cout << "GG, picked the same shit." << endl << "(draw)" << endl << endl;
		}else if(ur_choice == getsbeatby[cpu_choice]){
			cout << "WELL I PICKED ";
			switch(cpu_choice){
				case hand::rock:
					cout << "ROCK";
					break;
				case hand::paper:
					cout << "PAPER";
					break;
				case hand::scissor:
					cout << "SCISSOR";
					break;
			}
			cout << " YOU FUCKING FAGGOT" << endl; 
			cout << "(you lose)" << endl << endl;
		}else{
			wins++;
			if(wins == 20){
				cout << "Fuck off, I give up" << endl;
				return 0;
			}
			cout << "Fuck you, " << wins << " win" ;
			if(wins > 1){
				cout << "s";
			}
			cout << " doesnt mean anything" << endl;
			cout << "(you win)" << endl << endl;
		}	
	}
	
	return 0;

}
