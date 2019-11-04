//=============================================================================
//		キーエンスBT-900バーコード用ライブラリ
//		File name : nokbt900.c
//
//		生産技術本部  生産技術部
//		作成者 ： coba
//		作成日 ： ver.0.00 2004.06.11
//		更新日 ： ver.1.00 2004.06.28 TAKE 手直し＆関数の追加
//		更新日 ： ver.1.01 2004.08.17 TAKE 実績送信後、画面表示するように変更
//		更新日 ： ver.1.02 2004.12.28 TAKE btRfOpen()の返り値チェックを BT_RF_API_SET_ERR→BT_RF_API_SET_OKに変更
//		更新日 ： ver.1.03 2005.12.06 TAKE 作業者入力を２桁でも入力できるようにする
//		更新日 ： ver.0.02 2006.09.24 TAKE TPEブーツ原料FP版
//=============================================================================
//		上記を元にBT-1500用に置き換え 2010.08.03 ﾏﾂﾀﾞ
//		DINPUT_SETTINGの構造体の中にalpha（アルファベット入力）を追加 2012.04.18 okamura
//		画面表示を変更するためにDataInputBt2を追加 2012.04.18 okamura
//		2012.10.15 matsuda Initで読取りコードの設定を行うよう変更
//		2012.12.06 sanno	DataInputBt関数に、構造体中の初期表示値をセットしたときは初期値を表示するよう変更
//=============================================================================

#include "nokbt1500.h"

//TCPサーバーアドレスとポート
//TCP server address and TCP port No.
char g_HostAddr[100];
int g_PortNo;
int g_Lang;

void CloseSocket( long socketno );
//-----------------------------------------------------------------------------
//	関数名：short AllInit(void);
//	機能　：バーコード、ﾌｧｲﾙ、画面、キーボード等の初期化
//	引数　：なし
//	返値　：通常=0
//			その他：起動時に押されていたキー
//-----------------------------------------------------------------------------
//	Function Name:short AllInit(void)
//	Outline      :Initialize all devices.
//-----------------------------------------------------------------------------
short AllInit(void)
{
	return( AllInitial(FALSE));
}
// 2012.10.01 coba Resume指定を追加
short AllInitial(BOOL resume)
{
	long key,stat;
	short bkind;

	//Get language information;
	stat=btHandyGetParameter(PRM_LANGUAGE,&g_Lang);
	if(stat!=BT_OK)g_Lang = BT_LANG_JAPANESE;
	
	if( g_Lang == BT_LANG_JAPANESE )
		LineOut("   初期化中...",5,0);
	else 
		LineOut(" Initializing...",5,0);

	key = btKeyWaitEx( 0, 2);
	// Buzzer volume = MAX ブザー音量(MAX)
	btVolume(3);
g_Lang=100;
	//レジューム機能OFF 2012.10.01 coba 引数で機能の有効・無効を切替え
	if(resume == FALSE){	//if resume == FALSE, disable resume function
		stat = 0;
		btHandyGetParameter(PRM_RESUME,&stat);
		if( stat == BT_ON ){
			stat = BT_OFF;
			if( btHandySetParameter(PRM_RESUME,&stat) != BT_OK ){
				if( g_Lang == BT_LANG_JAPANESE )
					LineOut("ﾚｼﾞｭｰﾑﾊﾟﾗﾒｰﾀ設定失敗",8,0);
				else
					LineOut("Can not set resume params",8,0);

				FuncSel(0);
				return -20;
			}
		}
	}

	//Initialize the wireless setting.無線関係の設定確認
	if( WLAN_init() ){
		//失敗=IP設定されてないので設定させる
		//Falure = Don't have IP address setting. so Execute input function.
		WLAN_setting();
		if( WLAN_init() ){
			if( g_Lang == BT_LANG_JAPANESE ){
				LineOut( "無線通信設定ｴﾗｰ!!",2,0);
			}
			else{
				LineOut( "Wireless setting",2,0);
				LineOut( "error!!",4,0);
			}
			FuncSel(0);
			return(-2);
		}
	}

	//無線ポートオープン
	// 一度すべてのネットワーククローズ
	// Close all wireless module
	btNetworkClose( 0 );
	// Open wireless module
	if( btNetworkIsOpen( BT_NETWORK_WLAN_INFRA ) == BT_NETWORK_ALREADYOPEN ){
		btNetworkClose(BT_NETWORK_WLAN_INFRA);//If failure,Close once. 失敗したら一旦Closeして
	}
	//同期：BT_ON_SYNC、非同期:BT_ON_ASYNC
	stat=btNetworkOpen(BT_NETWORK_WLAN_INFRA,BT_ON_SYNC);
	if(stat != BT_NETWORK_OK ){	
		if( g_Lang == BT_LANG_JAPANESE )
			LineOut( "ﾎﾟｰﾄOpenｴﾗｰ!!",2,0);
		else 
			LineOut( "TCP Port Open error",2,0);

		FuncSel(0);
		return(-2);
	}

	// Setting of enable scan code.
	bkind = 0x00;
	bkind = bkind | 0x02;	// code39
	bkind = bkind |	0x08;	// code128
	bkind = bkind | 0x04;	// GS1-128
	bkind = bkind | 0x400;	// QR
	bkind = bkind | 0x200;	// gs1 data bar
	bkind = bkind | 0x1000;	// Data Matrix

	// バーコード読み取りオプションの変更
	if( btSetBcrProperty(BT_PROP_ENABLE_SYMBOLS,&bkind)!= NO_ERR ){
		if( g_Lang == BT_LANG_JAPANESE )
			btMessageBox("バーコード読み取りモード設定エラー",BT_MB_CONFONLY);
		else
			btMessageBox("Enable scan code setting error",BT_MB_CONFONLY);
	}

 //   btWait(2,0);
	// C + Fx
	if( key & KEY_CLR ){
		if( key & KEY_F1 ) return(-31);
		if( key & KEY_F2 ) return(-32);
		if( key & KEY_F3 ) return(-33);
	}

	// L + Fx
	if( key & KEY_L ){
		if( key & KEY_F1 ) return(-21);
		if( key & KEY_F2 ) return(-22);
		if( key & KEY_F3 ) return(-23);
	}

	// Fx
	if( key & KEY_F1 ) return(-11);
	if( key & KEY_F2 ) return(-12);
	if( key & KEY_F3 ) return(-13);

	return(0);
}
//-----------------------------------------------------------------------------
//	Function name: void SetScanCode(int code)
//	Outline      : Set scan code for scanner
//	Arg1         : scan code SCAN_ALL=39,128,QR, SCAN_CODE39=only code39 ,SCAN_CODE128=only code128, SCAN_CODEQR=coly QR code. 
//-----------------------------------------------------------------------------
void SetScanCode(int code)
{
	short bkind;

	bkind = 0x00;
	switch(code){
		case SCAN_ALL:bkind = bkind | 0x02;	// code39
					  bkind = bkind | 0x08;	// code128
					  bkind = bkind | 0x04;	// GS1-128
					  bkind = bkind | 0x400;	// QR
					  break;

		case SCAN_CODE39:
					bkind = bkind | 0x02;
					break;

		case SCAN_CODE128:
					bkind = bkind | 0x08;
					bkind = bkind | 0x04;	// GS1-128
					break;

		case SCAN_CODE39_128:
					bkind = bkind | 0x02;	// code39
					bkind = bkind | 0x08;	// code128
					bkind = bkind | 0x04;	// GS1-128
					break;

		case SCAN_CODEQR:
					bkind = bkind | 0x400;	// QR
					break;

		default:return;
	};
	// Change scanning option.
	if( btSetBcrProperty(BT_PROP_ENABLE_SYMBOLS,&bkind)!= NO_ERR ){
		if( g_Lang == BT_LANG_JAPANESE )
			btMessageBox("バーコード読み取りモード設定エラー",BT_MB_CONFONLY);
		else
			btMessageBox("Enable scan code setting error",BT_MB_CONFONLY);
	}
}


//-----------------------------------------------------------------------------
//	Function name: void WLAN_reset(void)
//	Outline      : Reset wireless devices.
//	Arg1         : nothing
//-----------------------------------------------------------------------------
void WLAN_reset(void)
{
	long stat;

	btNetworkClose( 0 );

	if( btNetworkIsOpen( BT_NETWORK_WLAN_INFRA ) == BT_NETWORK_ALREADYOPEN ){
		btNetworkClose(BT_NETWORK_WLAN_INFRA);// 失敗したら一旦Closeして
	}

	//同期：BT_ON_SYNC、非同期:BT_ON_ASYNC
	stat=btNetworkOpen(BT_NETWORK_WLAN_INFRA,BT_ON_SYNC);
	if(stat != BT_NETWORK_OK ){	
		if( g_Lang == BT_LANG_JAPANESE )
			LineOut( "ﾎﾟｰﾄOpenｴﾗｰ!!",2,0);
		else
			LineOut( "Port open error!!",2,0);
		FuncSel(0);
		return;
	}
}

//-----------------------------------------------------------------------------
//  Function name :void SetAutoPowerOff(void)
//	Outline       :Set auto power off time
//  Arg1          :power off time( minutes ) ,-1=Disable auto power off.
//-----------------------------------------------------------------------------
void SetAutoPowerOff( long min )
{
	if( min < -1 || min > 60 ) return;

	if( btHandySetParameter(PRM_AUTOPOWEROFF,&min)!= BT_OK ){
		if( g_Lang == BT_LANG_JAPANESE )
			LineOut("オートパワーオフ設定失敗",4,0);
		else{
			LineOut("Can not set ",4,0);
			LineOut("auto power off",6,0);
		}
		FuncSel(0);
		return;
	}
}

//-----------------------------------------------------------------------------
//	Function name:int StartDisplay(char *title,char *l1,char *l2,char *l3,char *l4,char *g1,char *2,char *3)
//  Outline      :Show start up screen.
//  Arg1         :title of screen
//  Arg2-4       :character for display.( big size character)
//  Arg5-7       :character for key guide (small size character)
//-----------------------------------------------------------------------------
int StartDisplay(char *title,char *l1,char *l2,char *l3,char *l4,char *g1,char *g2,char *g3)
{
	long key;
	char keycode;
	//title タイトル
	LineOut( title, 0, 1);	//Reversal character.タイトルのみ反転文字列
	
	//line 1. Big size char.１行目　大文字
	LineOut2( l1, 1, 0);
	//line 2. Big size char.２行目　大文字
	LineOut2( l2, 3, 0);
	//line 3. Big size char.３行目　大文字
	LineOut2( l3, 5, 0);

	//guide 1. small char. ガイド１　小文字
	LineOut( g1, 8, 0);
	//guide 1. small char.  ガイド２　小文字
	LineOut( g2, 9, 0);
	//guide 1. small char.  ガイド３　小文字
	LineOut( g3, 10, 0);

	while(1){
		// キー入力を待つ
		// wait to keyinput.
		btKeyBufClear();
		key = btKeyWait(0);
		// change logical key.  論理キーに変換する
		btTbl2Code(key,&keycode);
		break;
	}
	return keycode;
}

//-----------------------------------------------------------------------------
//	関数名：void LineOut4( char* dat, short lineNo,short reverse)
//	機能　：画面表示（表示only）
//	引数１：
//		４倍角表示
//-----------------------------------------------------------------------------
//  Function name:void LineOut4( char* dat, short lineNo,short rev_mode)
//  Outline      :Display function line by line. character size is 2 times vertical and 2 times horizon.
//  Arg1         :display data.
//  Arg2         :line No.(display position)
//  Arg3         :0:non reversal 1:reversal
//-----------------------------------------------------------------------------
void LineOut4( char* dat, short lineNo,short rev_mode)
{
	DSPATTR datr;
	char dsp[10+2];

	if( lineNo < 0 || lineNo >=LCD_ROW_MAX ) return;

	btmemset(dsp, 0,sizeof(dsp));
	btmemset(dsp, 0x20, 10);
	btmemcpy( dsp, dat,btstrlen(dat)>10 ? 10:btstrlen(dat));

	// set display attribute. 表示属性設定
	btmemset( &datr, 0, sizeof(DSPATTR));
	datr.tposition.x=1;
	datr.mag = BT_MAG_4;
	datr.fold = BT_FOLD_OFF;

	// reversal 反転
	if(!rev_mode ) datr.attribute=BT_ATRB_NORM;
	else datr.attribute=BT_ATRB_REV;

	// display position 行
	datr.tposition.y=1+(lineNo*2);

	btPrint( dsp, &datr);
}

//-----------------------------------------------------------------------------
//  Function name:void LineOut2( char* dat, short lineNo,short rev_mode)
//  Outline      :Display function of line by line. character size is 2 times vertical.
//  Arg1         :display data.
//  Arg2         :line No.(display position)
//  Arg3         :0:non reversal 1:reversal
//-----------------------------------------------------------------------------
void LineOut2( char* dat, short lineNo,short rev_mode)
{
	DSPATTR datr;
	char dsp[20+2];

	if( lineNo < 0 || lineNo >=LCD_ROW_MAX ) return;

	btmemset(dsp, 0,sizeof(dsp));
	btmemset(dsp, 0x20, 20);
	btmemcpy( dsp, dat,btstrlen(dat)>20 ? 20:btstrlen(dat));

	// set display attribute. 表示属性設定
	btmemset( &datr, 0, sizeof(DSPATTR));
	datr.tposition.x=1;
	datr.mag = BT_MAG_TALL;
	datr.fold = BT_FOLD_OFF;

	// reversal 反転
	if(!rev_mode ) datr.attribute=BT_ATRB_NORM;
	else datr.attribute=BT_ATRB_REV;

	// set display position 行
	datr.tposition.y=1+(lineNo*2);

	btPrint( dsp, &datr);
}

//-----------------------------------------------------------------------------
//	関数名：void LineOut( char* dat, short line,short reverse);
//	機能　：行を指定した画面表示（表示only）
//			指定行以外は変化しない
//	引数１：表示データ
//	引数２：表示行(1-8)
//	引数３：0=通常、1=反転表示
//-----------------------------------------------------------------------------
//  Function name:void LineOut2( char* dat, short lineNo,short rev_mode)
//  Outline      :Display function of line by line. character size is normal.
//  Arg1         :display data.
//  Arg2         :line No.(display position)
//  Arg3         :0:non reversal 1:reversal
//-----------------------------------------------------------------------------
void LineOut( char* dat, short lineNo, short rev_mode )
{
	DSPATTR datr;
	char dsp[20+2];

	if( lineNo < 0 || lineNo >=LCD_ROW_MAX ) return;

	btmemset(dsp, 0,sizeof(dsp));
	btmemset(dsp, 0x20, 20);
	btmemcpy( dsp, dat,btstrlen(dat)>20 ? 20:btstrlen(dat));

	// 表示属性設定
	btmemset( &datr, 0, sizeof(DSPATTR));
	datr.tposition.x=1;
	datr.mag = BT_MAG_NORM;
	datr.fold = BT_FOLD_OFF;

	// 反転
	if( !rev_mode )
		datr.attribute = BT_ATRB_NORM;
	else
		datr.attribute = BT_ATRB_REV;

	// 行
	datr.tposition.y=1+(lineNo*2);

	btPrint( dsp, &datr);

}
//-----------------------------------------------------------------------------
//	関数名：void LineOut( char* dat, short line,short reverse);
//	機能　：行を指定した画面表示（表示only）
//			指定行以外は変化しない
//	引数１：表示データ
//	引数２：表示行(1-8)
//	引数３：色指定
//-----------------------------------------------------------------------------
//  Function name:void LineOut2( char* dat, short lineNo,short rev_mode)
//  Outline      :Display function of line by line. character size is normal.
//  Arg1         :display data.
//  Arg2         :line No.(display position)
//  Arg3         :macro of font color (CLR_xxx)
//-----------------------------------------------------------------------------
void ClineOut( char* dat, short lineNo, char color)
{
	DSPATTR datr;
	char dsp[20+2];
	unsigned long dotcolor_tmp = 0; // カラー設定退避用
	unsigned long backcolor_tmp = 0;
	unsigned long seldotcolor_tmp = 0;
	unsigned long selbackcolor_tmp = 0;
	unsigned long nowcolor;
	int ret;

	// Back up color settings. カラー設定の退避
	ret = btGetColor( &dotcolor_tmp, &backcolor_tmp, &seldotcolor_tmp, &selbackcolor_tmp );

	if( lineNo < 0 || lineNo >=LCD_ROW_MAX ) return;

	btmemset(dsp, 0,sizeof(dsp));
	btmemset(dsp, 0x20, 20);
	btmemcpy( dsp, dat,btstrlen(dat)>20 ? 20:btstrlen(dat));

	// 表示属性設定
	btmemset( &datr, 0, sizeof(DSPATTR));
	datr.tposition.x=1;
	datr.mag = BT_MAG_NORM;
	datr.fold = BT_FOLD_OFF;
	datr.attribute = BT_ATRB_NORM;

	// 行
	datr.tposition.y=1+(lineNo*2);

	// 色設定
	if( color==CLR_RED ) nowcolor=RED ;
	else if( color==CLR_BLUE ) nowcolor=BLUE ;
	else if( color==CLR_YELLOW ) nowcolor=YELLOW ;
	else if( color==CLR_CYAN   ) nowcolor=CYAN   ;
	else if( color==CLR_GREEN  ) nowcolor=GREEN  ;
	else if( color==CLR_MAGENTA ) nowcolor=MAGENTA ;
	else if( color==CLR_WHITE   ) nowcolor=WHITE   ;
	else /*if( clolor==CLR_BLACK )*/ nowcolor=BLACK ;

	// 文字色のみ変更
	ret = btSetColor( nowcolor, backcolor_tmp, seldotcolor_tmp, selbackcolor_tmp );

	btPrint( dsp, &datr);

	// 文字色戻す
	ret = btSetColor( dotcolor_tmp, backcolor_tmp, seldotcolor_tmp, selbackcolor_tmp );

}
//-----------------------------------------------------------------------------
//	関数名：void LcdClear(void);
//	機能　：機能　：画面のクリア
//	引数　：なし
//	返値　：なし
//-----------------------------------------------------------------------------
//  Function name: void LcdClear(void);
//	Outline      : Clear screen.
//	argument     : nothing
//	return value : nothing
//-----------------------------------------------------------------------------
void LcdClear(void)
{
	btClear();
}
//-----------------------------------------------------------------------------
//  Function name:void SetScrollMsg( char* dat, short lineNo, short rev_mode )
//  Outline      :Set data for screen by scroll display
//  Arg1         :display data.
//  Arg2         :line No.(display position)
//  Arg3         :0:non reversal 1:reversal
//-----------------------------------------------------------------------------
void SetScrollMsg( char* dat, short lineNo, short rev_mode )
{
	DSPATTR datr;
	char dsp[20+2];

	if( lineNo < 0 ) return;

	//Don't show immediate to screen
	btLcdUpdate(BT_UPDATE_OFF);

	btmemset(dsp, 0,sizeof(dsp));
	btmemset(dsp, 0x20, 20);
	btmemcpy( dsp, dat,btstrlen(dat)>20 ? 20:btstrlen(dat));

	// setting of display attribute
	btmemset( &datr, 0, sizeof(datr));
	datr.tposition.x=1;
	datr.mag = BT_MAG_NORM;
	datr.fold = BT_FOLD_OFF;

	// reversal
	if( !rev_mode )
		datr.attribute = BT_ATRB_NORM;
	else
		datr.attribute = BT_ATRB_REV;

	// row
	datr.tposition.y=1+(lineNo*2);

	btPrint( dsp, &datr);
}
//-----------------------------------------------------------------------------
//  Function name:int ShowScrollScreen(const char *title,int rows)
//  Outline      :Show data by scroll mode.
//  Arg1         :Title of screen
//  Arg2         :Total rows for display
//	Return Value :keycode
//-----------------------------------------------------------------------------
int ShowScrollScreen(const char *title,int rows)
{
	short top = 0;//scroll header row
	short bottom; //bottom position(dot)
	short step = 25; //scroll step= 2 row
	short Pos = 0; //scroll Y position
	int i = 0;
	short ret = 0;

	bottom = (rows+1) * 25;	//rows * 1 rows(1rows=25dot)

	btLcdUpdate(BT_UPDATE_ON); //show VRAM buffer to screen
	ret = btViewer((char*)title, top, bottom, step, &Pos);
	switch(ret){
		case KEYCODE_ENT:ret=0;break;
		default:ret=-1;break;
	};
	return ret;
}
//-----------------------------------------------------------------------------
//	関数名：short InputAlphabet( char *databuf )
// 	機能　：アルファベットの入力関数…携帯電話のような入力A～Z
//			数字入力や小文字入力は大文字に変換する機能あり
//	引数１：char *databuf 取得文字列格納ポインタ(出力)
//	返値　：0=正常　:1=終了 -1=F1終了
//-----------------------------------------------------------------------------
//	Function name:short InputAlphabet( char *databuf ,char *msg)
//	Outline      :Input alphabet just 1 character.
//                This function change to uppercase.
//  Arg1         :destination data pointer
//  Arg2         :Display title name
//  Return value :-1:cancel(F1 key) 0:success
//-----------------------------------------------------------------------------
short InputAlphabet( char *databuf ,char *msg)
{
	short keycode;
	char dat[50+1];
	DSPATTR dspattr;
	INPUTSTRATTR strattr;

	// set attribute. 表示属性設定
	btmemset( &dspattr, 0, sizeof(DSPATTR));
	dspattr.mag = BT_MAG_4;
	dspattr.fold = BT_FOLD_ON;
	dspattr.attribute=BT_ATRB_NORM;

	LcdClear();

	// set input setting 入力設定
	btmemset( &strattr, 0, sizeof(INPUTSTRATTR));
	strattr.inputarea=20;
	strattr.mindigit=1;
	strattr.maxdigit=1;

	// key入力
	// 2行目に表示
	dspattr.tposition.x=1;
	dspattr.tposition.y=7;
	strattr.inputtype=BT_INPUT_KEY_SFT_ON|BT_INPUT_ESCAPE_ON;
	strattr.mindigit=1;
	LineOut2( msg,0,0);
	if( g_Lang == BT_LANG_JAPANESE ){
		LineOut( " (A～Zを1文字入力)",2,0);
		LineOut( "文字は大文字に自動",7,0);
		LineOut( "変換されます",8,0);
		LineOut( "ENT:OK C:Del F1:中止",9,0);
	}
	else{
		LineOut( " (Input A～Z)",2,0);
		LineOut( "Character will be ",7,0);
		LineOut( "uppercase",8,0);
		LineOut( "ENT:OK C:Del F1:ESC",9,0);
	}
	while(1){
		// Input function .入力
		btmemset( dat, 0, sizeof(dat));
		keycode = btInputString(dat,&dspattr,&strattr);
		// finish to input.入力完了
		if( keycode == KEYCODE_ENT && btstrlen(dat) ){
			//change to uppercasse.小文字アルファベットの場合の処理
			if( dat[0] >= 0x61 && dat[0] <= 0x7a ){
				dat[0] -= 0x20;
				break;
			}
			//If data is number....数字入力の場合の処理
			if( dat[0] >= 0x31 && dat[0] <= 0x39 ){
				dat[0] += 0x10;
				break;
			}
			//Uppercase is break.通常の大文字ならbreak;
			if( dat[0] >= 0x41 && dat[0] <= 0x5a ) break;
		}
		if( keycode == KEYCODE_F1 )return -1;
	}

	// バッファにコピー
	btmemcpy( databuf, dat,1);

	// カーソルOFF
	btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);
	LineOut("",1,0);
	return 0;
}

//-----------------------------------------------------------------------------
//	関数名：short NoSel(short no)
//	機能　：引数で指定した数の分だけテンキーを有効にして、押されたキーを返す。
//	引数　：short no 選択項目数(0～9)
//	返値　：押されたキー（ex.'1'が押されたら返値も１)
//-----------------------------------------------------------------------------
//  Function name:short NoSel(short no,int cancel)
//  Outline      :Enable key of argument 1, and return pushed key code.
//  Arg1         :Enable keys( 0-9 )
//  Arg2         :non 0=Disable cancel 1:Enable cancel
//  Return value :pushed key code.
//-----------------------------------------------------------------------------
short NoSel(short no,int cancel)
{
	long key;
	char keycode;

	while(1){
		// キー入力を待ち、論理キーに変換する
		btKeyBufClear();
		key = btKeyWait(0);
		btTbl2Code(key,&keycode);
		// Enable keys. 有効なキー
		if( keycode >='0' && keycode <= no+'0') return( (int)keycode-'0');
		if( keycode == KEYCODE_R ) return -1;
		if( keycode == KEYCODE_L ) return 0;
		if( keycode == KEYCODE_ENT ) return 0;
		// 2013.02.06 coba TRGも有効にした
		if( keycode == KEYCODE_TRG ) return 0;
		// 2013.07.22 matsuda F1も有効とするよう変更
		if( keycode == KEYCODE_F1 ) return -2;
		
		// cancel ｷｬﾝｾﾙ
		else if( cancel && (keycode == KEYCODE_CLR || keycode == KEYCODE_BS)) break;
	}
	return(-1);
}

//-----------------------------------------------------------------------------
//	関数名：short NoSel(short no)
//	機能　：引数で指定した数の分だけテンキーを有効にして、押されたキーを返す。
//	引数　：short no 選択項目数(0～9)
//	返値　：押されたキー（ex.'1'が押されたら返値も１)
//	カシオとの互換関数
//-----------------------------------------------------------------------------
//  Function name:short NoSel(short no)
//  Outline      :Enable key by argument 1's structure.and return pushed key.
//  Arg1         :ENABLE_WAITKEYS structure pointer.
//  Return value :If pushed key is number , return key code.
//                ENT & L=0,  R=-1,  F1=-2  ,Up key = 'U' , Down key='D'
//-----------------------------------------------------------------------------
short NoSelEnableKey(ENABLE_WAITKEYS *ekey)
{
	long key;
	char keycode;

	while(1){
		// キー入力を待ち、論理キーに変換する
		btKeyBufClear();
		key = btKeyWait(0);
		btTbl2Code(key,&keycode);
		if( ekey == NULL ) return 0;
		if( ekey->anykey ){
			// 有効なキー
			if( keycode >=KEYCODE_0 && keycode <= KEYCODE_9) return( (int)keycode-'0');
			if( keycode == KEYCODE_F1 ) return -2;

			if( g_Lang == BT_LANG_JAPANESE )
				if( keycode == KEYCODE_L ) return 0;
				if( keycode == KEYCODE_R ) return -1;
			else{
				if( keycode == KEYCODE_L ) return -1;
				if( keycode == KEYCODE_R ) return 0;
			}
			if( keycode == KEYCODE_ENT ) return 0;
			// ｷｬﾝｾﾙ
			else if( keycode == KEYCODE_CLR || keycode == KEYCODE_BS) return -1;
		}
		switch( keycode ){
			case KEYCODE_ENT:
				if(ekey->keyEnt==TRUE || ekey->keyL==TRUE) return 0; break;
			case KEYCODE_L:		
					if( g_Lang == BT_LANG_JAPANESE ){
						if(ekey->keyL==TRUE ) return 0; 
					}
					else{
						if(ekey->keyL==TRUE ) return -1; 
					}
					break;

			case KEYCODE_R:
					if( g_Lang == BT_LANG_JAPANESE ){
						if(ekey->keyR==TRUE ) return -1; 
					}
					else{
						if(ekey->keyR==TRUE ) return 0; 
					}
					break;
			case KEYCODE_CLR:
			case KEYCODE_BS:
			case KEYCODE_F1:
				if(ekey->keyClr==TRUE ) return -1;
				if(ekey->keyEsc==TRUE ) return -2;
				break;
			case KEYCODE_UP:	if(ekey->keyUp==TRUE ) return 'U'; break;
			case KEYCODE_DOWN:	if(ekey->keyDown==TRUE ) return 'D'; break;
			case KEYCODE_0:		if(ekey->key0==TRUE ) return 0; break;
			case KEYCODE_1:		if(ekey->key1==TRUE ) return 1; break;
			case KEYCODE_2:		if(ekey->key2==TRUE ) return 2; break;
			case KEYCODE_3:		if(ekey->key3==TRUE ) return 3; break;
			case KEYCODE_4:		if(ekey->key4==TRUE ) return 4; break;
			case KEYCODE_5:		if(ekey->key5==TRUE ) return 5; break;
			case KEYCODE_6:		if(ekey->key6==TRUE ) return 6; break;
			case KEYCODE_7:		if(ekey->key7==TRUE ) return 7; break;
			case KEYCODE_8:		if(ekey->key8==TRUE ) return 8; break;
			case KEYCODE_9:		if(ekey->key9==TRUE ) return 9; break;
		}
	}
	return(-1);
}
//-----------------------------------------------------------------------------
//	関数名：short PingTest( char *address )
//	機能　：PINGの実行
//	引数１：Host Address
//	返値　：0=成功  ：1=失敗
//-----------------------------------------------------------------------------
//  Function name:short PingTest( void )
//  Outline      :Execute ping command.
//  Arg1         :nothing.
//  Return value :status of ping command.
//-----------------------------------------------------------------------------
short PingTest( void )
{
	long stat,ip;
	char addr[20];
	short timeout;

	//Get host ip address from handy terminal setting.ホストIPを取得
	btmemset(addr,0,sizeof(addr));
	if( btNetworkGetParameter( BT_TARGET_NETWORK,PRM_HOSTIP,addr ) != BT_OK ){
		if( g_Lang == BT_LANG_JAPANESE )
			LineOut( "ホストIPが未設定",2,0);
		else{
			LineOut( "Host IP address",2,0);
			LineOut( "does not exists.",2,0);
		}
		FuncSel(0);
		return(-2);
	}

	btClear();

	if( g_Lang == BT_LANG_JAPANESE )
		LineOut( "  PING   送信中   ",1,1);
	else LineOut( " PING executing...",1,1);

	ip = btSocketInet_addr(addr);
	// 送信
	timeout=50;
	stat = btPing(ip,timeout);

	return((short)stat);
}
//------------------------------------------------------------------------------------------------------
// Function name:int MenuScreen(const char *title,MENU_ITEMS itemname,short itemqty)
// Outlie       :Show menu screen and waiting choise by user.
// Arg1         :title of screen
// Arg2         :MENU_ITEMS structure (NOK structure)
// Arg3         :Menu item qty
// Reuturn value:1-16=choosen No of menu, less then 0 = cancel or error
//------------------------------------------------------------------------------------------------------
int MenuScreen(const char *title,MENU_ITEMS itemname,short itemqty)
{
	int i,ret;
	MENUINFO menuinfo;
	btmemset(&menuinfo,0,sizeof(menuinfo));

	menuinfo.tenkeydirect=BT_MENU_TKEYDIRECT_ON2;// allow selectable keyinput.
	if( itemqty > BT_MENU_SELMAX )itemqty=BT_MENU_SELMAX;
	menuinfo.totalitem=itemqty;
	menuinfo.inititem=1;
	for(i=0;i<menuinfo.totalitem;i++){
		menuinfo.menuitem[i] = itemname.item[i].name;
	}

	ret=btMenu((char*)title,&menuinfo);
	if(ret<0)return -1;

	return ret;
}


//---------------------------------------------------------------------------------------
//	int DspMsg( HINSTANCE inst,HWND hwnd,char* l1,char* l2,char* l3,char* l4,char* l5)
//	機能　：メッセージを表示する(14バイト/1行、5行表示)
//	引数１：インスタンスハンドル
//	引数２：親ウインドウのハンドル
//	引数３：タイトル(unicode)
//	引数４：最下行に表示するキーガイドの種類
//			0:何かｷｰを押して下さい
//			1:処理を選択して下さい
//			2:番号を選択して下さい
//			3:L=はい R=いいえ
//			6:オリジナルメッセージ
//	引数4-：表示するエラーメッセージ
//	引数５：有効にするｷｰ(NULLにすると、全てのｷｰが有効)
//	戻り値：0～9=押されたキーの番号
//			ENT(L)=0;
//			ESC(R)=-1
//---------------------------------------------------------------------------------------
// Function name:int DspMsgBt3( char *title,int guideno,char* l1,char* l2,char* l3,char* l4,char* l5,char* l6,
//                               						char *l7,char* l8,char* guide,ENABLE_WAITKEYS* enkeys)
// Outline      :Show message on screen by multi line and return pushed key code.
// Arg1         :title character( looks like Window title )
// Arg2         :guide No.
//                 0:Push any key
//                 1:Select the operation.
//                 2:Choose the number.
//                 3: L:Yes    R:No
//                 4: L:OK     R:Cancel
//                 5: L:OK
//                 6: This mode can show unique guide message.( "guide" will become to guide message)
// Arg3         :Message for line 1.
// Arg4         :Message for line 2.
// Arg5         :Message for line 3.
// Arg6         :Message for line 4.
// Arg7         :Message for line 5.
// Arg8         :Message for line 6.
// Arg9         :Message for line 7.
// Arg10        :Message for line 8.
// Arg11        :When argument 2 is 6, Show this argument character to guide message.
// Arg12        :Enable key structure pointer.
//---------------------------------------------------------------------------------------
int DspMsgBt( char *title,int guideno
						,char* l1,char* l2,char* l3,char* l4,char* l5,char* l6,
						char *l7,ENABLE_WAITKEYS* enkeys)
{
	return(DspMsgBt2(title,guideno,l1,l2,l3,l4,l5,l6,l7,"",enkeys));
}
int DspMsgBt2( char *title,int guideno
						,char* l1,char* l2,char* l3,char* l4,char* l5,char* l6,
						char *l7,char* guide,ENABLE_WAITKEYS* enkeys)
{
	return(DspMsgBt3(title,guideno,l1,l2,l3,l4,l5,l6,l7,"",guide,enkeys));
}
int DspMsgBt3( char *title,int guideno
						,char* l1,char* l2,char* l3,char* l4,char* l5,char* l6,
						char *l7,char* l8,char* guide,ENABLE_WAITKEYS* enkeys)
{
	char buf[50];

	LcdClear();

	LineOut( title, 0, 1);	//Show title as reversal. タイトルのみ反転文字列
	LineOut2( l1, 1, 0);
	LineOut( l2, 3, 0);
	LineOut( l3, 4, 0);
	LineOut( l4, 5, 0);
	LineOut( l5, 6, 0);
	LineOut( l6, 7, 0);
	LineOut( l7, 8, 0);
	LineOut( l8, 9, 0);


	switch(guideno){
		case 0:
			if( g_Lang == BT_LANG_JAPANESE )
				btsprintf(buf,"何かｷｰを押して下さい");
			else btsprintf(buf,"    PUSH ANY KEY");
			break;
		case 1:
			if( g_Lang == BT_LANG_JAPANESE )
				btsprintf(buf,"処理を選択して下さい");
			else btsprintf(buf," Select Operations");
			break;
		case 2:
			if( g_Lang == BT_LANG_JAPANESE )
				btsprintf(buf,"番号を選択して下さい");
			else btsprintf(buf," Choose a number");
			break;
		case 3:
			if( g_Lang == BT_LANG_JAPANESE )
				btsprintf(buf,"  L:はい  R:いいえ");
			else btsprintf(buf,"    L:NO  R:YES");
			break;
		case 4:
			if( g_Lang == BT_LANG_JAPANESE )
				btsprintf(buf," L:了解  R:処理中断");
			else btsprintf(buf,"    L:OK  R:Cancel");
			break;

		case 5:
			if( g_Lang == BT_LANG_JAPANESE )
				btsprintf(buf,"      [1]:了解");
			else btsprintf(buf,"      [1]:OK");
			break;
		// 2012.12.06 coba オリジナルメッセージ表示を追加
		case 6:
			btstrcpy(buf,guide);
			break;
		default:
			btmemset(buf,0,sizeof(buf));
			break;
	}
	LineOut( buf, 10, 0);

	//動作
	switch(guideno){
		case 1://処理を選択して下さい
		case 2://番号を選択して下さい
		case 3://  L:はい  R:いいえ
		case 4:
		case 6:
			return ( NoSelEnableKey( enkeys ) );
			break;
		case 0://何かｷｰを押して下さい
		case 5://    [1]:了解
		default:
			return ( NoSel(10,1) );
			break;
	}
	return 0;
}
//Compatible function for Casio DT-950. DT-950用下位互換関数
int DspMsg( char *title,int guideno
						,char* l1,char* l2,char* l3,char* l4,char* l5
						,ENABLE_WAITKEYS* enkeys){
	return( DspMsgBt( title,guideno,l1,l2,l3,l4,l5,"","",enkeys));
}
//DT-950用下位互換関数
int DspMsg2( char *title,int guideno
						,char* l1,char* l2,char* l3,char* l4,char* l5,char* l6
						,ENABLE_WAITKEYS* enkeys){
	return( DspMsgBt( title,guideno,l1,l2,l3,l4,l5,l6,"",enkeys));
}
//-----------------------------------------------------------------------------
//	関数名：void BuzzerComp(void)
//	機能　：完了のブザー
//	引数　：なし
//	返値　：なし
//	カシオ互換関数
//-----------------------------------------------------------------------------
//  Function name: void BuzzerComp(void)
//  Outline      : Ring buzzer when operation is OK.
//-----------------------------------------------------------------------------
void BuzzerComp(void){Beep(0);}
//-----------------------------------------------------------------------------
//	関数名：void Buzzer(void)
//	機能　：エラーの時のブザー
//	引数　：なし
//	返値　：なし
//	カシオ互換関数
//-----------------------------------------------------------------------------
//  Function name: void Buzzer(void)
//  Outline      : Ring buzzer when operation is NG.
//-----------------------------------------------------------------------------
void Buzzer(void){Beep(2);}
//-----------------------------------------------------------------------------
//	関数名：void BuzzerV(void)
//	機能　：エラーの時のブザー(バイブレーション付き)
//	引数　：なし
//	返値　：なし
//	カシオ互換関数
//	2011.02.12 matuda 追加
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Function name: void BuzzerV(void)
//  Outline      : Ring buzzer with vibration when operation is NG.
//-----------------------------------------------------------------------------
void BuzzerV(void)
{
	//BT_OFF 停止
	//BT_ON_ASYNC 動作1（非同期）
	//BT_ON_SYNC 動作2（同期）
	//ブザーがなっている間振動する(0.7sec)
	btVibration(BT_ON_ASYNC,7,0,1);
	Beep(2);
}
//-----------------------------------------------------------------------------
//	関数名：void Beep(void);
//	機能　：エラー時にビープ音をならす
//	引数　：なし
//	返値　：なし
//		＊設定は別のところでやっています
//-----------------------------------------------------------------------------
//  Function name:void Beep(int mode)
//  Outline      :Ring the beep.
//  Arg1         :Beep mode.
//                0: OK sound.
//                1: Warning sound.
//                other: Error sound.
//-----------------------------------------------------------------------------
void Beep(int mode)
{
	switch(mode){
		case 0:		// OK
		// 2011.11.08 matsuda
			btLed(BT_ON_ASYNC,1,1,2,BT_LED_GREEN);
			btBuzzer(BT_ON_ASYNC,1,1,2,7);
		break;

		case 1:		// WARN
			btLed(BT_ON_ASYNC,2,1,5,BT_LED_ORANGE);
			btBuzzer(BT_ON_ASYNC,2,1,5,10);
		break;

		default:	// ERR
			btLed(BT_ON_ASYNC,5,1,3,BT_LED_RED);
			btBuzzer(BT_ON_ASYNC,5,1,3,15);
		break;
	}
}
//-----------------------------------------------------------------------------
//	関数名：void OkNgDisp( BOOL okng,int timer )
//	機能　：CASIO dt-950の互換関数
//	引数１：ＴＲＵＥ：ＯＫ表示、ＦＡＬＳＥ：ＮＧ表示
//	引数２：タイマー秒 内部で×100msecされます
//	返値　：なし
//		＊設定は別のところでやっています
//-----------------------------------------------------------------------------
//  Function name:void OkNgDisp( BOOL okng,int timer )
//  Outline      :Show "OK" or "NG" by big character during a time.
//  Arg1         :TRUE=Show OK, FALSE=Show NG
//  Arg2         :timer ( Now , Disable this argument )
//-----------------------------------------------------------------------------
void OkNgDisp( BOOL okng,int timer )
{
	DSPATTRG datr;
	char dsp[20+2];
	unsigned long dotcolor_tmp = 0; // カラー設定退避用
	unsigned long backcolor_tmp = 0;
	unsigned long seldotcolor_tmp = 0;
	unsigned long selbackcolor_tmp = 0;
	int ret;

	btClear();
	// Back up color settings. カラー設定の退避
	ret = btGetColor( &dotcolor_tmp, &backcolor_tmp, &seldotcolor_tmp, &selbackcolor_tmp );

	// Set display attribute.表示属性設定
	btmemset( &datr, 0, sizeof(DSPATTR));
	datr.mag = BT_MAG_25;
	datr.fold = BT_FOLD_OFF;
	datr.attribute = BT_ATRB_NORM;

	// 行
	datr.gposition.x=0;
	datr.gposition.y=70;

	if( timer < 0 || timer > 50 ) timer=1;
	//fix to 1 for a long time.当面は1固定にします。
	timer=1;

	//NG
	if(okng==FALSE){	
		if( g_Lang == BT_LANG_JAPANESE )
			btstrcpy(dsp,"ＮＧ");
		else btstrcpy(dsp," NG ");
		ret = btSetColor( RED ,WHITE, BLACK,BLACK);
		btPrintg( dsp, &datr);

		btLed(BT_ON_ASYNC,timer,1,1,BT_LED_RED);
		btBuzzer(BT_ON_SYNC,timer,1,1,15);

		ret = btSetColor( WHITE,RED , BLACK,BLACK);
		btPrintg( dsp, &datr);
		btLed(BT_ON_ASYNC,timer,1,1,BT_LED_RED);
		btBuzzer(BT_ON_SYNC,timer,1,1,15);

		ret = btSetColor( RED ,WHITE, BLACK,BLACK);
		btPrintg( dsp, &datr);
		btLed(BT_ON_ASYNC,timer,1,1,BT_LED_RED);
		btBuzzer(BT_ON_SYNC,timer,1,1,15);

		ret = btSetColor( WHITE,RED , BLACK,BLACK);
		btPrintg( dsp, &datr);
		btLed(BT_ON_ASYNC,timer,1,1,BT_LED_RED);
		btBuzzer(BT_ON_SYNC,timer,1,1,15);

		ret = btSetColor( RED ,WHITE, BLACK,BLACK);
		btPrintg( dsp, &datr);
		btLed(BT_ON_ASYNC,timer,0,1,BT_LED_RED);
		btBuzzer(BT_ON_SYNC,timer,0,1,15);
	}
	//OK
	else{
		if( g_Lang == BT_LANG_JAPANESE )
			btstrcpy(dsp,"ＯＫ");
		else btstrcpy(dsp," OK ");
		ret = btSetColor( BLUE ,WHITE, BLACK,BLACK);
		btPrintg( dsp, &datr);

		btLed(BT_ON_ASYNC,2,0,1,BT_LED_GREEN);
		btBuzzer(BT_ON_ASYNC,2,0,1,5);
		Wait(10);
	}
	// Return the color setting.カラー設定を戻す
	ret = btSetColor( dotcolor_tmp, backcolor_tmp, seldotcolor_tmp, selbackcolor_tmp );

	btClear();
}

//=============================================================================
//	void get_nowtime( char *buf, short mode)
//	機能：現時刻の取得
//=============================================================================
//  Function name :void get_nowtime( char *buf, short mode)
//  Outline       :Get current time
//  Arg1          :destinatin data pointer.
//  Arg2          :mode
//=============================================================================
void get_nowtime( char *buf, short mode)
{
	DATEINFO di; //日付情報構造体変数名
	TIMEINFO ti; //時刻情報構造体変数名
	TMSTRCT *outTime;
	short ret;
	char dummy[50];

	ret = btGetDateTime (&di, &ti); //日付および時刻の取得を行います
	if( ret ) return;	//異常終了
	btsprintf( dummy,"%04d%02d%02d%02d%02d%02d", di.year,di.month,di.day,ti.hour,ti.min,ti.sec);
    switch( mode){
       case HDATEL:
          btmemcpy( buf, dummy, HDATEL );
          break;

       case LDATEL:
          btmemcpy( buf, dummy, LDATEL );
          break;

       case MDATEL:
          btmemcpy( buf, dummy, MDATEL );
          break;

       case SDATEL:
          btmemcpy( buf, dummy, SDATEL );
          break;

       case HOST_L:
			buf[0]= (di.year/100) - 19+'0';
			btmemcpy( &buf[1],&dummy[2],HOST_L-1);
			break;

		case TIMEST:
			outTime = (TMSTRCT*)buf;
			outTime->year = di.year;
			outTime->month = di.month;
			outTime->day = di.day;
			outTime->hour = ti.hour;
			outTime->min = ti.min;
			outTime->sec = ti.sec;
          break;

	   default:
          break;
   }
}

//-----------------------------------------------------------------------------
//	関数名：int Calctime( char *src,char *dest,short days)
//	機能　：日付の計算
//　引数１：計算元日付
//　引数２：計算結果日付出力ポインタ
//　引数３：加算・減算する日付
//  返値　：なし
//-----------------------------------------------------------------------------
//  Function name:int Calctime( char *src,char *dest,short days)
//  Outline      :Calculete the date.
//  Arg1         :Orignal date
//  Arg2         :Destination data pointer.
//  Arg3         :Increase or decrease days
//  Return value :0=success else=failure.
//-----------------------------------------------------------------------------
int Calctime( char *src,char *dest,short days)
{
	DATEINFO pdate,rdate;
	char buf[20];
	long num;
	int ret;

	if( btstrlen(src) < 8 ) return -1;

	btmemcpy(buf,src,4);
	buf[4]=0;
	btchar2num(buf,&num);
	pdate.year=(short)num;

	btmemcpy(buf,src+4,2);
	buf[2]=0;
	btchar2num(buf,&num);
	pdate.month=(char)num;

	btmemcpy(buf,src+6,2);
	buf[2]=0;
	btchar2num(buf,&num);
	pdate.day=(char)num;

	btmemset(&rdate,0,sizeof(rdate));
	ret = btAddDate( &pdate,days,&rdate);
	if(ret) return -1;

	btsprintf(buf,"%04d%02d%02d",rdate.year,rdate.month,rdate.day);
	btstrcpy(dest,buf);
	return 0;
}
//-----------------------------------------------------------------------------
//  Function name:int InputDate( const char* title,const char *msg1,const char* msg2,const char *msg3,char *date )
//  Outline      : Show current date and select date by up/down/left/right key
//  Arg1         : title
//	Arg2         : messag1
//	Arg3         : messag2
//	Arg3         : messag3
//	Arg4         : Destination data pointer
//-----------------------------------------------------------------------------
int InputDate( const char* title,const char *msg1,const char* msg2,const char *msg3,char *date )
{
	short ret;
	DATEINFO dinfo;
	DSPATTR dspattr = {{1,15},BT_ATRB_NORM,BT_MAG_TALL,BT_FOLD_OFF};

	LcdClear();
	LineOut( (char*)title, 0, 1);//Show title as reversal.
	LineOut2((char*)msg1, 1, 0);
	LineOut( (char*)msg2, 3, 0);
	LineOut( (char*)msg3, 4, 0);
	LineOut( "Day:Up/Down Month:<>", 10, 0);
	LineOut2("YYYY/MM/DD", 5, 0);

	btmemset(&dinfo,0,sizeof(dinfo));

	while(1){
		ret = btInputDate(&dinfo,&dspattr,1);
		if(ret==KEYCODE_ENT){
			btsprintf(date,"%04d%02d%02d",dinfo.year,dinfo.month,dinfo.day);
			break;
		}
		else if(ret==KEYCODE_F1) return -1;	//cancel
	}
	return 0;
}
//-----------------------------------------------------------------------------
//  Function name:int InputDateEng( const char* title,const char *l1,const char* l2,const char *l3,char *dest_day)
//  Outline      : Show current date and select date by up/down/left/right key
//  Arg1         : title
//	Arg2         : messag1
//	Arg3         : messag2
//	Arg4         : messag3
//	Arg5         : Destination data pointer
//-----------------------------------------------------------------------------
int InputDateEng( const char* title,const char *l1,const char* l2,const char *l3,char *dest_day)
{
	long key;
	int days=0;
	char keycode;
	char buf[50],sday[30],newday[30],now[10];
	char msg[50];

	get_nowtime(now,8);now[8]=0;
	btstrcpy(sday,now);
	btstrcpy(newday,now);

	LcdClear();
	LineOut( (char*)title, 0, 1);
	LineOut2( (char*)l1, 1, 0);
	LineOut( (char*)l2, 3, 0);
	LineOut( (char*)l3, 4, 0);
	LineOut2("     DD/MM/YY", 5, 0);

	btmemset(buf,0,sizeof(buf));
	btmemcpy(buf,now+6,2);
	btmemcpy(buf+btstrlen(buf),"/",1);
	btmemcpy(buf+btstrlen(buf),now+4,2);
	btmemcpy(buf+btstrlen(buf),"/",1);
	btmemcpy(buf+btstrlen(buf),now+2,2);

	btsprintf(msg,"     %s",buf);
	LineOut2( msg,7,0);
	LineOut( "Up/Down    Cancel:F1", 10, 0);


	while(1){
		btKeyBufClear();
		key = btKeyWait(0);
		btTbl2Code(key,&keycode);
		// 
		if( keycode == KEYCODE_UP ||  keycode == KEYCODE_DOWN ){
			btmemset(newday,0,sizeof(newday));
			switch(keycode){
				case KEYCODE_DOWN:days++;Calctime(sday,newday,days);break;
				case KEYCODE_UP:days--;Calctime(sday,newday,days);break;
			};
			btmemset(buf,0,sizeof(buf));
			btmemcpy(buf,newday+6,2);
			btmemcpy(buf+btstrlen(buf),"/",1);
			btmemcpy(buf+btstrlen(buf),newday+4,2);
			btmemcpy(buf+btstrlen(buf),"/",1);
			btmemcpy(buf+btstrlen(buf),newday+2,2);
			btsprintf(msg,"     %s",buf);
			LineOut2( msg,7,0);
		}
		else if( keycode == KEYCODE_ENT ){
			LcdClear();
			btstrcpy(dest_day,newday);
			break;
		}
		// cancel
		else if( keycode == KEYCODE_CLR || keycode == KEYCODE_F1){
			LcdClear();
			return -1;
		}
	}
	return 0;
}
//-----------------------------------------------------------------------------
//	関数名：void Wait(long time);
//	機能　：ウエイトをかける
//	引数　：ウエイトする秒数(*100msec)
//	返値　：なし
//-----------------------------------------------------------------------------
//  Function name:void Wait(long time);
//  Outline      :waiting....
//  Arg1         : wait time( order by second * 10 ), If need to wait 1 second , argument value is 10.
//-----------------------------------------------------------------------------
void Wait(long time)
{
	btWait( (short)time ,0);
}
//-----------------------------------------------------------------------------
//	関数名：int set_nowtime( char *setdate
//	機能　：引数で指定されたYYYYMMDDhhmmssに設定する
//	返値　：0=成功
//-----------------------------------------------------------------------------
//  Function name:int set_nowtime( char *setdate
//  Outline      :Set current time to handy terminal from argument time.
//  Arg1         :date-time YYYYMMDDhhmmss
//-----------------------------------------------------------------------------
int set_nowtime( char *setdate )
{
	char buf[20];
	DATEINFO d_info;
	TIMEINFO t_info;

	if( NULL==setdate || btstrlen(setdate)!=14) return(-1);
	
	btGetDateTime (&d_info, &t_info);
	btsprintf(buf,"%04d%02d%02d%02d%02d",d_info.year,d_info.month,d_info.day,t_info.hour,t_info.min);

	// If data is same, don't set time.
	if( !btmemcmp(setdate,buf,12) ) return 0;	//分まで同じならセットしない

	//年月日のｾｯﾄ
	btmemcpy(buf,setdate,4);buf[4]=0;
	d_info.year = (short)Char2Num(buf);
	btmemcpy(buf,setdate+4,2);buf[2]=0;
	d_info.month = (char)Char2Num(buf);
	btmemcpy(buf,setdate+6,2);buf[2]=0;
	d_info.day = (char)Char2Num(buf);
	//時分秒のｾｯﾄ
	btmemcpy(buf,setdate+8,2);buf[2]=0;
	t_info.hour = (char)Char2Num(buf);
	btmemcpy(buf,setdate+10,2);buf[2]=0;
	t_info.min= (char)Char2Num(buf);
	btmemcpy(buf,setdate+12,2);buf[2]=0;
	t_info.sec = (char)Char2Num(buf);

	btSetDateTime ( &d_info,&t_info );
	return 0;
}

//-----------------------------------------------------------------------------
//	関数名：short htoi( char *hexstr , short len )
//	機能　：16進（文字列）を10進(short型)に変換する
//	引数　：変換元データ格納ポインタ
//	返値　：変換後データ
//-----------------------------------------------------------------------------
//  Function name:short htoi( char *hexstr , short len )
//  Outline      :Convert hex to decimal arithmetic.
//  Arg1         :hex character.
//  Arg2         :length of hex character.
//  Retrun value :decimal arithmetic value.
//-----------------------------------------------------------------------------
short	htoi( char *hexstr , short len )
{
	short ret = 0;
	short i;

	if( len > 4 ) return 0;
	for( i = 0 ; i < len ; i++ ){
		ret *= 16;
		if( hexstr[i] < 'A' ) ret += hexstr[i] - '0';
		else
		if( hexstr[i] < 'a' ) ret += hexstr[i] - 'A'+10;
		else
			ret += hexstr[i] - 'a'+10;
	}
	return(ret);
}

//---------------------------------------------------------------------------------------
//	void lTrim( char* buf,int len)
//	機能　：右側のｽﾍﾟｰｽをはずす
//	カシオから移植
//---------------------------------------------------------------------------------------
//  Function name:void lTrim( char* buf,int len)
//  Outline      :Remove the space from right side of character.
//---------------------------------------------------------------------------------------
void lTrim( char* buf,int len)
{
	int i;

	if( len < 1 ) return;

	for(i=len; i> 0; i--){
		if( buf[i-1]!=0x20) break;
		if( buf[i-1]==0x20) buf[i-1]=0;
	}
}
//---------------------------------------------------------------------------------------
//	void Trim( char* buf,int len)
//	機能　：スペースを全てを外す
//---------------------------------------------------------------------------------------
//  Function name:void Trim( char* buf,int len)
//  Outline      :Remove all space character from argument1 data pointer.
//---------------------------------------------------------------------------------------
void Trim(char *data,int len)
{
	char buf[100];
	int i,j;
	j=0;
	if(len>100) return;
	btmemset(buf,0,sizeof(buf));
	for(i=0;i<len;i++){
		if(data[i]!=' ') buf[j++]=data[i];
	}
	btstrcpy(data,buf);
}

//---------------------------------------------------------------------------------------
//	int DataInputBt( DINPUT_SETTING* set )
//	機能　：データを入力する
//	引数１：入力設定構造体
//	
//	戻り値：0=OK;SC(R)=-1,FN+CLR=-2,空文字列=-101
//	CASIO DT-950の互換関数(完全互換ではないので注意)
//---------------------------------------------------------------------------------------
//  Function name:int DataInputBt( DINPUT_SETTING* set )
//  Outline      :Input data( key input or scanner input )
//  Arg1         :The structure of input setting.(DINPUT_SETTING *set)
//  Return value :0=OK;(R)=-1,F1=-2, Data is blank but push enter key=-101
//---------------------------------------------------------------------------------------
int DataInputBt( DINPUT_SETTING *set )
{
	short keycode;
	//char dat[256+1];
	char dat[2048+1];	//2048文字まで読みとれるように拡張	//2013.06.12 sanno
	short bk;
	//unsigned int len=256;
	unsigned int len=2048;	//2048文字まで読みとれるように拡張	//2013.06.12 sanno

	DSPATTR dspattr;
	INPUTSTRATTR strattr;
	BCRCONF bconf;

	LcdClear();
	set->bar_read=FALSE;

	// 画面表示
	LineOut( set->title,0,1);	//Reversal display for title.タイトルのみ反転
	LineOut2( set->l1,1,0);
	ClineOut( set->l2,3,set->c2);
	ClineOut( set->l3,4,set->c3);
	ClineOut( set->l4,5,set->c4);
	ClineOut( set->l5,6,set->c5);
	ClineOut( set->l6,7,set->c6);
	if(btstrlen(set->l7)){
		ClineOut( set->l7,9,set->c7);
	}
	// 表示属性設定
	btmemset( &dspattr, 0, sizeof(DSPATTR));
	dspattr.mag = BT_MAG_NORM;
	dspattr.fold = BT_FOLD_ON;
	dspattr.tposition.x=1;
	dspattr.tposition.y=17;

	dspattr.attribute=BT_ATRB_NORM;

	// Input setting. 入力設定
	btmemset( &strattr, 0, sizeof(INPUTSTRATTR));
	strattr.inputarea=20;
	if( set->emptyOK==TRUE )strattr.mindigit=0;	//Allow empty input.カラ入力許可の場合
	else strattr.mindigit=1;
	strattr.maxdigit=len;

	//Check mode.モード確認
	if( set->mode < 0 || set->mode > 2 ) set->mode=2;

	//This mode is scanner input and key input. バーコード入力 & キー入力
	if( set->mode == 1 || set->mode == 2){
		// バーコード読み取りオプションの変更
		btGetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_MOMENTARY;
		btSetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_AUTO_OFF;			// 戻し用
		if( set->mode == 1 ){
			strattr.inputtype=BT_INPUT_BCR|BT_INPUT_ESCAPE_ON;	// バーコード入力
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "  [バーコード入力]  ",10,0);
			else LineOut( "  [Scanner Input]  ",10,0);
		}
		if( set->alpha !=1 && set->mode == 2 ){
			//BT_INPUT_ESCAPE_ON 途中入力の中断あり
			strattr.inputtype=BT_INPUT_KEYBCR|BT_INPUT_ESCAPE_ON;// バーコード入力 & キー入力
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "[バーコード＆手入力]",10,0);
			else LineOut( " [ Scanner & Key ]",10,0);
		}
		// Allow input alphabet .アルファベットOK
		if( set->alpha==1 && set->mode==2 ) {
			strattr.inputtype |= BT_INPUT_KEYBCR_SFT_OFF|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON 途中入力の中断あり シフト機能あり
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "[バーコード＆手入力]",10,0);
			else LineOut( " [ Scanner & Key ]",10,0);
		}

		bk=1;
		while(bk){
			// Input function.入力
			btmemset( dat, 0, sizeof(dat));
			keycode = btInputString(dat,&dspattr,&strattr);

			switch(keycode){
				// バーコードが読み込まれた場合、LED点灯し、ブザーを鳴らして数値入力画面を表示
				case KEYCODE_TRG:
					btLed(BT_ON_ASYNC,3,0,1,BT_LED_GREEN);
					btBuzzer(BT_ON_ASYNC,1,0,1,13);
					// Copy data. バッファにコピー
					btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize : len);
					btSetBcrCommonOption(&bconf);
					while(btKeySense(1));		// keyが話されるまで待つ
					// Turn on scanner flag. バーコード読み取りフラグON
					set->bar_read=TRUE;
					return 0;

				case KEYCODE_ENT:
					// Copy data.バッファにコピー
					if( !btstrlen(dat) )return -101;	//Allow input empty.カラ文字列が許容されている場合
					btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize:len );
					return 0;

				case KEYCODE_R:
					if( g_Lang == BT_LANG_JAPANESE ){
						return(-1);
					}
					else{		//In English mode,L key is cancel R key is OK
						return(0);
					}
				case KEYCODE_F1:return(-2);
				case KEYCODE_F2:return(202);
				case KEYCODE_F3:return(303);

				case KEYCODE_L:	
					if( g_Lang == BT_LANG_JAPANESE ){
						// 2011.04.12 coba Enable L key when only scanner input,バーコードonlyモードの時、Lを有効にする
						if(set->mode==1){
							return(-101);
						}
						// 2012.04.18 okamura Enable L key when scanner and key input.t賀仕様にてﾊﾞｰｺｰﾄﾞ及び手入力でも、Lを有効にする
						if(set->mode==2 ) return(-101);
					}
					else{		//In English mode,L key is cancel R key is OK
						// 2011.04.12 coba Enable L key when only scanner input,バーコードonlyモードの時、Lを有効にする
						if(set->mode==1)return(-1);
						// 2012.04.18 okamura Enable L key when scanner and key input.t賀仕様にてﾊﾞｰｺｰﾄﾞ及び手入力でも、Lを有効にする
						if(set->mode==2 ) return(-1);
					}
					break;


				default:
					break;
			}
		}
	}

	// key Input mode.入力
	// 2行目に表示
	dspattr.tposition.x=1;
	dspattr.tposition.y=17;

	// アルファベットOK
	if( set->alpha==1  )
		strattr.inputtype=BT_INPUT_KEY_SFT_ON|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON 途中入力の中断あり シフト機能あり
	// 数字のみ(従来通り)
	else	strattr.inputtype=BT_INPUT_KEY|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON 途中入力の中断ありシフト機能なし

	if(btstrlen(set->defdata))strattr.inputtype = strattr.inputtype | BT_INPUT_INIT_ERASE_OFF;	//2012.12.06 sanno

	if( set->emptyOK==TRUE )strattr.mindigit=0;	//カラ入力許可の場合
	else strattr.mindigit=1;

	if( g_Lang == BT_LANG_JAPANESE )
		LineOut( "   [テンキー入力]   ",10,0);
	else LineOut( "   [ Key input ]   ",10,0);
	while(1){
		// 入力
		//初期値が設定されている場合は初期値を表示	//2012.12.06 sanno
		btmemset( dat, 0, sizeof(dat));

		if(btstrlen(set->defdata))		//2012.12.06 sanno
			btstrcpy(dat,set->defdata);
		
		keycode = btInputString(dat,&dspattr,&strattr);

		// 入力完了
		if( keycode == KEYCODE_ENT) break;
		if( keycode == KEYCODE_L){
			if( g_Lang == BT_LANG_JAPANESE ){
				return(-101);
			}
			else{		//In English mode,L key is cancel R key is OK
				return -1;
			}
		}
		else if( keycode == KEYCODE_R ){
			if( g_Lang == BT_LANG_JAPANESE )
				return -1;		//cancel キャンセル
			else		//In English mode,L key is cancel R key is OK
				return 0;
		}
		else if( keycode == KEYCODE_F1 ) return -2;		//cancel キャンセル
		else if( keycode == KEYCODE_F2 ) return 202;
	}
	if( !btstrlen(dat) )return -101;	//Allow input empty.カラ文字列が許容されている場合

	// バッファにコピー
	btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize : len );

	// カーソルOFF
	btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);
	LineOut("",1,0);
	return 0;

}
//---------------------------------------------------------------------------------------
//	int DataInputBt2( DINPUT_SETTING* set )
//	機能　：データを入力する
//	引数１：入力設定構造体
//	(2012.04.24 okamura 作成　精密O/Rより文字の大きさ等修正があり対応)
//	戻り値：0=OK;SC(R)=-1,FN+CLR=-2,空文字列=-101
//	CASIO DT-950の互換関数(完全互換ではないので注意)
//---------------------------------------------------------------------------------------
int DataInputBt2( DINPUT_SETTING *set )
{
	short keycode;
	char dat[256+1];
	short bk;
	unsigned int len=256;

	DSPATTR dspattr;
	INPUTSTRATTR strattr;
	BCRCONF bconf;

	LcdClear();
	set->bar_read=FALSE;

	LineOut( set->title,0,1);	//タイトルのみ反転
	LineOut2( set->l1,1,0);
	LineOut( set->l2,3,0);
	btSetColor( RED ,WHITE, BLACK,BLACK);	// 画面表示
	LineOut2( set->l4,4,0);
	btSetColor( BLACK,WHITE, BLACK,BLACK);

	LineOut( set->l5,6,0);
	LineOut( set->l6,7,0);
	if(btstrlen(set->l7)){
		LineOut( set->l7,9,0);
	}
	// 表示属性設定
	btmemset( &dspattr, 0, sizeof(DSPATTR));
	dspattr.mag = BT_MAG_NORM;
	dspattr.fold = BT_FOLD_ON;
	dspattr.tposition.x=1;
	dspattr.tposition.y=17;

	dspattr.attribute=BT_ATRB_NORM;

	// アルファベットOK
/*	if( set->alpha==1 && set->mode==2 ) {
		strattr.inputtype |= BT_INPUT_KEY_SFT_ON|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON 途中入力の中断あり シフト機能あり
		LineOut( "[バーコード＆手入力]",10,0);
	}
*/	// 入力設定
	btmemset( &strattr, 0, sizeof(INPUTSTRATTR));
	strattr.inputarea=20;
	if( set->emptyOK==TRUE )strattr.mindigit=0;	//カラ入力許可の場合
	else strattr.mindigit=1;
	strattr.maxdigit=len;

	//モード確認
	if( set->mode < 0 || set->mode > 2 ) set->mode=2;

	// バーコード入力 & キー入力
	if( set->mode == 1 || set->mode == 2){
		// バーコード読み取りオプションの変更
		btGetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_MOMENTARY;
		btSetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_AUTO_OFF;			// 戻し用
		if( set->mode == 1 ){
			strattr.inputtype=BT_INPUT_BCR|BT_INPUT_ESCAPE_ON;	// バーコード入力
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "  [バーコード入力]  ",10,0);
			else LineOut( "  [Scanner Input]  ",10,0);
		}
		if( set->alpha !=1 && set->mode == 2 ){
			//BT_INPUT_ESCAPE_ON 途中入力の中断あり
			strattr.inputtype=BT_INPUT_KEYBCR|BT_INPUT_ESCAPE_ON;// バーコード入力 & キー入力
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "[バーコード＆手入力]",10,0);
			else LineOut( " [ Scanner & Key ]",10,0);
		}
		// Allow input alphabet .アルファベットOK
		if( set->alpha==1 && set->mode==2 ) {
			strattr.inputtype |= BT_INPUT_KEYBCR_SFT_OFF|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON 途中入力の中断あり シフト機能あり
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "[バーコード＆手入力]",10,0);
			else LineOut( " [ Scanner & Key ]",10,0);
		}

		bk=1;
		while(bk){
			// 入力
			btmemset( dat, 0, sizeof(dat));
			keycode = btInputString(dat,&dspattr,&strattr);
			switch(keycode){
				// バーコードが読み込まれた場合、LED点灯し、ブザーを鳴らして数値入力画面を表示
				case KEYCODE_TRG:
					btLed(BT_ON_ASYNC,3,0,1,BT_LED_GREEN);
					btBuzzer(BT_ON_ASYNC,1,0,1,13);
					// バッファにコピー
					btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize : len);
					btSetBcrCommonOption(&bconf);
					while(btKeySense(1));		// keyが話されるまで待つ
					//バーコード読み取りフラグON
					set->bar_read=TRUE;
					return 0;

				case KEYCODE_ENT:
					// バッファにコピー
					if( !btstrlen(dat) )return -101;	//カラ文字列が許容されている場合
					btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize:len );
					return 0;				
/*
				case KEYCODE_UP:
					if( set->upkey == TRUE )return 100;
					else continue;

				case KEYCODE_DOWN:
					if( set->downkey == TRUE )return 101;
					else continue;
*/
				case KEYCODE_R:	return(-1);
				case KEYCODE_F1:return(-2);
				case KEYCODE_F2:return(202);
				case KEYCODE_F3:return(303);

				case KEYCODE_L:	
				// 2011.04.12 coba バーコードonlyモードの時、Lを有効にする
					if(set->mode==1)return(-101);
				// 2012.04.18 okamura 佐賀仕様にてﾊﾞｰｺｰﾄﾞ及び手入力でも、Lを有効にする
					if(set->mode==2 ) return(-101);

				default:
					break;
			}
		}
	}

	// key入力
	// 2行目に表示
	dspattr.tposition.x=1;
	dspattr.tposition.y=17;

	// アルファベットOK
	if( set->alpha==1  ) 
		strattr.inputtype=BT_INPUT_KEY_SFT_ON|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON 途中入力の中断あり シフト機能あり
	// 数字のみ(従来通り)
	else	strattr.inputtype=BT_INPUT_KEY|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON 途中入力の中断ありシフト機能なし
//	strattr.inputtype=BT_INPUT_KEY_SFT_ON|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON 途中入力の中断あり シフト機能あり
	if( set->emptyOK==TRUE )strattr.mindigit=0;	//カラ入力許可の場合
	else strattr.mindigit=1;

	if( g_Lang == BT_LANG_JAPANESE )
		LineOut( "   [テンキー入力]   ",10,0);
	else LineOut( "   [ Key input ]   ",10,0);

	while(1){
		// 入力
		btmemset( dat, 0, sizeof(dat));
		keycode = btInputString(dat,&dspattr,&strattr);
		// 入力完了
		if( keycode == KEYCODE_ENT) break;
		else if( keycode == KEYCODE_R ) return -1;		//キャンセル
		else if( keycode == KEYCODE_F1 ) return -2;		//キャンセル
		else if( keycode == KEYCODE_F2 ) return 202;		//キャンセル
	}
	if( !btstrlen(dat) )return -101;	//カラ文字列が許容されている場合

	// バッファにコピー
	btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize : len );

	// カーソルOFF
	btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);
	LineOut("",1,0);
	return 0;

}

//---------------------------------------------------------------------------------------
//	int DataInputBt( DINPUT_SETTING* set )
//	機能　：データを入力する
//	引数１：入力設定構造体
//	引数２：文字表示モード　何行目までを大きい文字にするか指定(3行目までしかできない)
//	戻り値：0=OK;SC(R)=-1,FN+CLR=-2,空文字列=-101
//	CASIO DT-950の互換関数(完全互換ではないので注意)
//---------------------------------------------------------------------------------------
int DataInputBt3( DINPUT_SETTING *set, short col_count )
{
	short keycode;
	//char dat[256+1];
	char dat[2048+1];	//2048文字まで読みとれるように拡張	//2013.06.12 sanno
	short bk;
	//unsigned int len=256;
	unsigned int len=2048;	//2048文字まで読みとれるように拡張	//2013.06.12 sanno

	DSPATTR dspattr;
	INPUTSTRATTR strattr;
	BCRCONF bconf;

	LcdClear();
	set->bar_read=FALSE;

	// 画面表示
	if(col_count == 2){
		LineOut( set->title,0,1);	//Reversal display for title.タイトルのみ反転
		LineOut2( set->l1,1,0);
		LineOut2( set->l2,3,0);
		LineOut( set->l4,5,0);
		LineOut( set->l5,6,0);
		LineOut( set->l6,7,0);
		if(btstrlen(set->l7)){
			LineOut( set->l7,9,0);
		}
	}
	else if(col_count == 3){
		LineOut( set->title,0,1);	//Reversal display for title.タイトルのみ反転
		LineOut2( set->l1,1,0);
		LineOut2( set->l2,3,0);
		LineOut2( set->l4,5,0);
		LineOut( set->l6,7,0);
		LineOut( set->l7,8,0);
		if(btstrlen(set->l7)){
			LineOut( set->l7,9,0);
		}
	}
	else {
		LineOut( set->title,0,1);	//Reversal display for title.タイトルのみ反転
		LineOut2( set->l1,1,0);
		LineOut( set->l2,3,0);
		LineOut( set->l3,4,0);
		LineOut( set->l4,5,0);
		LineOut( set->l5,6,0);
		LineOut( set->l6,7,0);
		if(btstrlen(set->l7)){
			LineOut( set->l7,9,0);
		}
	}

	// 表示属性設定
	btmemset( &dspattr, 0, sizeof(DSPATTR));
	dspattr.mag = BT_MAG_NORM;
	dspattr.fold = BT_FOLD_ON;
	dspattr.tposition.x=1;
	dspattr.tposition.y=17;

	dspattr.attribute=BT_ATRB_NORM;

	// Input setting. 入力設定
	btmemset( &strattr, 0, sizeof(INPUTSTRATTR));
	strattr.inputarea=20;
	if( set->emptyOK==TRUE )strattr.mindigit=0;	//Allow empty input.カラ入力許可の場合
	else strattr.mindigit=1;
	strattr.maxdigit=len;

	//Check mode.モード確認
	if( set->mode < 0 || set->mode > 2 ) set->mode=2;

	//This mode is scanner input and key input. バーコード入力 & キー入力
	if( set->mode == 1 || set->mode == 2){
		// バーコード読み取りオプションの変更
		btGetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_MOMENTARY;
		btSetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_AUTO_OFF;			// 戻し用
		if( set->mode == 1 ){
			strattr.inputtype=BT_INPUT_BCR|BT_INPUT_ESCAPE_ON;	// バーコード入力
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "  [バーコード入力]  ",10,0);
			else LineOut( "  [Scanner Input]  ",10,0);
		}
		if( set->alpha !=1 && set->mode == 2 ){
			//BT_INPUT_ESCAPE_ON 途中入力の中断あり
			strattr.inputtype=BT_INPUT_KEYBCR|BT_INPUT_ESCAPE_ON;// バーコード入力 & キー入力
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "[バーコード＆手入力]",10,0);
			else LineOut( " [ Scanner & Key ]",10,0);
		}
		// Allow input alphabet .アルファベットOK
		if( set->alpha==1 && set->mode==2 ) {
			strattr.inputtype |= BT_INPUT_KEYBCR_SFT_OFF|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON 途中入力の中断あり シフト機能あり
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "[バーコード＆手入力]",10,0);
			else LineOut( " [ Scanner & Key ]",10,0);
		}

		bk=1;
		while(bk){
			// Input function.入力
			btmemset( dat, 0, sizeof(dat));
			keycode = btInputString(dat,&dspattr,&strattr);
			switch(keycode){
				// バーコードが読み込まれた場合、LED点灯し、ブザーを鳴らして数値入力画面を表示
				case KEYCODE_TRG:
					btLed(BT_ON_ASYNC,3,0,1,BT_LED_GREEN);
					btBuzzer(BT_ON_ASYNC,1,0,1,13);
					// Copy data. バッファにコピー
					btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize : len);
					btSetBcrCommonOption(&bconf);
					while(btKeySense(1));		// keyが話されるまで待つ
					// Turn on scanner flag. バーコード読み取りフラグON
					set->bar_read=TRUE;
					return 0;

				case KEYCODE_ENT:
					// Copy data.バッファにコピー
					if( !btstrlen(dat) )return -101;	//Allow input empty.カラ文字列が許容されている場合
					btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize:len );
					return 0;				
				case KEYCODE_R:
					if( g_Lang == BT_LANG_JAPANESE ){
						return(-1);
					}
					else{		//In English mode,L key is cancel R key is OK
						return(0);
					}
				case KEYCODE_F1:return(-2);
				case KEYCODE_F2:return(202);
				case KEYCODE_F3:return(303);

				case KEYCODE_L:	
					if( g_Lang == BT_LANG_JAPANESE ){
						// 2011.04.12 coba Enable L key when only scanner input,バーコードonlyモードの時、Lを有効にする
						if(set->mode==1)return(-101);
						// 2012.04.18 okamura Enable L key when scanner and key input.t賀仕様にてﾊﾞｰｺｰﾄﾞ及び手入力でも、Lを有効にする
						if(set->mode==2 ) return(-101);
					}
					else{		//In English mode,L key is cancel R key is OK
						// 2011.04.12 coba Enable L key when only scanner input,バーコードonlyモードの時、Lを有効にする
						if(set->mode==1)return(-1);
						// 2012.04.18 okamura Enable L key when scanner and key input.t賀仕様にてﾊﾞｰｺｰﾄﾞ及び手入力でも、Lを有効にする
						if(set->mode==2 ) return(-1);
					}
					break;

				default:
					break;
			}
		}
	}

	// key Input mode.入力
	// 2行目に表示
	dspattr.tposition.x=1;
	dspattr.tposition.y=17;

	// アルファベットOK
	if( set->alpha==1  )
		strattr.inputtype=BT_INPUT_KEY_SFT_ON|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON 途中入力の中断あり シフト機能あり
	// 数字のみ(従来通り)
	else	strattr.inputtype=BT_INPUT_KEY|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON 途中入力の中断ありシフト機能なし

	if(btstrlen(set->defdata))strattr.inputtype = strattr.inputtype | BT_INPUT_INIT_ERASE_OFF;	//2012.12.06 sanno

	if( set->emptyOK==TRUE )strattr.mindigit=0;	//カラ入力許可の場合
	else strattr.mindigit=1;

	if( g_Lang == BT_LANG_JAPANESE )
		LineOut( "   [テンキー入力]   ",10,0);
	else LineOut( "   [ Key input ]   ",10,0);
	while(1){
		// 入力
		//初期値が設定されている場合は初期値を表示	//2012.12.06 sanno
		btmemset( dat, 0, sizeof(dat));

		if(btstrlen(set->defdata))		//2012.12.06 sanno
			btstrcpy(dat,set->defdata);
		
		keycode = btInputString(dat,&dspattr,&strattr);

		// 入力完了
		if( keycode == KEYCODE_ENT) break;
		if( keycode == KEYCODE_L){
			if( g_Lang == BT_LANG_JAPANESE ){
				return(-101);
			}
			else{		//In English mode,L key is cancel R key is OK
				return -1;
			}
		}
		else if( keycode == KEYCODE_R ){
			if( g_Lang == BT_LANG_JAPANESE )
				return -1;		//cancel キャンセル
			else		//In English mode,L key is cancel R key is OK
				return 0;
		}
		else if( keycode == KEYCODE_F1 ) return -2;		//cancel キャンセル
		else if( keycode == KEYCODE_F2 ) return 202;
	}
	if( !btstrlen(dat) )return -101;	//Allow input empty.カラ文字列が許容されている場合

	// バッファにコピー
	btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize : len );

	// カーソルOFF
	btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);
	LineOut("",1,0);
	return 0;

}

//-----------------------------------------------------------------------------
//	Function name:long atol_bt( const char* data )
//  Outline      :instead of atoi or atol
//	Arg1         :Original character.
//	Return value :long value.
//-----------------------------------------------------------------------------
long  atoi_bt( const char* data )
{
	return ( Char2Num((char*)data) );
}
//-----------------------------------------------------------------------------
//	関数名：long Char2Num( char *pString )
//	機能　：文字列を整数に変換する atoiの変わり
//	引数１：変換元文字列
//	返値　：変換後の数値
//-----------------------------------------------------------------------------
long Char2Num( char *data )
{
	long val;
	btchar2num( data,&val );
	return val;
}

//-----------------------------------------------------------------------------
//	Function name:double atof_bt( const char* data )
//  Outline      :instead of  atof
//	Arg1         :Original character.
//	Return value :double value.
//-----------------------------------------------------------------------------
double atof_bt( const char* data )
{
	DECIMAL deci;
	double val;

	char buf[100];
	btstrcpy(buf,data);
	Trim(buf,btstrlen(buf));
	deci=0;
	Char2Dec( buf ,&deci );
	val = (double)deci / 1000;
	return val;
}
//-----------------------------------------------------------------------------
//	関数名：short Char2Dec( char *pString , DECIMAL* pDecimal )
//	機能　：文字列を浮動小数点数に変換する atofの代わり
//	atoiについてはKeyence標準関数のbtchar2numを使って下さい
//	引数１：変換元文字列
//	引数２：出力用データポインタ(DECIMAL型）
//	返値　：0：成功、それ以外：失敗
//	注意！！ DECIMAL型は「DECIMALは小数値を1000倍した値で扱います。」との事なので注意！！
//-----------------------------------------------------------------------------
short Char2Dec( char *pString , DECIMAL* pDecimal )
{
	char buf[50];
	short inlen;			// 入力文字列長
	short stpos = 0;		// 変換開始位置(マイナスが付く場合は1になる)
	short seppos = 0;		// 小数部位置
	long val1 = 0;
	long val2 = 0;
	double dval = 0;
	short i;
	short mag = 1;
	char maxnumstr[] = {"2147483647"};

	inlen = (short)btstrlen(pString);
	if ( inlen > 12 ) return -1;

	btstrcpy(buf, pString);
	btstrcat(buf, "000");

	for ( i = 0 ; i < inlen ; i++ ) {
		if ( buf[i] < '0' || '9' < buf[i] ) {
			if ( buf[i] == '.' ) {
				if ( seppos != 0 ) return -1;
				seppos = i + 1; buf[i] = 0x00;
			} else if ( i == 0 && (buf[i] == '-' || buf[i] == ' ') ) {	// 符号桁
			} else return -1;
		}
	}
	if ( seppos != 0 && inlen > seppos ) buf[seppos+3] = 0x00;
	else if ( seppos == 0 ) buf[inlen] = 0x00;

	if ( (buf[0] == '-' || buf[0] == ' ') ) stpos = 1;

	inlen = (short)btstrlen(&buf[stpos]);
	if ( seppos == 0 && inlen == 10 ) {
		for ( i = 0 ; i < inlen ; i++ ) {
			if ( buf[stpos+i] > maxnumstr[i] ) return -1;
		}
	}


	if ( seppos == 0 ) btchar2num(&buf[stpos], &val1);
	else if ( seppos == 1 ) btchar2num(&buf[seppos], &val2);
	else {
		btchar2num(&buf[stpos], &val1);
		btchar2num(&buf[seppos], &val2);
	}

	if ( buf[0] == '-' ) mag = -1;
	dval = (((double)val1*1000 + val2) * mag);
	if ( dval < -2147483648.000 || 2147483647.000 < dval ) return -1;

	*pDecimal = (DECIMAL)dval;
	return 0;
}

//-----------------------------------------------------------------------------
//	関数名：int MasterFind( char *keyname,char *data )
//	機能　：引数で指定されたデータをマスタファイルから検索する
//	引数１：検索するキー名称
//	引数２：出力用データポインタ
//	返値　：0:データあり、それ以外データなし
//-----------------------------------------------------------------------------
//	Function name: int MasterFind( char *keyname,char *data )
//	Outline      : Select master file by directed key name as argument1.
//	Arg1         : key name of select
//	Arg2         : found data pointer.
//-----------------------------------------------------------------------------
int MasterFind( char *keyname,char *data )
{
	MASTER_INDEX index;
	MASTER_SEARCH find;
	short mstno=0,reclen=0;
	long recno=0;
	char buf[256];

	if( !btstrlen(keyname) ) return -100;		//キー指定なしはダメ

	btmemset(&index,0,sizeof(index));
	index.datatype = 1;	//0:Fix record length.固定長、1:not fix record length可変長
	index.separator=',';//The sepalater.セパレータ
	index.fieldcnt=2;	//Field qty.( key field, data field)フィールド数は2個（キー部、データ部）

	//File open.ファイルオープン
	mstno = btMasterOpen( MTRFILE_NAME,MTRIDXFILE_NAME, NULL, &index);
	if(mstno<0) return -1;	//ファイルが開けない==異常

	//検索条件ｾｯﾄ
	btmemset(&find,0,sizeof(find));
	find.ope=1;	// 演算子(1:AND、2:OR）
	find.searchdata[0]=keyname;			// 第１フィールドに検索条件代入
	find.incdeldata = BT_OFF; // 削除データも含めて検索を行うか(BT_ON:含む, BT_OFF:含まない)
	
	recno = btMasterGetFirst(mstno, &find);
	if( recno < 0 ){	//Data does not exists.データなし
		btMasterClose(mstno);
		return -2;
	}

	btmemset(buf,0,sizeof(buf));
	reclen = btMasterRead(mstno,recno,BT_OFF,buf);
	if( reclen < 0 ){
		btMasterClose(mstno);
		return -3;	//読み出しエラー
	}
	btmemcpy(data,buf+btstrlen(keyname)+1,btstrlen(buf)-(btstrlen(keyname)+1));

	//close
	btMasterClose(mstno);
	return 0;
}

//-----------------------------------------------------------------------------
//	関数名：int MasterUpd( char *keyname,char *data )
//	機能　：引数で指定されたデータをマスタファイルに追加・更新する
//	引数１：検索するキー名称
//	引数２：出力用データポインタ
//	返値　：0:データあり、それ以外データなし
//-----------------------------------------------------------------------------
//	Function name: int MasterUpd( char *keyname,char *data )
//	Outline      : Insert or Update to master file by key name of argument1.
//	Arg1         : key name of insert or update.
//	Arg2         : data
//-----------------------------------------------------------------------------
int MasterUpd( char *keyname,char *data )
{

	MASTER_INDEX index;
	MASTER_SEARCH find;
	short mstno=0,reclen=0;
	long recno=0;
	short upd=0,ret;
	char buf[256];

	btmemset(&index,0,sizeof(index));
	index.datatype = 1;	//0:固定長、1:可変長
	index.separator=',';//セパレータ
	index.fieldcnt=2;	//フィールド数は2個（キー部、データ部）

	//Open Actual file.本番ファイルオープン
	mstno = btMasterOpen( MTRFILE_NAME,MTRIDXFILE_NAME, NULL, &index);
	if(mstno<0) return -1;	//ファイルが開けない==異常
	//Close Actual file.本番ファイル一旦クローズ
	btMasterClose(mstno);

	//Delete temporary files.編集用ファイル群を一旦削除
	btRemove(MODMTRFILE_NAME);
	btRemove(MODMTRIDXFILE_NAME);
	btRemove(MODMTRADDFILE_NAME);
	btRemove(MODMTRSYSFILE_NAME);

	//Data copy to temporary file from actual file.本番用ファイル→編集用ファイルへコピー
	if( btCopy(MTRFILE_NAME, MODMTRFILE_NAME, BT_ON) ) return -1;//	コピー失敗=異常

	//Open templrary file.編集用ファイルオープン
	mstno = btMasterOpen( MODMTRFILE_NAME,MODMTRIDXFILE_NAME, MODMTRADDFILE_NAME, &index);
	if(mstno<0) return -1;	//ファイルが開けない==異常

	//Delete actual files.本番用のファイル群を削除
	btRemove(MTRFILE_NAME);
	btRemove(MTRIDXFILE_NAME);
	btRemove(MTRSYSFILE_NAME);


	//検索条件ｾｯﾄ
	btmemset(&find,0,sizeof(find));
	find.ope=1;	// 演算子(1:AND、2:OR）
	find.searchdata[0]=keyname;			// 第１フィールドに検索条件代入
	find.incdeldata = BT_OFF; // 削除データも含めて検索を行うか(BT_ON:含む, BT_OFF:含まない)

	recno = btMasterGetFirst(mstno, &find);
	if( recno >= 0 )upd = 1;	//データあり=更新モード

	//セパレータをカンマにして、データ代入処理
	btsprintf(buf,"%s,%s",keyname,data);
	if( upd ) ret = btMasterModify(mstno,recno,buf);
	else ret = (short)btMasterAdd(mstno,buf);
	if( ret<0 ){
		btMasterClose(mstno);
		return -1;		//Insert error追加失敗！！
	}
	//Restore master file.マスタ再構築
	btMasterRestore(mstno,MTRFILE_NAME);
	//close
	btMasterClose(mstno);

	//Delete temporary files.編集用ファイル群削除
	btRemove(MODMTRFILE_NAME);
	btRemove(MODMTRIDXFILE_NAME);
	btRemove(MODMTRADDFILE_NAME);
	btRemove(MODMTRSYSFILE_NAME);

	return 0;
}

//=============================================================================
// int WLAN_change_usr_pc(const char* keyname)
// change target pc to user defined
// Arg1:[in] keyname *must be registerd by WLAN_setting_usr() function
// return:0=OK,-1=arg1 is over 32 bytes,-2= keyname is not registerd
//=============================================================================
int WLAN_change_usr_pc(char* keyname)
{
	char addr[30],port[30];
	char pname[64];

	if(btstrlen(keyname)>32) return(-1);

	// get stored information about ip
	btmemset(addr,0,sizeof(addr));
	if( MasterFind(keyname,addr) ) return(-2);

	// create port name using key name
	btsprintf( pname,"%s_port",keyname);

	btmemset(port,0,sizeof(port));
	if( MasterFind(pname,port) ) return(-2);

	WLAN_setserver(addr,Char2Num(port));

	return 0;
}

//=============================================================================
//	int WLAN_init(void);
//	無線LANの初期化
//	１．本体で設定されているホストアドレスを読み込む
//	２．それでもなかったらマスタファイルから読み込む
//=============================================================================
//	int WLAN_init(void);
//  Outline:Initialize of wireless device.
//          Get server adderess from master file.
//=============================================================================
int WLAN_init(void)
{
	char addr[30],port[30];

	btmemset(addr,0,sizeof(addr));
	if( MasterFind("server_addr",addr) ){
		LcdClear();
		if( g_Lang == BT_LANG_JAPANESE ){
			LineOut("IPアドレス未設定です",2,0);
			LineOut("設定して下さい",4,0);
		}
		else{
			LineOut("Set IP address!!",2,0);
		}
		FuncSel(0);
		return -1;
	}

	btmemset(port,0,sizeof(port));
	if( MasterFind("port_no",port) ){
		LcdClear();
		if( g_Lang == BT_LANG_JAPANESE ){
			LineOut("ﾎﾟｰﾄ番号未設定です",2,0);
			LineOut("設定して下さい",4,0);
		}
		else{
			LineOut("Set TCP port No.!!",2,0);
		}
		FuncSel(0);
		return -2;
	}

	WLAN_setserver(addr,Char2Num(port));

	return 0;
}
//=============================================================================
//	int WLAN_setting(void)
//	IPとポートの設定
//	１．本体で設定されているホストアドレスを読み込む
//	２．それでもなかったらマスタファイルから読み込む
//=============================================================================
//	Function name:int WLAN_setting(void)
//  Outline      :Input server address and TCP port No.
//=============================================================================
int WLAN_setting(void)
{
	char buf1[40],buf2[40],buf3[40],buf4[40];
	if( g_Lang == BT_LANG_JAPANESE ){
		btstrcpy(buf1,"server_addr\0");
		btstrcpy(buf2,"ｻｰﾊﾞｰｱﾄﾞﾚｽを入力\0");
		btstrcpy(buf3,"port_no\0");
		btstrcpy(buf4,"ﾎﾟｰﾄ番号を入力\0");
	}
	else{
		btstrcpy(buf1,"server_addr\0");
		btstrcpy(buf2,"Input Server Address\0");
		btstrcpy(buf3,"port_no\0");
		btstrcpy(buf4,"Input TCP Port No.\0");
	}
	return(WLAN_settingEx(buf1,buf2,buf3,buf4));
}
//=============================================================================
// int WLAN_setting_usr(char* keyname)
// set option server to user define
// Arg1:[in] keyname,64 bytes max,for example,name of application
// return:0=OK,-1=arg1 is over 32 bytes
//=============================================================================
int WLAN_setting_usr(char* keyname)
{
	int ret;
	char pname[64+1];

	if(btstrlen(keyname)>32) return(-1);

	// create port name using key name
	btsprintf( pname,"%s_port",keyname);

	// call setting function
	return(WLAN_settingEx(keyname,keyname,pname,keyname));

}

int WLAN_settingEx(char* keyname_ip,char *dspname_ip, char *keyname_port,char *dspname_port)
{
	char addr[30],port[10],dat[50];
	int cnt,ret,i;
	DINPUT_SETTING st;

	//IP設定
	//Read setting from master file.マスターファイルから読み出し
	btmemset(addr,0,sizeof(addr));
	MasterFind(keyname_ip,addr);

	LcdClear();

	btmemset(&st,0,sizeof(st));
	st.mode=0;
	if( g_Lang == BT_LANG_JAPANESE ){
		btstrcpy(st.title,"IPアドレス入力");
		if( btstrlen(addr) )btstrcpy(st.l5,"変更なしの場合はENT");
	}
	else{
		btstrcpy(st.title,"Input IP Address.");
		if( btstrlen(addr) )btstrcpy(st.l5,"Don't change -> ENT");
	}
	btsprintf(st.l1,"%s",dspname_ip);	//引数を表示
	btsprintf(st.l2,"addr=%s",addr);
	st.buffer=dat;
	st.bufsize=sizeof(dat)-1;
	st.emptyOK=TRUE;
	btmemset(dat,0,sizeof(dat));

	while(1){
		ret = DataInputBt( &st );
		if( btstrlen(addr) && ret==-101 ){
			btstrcpy(dat,addr);
		}
		if( !btstrlen(dat) ) continue;
		//チェック
		cnt=0;
		for(i=0;i<btstrlen(dat);i++){
			if( dat[i]=='.' )cnt++;
		}
		if( cnt != 3 ){
			if( g_Lang == BT_LANG_JAPANESE )
				DspMsg("  ＊＊＊エラー＊＊＊",0,"IPアドレスが","不正です(1)",dat,"","",NULL);
			else
				DspMsg(" ***** Error *****",0,"IP address is ","not correct.(1)",dat,"","",NULL);
				
			continue;
		}
		if( btSocketInet_addr(dat)==BTSOCK_INADDR_NONE ){
			if( g_Lang == BT_LANG_JAPANESE )
				DspMsg("  ＊＊＊エラー＊＊＊",0,"IPアドレスが","不正です(2)",dat,"","",NULL);
			else
				DspMsg(" ***** Error *****",0,"IP address is ","not correct.(2)",dat,"","",NULL);
			continue;
		}
		else{
			MasterUpd(keyname_ip,dat);
			break;
		}
	}
	//TCP Port No. setting ポート設定
	btmemset(port,0,sizeof(port));
	MasterFind(keyname_port,port);

	LcdClear();

	btmemset(&st,0,sizeof(st));
	st.mode=0;
	if( g_Lang == BT_LANG_JAPANESE ){
		btstrcpy(st.title,"ポート番号入力");
		if(Char2Num(port)) btstrcpy(st.l5,"変更なしの場合はENT");
	}
	else{
		btstrcpy(st.title,"Input Port No.");
		if(Char2Num(port)) btstrcpy(st.l5,"Don't change -> ENT");
	}
	btsprintf(st.l1,"%s",dspname_port);//引数を表示
	btsprintf(st.l2,"port=%d",Char2Num(port));
	st.buffer=dat;
	st.bufsize=sizeof(dat)-1;
	st.emptyOK=TRUE;
	btmemset(dat,0,sizeof(dat));
	while(1){
		ret = DataInputBt( &st );
		if( ret== -101 && Char2Num(port) ){
			btstrcpy(dat,port);
		}
		if( Char2Num(dat) < 0 || Char2Num(dat) > 65535 ){
			if( g_Lang == BT_LANG_JAPANESE )
				DspMsg("  ＊＊＊エラー＊＊＊",0,"ポート番号が","不正です(1)",dat,"","",NULL);
			else
				DspMsg(" ***** Error *****",0,"TCP Port No. is ","not correct.(1)",dat,"","",NULL);
			continue;
		}
		MasterUpd(keyname_port,dat);
		break;
	}
	return 0;
}//=============================================================================
//	Function name:int WLAN_settingIP(char* keyname_ip,char *dspname_ip)
//  Outline      :Input server address only.
//	Arg1		 :master file keyname of ip-address
//	Arg2		 :title for display
//	2013.10.01 matsuda
//=============================================================================
int WLAN_settingIP(char* keyname_ip,char *dspname_ip)
{
	char addr[30],dat[50];
	int cnt,ret,i;
	DINPUT_SETTING st;

	//IP設定
	//Read setting from master file.マスターファイルから読み出し
	btmemset(addr,0,sizeof(addr));
	MasterFind(keyname_ip,addr);

	LcdClear();

	btmemset(&st,0,sizeof(st));
	st.mode=0;
	if( g_Lang == BT_LANG_JAPANESE ){
		btstrcpy(st.title,"IPアドレス入力");
		if( btstrlen(addr) )btstrcpy(st.l5,"変更なしの場合はENT");
	}
	else{
		btstrcpy(st.title,"Input IP Address.");
		if( btstrlen(addr) )btstrcpy(st.l5,"Don't change -> ENT");
	}
	btsprintf(st.l1,"%s",dspname_ip);	//引数を表示
	btsprintf(st.l2,"addr=%s",addr);
	st.buffer=dat;
	st.bufsize=sizeof(dat)-1;
	st.emptyOK=TRUE;
	btmemset(dat,0,sizeof(dat));

	while(1){
		ret = DataInputBt( &st );
		if( btstrlen(addr) && ret==-101 ){
			btstrcpy(dat,addr);
		}
		if( !btstrlen(dat) ) continue;
		//チェック
		cnt=0;
		for(i=0;i<btstrlen(dat);i++){
			if( dat[i]=='.' )cnt++;
		}
		if( cnt != 3 ){
			if( g_Lang == BT_LANG_JAPANESE )
				DspMsg("  ＊＊＊エラー＊＊＊",0,"IPアドレスが","不正です(1)",dat,"","",NULL);
			else
				DspMsg(" ***** Error *****",0,"IP address is ","not correct.(1)",dat,"","",NULL);
				
			continue;
		}
		if( btSocketInet_addr(dat)==BTSOCK_INADDR_NONE ){
			if( g_Lang == BT_LANG_JAPANESE )
				DspMsg("  ＊＊＊エラー＊＊＊",0,"IPアドレスが","不正です(2)",dat,"","",NULL);
			else
				DspMsg(" ***** Error *****",0,"IP address is ","not correct.(2)",dat,"","",NULL);
			continue;
		}
		else{
			MasterUpd(keyname_ip,dat);
			break;
		}
	}
	return 0;
}

//=============================================================================
//	int WLAN_setserver(char* srvaddr,int port)
//	グローバル変数のサーバ設定を変更する
//	2007.03.17 引数にNULLor0を指定したときは現行設定を引き継ぐ
//=============================================================================
//	Function name:int WLAN_setserver(char* srvaddr,int port)
//  Outline      :Set server information to public member variables.
//=============================================================================
int WLAN_setserver(char* srvaddr,int port)
{

	if( srvaddr !=NULL){
		if( btSocketInet_addr(srvaddr)==BTSOCK_INADDR_NONE) return(-1);
		btmemset( g_HostAddr,0,sizeof(g_HostAddr));
		btmemcpy( g_HostAddr,srvaddr,btstrlen(srvaddr)> sizeof(g_HostAddr)-1 ? sizeof(g_HostAddr)-1:btstrlen(srvaddr));
	}
	if( port ) g_PortNo=port;
	return(0);
}
//=============================================================================
//	int WLAN_sendBt( char* data,int size,char* rcv,int rsize)
//	Casio互換のため
//=============================================================================
//	Function name:int WLAN_send( char* data,int size,char* rcv,int rsize)
//  Outline      :Send data to PC.(Easy version)
//  Arg1         :Data for sending.
//	Arg2         :Data length of sending
//	Arg3         :Data pointer for receiving data.
//	Arg4         :Buffer size of receiving data pointer.
//=============================================================================
int WLAN_send( char* data,int size,char* rcv,int rsize)
{
	int len=0;
	return(WLAN_sendBt(data,size,rcv,rsize,&len,3,15) );
}
//=============================================================================
//	int WLAN_sendBt( char* data,int size,char* rcv,int rsize,int* rcvlen,int send_to,int recv_to)
//	データを送信し、結果を受信する
//	引数１：送信データ
//	引数２：送信データサイズ
//	引数３：受信バッファ（出力用）
//	引数４：受信バッファサイズ
//	引数５：受信データ長さ（出力用）
//	引数６：
//=============================================================================
//	Function name:int WLAN_sendBt( char* data,int size,char* rcv,int rsize,int* rcvlen,int send_to,int recv_to)
//  Outline      :Send data to PC.( Detail version )
//  Arg1         :Data for sending.
//	Arg2         :Data length of sending
//	Arg3         :Data pointer for receiving data.
//	Arg4         :Buffer size of receiving data pointer.
//	Arg5         :Destination data pointer ( integer ) of received data length.
//	Arg6         :Time out time of sending operation.
//	Arg7         :Time out time of receiving operation.
//=============================================================================
int WLAN_sendBt( char* data,int size,char* rcv,int rsize,int* rcvlen,int send_to,int recv_to)
{
	BTSOCKADDR_IN btsockaddr_in;
	BTSOCKFD_SET writefds;
	BTSOCKTIMEVAL timeval;
	long ret,socketClient;
	//char buffer[BT_MAX_STRLEN] = "";
	char buffer[20480] = "";
	char ErrMsg[50],buf[10];
	int len,len2,wait_cnt,rcv_sock_len;
	int chk,dlen;		// 2013.09.24 coba 受信データ数をチェックしてすべて受信する
	long err;

	if( !btstrlen(g_HostAddr) ){
		LcdClear();
		if( g_Lang == BT_LANG_JAPANESE ){
			LineOut("IPアドレス未設定です",2,0);
			LineOut("設定して下さい",4,0);
		}
		else{
			LineOut("Set IP address!!",2,0);
			LineOut("IP does not exists.",4,0);
		}
		FuncSel(0);
		return -1;
	}
	if( !g_PortNo ){
		LcdClear();
		if( g_Lang == BT_LANG_JAPANESE ){
			LineOut("ポート番号未設定です",2,0);
			LineOut("設定して下さい",4,0);
		}
		else{
			LineOut("Set TCP Port No.!!",2,0);
			LineOut("Port does not exists",4,0);
		}
		FuncSel(0);
		return -1;
	}
	//
	LcdClear();
	if( g_Lang == BT_LANG_JAPANESE ){
		LineOut4("  通信中  ",5,0);
	}
	else LineOut("  Connecting...  ",5,0);

	//The preparation of headre data. データのヘッダー準備
	btsprintf(buffer,"%05ld",size);
	btmemcpy(buffer+5,data,size);

	// RFポートをオープンする(ブロッキングモード：BT_ON_ASYNC)
/*	ret = btNetworkOpen(BT_NETWORK_WLAN_INFRA, BT_ON_SYNC);
	if(ret == BT_RF_API_SET_ERR){
		btsprintf(ErrMsg,"無線接続に失敗しました。(%d)",ret);
		btMessageBox(ErrMsg,BT_MB_CONFONLY);
		return -1;
	}
*/
	// Open socket. ソケットをオープンする
	socketClient = btSocketOpen( BTSOCK_AF_INET, BTSOCK_STREAM, 0 );
	if( socketClient <= 0 ){
#ifdef WLAN_DEBUG
		btsprintf(ErrMsg,"ソケットのオープンに失敗しました。(%d)",socketClient);
		btMessageBox(ErrMsg,BT_MB_CONFONLY);
#endif
		return -2;
	}
	btsockaddr_in.sin_family = BTSOCK_AF_INET;
	btsockaddr_in.sin_port = btSocketHtons( (unsigned short)g_PortNo );
	btsockaddr_in.sin_addr.S_un.S_addr = btSocketInet_addr(g_HostAddr);

	// Connect to Socket.ソケットを接続する
	ret = btSocketConnect(socketClient, (BTSOCKADDR*)&btsockaddr_in, sizeof(BTSOCKADDR) );
	if( ret != BTSOCK_SOCKET_OK ){
#ifdef WLAN_DEBUG
		btsprintf(ErrMsg,"ソケットの接続に失敗しました｡",ret);
		btMessageBox(ErrMsg,BT_MB_CONFONLY);
#endif
		// ソケットを閉じる
		btSocketShutdown(socketClient,BTSOCK_SD_BOTH);
		btSocketClose(socketClient);
		// RFポートを閉じる
//		btNetworkClose(BT_NETWORK_WLAN_INFRA);
		return -3;
	}

	// Wait the connection .接続ができるまで待ち合わせる
	btFD_ZERO( &writefds );
	btFD_SET(socketClient, &writefds);
	timeval.tv_sec=send_to;	//Time out time. タイムアウト時間
	timeval.tv_usec=0;
	ret = btSocketSelect( 0, 0, &writefds, 0, &timeval );
	//Time out.タイムアウト
	if( !ret ){
		CloseSocket(socketClient);
//		btNetworkClose(BT_NETWORK_WLAN_INFRA);
#ifdef WLAN_DEBUG
		btMessageBox("サーバからの応答がありません｡",BT_MB_CONFONLY);
#endif
		return -4;
	}
	else if( ret == BTSOCK_SOCKET_ERROR ){
#ifdef WLAN_DEBUG
		btsprintf(ErrMsg,"サーバへの接続に失敗しました｡(%d)",btSocketGetLastError() );
		btMessageBox(ErrMsg,BT_MB_CONFONLY);
#endif
		CloseSocket(socketClient);
//		btNetworkClose(BT_NETWORK_WLAN_INFRA);
		return -5;
	}
	else if( btFD_ISSET(socketClient,&writefds)>0 ){
		btFD_CLR(socketClient,&writefds);
	}
	else{
		CloseSocket(socketClient);
//		btNetworkClose(BT_NETWORK_WLAN_INFRA);
		return -6;
	}

	// Sending operation. 送信
	len = size+5;
	len2 = 0;
	wait_cnt=0;
	do{
		ret = btSocketSendto(socketClient, &buffer[len2],len-len2, 0 ,(BTSOCKADDR*)&btsockaddr_in ,sizeof(BTSOCKADDR));
//		ret = btSocketSend(socketClient, &buffer[len2],len-len2, 0);
		if( ret < 0 ){
			// 処理が完了していない場合があるのでその場合は継続 2013.09.24 coba
			err=btSocketGetLastError();
			if( err == BTSOCK_EWOULDBLOCK){
				ret=0;
			}
			else{
				CloseSocket(socketClient);
				return(-7);
			}
		}
		// The length of alredy sent data. 送信済みバイト数
		len2 += ret;
		if( len !=len2 ){
			if( ++wait_cnt > 10 ){
				CloseSocket(socketClient);
//				btNetworkClose(BT_NETWORK_WLAN_INFRA);
				return -8;										// データ送信失敗
			}
			btWait(1,KEY_CLR);//1=100msec
		}
		else wait_cnt=0;

	}while(len !=len2);
	// Receive operation.
	// 接続ができるまで待ち合わせる
	btFD_ZERO( &writefds );
	btFD_SET(socketClient, &writefds);
    timeval.tv_sec  = recv_to;  // Time out time of receiving.受信タイムアウト
    timeval.tv_usec = 0;
	ret = btSocketSelect( 0, 0, &writefds, 0, &timeval );
	if( !ret ){
		CloseSocket(socketClient);
//		btNetworkClose(BT_NETWORK_WLAN_INFRA);
#ifdef WLAN_DEBUG
		btMessageBox("サーバからの応答がありません｡",BT_MB_CONFONLY);
#endif
		return -9;
	}
	else if( ret == BTSOCK_SOCKET_ERROR ){
#ifdef WLAN_DEBUG
		btsprintf(ErrMsg,"サーバへの接続に失敗しました｡(%d)",btSocketGetLastError() );
		btMessageBox(ErrMsg,BT_MB_CONFONLY);
#endif
		CloseSocket(socketClient);
//		btNetworkClose(BT_NETWORK_WLAN_INFRA);
		return -10;
	}
	else if( btFD_ISSET(socketClient,&writefds) > 0 ){
		btFD_CLR(socketClient,&writefds);
	}
	else{
		CloseSocket(socketClient);
//		btNetworkClose(BT_NETWORK_WLAN_INFRA);
		btMessageBox(ErrMsg,BT_MB_CONFONLY);
		return -11;
	}

	//Receiving...データ受信
	wait_cnt=0;
	len=0;
	rcv_sock_len=sizeof(BTSOCKADDR);
	btmemset(buffer,0,sizeof(buffer));
	chk=0;		// 送信データをチェックし、必要分待つ 2013.09.24
	while(1){
		ret = btSocketRecvfrom(socketClient, &buffer[len], sizeof(buffer)-len, 0, (BTSOCKADDR*)&btsockaddr_in ,&rcv_sock_len );
		//ret = btSocketRecvfrom(socketClient, buffer, sizeof(buffer), 0, (BTSOCKADDR*)&btsockaddr_in ,&rcv_sock_len );
		if( ret < 0 ){
			wait_cnt++;
			btWait(1,KEY_CLR);//1=100msec
		}
		else{
			len += ret;
			wait_cnt=0;
		}
		if( wait_cnt > recv_to * 10 ){	//recv_toは秒なので10をかける
#ifdef WLAN_DEBUG
			btMessageBox("データを受信できません",BT_MB_CONFONLY);
#endif
			btSocketShutdown(socketClient,BTSOCK_SD_BOTH);
			btSocketClose(socketClient);
//			btNetworkClose(BT_NETWORK_WLAN_INFRA);
			return -12;
		}
		//Check received data rength. 受信データ長を調べる 2013.09.24
		if( len >=5 && !chk ){
			btmemcpy( buf,buffer,5),buf[5]=0;
			dlen=atoi_bt(buf);
			chk=1;
		}
		// 指定データ数を受信したら終了
		if( chk && len-5 >=dlen ) break;

	}
	// 出力
	//2013.09.27 matsuda following part is not necessary because checking received data in above process.
//	len -=5;	// 送信バイト長のヘッダを引く
//	if(len< 1 ) return(-13);
	//2008.04.25 ﾏﾂﾀﾞ 追加
//	if( len > rsize-1) len = rsize-1;
	btmemcpy( rcv, buffer+5, dlen );
	buffer[ dlen ]=0;
	*rcvlen=dlen;

	// Close socket.ソケットを閉じる
	btSocketShutdown(socketClient,BTSOCK_SD_BOTH);
	btSocketClose(socketClient);
	// RFポートを閉じる
//	btNetworkClose(BT_NETWORK_WLAN_INFRA);
	return 0;
}
// ******************************************************************************
void CloseSocket( long socketno )
//	説明		:	ソケットを閉じます
//  Outline     :   Close the socket.
// ******************************************************************************
{
	// ソケットを閉じる
	btSocketShutdown(socketno,BTSOCK_SD_BOTH);
	btSocketClose(socketno);
}
//=====================================================================================================
//	Function name : int UpdateProgram(void);
//  Outline		  : Connect to program update server by socket communication
//					and update program and replace program.
//	2013.10.01 matsuda
//=====================================================================================================
int UpdateProgram(void)
{
	char buf[10240+10];
	char sendstr[20],fname[256];//,head[5];
	char dt_name[50],addr[50];
	int ret,i,rcvlen;
	char keycode;
	ENABLE_WAITKEYS key;
	int f_no;

	ret = 0;
	while(!ret){
		//メイン画面表示
		LcdClear();

		keycode=StartDisplay("BT Program Updater","BT Program updater","","","","tcp port=35963","","L:Exec F3:IP setting");
		switch(keycode){
			//2013.10.15 matsuda
			case KEYCODE_F1:
				return -1;
				break;

			//IP設定
			case KEYCODE_F3:
				WLAN_settingIP("prg_updater_addr","Input IP");
				break;

			//実行
			case KEYCODE_L:
			case KEYCODE_ENT:
				ret = 1;
				break;
		}
	}

	btmemset(addr,0,sizeof(addr));
	if( MasterFind("prg_updater_addr",addr) ){
		LcdClear();
		LineOut("Set IP address!!",2,0);
		FuncSel(0);
		return -1;
	}

	WLAN_setserver(addr,35963);

	// はい/いいえ確認
	btmemset( &key,0,sizeof(ENABLE_WAITKEYS));
	key.keyL=TRUE;
	key.keyR=TRUE;

	//①PCから更新用の名前を貰う
	rcvlen=0;
	btmemset(dt_name,0,sizeof(dt_name));
	btsprintf(sendstr,"R0");//ﾌﾟﾛｸﾞﾗﾑ名の要求
	ret=WLAN_sendBt(sendstr,btstrlen(sendstr),dt_name,sizeof(dt_name)-1,&rcvlen,3,3);
	if(ret){
		DspMsg("BT update",3,"Can not connect","to server","","","",&key);
		return(-1);
	}
	// エラー発生
	if( !btmemcmp( dt_name,"NG",2)){
		DspMsg("BT update",3,"PC return error","Stop operation","","","",&key);
		return(-2);
	}
	if( !btstrlen(dt_name) ) return -1;

	//確認
	ret = DspMsg("BT update",3,dt_name,"Update OK?","","","",&key);
	if(ret) return -1;

	// Data copy to drive 1
	btsprintf(fname,"1:%s",dt_name);

	f_no = btFileOpen( fname,"w");
	if( f_no <= 0 ){
		DspMsg("BT update",0,"Can not open","local file.","Can not update","","",NULL);
		return -2;
	}
	i=0;
	// MAX10240byte *1000=10Mbyte
	for(i=0;i<1000;i++){
		btmemset(sendstr,0,sizeof(sendstr));
		btsprintf(sendstr,"Q%03d",i);

		btmemset(buf,0,sizeof(buf));
		rcvlen=0;
		ret=WLAN_sendBt(sendstr,btstrlen(sendstr),buf,sizeof(buf),&rcvlen,3,5);
		if(ret){
			btFileClose(f_no);
			DspMsg("BT update",0,"Transmission failure","","","","",NULL);
			return(-1);
		}
		// NG
		if( !btmemcmp( buf,"NG",2)){
			btFileClose(f_no);
			DspMsg("BT update",0,"Can not open","local file.","Can not update","","",NULL);
			return(-2);
		}
		btFileWrite(f_no,buf+1,rcvlen-1);

		//EndのE
		if( buf[0] == 'E' ) break;
	}
	btFileClose(f_no);

	DspMsg("BT update",0,"Complete!!","",dt_name,"","",NULL);
	//program execute
	btChangeApplication(fname);

	return(0);
}


//========================================================================================================================
//	Notice !!!!
//	Following function is legacy function for compatible.
//	So Don't use following function in new program .
//========================================================================================================================

//-----------------------------------------------------------------------------
//	関数名：void LcdOut( void *line1, void *line2, void *line3)
//	機能　：画面表示（表示only）
//	引数１：表示用の構造体
//		＊表示可能な文字数は、１行あたり20バイトです。また、文字の最後に
//		　必ずNullを付加して下さい。
//-----------------------------------------------------------------------------
void LcdOut( BT900_LCD* lcd)
{
	int i;
	DSPATTR datr;

	btClear();

	// 表示属性設定
	btmemset( &datr, 0, sizeof(DSPATTR));
	datr.tposition.x=1;
	datr.attribute=BT_ATRB_NORM;
	datr.mag = BT_MAG_NORM;
	datr.fold = BT_FOLD_OFF;

	for(i=0;i<LCD_ROW_MAX; i++ ){
		datr.tposition.y=1+(i*2);
		btPrint( lcd->data[i], &datr);
	}
}
//-----------------------------------------------------------------------------
//	関数名：void LcdOutLeg( void *line1, void *line2, void *line3)
//	機能　：画面表示（表示only）
//	引数１：表示用の構造体
//		＊表示可能な文字数は、１行あたり20バイトです。また、文字の最後に
//		　必ずNullを付加して下さい。
//	SMGの移植のために追加 2011.04.24 matsuda
//	LegacyのLeg
//-----------------------------------------------------------------------------
void LcdOutLeg( char* msg1,char *msg2,char *msg3)
{
//	int i;
//	DSPATTR datr;

	btClear();
	
	//2011.11.08 matsuda あまりにも表示が小さいため下記から変更
	LineOut2(msg1,0,0);
	LineOut2(msg2,2,0);
	LineOut2(msg3,4,0);

/*
	// 表示属性設定
	btmemset( &datr, 0, sizeof(DSPATTR));
	datr.tposition.x=1;
	datr.attribute=BT_ATRB_NORM;
	datr.mag = BT_MAG_NORM;
	datr.fold = BT_FOLD_OFF;

	i=0;
	datr.tposition.y=1+(i*2);
	btPrint( msg1, &datr);

	i++;
	datr.tposition.y=1+(i*2);
	btPrint( msg2, &datr);

	i++;
	datr.tposition.y=1+(i*2);
	btPrint( msg3, &datr);
*/
}
//-----------------------------------------------------------------------------
//	関数名：short BarRead(char *databuf, short len,
//								char *name, char *msg, short mode);
// 	機能　：バーコードの読み込み。入力したデータを文字列で返す。
//			指定した桁数以内のバーコードであれば全て読み込む！
//			画面表示は何もしないのでアプリ側でする
//	引数１：char *databuf 取得文字列格納ポインタ(出力)、最大桁分必要！
//	引数２：short len	　読み取り可能な最大桁（'*'は含まない）
//	引数３：short mode 　 ﾃﾞｰﾀ入力ﾓｰﾄﾞ	0=終了機能なし
//						  F1=1,F2=2,F3=4として、終了ｷｰをorで指定
//	返値　：0=桁数over　:'-1=C'キー -11=F1キーで終了　:それ以外=読み取った桁数
//			-12=F2キーで終了	:-13=F3で終了
//-----------------------------------------------------------------------------
short BarRead(char *databuf, short len,short mode)
{
	short keycode,ret,bkind;
	char dat[512+1];
	long stat;
	KEYFUNC keyfunc;
	DSPATTR dspattr;
	INPUTSTRATTR strattr;
	BCRCONF bconf;


	// LキーをCLRキーに割り当てる
	btGetKeyFunction(&keyfunc);
	keyfunc.L = KEYCODE_CLR;
	btSetKeyFunction(&keyfunc);

	// 読み取り許可コードの設定
	bkind = 0;		//ｸﾘｱ
	bkind =bkind | 0x01;		// code39
	bkind =bkind | 0x03;		// code128
	bkind =bkind | 0x0a;		// QR code
//	bkind = 0;		//ｸﾘｱ
	// バーコード読み取りオプションの変更
	if( btSetBcrProperty(BT_PROP_ENABLE_SYMBOLS,&bkind)!= NO_ERR ){
		btMessageBox("バーコード読み取りモード設定エラー",BT_MB_CONFONLY);
	}
	// トリガモードの変更
	stat = BT_BCR_MOMENTARY;
	if( btSetBcrProperty( BT_PROP_TRIGGER_MODE,&stat ) != NO_ERR ){
		btMessageBox("トリガモード設定エラー",BT_MB_CONFONLY);
	}

	// 表示属性設定
	btmemset( &dspattr, 0, sizeof(DSPATTR));
	dspattr.mag = BT_MAG_NORM;
	dspattr.fold = BT_FOLD_ON;
	// 画面外で表示
	dspattr.tposition.x=1;
	dspattr.tposition.y=3;
	dspattr.attribute=BT_ATRB_NORM;


	// 入力設定
	btmemset( &strattr, 0, sizeof(INPUTSTRATTR));
	strattr.inputarea=20;
	if( len > 512 ) len=512;
	strattr.mindigit=0;
	strattr.maxdigit=len;
	strattr.inputtype=BT_INPUT_BCR;

	ret =1;
	while(ret==1){
		// 入力
		btmemset( dat, 0, sizeof(dat));
		keycode = btInputString(dat,&dspattr,&strattr);
		switch(keycode){
			// バーコードが読み込まれた場合、LED点灯し、ブザーを鳴らして数値入力画面を表示
			case KEYCODE_TRG:
				//btVibration(BT_ON_ASYNC,4,0,1);
				btLed(BT_ON_ASYNC,3,0,1,BT_LED_GREEN);
				btBuzzer(BT_ON_ASYNC,1,0,1,13);
				ret = 0;
				break;

			case KEYCODE_F1:
				if( mode & 0x01)ret=-11;
				break;
			case KEYCODE_F2:
				if( mode & 0x02)ret=-12;
				break;
			case KEYCODE_F3:
				if( mode & 0x04)ret=-13;
				break;
			case KEYCODE_CLR:
				ret=-4;
				break;
			default:
				break;
		}
	}

	// カーソルOFF
	btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);
	LineOut("",1,0);

	// バーコード読み取りオプションの変更
	bconf.mode = BT_BCR_AUTO_OFF;
	btSetBcrCommonOption(&bconf);

	// バッファにコピー
	if(!ret) btmemcpy( databuf, dat,btstrlen(dat) > len ? len :btstrlen(dat));
	while(btKeySense(1));		// keyが話されるまで待つ
	return(ret);
}
//-----------------------------------------------------------------------------
//	関数名：short KeyInput(char *databuf,char *name,
//										char *msg,short len,char moji );
//	機能　：ﾃﾞｰﾀをキー入力で取得する。
//			画面表示は何もしないのでアプリ側ですること
//	引数２：char *databuf  データ格納ポインタ(第4引数+1以上確保のこと)
//	引数３：short len  最大入力可能文字数（20以上は20になります)
//	返値　：キー入力されたﾃﾞｰﾀ長('C'キー時:: -1)
//-----------------------------------------------------------------------------
int KeyInput( char *databuf, short len)
{
	short keycode;
	char dat[50+1];

	DSPATTR dspattr;
	INPUTSTRATTR strattr;

	// 表示属性設定
	btmemset( &dspattr, 0, sizeof(DSPATTR));
	dspattr.mag = BT_MAG_NORM;
	dspattr.fold = BT_FOLD_ON;

	// 入力設定
	btmemset( &strattr, 0, sizeof(INPUTSTRATTR));
	strattr.inputarea=20;
	if( len > 20 || len < 1) len=20;
	strattr.mindigit=0;
	strattr.maxdigit=len;
	dspattr.attribute=BT_ATRB_NORM;

	// key入力
	// 2行目に表示
	dspattr.tposition.x=1;
	dspattr.tposition.y=3;
	strattr.inputtype=BT_INPUT_KEY;
	while(1){
		// 入力
		btKeyBufClear();
		btmemset( dat, 0, sizeof(dat));
		keycode = btInputString(dat,&dspattr,&strattr);
		// 入力完了
		if( keycode == KEYCODE_ENT ) break;
		if( keycode == KEYCODE_F1 ){
			// カーソルOFF
			LineOut("",1,0);
			btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);
			return-1;
		}
	}

	// カーソルOFF
	LineOut("",1,0);
	btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);

	// バッファにコピー
	btmemcpy( databuf, dat,btstrlen(dat));
	while(btKeySense(1));		// keyが話されるまで待つ
	return((int)btstrlen(dat));

}
//-----------------------------------------------------------------------------
//	関数名：short DataInput(short mode, char *databuf, 
//								char *name, char *msg, short len);
// 	機能　：バーコードの読み込みorデータの手入力。入力したデータを文字列で返す。
//	引数１：short mode 　ﾃﾞｰﾀ入力ﾓｰﾄﾞ
//								1=ﾊﾞｰｺｰﾄﾞのみ
//								2=手入力のみ
//								3=両方
//								4=ﾊﾞｰｺｰﾄﾞのみ/終了機能つき・表示は"[F1]ｷｬﾝｾﾙ"
//	引数２：char *databuf 取得文字列格納ポインタ(出力)
//	引数３：char *name  入力項目名(1行目に表示します)
//	引数４：char *msg 	メッセージ(3行目に表示します)
//	引数５：short len	入力可能な最大文字数。ﾊﾞｰｺｰﾄﾞの場合はこれ以外の長さはNG
//	返値　：-1=強制終了　:0=正常　:1=終了 -11=F1終了
//	ディスプレイの5-7行はクリアされません
//-----------------------------------------------------------------------------
//	Function name:short DataInput(short mode, char *databuf, 
//								char *name, char *msg, short len);
// 	Outline      :Scan barcode & QR code and input key data. Return inputed character.
//	Arg1         :Input mode
//					1=only barcode(including QR code)
//					2=input key by manual
//					3=both(barcode & manual)
//					4=only barcode but this mode have cancel function.
//  Arg2         :Data destination pointer.
//  Arg3         :Input item name ( display on line 1 )
//  Arg4         :Maximum input size.
//  Return value : -2=cancel 0:success 
//-----------------------------------------------------------------------------
short DataInput( short mode, char *databuf, char *name, char *msg, short len)
{
	short keycode;
	char dat[50+1];
	short bk;

	DSPATTR dspattr;
	INPUTSTRATTR strattr;
	BCRCONF bconf;

	// 画面表示
	LineOut2( name,0,0);
	LineOut( msg,4,0);

	// 表示属性設定
	btmemset( &dspattr, 0, sizeof(DSPATTR));
	dspattr.mag = BT_MAG_NORM;
	dspattr.fold = BT_FOLD_ON;
	dspattr.tposition.x=1;
	dspattr.tposition.y=20;
	dspattr.attribute=BT_ATRB_NORM;

	// 入力設定
	btmemset( &strattr, 0, sizeof(INPUTSTRATTR));
	strattr.inputarea=20;
	if( len > 50 ) len=50;
//	strattr.mindigit=len;
	strattr.mindigit=1;
	strattr.maxdigit=len;

	// バーコード入力 & キー入力
	if( mode != 2 ){
		// バーコード読み取りオプションの変更
		btGetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_MOMENTARY;
		btSetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_AUTO_OFF;			// 戻し用
		if( mode == 1 )	strattr.inputtype=BT_INPUT_BCR;		// バーコード入力 & キー入力
		if( mode == 3 )	strattr.inputtype=BT_INPUT_KEYBCR;		// バーコード入力 & キー入力
		bk=1;
	//	if( mode == 3 )	LineOut( "[Ｌ]   キー入力",8,0);
		if( mode == 4 ){
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "[Ｆ１] キャンセル",8,0);
			else LineOut( "[F1] CANCEL",8,0);
		}
		while(bk){
			// 入力
			btmemset( dat, 0, sizeof(dat));
			keycode = btInputString(dat,&dspattr,&strattr);
			switch(keycode){
				// バーコードが読み込まれた場合、LED点灯し、ブザーを鳴らして数値入力画面を表示
				case KEYCODE_TRG:
					//btVibration(BT_ON_ASYNC,4,0,1);
					btLed(BT_ON_ASYNC,3,0,1,BT_LED_GREEN);
					btBuzzer(BT_ON_ASYNC,1,0,1,13);
					// バッファにコピー
					btmemcpy( databuf, dat,len);
					btSetBcrCommonOption(&bconf);
					while(btKeySense(1));		// keyが話されるまで待つ
					return 0;

				case KEYCODE_ENT:
					// バッファにコピー
					btmemcpy( databuf, dat,len);
					return 0;				
				
				case KEYCODE_F1:
					return -2;
					break;
				case KEYCODE_L:
					if( mode == 3 ){
						bk=0;
						btSetBcrCommonOption(&bconf);
					}
					break;
				// F3もOKにした 2015.10.22 coba
				case KEYCODE_F3:
					return -4;
					break;

				default:
					break;
			}
		}
	}

	// key入力
	// 2行目に表示
	dspattr.tposition.x=1;
	dspattr.tposition.y=5;
	strattr.inputtype=BT_INPUT_KEY;
	strattr.mindigit=1;
//	LineOut( "01234567890123456789",8,0);
	if( g_Lang == BT_LANG_JAPANESE )
		LineOut( "   [テンキー入力]   ",7,0);
	else LineOut( "    [KEY INPUT]   ",7,0);
	LineOut( "[ENT] OK    [C] Back",8,0);
	while(1){
		// 入力
		btmemset( dat, 0, sizeof(dat));
		keycode = btInputString(dat,&dspattr,&strattr);
		// 入力完了
		if( keycode == KEYCODE_ENT ) break;
	}

	// バッファにコピー
	btmemcpy( databuf, dat,len);

	// カーソルOFF
	btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);
	LineOut("",1,0);
	return 0;
}
//-----------------------------------------------------------------------------
//	関数名：short FuncSel(short mode)
//	機能　：ファンクションキーの選択入力。
//	引数　：入力ﾓｰﾄﾞ
//					 0="何かキーを押してください"のとき。
//					 1="[L]OK [R]NG"のとき。
//					 2="L=OK R=NG ENT=追加"のとき。
//					 3="[L]YES [R]NO" のとき
//					 4="何かキーを押してください"のとき。
//						隠しキーあり
//	返値　：押されたキーの種類（0=L :-1=R :-3=ENT :-11=F1 :-13=F3)
//-----------------------------------------------------------------------------
short FuncSel(short mode)
{
	long key;
	char keycode;
	short ret;

	btKeyBufClear();

	// mode0は別
	if(mode==4){
		while(btKeySense(1));		// keyが話されるまで待つ
	    btWait(1,0);
		btKeyBufClear();
		if(g_Lang==BT_LANG_JAPANESE){
			LineOut("何かｷｰを押して下さい",10,0);
		}
		else LineOut("    PUSH ANY KEY",10,0);
		ret=1;
	
		while(ret==1){
		    btWait(1,0);
			// キー入力を待つ
			key = btKeySense(0);
			// C + Fx
			if( key & KEY_CLR ){
				if( key & KEY_F1 ) ret=-31;
				if( key & KEY_F2 ) ret=-32;
				if( key & KEY_F3 ) ret=-33;
			}
			// L + Fx
			else if( key & KEY_L ){
				if( key & KEY_F1 ) ret=-21;
				if( key & KEY_F2 ) ret=-22;
				if( key & KEY_F3 ) ret=-23;
			}
			else if( key ) ret=0;
		}
		while(btKeySense(1));		// keyが話されるまで待つ
		return(ret);
	}

	// mode !=0
//	if( mode == 2) LineOut("01234567890123456789",8,0);
	if( mode == 2) LineOut(" L:OK  R:NG ENT:追加",10,0);
	else if( mode == 3) LineOut(" [Ｌ] YES  [Ｒ] NO",10,0);
	else if (mode ==0){
		if(g_Lang==BT_LANG_JAPANESE){
			LineOut("何かｷｰを押して下さい",10,0);
		}
		else{
			LineOut("    PUSH ANY KEY",10,0);
		}
	}
	else LineOut("  [Ｌ] OK  [Ｒ] NG",10,0);
	while(1){
		// キー入力を待つ
		btKeyBufClear();
		key = btKeyWait(0);
		if(!mode)return(0);

		// 論理キーに変換する
		btTbl2Code(key,&keycode);

		// Lキー
		if( keycode == KEYCODE_L || keycode == KEYCODE_ENT){
			ret=0;
			break;
		}
		// Rキー
		else if( keycode == KEYCODE_R){
			ret=-1;
			break;
		}
		// ENTキー
		else if( keycode == KEYCODE_ENT && mode == 2){
			ret=-3;
			break;
		}
	}

	return(ret);
}
//-----------------------------------------------------------------------------
//	関数名：void GetNowtime( char *data )
//	機能　：現在の日付をYYYYMMDDHHMMSSの形で取得する。
//　引数　：取得データ格納ポインタ（出力） *14バイト以上確保必要！
//  返値　：なし
//-----------------------------------------------------------------------------
short GetNowtime( char *data)
{
	char buf[40];
	DATEINFO di;
	TIMEINFO ti;

	btmemset( buf,0,sizeof(buf));

	if( btGetDateTime(&di,&ti)) return(-1);

	// 年
	if( di.year <1000 && di.year > 9999 ) return(-2);
	btnum2char((long)di.year,buf);

	// 月
	if( di.month <1 && di.month > 12 ) return(-3);
	if( di.month < 10 ){
		buf[4]='0';
		btnum2char((long)di.month,&buf[5]);
	}
	else {
		btnum2char((long)di.month,&buf[4]);
	}
	btmemcpy( data, buf,14);

	// 日
	if( di.day <1 && di.day > 31 ) return(-4);
	if( di.day < 10 ){
		buf[4]='0';
		btnum2char((long)di.day,&buf[7]);
	}
	else {
		btnum2char((long)di.day,&buf[6]);
	}

	// 時
	if( (int)ti.hour <0 && ti.hour > 24 ) return(-5);
	if( ti.hour < 10 ){
		buf[4]='0';
		btnum2char((long)ti.hour,&buf[9]);
	}
	else {
		btnum2char((long)ti.hour,&buf[8]);
	}
	// 分
	if( (int)ti.min <0 && ti.min >= 60 ) return(-6);
	if( ti.min < 10 ){
		buf[4]='0';
		btnum2char((long)ti.min,&buf[11]);
	}
	else {
		btnum2char((long)ti.min,&buf[10]);
	}
	// 秒
	if( (int)ti.sec <0 && ti.sec >= 60 ) return(-7);
	if( ti.sec < 10 ){
		buf[4]='0';
		btnum2char((long)ti.sec,&buf[13]);
	}
	else {
		btnum2char((long)ti.sec,&buf[12]);
	}

	// 出力
	btmemcpy( data, buf,14);
	return(0);
}
//-----------------------------------------------------------------------------
//	関数名： long Ltime( void )
//	機能　：午前0時からの経過秒数を取得
//　引数　：取得データ格納ポインタ（出力） *12バイト以上確保必要！
//  返値　：なし
//-----------------------------------------------------------------------------
long Ltime( void )
{
	TIMEINFO ti;

	if( btGetDateTime(NULL,&ti)) return(-1);

	return( (long)ti.hour * 3600+ (long)ti.sec*60+ti.min);

}