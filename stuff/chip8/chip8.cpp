#include <iostream>
#include <string>
#include <cmath>
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
using namespace std;

typedef sf::Vector2f v2f;
v2f PIX_SIZE;
sf::RectangleShape PIX;
sf::Music BEEP;
sf::Clock CK;
sf::Time DT;
sf::Event event;
sf::RenderWindow window;
unsigned long int CODETIMER = 0;//hacky shit
unsigned long int TICKTIMER = 0;
unsigned int GAME_CODE_LEN = 0;
unsigned char GAME_CODE[4000];
char *GAME_NAME = "c8games/TETRIS";
int res_x = 1440;
int res_y = res_x / 2;

#include "hexsprites.h"

struct CHIP8{
	//CONSTANT DECLARATIONS//
	//Memory bounds
	static const unsigned short int RAM_START = 0x0000;
	static const unsigned short int RAM_END = 0x0FFF;//add compatibility for 2k ram emulation
	//Interpreter bounds
	static const unsigned short int INTPR_START = 0x0000;
	static const unsigned short int INTPR_END = 0x01FF;//512 bytes long
	//Regular program
	static const unsigned short int PROG_START = 0x0200;//program starts after interpreter
	static const unsigned short int PROG_LEN_MAX = 0x0C8F;//maximum program length
	//ETI 660 program
	static const unsigned short int ETI_PROG_START = 0x0600;//custom memory map for ETI660 programs
	static const unsigned short int ETI_PROG_LEN_MAX = 0x09FF;//TODO: FIX THIS, VALUES INCORRECT
	//12-bit Address bitmask
	static const unsigned short int ADDR_MASK = 0x0FFF;//and-out opcode nibble
	//Nibble access
	static const unsigned char BLN = 0x0F;//byte lower nipple
	static const unsigned char BUNS = 0x04;//byte upper nipple shift
	//Shift MSB from top of byte
	static const unsigned char MSB = 0x07;//most significant bit
	//Length of stack
	static const unsigned char STACK_LEN = 0x10;//stack size
	//Access upper and lower bytes of opcode
	static const unsigned char UPPER = 0x01;//little endian to big endian
	static const unsigned char LOWER = 0x00;//avoid brain strain
	
	//Hex character set sprite size
	static const unsigned char HEX_SPRITE_SIZE = 0x05;
	//Opcode length
	static const unsigned char OPCODE_LEN = 0x02;
	//END CONSTANT DECLARATIONS//
	
	//SIGNAL DECLARATIONS//
	enum SIG{
		RUN = 0x00, //Program still executing
		END = 0x01, //Program ended without problems
		ERR = 0x02, //Miscellaneous error
		UOE = 0x03, //Unknown opcode encountered
		SCL = 0x04, //Stack is packed, cant call subroutine
		SRT = 0x05};//Stack is empty, cant return from subroutine
	//END SIGNAL DECLARATIONS//
	
	//STATE VARIBLE DECLARATIONS//
	union OPCODE{
		unsigned short int WORD;//full instruction
		unsigned char BYTE[2];//upper and lower bytes
	}CI;//holds current instruction
	char SPX;//sprite x pos
	char SPY;//sprite y pos
	unsigned char SIGNAL;//used for killing the program if theres a runtime error and explaining why
	unsigned char RAM[RAM_END - RAM_START];//random access memory lol
	unsigned char DRX;//screen x size
	unsigned char DRY;//screen y size
	unsigned char V[16];//16 8bit registers
	unsigned char DT;//delay register
	unsigned char ST;//sound register
	unsigned char SP;//stack pointer
	unsigned short int STACK[(int)STACK_LEN];//16bit stack, STACK_LEN maximum nested subroutines
	unsigned short int PC;//program counter
	unsigned short int I;//store memory addresses
	unsigned short int PROG_END;//program end variable
	unsigned short int PROG_LEN;//program length variable
	unsigned short int CHAR_OFFSET;//cant be bigger than 431
	unsigned short int CARRY;//used for arithmetic overflow
	int ITER;//slut counter, used by every loop in the emulator
	int ITER2;//slut counter's slutty sister, same proclivities as her sibling
	bool * SCREEN;//screen state
	bool NOINC;//keeps program counter from incrementing after jump opcodes
	bool TICK;//true every 0.166666666666666666667 seconds (60hz)
	bool KEY[16];//current state of hex keypad
	bool INITIALIZED;//chip emulator will only run if initialized properly
	bool ETI;//ETI660 mode flag
	bool SUPER;//Super Chip-8 mode flag
	bool PAUSE;//pause emulation
	bool PIXEL;//just a holding variable
	bool COLL;//sprite collision
	bool BEEPING;//deals with sfml sound shit
	bool DEBUG;//self explanatory
	//END STATE VARIBLE DECLARATIONS//
	
	bool INITIALIZE(unsigned char * PROGRAM, unsigned short int PL, unsigned char SL = 0x10, bool SUPERCHIP = false, unsigned short int START = PROG_START, bool ETI660 = false){
		//INITIALIZATION//
		//registers not blanked, code accordingly
		SIGNAL = SIG::RUN;
		SPX = 0x00;
		SPY = 0x00;
		DT = 0x00;
		ST = 0x00;
		SP = 0x00;
		I = 0x0000;
		CARRY = 0x0000;
		ITER = 0x00000000;
		ITER2 = 0x00000000;
		ETI = ETI660;
		SUPER = SUPERCHIP;
		NOINC = false;
		PAUSE = false;
		PIXEL = false;
		COLL = false;
		BEEPING = false;
		
		PROG_LEN = PL;
		if(PROG_LEN < 2){//is there at least one instruction to execute
			cout << "CHIP8: PROGRAM SIZE IS ZERO" << endl;
			INITIALIZED = false;
			return INITIALIZED;
		}
		
		if(START == PROG_START){//checks for custom start position
			PC = PROG_START;
		}else if(START > INTPR_END){
			if(ETI){//legacy compatibility
				if(PROG_LEN > ETI_PROG_LEN_MAX){
					cout << "CHIP8: PROGRAM EXCEEDS MAXIMUM SIZE OF " << ETI_PROG_LEN_MAX << endl;
					INITIALIZED = false;
					return INITIALIZED;
				}else{
					PC = ETI_PROG_START;	
				}	
			}else{
				if(PROG_LEN > ((PROG_START + PROG_LEN_MAX) - START)){
					PC = START;
				}else{
					cout << "CHIP8: PROGRAM EXCEEDS MAXIMUM SIZE OF " << ((PROG_START + PROG_LEN_MAX) - START) << endl;
					INITIALIZED = false;
					return INITIALIZED;
				}	
			}
		}else{
			cout << "CHIP8: PROGRAM LOAD ADDRESS CAN NOT BE IN INTERPRETER MEMORY (0x000-0x1FF)" << endl;
			INITIALIZED = false;
			return INITIALIZED;
		}
		PROG_END = PC + PROG_LEN;
		if(SUPER){//enlarge screensize if emulating Super Chip-8
			DRX = 0x80;
			DRY = 0x40;
		}else{
			DRX = 0x40;
			DRY = 0x20;
		}
		
		SCREEN = new bool[DRX*DRY];//allocate screen buffer
		
		for(ITER = 0x0;ITER < DRX*DRY;ITER++){//bleach screen buffer
			SCREEN[ITER] = false;
		}
		
		for(ITER = 0x0;ITER < PROG_LEN;ITER++){//load program code into memory
			RAM[PC+ITER] = PROGRAM[ITER];
		}
		
		CHAR_OFFSET = 0x0000;//offset of character set sprites within interpreter memory
		for(ITER = 0x0;ITER < 80;ITER++){//TODO: make sure char offset isnt bigger than 431
			RAM[RAM_START+CHAR_OFFSET+ITER] = HEX_SPRITES[ITER];//character set accessable by all programs
		}//idk why I put CHAR_OFFSET in here, maybe some legacy program needs hex sprites in a different location
		//but that doesnt really make sense because of LD F, Vx
		//but whatever
		
		INITIALIZED = true;
		return INITIALIZED;
		//END INITIALIZATION//
	}
	
	void STEP(bool END = false, bool PAUSE = false){
		
		if(PAUSE || !INITIALIZED){//lol, dont do anything
			return;
		}
		
		POLL_INPUT(false);//check keypad
		
		//OPCODE PROCESSING//
		CI.BYTE[UPPER] = RAM[PC];//load opcode
		CI.BYTE[LOWER] = RAM[PC+0x1];//dont forget the second byte
		
		//TODO: Comment this whole shit
		//OPCODE DECODER//
		switch(CI.BYTE[UPPER]>>BUNS){
			case 0x0://CLS and RET
				switch(CI.BYTE[LOWER]){
					case 0xEE://RET
						if(SP==0x00){
							SIGNAL = SIG::SRT;//Returning to nothing
							break;
						}else{
							SP--;
							PC = STACK[SP];
						}
						break;
					case 0xE0://CLS
						for(ITER = 0;ITER < DRY;ITER++){
							for(ITER2 = 0;ITER2 < DRX;ITER2++){
								SCREEN[(ITER*DRX)+ITER2] = false;
							}
						}
						break;
					default:
						if(SUPER){//TODO: add super chip opcode functions
							switch(CI.BYTE[LOWER]){
								case 0xFB://SCR
									break;
								case 0xFC://SCL
									break;
								case 0xFD://EXIT
									break;
								case 0xFE://LOW
									break;
								case 0xFF://HIGH
									break;
								default://SCD OR SIG::UOE
									if((CI.BYTE[LOWER]&0x0F) == 0xC0){//if upper nibble equals 0xC
										//then SCD
									}else{
										//else idk wtf this is
										SIGNAL = SIG::UOE;//Unknown opcode
									}
									break;
							}
						}else{
							SIGNAL = SIG::UOE;//Unknown opcode
						}
						break;
				}
				break;
			case 0x1://JP ADDR
				PC = CI.WORD & ADDR_MASK;		   
				NOINC = true;								  
				break;
			case 0x2://CALL ADDR
				if(SP==STACK_LEN){
					SIGNAL = SIG::SCL;//Stack packed
					break;
				}else{
					STACK[SP] = PC;
					SP++;
					PC = CI.WORD & ADDR_MASK;	
					NOINC = true;
				}
				break;
			case 0x3://SE VX, BYTE
				if(V[CI.BYTE[UPPER]&BLN] == CI.BYTE[LOWER]){
					PC += OPCODE_LEN;
				}
				break;
			case 0x4://SNE VX, BYTE
				if(V[CI.BYTE[UPPER]&BLN] != CI.BYTE[LOWER]){
					PC += OPCODE_LEN;
				}
				break;
			case 0x5://SE VX, VY
				if(V[CI.BYTE[UPPER]&BLN] == V[CI.BYTE[LOWER]>>BUNS]){
					PC += OPCODE_LEN;
				}
				break;
			case 0x6://LD VX, BYTE
				V[CI.BYTE[UPPER]&BLN] = CI.BYTE[LOWER];
				break;
			case 0x7://ADD VX, BYTE
				//TODO: Lookup whether this should be wrap-around
				V[CI.BYTE[UPPER]&BLN] += CI.BYTE[LOWER];
				break;
			case 0x8://LD, LOGIC, AND ARITH
				switch(CI.BYTE[LOWER]&0xF){
					case 0x00://LD VX, VY
						V[CI.BYTE[UPPER]&BLN] = V[CI.BYTE[LOWER]>>BUNS];
						break;
					case 0x01://OR VX, VY
						V[CI.BYTE[UPPER]&BLN] |= V[CI.BYTE[LOWER]>>BUNS];
						break;
					case 0x02://AND VX, VY
						V[CI.BYTE[UPPER]&BLN] &= V[CI.BYTE[LOWER]>>BUNS];
						break;
					case 0x03://XOR VX, VY
						V[CI.BYTE[UPPER]&BLN] ^= V[CI.BYTE[LOWER]>>BUNS];
						break;
					case 0x04://ADD VX, VY
						CARRY = V[CI.BYTE[UPPER]&BLN] + V[CI.BYTE[LOWER]>>BUNS];
						V[0xF] = CARRY>>(MSB+1);//get LSB of upper byte
						V[CI.BYTE[UPPER]&BLN] = CARRY & 0x00FF;//and'ed with low-byte mask
						break;
					case 0x05://SUB VX, VY
						//TODO: Look up if subtraction should wrap-around
						V[0xF] = (V[CI.BYTE[UPPER]&BLN] > V[CI.BYTE[LOWER]>>BUNS]);
						V[CI.BYTE[UPPER]&BLN] -= V[CI.BYTE[LOWER]>>BUNS];
						break;
					case 0x06://SHR VX, VY
						//TODO: Look up if this should rotate by VY bits
						V[0xF] = V[CI.BYTE[UPPER]&BLN]&0x01;//and'ed with LSB mask
						V[CI.BYTE[UPPER]&BLN] >>= 0x1;
						break;
					case 0x07://SUBN VX, VY
						//TODO: Look up if subtraction should wrap-around
						V[0xF] = (V[CI.BYTE[LOWER]>>BUNS] > V[CI.BYTE[UPPER]&BLN]);
						V[CI.BYTE[UPPER]&BLN] = V[CI.BYTE[LOWER]>>BUNS] - V[CI.BYTE[UPPER]&BLN];
						break;
					case 0x0E://SHL VX, VY
						//TODO: Look up if this should rotate by VY bits
						V[0xF] = (V[CI.BYTE[UPPER]&BLN]&0x80)>>MSB;//and'ed with MSB mask
						V[CI.BYTE[UPPER]&BLN] <<= 0x1;
						break;
					default:
						SIGNAL = SIG::UOE;//Unknown opcode
						break;
				}
				break;
			case 0x9://SNE VX, VY
				if(V[CI.BYTE[UPPER]&BLN] != V[CI.BYTE[LOWER]>>BUNS]){
					PC += OPCODE_LEN;
				}
				break;
			case 0xA://LD I, ADDR
				I = CI.WORD & ADDR_MASK;
				break;
			case 0xB://JP V0, ADDR
				PC = V[0] + (CI.WORD & ADDR_MASK);
				NOINC = true;
				break;
			case 0xC://RND VX, BYTE
				V[CI.BYTE[UPPER]&BLN] = RAND255(CI.BYTE[LOWER]);
				break;
			case 0xD://DRW VX, VY, NIBBLE
				PIXEL = false;
				COLL = false;
				if((CI.BYTE[LOWER]&BLN) > 0x0){//if 0 bytes specified then this is the super chip version of drw
					SPY = V[CI.BYTE[LOWER]>>BUNS];
					for(ITER = 0x0; ITER < (CI.BYTE[LOWER] & 0x0F); ITER++){
						if((ITER + SPY) >= DRY){//y-wraparound
							SPY = 0x0;
							SPY -= ITER;
						}
						SPX = V[CI.BYTE[UPPER]&BLN];
						for(ITER2 = 0x0; ITER2 < 0x8; ITER2++){
							if(ITER2+SPX >= DRX){//x-wraparound
								SPX = 0x0;
								SPX -= ITER2;
							}
							PIXEL = (RAM[I+ITER]>>(0x07-ITER2))&0x01;//rotate to current bit of sprite, mask out everything else 
							if(SCREEN[((ITER+SPY)*DRX)+ITER2+SPX] && PIXEL && !COLL){//if pixel was erased, set collision flag
								V[0xF] = 0x01;
								COLL = true;
							}
							SCREEN[((ITER+SPY)*DRX)+ITER2+SPX] ^= PIXEL;//blit pixel to screen
						}	
					}
					if(!COLL){
						V[0xF] = 0x00;
					}
				}else{
					if(SUPER){
						//TODO: add superchip-48 DRW VX, VY, 0
					}else{
						SIGNAL = SIG::UOE;
					}
				}
				break;
			case 0xE://SKP VX AND SKNP VX
				switch(CI.BYTE[LOWER]){
					case 0x9E://SKP VX
						if(KEY[V[CI.BYTE[UPPER]&BLN]]){
							PC += OPCODE_LEN;
						}
						break;
					case 0xA1://SKNP VX			
						if(!(KEY[V[CI.BYTE[UPPER]&BLN]])){
							PC += OPCODE_LEN;
						}
						break;
					default:
						SIGNAL = SIG::UOE;
				}
				break;
			case 0xF://LD VARIANTS
				switch(CI.BYTE[LOWER]){
					case 0x07://LD VX, DT
						V[CI.BYTE[UPPER]&BLN] = DT;
						break;
					case 0x0A://LD VX, K
						V[CI.BYTE[UPPER]&BLN] = POLL_INPUT(true);
						break;
					case 0x15://LD DT, VX
						DT = V[CI.BYTE[UPPER]&BLN];
						break;
					case 0x18://LD ST, VX
						ST = V[CI.BYTE[UPPER]&BLN];
						break;
					case 0x1E://ADD I, VX
						I += V[CI.BYTE[UPPER]&BLN];
						break;
					case 0x29://LD F, VX
						if(V[CI.BYTE[UPPER]&BLN] <= 0xF){//Is value a hexadecimal digit
							I = (V[CI.BYTE[UPPER]&BLN] * HEX_SPRITE_SIZE) + CHAR_OFFSET;
						}else{
							SIGNAL = SIG::ERR;//Miscellaneous error
						}
						break;
					case 0x33://LD B, VX
						RAM[I + 0x2] = V[CI.BYTE[UPPER]&BLN] % 10;//ones place
						RAM[I + 0x1] = ((V[CI.BYTE[UPPER]&BLN] % 100) - RAM[I + 0x2])/10;//tens place
						RAM[I] = (V[CI.BYTE[UPPER]&BLN] - (V[CI.BYTE[UPPER]&BLN] % 100))/100;//hundreds place
						break;
					case 0x55://LD [I], VX
						for(ITER = 0; ITER <= (CI.BYTE[UPPER]&BLN); ITER++){
							RAM[I+ITER] = V[ITER];
						}
						break;
					case 0x65://LD X, [I]
						for(ITER = 0; ITER <= (CI.BYTE[UPPER]&BLN); ITER++){
							V[ITER] = RAM[I+ITER];
						}
						break;
					default:
						if(SUPER){//TODO: add super chip opcode functions
							switch(CI.BYTE[LOWER]){
							case 0x30:
								break;
							case 0x75:
								break;
							case 0x85:
								break;	
							default:
								SIGNAL = SIG::UOE;//Unknown opcode
								break;	
							}	
						}else{
							SIGNAL = SIG::UOE;//Unknown opcode
						}		
				}
				break;
			default:
				SIGNAL = SIG::UOE;//Unknown opcode
				break;
		}
		//END OPCODE DECODER//
		
		
		//ADJUST SYSTEM STATE//
		if(SIGNAL==SIG::RUN){//if program is still running
			if((PC+OPCODE_LEN >= PROG_END)||END){//if end of program or killed by user, send end signal
				SIGNAL = SIG::END;
			}else{
				if(!NOINC){
					PC += OPCODE_LEN;//Increment program counter
				}else{
					NOINC = false;//skip PC increment if CALL or JMP or whatever
				}
			}
		}
		
		if(SIGNAL){//end sim if signal recieved and explain why
			cout << "CHIP8: SIGNAL ";
			switch(SIGNAL){
				case SIG::END:
					cout << "'SIG::END': PROGRAM ENDED NORMALLY AT INSTRUCTION "; 
					break;
				case SIG::ERR: 
					cout << "'SIG::ERR': PROGRAM HALTED EXECUTION DUE TO AN EXCEPTION AT INSTRUCTION "; 
					break;
				case SIG::UOE:
					cout << "'SIG::UOE': PROGRAM ENCOUNTERED UNKNOWN OPCODE AT INSTRUCTION "; 
					break;
				case SIG::SCL:
					cout << "'SIG::SCL': PROGRAM ATTEMPTED TO NEST MORE THAN " << STACK_LEN << " SUBROUTINES AT INSTRUCTION "; 
					break;
				case SIG::SRT:
					cout << "'SIG::SRT': PROGRAM CALLED RETURN WITHOUT PREVIOUS CALL TO SUBROUTINE AT INSTRUCTION "; 
					break;
				default://it should never get here lmao
					cout << "'SIG::UNKNOWN': PROGRAM HALTED EXECUTION DUE TO AN UNKNOWN EXCEPTION AT INSTRUCTION "; 
					break;
			}
			free(SCREEN);
			INITIALIZED = false;
			cout << ((PC-PROG_START)/2)+1 << endl;//output relative PC
			printf("CURRENT OPCODE: %x\n",CI.WORD);//show current opcode
			DUMP();//spill guts
			return;
		}
		//END ADJUST SYSTEM STATE//
		
		//END OPCODE PROCESSING//
		
		//UPDATE IO//
		
		UPDATE_TIMERS();

		UPDATE_OUTPUT();	

		//END UPDATE IO//
	}
	
	void DUMP(void){
		
		//barf out state
		cout << "(PC): " << PC << " (CI): ";//figure out how to output hex with cout
		printf("%x\n", CI.WORD);
		printf("(I): %x \n", I);
		for(ITER = 0; ITER < 0x10; ITER++){
			printf("(V[%x]): %x {%i}\n", ITER, V[ITER], V[ITER]);
		}
		
		return;	
	}
	
	unsigned char RAND255(unsigned char MASK){
		float SCALE = (float)rand()/(float)RAND_MAX;
		return ((unsigned char)floor(SCALE*255.0f))&MASK;
	}
	
	unsigned char POLL_INPUT(bool BLOCK){
		if(BLOCK){
			while(1){
				while(window.pollEvent(event)){
					if(event.type == sf::Event::Closed){
						window.close();
						return 0x00;
					}
					if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)){
						window.close();
						return 0x00;
					}	
				}
				
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){
					return 0x01;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){
					return 0x02;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)){
					return 0x03;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)){
					return 0x0C;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
					return 0x04;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
					return 0x05;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
					return 0x06;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
					return 0x0D;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
					return 0x07;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
					return 0x08;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
					return 0x09;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)){
					return 0x0E;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
					return 0x0A;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
					return 0x00;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::C)){
					return 0x0B;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::V)){
					return 0x0F;
				}
			}
		}else{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){
				KEY[0x01] = true;
			}else{
				KEY[0x01] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){
				KEY[0x02] = true;
			}else{
				KEY[0x02] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)){
				KEY[0x03] = true;
			}else{
				KEY[0x03] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)){
				KEY[0x0C] = true;
			}else{
				KEY[0x0C] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
				KEY[0x04] = true;
			}else{
				KEY[0x04] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
				KEY[0x05] = true;
			}else{
				KEY[0x05] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
				KEY[0x06] = true;
			}else{
				KEY[0x06] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
				KEY[0x0D] = true;
			}else{
				KEY[0x0D] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
				KEY[0x07] = true;
			}else{
				KEY[0x07] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
				KEY[0x08] = true;
			}else{
				KEY[0x08] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
				KEY[0x09] = true;
			}else{
				KEY[0x09] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)){
				KEY[0x0E] = true;
			}else{
				KEY[0x0E] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
				KEY[0x0A] = true;
			}else{
				KEY[0x0A] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
				KEY[0x00] = true;
			}else{
				KEY[0x00] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::C)){
				KEY[0x0B] = true;
			}else{
				KEY[0x0B] = false;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::V)){
				KEY[0x0F] = true;
			}else{
				KEY[0x0F] = false;
			}
		}
		return 0x00;
	}
	
	void UPDATE_TIMERS(void){
		//decrement nonzero timers every 1/60th of a second
		if((DT>0x0)&&TICK){
			DT--;
		}
		if((ST>0x0)&&TICK){
			ST--;
		}
		if(ST==0x0){
			BEEP.pause();
			BEEPING = false;
		}else{
			if(!BEEPING){
				BEEP.play();
				BEEPING = true;
			}
			
		}
	}
	
	void UPDATE_OUTPUT(void){
		window.clear();
		for(ITER = 0;ITER < DRY;ITER++){
			for(ITER2 = 0;ITER2 < DRX;ITER2++){
				if(SCREEN[(ITER*DRX)+ITER2]){
					PIX.setPosition(PIX_SIZE.x*ITER2, PIX_SIZE.y*ITER);
					window.draw(PIX);//draw all activated pixels
				}
			}
		}
		window.display();
		return;
	}
	
}CHIP;

bool init(void){
	//init start
	
	//load game
	int BYTE;
	FILE * LOAD = fopen(GAME_NAME,"rb");
	if(LOAD != NULL){
		while((BYTE = getc(LOAD)) != EOF){
		GAME_CODE[GAME_CODE_LEN] = (unsigned char)BYTE;
		GAME_CODE_LEN++;
		}
	}else{
		cout << "COULD NOT LOAD PROGRAM ";
		printf("%s\n", GAME_NAME);
		return false;
	}
	//load game end
	
	cout << "PROG_LEN = " << GAME_CODE_LEN << endl;

	if(!CHIP.INITIALIZE(GAME_CODE,GAME_CODE_LEN)){
		return false;
	}else{
		cout << "CHIP8: SUCCESSFULLY INITIALIZED" << endl;
	}
	window.create(sf::VideoMode(res_x, res_y, 32), "CHIP 8 EMULATOR", sf::Style::Close);
	window.setMouseCursorVisible(false);
	if(!BEEP.openFromFile("beep.wav")){
		cout << "beep was not loaded" << endl;
		return false;
	}
	BEEP.setLoop(true);
	PIX_SIZE.x = (float)res_x/(float)CHIP.DRX;
	PIX_SIZE.y = (float)res_y/(float)CHIP.DRY;
	PIX.setSize(PIX_SIZE);
	PIX.setFillColor(sf::Color::White);
	srand(time(NULL));
	CK.restart();
	
	//init end
	return true;
}

int main(int argc, char ** argv){
	
	if(argc > 1){
		GAME_NAME = argv[1];
		printf("%s\n", argv[1]);
	}
	
	if(!init()){
		cout << "ERROR: main: init failed" << endl;
		return EXIT_FAILURE;
	}
	
	while(window.isOpen()){
		
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				CHIP.STEP(true);//spill guts before closing
				window.close();
			}
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)){
				CHIP.STEP(true);//here too
				window.close();
			}	
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Return)){
				CHIP.DUMP();//also here
			}
			if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::BackSpace)){
				CHIP.STEP(true);//reset emulator
				if(!CHIP.INITIALIZE(GAME_CODE,GAME_CODE_LEN)){
					return EXIT_FAILURE;
				}
			}	
		}
		
		DT = CK.getElapsedTime();
		CODETIMER += DT.asMicroseconds();
		TICKTIMER += DT.asMicroseconds();
		CK.restart();
		
		if(CODETIMER >= 2000){//hacky timing constant bullshit
			CHIP.STEP();
			CODETIMER = 0;
		}
		if(TICKTIMER >= 16666){
			CHIP.TICK = true;
			TICKTIMER = 0;
		}else{
			if(CODETIMER == 0){
				CHIP.TICK = false;
			}
		}
	}
	return EXIT_SUCCESS;
}
