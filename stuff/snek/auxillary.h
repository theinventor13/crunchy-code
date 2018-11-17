
sf::Font arial;
sf::Text print;
bool c_init = false;
//console vars
struct c_entry{
	string entry;
	sf::Color col;
	c_entry * prev;
};

unsigned char c_len = 2;
c_entry head;
c_entry * c_iter;
//console var end

enum side{
	top, bot, left, right
};

enum tiles{
	air, ground
};

void error(const string &em){
	cerr << " GAME ERROR: " << em << endl;
}

void start_sfml_print(void){
	arial.loadFromFile("fonts/mono2.ttf");
	print.setFont(arial);
	head.entry = "SFML ingame console v1";
	head.col = sf::Color::Green;
	head.prev = new c_entry;	
	c_iter = head.prev;
	c_iter->entry = "Current entry limit is 256";
	c_iter->col = sf::Color::Green;
	c_iter->prev = &head;
	c_init = true;
	return;
}
void sfml_print(string out, sf::Vector2f tpos = sf::Vector2f(0,0), sf::Color tc = sf::Color::Black){
	
	print.setColor(tc);
	print.setPosition(tpos);
	print.setString(out);
	window.draw(print);
	
	return;
	
}
void sfml_print(int val, sf::Vector2f tpos = sf::Vector2f(0,0), sf::Color tc = sf::Color::Black){
	
	string out = slow_itos(val);
	print.setColor(tc);
	print.setPosition(tpos);
	print.setString(out);
	window.draw(print);
	
	return;
	
}
void sfml_print(string out, int val, sf::Vector2f tpos = sf::Vector2f(0,0), sf::Color tc = sf::Color::Black){
	
	out.append(slow_itos(val));
	print.setColor(tc);
	print.setPosition(tpos);
	print.setString(out);
	window.draw(print);
	
	return;
	
}
void sfml_print(string out, float x, float y, sf::Color tc = sf::Color::Black){
	
	print.setColor(tc);
	print.setPosition(sf::Vector2f(x,y));
	print.setString(out);
	window.draw(print);
	
	return;
	
}
void sfml_print(int val, float x, float y, sf::Color tc = sf::Color::Black){
	
	string out = slow_itos(val);
	print.setColor(tc);
	print.setPosition(sf::Vector2f(x,y));
	print.setString(out);
	window.draw(print);
	
	return;
	
}
void sfml_print(string out, int val, float x, float y, sf::Color tc = sf::Color::Black){
	
	out.append(slow_itos(val));
	print.setColor(tc);
	print.setPosition(sf::Vector2f(x,y));
	print.setString(out);
	window.draw(print);
	
	return;
	
}
void end_sfml_print(void){
	c_entry * hold;
	c_len--;
	for(unsigned char k = 0; k < c_len; k++){
		hold = c_iter;
		c_iter = c_iter->prev;
		free(hold);
	}
	c_init = false;
	return;
}

void sfml_Cprint(string out, sf::Color clr = sf::Color::Black){
	if(!c_init){
		error("sfml_Cprint: sfml_print not initialized");
		return;
	}
	head.prev = new c_entry;
	(head.prev)->prev = c_iter;
	c_iter = head.prev;
	c_iter->entry = out;
	c_iter->col = clr;
	
	if(c_len == 255){//implement old entry deleter
//		head.entry = (head.prev)->entry;
//		hold = (head.prev)->prev;
//		free(head.prev);
//		head.prev = hold;
	}else{
		c_len++;	
	}
	return;
	
}
void sfml_Cprint(int val, sf::Color clr = sf::Color::Black){
	if(!c_init){
		error("sfml_Cprint: sfml_print not initialized");
		return;
	}
	head.prev = new c_entry;
	(head.prev)->prev = c_iter;
	c_iter = head.prev;
	c_iter->entry = slow_itos(val);
	c_iter->col = clr;
	
	if(c_len == 255){//implement old entry deleter
//		head.entry = (head.prev)->entry;
//		hold = (head.prev)->prev;
//		free(head.prev);
//		head.prev = hold;
	}else{
		c_len++;	
	}
	return;
	
}
void sfml_Cprint(string out, int val, sf::Color clr = sf::Color::Black){
	if(!c_init){
		error("sfml_Cprint: sfml_print not initialized");
		return;
	}
	head.prev = new c_entry;
	(head.prev)->prev = c_iter;
	c_iter = head.prev;
	c_iter->entry = out.append(slow_itos(val));
	c_iter->col = clr;
	
	if(c_len == 255){//implement old entry deleter
//		head.entry = (head.prev)->entry;
//		hold = (head.prev)->prev;
//		free(head.prev);
//		head.prev = hold;
	}else{
		c_len++;	
	}
	return;
	
}
void sfml_console(void){
	if(!c_init){
		error("sfml_console: sfml_print not initialized");
		return;
	}
	sf::Color shadow;
	bool c_scroll = true;
	float c_y = (float)res_y - 60;
	while(1){
		
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			}
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)){
				window.close();
			}
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Tilde)){
				return;
			}	
		}	
		
		if(c_scroll){								 //add console commands,
			window.clear(sf::Color(100,100,100,255));//make independant of resolution,
			for(unsigned char k = 0; k < c_len; k++){//and implement line number printing
				shadow = c_iter->col;
				shadow.a = 100;
				sfml_print(c_iter->entry, sf::Vector2f(98,c_y+2), shadow);
				sfml_print(c_iter->entry, sf::Vector2f(100,c_y), c_iter->col);
				c_y -= 40;
				if(c_y < 0){
					break;
				}
				c_iter = c_iter->prev;
			} 
			window.display();
			c_scroll = false;
			c_iter = head.prev;
		}
	
	}
}

