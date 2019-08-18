
/*
 * File:   newmain.c
 * Author: shuic
 *
 * Created on 2019/08/06, 9:17
 */

#include <xc.h>
#include "Controller_Protocol.h"
#include "stdbool.h"

#include <stdio.h>
#include <stdint.h>

//プロトタイプ宣言//
//LCD関数//

void interrupt INTERRUPT_InterruptManager(void);
void EUSART_Write(unsigned char);
void EUSART_TxInterrupt_Control(unsigned char);
void data_input(uint8_t);
#define N 100
#define M 5
//void data_input(uint8_t);
//uint8_t new_sensor_data[2][26];
//bool FaB = 1, new_data_flag = 0;

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator mode selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT32 // Power-up default value for COSC bits (HFINTOSC with OSCFRQ= 32 MHz and CDIV = 1:1)
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = OFF      // Clock Switch Enable bit (The NOSC and NDIV bits cannot be changed by user software)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (FSCM timer disabled)

// CONFIG2
#pragma config MCLRE = OFF      // Master Clear Enable bit (MCLR pin function is port defined function)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = OFF      // Brown-out reset enable bits (Brown-out reset disabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = OFF    // Peripheral Pin Select one-way control (The PPSLOCK bit can be set and cleared repeatedly by software)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will not cause a reset)


// CONFIG3
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled, SWDTEN is ignored)
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4
#pragma config BBSIZE = BB512   // Boot Block Size Selection bits (512 words boot block size)
#pragma config BBEN = OFF       // Boot Block Enable bit (Boot Block disabled)
#pragma config SAFEN = OFF      // SAF Enable bit (SAF disabled)
#pragma config WRTAPP = OFF     // Application Block Write Protection bit (Application Block not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not write protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration Register not write protected)
#pragma config WRTSAF = OFF     // Storage Area Flash Write Protection bit (SAF not write protected)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (High Voltage on MCLR/Vpp must be used for programming)

// CONFIG5
#pragma config CP = OFF         // UserNVM Program memory code protection bit (UserNVM code protection disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//#include <xc.h>

#define _XTAL_FREQ 32000000

void PIN_MANAGER_Initialize(void) {
    /**
    LATx registers
     */
    LATE = 0x00;
    LATD = 0x00;
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    //    LATCbits.LATC2=0;

    /**
    TRISx registers
     */
    TRISE = 0x07;
    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0xF8;
    TRISD = 0xFD;
    TRISDbits.TRISD6 = 1;
    //    TRISDbits.TRISD1 = 0;

    /**
    ANSELx registers
     */
    ANSELC = 0x00;
    ANSELB = 0x00;
    ANSELD = 0x00;
    ANSELE = 0x00;
    ANSELA = 0x00;

    /**
    WPUx registers
     */
    WPUD = 0x00;
    WPUE = 0x00;
    WPUB = 0x00;
    WPUA = 0x00;
    WPUC = 0x00;

    /**
    ODx registers
     */
    ODCONE = 0x00;
    ODCONA = 0x00;
    ODCONB = 0x00;
    ODCONC = 0x00;
    ODCOND = 0x00;


    /**
    IOCx registers
     */
    // interrupt on change for group IOCAF - flag
    IOCAFbits.IOCAF0 = 0;
    // interrupt on change for group IOCAN - negative
    IOCANbits.IOCAN0 = 1;
    // interrupt on change for group IOCAP - positive
    IOCAPbits.IOCAP0 = 1;



    //configration Bits
    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD2CON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC2STA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX2STA = 0x24;

    // Baud Rate = 57600; SP2BRGL 138; 
    SP2BRGL = 0x8A;

    // Baud Rate = 57600; SP2BRGH 0; 
    SP2BRGH = 0x00;

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC1STA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX1STA = 0x24;

    // Baud Rate = 57600; SP1BRGL 138; 
    SP1BRGL = 0x8A;
    // Baud Rate = 57600; SPBRGH 0; 
    SP1BRGH = 0x00;

    INTCONbits.PEIE = 1;
    PIE3bits.RC2IE = 1;
    PIE3bits.RC1IE = 1;

    //    TRISAbits.TRISA2 = 0;

    bool state = GIE;
    GIE = 1;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    ////EUSART_SSP////
    //    RC3PPS = 0x0F; //RC2->EUSART1:TX1;
    RX1DTPPSbits.RX1DTPPS = 0x08; //RC3->EUSART2:RX1;
    //    RD1PPS = 0x11; //RD1->EUSART2:TX2;
    RX2DTPPSbits.RX2DTPPS = 0x1F; //RD0->EUSART2:RX2;
    //RD6PPS = 0x0C;   //RD6->PWM4:PWM4OUT;a


    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS

    GIE = 1;

    // NOSC HFINTOSC; NDIV 1; 
    OSCCON1 = 0x60;
    // CSWHOLD may proceed; SOSCPWR Low power; 
    OSCCON3 = 0x00;
    // MFOEN disabled; LFOEN disabled; ADOEN disabled; SOSCEN disabled; EXTOEN disabled; HFOEN disabled; 
    OSCEN = 0x00;
    // HFFRQ 32_MHz; 
    OSCFRQ = 0x06;
    // MFOR not ready; 
    OSCSTAT = 0x00;
    // HFTUN 0; 
    OSCTUNE = 0x00;


}

void setup_TMR1(void) {
    T1CON = 0;
    T1CONbits.nSYNC = 1;
    T1GCON = 0x00;
    T1GATE = 0x00;
    T1CLK = 0x01;
    T1CONbits.ON = 1;
}

void main(void) {

    int display_reset_count = 0;

    OSCSTATbits.PLLR = 1;

    OSCFRQ = 0x06;

    Initialize_Parameters();

    PIN_MANAGER_Initialize();
    LATCbits.LATC0 = 0;
    LATCbits.LATC1 = 0;
    setup_TMR1();


    LATAbits.LATA3 = 1;
    TRISDbits.TRISD0 = 1;
    TRISDbits.TRISD6 = 1;

    while (1) {
        if (RxData0[0].all_data == 1 && RxData1[0].all_data == 1) {
            LATCbits.LATC1 = 1;
            LATDbits.LATD0 = 1;
        } else if(RxData0[0].all_data == 0||RxData1[0].all_data == 0){
            LATCbits.LATC1 = 0;
            LATDbits.LATD0 = 0;
        }
        /*LED*/
        LATCbits.LATC0 = RxData0[0].all_data; /*点灯：運転中*/
        LATCbits.LATC2 = RxData1[0].all_data; /*点灯：運転中*/

        Organize_Datas(RxData0, Buffer0, number_of_rxdata0, 0);
        Organize_Datas(RxData1, Buffer1, number_of_rxdata1, 1);
    }

}


//割り込み関数//  

void interrupt INTERRUPT_InterruptManager(void) {

    if (INTCONbits.PEIE == 1 && PIE3bits.RC2IE == 1 && PIR3bits.RC2IF == 1) {
        //        //EUSART2_Transmit_ISR();

        Store_Datas(Buffer1, RC2REG, number_of_rxdata1 * 2 + 3, 1);
        PIR3bits.RC2IF = 1;

    } else if (INTCONbits.PEIE == 1 && PIE3bits.TX2IE == 1 && PIR3bits.TX2IF == 1) {
        Send_Till_EndSignal(TxData0, EUSART_Write, EUSART_TxInterrupt_Control, number_of_txdata0, 0);
        PIR3bits.TX2IF = 0;

    } else if (INTCONbits.PEIE == 1 && PIE3bits.RC1IE == 1 && PIR3bits.RC1IF == 1) {

        Store_Datas(Buffer0, RC1REG, number_of_rxdata0 * 2 + 3, 0);
        PIR3bits.RC1IF = 1;

    } else if (INTCONbits.PEIE == 1 && PIE3bits.TX1IE == 1 && PIR3bits.TX1IF == 1) {

    }
}

void EUSART_Write(unsigned char txData) {
    TX2REG = txData;

}

void EUSART_TxInterrupt_Control(unsigned char enable_or_disable) {
    PIE3bits. TX2IE = enable_or_disable;
}


/*----確認用LED----
 * 
 * RC0 : 無線非常が押されていないとき　（点灯）
 * RC1 : 非常停止したとき（消灯）
 * RC2 : ラズパイの停止スイッチが押されていないとき　（点灯）
 * 
 */


