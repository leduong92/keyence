#ifndef COMMON_H
#define COMMON_H


#include "P:\pdrv\barlib\Keyence\nokbt1500.h"
#include "BCR.h"


#define CHECK_BOX			3
#define CHECK_PCL			1
#define CHECK_PALLETE		2
#define CHECK_SHIPPINGTO	4
#define CHECK_SHIPPINGNO	5

#define BOX_LENGTH			12
#define PALLETE_LENGTH		15
#define SHIPPING_TO_LENGTH  15
#define PCL_LENGTH			16
#define SIZE_OF_DATA        20480    
#define SIZE_OF_BUF_1       128
#define SIZE_OF_BUF_2       512

#define SCAN_PCL_NO         1
#define SCAN_BOX_NO         2
#define SCAN_PALLETE_NO     3
#define SCAN_SHIPPING_TO    4
#define SCAN_SHIPPING_NO    5

extern char send[SIZE_OF_DATA], rcv[SIZE_OF_DATA];
extern char buf[SIZE_OF_BUF_2];
extern int keyCode;
extern char loc[3+1], userId[10];
extern char msg[SIZE_OF_BUF_1], msg2[SIZE_OF_BUF_1];
extern int ret,cntError;

int InputPassword(void);

int CheckAdminPassword(void);

int DataSend(char* data,int size,char* rcv,int rsize,BOOL ok_disp);

//void ShowMessage(char* title,BCR_MSG* hmsg,char* line5,BOOL buzzer);

void ShowMessage(char* title,BCR_MSG* hmsg,char* line5,BOOL buzzer, int beepMode);

int SetLocation(void);

int MakeDataToSend(char* cmd, char* sub, char* data, char* sendData);

int CheckDataByType(char* loc, char* val, int type);

int GetBoxNoFromString(char* str, char* boxNo);

int CheckDataExistInString(char* listData, char* data, int flag);

int CheckDuplicate(char* listData, char* data, int flag);

int GetUserAndLocation(void);

int ReadData(char* value, char* list, char* msg1, char* msg2, int mode);

int isLeapYear(int year);

int isValidDate(char* date);

int GetCurrentDate(char *date, int type);

void SetBuzzerV(int mode);

#endif
