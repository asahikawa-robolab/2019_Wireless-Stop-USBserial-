#ifndef Controller_Protocol_H
#define	Controller_Protocol_H

#include "Controller_Protocol.h"
#include <stdbool.h>

typedef struct{
    int num;
    int count;
}Organize_Parameter;

typedef struct{
    int Buffer_count;
    bool Start_signal_has_come;
    bool S_flag;
}Store_Parameter;

typedef struct{
    int data_send_count;
    int last_signal;
    bool byte_data_complete;
}Send_Parameter;

typedef union{
    struct{
        unsigned lower_data :4;
        unsigned upper_data :4;
    };
    unsigned char all_data;
}Untreated;

typedef union{
    
    struct{
        unsigned d0:1;
        unsigned d1:1;
        unsigned d2:1;
        unsigned d3:1;
        unsigned d4:1;
        unsigned d5:1;
        unsigned d6:1;
        unsigned d7:1;
    };
    
    struct{
        unsigned lower_data :4;
        unsigned upper_data :4;
        unsigned LD_error :1;
        unsigned UD_error :1;
    };
    struct{
        unsigned char all_data;
        unsigned errors :2;
    };
}Processed;

typedef union{
    
    struct{
        unsigned d0:1;
        unsigned d1:1;
        unsigned d2:1;
        unsigned d3:1;
        unsigned d4:1;
        unsigned d5:1;
        unsigned d6:1;
        unsigned d7:1;
    };
    
    unsigned char all_data;
}TxData_divided;

/*受信用記憶領域確保*/
#define number_of_rxdata0 1
//#define number_of_rxdata1 2
//#define number_of_rxdata2 8

/*送信用記憶領域確保*/
#define number_of_txdata0 1
//#define number_of_txdata1 8

//#define number_of_txdata2 8

/*通信バス数指定*/
#define rx_number 3
#define tx_number 3

#define error 1
#define not_error 0 

#ifdef number_of_rxdata0
Untreated Buffer0[number_of_rxdata0 * 2 + 3];
Processed RxData0[number_of_rxdata0];
#endif
#ifdef number_of_rxdata1
Untreated Buffer1[number_of_rxdata1 * 2 + 3];
Processed RxData1[number_of_rxdata1];
#endif
#ifdef number_of_rxdata2
Untreated Buffer2[number_of_rxdata2 * 2 + 3];
Processed RxData2[number_of_rxdata2];
#endif

#ifdef number_of_txdata0
unsigned char TxData0[number_of_txdata0];
#endif
#ifdef number_of_txdata1
unsigned char TxData1[number_of_txdata1];

#endif
#ifdef number_of_txdata2
unsigned char TxData2[number_of_txdata2];
#endif

extern void Initialize_Parameters(void);
extern void Organize_Datas(Processed *,Untreated *,int,int);
extern bool Store_Datas(Untreated *,unsigned char,int,int, bool (*)(void));
extern void Send_StartSignal(void (*)(unsigned char),void (*)(bool),bool);
extern bool Send_Till_EndSignal(unsigned char *,void (*)(unsigned char),void (*)(bool),int,int);

#ifdef	__cplusplus
#endif
#ifdef	__cplusplus
#endif
#endif