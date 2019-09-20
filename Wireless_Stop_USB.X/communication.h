/* 
 * File:   communication.h
 * Author: nemoto
 *
 * Created on 2018/05/27, 9:33
 */

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

////�t���Obit////
#define NORD number_of_rxdata0                  ////data��(RxDatax�̔z��)////
#define NOB NORD * 2 + 3                        ////data��M��(Bufferx�̔z��)////
#define enablebit_from_master IEC0bits.U1RXIE   ////��M���r�b�g////
#define flagbit_from_master IFS0bits.U1RXIF     ////��M�t���O�r�b�g////
#define REG_from_master U1RXREG                 ////��M���W�X�^////
#define Buffer_from_master Buffer0              ////Buffer////
#define Data_from_master RxData0                ////RxData////
#define FERR_from_master U1STAbits.FERR         ////�t���[�~���O�G���[�t���O////
#define OERR_from_master U1STAbits.OERR         ////�I�[�o�[�����G���[�t���O////
//#define CREN_from_master RC2STAbits.CREN        ////�A���I�Ȏ�M���r�b�g////

////�t���Obit////
#define NORDE number_of_rxdata1                  ////data��(RxDatax�̔z��)////
#define NOBE NORDE * 2 + 3                        ////data��M��(Bufferx�̔z��)////
#define enablebit_from_emargency IEC0bits.U2RXIE   ////��M���r�b�g////
#define flagbit_from_emargency IFS0bits.U2RXIF     ////��M�t���O�r�b�g////
#define REG_from_emargency U2RXREG                 ////��M���W�X�^////
#define Buffer_from_emargency Buffer1              ////Buffer////
#define Data_from_emargency RxData1                ////RxData////
#define FERR_from_emargency U2STAbits.FERR         ////�t���[�~���O�G���[�t���O////
#define OERR_from_emargency U2STAbits.OERR         ////�I�[�o�[�����G���[�t���O////
//#define CREN_from_emargency RC2STAbits.CREN        ////�A���I�Ȏ�M���r�b�g////

#define T2_TRIS TRISDbits.TRISD1

////�o�b�t�@////
#define ADD_upper Buffer0[1].upper_data
#define ADD_lower Buffer0[2].upper_data

////�X���[�u�A�h���X////
/*
�X���[�u�A�h���X�͂��ꂼ��ς��Ă�������
���[�^�͉��ʃr�b�g���A�d���قƃT�[�{�͏�ʃr�b�g��ݒ肵�Ă�������  
 */
#define Motor_slave_address (PORTC & 0x0f)
#define Encoder_slave_Address 0xe0

////��M�p�X�e�[�^�X////
#define reception_complete 0 //��M����(��M�ҋ@��)
#define waiting_to_receive 1 //��M��
////��M�p�g������////
#define master_COM 0
#define emargency_COM 1

////slave_check�֐��p�X�e�[�^�X
#define continuation 0
#define slave_match 1

#define not_transmit 0
#define match_and_transmit 1


////���M�p�X�e�[�^�X////
#define send_start 0
#define send_till 1
#define send_stop 2


extern bool Receive_flag;

extern bool Reception_from_master(void); //slave��master
extern void Reception_from_master_main(void);
extern void transmit_for_master(bool);

extern bool Reception_from_emargency(void); //slave��master
extern void Reception_from_emargency_main(void);

extern void Flag_reset(void);

#endif	/* COMMUNICATION_H */