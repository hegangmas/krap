/*
 * File:   newmain18.c
 * Author: uros
 *
 * Created on Sobota, 13 oktober 2012, 14:03
 */


/*
 // SSPSTAT REGISTER

// Master SPI mode only

#define   SMPEND        0b10000000           // Input data sample at end of data out
#define   SMPMID        0b00000000           // Input data sample at middle of data out


#define   MODE_00       0b00000000              // Setting for SPI bus Mode 0,0
//CKE           0x40                   // SSPSTAT register
//CKP           0x00                   // SSPCON1 register

#define   MODE_01       0b00000001              // Setting for SPI bus Mode 0,1
//CKE           0x00                   // SSPSTAT register
//CKP           0x00                   // SSPCON1 register

#define   MODE_10       0b00000010              // Setting for SPI bus Mode 1,0
//CKE           0x40                   // SSPSTAT register
//CKP           0x10                   // SSPCON1 register

#define   MODE_11       0b00000011              // Setting for SPI bus Mode 1,1
//CKE           0x00                   // SSPSTAT register
//CKP           0x10                   // SSPCON1 register

// SSPCON1 REGISTER
#define   SSPENB        0b00100000           // Enable serial port and configures SCK, SDO, SDI

#define   SPI_FOSC_4    0b00000000              // SPI Master mode, clock = Fosc/4
#define   SPI_FOSC_16   0b00000001              // SPI Master mode, clock = Fosc/16
#define   SPI_FOSC_64   0b00000010              // SPI Master mode, clock = Fosc/64
#de
 * fine   SPI_FOSC_TMR2 0b00000011              // SPI Master mode, clock = TMR2 output/2
 *
 * #if defined (SPI_V1) || defined (SPI_V4)

/* ***** SPI ***** */

/**************************************************************************
Macro       : EnableIntSPI

Description : Enables SPIx Interrupt

Arguments   : None

Remarks     : This macro sets SPI Interrupt Enable bit of Interrupt Enable
		Control register.
**************************************************************************
#define EnableIntSPI                   (PIE1bits.SSPIE = 1)

/**************************************************************************
Macro       : DisableIntSPI

Description : This macro disables the SPI Interrupt

Arguments   : None

Remarks     : This macro clears SPI Interrupt Enable bit of Interrupt Enable
		Control register.
**************************************************************************
#define DisableIntSPI                  (PIE1bits.SSPIE = 0)

/**************************************************************************
Macro       : SetPriorityIntSPI(priority)

Description : This macro sets the priority level for SPI interrupt

Arguments   : priority - This input parameter is the level of interrupt priority.
		* 0 -- Low priority (Default Value)
		* 1 -- High Priority

Remarks     : None
*************************************************************************
#define SetPriorityIntSPI(priority)     (IPR1bits.SSPIP = priority)

/*******************************************************************
Macro       : SPI_Clear_Intr_Status_Bit

Description : Macro to Clear external Interrupt Status bit

Arguments   : None

Remarks     : None
******************************************************************
#define SPI_Clear_Intr_Status_Bit     (PIR1bits.SSPIF = 0)

/*******************************************************************
Macro       : SPI_Intr_Status

Description : Macro to Return SPI Interrupt Status

Arguments   : None

Remarks     : None
******************************************************************
#define SPI_Intr_Status     (PIR1bits.SSPIF)

/**************************************************************************
Macro       : SPI_Clear_Recv_OV

Description : macro clears Receive Overflow flag

Arguments   : None

Remarks     : None
**************************************************************************
#define  SPI_Clear_Recv_OV    SSPCONbits.SSPOV = 0



/*********************************************************************
Macro  	:	CloseSPI()

Include        : 	spi.h

Description : 	This Macro turns off the SPI module

Arguments   : 	None

Return Value: 	None

Remarks      : 	This Macro disables the SPI module.
********************************************************************
#define  CloseSPI()    (SSPCON1 &=0xDF)

/************************************************************************
Macro :  DataRdySPI()

Include            : spi.h

Description        : This Macro provides status back to user if SSPxBUF
			register contain data.

Arguments          : None

Remarks            : This Macro determines if there is any byte to read from
			SSPxBUF register.
************************************************************************
#define  DataRdySPI()    (SSPSTATbits.BF)

/**************************************************************************
Macro       : getcSPI

Description : macro is identical to ReadSPI,#define to ReadSPI in spi.h

Arguments   : None

Remarks     : None
**************************************************************************
#define  getcSPI  ReadSPI

/**************************************************************************
Macro       : putcSPI

Description : macro is identical to WriteSPI,#define to WriteSPI in spi.h

Arguments   : None

Remarks     : None
**************************************************************************
#define  putcSPI  WriteSPI


void OpenSPI(  unsigned char sync_mode,
               unsigned char bus_mode,
               unsigned char smp_phase );

unsigned char WriteSPI(  unsigned char data_out );

void getsSPI(  unsigned char *rdptr,  unsigned char length );

void putsSPI(  unsigned char *wrptr );

unsigned char ReadSPI( void );
 *
 *
 *
 *
 */

//#define __18F4585
#define SPI_V1
#define SW_SPI_IO_V1
//#include <pic18.h>
//#include <htc.h>
#include <xc.h>
#include<plib/usart.h>
#include <plib/spi.h>
//#include <stdio.h>
//#include <plib/sw_spi.h>

#pragma config OSC=HS, FCMEN=ON, WDT=OFF, IESO=ON, XINST=OFF, LVP=OFF 
#define NUMBER_OF_ATA_CHIPS 12

#define CHIP_SELECT LATCbits.LATC0



void initSpi(void){
     CloseSPI();	//close if was on
    OpenSPI(SPI_FOSC_64,MODE_01,SMPMID);
   // WriteSPI()

}


void initApp(void){

    TRISA=0;
    TRISB=0;
    TRISD=0;

    PORTA=0;
    PORTB=0;
    PORTD=0;

  //  OSCCONbits_t.IDLEN idle/sleep
  //  OSCCONbits_t.IRCF0 //speed from 8mhz to 32k
  //  OSCCONbits_t.IRCF1
  //  OSCCONbits_t.IRCF2
  //  OSCCONbits_t.OSTS ////oscilator timer ended
  //  OSCCONbits_t.IOFS //stable or not
  //  OSCCONbits_t.SCS0 //internal
  //  OSCCONbits_t.SCS1 //timer / primal oscilator

    OSCCON=0b01110011; //8Mhz / internal
  //  LATCbits_t.LATC0=1; //external
    LATC0=1;
    
 //   WDTCON=0x00;
    initSpi();
}


/*
//funkcija gre skozi vse ?ipe.
//po?lje najprej: chipId1, potem chipId0, potem CONTROL,potem pa prejema podatke
//pogruntaj ali morem brati za vsakim po?iljanjem ter po?iljati
//pred vsakimbranjem
//-za zacetek je fiksno stevilo cipov definirano z NUMBER_OF_ATA_CHIPS
// kasneje bo to globalna spremenjljivka
//
//

unsigned char* askATA6870(unsigned char *chipData, char sizeOfData){
    unsigned char i;
    unsigned char chipId0; 
    unsigned char chipId1;

    CHIP_SELECT=1;
    OpenSPI(SPI_FOSC_64,MODE_01,SMPEND);

    for(i=0;i<NUMBER_OF_ATA_CHIPS;i++){
        if(i<8){
            chipId0=1<<(i);
            chipId1=0;
            }
        else {
            chipId0=0;
            chipId1=1<<(i-8);
        }
       
        
    }
    CloseSPI();
    CHIP_SELECT=0;
    return chipData;
}
*/
/*
void askAtaForSingleData(unsigned char chipId1,unsigned char chipId0,unsigned char bat,unsigned char control, unsigned char* batData){//posamezni dostop

     //IDENTIFICATION
        WriteSPI(chipId1);
        while(DataRdySPI()==0);
        WriteSPI(chipId0);
        while(DataRdySPI()==0);
        //CONTROL
        //WriteSPI();//ugotovi kaj je treba nastaviti ata ?ipu
        while(DataRdySPI()==0);
        //DATA
        char r;
        for(r=0;r<1;r++)//size of data je lahko 1 2 ali 14
            batData[r]=ReadSPI();//odvisno koliko je podatkov, najverjetneje sta potem potrebna minimalno 2 klica

        while(DataRdySPI()==0);
}*/
/*
void askAtaForDoubleData(unsigned char chipId1,unsigned char chipId0,unsigned char bat,unsigned char control, unsigned char* batData){//posamezni dostop 2 bayta

     //IDENTIFICATION
        WriteSPI(chipId1);
        while(DataRdySPI()==0);
        WriteSPI(chipId0);
        while(DataRdySPI()==0);
        //CONTROL
        //WriteSPI();//ugotovi kaj je treba nastaviti ata ?ipu
        while(DataRdySPI()==0);
        //DATA
        char r;
        for(r=0;r<2;r++)//size of data je lahko 1 2 ali 14
            batData[r]=ReadSPI();//odvisno koliko je podatkov, najverjetneje sta potem potrebna minimalno 2 klica

        while(DataRdySPI()==0);
}
*/
void askAtaForAllData(unsigned char chipId1,unsigned char chipId0,unsigned char control, unsigned char* batData){//burst

     //IDENTIFICATION
        WriteSPI(chipId1);
        while(DataRdySPI()==0);
        WriteSPI(chipId0);
        while(DataRdySPI()==0);
        //CONTROL
        WriteSPI(control);//ugotovi kaj je treba nastaviti ata ?ipu
        while(DataRdySPI()==0);
        //DATA
        char r;
        for(r=0;r<14;r++)//size of data je lahko 1 2 ali 14
        {
         batData[r]=ReadSPI();//odvisno koliko je podatkov, najverjetneje sta potem potrebna minimalno 2 klica
        ;
        }
        while(DataRdySPI()==0);


}

unsigned char BatteryData[14];
int BatteryState[7];//imamo 7 vrednosti 6 baterij in 1 temperaturo

//funcija preme?e bite iz podatkov ki jih dobimo iz ata ?ipa
//potem so shranjeni v voltage
void rearangeBatteryData(void){
         unsigned char b;
         char counter=0;
         for(b=0;b<7;b++){
           
         // naredi nekaj z podatkom o vrednosti baterij najbolje da shrani? za po?iljanje z can
         //printf("batteri data%d:%d-%d\n",b,BatteryData[b],BatteryData[b+1]);
             
         BatteryState[b]=(BatteryData[counter]*256);
         counter++;
         BatteryState[b]|=(BatteryData[counter]);
         counter++;
         }
}


int main(void) {
  
    initApp();

    for(;;){

    unsigned char i;
    unsigned char chipId0=0;
    unsigned char chipId1=0;

    CHIP_SELECT=1;
 //   OpenSPI(SPI_FOSC_64&,MODE_01,SMPEND);
   // OpenSP
    for(i=0;i<NUMBER_OF_ATA_CHIPS;i++){
        if(i<8){
            chipId0=1<<(i);
            chipId1=0;
            }
        else {
            chipId0=0;
            chipId1=1<<(i-8);
        }
//control je 1111110
        
     
         askAtaForAllData(chipId1,chipId0,0xfe,BatteryData);

    }
    CloseSPI();
    CHIP_SELECT=0;


    // tu kli?i funkcije za can

    }

    for(;;);//nikoli ne smemo priti na konec
    return 0;
}
