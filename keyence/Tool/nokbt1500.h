#ifndef BT1500_NOKLIB_H
#define BT1500_NOKLIB_H
//=============================================================================
//
//		キーエンスBT-1500バーコード用ライブラリ・ヘッダーファイル
//		The libraly header file for keyence BT-1500
//		File name : nokbt1500.c
//		Production Engineering Dept
//		Developper: matsuda
//		Create date ： 2010.08.03
//=============================================================================
// 2011.04.12 coba DataInputBt()で、Lキーで抜けられるように変更
//				    DINPUT_SETTING構造体にl6メンバを追加
// 2013.02.06 coba NoSel()で、TRGも有効にした
// 2013.05.21 coba AutoPowerOff()で-1(無効)を指定できるように変更
// 2013.06.12 sanno DataInputBt()の読みとり文字数を2048字に変更
// 2013.09.18 sanno	DataInputBt3関数を追加(3行目まで文字サイズを大きくできる)
// 2013.09.24 coba 無線LAN送受信関数の見直し(最大20480bytes送受信OK)
// 2013.12.13 coba DataInputBt()に文字色指定機能を追加
//=============================================================================
#include "btlib.h"
#include "btext.h"


//色定義
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


// 汎用メッセージ表示(各メンバをTRUEにすると、DspMsg()での有効ｷｰになる)
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
	BOOL keyL;		// **keyEntと同じ
	BOOL keyR;		// **keyClrと同じ
	BOOL keyUp;
	BOOL keyDown;
} ENABLE_WAITKEYS;

// このライブラリのバージョン
// The version of this libraly
#define LIB_VERSION_STR "1.00"

// LCD表示用データ構造体
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


//標準関数の代わり
//Instead of standard function.
short Char2Dec( char *pString , DECIMAL* pDecimal );
double atof_bt( const char* data);
long Char2Num(char* data);
long atoi_bt( const char* data); 

// 入出力関係のｲﾆｼｬﾗｲｽﾞ
// Initialize all devices.
short AllInit(void);
short AllInitial(BOOL resume);
// 起動直後の初期画面
// Initial display after startup
int StartDisplay(char *title,char *l1,char *l2,char *l3,char *l4,char *g1,char *g2,char *g3);

//Set scan code for scanner.
void SetScanCode(int code);

//オートパワーオフの時間設定
//Set auto power off time.
void SetAutoPowerOff(long min );

// 画面表示系
// Concern display
void LcdOut( BT900_LCD* lcd);
void LcdOutLeg( char* msg1,char *msg2,char *msg3);
void LineOut( char* dat, short lineNo,short reverse);// normal size char. 行指定の画面表示
void ClineOut( char* dat, short lineNo,char color);// normal size char. 行指定の画面表示
void LineOut2( char* dat, short lineNo,short rev_mode);//2 times size char of vertical. 縦のみ2倍角
void LineOut4( char* dat, short lineNo,short reverse);// 4 times size char.メッセージの出力（４倍角）
void LcdClear(void);// 画面クリア Clear display

// ビープ音をならす
// Ring the beep
void Beep(int mode);
void Buzzer(void);
void BuzzerV(void);			//2012.02.12 matuda カシオ互換関数追加 legacy function.
void BuzzerComp(void);

//Concern datatime
// 現在の時刻の取得
// Get current time
short GetNowtime( char *data );		//legacy function. don't use to new program.
void get_nowtime( char *data ,short mode);
int Calctime( char *src,char *dest,short days);
int InputDate( const char* title,const char *msg1,const char* msg2,const char *msg3,char *date );
int InputDateEng( const char* title,const char *l1,const char* l2,const char *l3,char *dest_day);

// get_nowtimeの第４引数の定数
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
// getnowtimeの第2引数の定数
#define TIMEST	0			// 数値構造体で返す場合
// 時間取得用の構造体
typedef struct{
	short year;
	short month;
	short day;
	short hour;
	short min;
	short sec;
} TMSTRCT;
long Ltime( void );// 午前0時からの経過秒数の取得
//Set current time by argument.
int set_nowtime( char *setdate);	// 2008.02.29


void Wait(long time);// ウエイト
// 16進（文字列）--> 10進(short)変換
// hex to decimal arithmetic.
short	htoi( char *hexstr , short len );

//Trim function
void lTrim( char* buf,int len);
void Trim( char* buf,int len);

int DspMsgBt( char *title,int guideno,char* l1,char* l2,char* l3,char* l4,char* l5,
				char* l6,char *l7,ENABLE_WAITKEYS* enkeys);
// 2012.12.06 coba ガイドも指定できるように追加 add guide message.
int DspMsgBt2( char *title,int guideno,char* l1,char* l2,char* l3,char* l4,char* l5,
				char* l6,char *l7,char* l9,ENABLE_WAITKEYS* enkeys);
// 2013.02.14 coba 1行増やす
int DspMsgBt3( char *title,int guideno,char* l1,char* l2,char* l3,char* l4,char* l5,
				char* l6,char *l7,char* l8,char* l9,ENABLE_WAITKEYS* enkeys);

//DT-950用下位互換関数
int DspMsg( char *title,int guideno	,char* l1,char* l2,char* l3,char* l4,char* l5,ENABLE_WAITKEYS* enkeys);
int DspMsg2( char *title,int guideno	,char* l1,char* l2,char* l3,char* l4,char* l5,char *l6,ENABLE_WAITKEYS* enkeys);

// Scroll screen
void SetScrollMsg( char* dat, short lineNo, short rev_mode );
int ShowScrollScreen(const char *title,int rows);

//Menu Screen Function ===============================================
int MenuScreen(const char *title,MENU_ITEMS itemname,short itemqty);


// データ入力
// Structure for DataInput*** function.
typedef struct{
	// 設定
	int mode;			// 0=edit,1=bar,2=edit+bar
	char title[20+1];	// titale.ウインドウタイトルへのポインタ(LPCTSTR=ワイド文字列へのポインタ)
	char l1[20+1];		// charactor for line 1. 1行目に表示する文字列
	char l2[20+1];		// charactor for line 2. 2行目に表示する文字列
	char l3[20+1];		// charactor for line 3. 3行目に表示する文字列
	char l4[20+1];		// charactor for line 4. 4行目に表示する文字列
	char l5[20+1];		// charactor for line 5. 5行目に表示する文字列
	char l6[20+1];		// charactor for line 6. 5行目に表示する文字列 2011.04.12 coba追加
	char l7[20+1];		// charactor for line 7. 5行目に表示する文字列
	char* buffer;		// The data pointer for storing the data( key ,barcode )データを格納するポインタ
	unsigned int bufsize;// The data size of storing data buffer.↑のバッファサイズ
	BOOL emptyOK;		// If TRUE,allow to imput empty.  TRUE=空文字列の入力を許可する
	BOOL upkey;			// If TRUE,Enable up key.↑キー抜け有効
	BOOL downkey;		// If TRUE,Enable down key.↓キー抜け有効
	// 出力
	BOOL bar_read;		// When scan barcode,this value become TRUE,バーコードを読んだとき=TRUEになる
	int keychange;		// 0:only number,1:can change input mode. 0:数字のみ(デフォルト) 1:入力切替あり 2006.11.25 matsuda
	char defdata[20+1];	// The default value of key input.キー入力時の初期値 2008.02.29
	char alpha;			// 0:can not input alphabet,1: can input alphabet.アルファベット入力あり
	// 色指定,0=黒、あとは下のマクロを参照 font color,0=black else refer CLR_??? macros
	char c1;
	char c2;
	char c3;
	char c4;
	char c5;
	char c6;
	char c7;
} DINPUT_SETTING;
// 文字色指定マクロ
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
short InputAlphabet( char *databuf,char *msg );	//Input just alphabet.アルファベット入力のみの関数

//データ入力系
//Legacy input data functions.Don't use to new program.-----------------------------------------
short DataInput( short mode, char *databuf, char *name, char *msg, short len);
int KeyInput( char *databuf, short len );// Input data by keyboard.キーボードからのデータ入力（置き換え機能付き）
short BarRead(char *databuf, short len,short mode);// Legacy function.
short FuncSel(short mode);// Function key selection.ファンクションキー選択
//----------------------------------------------------------------------------------------------

//Show OK,NG by big font OK,NGのデカ文字表示
void OkNgDisp( BOOL okng,int timer );

//無線通信関数
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
// 以下は内部で保持するマスタファイル関係。マスタファイル＝TTextTable的な使い方ができるファイル
//------------------------------------------------------------------------------------------------------------
// 2011.07.23 matsuda 元々ドライブ2にしていたが、これは揮発メモリだったので
// 					  不揮発メモリ上のドライブ1に変更(以下７ファイル)
#define MTRFILE_NAME "1:master.dat"
#define MTRIDXFILE_NAME "1:master.idx"
#define MTRSYSFILE_NAME "1:__master.idx"	//BT-1500で勝手に作成されるIndexファイル

#define MODMTRFILE_NAME "1:master_mod.dat"
#define MODMTRIDXFILE_NAME "1:master_mod.idx"
#define MODMTRADDFILE_NAME "1:master_mod.add"
#define MODMTRSYSFILE_NAME "1:__master_mod.idx"	//BT-1500で勝手に作成されるIndexファイル

int MasterFind( char *keyname,char *data );	//マスタファイル内の検索処理
int MasterUpd( char *keyname,char *data );	//マスタへのデータ追加

#endif
