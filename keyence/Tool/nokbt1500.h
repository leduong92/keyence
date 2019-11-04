#ifndef BT1500_NOKLIB_H
#define BT1500_NOKLIB_H
//=============================================================================
//
//		�L�[�G���XBT-1500�o�[�R�[�h�p���C�u�����E�w�b�_�[�t�@�C��
//		The libraly header file for keyence BT-1500
//		File name : nokbt1500.c
//		Production Engineering Dept
//		Developper: matsuda
//		Create date �F 2010.08.03
//=============================================================================
// 2011.04.12 coba DataInputBt()�ŁAL�L�[�Ŕ�������悤�ɕύX
//				    DINPUT_SETTING�\���̂�l6�����o��ǉ�
// 2013.02.06 coba NoSel()�ŁATRG���L���ɂ���
// 2013.05.21 coba AutoPowerOff()��-1(����)���w��ł���悤�ɕύX
// 2013.06.12 sanno DataInputBt()�̓ǂ݂Ƃ蕶������2048���ɕύX
// 2013.09.18 sanno	DataInputBt3�֐���ǉ�(3�s�ڂ܂ŕ����T�C�Y��傫���ł���)
// 2013.09.24 coba ����LAN����M�֐��̌�����(�ő�20480bytes����MOK)
// 2013.12.13 coba DataInputBt()�ɕ����F�w��@�\��ǉ�
//=============================================================================
#include "btlib.h"
#include "btext.h"


//�F��`
//Declarer color
#define YELLOW        (0xFFFF00)
#define CYAN          (0x00FFFF)
#define GREEN         (0x00FF00)
#define MAGENTA       (0xFF00FF)
#define RED           (0xFF0000)
#define BLUE          (0x0000FF)
#define BLACK         (0x000000)
#define WHITE         (0xFFFFFF)

//Declear scan code setting
#define SCAN_ALL 0
#define SCAN_CODE39 1
#define SCAN_CODE128 2
#define SCAN_CODEQR 3
#define SCAN_CODE39_128 4
#define SCAN_CODEDM 5


// �ėp���b�Z�[�W�\��(�e�����o��TRUE�ɂ���ƁADspMsg()�ł̗L�����ɂȂ�)
// All purpose key data structure
typedef struct{
	BOOL anykey;		// All key
	BOOL key0;
	BOOL key1;
	BOOL key2;
	BOOL key3;
	BOOL key4;
	BOOL key5;
	BOOL key6;
	BOOL key7;
	BOOL key8;
	BOOL key9;
	BOOL keyEnt;
	BOOL keyEsc;
	BOOL keyFnUp;
	BOOL keyClr;
	BOOL keyL;		// **keyEnt�Ɠ���
	BOOL keyR;		// **keyClr�Ɠ���
	BOOL keyUp;
	BOOL keyDown;
} ENABLE_WAITKEYS;

// ���̃��C�u�����̃o�[�W����
// The version of this libraly
#define LIB_VERSION_STR "1.00"

// LCD�\���p�f�[�^�\����
// Data structure for Lcd*** functions.
#define LCD_ROW_MAX (10+1)
typedef struct{
	char data[LCD_ROW_MAX][20+2];
} BT900_LCD;

// This structure for Menu function
typedef struct{
	char name[20+1];
}MENU_NAME;
typedef struct{
	MENU_NAME item[BT_MENU_SELMAX];
}MENU_ITEMS;


//�W���֐��̑���
//Instead of standard function.
short Char2Dec( char *pString , DECIMAL* pDecimal );
double atof_bt( const char* data);
long Char2Num(char* data);
long atoi_bt( const char* data); 

// ���o�͊֌W�̲Ƽ�ײ��
// Initialize all devices.
short AllInit(void);
short AllInitial(BOOL resume);
// �N������̏������
// Initial display after startup
int StartDisplay(char *title,char *l1,char *l2,char *l3,char *l4,char *g1,char *g2,char *g3);

//Set scan code for scanner.
void SetScanCode(int code);

//�I�[�g�p���[�I�t�̎��Ԑݒ�
//Set auto power off time.
void SetAutoPowerOff(long min );

// ��ʕ\���n
// Concern display
void LcdOut( BT900_LCD* lcd);
void LcdOutLeg( char* msg1,char *msg2,char *msg3);
void LineOut( char* dat, short lineNo,short reverse);// normal size char. �s�w��̉�ʕ\��
void ClineOut( char* dat, short lineNo,char color);// normal size char. �s�w��̉�ʕ\��
void LineOut2( char* dat, short lineNo,short rev_mode);//2 times size char of vertical. �c�̂�2�{�p
void LineOut4( char* dat, short lineNo,short reverse);// 4 times size char.���b�Z�[�W�̏o�́i�S�{�p�j
void LcdClear(void);// ��ʃN���A Clear display

// �r�[�v�����Ȃ炷
// Ring the beep
void Beep(int mode);
void Buzzer(void);
void BuzzerV(void);			//2012.02.12 matuda �J�V�I�݊��֐��ǉ� legacy function.
void BuzzerComp(void);

//Concern datatime
// ���݂̎����̎擾
// Get current time
short GetNowtime( char *data );		//legacy function. don't use to new program.
void get_nowtime( char *data ,short mode);
int Calctime( char *src,char *dest,short days);
int InputDate( const char* title,const char *msg1,const char* msg2,const char *msg3,char *date );
int InputDateEng( const char* title,const char *l1,const char* l2,const char *l3,char *dest_day);

// get_nowtime�̑�S�����̒萔
#define MD_MIN 0
#define MD_HOUR 1
#define MD_SEC 2
#define MD_DAY 3

#ifndef A_MONTH
#define A_MONTH 30
#endif
#ifndef A_YEAR
#define A_YEAR  12
#endif
#ifndef HDATEL
#define HDATEL	14
#endif
#ifndef LDATEL
#define LDATEL	12
#endif
#ifndef MDATEL
#define MDATEL	10
#endif
#ifndef SDATEL
#define SDATEL	8
#endif
#ifndef HOST_L
#define HOST_L 13
#endif
// getnowtime�̑�2�����̒萔
#define TIMEST	0			// ���l�\���̂ŕԂ��ꍇ
// ���Ԏ擾�p�̍\����
typedef struct{
	short year;
	short month;
	short day;
	short hour;
	short min;
	short sec;
} TMSTRCT;
long Ltime( void );// �ߑO0������̌o�ߕb���̎擾
//Set current time by argument.
int set_nowtime( char *setdate);	// 2008.02.29


void Wait(long time);// �E�G�C�g
// 16�i�i������j--> 10�i(short)�ϊ�
// hex to decimal arithmetic.
short	htoi( char *hexstr , short len );

//Trim function
void lTrim( char* buf,int len);
void Trim( char* buf,int len);

int DspMsgBt( char *title,int guideno,char* l1,char* l2,char* l3,char* l4,char* l5,
				char* l6,char *l7,ENABLE_WAITKEYS* enkeys);
// 2012.12.06 coba �K�C�h���w��ł���悤�ɒǉ� add guide message.
int DspMsgBt2( char *title,int guideno,char* l1,char* l2,char* l3,char* l4,char* l5,
				char* l6,char *l7,char* l9,ENABLE_WAITKEYS* enkeys);
// 2013.02.14 coba 1�s���₷
int DspMsgBt3( char *title,int guideno,char* l1,char* l2,char* l3,char* l4,char* l5,
				char* l6,char *l7,char* l8,char* l9,ENABLE_WAITKEYS* enkeys);

//DT-950�p���ʌ݊��֐�
int DspMsg( char *title,int guideno	,char* l1,char* l2,char* l3,char* l4,char* l5,ENABLE_WAITKEYS* enkeys);
int DspMsg2( char *title,int guideno	,char* l1,char* l2,char* l3,char* l4,char* l5,char *l6,ENABLE_WAITKEYS* enkeys);

// Scroll screen
void SetScrollMsg( char* dat, short lineNo, short rev_mode );
int ShowScrollScreen(const char *title,int rows);

//Menu Screen Function ===============================================
int MenuScreen(const char *title,MENU_ITEMS itemname,short itemqty);


// �f�[�^����
// Structure for DataInput*** function.
typedef struct{
	// �ݒ�
	int mode;			// 0=edit,1=bar,2=edit+bar
	char title[20+1];	// titale.�E�C���h�E�^�C�g���ւ̃|�C���^(LPCTSTR=���C�h������ւ̃|�C���^)
	char l1[20+1];		// charactor for line 1. 1�s�ڂɕ\�����镶����
	char l2[20+1];		// charactor for line 2. 2�s�ڂɕ\�����镶����
	char l3[20+1];		// charactor for line 3. 3�s�ڂɕ\�����镶����
	char l4[20+1];		// charactor for line 4. 4�s�ڂɕ\�����镶����
	char l5[20+1];		// charactor for line 5. 5�s�ڂɕ\�����镶����
	char l6[20+1];		// charactor for line 6. 5�s�ڂɕ\�����镶���� 2011.04.12 coba�ǉ�
	char l7[20+1];		// charactor for line 7. 5�s�ڂɕ\�����镶����
	char* buffer;		// The data pointer for storing the data( key ,barcode )�f�[�^���i�[����|�C���^
	unsigned int bufsize;// The data size of storing data buffer.���̃o�b�t�@�T�C�Y
	BOOL emptyOK;		// If TRUE,allow to imput empty.  TRUE=�󕶎���̓��͂�������
	BOOL upkey;			// If TRUE,Enable up key.���L�[�����L��
	BOOL downkey;		// If TRUE,Enable down key.���L�[�����L��
	// �o��
	BOOL bar_read;		// When scan barcode,this value become TRUE,�o�[�R�[�h��ǂ񂾂Ƃ�=TRUE�ɂȂ�
	int keychange;		// 0:only number,1:can change input mode. 0:�����̂�(�f�t�H���g) 1:���͐ؑւ��� 2006.11.25 matsuda
	char defdata[20+1];	// The default value of key input.�L�[���͎��̏����l 2008.02.29
	char alpha;			// 0:can not input alphabet,1: can input alphabet.�A���t�@�x�b�g���͂���
	// �F�w��,0=���A���Ƃ͉��̃}�N�����Q�� font color,0=black else refer CLR_??? macros
	char c1;
	char c2;
	char c3;
	char c4;
	char c5;
	char c6;
	char c7;
} DINPUT_SETTING;
// �����F�w��}�N��
#define CLR_BLACK 0
#define CLR_RED 1
#define CLR_BLUE 2
#define CLR_YELLOW 3
#define CLR_CYAN   4
#define CLR_GREEN  5
#define CLR_MAGENTA 6
#define CLR_WHITE   7


int DataInputBt( DINPUT_SETTING* set );
int DataInputBt2( DINPUT_SETTING* set ); //2012.04.24 okamura
int DataInputBt3( DINPUT_SETTING* set, short col_count ); //2013.09.18 sanno

short NoSel(short no,int cancel);// Number selection
short NoSelEnableKey(ENABLE_WAITKEYS *key);// Number selection by arument key
short InputAlphabet( char *databuf,char *msg );	//Input just alphabet.�A���t�@�x�b�g���݂͂̂̊֐�

//�f�[�^���͌n
//Legacy input data functions.Don't use to new program.-----------------------------------------
short DataInput( short mode, char *databuf, char *name, char *msg, short len);
int KeyInput( char *databuf, short len );// Input data by keyboard.�L�[�{�[�h����̃f�[�^���́i�u�������@�\�t���j
short BarRead(char *databuf, short len,short mode);// Legacy function.
short FuncSel(short mode);// Function key selection.�t�@���N�V�����L�[�I��
//----------------------------------------------------------------------------------------------

//Show OK,NG by big font OK,NG�̃f�J�����\��
void OkNgDisp( BOOL okng,int timer );

//�����ʐM�֐�
//Concern wireless communication.
#define MAX_SENDLEN 4095
int WLAN_setting(void);
int WLAN_settingEx(char* keyname_ip,char* dspname_ip, char* keynaem_port,char* dspname_port);
int WLAN_settingIP(char* keyname_ip,char *dspname_ip);
int WLAN_init(void);
void WLAN_reset(void);	//2013.06.11 matsuda
int WLAN_setserver(char* srvaddr,int port);

int WLAN_setting_usr(char* keyname);
int WLAN_change_usr_pc(char* keyname);

int WLAN_send( char* data,int size,char* rcv,int rsize);
int WLAN_sendBt( char* data,int size,char* rcv,int rsize,int* rcvlen,int send_to,int recv_to);
short PingTest( void );//ping test
int UpdateProgram(void);// update program function. 2013.10.01 matsuda
//------------------------------------------------------------------------------------------------------------
// The following function is operation of master files. Do not change these functions.....
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
// �ȉ��͓����ŕێ�����}�X�^�t�@�C���֌W�B�}�X�^�t�@�C����TTextTable�I�Ȏg�������ł���t�@�C��
//------------------------------------------------------------------------------------------------------------
// 2011.07.23 matsuda ���X�h���C�u2�ɂ��Ă������A����͊����������������̂�
// 					  �s������������̃h���C�u1�ɕύX(�ȉ��V�t�@�C��)
#define MTRFILE_NAME "1:master.dat"
#define MTRIDXFILE_NAME "1:master.idx"
#define MTRSYSFILE_NAME "1:__master.idx"	//BT-1500�ŏ���ɍ쐬�����Index�t�@�C��

#define MODMTRFILE_NAME "1:master_mod.dat"
#define MODMTRIDXFILE_NAME "1:master_mod.idx"
#define MODMTRADDFILE_NAME "1:master_mod.add"
#define MODMTRSYSFILE_NAME "1:__master_mod.idx"	//BT-1500�ŏ���ɍ쐬�����Index�t�@�C��

int MasterFind( char *keyname,char *data );	//�}�X�^�t�@�C�����̌�������
int MasterUpd( char *keyname,char *data );	//�}�X�^�ւ̃f�[�^�ǉ�

#endif
