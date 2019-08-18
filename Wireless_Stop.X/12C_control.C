/* 
 * File:   I2C_control.c
 * Author: nemoto
 *
 * Created on February 27, 2019, 2:40 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "12C_control.h"

//define//
#define I2C_finished_operation 0 //I2Cの動作完了
#define I2C_in_operation 1 //I2C動作中
//////////

//グローバル変数宣言//
unsigned char I2C_status = I2C_finished_operation;
/*
 * 動作完了（次のstart_conditionを送れる）  →　I2C_finished_operation
 * 動作中（送信もしくは受信中）             →　I2C_in_operation
 */
////////////////////

//プロトタイプ宣言//
void I2C_setup(void);
void I2C_Six_axis_sensor_setup(unsigned char [][2]);
void I2C_send_start(bool);
void I2C_send_stop(void);
bool I2C_send(unsigned char [], unsigned char);
bool I2C_receive(unsigned char [], unsigned char);
bool I2C_interrupt_process(unsigned char [][2], unsigned char []);
////////////////////

//////////I2C用関数//////////

//void I2C_setup(void) {
//    SSPCON1 = 0x00;
//
//    //初期設定//
//    SSPSTAT = 0x00;
//    /*
//     * SMP : 高速モード(400kHz)
//     */
//
//    SSPCON1 = 0x28;
//    /* 
//     * SSPEN : SSPモジュール使用許可
//     * SSPM : I^2C マスターモード
//     */
//
//    SSPCON2 = 0x00;
//    SSPADD = 9;
//    I2C_interrupt_enable_flag = 0; //割り込み不可
//}
//
//void I2C_Six_axis_sensor_setup(unsigned char data[][2]) {
//    unsigned char i = 0, j = 0;
//    for (i = 0; i < 4; i++) {
//        I2C_send_start(I2C_write);
//        I2C_interrupt_enable_flag = 0; //割り込み禁止
//        while (I2C_interrupt_flag == 0);
//        I2C_interrupt_flag = 0;
//
//        for (j = 0; j < 2; j++) {
//            I2C_send(data[i], 2);
//            while (I2C_interrupt_flag == 0);
//            I2C_interrupt_flag = 0;
//        }
//
//        I2C_send_stop();
//        I2C_status = I2C_finished_operation; //start_conditionを再度送れる
//    }
//}
//
//void I2C_send_start(bool WR) {
//    if (I2C_status == I2C_finished_operation) {
//        I2C_start_condition = 1; //start_conditionの送信
//        while (I2C_start_condition == 1); //start_conditionの送信終了まで待機
//
//        I2C_status = I2C_in_operation; //送信または受信終了までこのif文には入らない
//
//        I2C_interrupt_flag = 0; //割り込みフラグクリア
//        I2C_interrupt_enable_flag = 1; //割り込み許可
//
//        I2C_BUF = (Six_axis_address | WR);
//
//    }
//}
//
//void I2C_send_stop(void) {
//    I2C_interrupt_enable_flag = 0; //割り込み禁止
//    I2C_interrupt_flag = 0; //割り込みフラグクリア
//    
//    I2C_stop_condition = 1; //stop_conditionの送信
//    while (I2C_stop_condition == 1); //stop_conditionの送信終了まで待機
//
//}
//
//bool I2C_send(unsigned char data[], unsigned char length) {
//    static unsigned char data_send_count = 0;
//
//    if ((I2C_ack_receive == 1)/* && (data_send_count != 0)*/); //エラー処理（何も書いていない）
//
//    I2C_BUF = data[data_send_count]; //バッファにデータを代入
//    data_send_count++;
//
//    if (data_send_count == length) {
//        data_send_count = 0;
//        return I2C_finished_operation;
//    }
//    return I2C_in_operation;
//
//}
//
//bool I2C_receive(unsigned char data[], unsigned char length) {
//    static unsigned char data_read_count = 0;
//
//    data[data_read_count] = I2C_BUF; //受信データを配列に格納
//    data_read_count++;
//
//    if (data_read_count == length) {
//        I2C_ack_data = 1; //nackをセット
//        I2C_send_ack = 1; //ackを送信
//        while (I2C_send_ack == 1); //ackの送信完了まで待機
//        data_read_count = 0;
//        return I2C_finished_operation;
//    }
//
//    I2C_ack_data = 0; //ackをセット
//    I2C_send_ack = 1; //ackを送信
//    while (I2C_send_ack == 1); //ackの送信完了まで待機
//
//    return I2C_in_operation;
//
//}
//
//bool I2C_interrupt_process(unsigned char data[][2], unsigned char I2C_read_data[]) {
//    static unsigned char process_count = 0;
//    static unsigned char data_count = 0;
//    static unsigned char save_process_count = 0;
//    static unsigned char i;
//
//    I2C_interrupt_flag = 0; //割り込みフラグクリア
//
//    switch (process_count) {
//        case 0:
//            debug_LED0 = 1;
//            process_count++;
//            I2C_send(data[4], 1); //センサのレジスタアドレスを送信
//            break;
//        case 1:
//            process_count++;
//            //I2C_send_stop(); //stop
//            I2C_status = I2C_finished_operation;
//            I2C_send_start(I2C_read); //start(受信モード)
//            break;
//        case 2:
//            process_count++;
//            I2C_receive_enable = 1; //受信を許可
//            break;
//        case 3:
//            if (I2C_receive(I2C_read_data, 10) == I2C_finished_operation) {//14個のデータ取得後までここを通る
//                process_count++;
//            } else break;
//        case 4:
//            debug_LED0 = 0;
//            I2C_receive_enable = 0; //受信を禁止
//            I2C_send_stop(); //stop
//            I2C_status = I2C_finished_operation; //start_conditionを再度送れる
//            I2C_interrupt_enable_flag = 0; //割り込み禁止
//            process_count = 0;
//            break;
//        default:
//            process_count = 0;
//            break;
//    }
//
//    return 0;
//
//}
//
///////////////////////////////
//
