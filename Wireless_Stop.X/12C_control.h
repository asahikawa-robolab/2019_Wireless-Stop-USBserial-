/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

/* 
 * File:   I2C_control.h
 * Author: nemoto
 *
 * Created on February 27, 2019, 2:41 PM
 */

#ifndef I2C_CONTROL_H
#define	I2C_CONTROL_H

#include <xc.h>

//////////�\���̐錾//////////
typedef union {
    unsigned char Array[14];
    
    struct {
        int accel_x;
        int accel_y;
        int accel_z;
        int temp;
        int gyro_x;
        int gyro_y;
        int gyro_z;
    };
    
}Six_axis_sensor_data;
/////////////////////////////

//I^2C�p//
#define I2C_start_condition SSPCON2bits.SEN //�X�^�[�g�R���f�B�V�����̑��M
#define I2C_restart_condition SSPCON2bits.RSEN //���X�^�[�g�R���f�B�V�����̑��M
#define I2C_stop_condition SSPCON2bits.PEN //�X�g�b�v�R���f�B�V�����̑��M
#define I2C_receive_enable SSPCON2bits.RCEN //��M����
#define I2C_send_ack SSPCON2bits.ACKEN //ACK�̑��M
#define I2C_ack_data SSPCON2bits.ACKDT //ACK�̃f�[�^�̕ύX (1:nack, 0:ack)
#define I2C_ack_receive SSPCON2bits.ACKSTAT //�X���[�u�����ACK���o (1:����M, 0:��M��)

#define I2C_BF_status SSPSTATbits.BF
/*
 * ��M�� (1:��M����, 0:��M��)
 * ���M�� (1:���M��, 0:���M����)
 * ��(0:�o�b�t�@��, 1:�o�b�t�@���f�[�^����)
 */

#define I2C_BUF SSPBUF

#define I2C_interrupt_enable_flag PIE1bits.SSPIE //���荞�݋��r�b�g
#define I2C_interrupt_flag PIR1bits.SSPIF //���荞�݃t���O

//WR�p//
#define I2C_write 0
#define I2C_read 1
////////

//I2C�p�֐�//
extern void I2C_setup(void);
void I2C_Six_axis_sensor_setup(unsigned char [][2]);
extern void I2C_send_start(bool);
extern bool I2C_interrupt_process(unsigned char [][2], unsigned char []);

//extern Six_axis_sensor_data sensor_data;

//#define I2C_read_data sensor_data.Array

//address//
#define Six_axis_address 0xD0 //0x68 //���ł�1bit���ɃV�t�g���Ă���

//debug�p//
#define debug_LED0 LATAbits.LATA0

#define debug_switch0 PORTAbits.RA1
#define debug_switch1 PORTAbits.RA2

#endif