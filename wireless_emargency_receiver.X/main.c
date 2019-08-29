#include "mcc_generated_files/mcc.h"
#include "Controller_Protocol.h"
#include <stdbool.h>

#define Bluetooth_Reset_pin0 LATAbits.LATA2
#define Bluetooth_Reset_pin1 LATCbits.LATC0
#define Bluetooth_Reset_TRIS0 TRISAbits.TRISA2
#define Bluetooth_Reset_TRIS1 TRISCbits.TRISC0
#define emargency_switch LATCbits.LATC2
#define debug_LED0 LATAbits.LATA4
#define debug_LED1 LATAbits.LATA5

#define NOB (number_of_rxdata0 * 2 + 3)
#define waiting_to_receive 0
#define reception_complete 1

#define clear 0

bool EUSART_ERROR_emargency(void);
void Reception_from_emargency(void);
void Reception_from_emargency_main(void);

bool Receive_flag = waiting_to_receive;

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    Initialize_Parameters();
    
    Bluetooth_Reset_TRIS0 = 1;
    Bluetooth_Reset_TRIS1 = 1;

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    while (1)
    {
        // Add your application code
        Reception_from_emargency_main();
        
        emargency_switch = RxData0[0].all_data;
        debug_LED0 = RxData0[0].all_data;
    }
}
/**
 End of File
*/

bool EUSART_ERROR_emargency(void) {
    if ((RCSTAbits.FERR == 1) || (RCSTAbits.OERR == 1)) {
        //フレーミングエラー処理
        uint8_t a;
        a = RCREG;
        //オーバーランエラー処理
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
        return error;
    }
    return not_error;
}


void Reception_from_emargency(void) {
    static bool Receive_data_count = 0;

    if (Store_Datas(Buffer0, RCREG, NOB, 0, EUSART_ERROR_emargency) == error) {
        //エラー時クリア//
        Receive_data_count = clear;
    } else {
        Receive_data_count++;
    }

    if (Receive_data_count == NOB) {
        //全データの受信完了//
        Receive_flag = reception_complete;
        Receive_data_count = clear;
    }
}

void Reception_from_emargency_main(void) {
    if (Receive_flag == reception_complete) {
        //全データの受信完了後処理//
        Organize_Datas(RxData0, Buffer0, number_of_rxdata0, 0);
        Receive_flag = waiting_to_receive;
    }
}


void interrupt INTERRUPT_InterruptManager (void)
{
    // interrupt handler
    if(INTCONbits.PEIE == 1 && PIE1bits.TXIE == 1 && PIR1bits.TXIF == 1)
    {
        //EUSART_Transmit_ISR();
    }
    else if(INTCONbits.PEIE == 1 && PIE1bits.RCIE == 1 && PIR1bits.RCIF == 1)
    {
        //EUSART_Receive_ISR();
        Reception_from_emargency();
        debug_LED1 ^= 1;
        
    }
    else
    {
        //Unhandled Interrupt
    }
}