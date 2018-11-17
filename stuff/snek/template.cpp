#include <iostream>
#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "inttostring.h"
#include "rand_util.h"
using namespace std;

typedef sf::Vector2f v2f;
typedef sf::Vector2i v2i;
sf::RectangleShape seg;
sf::Event event;
sf::RenderWindow window;
void handleInput(void);
void updateGame(void);
void drawGame(void);
int map_x = 50;
int map_y = 50;
int res_x = 500;
int res_y = 500;
v2i ps(res_x/map_x,res_y/map_y);
v2i food;
bool food_ate = true;
bool game_over = false;
char button = 0;
char slow = 0;

struct snek;

snek * tail;//dev cpp being bitchy about static vars in structs so i externed it
int snek_count;

struct snek{
	bool head;
	char dir;
	v2i pos;
	snek * next;
	
	snek(bool h = false){
		if(h){
			head = true;
			tail = this;
		}else{
			head = false;
		}
		dir = 0;
		pos = v2i(map_x/2, map_y/2);
		next = NULL;
	}
	
	void add_snek(void){
		tail->next = new snek;
		
		(tail->next)->dir = tail->dir;
		(tail->next)->pos = tail->pos;
		switch(tail->dir){
			case 0:
				(tail->next)->pos.y--;
				break;
			case 1:
				(tail->next)->pos.x--;
				break;
			case 2:
				(tail->next)->pos.y++;
				break;
			case 3:
				(tail->next)->pos.x++;
				break;
			default:
				cout << "wtf" << endl;
				break;
		}
		tail = tail->next;
		tail->next = NULL;
		return;
	}
	
	void step_snek(void){
		snek * iter = this;
		while(iter != NULL){
			switch(iter->dir){
				case 0:
					iter->pos.y++;
					break;
				case 1:
					iter->pos.x++;
					break;
				case 2:
					iter->pos.y--;
					break;
				case 3:
					iter->pos.x--;
					break;
				default:
					cout << "wtf" << endl;
			}
			iter = iter->next;
		}
		return;
	}
	
	void push_back(char newt = 4){
		if(next != NULL){
			next->push_back(dir);
			if(newt < 4){
				dir = newt;	
			}
		}else{
			if(!head){
				dir = newt;	
			}
		}
		return;
	}
	
	bool check_collision(void){
		if((pos.x < 0)||(pos.y < 0)||(pos.x >= map_x)||(pos.y >= map_y)){
			return true;
		}
		snek * iter = next;
		while(iter != NULL){
			if(pos == (iter)->pos){
				return true;
			}else{
				iter = iter->next;
			}
		}
		return false;
	}
	
	void update_snek(char which){
		dir = which?(which-1):dir;
		step_snek();
		if(check_collision()){
			game_over = true;
			return;
		}
		
		if(pos == food){
			add_snek();
			food_ate = true;
		}
		push_back();
		return;
	}
	
	void draw_snek(void){
		snek * iter = this;
		while(iter != NULL){
			seg.setPosition(iter->pos.x*ps.x, iter->pos.y*ps.y);
			window.draw(seg);
			iter = iter->next;
		}
		return;
	}
};

snek * iSnek = new snek(true);

#include "auxillary.h"
bool init(void){
	//init start
	
	seg.setFillColor(sf::Color::Black);
	seg.setSize(v2f(ps));
	window.create(sf::VideoMode(res_x, res_y, 32), "Snek", sf::Style::Close);//implement resolution solution
	window.setMouseCursorVisible(false);//lol 
	window.setVerticalSyncEnabled(true);
	start_sfml_print();//load font to use for sfml_print and start console
	srand(time(NULL));
	//init end
	return true;
}

int main(void){

	if(!init()){
		error("main: init failed");
		return EXIT_FAILURE;
	}
	
	while(window.isOpen()){
		
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			}
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)){
				window.close();
			}	
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Up)){
				button = 3;
			}
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down)){
				button = 1;
			}	
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Left)){
				button = 4;
			}	
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Right)){
				button = 2;
			}	
		}
		
		if(!slow){
			updateGame();	
			slow++;
		}else{
			slow++;
			if(slow > 4){
				slow = 0;
			}
		}
		
		if(game_over){
			cout << "GAME OVER" << endl;
			return EXIT_SUCCESS;
		}
		drawGame();


	}
	
	return EXIT_SUCCESS;
	
}



void updateGame(void){
	iSnek->update_snek(button);
	button = 0;
	if(game_over){
		return;
	}
	if(food_ate){
		food = v2i(i_rand(map_x),i_rand(map_y));
		food_ate = false;
	}
	return;
}

void drawGame(void){
	window.clear(sf::Color::White);
	iSnek->draw_snek();
	seg.setPosition(food.x*ps.x, food.y*ps.y);
	window.draw(seg);
	window.display();
	return;
}
