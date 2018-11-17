#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>				
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
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
	
	int tx = 10;
	int ty = 10;
	int gx = 128;
	int gy = 72;
	//rules set to conway by default
	int nlb = 2;//neighbor lower bound
	int nub = 3;//neighbor upper bound
	int nrc = 3;//neighbor revive count
	char * old_grid;
	char * new_grid;
	char * hold_grid;
	
	
	bool click1;
	bool click2;
	bool clicked1;
	bool clicked2;
	bool step;
	void handleInput();
	void handle_grid(char * grid);
	void clear_grid(char * grid);
	void step_sim(char * ogrid, char * ngrid);
	void flip(char * grid, int x, int y);
	sf::Clock ck;
	sf::Time delta;
	sf::Font font;
	sf::Text cells_alive;
	sf::Text cells_dead;
	sf::Text avg_n;//average neighbors	
	sf::Event event;
	sf::RenderWindow window(sf::VideoMode(width, height, 32), "ayy lmao");
	sf::RectangleShape cell;
	
int main(){
	
	ck.restart();
	window.setVerticalSyncEnabled(true);
	cell.setPosition(sf::Vector2f(0,0));
	cell.setSize(sf::Vector2f(tx,ty));
	old_grid = (char *)malloc(gx*gy);
	new_grid = (char *)malloc(gx*gy);
	clear_grid(old_grid);
	clear_grid(new_grid);
	bool check[8] = {false, true, false, false, false, false, true, true};
	while(window.isOpen()){
		
		font.loadFromFile("arial.ttf");
		
		
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
		if(clicked2){
			step_sim(old_grid, new_grid);
			hold_grid = new_grid;
			new_grid = old_grid;
			old_grid = hold_grid;
			clear_grid(new_grid);
		}
		
		for(int i = 0; i < gy; i++){
			for(int k = 0; k < gx; k++){
				cell.setPosition(sf::Vector2f(k*tx,i*ty));
				if(old_grid[(i*gx)+k] > 0){
					cell.setFillColor(sf::Color::White);
				}else{
					cell.setFillColor(sf::Color::Black);
				}
				window.draw(cell);
			}
		}
		window.display();
		
	}
		
	
//	cout << *(old_grid) << endl;		
//	cout << *(old_grid+1) << endl;
//	cout << *(old_grid+2) << endl;
//	cout << *(old_grid+3) << endl;
//	cout << *(old_grid+4) << endl;
//	cout << *(old_grid+5) << endl;
//	cout << *(new_grid) << endl;		
//	cout << *(new_grid+1) << endl;
//	cout << *(new_grid+2) << endl;
//	cout << *(new_grid+3) << endl;
//	cout << *(new_grid+4) << endl;
//	cout << *(new_grid+5) << endl;	
	//cout << endl << sizeof(char);
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
	}else{
		grid[(y*gx)+x] = 0;
	}
		
	
}

void handle_grid(char * grid){
	
	if(clicked1){
		flip(grid, floor((float)m_pos.x/(float)tx), floor((float)m_pos.y/(float)ty));
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
	//cout << "here" << endl;
	for(int i = 0; i < gy; i++){
		for(int k = 0; k < gx; k++){
//			if(ogrid[(i*gx)+k] > 0)
//				cout << "doing: (" << k << "," << i << ")" << endl << endl; 
			for(int s = -1; s < 2; s++){
			//check neighbor count
			//todo: optimize so edges are done in special loop so
			//you dont have to do conditional check for every cell 
			//and neighbors
				if(((i + s) >= 0)&&((i + s) < gy)){
//					if(ogrid[(i*gx)+k] > 0)
//						cout << (i+s) << endl;
					if(s != 0){
						n += (int)ogrid[((i+s)*gx)+k];	
//						if(ogrid[(i*gx)+k] > 0)
//							cout << "(" << k << "," << (i+s) << "): " << ogrid[((i+s)*gx)+k] << endl;
					}
					if((k-1) >= 0){
						n += (int)ogrid[((i+s)*gx)+(k-1)];
//						if(ogrid[(i*gx)+k] > 0)
//							cout << "(" << (k-1) << "," << (i+s) << "): " << ogrid[((i+s)*gx)+(k-1)] << endl;
					}
					if((k+1) < gx){
						n += (int)ogrid[((i+s)*gx)+(k+1)];
//						if(ogrid[(i*gx)+k] > 0)
//							cout << "(" << (k+1) << "," << (i+s) << "): " << ogrid[((i+s)*gx)+(k+1)] << endl;
					}
				}
				
			}
//			if(ogrid[(i*gx)+k] > 0)
//				cout << endl;
				
				if((n >= nlb) && (n <= nub) && (ogrid[(i*gx)+k] == 1)){//if inside neighbor bounds keep alive
//					if(n > 0){
//						cout << "(" << k << "," << i << ") with " << n << "neighbors killt" << endl;
//					}
					ngrid[(i*gx)+k] = 1;	 	
				}
				if((ogrid[(i*gx)+k] == 0) && (n == nrc)){//if (nrc) neighbors revive
					ngrid[(i*gx)+k] = 1;
				}
				n = 0;//reset neighbor count
								
		}
	}
	
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
