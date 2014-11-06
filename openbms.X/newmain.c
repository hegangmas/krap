/*
   File:   newmain.c
   Author: uros

   Created on Sobota, 27 oktober 2012, 0:01
 */



//#include<plib/usart.h>
//#include <plib/spi.h>
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
}

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
#include <pic16/pic18f4585.h>
#include <plib/spi.h>


void main(void) {
    //Infinite loop


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
        unsigned char b;
         askAtaForAllData(chipId1,chipId0,0xfe,BatteryData);
         for(b=0;b<14;b++){
             b++;
            // naredi nekaj z podatkom o vrednosti baterij najbolje da shrani? za po?iljanje z can
             //printf("batteri data%d:%d-%d\n",b,BatteryData[b],BatteryData[b+1]);

             ;
         }

    }
    CloseSPI();
    CHIP_SELECT=0;


    // tu kli?i funkcije za can

    }


    for (;;) {

    }
}

void highInterrupt(void) interrupt 1 {

}

void lowInterrupt(void) interrupt 2 {

}
