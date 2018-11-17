#include <iostream>
#include <cmath>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

//vars

	int width = 1280;
	int height = 720;
	int bpp = 32;
	int i1 = 0;
	int i2 = 1;
	int i3 = 2; 
	float sens = 1;
	float mouse;
	bool empty = false;
	bool flat = false;
	bool up = false;
	bool vchange = false;
	bool software = true;
	
	void handleInput(void);
	void sortVerts(void);
	void vswap(int s1, int s2);
	void rasterize(sf::Color col);
	sf::Font font;
	sf::Text text;
	sf::ConvexShape face;
	sf::Vector2i v[3];
	sf::Vector2i swap(0,0);
	sf::Vector2i mP(0,0);
	sf::Vector2f mD(0,0);
	sf::VertexArray edges(sf::Lines, 6);
	sf::VertexArray scan(sf::Lines, 2);
	sf::RenderWindow window(sf::VideoMode(width, height, bpp), "ayy lmao"/*, sf::Style::Fullscreen*/);
	sf::Clock timer;
	sf::Event event;	

//vars

int main(void){

	v[0] = sf::Vector2i(500, 100);
	v[1] = sf::Vector2i(600, 200);
	v[2] = sf::Vector2i(400, 200);	
	face.setPointCount(3);
	face.setFillColor(sf::Color(200,100,0,255));
	font.loadFromFile("arial.ttf");
	text.setString("b");
	text.setFont(font);
	
	window.setMouseCursorVisible(false);
	sortVerts();
	while(window.isOpen()){
		
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed)
				window.close();
			if(event.type == sf::Event::MouseWheelScrolled)
				mouse = event.mouseWheelScroll.delta;
		}
			
		
		
		window.clear();
		handleInput();
//		edges[0].position = sf::Vector2f(v[0]);
//		edges[1].position = sf::Vector2f(v[1]);
//		edges[2].position = sf::Vector2f(v[1]);
//		edges[3].position = sf::Vector2f(v[2]);
//		edges[4].position = sf::Vector2f(v[2]);
//		edges[5].position = sf::Vector2f(v[0]);	
//		
//		window.draw(edges);
		if(vchange){
			sortVerts();
		}
		
		if(software){
			//software tri-fill
			rasterize(sf::Color(200,100,0,255));
		}else{
			//hardware tri-fill
			face.setPoint(0, sf::Vector2f(v[0]));
			face.setPoint(1, sf::Vector2f(v[1]));
			face.setPoint(2, sf::Vector2f(v[2]));
			window.draw(face);
		}
		
		if(!flat){
			text.setString("c");
			text.setPosition(sf::Vector2f(v[i1]));
			window.draw(text);
			
			text.setString("b");
			text.setPosition(sf::Vector2f(v[i2]));
			window.draw(text);
			
			text.setString("a");
			text.setPosition(sf::Vector2f(v[i3]));
			window.draw(text);			
		}
	
		window.display();
		
	}
	
	return EXIT_SUCCESS;
	
}

void handleInput(void){
	vchange = false;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
		window.close();
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)&&
	  (!sf::Keyboard::isKeyPressed(sf::Keyboard::S))&&
	  (!sf::Keyboard::isKeyPressed(sf::Keyboard::D))){
	  	
	  	v[0] += sf::Vector2i(mD * sens);
		vchange = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)&&
	  (!sf::Keyboard::isKeyPressed(sf::Keyboard::A))&&
	  (!sf::Keyboard::isKeyPressed(sf::Keyboard::D))){
		  	
		v[1] += sf::Vector2i(mD * sens);
		vchange = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)&&
	  (!sf::Keyboard::isKeyPressed(sf::Keyboard::A))&&
	  (!sf::Keyboard::isKeyPressed(sf::Keyboard::S))){
		  	
		v[2] += sf::Vector2i(mD * sens);
		vchange = true;
	}		
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)&&
	  (!sf::Keyboard::isKeyPressed(sf::Keyboard::W))){
			software = true;	
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)&&
	  (!sf::Keyboard::isKeyPressed(sf::Keyboard::Q))){
			software = false;	
	}
	
	mD = sf::Vector2f(sf::Mouse::getPosition(window)) - sf::Vector2f((width/2),(height/2));	
	sf::Mouse::setPosition(sf::Vector2i((width/2),(height/2)),window);
}

void rasterize(sf::Color col = sf::Color::White){
	
	scan[0].color = col;
	scan[1].color = col;
	if(!empty){
		if(flat){
			if(v[i1].x == v[i2].x){
				return;
			}
			float k = 0.0f;
			float ystart = (float)v[i3].y;
			float ex = v[i3].x;
			
			float yscans;
			if(up){
				yscans = (float)v[i1].y - ystart;
			}else{
				yscans = ystart - (float)v[i1].y;
			}
			
			float lslope;
			if(v[i1].x != v[i3].x){
				lslope = ((float)v[i1].x - (float)v[i3].x)/yscans; 
			}else{
				lslope = 0.0f;
			}
			
			float rslope;
			if(v[i2].x != v[i3].x){
				rslope = ((float)v[i2].x - (float)v[i3].x)/yscans; 
			}else{
				rslope = 0.0f;
			}
			
			if(up){
				for(float i=ystart; i < (ystart+yscans);i++){
					scan[0].position = sf::Vector2f(ex+(lslope*k),i);
					scan[1].position = sf::Vector2f(ex+(rslope*k),i);
					window.draw(scan);
					k++;
				}
				return;
			}else{
				for(float i = ystart; i > (ystart-yscans);i--){
					scan[0].position = sf::Vector2f(ex+(lslope*k),i);
					scan[1].position = sf::Vector2f(ex+(rslope*k),i);
					window.draw(scan); 
					k++;
				}
				return;	
			}
		}else{
			if(v[i1].x == v[i2].x && v[i2].x == v[i3].x){
				return;
			}
			
			float ystart = (float)v[i1].y;
			float yscans = ((float)v[i3].y - (float)v[i1].y);
			float tps = ((float)v[i2].y - (float)v[i1].y);
			float bts = ((float)v[i3].y - (float)v[i2].y);
			float longslope = ((float)v[i3].x - (float)v[i1].x)/yscans;
			float topslope = ((float)v[i2].x - (float)v[i1].x)/tps;
			float botslope = ((float)v[i3].x - (float)v[i2].x)/bts;
			float lc = (float)v[i1].x;//left counter
			float rc = lc;//right counter
			float k = 0.0f;
			float i;
			
			for(i = ystart;i < (ystart+tps);i++){
				lc += longslope;
				rc += topslope;
				scan[0].position = sf::Vector2f(lc,i);
				scan[1].position = sf::Vector2f(rc,i);
				window.draw(scan); 
				k++;
			}
			for(;i < (ystart+yscans);i++){
				lc += longslope;
				rc += botslope;
				scan[0].position = sf::Vector2f(lc,i);
				scan[1].position = sf::Vector2f(rc,i);
				window.draw(scan); 
				k++;
			}
			
			
			
		}	
	}
	
}

void sortVerts(void){
	i1 = 0;
	i2 = 1;
	i3 = 2;
	
	//sorts so that: v[i1].y <= v[i2].y <= v[i3].y
	//if flat, then: v[i1].x <= v[i2].x 
	if((v[0] == v[1])&&(v[1] == v[2])&&(v[2] == v[0])){
		empty = true;
		return;
	}
	
	flat = true;
	
	if(v[0].y == v[1].y){
		
		if(v[1].y < v[2].y){
			up = false;
		}else{
			up = true;
		}
		if(v[0].x > v[1].x){
			i1 = 1;
			i2 = 0;
		}
		return;	
	}
	if(v[1].y == v[2].y){
		
		if(v[2].y < v[0].y){
			up = false;
		}else{
			up = true;
		}
		i3 = 0;
		if(v[1].x > v[2].x){
			i1 = 2;
			i2 = 1;
		}else{
			i1 = 1;
			i2 = 2;
		}
		
		return;	
	}
	if(v[0].y == v[2].y){
		
		if(v[0].y < v[1].y){
			up = false;
		}else{
			up = true;
		}
		i3 = 1;
		if(v[0].x > v[2].x){
			i1 = 2;
			i2 = 0;
		}else{
			i1 = 0;
			i2 = 2;
		}
		return;	
	}
	
	flat = false;
	//a<b<c check
	if(v[0].y < v[1].y){//a < b
		if(v[1].y < v[2].y){//b < c
			return;
		}else{
			if(v[0].y < v[2].y){//a < c
				i1=0;
				i2=2;
				i3=1;
				return;
			}else{
				i1=2;
				i2=0;
				i3=1;
				return;
			}
		}
	}else{
		if(v[0].y < v[2].y){//a < c
			i1=1;
			i2=0;
			i3=2;
			return;
		}else{
			if(v[1].y < v[2].y){
				i1=1;
				i2=2;
				i3=0;
				return;
			}else{
				i1=2;
				i2=1;
				i3=0;
				return;
			}
		}
	}
}

