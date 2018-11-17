#include <iostream>
#include <cmath>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


	int width = 1280;
	int height = 720;
	int bpp = 32;
	int bot; 
	float sens = 1;
	float mouse;
	bool empty = false;
	bool flat = false;
	bool up = false;
	bool vchange = false;
	int i1 = 0;
	int i2 = 1;
	int i3 = 2;
	void handleInput(void);
	void sortVerts(void);
	void vswap(int s1, int s2);
	void rasterize(void);
	sf::Font font;
	sf::Text text;
	sf::Vector2i v[3];
	sf::Vector2i swap(0,0);
	sf::Vector2i mP(0,0);
	sf::Vector2f mD(0,0);
	sf::VertexArray edges(sf::Lines, 6);
	sf::VertexArray scan(sf::Lines, 2);
	sf::RenderWindow window(sf::VideoMode(width, height, bpp), "ayy lmao"/*, sf::Style::Fullscreen*/);
	sf::Clock timer;
	sf::Event event;	
	
int main(void){

	v[0] = sf::Vector2i(500, 100);
	v[1] = sf::Vector2i(600, 200);
	v[2] = sf::Vector2i(400, 200);	
	font.loadFromFile("arial.ttf");
	text.setString("b");
	text.setFont(font);
	
	window.setMouseCursorVisible(true);
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
		if(vchange){
			sortVerts();
			vchange = false;
		}
		rasterize();
	
		
//		for(int i = 0; i < bot;i++){
//			//logic
			


//			scan[0].position = sf::Vector2f(,);
//			scan[1].position = sf::Vector2f(,);
//			window.draw(scan);
//		}
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
		
		edges[0].position = sf::Vector2f(v[0]);
		edges[1].position = sf::Vector2f(v[1]);
		edges[2].position = sf::Vector2f(v[1]);
		edges[3].position = sf::Vector2f(v[2]);
		edges[4].position = sf::Vector2f(v[2]);
		edges[5].position = sf::Vector2f(v[0]);	
		
		window.draw(edges);
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
	
	mD = sf::Vector2f(sf::Mouse::getPosition(window)) - sf::Vector2f((width/2),(height/2));	
	sf::Mouse::setPosition(sf::Vector2i((width/2),(height/2)),window);
	
}

void rasterize(void){
	
	if(!empty){
		if(flat){
			if(v[i1].x == v[i2].x){
				return;
			}
			if(up){
				int ystart = v[i3].y;
				int yscans = v[i1].y - ystart;
				float lslope;
				float rslope;
				
				if(v[i1].x != v[i3].x){
					lslope = ((float)v[i1].x - (float)v[i3].x)/(float)yscans; 
				}else{
					lslope = 0.0f;
				}
				
				if(v[i2].y != v[i3].y){
					rslope = ((float)v[i2].x - (float)v[i3].x)/(float)yscans; 
				}else{
					rslope = 0.0f;
				}
				
				for(float i = (float)ystart; i < ((float)ystart+(float)yscans);i++){
					scan[0].position = sf::Vector2f((float)v[i3].x+(lslope*(i-(float)ystart)),i);
					scan[1].position = sf::Vector2f((float)v[i3].x+(rslope*(i-(float)ystart)),i);
					window.draw(scan); 
					
				}
			}else{
				float k = 0.0f;
				int ystart = v[i3].y;
				int yscans = ystart - v[i1].y;
				float lslope;
				float rslope;
				
				if(v[i1].x != v[i3].x){
					lslope = ((float)v[i1].x - (float)v[i3].x)/(float)yscans; 
				}else{
					lslope = 0.0f;
				}
				
				if(v[i2].y != v[i3].y){
					rslope = ((float)v[i2].x - (float)v[i3].x)/(float)yscans; 
				}else{
					rslope = 0.0f;
				}
				
				for(float i = (float)ystart; i > ((float)ystart-(float)yscans);i--){
					scan[0].position = sf::Vector2f((float)v[i3].x+(lslope*k),i);
					scan[1].position = sf::Vector2f((float)v[i3].x+(rslope*k),i);
					window.draw(scan); 
					k++;
				}
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

