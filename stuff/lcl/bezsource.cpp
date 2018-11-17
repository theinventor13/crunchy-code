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
sf::ConvexShape line(4);
v2f p1;
v2f p2;
v2f p3;
v2f p4;
v2f center;
v2f collpoint;
float radius;
void handleInput(void);
bool lcl(v2f l1p1,v2f l1p2,v2f l2p1,v2f l2p2);//line collide line
bool lclwr(v2f l1p1,v2f l1p2,v2f l2p1,v2f l2p2);//line collide line with return
bool coll = true;
//bool change = true;
//bool test = true;
int res_x = 1000;
int res_y = 600;
#include "auxillary.h"
void draw_line(v2f v1, v2f v2){
	//this is super shit lmao
	line.setPoint(0, v1);
	line.setPoint(1, v1);
	line.setPoint(2, v2);
	line.setPoint(3, v2);
	window.draw(line);
	return;
} 

bool init(void){
	//init start
	radius = 50.0f;
	center = v2f(100,100);
	p1 = v2f(0,0);
	p2 = v2f(200,200);
	p3 = v2f(0,200);
	p4 = v2f(200,0);
	orb.setRadius(4);
	orb.setOrigin(v2f(4,4));
	orb.setFillColor(sf::Color(0,0,0,0));
	orb.setOutlineThickness(1.0f);
	line.setOutlineThickness(.5f);
	line.setOutlineColor(sf::Color::Black);
	line.setFillColor(sf::Color::Black);
	window.create(sf::VideoMode(res_x, res_y, 32), "Line Segment Collision Demo", sf::Style::Close);
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
		//draw circle
		sfml_print("Hold down either A, S, D, or F, and then move the mouse",20.0f,10.0f,sf::Color::Black);

		//draw center
		if(coll){
			orb.setOutlineColor(sf::Color::Cyan);	
		}else{
			orb.setOutlineColor(sf::Color::Blue);
		}
		
		window.draw(orb);
		//draw line seg points
		if(coll){
			orb.setOutlineColor(sf::Color::Cyan);	
		}else{
			orb.setOutlineColor(sf::Color::Black);
		}
		
		
		orb.setPosition(collpoint);
		window.draw(orb);
		orb.setOutlineColor(sf::Color::Red);
		orb.setPosition(p1);
		window.draw(orb);
		orb.setOutlineColor(sf::Color::Blue);
		orb.setPosition(p2);
		window.draw(orb);
		orb.setOutlineColor(sf::Color::Green);
		orb.setPosition(p3);
		window.draw(orb);
		orb.setOutlineColor(sf::Color::Yellow);
		orb.setPosition(p4);
		window.draw(orb);
		draw_line(p1,p2);
		draw_line(p3,p4);
//		if(change&&test){
		coll = lclwr(p1, p2, p3, p4);	
//		}else if(!test){
//			coll = ccl(center, radius, p1, p2);
//		}
		handleInput();
		window.display();
	}
	
	return EXIT_SUCCESS;
	
}

bool lcl(v2f l1p1,v2f l1p2,v2f l2p1,v2f l2p2){
	if((l1p1 == l2p1)||(l1p1 == l2p2)||(l1p2 == l2p1)||(l1p2 == l2p2)){// if 2 of the points are equal then collision
		return true;
	}
	//fix degen case
	//equal endpoint lines
	bool vt1 = false;
	bool vt2 = false;
	float m1 = (l1p1.x - l1p2.x);
	if(m1){//if run is not zero, calc slope
		m1 = (l1p1.y - l1p2.y) / m1;	
	}else{//else deal with vertical line degenerate case
		vt1 = true;
	}
	float m2 = (l2p1.x - l2p2.x);//ditto here
	if(m2){
		m2 = (l2p1.y - l2p2.y) / m2;
	}else{
		vt2 = true;
	}
	if(vt1 && vt2){//if both vertical 
		return (l1p1.x && l2p1.x);//return true if they are coplanar, else false
	}
	//if both lines straddle each others halfplanes return true
	float b1 = -m1 * l1p1.x + l1p1.y;
	if(vt2){
		return (((l2p1.y > m1 * l2p1.x + b1) ^ (l2p2.y > m1 * l2p2.x + b1)) && ((l1p1.x < l2p1.x) ^ (l1p2.x < l2p1.x)));
	}
	float b2 = -m2 * l2p1.x + l2p1.y;
	if(vt1){
		return (((l1p1.y > m2 * l1p1.x + b2) ^ (l1p2.y > m2 * l1p2.x + b2)) && ((l2p1.x < l1p1.x) ^ (l2p2.x < l1p1.x)));
	}
	return (((l2p1.y > m1 * l2p1.x + b1) ^ (l2p2.y > m1 * l2p2.x + b1))
			&&((l1p1.y > m2 * l1p1.x + b2) ^ (l1p2.y > m2 * l1p2.x + b2)));
}
bool lclwr(v2f l1p1,v2f l1p2,v2f l2p1,v2f l2p2){
	
	if(l1p1 == l2p1){
		collpoint = l1p1;
		return true;
	}
	if(l1p1 == l2p2){
		collpoint = l1p1;
		return true;
	}
	if(l1p2 == l2p1){
		collpoint = l1p2;
		return true;
	}
	if(l1p2 == l2p2){
		collpoint = l1p2;
		return true;
	}
	
	//fix degen case
	//equal endpoint lines
	bool vt1 = false;
	bool vt2 = false;
	float m1 = (l1p1.x - l1p2.x);
	if(m1){//if run is not zero, calc slope
		m1 = (l1p1.y - l1p2.y) / m1;	
	}else{//else deal with vertical line degenerate case
		vt1 = true;
	}
	float m2 = (l2p1.x - l2p2.x);//ditto here
	if(m2){
		m2 = (l2p1.y - l2p2.y) / m2;
	}else{
		vt2 = true;
	}
	
	
	float b1 = -m1 * l1p1.x + l1p1.y;
	if(vt2){
		return (((l2p1.y > m1 * l2p1.x + b1) ^ (l2p2.y > m1 * l2p2.x + b1)) && ((l1p1.x < l2p1.x) ^ (l1p2.x < l2p1.x)));
	}
	float b2 = -m2 * l2p1.x + l2p1.y;
	if(vt1){
		return (((l1p1.y > m2 * l1p1.x + b2) ^ (l1p2.y > m2 * l1p2.x + b2)) && ((l2p1.x < l1p1.x) ^ (l2p2.x < l1p1.x)));
	}
	
	b2 = b2 - b1;
	m2 = m1 - m2;
		
	if(!b2){
		collpoint = v2f(0.0f,b1);
		return true;
	}
	if(!m2){
		collpoint = v2f(0.0f,0.0f);
		return false;
	}
	b2 = b2 / m2;
	collpoint = v2f(b2, b2 * m1 + b1);
	
	return ((l1p1.x > collpoint.x)^(l1p2.x > collpoint.x))&&((l2p1.x > collpoint.x)^(l2p2.x > collpoint.x)); 

}
void handleInput(void){
//	change = false;
	mvec = sf::Mouse::getPosition(window) - mvec;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		p1 += v2f(mvec);
//		change = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
		p2 += v2f(mvec);
//		change = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		p3 += v2f(mvec);
//		change = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)){
		p4 += v2f(mvec);
//		change = true;
	}
	mvec = sf::Vector2i(res_x/2,res_y/2);
	if(window.hasFocus()){
		sf::Mouse::setPosition(mvec,window);
	}
	
	return;
}
