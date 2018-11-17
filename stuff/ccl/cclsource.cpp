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
v2f center;
float radius;
void handleInput(void);
bool ccl(v2f c, float r, v2f lp1, v2f lp2);//circle collide line
bool lcl(v2f l1p1,v2f l1p2,v2f l2p1,v2f l2p2);//line collide line
bool coll = true;
//bool change = true;
//bool test = true;
int res_x = 1000;
int res_y = 600;
#include "auxillary.h"
void draw_line(float x1=0, float y1=0, float x2=0, float y2=0){
	//this is super shit lmao
	line.setPoint(0, v2f(x1, y1));
	line.setPoint(1, v2f(x1, y1));
	line.setPoint(2, v2f(x2, y2));
	line.setPoint(3, v2f(x2, y2));
	window.draw(line);
	return;
} 

bool init(void){
	//init start
	radius = 50.0f;
	center = v2f(100,100);
	p1 = v2f(0,0);
	p2 = v2f(200,200);
	orb.setFillColor(sf::Color(0,0,0,0));
	orb.setOutlineThickness(1.0f);
	line.setOutlineThickness(.5f);
	line.setOutlineColor(sf::Color::Black);
	line.setFillColor(sf::Color::Black);
	window.create(sf::VideoMode(res_x, res_y, 32), "Circle and Line Segment Collision Demo", sf::Style::Close);
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
		sfml_print("Hold down either A, S, or D, and then move the mouse",20.0f,10.0f,sf::Color::Black);
		orb.setOutlineColor(sf::Color::Black);
		orb.setRadius(radius);
		orb.setOrigin(v2f(radius,radius)); 
		orb.setPosition(center);
		window.draw(orb);
		//draw center
		if(coll){
			orb.setOutlineColor(sf::Color::Cyan);	
		}else{
			orb.setOutlineColor(sf::Color::Blue);
		}
		orb.setRadius(4);
		orb.setOrigin(v2f(4,4));
		window.draw(orb);
		//draw line seg points
		orb.setOutlineColor(sf::Color::Black);
		orb.setPosition(p1);
		window.draw(orb);
		orb.setPosition(p2);
		window.draw(orb);
		//draw perpendicular points
//		if(change&&test){
			coll = ccl(center, radius, p1, p2);	
//		}else if(!test){
//			coll = ccl(center, radius, p1, p2);
//		}
		handleInput();
		window.display();
	}
	
	return EXIT_SUCCESS;
	
}

bool ccl(v2f c, float r, v2f lp1, v2f lp2){
	if((lp1.x  -c.x) * (lp1.x - c.x) + (lp1.y - c.y) * (lp1.y - c.y) < (r * r)){
		return true;//if lp1 is in circle, collision
	}
	v2f d = lp2 - lp1;
	if(!d.x&&!d.y){//lp1 is not in the circle, so if lp2 is equal to lp1, no collision
		return false;
	}
	if((lp2.x - c.x) * (lp2.x - c.x) + (lp2.y - c.y) * (lp2.y - c.y) < (r * r)){
		return true;
	}//if lp2 in circle, collision
	float m = r / sqrt(d.x*d.x+d.y*d.y);
	v2f v1;
	v2f v2;
	v1.x = -d.y;
	v1.y = d.x;
	v2 = -v1;//opposite vectors perpendicular to lp1->lp2	
	v1 = (v1*m)+c;
	v2 = (v2*m)+c;//normalized and scaled to radius
	draw_line(v2.x, v2.y, v1.x, v1.y);//draw lines
	draw_line(lp2.x, lp2.y, lp1.x, lp1.y);
	//draw points
	orb.setOutlineColor(sf::Color::Red);
	orb.setPosition(v1);
	window.draw(orb);
	orb.setOutlineColor(sf::Color::Green);
	orb.setPosition(v2);
	window.draw(orb);
	return lcl(v1,v2,lp1,lp2);
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
		center += v2f(mvec);
//		change = true;
	}
	mvec = sf::Vector2i(res_x/2,res_y/2);
	if(window.hasFocus()){
		sf::Mouse::setPosition(mvec,window);
	}
	
	return;
}
