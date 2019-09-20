/* 
 * File:   main.c
 * Author: pguz1
 *
 * Created on 2019/01/02, 22:38
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <p33FJ128MC802.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "Controller_Protocol.h"
#include "communication.h"
#include "variable.h"
#include "dsPIC33FJ128MC802.h"
/* Delay */
#define FCY 4000000UL   /*　delay関数用定義 Forc/2　*/
#include <libpic30.h>   /*　delay関数用　*/

/*-----------------------------------------------
 * 
 * 基板毎の設定
 * 
-----------------------------------------------*/
uint8_t MY_ADDRESS = 0x80; /* この基板のアドレス */
#define ENCODER_POL FORWARD /* エンコーダの極性（FORWARD or REVERSE） */
#define ENCODER_KIND NEW    /* エンコーダの種類（NEW or OLD） */


/*-----------------------------------------------
 * Define
-----------------------------------------------*/
/* ControllerProtocol */
#define bus1 0

/* debug */
#define debug_LED0 LATBbits.LATB13
#define debug_LED1 LATBbits.LATB14

#define debug_LED2 LATBbits.LATB10
#define debug_LED3 LATBbits.LATB11
#define debug_LED4 LATBbits.LATB12

/* 非常停止信号出力 */
#define emargency0 LATAbits.LATA3
#define emargency1 LATBbits.LATB4
#define emargency2 LATBbits.LATB5
#define emargency3 LATAbits.LATA4


/* エンコーダ */
#if ENCODER_KIND == OLD
#define RESOLUTION 300
#elif ENCODER_KIND == NEW
#define RESOLUTION 384
#else
#error "invalid ENCODER_KIND"
#endif

#define TIMER_1MS 39201 /* 1 [ms] 分のカウント */

/* 通信用 */
#define UP(data) (((data) >> 8) & 0xFF)
#define LOW(data) ((data) & 0xFF)
#define ASBL(up, low) ((short)((up) << 8) | (low)) /* Assemble */

/*-----------------------------------------------
 * プロトタイプ宣言
-----------------------------------------------*/
void Initialize();
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void);

/*-----------------------------------------------
 * グローバル変数
-----------------------------------------------*/
/* 通信フラグ */
bool TxFrag = 1;
bool rx_has_started = 0; /* 受信開始したフラブ */
int status = 0, status2 = 0;
unsigned char sta2 = 0;

/* タイムアウト用カウント変数 */
unsigned char Rx1_count = 0, Rx2_count = 0;

int main(int argc, char** argv) {
    Initialize(); /* 初期化 */

    //    int i;
    //    short           Rev[2] = {0, 0},                /* 回転 */
    //                    Residue[2] = {0, 0},            /* １回転に満たないカウント */
    //                    IntCnt[2] = {0, 0};             /* 割り込み回数 */
    //    bool            Reset[2] = {false, false};      /* カウントクリアフラグ */
    //    uint16_t        pre_POSCNT[2] = {0, 0};         /* １サイクル前の POSxCNT を保存する */
    //    const short     Coef = RESOLUTION * 4;          /* 回転を求めるときに使用する係数（coefficient） */



    while (1) {
        /* ??? */
        //        __delay_ms(5);

        /*-----------------------------------------------
         * 受信
        -----------------------------------------------*/

        Reception_from_master_main();


        Reception_from_emargency_main();


        debug_LED3 = RxData0[0].d0;
        debug_LED4 = RxData1[0].d0;

        if ((RxData0[0].all_data == 1) && (RxData1[0].all_data == 1)) {
            debug_LED2 = 1;

            emargency0 = 1;
            emargency1 = 1;
            emargency2 = 1;
            emargency3 = 1;
        } else {
            debug_LED2 = 0;

            emargency0 = 0;
            emargency1 = 0;
            emargency2 = 0;
            emargency3 = 0;
        }


        //        if(QEI1IF || QEI2IF) debug_LED1 ^= 1;
        //        
        //        /*-----------------------------------------------
        //         * 割り込み回数をカウントする
        //        -----------------------------------------------*/
        //        if(QEI1IF == true)
        //        {
        //            /* 割り込み方向を判断する */
        //            if(POS1CNT < pre_POSCNT[0]) IntCnt[0]++;
        //            else if(POS1CNT > pre_POSCNT[0]) IntCnt[0]--;
        //            QEI1IF = false;
        //        }
        //        if(QEI2IF == true)
        //        {
        //            /* 割り込み方向を判断する */
        //            if(POS2CNT < pre_POSCNT[1]) IntCnt[1]++;
        //            else if(POS2CNT > pre_POSCNT[1]) IntCnt[1]--;
        //            QEI2IF = false;
        //        }
        //        
        //        /*-----------------------------------------------
        //         * POSxCNT と IntCnt から Rev と Residue 計算する
        //        -----------------------------------------------*/
        //        Rev[0] = (IntCnt[0] * 65536 + POS1CNT) / Coef;
        //        Residue[0] = ((IntCnt[0] * 65536 + POS1CNT) % Coef) * 360 / Coef;   /* [cnt] → [deg] */
        //        Rev[1] = (IntCnt[1] * 65536 + POS2CNT) / Coef;
        //        Residue[1] = ((IntCnt[1] * 65536 + POS2CNT) % Coef) * 360 / Coef;   /* [cnt] → [deg] */
        //        
        //        /*-----------------------------------------------
        //         * pre_POSCNT を更新
        //        -----------------------------------------------*/
        //        pre_POSCNT[0] = POS1CNT;
        //        pre_POSCNT[1] = POS2CNT;        
        //        
        //        /*-----------------------------------------------
        //         * 送信
        //        -----------------------------------------------*/
        //        if (TxFrag)
        //        {   
        //            TxData0[0] = UP(Rev[0]);
        //            TxData0[1] = LOW(Rev[0]);
        //            TxData0[2] = UP(Residue[0]);
        //            TxData0[3] = LOW(Residue[0]);
        //            TxData0[4] = UP(Rev[1]);
        //            TxData0[5] = LOW(Rev[1]);
        //            TxData0[6] = UP(Residue[1]);
        //            TxData0[7] = LOW(Residue[1]);
        //            
        //            Send_StartSignal(EUSART_Write, EUSART_TxInterrupt_Control, U2TXIE);
        //        }
    }
    return (EXIT_SUCCESS);
}

//void EUSART_Write(unsigned char data)
//{
//    U2TXREG = data;
//}
//
//void EUSART_TxInterrupt_Control(bool enable_or_disable)
//{
//    U2TXIE = enable_or_disable;
//}

//void __attribute__((interrupt, no_auto_psv)) _U2TXInterrupt()
//{
//    debug_LED2 ^= 1;
//    TxFrag = Send_Till_EndSignal(TxData0, EUSART_Write, EUSART_TxInterrupt_Control, number_of_txdata0, bus1);
//    U2TXIF = disable;
//}

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt() {
    U1RXIF = 0;

    status = Reception_from_master();
    Rx1_count = 0;
    debug_LED0 ^= 1;
}

void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt() {
    U2RXIF = 0;
    status2 = Reception_from_emargency();
    Rx2_count = 0;
    debug_LED1 ^= 1;
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    int i = 0;

    Rx1_count++;
    Rx2_count++;

    if (Rx1_count > 50) {
        Rx1_count = 0;
        for (i = 0; i < number_of_rxdata0; i++) {
            RxData0[i].all_data = 0;
        }

        for (i = 0; i < number_of_rxdata0 * 2 + 3; i++) {
            Buffer0[i].all_data = 0;
        }
        debug_LED0 ^= 1;

    }

    if (Rx2_count > 50) {
        Rx2_count = 0;
        for (i = 0; i < number_of_rxdata1; i++) {
            RxData1[i].all_data = 0;
        }

        for (i = 0; i < number_of_rxdata1 * 2 + 3; i++) {
            Buffer1[i].all_data = 0;
        }
        debug_LED1 ^= 1;
    }

    T1IF = 0;
    PR1 = TIMER_1MS;
}

void Initialize() {
    Initialize_Parameters();

    /**********************
     * 
     *  オシレータ設定
     * 
     **********************/
    OSCTUN = 18;
    PLLFBD = 40 - 2; //M=18
    CLKDIVbits.PLLPRE = 0; //PLL prescaler : /2  = N1
    CLKDIVbits.PLLPOST = 0; //PLL postscaler : /2 = N2
    //#pragma config FNOSC = FRC より　Fin=7.37MHz
    /* ↑ より　Fosc=Fin*(M/(N1*N2)) =78.4MHz≒80MHz
     Fcy≒40MHz=80MHz/2 */

    /*Initiate Clock Switch to Fast RC oscillator with PLL(NOSC=0b001)*/
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(0x01);

    /*Wait for Clock switch to occur
     * (current oscillator が0b001＝FRCでない間は待機)*/
    while (OSCCONbits.COSC != 0b001);

    /*Wait for PLL to lock*/
    while (OSCCONbits.LOCK != 1);


    /******************************
     * 
     *  割り込み設定
     * 
     * **************************** */
#if 1 //割り込みする:1 しない:0
    /* 割り込み基本設定 */
    SRbits.IPL = 0; // CPU interrupt priority level:zero
    CORCONbits.IPL3 = 0; //CPU interrupt priority level:<7
    INTCON1bits.NSTDIS = 0; //割り込みのネスティング有効（1：無効　0：有効）
    INTCON2bits.ALTIVT = 0; //use primary vector table

    /* 割り込み有効・無効 */
    U1RXIE = enable;
    U2RXIE = enable;
    T1IE = enable; /*タイマ1割り込み有効 */

    /* 割り込み優先度　7が最優先 */
    U2RXIP = 0x7;
    U1RXIP = 0x6;
    T1IP = 0x5; //set interrupt priority level
#endif

    /******************************
     * 
     *  ピン設定
     * 
     * **************************** */

    TRISBbits.TRISB2 = 0; //debug1
    TRISBbits.TRISB3 = 0; //debug2

    TRISBbits.TRISB15 = 1; //UART1 reception
    U1RXR = 0xF; //RP15 - UART Reception

    TRISBbits.TRISB7 = 1; //UART2 reception
    U2RXR = 0x7; //RP07 - UART Reception

    //    RPOR7bits.RP14R = 0x000; //RP14 I/O
    //    RPOR6bits.RP13R = 0x000;
    //    RPOR6bits.RP12R = 0x000;
    //    RPOR5bits.RP11R = 0x000;

    TRISBbits.TRISB10 = 0; //LED
    TRISBbits.TRISB11 = 0; //LED
    TRISBbits.TRISB12 = 0; //LED

    TRISBbits.TRISB13 = 0; //通信確認用LED 
    TRISBbits.TRISB14 = 0; //通信確認用LED 

    TRISAbits.TRISA3 = 0; //非常停止信号用
    TRISAbits.TRISA4 = 0; //非常停止信号用
    TRISBbits.TRISB4 = 0; //非常停止信号用
    TRISBbits.TRISB5 = 0; //非常停止信号用

    AD1PCFGL = 0xFFFF; /* すべてデジタル入出力 */

    /*****************
     * 
     * UART 1
     ******************/
    U1MODE = 0x0808;
    U1STA = 0x2000;
    U1RXIF = disable; /* 割り込みフラグクリア */
    U1BRG = 165; /* ボーレート設定 */
    UART1EN = enable; /* UART1有効 */


    /*****************
     * 
     * UART 2
     ******************/
    U2MODE = 0x0808;
    U2STA = 0x2000;
    U2RXIF = disable; /* 割り込みフラグクリア */
    U2BRG = 165; /* ボーレート設定 */
    UART2EN = enable; /* UART1有効 */


    /******************
     * 
     *  Timer1
     * 
     * *************** */
    T1ON = ON; //Timer on   
    T1CONbits.TCS = T1CONbits.TGATE = T1CONbits.TSYNC = 0; //Timer mode
    T1CONbits.TSIDL = 0; // アイドルモードでも動作を継続
    T1CKPS = 0x00; //プリスケール 1:1
    TMR1 = 0x0;
    PR1 = TIMER_1MS;

}