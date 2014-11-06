/*
 * File:   newmain18.c
 * Author: uros
 *
 * Created on Sobota, 13 oktober 2012, 14:03
 */




//#define __18F4585
#define ECAN_IS_CAN_MODULE
#define USE_OR_MASKS
#define SPI_V1
#define USART
#define USART1
#define EAUSART_V3
#define SW_SPI_IO_V1
//#include <pic18.h>
//#include <htc.h>

#include <xc.h>
#include <plib/can2510.h>
#include <plib/usart.h>
//#include "J1939.H"


#include <plib/spi.h>
#include "can.h"
//#include <stdio.h>
//#include <plib/sw_spi.h>


/*
// CONFIG1H
#pragma config OSC = RCIO       // Oscillator Selection bits (External RC oscillator, port function on RA6)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Internal/External Oscillator Switchover bit (Oscillator Switchover mode enabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = BOHW     // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (VBOR set to 2.1V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config BBSIZ = 1024     // Boot Block Size Select bits (1K words (2K bytes) Boot Block)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)




 */



#pragma config OSC=XT, FCMEN=ON, WDT=OFF, IESO=ON, XINST=OFF, LVP=OFF
//#pragma config /*OSC=RC,*/ FCMEN=ON, WDT=OFF, IESO=ON, XINST=OFF, LVP=OFF
#define NUMBER_OF_ATA_CHIPS 1 //odvisno od velikosti verige
//2

#define CHIP_SELECT LATCbits.LATC0

unsigned char BatteryData[NUMBER_OF_ATA_CHIPS][14]; //zacasni podatki za spi zajem
int BatteryState[NUMBER_OF_ATA_CHIPS][7]; //imamo 7 vrednosti 6 baterij in 1 temperaturo
int BatteryStateOffset[NUMBER_OF_ATA_CHIPS][7]; //imamo 7 vrednosti 6 baterij in 1 temperaturo za referenco pri izra?unu
float BatteryVoltage[NUMBER_OF_ATA_CHIPS][6]; // rezultat
float Temperature[NUMBER_OF_ATA_CHIPS][2]; //2 tipali so, ampak beremo lahko na enkrat le enega
//unsigned char EmptyBateries[NUMBER_OF_ATA_CHIPS];
unsigned char Current = 10;// 1A polnilec ne mara float ampak je vse pomno?no z 10
//sck pri spi mora biti najve? 62.6khz
//sinhronizacija ata ?ipov pa 500khz



void initSpi(void) {
    CloseSPI(); //close if was on
    // OpenSPI(SPI_FOSC_64,MODE_01,SMPMID); //Odpirami in zapiramo sproti med pisanjem


}

void initApp(void) {

    TRISA = 0;
    TRISB = 0;
    TRISC = 0b10010000;
    TRISD = 0;

    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;

    //  OSCCONbits_t.IDLEN idle/sleep
    //  OSCCONbits_t.IRCF0 //speed from 8mhz to 32k
    //  OSCCONbits_t.IRCF1
    //  OSCCONbits_t.IRCF2
    //  OSCCONbits_t.OSTS ////oscilator timer ended
    //  OSCCONbits_t.IOFS //stable or not
    //  OSCCONbits_t.SCS0 //internal
    //  OSCCONbits_t.SCS1 //timer / primal oscilator

    //  OSCCON = 0b01110000; //8Mhz / internal
    //  OSCCON = 0b01100000; //4Mhz / internal
    OSCCON = 0b01010000; //2mhz / internal
    //  OSCCON = 0b01000000; //2mhz / internal
    //  OSCCON = 0b00110000; //500khz / internal
    //  OSCCON = 0b00100000; //250khz / internal
    //  OSCCON = 0b00010000; //125khz / internal
    //  OSCCON = 0b00000000; //31khz / internal
    //  LATCbits_t.LATC0=1; //external
    // LATC0 = 1;

    //   WDTCON=0x00;



    //OpenUSART( USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 51); //8Mhz
	//
    initSpi();
    // to je treba ?e pre?tudirat

//J1939_Initialization(J1939_TRUE);


    /*
10Mhz and 1Mb can-bus: impossible (higger pic fosc needed 16+Mhz)
10Mhz and 500Kb can-bus: CANInitialize(1,1,4,3,2,CAN_CONFIG_SAMPLE_ONCE & CAN_CONFIG_PHSEG2_PRG_OFF & CAN_CONFIG_LINE_FILTER_ON);
10Mhz and 250Kb can-bus: CANInitialize(1,2,4,3,2,CAN_CONFIG_SAMPLE_ONCE & CAN_CONFIG_PHSEG2_PRG_OFF & CAN_CONFIG_LINE_FILTER_ON);
10Mhz and 125Kb can-bus: CANInitialize(1,4,4,3,2,CAN_CONFIG_SAMPLE_ONCE & CAN_CONFIG_PHSEG2_PRG_OFF & CAN_CONFIG_LINE_FILTER_ON);

And 10 Mbit can-bus = flexray  (Philips SJA2510 and freescale MPC5567)

CanInitalize parameters depending form fosc and BRP:
PIC   and CAN: (SJW,BRP,PH1,PH2,PROP) because (NBT=SJW+PH1+PH2+PROP) and (PROP+PH1>PH2) and (PH2>SJW)
 5Mhz and 1000Kb: impossible (NBT= 2.5Tq -> not in range 8..25)
 5Mhz and 500Kb:  impossible (NBT= 5 Tq -> not in range 8..25)
 5Mhz and 250Kb: (1,1,4,3,2)
 5Mhz and 125Kb: (1,2,4,3,2)
10Mhz and 1000Kb: impossible (NBT = 5Tq -> not in range 8...25).
10Mhz and 500Kb: (1,1,4,3,2) because (10=1+4+3+2) and (2+4)>3 and 3>1
10Mhz and 250Kb: (1,2,4,3,2) because (10=1+4+3+2) and (2+4)>3 and 3>1
10Mhz and 125Kb: (1,4,4,3,2) because (10=1+4+3+2) and (2+4)>3 and 3>1
20Mhz and 1000Kb: (1,1,4,3,2)
20Mhz and 500Kb:  (1,2,4,3,2)
20Mhz and 250Kb:  (1,4,4,3,2)
20Mhz and 125Kb:  (1,8,4,3,2)
40Mhz and 2000Kb: (1,1,4,3,2) impossible, can-bus overspeeding
40Mhz and 1000Kb: (1,2,4,3,2)
40Mhz and 500Kb:  (1,4,4,3,2)
40Mhz and 250Kb:  (1,8,4,3,2)
40Mhz and 125Kb:  (1,16,4,3,2)
80Mhz and 4000Kb: (1,1,4,3,2) impossible, pic & can-bus overspeeding
80Mhz and 2000Kb: (1,2,4,3,2) impossible, pic & can-bus overspeeding
80Mhz and 1000Kb: (1,4,4,3,2) impossible, pic overspeeding
80Mhz and 500Kb:  (1,8,4,3,2) impossible, pic overspeeding
80Mhz and 250Kb:  (1,16,4,3,2) impossible, pic overspeeding
80Mhz and 125Kb:  (1,32,4,3,2) impossible, pic overspeeding

 4Mhz and 1000Kb: impossible
 4Mhz and 500Kb:  impossible
 4Mhz and 250Kb: (1,1,3,2,2)
 4Mhz and 125Kb: (1,1,3,2,2)
 8Mhz and 1000Kb: impossible
 8Mhz and 500Kb: (1,1,3,2,2)
 8Mhz and 250Kb: (1,2,3,2,2)
 8Mhz and 125Kb: (1,4,3,2,2)
16Mhz and 1000Kb: (1,1,3,2,2)
16Mhz and 500Kb: (1,2,3,2,2)
16Mhz and 250Kb: (1,4,3,2,2)
16Mhz and 125Kb: (1,8,3,2,2)
32Mhz and 2000Kb: (1,1,3,2,2) impossible, can-bus overspeeding
32Mhz and 1000Kb: (1,2,3,2,2)
32Mhz and 500Kb:  (1,4,3,2,2)
32Mhz and 250Kb:  (1,8,3,2,2)
32Mhz and 125Kb:  (1,16,3,2,2)
64Mhz and 4000Kb: (1,1,3,2,2) impossible, pic and can-bus overspeeding
64Mhz and 2000Kb: (1,2,3,2,2) impossible, pic and can-bus overspeeding
64Mhz and 1000Kb: (1,4,3,2,2) impossible, pic overspeeding
64Mhz and 500Kb:  (1,8,3,2,2) impossible, pic overspeeding
64Mhz and 250Kb:  (1,16,3,2,2) impossible, pic overspeeding
64Mhz and 125Kb:  (1,32,3,2,2) impossible, pic overspeeding */
    CANInitialize(1,2,3,2,2 , CAN_CONFIG_XTD_MSG);

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

char emptyCells(unsigned char chip,unsigned char chipId1, unsigned char chipId0, int treshold, int** data) {
    char action = 0;
    if (/*BatteryState*/data[chip][0] < treshold) {
        action |= 0x01;
    }
    if (/*BatteryState*/data[chip][1] < treshold) {
        action |= 0x02;
    }
    if (/*BatteryState*/data[chip][2] < treshold) {
        action |= 0x04;
    }
    if (/*BatteryState*/data[chip][3] < treshold) {
        action |= 0x08;
    }
    if (/*BatteryState*/data[chip][4] < treshold) {
        action |= 0x10;
    }
    if (/*BatteryState*/data[chip][5] < treshold) {
        action |= 0x20;
    }
    return action;
}

void dischargeCells(unsigned char chip,unsigned char chipId1, unsigned char chipId0, int treshold, int** data) {
    char action = 0x00;

    if (/*BatteryState*/data[chip][0] > treshold) {
        action |= 0x01;
    }
    if (/*BatteryState*/data[chip][1] > treshold) {
        action |= 0x02;
    }
    if (/*BatteryState*/data[chip][2] > treshold) {
        action |= 0x04;
    }
    if (/*BatteryState*/data[chip][3] > treshold) {
        action |= 0x08;
    }
    if (/*BatteryState*/data[chip][4] > treshold) {
        action |= 0x10;
    }
    if (/*BatteryState*/data[chip][5] > treshold) {
        action |= 0x20;
    }

    if (action != 0x00) {
        WriteSPI(chipId1);//chip
        while (DataRdySPI() == 0);
        WriteSPI(chipId0);//address
        while (DataRdySPI() == 0);
        WriteSPI(0x13); //CONTROL
        while (DataRdySPI() == 0);
        WriteSPI(action); //DATA
        while (DataRdySPI() == 0);
    } else {
        WriteSPI(chipId1);
        while (DataRdySPI() == 0);
        WriteSPI(chipId0);
        while (DataRdySPI() == 0);
        WriteSPI(0x13); //CONTROL
        while (DataRdySPI() == 0);
        WriteSPI(0x00); //DATA
        while (DataRdySPI() == 0);
    }
}

void askAtaDataRd16(unsigned char chipId1, unsigned char chipId0, unsigned char *data) {//posamezni dostop
     CHIP_SELECT = 1;
     OpenSPI(SPI_FOSC_64, MODE_01, SMPMID);

    //IDENTIFICATION
    WriteSPI(chipId1);
    while (DataRdySPI() == 0);
    WriteSPI(chipId0);
    while (DataRdySPI() == 0);
    //CONTROL
    WriteSPI(0x22); //ugotovi kaj je treba nastaviti ata ?ipu //0x22
    while (DataRdySPI() == 0);
    //DATA

    //while (DataRdySPI() == 0);
    data[1] = ReadSPI(); //potrebna 2 klica
    while (DataRdySPI() == 0);
    data[0] = ReadSPI();

      CloseSPI();
    CHIP_SELECT = 0;
}

void askAtaForAllData(unsigned char chip, unsigned char chipId1, unsigned char chipId0, unsigned char control/*, unsigned char** data*/) {//burst

    CHIP_SELECT = 1;
    OpenSPI(SPI_FOSC_64, MODE_01, SMPMID);
    //IDENTIFICATION
    WriteSPI(chipId1);
    while (DataRdySPI() == 0);
    WriteSPI(chipId0);
    while (DataRdySPI() == 0);
    //CONTROL
    WriteSPI(control); //ugotovi kaj je treba nastaviti ata ?ipu
    while (DataRdySPI() == 0);
    //DATA
    char r;
    for (r = 0; r < 14; r++)//size of data je lahko 1 2 ali 14
    {
        //  WriteUSART('A');
        BatteryData[chip][r] = ReadSPI(); //odvisno koliko je podatkov, najverjetneje sta potem potrebna minimalno 2 klica
       // data[chip][r] = ReadSPI(); //odvisno koliko je podatkov, najverjetneje sta potem potrebna minimalno 2 klica

        //   WriteUSART('a');
        // while (DataRdySPI() == 0);
        // ;
    }

    CloseSPI();
    CHIP_SELECT = 0;
}



//funcija preme?e bite iz podatkov ki jih dobimo iz ata ?ipa
//potem so shranjeni v voltage

void rearangeBatteryData(unsigned char chip, int **out) {
    unsigned char counter, b;
    counter = 0;
    for (b = 0; b < 7; b++) {

        // naredi nekaj z podatkom o vrednosti baterij najbolje da shrani? za po?iljanje z can
        //printf("batteri data%d:%d-%d\n",b,BatteryData[b],BatteryData[b+1]);

        out[chip][b] = (BatteryData[chip][counter]*256);
        counter++;
        out[chip][b] |= (BatteryData[chip][counter]);
        counter++;
    }

}

void voltageCalculation(float** vbat, int** acq, int** offset)
{
    char b, c;
    //float tmp0, tmp1;
    for(c=0;c<NUMBER_OF_ATA_CHIPS;c++)
    {
        for (b=0;b<6;b++)
        {
       //     tmp0 =(float)acq[c][b]; // casting data from adc - value to float
       //     tmp1 =(float)offset[c][b];
       //     vbat[c][b] = ((tmp0-tmp1)/(3031-tmp1) * 4);
        vbat[c][b] = (((float)acq[c][b]-(float)offset[c][b])/(3031f-(float)offset[c][b]) * 4);
        }
    }
}

int main(void) {

    // USART_SINGLE_RX;
    //USART_SYNCH_MODE;
    //-----configure USART -----

   // OpenUSART(USART_TX_INT_OFF | USART_RX_INT_OFF | USART_ASYNCH_MODE | USART_EIGHT_BIT | USART_CONT_RX | USART_BRGH_LOW, 51);


 //   baudUSART(BAUD_8_BIT_RATE | BAUD_AUTO_OFF);


    unsigned char read_or_send = 0;

    initApp();

    for (;;) {

        unsigned char chip;
        unsigned char chipId0 = 0;
        unsigned char chipId1 = 0;

        //CHIP_SELECT = 1;
        //   OpenSPI(SPI_FOSC_64&,MODE_01,SMPEND);

        if (read_or_send == 0) {

            for (chip = 0; chip < NUMBER_OF_ATA_CHIPS; chip++) { //genereramo 2 bajta za naslov ata ?ipa
                if (chip < 8) {
                    chipId0 = 1 << (chip);
                    chipId1 = 0;
                } else {
                    chipId0 = 0;
                    chipId1 = 1 << (chip - 8);
                }

                //control je 1111110
                //control bit 0 je za dolocanje smeri pisanje:1 branje:0
                //askAtaForAllData(chipId1, chipId0, 0x7f);
                askAtaForAllData(chip, chipId1, chipId0, 0xfe); //burst dokumentacija ni konsistentna ponekod je 0x7f drugje 0xfe
                rearangeBatteryData(chip,(int**)&BatteryState);

                askAtaForAllData(chip, chipId1, chipId0, 0xfe); //referen?na za izra?un
                rearangeBatteryData(chip,(int**)&BatteryStateOffset);//pretvorba prebranih vrednosti iz 2x char v 1x int
            

                voltageCalculation(BatteryVoltage,BatteryState, BatteryStateOffset);
             //  preveri katere so baterije so prazne
            // char empty = emptyCells(NEKA VREDNOST nastavi sati?no)
           // char emptyCells(chip, chipId1, chipId0, 350, int** data) ;

                // sedaj ima? vrednost o praznih celicah
            //

            // ?e je-bo potrebno praznjenje... po?lji najprej na can da neha polnit!!!
            // dischargeCells(chip,chipId1,chipId0, treshold je NEKA VREDNOST nastavi stati?no)
//void dischargeCells(unsigned char chip,unsigned char chipId1, unsigned char chipId0, int treshold, int** data) {


            }
            read_or_send=1;
            
        } else {
            //tukaj po?iljamo na can
            //vrednosti za napetost in tok so pomne?ene za 10!

         //   J1939_MESSAGE Msg[4];

            //tukj prera?unamo napetost in poljemo napetost na polnilc
            //prepra?unamo tudi kak?nen tok rabimon(?e je razmeroma prazno z polnim tokom
            //(naprimer 1A) potem zni?ujemo po recimo 25mA...
            //(ko bomo imeli tudi izra?une temperature polnimo z samo naprimer 250mA))
            //in to po?ljemo na polnilec

            //!!!CE PRAZNIMO NAPREJ IZKLOPI POLNILEC!!!

          //  J1939_MESSAGE* MsgPtr = Msg;
            //funkcija za dodajanje v vrsto
           // J1939_EnqueueMessage( *MsgPtr );

            //funkcija za po?iljanje
            //J1939_TransmitMessages();





            //nova knji?nica(ne j1939)
            //nekatere nastavitve glej http://www.microchip.com/forums/m171530-print.aspx
            // polnilc po can zahteva maksimalno 250bps pri
            //8mhz (1,2,3,2,2)
            // 16mhz  (1,4,3,2,2)
            // 32mhz (1,8,3,2,2)
            // 4mhz (1,1,3,2,2)


            read_or_send=0;
        }

    }



return 0;
}



