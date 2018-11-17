#include <iostream>
#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "rand_util.h"
#include "inttostring.h"
#include "tets.h"
#define MIN_MAP_X 4
#define MIN_MAP_Y 4
#define MAX_MAP_X 256
#define MAX_MAP_Y 256//this is arbitrary, feel free to set this to ludacris values
//TODO: make these defines constant variables

using namespace std;

typedef sf::Vector2f v2f;
sf::Event event;
sf::RenderWindow window;
sf::RectangleShape pix;
void checkrows(int rf, int rs);
void handleInput(void);
void updateGame(void);
void drawGame(void);
int rs = 30;
int map_x = 10;
int map_y = 28;
int res_x = map_x * rs;
int res_y = map_y * rs;
bool placed = true;
bool turn = false;
bool direction = true;
bool moveright = false;
bool moveleft = false;
bool fall = false;
bool gameover = false;
unsigned char piece = 1;
v2f pix_size = v2f((float)rs, (float)rs);

//0 reserved for blank space
enum tet_type{
	box = 1,
	line,
	tee,
	l,
	r_l,
	s,
	r_s = 7
};
//if you want to define pentominos or some shit, start at 8
struct poly_grid{
	unsigned int rows;
	unsigned int cols;
	char * map;
	bool init(int grid_x, int grid_y){
		if(grid_x >= MIN_MAP_X && grid_x <= MAX_MAP_X && grid_y >= MIN_MAP_Y && grid_y <= MAX_MAP_Y){
			rows = grid_y;
			cols = grid_x;
			map = new char[rows*cols];
			if(map == NULL){
				cout << "could not allocate map in heap" << endl;
				return false;
			}else{
				for(int gg = 0; gg < rows*cols;gg++){
					map[gg] = 0;
				}
				return true;
			}
		}else{
			cout << "grid dimensions not within defined bounds" << endl;
			return false;
		}
	}
	
	void draw(void){
		for(int j = 0; j < map_y; j++){
			for(int k = 0; k < map_x; k++){
				if(map[(j*map_x)+k] > 0){
					pix.setPosition(float(k * rs), float(j * rs));
					window.draw(pix);
				}
			}
		}
		return;
	}
	
}grid;

struct poly{
	unsigned int x;
	unsigned int y;
	unsigned char size;
	unsigned char rots;
	unsigned char frame;
	unsigned char type;
	bool * bitmap;
	void init(bool * data, unsigned char s, unsigned char r, unsigned char t){
		size = s;
		rots = r;
		type = t;
		frame = 0;
		bitmap = new bool [size*size*rots];
		for(unsigned char j = 0;j < size*size*rots;j++){
			bitmap[j] = data[j];
		}
		return;
	}
	
	void draw(void){
		for(unsigned char j = 0; j < size;j++){
			for(unsigned char k = 0; k < size;k++){
				if(bitmap[(frame*size*size)+((j*size)+k)]){
					pix.setPosition((x+k)*rs,(y+j)*rs);
					window.draw(pix);
				}
			}
		}
		return;
	}
	
	void rotate(bool dir){
		if(dir){
			if(frame < rots - 1){
				frame++;
			}else{
				frame = 0;
			}
		}else{
			if(frame > 0){
				frame--;
			}else{
				frame = rots - 1;
			}
		}
	}
	
	bool collide(poly_grid& check){
		for(unsigned int i = 0;i < size;i++){
			for(unsigned int k = 0;k < size;k++){
				if(((x+k) >= check.cols) || ((x+k) < 0) || ((y+i) >= check.rows)){
					if(bitmap[(frame*size*size)+((i*size)+k)]){
						return true;
					}
				}else{
					if(check.map[((i+y)*check.cols)+(k+x)]&&bitmap[(frame*size*size)+((i*size)+k)]){
						return true;
					}
				}
			}
		}
		return false;
	}
	
	void addtomap(poly_grid& add){
		for(unsigned int i = 0;i < size;i++){
			for(unsigned int k = 0;k < size;k++){
				if(bitmap[(frame*size*size)+((i*size)+k)]){
					add.map[((i+y)*add.cols)+(k+x)] = type;
				}
			}
		}
		return;
	}
	
}tet[7];



//#include "auxillary.h"

bool init(void){
	//init start
	//spawn polys
	tet[0].init(t_box, (unsigned char)2, (unsigned char)1, (unsigned char)tet_type::box);
	tet[1].init(t_line, (unsigned char)4, (unsigned char)2, (unsigned char)tet_type::line);
	tet[2].init(t_tee, (unsigned char)3, (unsigned char)4, (unsigned char)tet_type::tee);
	tet[3].init(t_l, (unsigned char)3, (unsigned char)4, (unsigned char)tet_type::l);
	tet[4].init(t_r_l, (unsigned char)3, (unsigned char)4, (unsigned char)tet_type::r_l);
	tet[5].init(t_s, (unsigned char)3, (unsigned char)2, (unsigned char)tet_type::s);
	tet[6].init(t_r_s, (unsigned char)3, (unsigned char)2, (unsigned char)tet_type::r_s);//sorry for the typecasting, compiler wants to be a bitch about it
	grid.init(map_x, map_y);
	//end spawn polys
	//set pixel params
	pix.setSize(pix_size);
	pix.setFillColor(sf::Color::Black);
	//end set pixel params
	window.create(sf::VideoMode(res_x, res_y, 32), "Kektris", sf::Style::Close);//implement resolution solution
	window.setMouseCursorVisible(false);//lol 
	window.setVerticalSyncEnabled(true);
	//init end
	srand(time(NULL));
	return true;
}

int main(void){

	if(!init()){
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
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down)){//change to one hit one move
				fall = true;
			}else{
				fall = false;
			}
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Right)){
				moveright = true;
			}else{
				moveright = false;
			}
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Left)){
				moveleft = true;
			}else{
				moveleft = false;
			}
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space)){
				turn = true;
			}
			
		}
		
		window.clear(sf::Color::White);
		
		//handleInput();
		updateGame();
		if(gameover){
			cout << "GAME OVER" << endl;
			return EXIT_SUCCESS;
		}
		drawGame();
		
		window.display();
		
	}
	
	return EXIT_SUCCESS;
	
}

void checkrows(poly_grid & blast, int rf, int rs){
	bool kill;
	for(int i = rf; i < rf+rs; i++){
		kill = true;
		for(int k = 0; k < map_x; k++){
			if(!(blast.map[(i*map_x)+k])){
				kill = false;
				break;
			}
		}
		if(kill){
			if(!i){
				gameover = true;
				return;
			}
			//play animation or something here to show the rows disappearing
			for(int j = i;j > 0;j--){
				for(int k = 0; k < map_x; k++){
					blast.map[(j*map_x)+k] = blast.map[((j-1)*map_x)+k];
				}
			}
		}
	}
	return;
}

void updateGame(void){
	
	if(placed){
		
		piece = (unsigned char)i_rand(6);//change so next piece is prepicked and displayed ahead of time
		tet[piece].x = (map_x - tet[piece].size)/2;//center piece on x axis
		tet[piece].y = 0;//put at top
		tet[piece].frame = 0;//set to unrotated tet
		placed = false;//reset bool
		
	}else{
		
		//cout << turn << endl;
		if(turn){
			
			tet[piece].rotate(direction);
			if(tet[piece].collide(grid)){
				tet[piece].rotate(!direction);//optimize collison checks
			}
			turn = false;	
			
		}
		
		if(moveright){
			tet[piece].x++;
			if(tet[piece].collide(grid)){
				tet[piece].x--;
			}
		}
		
		if(moveleft){
			tet[piece].x--;
			if(tet[piece].collide(grid)){
				tet[piece].x++;
			}
		}
		
		if(fall){
			tet[piece].y++;
			if(tet[piece].collide(grid)){
				//cout << "here" << endl;
				if(tet[piece].y == 1){//change to check top row of grid
					gameover = true;
					return;
				}
				tet[piece].y--;
				tet[piece].addtomap(grid);
				checkrows(grid, tet[piece].y,tet[piece].size);
				if(gameover){
					return;
				}
				placed = true;
			}
		}
		
		
		
	}

	return;	
}

void drawGame(void){
	
	grid.draw();
		
	if(!placed){
		tet[piece].draw();
	}
	
	return;
}

