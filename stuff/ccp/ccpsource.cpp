#include <iostream>
#include <cmath>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "inttostring.h"
using namespace std;

typedef sf::Vector2f v2f;
sf::Event event;
sf::RenderWindow window;
sf::Vector2i mvec;
sf::CircleShape orb;
v2f p;
v2f center;
float radius;
void handleInput(void);
bool ccp(float cx, float cy, float r, float px, float py);//circle collide point
bool coll = true;
int res_x = 1000;
int res_y = 600;

#include "auxillary.h"

bool init(void){
	//init start
	radius = 50.0f;
	center = v2f(100,100);
	p = v2f(200,200);
	orb.setFillColor(sf::Color(0,0,0,0));
	orb.setOutlineThickness(1.0f);
	window.create(sf::VideoMode(res_x, res_y, 32), "Circle and Point Collision Demo", sf::Style::Close);
	window.setMouseCursorVisible(false);//lol
	start_sfml_print();
	//init end
	return true;
}

int main(void){

	init();
	
	while(window.isOpen()){
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			}
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)){
				window.close();
			}	
		}
		window.clear(sf::Color::White);
		sfml_print("Hold down either A or S and then move the mouse",20.0f,10.0f,sf::Color::Black);
		//draw circle
		orb.setOutlineColor(sf::Color::Black);
		orb.setRadius(radius);
		orb.setOrigin(v2f(radius,radius)); 
		orb.setPosition(center);
		window.draw(orb);
		//draw center
		if(coll){
			orb.setOutlineColor(sf::Color::Red);	
		}else{
			orb.setOutlineColor(sf::Color::Blue);
		}
		orb.setRadius(4);
		orb.setOrigin(v2f(4,4));
		window.draw(orb);
		//draw line seg points
		orb.setOutlineColor(sf::Color::Black);
		orb.setPosition(p);
		window.draw(orb);
		//is point colliding
		coll = ccp(center.x, center.y, radius, p.x, p.y);	
		handleInput();
		window.display();
	}
	
	return EXIT_SUCCESS;
	
}

bool ccp(float cx, float cy, float r, float px, float py){
	return ((px - cx) * (px - cx) + (py - cy) * (py - cy) < (r * r));//cause pythagoras (recognize a^2+b^2?)
}

void handleInput(void){
	mvec = sf::Mouse::getPosition(window) - mvec;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		p += v2f(mvec);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
		center += v2f(mvec);
	}
	mvec = sf::Vector2i(res_x/2,res_y/2);
	if(window.hasFocus()){
		sf::Mouse::setPosition(mvec,window);
	}
	return;
}
