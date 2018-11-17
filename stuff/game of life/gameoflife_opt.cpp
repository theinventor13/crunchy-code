#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>				
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include "inttostring.h"
#define PI 3.1415926535

	using namespace std;
	struct cell_pos{
		int x;
		int y;
	};
	sf::Vector2i m_pos;
	float mouse;
	float dt;
	int mt;
	int ht;
	int width = 1280;
	int height = 720;
	
	int tx = 5;
	int ty = 5;
	int gx = 258;//going to be witdh and height divided by tx and ty floored +2
	int gy = 146;//(+2 for borders to avoid edge checking)
	//rules set to conway by default
	int nlb = 2;//neighbor lower bound
	int nub = 3;//neighbor upper bound
	int nrc = 3;//neighbor revive count
	float avg_n = 0;//average neighbors
	int oca = 0;//old cells alive
	int nca = 0;//new cells alive
	char * old_grid;
	char * new_grid;
	char * hold_grid;
	
	
	bool click1;
	bool click2;
	bool clicked1;
	bool clicked2;
	bool step;
	void handleInput(void);
	void handle_grid(char * grid);
	void clear_grid(char * grid);
	void step_sim(char * ogrid, char * ngrid);
	void set_strings(void);
	void flip(char * grid, int x, int y);
	sf::Clock ck;
	sf::Time delta;
	sf::Font font;
	sf::Text cells_a;//cells alive
	sf::Text cells_d;//cells dead
	char * c_cells_a;//cells alive
	char * c_cells_d;//cells dead
	sf::Text l_cells_a;//cells alive (label)
	sf::Text l_cells_d;//cells dead (label) 
	sf::Event event;
	sf::RenderWindow window(sf::VideoMode(width, height, 32), "ayy lmao");
	sf::RectangleShape cell;
	
int main(){
	
	ck.restart();
	window.setVerticalSyncEnabled(true);
	cell.setPosition(sf::Vector2f(0,0));
	cell.setSize(sf::Vector2f(tx,ty));
	cell.setFillColor(sf::Color::White);
	set_strings();
	old_grid = (char *)malloc(gx*gy);
	new_grid = (char *)malloc(gx*gy);
	clear_grid(old_grid);
	clear_grid(new_grid);
	while(window.isOpen()){
		
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed)
				window.close();
			if(event.type == sf::Event::MouseWheelScrolled)
				mouse = event.mouseWheelScroll.delta;
		}
		
		
		delta = ck.getElapsedTime();
		dt = delta.asSeconds();
		mt = floor(dt/60);
		ht = floor(mt/60);
		window.clear();
		
		handleInput();
		handle_grid(old_grid);
		
		if(clicked2 || step){
			step_sim(old_grid, new_grid);
			hold_grid = new_grid;
			new_grid = old_grid;
			old_grid = hold_grid;
			clear_grid(new_grid);
			if(step){
				sf::sleep(sf::milliseconds(100));
			}
		}
		
		for(int i = 1; i < gy-1; i++){
			for(int k = 1; k < gx-1; k++){	
				if(old_grid[(i*gx)+k] > 0){
					cell.setPosition(sf::Vector2f((k-1)*tx,(i-1)*ty));
					window.draw(cell);
					
				}
			}
		}
		
		//change this disgusting shit
		c_cells_a = slow_itos(nca);
		c_cells_d = slow_itos(((gx-2)*(gy-2))-nca);
	
		cells_a.setString(c_cells_a);
		cells_d.setString(c_cells_d);
	
		window.draw(l_cells_a);
		window.draw(l_cells_d);

		window.draw(cells_a);
		window.draw(cells_d);

		
		window.display();
		
	}
		
	//for(int i = 0; i < gx*gy; i++){
		//if(old_grid[i] > 0)
		//	cout << i << " = " << old_grid[i] << ":";
	//}
	free(old_grid);
	free(new_grid);
	return EXIT_SUCCESS;
	
}


float dta(float inc){
	
	return inc * dt;
	
}

void flip(char * grid, int x, int y){
	
	if(grid[(y*gx)+x] == 0){
		grid[(y*gx)+x] = 1;	
		nca++;
	}else{
		grid[(y*gx)+x] = 0;
		nca--;
	}
	
	return;	
	
}

void handle_grid(char * grid){
	
	if(clicked1){
		flip(grid, floor((float)m_pos.x/(float)tx)+1, floor((float)m_pos.y/(float)ty)+1);
	}
	
}

void clear_grid(char * grid){
	for(int i = 0;i < gy; i++){
		for(int k = 0;k < gx; k++){
			grid[(i*gx)+k] = 0;
		}
	}
}

void step_sim(char * ogrid, char * ngrid){
	int n = 0;
	avg_n = 0;//average neighbors
	nca = 0;//new cells alive
	oca = 0;//old cells alive
	
	//cout << "here" << endl;
	for(int i = 1; i < gy-1; i++){
		for(int k = 1; k < gx-1; k++){
			for(int s = -1; s < 2; s++){
				if(s != 0){
					n += (int)ogrid[((i+s)*gx)+k];	
				}
				n += (int)ogrid[((i+s)*gx)+(k-1)];
				n += (int)ogrid[((i+s)*gx)+(k+1)];
			}
				
			if((n >= nlb) && (n <= nub) && (ogrid[(i*gx)+k] == 1)){//if inside neighbor bounds keep alive
//					if(n > 0){
//						cout << "(" << k << "," << i << ") with " << n << "neighbors killt" << endl;
//					}
				ngrid[(i*gx)+k] = 1;	
				nca++; 	
			}
			if((ogrid[(i*gx)+k] == 0) && (n == nrc)){//if (nrc) neighbors revive
				ngrid[(i*gx)+k] = 1;
				nca++;
			}
			if(ogrid[(i*gx)+k] == 1){
				oca++;
			}
			
			n = 0;//reset neighbor count
								
		}
	}
	
}

void set_strings(void){
	
	font.loadFromFile("arial.ttf");
	cells_a.setFont(font);
	cells_d.setFont(font);
	l_cells_a.setString("CELLS ALIVE:");
	l_cells_a.setFont(font);
	l_cells_d.setString("CELLS DEAD:");
	l_cells_d.setFont(font);
	cells_a.setPosition(230,10);
	cells_d.setPosition(230,50);
	l_cells_a.setPosition(10,10);
	l_cells_d.setPosition(10,50);

	return;
}
void handleInput(){
		step = false;
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
			window.close();
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
			step = true;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
			
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
			
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
		
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
			
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
			
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
			
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
			
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
			
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
			
		}
		if(mouse != 0){
			
		}
		
		if(click1 && !sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			
			clicked1 = true;
			
		}else{
		
			clicked1 = false;
				
		}
		if(click2 && !sf::Mouse::isButtonPressed(sf::Mouse::Right)){
			
			clicked2 = true;
			
		}else{
		
			clicked2 = false;
				
		}
		m_pos = sf::Mouse::getPosition(window);
		click1 = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		click2 = sf::Mouse::isButtonPressed(sf::Mouse::Right);
		
		return;
		
}
