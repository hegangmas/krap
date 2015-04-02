#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#include "i2cmaster.h"

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

#define button_frequency_down PORTB,0x1
#define button_frequency_up PORTB,0x2
#define direction_down DDRB,0x1
#define direction_up DDRB,0x2

#define TEA5767  0x60                    // device address of tea5767 => 0b1100000

int frequency = 8890;                  //starting frequency

void setFrequency(){
	char frequencyB = 4 * (frequency * 10000 + 225000) / 32768;  //calculating PLL word
	char frequencyH = frequencyB>>8;
	char frequencyL = frequencyB&0XFF;
	unsigned char ret;
	i2c_init();                          // init I2C interface
	ret = i2c_start(TEA5767+I2C_WRITE);            // set device address and write mode
  
    if ( ret ) {
        /* failed to issue start condition, possibly no device found */
        i2c_stop();
    }
	else {  
		i2c_write(frequencyH);
		i2c_write(frequencyL);
		i2c_write(0xB0);
		i2c_write(0x10);
		i2c_write(0x00);
		i2c_stop();
	}
	
}

int main(void) {
  
  C_CLEARBIT(direction_down);
  C_CLEARBIT(direction_up);
  
  for(;;){
	  
	if(PORTB & (1<<1)){
    
		frequency=frequency-5;
		if(frequency<8750)
		{
			frequency=10800;
			setFrequency();
		}
    }
    
    if(PORTB & (1<<2)){
		
		frequency=frequency+5;
		if(frequency>10800){
			frequency=8750;
			setFrequency();
		}
	}
		
	setFrequency(frequency);
	_delay_ms(250);	
	}
		 
}
