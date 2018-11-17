#include <SFML/Graphics.hpp>				
#include <iostream>
#include <cmath>
#include "inttostring.h"

	using namespace std;
	
	int iter = 0;
	int iter_max = 128;
	int width = 1280;
	int height = 720;
	int check_off;
	int index = 0;
	
	float iter_scale = (255.0f/(float)iter_max);
	float vux = 0;
	float vlx = 1;
	float dt = 1;
	
	double dist = 0;
	double zoom = 0.01f;
	double kek = 0;
	double kek2 = 0;
	double rl_arr[320];
	double rl = 0;
	double im = 0;
	double rlw = 0;
	double imw = 0;
	double offx = -0.5f;
	double offy = 0;
	
	char * str;
	
	bool slow = false;
	bool first = true;
	bool step;
	
	void handleInput(void);
	void determineViewport(void);
	void sfml_print(const char * text, int x = 0, int y = 0, float xs = 0, float ys = 0);
	void sfml_print(int num = 0, int x = 0, int y = 0, float xs = 0, float ys = 0);
	float dta(float inc);
	
	sf::Uint8* pixels = new sf::Uint8[320*240*4];
	sf::Texture tex;
	sf::Sprite sprite; 
	sf::View port;
	sf::Clock ck;
	sf::Time delta;
	sf::Font font;
	sf::Text txt;
	sf::Event event;
	sf::RenderWindow window(sf::VideoMode(width, height, 32), "ayy lmao");
	
int main(){
	
	ck.restart();
	
	port.reset(sf::FloatRect(0,0,320,240));
	determineViewport();
	port.setViewport(sf::FloatRect(vux,0,vlx,1));
	window.setView(port);
	
	font.loadFromFile("arial.ttf");
	txt.setFont(font);
	tex.create(320, 240); 
	sprite.setTexture(tex);
	
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
		}
		
		delta = ck.getElapsedTime();
		dt = delta.asSeconds();
		ck.restart();
		
		window.clear();
		handleInput();
		
		index = 0;
		for(float i = 0; i < 240; i++){
			im = ((i-120)*zoom)+offy;
			for(float k = 0; k < 320; k++){
				if(i==0){
					rl = rl_arr[(int)k] = ((k-160)*zoom)+offx;
				}else{
					rl = rl_arr[(int)k];
				}
				rlw = rl;
				imw = im;
				while((iter < iter_max)&&(dist < 4.0f)){	
					kek = (rlw*rlw)-(imw*imw);
					imw = (2*rlw*imw)+im;
					rlw = kek+rl;		
					dist = (rlw*rlw)+(imw*imw);
					iter++;
				}
				if(first){
					pixels[index+3] = 255;
				}
				if(iter < iter_max){
					pixels[index] = (sf::Uint8)floor((float)iter * iter_scale);
				}else{
					pixels[index] = 0;
				}
				index += 4;
				dist = 0;
				iter = 0;
			}
		}
		first = false;
		
		tex.update(pixels);
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
	free(text);	
}

void determineViewport(void){
	float w = (float) width;
	float t = (4.0f/3.0f) * (float) height;
	vux = ((w-t)/2.0f)/w;
	vlx = t/w;
	return;
}

void handleInput(void){

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
		if(zoom < .000001){
			zoom = .000001;
		}
		return;
}
