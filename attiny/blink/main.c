#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>

int main(void)
{
	DDRB |= (1 << 4);		/* Set PORTB bit 4 to output. */

	while (1) {
		PORTB &= ~(1 << 4);	/* LED on */
		_delay_ms(995);
		PORTB |= (1 << 4);	/* LED off */
		_delay_ms(5);
	}

	return 0;
}
