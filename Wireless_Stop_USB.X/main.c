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
#define FCY 4000000UL   /*�@delay�֐��p��` Forc/2�@*/
#include <libpic30.h>   /*�@delay�֐��p�@*/

/*-----------------------------------------------
 * 
 * ����̐ݒ�
 * 
-----------------------------------------------*/
uint8_t MY_ADDRESS = 0x80; /* ���̊�̃A�h���X */
#define ENCODER_POL FORWARD /* �G���R�[�_�̋ɐ��iFORWARD or REVERSE�j */
#define ENCODER_KIND NEW    /* �G���R�[�_�̎�ށiNEW or OLD�j */


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

/* ����~�M���o�� */
#define emargency0 LATAbits.LATA3
#define emargency1 LATBbits.LATB4
#define emargency2 LATBbits.LATB5
#define emargency3 LATAbits.LATA4


/* �G���R�[�_ */
#if ENCODER_KIND == OLD
#define RESOLUTION 300
#elif ENCODER_KIND == NEW
#define RESOLUTION 384
#else
#error "invalid ENCODER_KIND"
#endif

#define TIMER_1MS 39201 /* 1 [ms] ���̃J�E���g */

/* �ʐM�p */
#define UP(data) (((data) >> 8) & 0xFF)
#define LOW(data) ((data) & 0xFF)
#define ASBL(up, low) ((short)((up) << 8) | (low)) /* Assemble */

/*-----------------------------------------------
 * �v���g�^�C�v�錾
-----------------------------------------------*/
void Initialize();
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void);
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void);

/*-----------------------------------------------
 * �O���[�o���ϐ�
-----------------------------------------------*/
/* �ʐM�t���O */
bool TxFrag = 1;
bool rx_has_started = 0; /* ��M�J�n�����t���u */
int status = 0, status2 = 0;
unsigned char sta2 = 0;

/* �^�C���A�E�g�p�J�E���g�ϐ� */
unsigned char Rx1_count = 0, Rx2_count = 0;

int main(int argc, char** argv) {
    Initialize(); /* ������ */

    //    int i;
    //    short           Rev[2] = {0, 0},                /* ��] */
    //                    Residue[2] = {0, 0},            /* �P��]�ɖ����Ȃ��J�E���g */
    //                    IntCnt[2] = {0, 0};             /* ���荞�݉� */
    //    bool            Reset[2] = {false, false};      /* �J�E���g�N���A�t���O */
    //    uint16_t        pre_POSCNT[2] = {0, 0};         /* �P�T�C�N���O�� POSxCNT ��ۑ����� */
    //    const short     Coef = RESOLUTION * 4;          /* ��]�����߂�Ƃ��Ɏg�p����W���icoefficient�j */



    while (1) {
        /* ??? */
        //        __delay_ms(5);

        /*-----------------------------------------------
         * ��M
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
        //         * ���荞�݉񐔂��J�E���g����
        //        -----------------------------------------------*/
        //        if(QEI1IF == true)
        //        {
        //            /* ���荞�ݕ����𔻒f���� */
        //            if(POS1CNT < pre_POSCNT[0]) IntCnt[0]++;
        //            else if(POS1CNT > pre_POSCNT[0]) IntCnt[0]--;
        //            QEI1IF = false;
        //        }
        //        if(QEI2IF == true)
        //        {
        //            /* ���荞�ݕ����𔻒f���� */
        //            if(POS2CNT < pre_POSCNT[1]) IntCnt[1]++;
        //            else if(POS2CNT > pre_POSCNT[1]) IntCnt[1]--;
        //            QEI2IF = false;
        //        }
        //        
        //        /*-----------------------------------------------
        //         * POSxCNT �� IntCnt ���� Rev �� Residue �v�Z����
        //        -----------------------------------------------*/
        //        Rev[0] = (IntCnt[0] * 65536 + POS1CNT) / Coef;
        //        Residue[0] = ((IntCnt[0] * 65536 + POS1CNT) % Coef) * 360 / Coef;   /* [cnt] �� [deg] */
        //        Rev[1] = (IntCnt[1] * 65536 + POS2CNT) / Coef;
        //        Residue[1] = ((IntCnt[1] * 65536 + POS2CNT) % Coef) * 360 / Coef;   /* [cnt] �� [deg] */
        //        
        //        /*-----------------------------------------------
        //         * pre_POSCNT ���X�V
        //        -----------------------------------------------*/
        //        pre_POSCNT[0] = POS1CNT;
        //        pre_POSCNT[1] = POS2CNT;        
        //        
        //        /*-----------------------------------------------
        //         * ���M
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
     *  �I�V���[�^�ݒ�
     * 
     **********************/
    OSCTUN = 18;
    PLLFBD = 40 - 2; //M=18
    CLKDIVbits.PLLPRE = 0; //PLL prescaler : /2  = N1
    CLKDIVbits.PLLPOST = 0; //PLL postscaler : /2 = N2
    //#pragma config FNOSC = FRC ���@Fin=7.37MHz
    /* �� ���@Fosc=Fin*(M/(N1*N2)) =78.4MHz��80MHz
     Fcy��40MHz=80MHz/2 */

    /*Initiate Clock Switch to Fast RC oscillator with PLL(NOSC=0b001)*/
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(0x01);

    /*Wait for Clock switch to occur
     * (current oscillator ��0b001��FRC�łȂ��Ԃ͑ҋ@)*/
    while (OSCCONbits.COSC != 0b001);

    /*Wait for PLL to lock*/
    while (OSCCONbits.LOCK != 1);


    /******************************
     * 
     *  ���荞�ݐݒ�
     * 
     * **************************** */
#if 1 //���荞�݂���:1 ���Ȃ�:0
    /* ���荞�݊�{�ݒ� */
    SRbits.IPL = 0; // CPU interrupt priority level:zero
    CORCONbits.IPL3 = 0; //CPU interrupt priority level:<7
    INTCON1bits.NSTDIS = 0; //���荞�݂̃l�X�e�B���O�L���i1�F�����@0�F�L���j
    INTCON2bits.ALTIVT = 0; //use primary vector table

    /* ���荞�ݗL���E���� */
    U1RXIE = enable;
    U2RXIE = enable;
    T1IE = enable; /*�^�C�}1���荞�ݗL�� */

    /* ���荞�ݗD��x�@7���ŗD�� */
    U2RXIP = 0x7;
    U1RXIP = 0x6;
    T1IP = 0x5; //set interrupt priority level
#endif

    /******************************
     * 
     *  �s���ݒ�
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

    TRISBbits.TRISB13 = 0; //�ʐM�m�F�pLED 
    TRISBbits.TRISB14 = 0; //�ʐM�m�F�pLED 

    TRISAbits.TRISA3 = 0; //����~�M���p
    TRISAbits.TRISA4 = 0; //����~�M���p
    TRISBbits.TRISB4 = 0; //����~�M���p
    TRISBbits.TRISB5 = 0; //����~�M���p

    AD1PCFGL = 0xFFFF; /* ���ׂăf�W�^�����o�� */

    /*****************
     * 
     * UART 1
     ******************/
    U1MODE = 0x0808;
    U1STA = 0x2000;
    U1RXIF = disable; /* ���荞�݃t���O�N���A */
    U1BRG = 165; /* �{�[���[�g�ݒ� */
    UART1EN = enable; /* UART1�L�� */


    /*****************
     * 
     * UART 2
     ******************/
    U2MODE = 0x0808;
    U2STA = 0x2000;
    U2RXIF = disable; /* ���荞�݃t���O�N���A */
    U2BRG = 165; /* �{�[���[�g�ݒ� */
    UART2EN = enable; /* UART1�L�� */


    /******************
     * 
     *  Timer1
     * 
     * *************** */
    T1ON = ON; //Timer on   
    T1CONbits.TCS = T1CONbits.TGATE = T1CONbits.TSYNC = 0; //Timer mode
    T1CONbits.TSIDL = 0; // �A�C�h�����[�h�ł�������p��
    T1CKPS = 0x00; //�v���X�P�[�� 1:1
    TMR1 = 0x0;
    PR1 = TIMER_1MS;

}