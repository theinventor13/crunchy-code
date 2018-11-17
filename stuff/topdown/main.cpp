#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

	int width = 1920;
	int height = 1080;
	int bpp = 32;//hard constants until res picker is implemented
	int vx;
	int vy;
	int sx;
	int sy;
	
	float view_zoom;
	
	void handleInput(void);
	void drawGrid(void);
	void determineViewport(void);
	float ox = 0;
	float oy = 0;
	float vux = 0;
	float vuy = 0;
	float vlx = 1;
	float vly = 1;
	float cx = 0; 
	float cy = 0;
	float ix = width/2;
	float iy = 0;
	float zoom = 1;
	float mouse = 0;
	float mx = 1;
	float dt = 1;
	float sens = 2;
	sf::Vector2i pos;
	sf::RenderWindow window(sf::VideoMode(width, height, bpp), "ayy lmao", sf::Style::Fullscreen);
	sf::Clock ck;
	sf::Time delta;
	sf::View port;
	sf::Texture one;
	sf::Texture two;
	sf::Texture bac;
	sf::Sprite iso;
		
	float pps(float inc);
	
int main(void){
		//std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
		//for (std::size_t i = 0; i < modes.size(); ++i){
			//sf::VideoMode mode = modes[i];
			//std::cout << "Mode #" << i << ": "<< mode.width << "x" << mode.height << " - "<< mode.bitsPerPixel << " bpp" << std::endl;
		//}
		
		port.reset(sf::FloatRect(0,0,320,240));//change this to whatever 4:3 res you want
		determineViewport();
		port.setViewport(sf::FloatRect(vux,0,vlx,1));
		window.setView(port);
		window.setVerticalSyncEnabled(true);
		
		if(!bac.loadFromFile("iso.png"))
			return EXIT_FAILURE;
		iso.setTexture(bac, true);
		iso.setPosition(0,0);	
		sf::Event event;
		ck.restart();
		
	while(window.isOpen()){
		
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed)
				window.close();
			if(event.type == sf::Event::MouseWheelScrolled)
				mouse = event.mouseWheelScroll.delta;
				//mouse = sf::Event::MouseWheelScrollEvent::delta;
		}
		
		delta = ck.getElapsedTime();
		dt = delta.asSeconds();
		ck.restart();	
		
		
		window.clear();
		handleInput();
		port.rotate(pps(mx*sens));
		
		window.draw(iso);
		window.setView(port);
		window.display();
		
	}
	
	return EXIT_SUCCESS;
	
}

float pps(float inc){
	
	return inc * dt;
	
}


void determineViewport(void){
	//just puts 4:3 viewport in the middle of any ratio screen (nuclear throne effect)
	
	float ratio = (float)4/(float)3;
	vux = (((float)width - ((float)height * ratio)) / 2) / (float)width;
	vlx = ((float)height * ratio)/(float)width;
	
	return;
	
}


void handleInput(void){
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
			window.close();
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
			ox -= pps(66);
			if(ox < -33) 
				ox = (-1)*(ox+33);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
			ox += pps(66);
			if(ox > 33) 
				ox = ox-33;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
			oy += pps(66);
			if(oy > 33) 
				oy = oy-33;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
			oy -= pps(66);
			if(oy < -33) 
				oy = (-1)*(oy+33);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
			//window.create(sf::VideoMode(1920, 1080), "ayy lmao", sf::Style::Fullscreen);
			//back.setScale(1,1);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
			//window.create(sf::VideoMode(1280, 720), "ayy lmao", sf::Style::Fullscreen);
			//back.setScale(seven20, seven20);
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
			cy -= 2;
			cx -= 2;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
			cy += 2;
			cx += 2;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
			cy += 2;
			cx -= 2;
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
			cy -= 2;
			cx += 2;
		}
		if(mouse != 0){
			
			port.zoom(1+mouse/2);
			window.setView(port);
			mouse = 0;
			
		}
		pos = sf::Mouse::getPosition();
		mx = pos.x-960;
		sf::Mouse::setPosition(sf::Vector2i(960,0),window);
		
}



