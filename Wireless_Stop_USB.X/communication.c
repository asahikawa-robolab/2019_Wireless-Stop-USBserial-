/* 
 * File:   communication.c
 * Author: nemoto
 *
 * Created on 2018/05/27, 9:32
 */
#include "variable.h"
#include "communication.h"
#include "Controller_Protocol.h"
#include <xc.h>

/*
 * 
 */

bool Receive_flag = waiting_to_receive;

bool slave_different_flag = continuation;
bool slave_transmit_flag = not_transmit;

bool EUSART_ERROR_from_master(void);
bool Reception_from_master(void);
bool slave_address_check(uint8_t, uint8_t, uint8_t);
void Reception_from_master_main(void);

bool EUSART_ERROR_from_emargency(void);
bool Reception_from_emargency(void);
void Reception_from_emargency_main(void);

void transmit_for_master(bool);
void EUSART_Write_for_master(unsigned char);
void EUSART_TxInterrupt_Control_for_master(bool);

void Flag_reset(void);

/*debug*/
int debug_buf;

//受信の割り込みを許可

bool EUSART_ERROR_from_master(void) {
    if ((FERR_from_master == 1) || (OERR_from_master == 1)) {
        //フレーミングエラー処理
        uint8_t waste_data;
        waste_data = REG_from_master;
        //オーバーランエラー処理
        U1MODE = 0x0808;
        U1STA = 0x2000;
        U1MODEbits.UARTEN = 1;
        U1STAbits.UTXEN = 1;
        return error;
    }

    return not_error;
}

bool Reception_from_master(void) {
    extern char data;
    static uint8_t Receive_data_count = 0;
    uint8_t error_complete = 0;
    /*debug*/
    unsigned char Rx_buf;

    Rx_buf = REG_from_master;
    if (Store_Datas(Buffer_from_master, Rx_buf, NOB, master_COM, EUSART_ERROR_from_master) == error) {
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
    return Receive_flag;

}

void Reception_from_master_main(void) {
    if (Receive_flag == reception_complete) {
        Receive_flag = waiting_to_receive;
        Organize_Datas(Data_from_master, Buffer_from_master, number_of_rxdata0, master_COM);
    }
}
////////////////////////////////////////////////////////////////////////////////

bool EUSART_ERROR_from_emargency(void) {
    if ((FERR_from_emargency == 1) || (OERR_from_emargency == 1)) {
        //フレーミングエラー処理
        uint8_t waste_data;
        waste_data = REG_from_emargency;
        //オーバーランエラー処理
        U2MODE = 0x0808;
        U2STA = 0x2000;
        U2MODEbits.UARTEN = 1;
        U2STAbits.UTXEN = 1;
        return error;
    }

    return not_error;
}

bool Reception_from_emargency(void) {
    extern char data;
    static uint8_t Receive_data_count = 0;
    uint8_t error_complete = 0;
    /*debug*/
    unsigned char Rx_buf;


    Rx_buf = REG_from_emargency;

    if (Store_Datas(Buffer_from_emargency, Rx_buf, NOBE, emargency_COM, EUSART_ERROR_from_emargency) == error) {
        //エラー時クリア//
        Receive_data_count = clear;
    } else {
        Receive_data_count++;
    }

    if (Receive_data_count == NOBE) {
        //全データの受信完了//
        Receive_flag = reception_complete;
        Receive_data_count = clear;
    }
    return Receive_flag;
}

void Reception_from_emargency_main(void) {
    if (Receive_flag == reception_complete) {
        Receive_flag = waiting_to_receive;
        Organize_Datas(Data_from_emargency, Buffer_from_emargency, number_of_rxdata1, emargency_COM);
    }
}

void transmit_for_master(bool start_endtill) {
    //    static uint8_t TxData_number = 0;
    bool send_till_flag = 0;
    static bool send_complete_flag = enable;

    if (slave_transmit_flag == match_and_transmit) {
        if ((start_endtill == send_start) && (send_complete_flag == enable)) {
            send_complete_flag = disable;
            Send_StartSignal(EUSART_Write_for_master, EUSART_TxInterrupt_Control_for_master, T_enable);
        } else if (start_endtill == send_till) {
            send_till_flag = Send_Till_EndSignal(TxData0, EUSART_Write_for_master, EUSART_TxInterrupt_Control_for_master, number_of_txdata0, 0);
            if (send_till_flag == true) {
                slave_transmit_flag = not_transmit;
                send_complete_flag = enable;
            }
        }
    }
}

void EUSART_Write_for_master(unsigned char txData) {
    //配列 txDataの先頭アドレスを渡す
    U1TXREG = txData;
}

//送信の割り込み許可

void EUSART_TxInterrupt_Control_for_master(bool enable_or_disable) {
    //Transmit_enable = enable_or_disable;
    T_enable = enable_or_disable;
}

void Flag_reset(void) {
    slave_different_flag = continuation;
}
