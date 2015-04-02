#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <util/delay.h>
#include <avr/io.h>

#include <inttypes.h>
#include <avr/interrupt.h>
#include "USI_TWI_Master.h"



/* define CPU frequency in Mhz here if not defined in Makefile */
#ifndef F_CPU
#define F_CPU 8000000UL
#endif


#define _BV(bit) (1 << (bit))

#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))

#define SETBITMASK(x,y) (x |= (y))
#define CLEARBITMASK(x,y) (x &= (~y))
#define FLIPBITMASK(x,y) (x ^= (y))
#define CHECKBITMASK(x,y) (x & (y))

#define VARFROMCOMB(x, y) x
#define BITFROMCOMB(x, y) y
/*use this ones*/
#define C_SETBIT(comb) SETBIT(VARFROMCOMB(comb), BITFROMCOMB(comb))
#define C_CLEARBIT(comb) CLEARBIT(VARFROMCOMB(comb), BITFROMCOMB(comb))
#define C_FLIPBIT(comb) FLIPBIT(VARFROMCOMB(comb), BITFROMCOMB(comb))
#define C_CHECKBIT(comb) CHECKBIT(VARFROMCOMB(comb), BITFROMCOMB(comb))
/*use this ones*/
#define MIN(A,B)  (((A)<(B)) ? (A) : (B) )
#define MAX(A,B)  (((A)>(B)) ? (A) : (B) )
#define PUT_IN_RANGE(V,VMIN,VMAX) MAX(VMIN,MIN(VMAX,V))
#define MAP_TO_RANGE(V,VMIN0,VMAX0,VMIN1,VMAX1) ( (VMIN1) +  ( (V) – (VMIN0) ) * ( (VMAX1) – (VMIN1) ) / ( (VMAX0) – (VMIN0) ) )

#define button_frequency_down PORTB,0x3
#define button_frequency_up PORTB,0x4
#define direction_down DDRB,0x3
#define direction_up DDRB,0x4

#define TEA5767  0x60                    // device address of tea5767 => 0b1100000
int frequency = 8890;                  //starting frequency
void setFrequency(void);
/* I2C clock in Hz */

#define MESSAGEBUF_SIZE       6
unsigned char messageBuf[MESSAGEBUF_SIZE];

void setFrequency(){

	messageBuf[0]=TEA5767;
	messageBuf[1]=(4 * (frequency * 10000 + 225000) / 32768)>>8;
	messageBuf[2]=(4 * (frequency * 10000 + 225000) / 32768)&0XFF;
	messageBuf[3]=0xB0;
	messageBuf[4]=0x10;
	messageBuf[5]=0x00;
	
	USI_TWI_Master_Initialise();

	USI_TWI_Start_Transceiver_With_Data(messageBuf,(unsigned char)MESSAGEBUF_SIZE);
		
}

short up=0;
short down=0;

int main(void) {
  
  C_CLEARBIT(direction_down);
  C_CLEARBIT(direction_up);
  
  for(;;){
	  
	if(!(PORTB & (1<<3))&&down==0){
    down=1;
	}
	if(!(PORTB & (1<<3))&&down==1){
		down=0;
		if(frequency>8750)
		{
			frequency=frequency-10;
			setFrequency();
		}
    }
    
    if(!(PORTB & (1<<4))&&up==0){
		up=1;
	}
	if(!(PORTB & (1<<4))&&up==1){
		up=0;
		if(frequency<10800){
			frequency=frequency+10;
			setFrequency();
		}
	}

	_delay_ms(250);
	}
		 
}
