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
	
	
	float vux = 0;
	float vlx = 1;
	float mouse = 0;
	float dt = 1;
	float dist = 0;
	float zoom = 0.01f;
	float kek;
	float kek2;
	int iter = 0;
	int check_off;
	int index = 0;
	int in = 0;
	char * str;
	double rl = 0;
	double im = 0;
	double rlw = 0;
	double imw = 0;
	double offx = 0;
	double offy = 0;
	int mt;
	int ht;
	int width = 1280;
	int height = 720;
	bool slow = false;
	bool click1;
	bool click2;
	bool clicked1;
	bool clicked2;
	bool step;
	void handleInput();
	void determineViewport(void);
	void sfml_print(const char * text, int x = 0, int y = 0, float xs = 0, float ys = 0);
	void sfml_print(int num = 0, int x = 0, int y = 0, float xs = 0, float ys = 0);
	float dta(float inc);
	sf::View port;
	sf::Clock ck;
	sf::Time delta;
	sf::Font font;
	sf::Text txt;
	sf::Event event;
	sf::RenderWindow window(sf::VideoMode(width, height, 32), "ayy lmao");
	sf::RectangleShape cell;
	sf::Vector2i m_pos;
	sf::Uint8* pixels = new sf::Uint8[320*240*4];
	sf::Texture tex;
	sf::Sprite sprite; 
	
int main(){
	
	ck.restart();
	//window.setVerticalSyncEnabled(true);
	cell.setPosition(sf::Vector2f(0,0));
	cell.setSize(sf::Vector2f(1,1));
	port.reset(sf::FloatRect(0,0,320,240));
	determineViewport();
	port.setViewport(sf::FloatRect(vux,0,vlx,1));
	window.setView(port);
	//init text
	font.loadFromFile("arial.ttf");
	txt.setFont(font);
	tex.create(320, 240); 
	sprite.setTexture(tex);
	
	//init text done
	while(window.isOpen()){
		
		
		
		
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed)
				window.close();
			if(event.type == sf::Event::Resized){
				width = event.size.width;
				height = event.size.height;	
				determineViewport();
				port.setViewport(sf::FloatRect(vux,0,vlx,1));
				window.setView(port);
			}				
			if(event.type == sf::Event::MouseWheelScrolled)
				mouse = event.mouseWheelScroll.delta;
		}
		
		
		delta = ck.getElapsedTime();
		dt = delta.asSeconds();
		ck.restart();
		mt = floor(dt/60);
		ht = floor(mt/60);
		window.clear();
		handleInput();
		if(zoom < .00000001){
			zoom = .00000001;
		}
		
		index = 0;
		for(float i = 0; i < 240; i++){
			im = ((i-120)*zoom)+offy;
			for(float k = 0; k < 320; k++){
				rl = ((k-160)*zoom)+offx;
				rlw = rl;
				imw = im;
				while((iter < 200)&&(dist < 4.0f)){	
					kek = (rlw*rlw)-(imw*imw);
					//kek2 = (imw*imw);
					imw = (2*rlw*imw)+im;
					rlw = kek+rl;		
					dist = (rlw*rlw)+(imw*imw);
					iter++;
				}
				
				if(iter < 200){
					iter = floor(iter * 2.55f);
					pixels[index] = (sf::Uint8)iter;
					pixels[index+3] = 255;
					
				}else{
					pixels[index+3] = 0;
					in++;
				}
				
				index += 4;
				dist = 0;
				iter = 0;
				
			}
		}
		
		tex.update(pixels);
		sprite.setTexture(tex);
		window.draw(sprite);
		check_off = floor(offx*1000000);
		sfml_print("offset x:", 10, 10, .25, .25);
		sfml_print(check_off, 40, 10, .25, .25);
		check_off = floor(offy*1000000);
		sfml_print("offset y:", 10, 20, .25, .25);
		sfml_print(check_off, 40, 20, .25, .25);
		
		window.display();
		
	}


	return EXIT_SUCCESS;
	
}


float dta(float inc){
	
	return inc * dt;
	
}

void sfml_print(const char * text, int x = 0, int y = 0, float xs = 0, float ys = 0){
	
	txt.setPosition(x,y);
	txt.setString(text);
	
	if(xs > 0){
		if(ys > 0){
			txt.setScale(xs,ys);	
		}else{
			txt.setScale(xs,1);
		}
	}else{
		if(ys > 0){
			txt.setScale(1,ys);
		}else{
			txt.setScale(1,1);
		}
	}
		
	window.draw(txt);
}

void sfml_print(int num = 0, int x = 0, int y = 0, float xs = 0, float ys = 0){
	
	char * text = slow_itos(num);
	txt.setPosition(x,y);
	txt.setString(text);
	if(xs > 0){
		if(ys > 0){
			txt.setScale(xs,ys);	
		}else{
			txt.setScale(xs,1);
		}
	}else{
		if(ys > 0){
			txt.setScale(1,ys);
		}else{
			txt.setScale(1,1);
		}
	}
	window.draw(txt);
	
}

void determineViewport(void){
	
	float w = (float) width;
	float t = (4.0f/3.0f) * (float) height;
	vux = ((w-t)/2.0f)/w;
	vlx = t/w;
	return;
	
}

void handleInput(){
	
		slow = false;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
			window.close();
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
			slow = true;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
			if(slow){
				offx += dta(0.001);
			}else{
				offx += dta(0.1);
			}
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
			if(slow){
				offx -= dta(0.001);
			}else{
				offx -= dta(0.1);
			}
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
			if(slow){
				offy -= dta(0.001);
			}else{
				offy -= dta(0.1);
			}
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
			if(slow){
				offy += dta(0.001);
			}else{
				offy += dta(0.1);
			}
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
			
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
			
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
			zoom -= dta(0.001);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
			zoom += dta(0.001);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
			zoom -= dta(0.00001);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
			zoom += dta(0.00001);
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
