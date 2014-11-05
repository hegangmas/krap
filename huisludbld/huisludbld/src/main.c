/*! \mainpage
* \section intro Introduction to firmware for double action domes
* This part of code contains most firmware logic for XMEGA.
*
* \section files Main Files
* - conf_board.h: board configuration
* - conf_usart_example.h: configuration of uart speed
* - conf_clock.h contains configuration for clock speed
*/
#include <asf.h>

#include <ASF/common/services/ioport/xmega/ioport_compat.h>
#include <ASF/common/services/ioport/ioport.h>
///////////////////PARAMETERS//////////////////////////////
/// this section contains parameters for debouncer and gesture recognition
/// Numbers are based on timer counter interrupt module
///
////////////////////////PARAMETERS///////////////////////////////////
#define BUTTONDEBOUNCERSTART 5 //how many cycles should take to detect push
#define BUTTONDEBOUNCERSTOP 25 //this is still buton push
#define BUTTONDEBOUNCERREPETITION 85 //how many cycles does it take to detect repetition
#define BUTTONDEBOUNCERREPETITIONRESETSHORT 67 //how long between 2 repetition samples first //100ms
#define BUTTONDEBOUNCERREPETITIONRESETLONG 48 //longest repetition 2 repetition samples //200ms
#define BUTTONDEBOUNCERREPETITIONSTEP 2 //how long between 2 repetition samples step increment
#define BUTTONDEBOUNCERRELEASED 5
#define DEBOUNCETIMER 10 //this is interupt timer for above numbers (for how long should it take to trigger timmer interupt)
#define SLEEPTIME 60 //time of inactivity before deepsleep 60s
#define LOWSPEEDTIMELIMIT 100 //low speed limit
#define HISPEEDTIME 30 //high speed treshold
#define DEBOUNCETOUCH 3


////////////////////////PARAMETERS///////////////////////////////////

/*
sleepmanager states
power on only after power on in init set state to 1
non active for 60s sleep state 2
sleep on demand short wake up press set state 3
wake up
power off
*/

/// simple macros can be used for work with bits and bytes
#define BIT(x) (1 << (x))
#define SETBITS(x,y) ((x) |= (y))
#define CLEARBITS(x,y) ((x) &= (~(y)))
#define SETBIT(x,y) SETBITS((x), (BIT((y))))
#define CLEARBIT(x,y) CLEARBITS((x), (BIT((y))))
#define BITSET(x,y) ((x) & (BIT(y)))
#define BITCLEAR(x,y) !BITSET((x), (y))
#define BITSSET(x,y) (((x) & (y)) == (y))
#define BITSCLEAR(x,y) (((x) & (y)) == 0)
#define BITVAL(x,y) (((x)>>(y)) & 1)



/// gpio port pins definitions for double action domes
#define BUTTON1 IOPORT_CREATE_PIN(PORTA, 1)
#define BUTTON2 IOPORT_CREATE_PIN(PORTA, 0)
#define BUTTON3 IOPORT_CREATE_PIN(PORTA, 3)
#define BUTTON4 IOPORT_CREATE_PIN(PORTA, 5)
#define BUTTON5 IOPORT_CREATE_PIN(PORTD, 0)
#define BUTTON6 IOPORT_CREATE_PIN(PORTC, 0)
#define BUTTON7 IOPORT_CREATE_PIN(PORTD, 5)
#define BUTTON31 IOPORT_CREATE_PIN(PORTA, 4)
#define BUTTON41 IOPORT_CREATE_PIN(PORTA, 6)
#define BUTTON51 IOPORT_CREATE_PIN(PORTC, 1)
#define BUTTON71 IOPORT_CREATE_PIN(PORTD, 6)
#define BUTTON61 IOPORT_CREATE_PIN(PORTD, 1)

///external wake up button
#define BUTTONWAKEUP IOPORT_CREATE_PIN(PORTA, 2)
//external sw wakeup line
#define IMXWAKEUP IOPORT_CREATE_PIN(PORTC, 2)

///  pins for uart
#define RX		IOPORT_CREATE_PIN(PORTD, 2)
#define TX		IOPORT_CREATE_PIN(PORTD, 3)


//defines for message
#define UP 0x10
#define DOWN 0x20
#define LEFT 0x30
#define RIGHT 0x40
#define B1 0x50
#define B2 0x60
#define B3 0x70
#define B4 0x80
#define B5 0x90
#define B6 0xa0
#define B7 0xb0
#define LO 0x1
#define HI 0x2
#define SERVED_RELEASED 0x0
#define PUSHED 0x1
#define RELEASED 0x2
#define REPETITION 0x3
#define SERVED_PUSHED 0x4
#define SERVED_REPETITION 0x5

#define SHUTDOWNMSG 0x04

volatile uint8_t lock=0;

volatile uint8_t up;
volatile uint8_t down;
volatile uint8_t left;
volatile uint8_t right;
volatile uint8_t b1;
volatile uint8_t b2;
volatile uint8_t b3;
volatile uint8_t b4;
volatile uint8_t b5;
volatile uint8_t b6;
volatile uint8_t b7;
volatile uint8_t b13;

volatile uint16_t b1c=0;
volatile uint16_t b2c=0;
volatile uint16_t b3c=0;
volatile uint16_t b4c=0;
volatile uint16_t b5c=0;
volatile uint16_t b6c=0;
volatile uint16_t b7c=0;
volatile uint16_t b8c=0;
volatile uint16_t b9c=0;
volatile uint16_t b10c=0;
volatile uint16_t b11c=0;
volatile uint16_t b12c=0;

volatile uint32_t b13c=0;
volatile uint32_t b14c=0;


volatile uint16_t b1co=0;
volatile uint16_t b2co=0;
volatile uint16_t b3co=0;
volatile uint16_t b4co=0;
volatile uint16_t b5co=0;
volatile uint16_t b6co=0;
volatile uint16_t b7co=0;

volatile uint8_t b_data;
volatile uint8_t t_data;
volatile uint8_t b_time;
volatile uint8_t t_time;
volatile uint8_t msg[5];

//USART_data_t USART_data;


uint8_t buttondebouncestart= BUTTONDEBOUNCERSTART; //how many cycles should take to detect push
uint8_t buttondebouncestop= BUTTONDEBOUNCERSTOP; //this is still buton push
uint8_t buttondebouncerepetition= BUTTONDEBOUNCERREPETITION; //how many cycles does it take to detect repetition
uint8_t buttondebouncerepetitionreset= BUTTONDEBOUNCERREPETITIONRESETLONG; //how long between 2 repetition samples
uint8_t buttondebouncereleased= BUTTONDEBOUNCERRELEASED;

uint16_t debouncetimer= DEBOUNCETIMER; //this is interupt timer for above numbers

uint16_t sleeptime= SLEEPTIME; //time of inactivity before deepsleep

uint16_t speedtimelimit= LOWSPEEDTIMELIMIT; //speed1 limit

//uint16_t speedtime= SPEEDTIME; //high speed

uint8_t debouncetouch= DEBOUNCETOUCH; //how many cycles should take to detect touch
//uint8_t debouncenotouch= DEBOUNCENOTOUCH; //this is still button touch //not implemented


volatile uint8_t state=0;
volatile uint16_t global_time=0;
volatile uint16_t sleep_timer=0;

volatile uint16_t gesturestart=0;
volatile uint16_t duration=0;
volatile uint8_t setting=0;

void buttons_worker(volatile uint16_t* counter,volatile uint8_t* button, uint8_t number);
void lock_worker(volatile uint32_t* counter,volatile uint8_t* button, uint8_t number);
void buttons_dispatcher(void);
void debouncer(void);
void huievent_decoder(void);
void sleep(void);
void init_interrupts(void);
void init(void);

void sleep(){
	wdt_disable();
	sleep_set_mode(SLEEP_SMODE_PDOWN_gc);
	
	sleep_enable();
	sleep_enter();
	sleep_disable();
	wdt_enable();
	sleep_timer=0;
}




/**********************************************************
* Debouncer function enables
* this function rapidly checks for double action button domes state
* it is called by TC45 callback function
********************************************************/
void debouncer(void)
{
	bool noaction=true;
	b_time++;
	if(!ioport_get_value(BUTTON1))
	{
		b1c++;
		noaction=false;
		state=0;
		global_time=0;
	}
	else{
		if (b1c>0){//b1c--;
			b1c=0;
		}
		
	}
	
	if(!ioport_get_value(BUTTON2))
	{
		b2c++;
		noaction=false;
		state=0;
		global_time=0;
	}
	else{
		if (b2c>0){//b2c--;
			b2c=0;
		}

		
	}
	
	if(!ioport_get_value(BUTTON3))
	{
		b3c++;
		noaction=false;
		state=0;
		global_time=0;
	}
	else{
		if (b3c>0){//b3c--;
			b3c=0;
		}
		
		
	}
	
	if(!ioport_get_value(BUTTON4))
	{
		b4c++;
		noaction=false;
		state=0;
		global_time=0;
	}
	else{
		if (b4c>0){//b4c--;
			b4c=0;
		}
		
		
	}
	
	if(!ioport_get_value(BUTTON5))
	{
		b5c++;
		noaction=false;
		state=0;
		global_time=0;
	}
	else{
		if (b5c>0){//b5c--;
		b5c=0;}
	}
	
	if(!ioport_get_value(BUTTON6))
	{
		b6c++;
		noaction=false;
		state=0;
		global_time=0;
	}
	else{
		if (b6c>0){//b6c--;
			b6c=0;
		}
		
	}
	
	if(!ioport_get_value(BUTTON7))
	{
		b7c++;
		noaction=false;
		state=0;
		global_time=0;
	}
	else{
		if (b7c>0){//b7c--;
			b7c=0;
		}
		
		
	}
	
	if(!ioport_get_value(BUTTON51))
	{
		b8c++;
	//	noaction=false;
	}
	else{
		if (b8c>0){b8c--;
			//b8c=0;
		}
		
		
	}
	
	if(!ioport_get_value(BUTTON71))
	{
		b9c++;
	//	noaction=false;
	}
	else{
		if (b9c>0){b9c--;
			//b9c=0;
		}
	}
	
	if(!ioport_get_value(BUTTON41))
	{
		b10c++;
	//	noaction=false;
	}
	else{
		if (b10c>0){b10c--;
			//b10c=0;
		}
	}
	
	if(!ioport_get_value(BUTTON31))
	{
		b11c++;
	//	noaction=false;
	}
	else{
		if (b11c>0){b11c--;
			//b11c=0;
		}
		
		
	}
	
	if(!ioport_get_value(BUTTON61))
	{
		b12c++;
	//	noaction=false;
	}
	else{
		if (b12c>0){b12c--;
			//b12c=0;
		}
		
		
	}

	if(ioport_get_value(BUTTONWAKEUP))
	{
		b13c++;
	}
	else{
		if (b13c>0){b13c--;
			//b13c=0;
		}
	}
	
	if(ioport_get_value(IMXWAKEUP))
	{
		b14c++;
	}
	else{
		if (b14c>0){b14c--;
			//b14c=0;
		}
	}
	if(noaction) sleep_timer++;
	else sleep_timer=0;
}
/**
* this function is state machine it determines if button is pressed, realesed or in repetetition state
* input; button debounce counter, button state, button number
*
* output none
* effect sending uart message
*/
void buttons_worker(volatile uint16_t* counter,volatile uint8_t* button, uint8_t number)
{

	if((*counter>buttondebouncestart)&&(*counter<buttondebouncestop)&&(*button==SERVED_RELEASED)){
		*button=PUSHED;
	}

	if((*counter>buttondebouncerepetition)&&((*button==SERVED_REPETITION)||(*button==SERVED_PUSHED))){
		*counter=buttondebouncerepetitionreset;
		if (buttondebouncerepetitionreset<BUTTONDEBOUNCERREPETITIONRESETSHORT)
		{
			buttondebouncerepetitionreset+=BUTTONDEBOUNCERREPETITIONSTEP;
		}
		
		*button=REPETITION;
	}
	
	if((*button==SERVED_PUSHED||*button==SERVED_REPETITION)&&*counter<=buttondebouncereleased){
		*button=RELEASED;
		buttondebouncerepetitionreset=BUTTONDEBOUNCERREPETITIONRESETLONG;
	}
	
	if(*button==RELEASED||*button==PUSHED||*button==REPETITION){
		b_data+=number;
		
		if (*button==PUSHED)
		{
			*button=SERVED_PUSHED;
			b_data+=PUSHED;
			
		}
		
		if (*button==REPETITION)
		{
			*button=SERVED_REPETITION;
			b_data+=REPETITION;
			
		}
		if (*button==RELEASED)
		{
			*button=SERVED_RELEASED;
			b_data+=RELEASED;
			
		}
		
		if (b_data!=0)
		{
			
		if (!lock)
		{
		
			cli();
			usart_putchar(USART_SERIAL_EXAMPLE,0xa5);
			usart_putchar(USART_SERIAL_EXAMPLE,b_time);
			usart_putchar(USART_SERIAL_EXAMPLE,b_data);
			usart_putchar(USART_SERIAL_EXAMPLE,b_time + b_data + 0xa5);
			sei();
			b_data=0;
		}
		}
	}
}


void lock_worker(volatile uint32_t* counter,volatile uint8_t* button, uint8_t number)
{

	if((*counter>buttondebouncestart)&&(*counter<buttondebouncestop)&&(*button==SERVED_RELEASED)){
		*button=PUSHED;
	}
	
	if((*button==SERVED_PUSHED)&&*counter<=buttondebouncereleased){
		*button=RELEASED;

	}
	
	if(*button==RELEASED||*button==PUSHED){
		b_data+=number;
		
		if (*button==PUSHED)
		{
			*button=SERVED_PUSHED;
			/*if (lock)
			{
				lock=0;
			}
			else
			{
				lock=1;
			}*/
		}
		
		if (*button==RELEASED)
		{
			*button=SERVED_RELEASED;
			if (!lock)
			{
				lock=1;
			}
			else
			{
				lock=0;
			}
		}

	}
}
/***
*function runs button worker function on parameters for each button
*it is called from main functin and it runs button worker function that determines sate of button.
*/
void buttons_dispatcher(void){
	buttons_worker(&b1c,&b1,B1);
	buttons_worker(&b2c,&b2,B2);
	buttons_worker(&b3c,&b3,B3);
	buttons_worker(&b4c,&b4,B4);
	buttons_worker(&b5c,&b5,B5);
	buttons_worker(&b6c,&b6,B6);
	buttons_worker(&b7c,&b7,B7);
	lock_worker(&b13c,&b13,BUTTONWAKEUP);
}

/*
new gesture function states

none state 0
states
1
2
3
4
are first press
states
5
6
7
8
press of middle button
final press for second button is for acknowledge of gesture send message and reset to state 0


*/

///////////////////////////////////////////////
/// huievent_decoder function changes thou states to determine if we are in middle of gesture.
/// debouncer numbers tell us if dome is pressed
///
///
///						BUTTON31 b11c
///	BUTTON41 b10c		BUTTON61 b12c	BUTTON51 b8c 1
///						BUTTON71 b9c
//////////////////////////////////////////////////////////////////////////

void huievent_decoder(){
	
	if(state==0){
		if((b8c>debouncetouch)){
			state=1;//right
			gesturestart=global_time;
		}
		if((b9c>debouncetouch)){
			state=2;//down
			gesturestart=global_time;
		}
		if((b10c>debouncetouch)){
			state=3;//left
			gesturestart=global_time;
		}
		if((b11c>debouncetouch)){
			state=4;//up
			gesturestart=global_time;
		}
	}
	
	if (//just move start to time when finger leaves first button
	(state==1 && b8c>debouncetouch)||
	(state==2 && b9c>debouncetouch)||
	(state==3 && b10c>debouncetouch)||
	(state==4 && b11c>debouncetouch))
	{
		gesturestart= global_time;
	}
	//we must press middle button to determine proper direction of finger (detect wrapping)
	if((b12c>debouncetouch) && (state==1)){
		state=5;//to left
	}
	if((b12c>debouncetouch) && (state==2)){
		state=6;//to up
	}
	if((b12c>debouncetouch) && (state==3)){
		state=7; //to right
	}
	if((b12c>debouncetouch) && (state==4)){
		state=8;	//to down
	}


	if ((state>0)&&(state<9)&&(global_time-gesturestart>LOWSPEEDTIMELIMIT))
	{
		t_data=0;
		state=0;
		b8c=0;
		b9c=0;
		b10c=0;
		b11c=0;
		b12c=0;
		global_time=0;
	}


	if ((state==5)&&(b10c>debouncetouch))
	{
		//left
		duration=global_time-gesturestart;
		if (duration < HISPEEDTIME){
			t_data+=2;
		}
		else{
			t_data+=1;
		}
		t_data+=LEFT;
		t_time=(uint8_t)0xff&global_time;
		if (!lock)
		{
		
			cli();
			usart_putchar(USART_SERIAL_EXAMPLE,0xa5);
			usart_putchar(USART_SERIAL_EXAMPLE,t_time);
			usart_putchar(USART_SERIAL_EXAMPLE,t_data);
			usart_putchar(USART_SERIAL_EXAMPLE,t_time + t_data + 0xa5);
			t_data=0;
			state=0;
			b8c=0;
			b9c=0;
			b10c=0;
			b11c=0;
			b12c=0;
			sleep_timer=0;
			sei();
		}

	}

	//up
	if ((state==6)&&((b11c>debouncetouch))){
		duration=global_time-gesturestart;
		if (duration < HISPEEDTIME){
			t_data+=2;
		}
		else{
			t_data+=1;
		}
		
		t_data+=UP;
		t_time=(uint8_t)0xff&global_time;

		if (!lock)
		{
		
			cli();
			usart_putchar(USART_SERIAL_EXAMPLE,0xa5);
			usart_putchar(USART_SERIAL_EXAMPLE,t_time);
			usart_putchar(USART_SERIAL_EXAMPLE,t_data);
			usart_putchar(USART_SERIAL_EXAMPLE,t_time + t_data + 0xa5);
			t_data=0;
			state=0;
			b8c=0;
			b9c=0;
			b10c=0;
			b11c=0;
			b12c=0;
			sleep_timer=0;
			sei();
		}

	}
	//right
	if ((state==7)&&(b8c>debouncetouch)){
		duration=global_time-gesturestart;
		if (duration < HISPEEDTIME){
			t_data+=2;
		}
		else{
			t_data+=1;
		}

		t_data+=RIGHT;
		t_time=(uint8_t)0xff&global_time;

		if (!lock)
		{
			cli();
			usart_putchar(USART_SERIAL_EXAMPLE,0xa5);
			usart_putchar(USART_SERIAL_EXAMPLE,t_time);
			usart_putchar(USART_SERIAL_EXAMPLE,t_data);
			usart_putchar(USART_SERIAL_EXAMPLE,t_time + t_data + 0xa5);
			
			t_data=0;
			state=0;
			b8c=0;
			b9c=0;
			b10c=0;
			b11c=0;
			b12c=0;
			sleep_timer=0;
			sei();
		}
	}
	//down
	if ((state==8)&&(b9c>debouncetouch)){
		duration=global_time-gesturestart;
		if (duration < HISPEEDTIME){
			t_data+=2;
		}
		else{
			t_data+=1;
		}
		
		t_data+=DOWN;
		

		t_time=(uint8_t)0xff&global_time;
		
	if (!lock)
		{	
		cli();
			usart_putchar(USART_SERIAL_EXAMPLE,0xa5);
			usart_putchar(USART_SERIAL_EXAMPLE,t_time);
			usart_putchar(USART_SERIAL_EXAMPLE,t_data);
			usart_putchar(USART_SERIAL_EXAMPLE,t_time + t_data + 0xa5);
		
			state=0;
			t_data=0;
			b8c=0;
			b9c=0;
			b10c=0;
			b11c=0;
			b12c=0;
			sleep_timer=0;
			sei();
		}
	}

}


/************************************************************************
*   callback function is called by timer counter tc4 interrupt event
************************************************************************/

static void my_callback(void)
{
	// User code to execute when the overflow occurs here
	debouncer();
	t_time++;
	b_time++;
	global_time++;
	
	if (global_time>2000)
	{
		global_time=0;
		sleep_timer++;
	}
	
	tc45_clear_overflow(&TCC4);
	//huievent_decoder();
}


///function initialize wake up interrupts
//todo: test wake up
void init_interrupts(){
	//enable wake up function on pin change
	PORTA.INTMASK = PIN2_bm;
	PORTA.INTCTRL = PORT_INTLVL_HI_gc;
	PORTA.DIRCLR = PIN2_bm;
	PORTC.INTMASK = PIN2_bm;
	PORTC.INTCTRL = PORT_INTLVL_HI_gc;
	PORTC.DIRCLR = PIN2_bm;
	
	PMIC.CTRL = PMIC_HILVLEN_bm; // enable hi level interrupts
	sei(); // enable interrupts

	//
	////Enable rx interrupt level
	USARTD0_CTRLA = (USARTD0_CTRLA &= ~ USART_RXCINTLVL_gm) | USART_RXCINTLVL_HI_gc;
	//

	pmic_init();
	pmic_set_scheduling(PMIC_SCH_ROUND_ROBIN);
	cpu_irq_enable();

}


/************************************************************************
* Initialize the board.
* The board-specific conf_board.h file contains the configuration of
* the board initialization this is mainly cecouse of internal settings.
*   init function initializes xmega mcu with
*   ports, interrupts, system clock, uart...
************************************************************************/
void init(){
	b1=b2=b3=b4=b5=b6=b7=SERVED_RELEASED;
	state=0;
	
	//manualy set pins direction
	ioport_init();
	ioport_set_pin_dir(BUTTON1,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE);
	ioport_set_pin_dir(BUTTON2,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE);
	ioport_set_pin_dir(BUTTON3,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE);
	ioport_set_pin_dir(BUTTON4,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE);
	ioport_set_pin_dir(BUTTON5,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE);
	ioport_set_pin_dir(BUTTON6,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE);
	ioport_set_pin_dir(BUTTON7,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE);
	ioport_set_pin_dir(BUTTON31,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE);
	ioport_set_pin_dir(BUTTON41,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE);
	ioport_set_pin_dir(BUTTON51,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE);
	ioport_set_pin_dir(BUTTON71,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE);
	ioport_set_pin_dir(BUTTONWAKEUP,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE|PORT_ISC_BOTHEDGES_gc);
	ioport_set_pin_dir(IMXWAKEUP,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE| PORT_ISC_BOTHEDGES_gc);
	ioport_configure_pin(BUTTONWAKEUP,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE|PORT_ISC_BOTHEDGES_gc);
	ioport_configure_pin(BUTTONWAKEUP,IOPORT_DIR_INPUT| IOPORT_FORCE_ENABLE| PORT_ISC_BOTHEDGES_gc);
	ioport_configure_pin(TX, IOPORT_DIR_OUTPUT	| IOPORT_INIT_HIGH);
	ioport_configure_pin(RX, IOPORT_DIR_INPUT	| IOPORT_FORCE_ENABLE);


	//clock settings
	sysclk_init();
	
	// USART options.
	static usart_rs232_options_t USART_SERIAL_OPTIONS = {
		.baudrate = USART_SERIAL_EXAMPLE_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT

	};
	
	/*
	
	//add xmega x5 specific uart files and use it with
    static usart_xmegae_rs232_options_t USART_SERIAL_OPTIONS = {
         .baudrate = USART_SERIAL_EXAMPLE_BAUDRATE,
         .charlength = USART_SERIAL_CHAR_LENGTH,
         .paritytype = USART_SERIAL_PARITY,
         .stopbits = USART_SERIAL_STOP_BIT,
         .start_frame_detection = true,
         .one_wire = false,
         .pec_length = 0,
         .encoding_type = USART_DECTYPE_DATA_gc,
         .encoding_stream = USART_LUTACT_OFF_gc,
};

   usart_xmegae_init_rs232(USART_SERIAL_EXAMPLE, &USART_SERIAL_OPTIONS);
    usart_set_rx_interrupt_level(USART_SERIAL_EXAMPLE, USART_INT_LVL_LO);
	*/



	// Initialize usart driver in RS232 mode
	usart_init_rs232(USART_SERIAL_EXAMPLE, &USART_SERIAL_OPTIONS);

	//configure interrupt for debouncer
	tc45_enable(&TCC4);
	tc45_set_overflow_interrupt_callback(&TCC4, my_callback);
	tc45_set_wgm(&TCC4, TC45_WG_NORMAL);
	tc45_write_period(&TCC4, DEBOUNCETIMER);
	tc45_set_overflow_interrupt_level(&TCC4, TC45_INT_LVL_LO);
	irq_initialize_vectors();
	cpu_irq_enable();
	tc45_write_clock_source(&TCC4, TC45_CLKSEL_DIV1024_gc);
	
	wdt_set_timeout_period(WDT_TIMEOUT_PERIOD_250CLK);
	wdt_enable();
	
	init_interrupts();

	
	lock=0;

}
/*! \brief Main function.
*/
int main(void)
{
	init();
	while (true) {
		//	t_time++;
		//	b_time++;
		
		if (b13c>555)//we should go to sleep on demand of user
		{//off message
			wdt_disable();
			cli();
			usart_putchar(USART_SERIAL_EXAMPLE,0xa5);
			usart_putchar(USART_SERIAL_EXAMPLE,0x4);
			usart_putchar(USART_SERIAL_EXAMPLE,SHUTDOWNMSG);
			usart_putchar(USART_SERIAL_EXAMPLE,0x4 + SHUTDOWNMSG + 0xa5);
			sei();
			//				sleep();
			while(1);
		}
		wdt_reset();
		//if (b13c>debouncetouch&&b13c<(buttondebouncerrepetition-buttondebouncerrepetitionresetlong)&&!ioport_get_value(buttonwakeup))//pressed for longer then 1s and released
		//{
			//
	///*		//		sleep();
			//if (lock)
			//{
					//lock=0;
				//}
			//else
				//{
					//lock=1;
				//}*/
		//}
	//	wdt_reset();
		
		buttons_dispatcher();
		wdt_reset();
		huievent_decoder();
		wdt_reset();
		
		
		//incactive to long go to sleep(lock)
		
		//if(sleep_timer>((BUTTONDEBOUNCERREPETITION-BUTTONDEBOUNCERREPETITIONRESETLONG)*5)*SLEEPTIME)
		if(sleep_timer>=(185*SLEEPTIME))
		{
			lock=1;
			sleep();
			lock=0;
		}
		wdt_reset();
	}
}

ISR(PORTA_INT_vect)
{
/*	if (lock)
	{
		lock=0;
	}
		else
	{
		lock=1;
	}*/
	PORTA.INTFLAGS |= PORT_INT2IF_bm;
}
ISR(PORTC_INT_vect)
{
	if (lock)
	{
		lock=0;
	}
/*	else
	{
		lock=1;
	}*/
	PORTC.INTFLAGS |= PORT_INT2IF_bm;

}

/*
ISR(USART_RX_Vect)
{ 
		if (usart_getchar(USART_SERIAL_EXAMPLE)==SHUTDOWNMSG)
		{
			lock=1;
			sleep_timer=((BUTTONDEBOUNCERREPETITION-BUTTONDEBOUNCERREPETITIONRESETLONG)*5)*SLEEPTIME+1;
		}
			else{
			lock=0;
		}
}
*/

/*
ISR(USARTD0_RXC_vect)
{
	char rbuf=0;
	usart_serial_getchar(USART_SERIAL_EXAMPLE,&rbuf);
	if (rbuf==SHUTDOWNMSG)
	{
		lock=1;
		sleep_timer=((BUTTONDEBOUNCERREPETITION-BUTTONDEBOUNCERREPETITIONRESETLONG)*5)*SLEEPTIME+1;
	}
	else{
		lock=0;
	}
}
*/