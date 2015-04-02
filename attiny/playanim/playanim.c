#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <string.h>
#include <uWireM/uWireM.h>
#include <matrix8x8/matrix8x8.h>
#include "anim.h"

#define BRIGHTNESS 10 /* 0=dim, 15=max */

/*** delay() -- delay for (at least) a specified number of milliseconds

This function delay execution for the number of milliseconds specified
by its argument. (The delay is not precise, but will always be at least
as long as requested.)

This function is necessary because the _delay_ms() function provided by
the library can only take integer arguments.
***/
static void delay(uint8_t x) { for(; x; x--) _delay_ms(1); }

/* When we receive a pin-change interrupt, we want it to wake us up
   from sleep but do nothing else special. We still need to declare
   the vector, but we don't need any code in the interrupt service
   routine. Using EMPTY_INTERRUPT saves 18 bytes over having an
   ISR with an empty function body! */
EMPTY_INTERRUPT(PCINT0_vect);

/*** sleep_until_pin_change() -- power down and wait for a pin change

Puts the processor and all on- and off-board peripherals into the state
with the lowest possible power consumption while retaining the ability
to detect a pin-change interrupt.

Sleeps until such an interrupt is received, then powers back up and
returns.

The code here is lifted almost verbatime from Philip Burgess' original
Space Invader Pendant code.
***/
static void sleep_until_pin_change(void) {
   /* Go to the lowest-power state we can achieve and wait until
      we get awakened by a pin change interrupt: */

   matrix8x8_pwr(MATRIX8X8_PWR_STANDBY); /* put display in standby */
   GIMSK = _BV(PCIE);     /* enable pin change interrupts */
   power_all_disable();   /* turn off all peripheral power */
   set_sleep_mode(SLEEP_MODE_PWR_DOWN);
   sleep_enable();
   sei();                 /* keep interrupts enabled */
   sleep_mode();          /* power down CPU until pin change wakes us */

   /* When we get here, it means we were awakened by a pin change. */
   GIMSK = 0;             /* disable pin change interrupts */
   power_timer0_enable();
   power_usi_enable();
   uWireM_init();         /* I2C needs re-init after USI poweroff */
   matrix8x8_clear();
   matrix8x8_pwr(MATRIX8X8_PWR_ON);
}

int main(void) {
   uint8_t i, n, wait, rep = 0;

   power_timer1_disable(); /* turn off to save power */
   power_adc_disable();    /* likewise */
   PCMSK |= _BV(PCINT1);   /* get interrupt on change to pin 1 */
   uWireM_init();          /* set up I2C communication library */

   matrix8x8_init(BRIGHTNESS); /* set up 8x8 LED matrix */

   while(1) { /* loop forever, displaying the animation */

      /* for all the steps in the step list... */
      for(wait = i = 0; i < anim_steps(); i++) {

         /* Get the next step. If the value is 255, then the next
            byte is a new wait value. */
         if((n = pgm_read_byte(_step + i)) == 255) {
            wait = pgm_read_byte(_step + (++i));
            continue;
         }

         /* Draw the specified frame: */
         matrix8x8_drawP(_frame + (n<<3));

         /* Leave it showing for the current wait value (in ms): */
         delay(wait);
      }

      if(++rep == ANIM_REPS) { /* if we just finished the last cycle... */
         rep = 0;
         sleep_until_pin_change();
      }
   } /* end while(1) */
}
