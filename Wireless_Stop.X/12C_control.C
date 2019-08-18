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
#define I2C_finished_operation 0 //I2C�̓��슮��
#define I2C_in_operation 1 //I2C���쒆
//////////

//�O���[�o���ϐ��錾//
unsigned char I2C_status = I2C_finished_operation;
/*
 * ���슮���i����start_condition�𑗂��j  ���@I2C_finished_operation
 * ���쒆�i���M�������͎�M���j             ���@I2C_in_operation
 */
////////////////////

//�v���g�^�C�v�錾//
void I2C_setup(void);
void I2C_Six_axis_sensor_setup(unsigned char [][2]);
void I2C_send_start(bool);
void I2C_send_stop(void);
bool I2C_send(unsigned char [], unsigned char);
bool I2C_receive(unsigned char [], unsigned char);
bool I2C_interrupt_process(unsigned char [][2], unsigned char []);
////////////////////

//////////I2C�p�֐�//////////

//void I2C_setup(void) {
//    SSPCON1 = 0x00;
//
//    //�����ݒ�//
//    SSPSTAT = 0x00;
//    /*
//     * SMP : �������[�h(400kHz)
//     */
//
//    SSPCON1 = 0x28;
//    /* 
//     * SSPEN : SSP���W���[���g�p����
//     * SSPM : I^2C �}�X�^�[���[�h
//     */
//
//    SSPCON2 = 0x00;
//    SSPADD = 9;
//    I2C_interrupt_enable_flag = 0; //���荞�ݕs��
//}
//
//void I2C_Six_axis_sensor_setup(unsigned char data[][2]) {
//    unsigned char i = 0, j = 0;
//    for (i = 0; i < 4; i++) {
//        I2C_send_start(I2C_write);
//        I2C_interrupt_enable_flag = 0; //���荞�݋֎~
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
//        I2C_status = I2C_finished_operation; //start_condition���ēx�����
//    }
//}
//
//void I2C_send_start(bool WR) {
//    if (I2C_status == I2C_finished_operation) {
//        I2C_start_condition = 1; //start_condition�̑��M
//        while (I2C_start_condition == 1); //start_condition�̑��M�I���܂őҋ@
//
//        I2C_status = I2C_in_operation; //���M�܂��͎�M�I���܂ł���if���ɂ͓���Ȃ�
//
//        I2C_interrupt_flag = 0; //���荞�݃t���O�N���A
//        I2C_interrupt_enable_flag = 1; //���荞�݋���
//
//        I2C_BUF = (Six_axis_address | WR);
//
//    }
//}
//
//void I2C_send_stop(void) {
//    I2C_interrupt_enable_flag = 0; //���荞�݋֎~
//    I2C_interrupt_flag = 0; //���荞�݃t���O�N���A
//    
//    I2C_stop_condition = 1; //stop_condition�̑��M
//    while (I2C_stop_condition == 1); //stop_condition�̑��M�I���܂őҋ@
//
//}
//
//bool I2C_send(unsigned char data[], unsigned char length) {
//    static unsigned char data_send_count = 0;
//
//    if ((I2C_ack_receive == 1)/* && (data_send_count != 0)*/); //�G���[�����i���������Ă��Ȃ��j
//
//    I2C_BUF = data[data_send_count]; //�o�b�t�@�Ƀf�[�^����
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
//    data[data_read_count] = I2C_BUF; //��M�f�[�^��z��Ɋi�[
//    data_read_count++;
//
//    if (data_read_count == length) {
//        I2C_ack_data = 1; //nack���Z�b�g
//        I2C_send_ack = 1; //ack�𑗐M
//        while (I2C_send_ack == 1); //ack�̑��M�����܂őҋ@
//        data_read_count = 0;
//        return I2C_finished_operation;
//    }
//
//    I2C_ack_data = 0; //ack���Z�b�g
//    I2C_send_ack = 1; //ack�𑗐M
//    while (I2C_send_ack == 1); //ack�̑��M�����܂őҋ@
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
//    I2C_interrupt_flag = 0; //���荞�݃t���O�N���A
//
//    switch (process_count) {
//        case 0:
//            debug_LED0 = 1;
//            process_count++;
//            I2C_send(data[4], 1); //�Z���T�̃��W�X�^�A�h���X�𑗐M
//            break;
//        case 1:
//            process_count++;
//            //I2C_send_stop(); //stop
//            I2C_status = I2C_finished_operation;
//            I2C_send_start(I2C_read); //start(��M���[�h)
//            break;
//        case 2:
//            process_count++;
//            I2C_receive_enable = 1; //��M������
//            break;
//        case 3:
//            if (I2C_receive(I2C_read_data, 10) == I2C_finished_operation) {//14�̃f�[�^�擾��܂ł�����ʂ�
//                process_count++;
//            } else break;
//        case 4:
//            debug_LED0 = 0;
//            I2C_receive_enable = 0; //��M���֎~
//            I2C_send_stop(); //stop
//            I2C_status = I2C_finished_operation; //start_condition���ēx�����
//            I2C_interrupt_enable_flag = 0; //���荞�݋֎~
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