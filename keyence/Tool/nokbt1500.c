//=============================================================================
//		�L�[�G���XBT-900�o�[�R�[�h�p���C�u����
//		File name : nokbt900.c
//
//		���Y�Z�p�{��  ���Y�Z�p��
//		�쐬�� �F coba
//		�쐬�� �F ver.0.00 2004.06.11
//		�X�V�� �F ver.1.00 2004.06.28 TAKE �蒼�����֐��̒ǉ�
//		�X�V�� �F ver.1.01 2004.08.17 TAKE ���ё��M��A��ʕ\������悤�ɕύX
//		�X�V�� �F ver.1.02 2004.12.28 TAKE btRfOpen()�̕Ԃ�l�`�F�b�N�� BT_RF_API_SET_ERR��BT_RF_API_SET_OK�ɕύX
//		�X�V�� �F ver.1.03 2005.12.06 TAKE ��Ǝғ��͂��Q���ł����͂ł���悤�ɂ���
//		�X�V�� �F ver.0.02 2006.09.24 TAKE TPE�u�[�c����FP��
//=============================================================================
//		��L������BT-1500�p�ɒu������ 2010.08.03 ����
//		DINPUT_SETTING�̍\���̂̒���alpha�i�A���t�@�x�b�g���́j��ǉ� 2012.04.18 okamura
//		��ʕ\����ύX���邽�߂�DataInputBt2��ǉ� 2012.04.18 okamura
//		2012.10.15 matsuda Init�œǎ��R�[�h�̐ݒ���s���悤�ύX
//		2012.12.06 sanno	DataInputBt�֐��ɁA�\���̒��̏����\���l���Z�b�g�����Ƃ��͏����l��\������悤�ύX
//=============================================================================

#include "nokbt1500.h"

//TCP�T�[�o�[�A�h���X�ƃ|�[�g
//TCP server address and TCP port No.
char g_HostAddr[100];
int g_PortNo;
int g_Lang;

void CloseSocket( long socketno );
//-----------------------------------------------------------------------------
//	�֐����Fshort AllInit(void);
//	�@�\�@�F�o�[�R�[�h�A̧�فA��ʁA�L�[�{�[�h���̏�����
//	�����@�F�Ȃ�
//	�Ԓl�@�F�ʏ�=0
//			���̑��F�N�����ɉ�����Ă����L�[
//-----------------------------------------------------------------------------
//	Function Name:short AllInit(void)
//	Outline      :Initialize all devices.
//-----------------------------------------------------------------------------
short AllInit(void)
{
	return( AllInitial(FALSE));
}
// 2012.10.01 coba Resume�w���ǉ�
short AllInitial(BOOL resume)
{
	long key,stat;
	short bkind;

	//Get language information;
	stat=btHandyGetParameter(PRM_LANGUAGE,&g_Lang);
	if(stat!=BT_OK)g_Lang = BT_LANG_JAPANESE;
	
	if( g_Lang == BT_LANG_JAPANESE )
		LineOut("   ��������...",5,0);
	else 
		LineOut(" Initializing...",5,0);

	key = btKeyWaitEx( 0, 2);
	// Buzzer volume = MAX �u�U�[����(MAX)
	btVolume(3);
g_Lang=100;
	//���W���[���@�\OFF 2012.10.01 coba �����ŋ@�\�̗L���E������ؑւ�
	if(resume == FALSE){	//if resume == FALSE, disable resume function
		stat = 0;
		btHandyGetParameter(PRM_RESUME,&stat);
		if( stat == BT_ON ){
			stat = BT_OFF;
			if( btHandySetParameter(PRM_RESUME,&stat) != BT_OK ){
				if( g_Lang == BT_LANG_JAPANESE )
					LineOut("ڼޭ�����Ұ��ݒ莸�s",8,0);
				else
					LineOut("Can not set resume params",8,0);

				FuncSel(0);
				return -20;
			}
		}
	}

	//Initialize the wireless setting.�����֌W�̐ݒ�m�F
	if( WLAN_init() ){
		//���s=IP�ݒ肳��ĂȂ��̂Őݒ肳����
		//Falure = Don't have IP address setting. so Execute input function.
		WLAN_setting();
		if( WLAN_init() ){
			if( g_Lang == BT_LANG_JAPANESE ){
				LineOut( "�����ʐM�ݒ�װ!!",2,0);
			}
			else{
				LineOut( "Wireless setting",2,0);
				LineOut( "error!!",4,0);
			}
			FuncSel(0);
			return(-2);
		}
	}

	//�����|�[�g�I�[�v��
	// ��x���ׂẴl�b�g���[�N�N���[�Y
	// Close all wireless module
	btNetworkClose( 0 );
	// Open wireless module
	if( btNetworkIsOpen( BT_NETWORK_WLAN_INFRA ) == BT_NETWORK_ALREADYOPEN ){
		btNetworkClose(BT_NETWORK_WLAN_INFRA);//If failure,Close once. ���s�������UClose����
	}
	//�����FBT_ON_SYNC�A�񓯊�:BT_ON_ASYNC
	stat=btNetworkOpen(BT_NETWORK_WLAN_INFRA,BT_ON_SYNC);
	if(stat != BT_NETWORK_OK ){	
		if( g_Lang == BT_LANG_JAPANESE )
			LineOut( "�߰�Open�װ!!",2,0);
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

	// �o�[�R�[�h�ǂݎ��I�v�V�����̕ύX
	if( btSetBcrProperty(BT_PROP_ENABLE_SYMBOLS,&bkind)!= NO_ERR ){
		if( g_Lang == BT_LANG_JAPANESE )
			btMessageBox("�o�[�R�[�h�ǂݎ�胂�[�h�ݒ�G���[",BT_MB_CONFONLY);
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
			btMessageBox("�o�[�R�[�h�ǂݎ�胂�[�h�ݒ�G���[",BT_MB_CONFONLY);
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
		btNetworkClose(BT_NETWORK_WLAN_INFRA);// ���s�������UClose����
	}

	//�����FBT_ON_SYNC�A�񓯊�:BT_ON_ASYNC
	stat=btNetworkOpen(BT_NETWORK_WLAN_INFRA,BT_ON_SYNC);
	if(stat != BT_NETWORK_OK ){	
		if( g_Lang == BT_LANG_JAPANESE )
			LineOut( "�߰�Open�װ!!",2,0);
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
			LineOut("�I�[�g�p���[�I�t�ݒ莸�s",4,0);
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
	//title �^�C�g��
	LineOut( title, 0, 1);	//Reversal character.�^�C�g���̂ݔ��]������
	
	//line 1. Big size char.�P�s�ځ@�啶��
	LineOut2( l1, 1, 0);
	//line 2. Big size char.�Q�s�ځ@�啶��
	LineOut2( l2, 3, 0);
	//line 3. Big size char.�R�s�ځ@�啶��
	LineOut2( l3, 5, 0);

	//guide 1. small char. �K�C�h�P�@������
	LineOut( g1, 8, 0);
	//guide 1. small char.  �K�C�h�Q�@������
	LineOut( g2, 9, 0);
	//guide 1. small char.  �K�C�h�R�@������
	LineOut( g3, 10, 0);

	while(1){
		// �L�[���͂�҂�
		// wait to keyinput.
		btKeyBufClear();
		key = btKeyWait(0);
		// change logical key.  �_���L�[�ɕϊ�����
		btTbl2Code(key,&keycode);
		break;
	}
	return keycode;
}

//-----------------------------------------------------------------------------
//	�֐����Fvoid LineOut4( char* dat, short lineNo,short reverse)
//	�@�\�@�F��ʕ\���i�\��only�j
//	�����P�F
//		�S�{�p�\��
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

	// set display attribute. �\�������ݒ�
	btmemset( &datr, 0, sizeof(DSPATTR));
	datr.tposition.x=1;
	datr.mag = BT_MAG_4;
	datr.fold = BT_FOLD_OFF;

	// reversal ���]
	if(!rev_mode ) datr.attribute=BT_ATRB_NORM;
	else datr.attribute=BT_ATRB_REV;

	// display position �s
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

	// set display attribute. �\�������ݒ�
	btmemset( &datr, 0, sizeof(DSPATTR));
	datr.tposition.x=1;
	datr.mag = BT_MAG_TALL;
	datr.fold = BT_FOLD_OFF;

	// reversal ���]
	if(!rev_mode ) datr.attribute=BT_ATRB_NORM;
	else datr.attribute=BT_ATRB_REV;

	// set display position �s
	datr.tposition.y=1+(lineNo*2);

	btPrint( dsp, &datr);
}

//-----------------------------------------------------------------------------
//	�֐����Fvoid LineOut( char* dat, short line,short reverse);
//	�@�\�@�F�s���w�肵����ʕ\���i�\��only�j
//			�w��s�ȊO�͕ω����Ȃ�
//	�����P�F�\���f�[�^
//	�����Q�F�\���s(1-8)
//	�����R�F0=�ʏ�A1=���]�\��
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

	// �\�������ݒ�
	btmemset( &datr, 0, sizeof(DSPATTR));
	datr.tposition.x=1;
	datr.mag = BT_MAG_NORM;
	datr.fold = BT_FOLD_OFF;

	// ���]
	if( !rev_mode )
		datr.attribute = BT_ATRB_NORM;
	else
		datr.attribute = BT_ATRB_REV;

	// �s
	datr.tposition.y=1+(lineNo*2);

	btPrint( dsp, &datr);

}
//-----------------------------------------------------------------------------
//	�֐����Fvoid LineOut( char* dat, short line,short reverse);
//	�@�\�@�F�s���w�肵����ʕ\���i�\��only�j
//			�w��s�ȊO�͕ω����Ȃ�
//	�����P�F�\���f�[�^
//	�����Q�F�\���s(1-8)
//	�����R�F�F�w��
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
	unsigned long dotcolor_tmp = 0; // �J���[�ݒ�ޔ�p
	unsigned long backcolor_tmp = 0;
	unsigned long seldotcolor_tmp = 0;
	unsigned long selbackcolor_tmp = 0;
	unsigned long nowcolor;
	int ret;

	// Back up color settings. �J���[�ݒ�̑ޔ�
	ret = btGetColor( &dotcolor_tmp, &backcolor_tmp, &seldotcolor_tmp, &selbackcolor_tmp );

	if( lineNo < 0 || lineNo >=LCD_ROW_MAX ) return;

	btmemset(dsp, 0,sizeof(dsp));
	btmemset(dsp, 0x20, 20);
	btmemcpy( dsp, dat,btstrlen(dat)>20 ? 20:btstrlen(dat));

	// �\�������ݒ�
	btmemset( &datr, 0, sizeof(DSPATTR));
	datr.tposition.x=1;
	datr.mag = BT_MAG_NORM;
	datr.fold = BT_FOLD_OFF;
	datr.attribute = BT_ATRB_NORM;

	// �s
	datr.tposition.y=1+(lineNo*2);

	// �F�ݒ�
	if( color==CLR_RED ) nowcolor=RED ;
	else if( color==CLR_BLUE ) nowcolor=BLUE ;
	else if( color==CLR_YELLOW ) nowcolor=YELLOW ;
	else if( color==CLR_CYAN   ) nowcolor=CYAN   ;
	else if( color==CLR_GREEN  ) nowcolor=GREEN  ;
	else if( color==CLR_MAGENTA ) nowcolor=MAGENTA ;
	else if( color==CLR_WHITE   ) nowcolor=WHITE   ;
	else /*if( clolor==CLR_BLACK )*/ nowcolor=BLACK ;

	// �����F�̂ݕύX
	ret = btSetColor( nowcolor, backcolor_tmp, seldotcolor_tmp, selbackcolor_tmp );

	btPrint( dsp, &datr);

	// �����F�߂�
	ret = btSetColor( dotcolor_tmp, backcolor_tmp, seldotcolor_tmp, selbackcolor_tmp );

}
//-----------------------------------------------------------------------------
//	�֐����Fvoid LcdClear(void);
//	�@�\�@�F�@�\�@�F��ʂ̃N���A
//	�����@�F�Ȃ�
//	�Ԓl�@�F�Ȃ�
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
//	�֐����Fshort InputAlphabet( char *databuf )
// 	�@�\�@�F�A���t�@�x�b�g�̓��͊֐��c�g�ѓd�b�̂悤�ȓ���A�`Z
//			�������͂⏬�������͂͑啶���ɕϊ�����@�\����
//	�����P�Fchar *databuf �擾������i�[�|�C���^(�o��)
//	�Ԓl�@�F0=����@:1=�I�� -1=F1�I��
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

	// set attribute. �\�������ݒ�
	btmemset( &dspattr, 0, sizeof(DSPATTR));
	dspattr.mag = BT_MAG_4;
	dspattr.fold = BT_FOLD_ON;
	dspattr.attribute=BT_ATRB_NORM;

	LcdClear();

	// set input setting ���͐ݒ�
	btmemset( &strattr, 0, sizeof(INPUTSTRATTR));
	strattr.inputarea=20;
	strattr.mindigit=1;
	strattr.maxdigit=1;

	// key����
	// 2�s�ڂɕ\��
	dspattr.tposition.x=1;
	dspattr.tposition.y=7;
	strattr.inputtype=BT_INPUT_KEY_SFT_ON|BT_INPUT_ESCAPE_ON;
	strattr.mindigit=1;
	LineOut2( msg,0,0);
	if( g_Lang == BT_LANG_JAPANESE ){
		LineOut( " (A�`Z��1��������)",2,0);
		LineOut( "�����͑啶���Ɏ���",7,0);
		LineOut( "�ϊ�����܂�",8,0);
		LineOut( "ENT:OK C:Del F1:���~",9,0);
	}
	else{
		LineOut( " (Input A�`Z)",2,0);
		LineOut( "Character will be ",7,0);
		LineOut( "uppercase",8,0);
		LineOut( "ENT:OK C:Del F1:ESC",9,0);
	}
	while(1){
		// Input function .����
		btmemset( dat, 0, sizeof(dat));
		keycode = btInputString(dat,&dspattr,&strattr);
		// finish to input.���͊���
		if( keycode == KEYCODE_ENT && btstrlen(dat) ){
			//change to uppercasse.�������A���t�@�x�b�g�̏ꍇ�̏���
			if( dat[0] >= 0x61 && dat[0] <= 0x7a ){
				dat[0] -= 0x20;
				break;
			}
			//If data is number....�������͂̏ꍇ�̏���
			if( dat[0] >= 0x31 && dat[0] <= 0x39 ){
				dat[0] += 0x10;
				break;
			}
			//Uppercase is break.�ʏ�̑啶���Ȃ�break;
			if( dat[0] >= 0x41 && dat[0] <= 0x5a ) break;
		}
		if( keycode == KEYCODE_F1 )return -1;
	}

	// �o�b�t�@�ɃR�s�[
	btmemcpy( databuf, dat,1);

	// �J�[�\��OFF
	btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);
	LineOut("",1,0);
	return 0;
}

//-----------------------------------------------------------------------------
//	�֐����Fshort NoSel(short no)
//	�@�\�@�F�����Ŏw�肵�����̕������e���L�[��L���ɂ��āA�����ꂽ�L�[��Ԃ��B
//	�����@�Fshort no �I�����ڐ�(0�`9)
//	�Ԓl�@�F�����ꂽ�L�[�iex.'1'�������ꂽ��Ԓl���P)
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
		// �L�[���͂�҂��A�_���L�[�ɕϊ�����
		btKeyBufClear();
		key = btKeyWait(0);
		btTbl2Code(key,&keycode);
		// Enable keys. �L���ȃL�[
		if( keycode >='0' && keycode <= no+'0') return( (int)keycode-'0');
		if( keycode == KEYCODE_R ) return -1;
		if( keycode == KEYCODE_L ) return 0;
		if( keycode == KEYCODE_ENT ) return 0;
		// 2013.02.06 coba TRG���L���ɂ���
		if( keycode == KEYCODE_TRG ) return 0;
		// 2013.07.22 matsuda F1���L���Ƃ���悤�ύX
		if( keycode == KEYCODE_F1 ) return -2;
		
		// cancel ��ݾ�
		else if( cancel && (keycode == KEYCODE_CLR || keycode == KEYCODE_BS)) break;
	}
	return(-1);
}

//-----------------------------------------------------------------------------
//	�֐����Fshort NoSel(short no)
//	�@�\�@�F�����Ŏw�肵�����̕������e���L�[��L���ɂ��āA�����ꂽ�L�[��Ԃ��B
//	�����@�Fshort no �I�����ڐ�(0�`9)
//	�Ԓl�@�F�����ꂽ�L�[�iex.'1'�������ꂽ��Ԓl���P)
//	�J�V�I�Ƃ̌݊��֐�
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
		// �L�[���͂�҂��A�_���L�[�ɕϊ�����
		btKeyBufClear();
		key = btKeyWait(0);
		btTbl2Code(key,&keycode);
		if( ekey == NULL ) return 0;
		if( ekey->anykey ){
			// �L���ȃL�[
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
			// ��ݾ�
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
//	�֐����Fshort PingTest( char *address )
//	�@�\�@�FPING�̎��s
//	�����P�FHost Address
//	�Ԓl�@�F0=����  �F1=���s
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

	//Get host ip address from handy terminal setting.�z�X�gIP���擾
	btmemset(addr,0,sizeof(addr));
	if( btNetworkGetParameter( BT_TARGET_NETWORK,PRM_HOSTIP,addr ) != BT_OK ){
		if( g_Lang == BT_LANG_JAPANESE )
			LineOut( "�z�X�gIP�����ݒ�",2,0);
		else{
			LineOut( "Host IP address",2,0);
			LineOut( "does not exists.",2,0);
		}
		FuncSel(0);
		return(-2);
	}

	btClear();

	if( g_Lang == BT_LANG_JAPANESE )
		LineOut( "  PING   ���M��   ",1,1);
	else LineOut( " PING executing...",1,1);

	ip = btSocketInet_addr(addr);
	// ���M
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
//	�@�\�@�F���b�Z�[�W��\������(14�o�C�g/1�s�A5�s�\��)
//	�����P�F�C���X�^���X�n���h��
//	�����Q�F�e�E�C���h�E�̃n���h��
//	�����R�F�^�C�g��(unicode)
//	�����S�F�ŉ��s�ɕ\������L�[�K�C�h�̎��
//			0:�������������ĉ�����
//			1:������I�����ĉ�����
//			2:�ԍ���I�����ĉ�����
//			3:L=�͂� R=������
//			6:�I���W�i�����b�Z�[�W
//	����4-�F�\������G���[���b�Z�[�W
//	�����T�F�L���ɂ��鷰(NULL�ɂ���ƁA�S�Ă̷����L��)
//	�߂�l�F0�`9=�����ꂽ�L�[�̔ԍ�
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

	LineOut( title, 0, 1);	//Show title as reversal. �^�C�g���̂ݔ��]������
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
				btsprintf(buf,"�������������ĉ�����");
			else btsprintf(buf,"    PUSH ANY KEY");
			break;
		case 1:
			if( g_Lang == BT_LANG_JAPANESE )
				btsprintf(buf,"������I�����ĉ�����");
			else btsprintf(buf," Select Operations");
			break;
		case 2:
			if( g_Lang == BT_LANG_JAPANESE )
				btsprintf(buf,"�ԍ���I�����ĉ�����");
			else btsprintf(buf," Choose a number");
			break;
		case 3:
			if( g_Lang == BT_LANG_JAPANESE )
				btsprintf(buf,"  L:�͂�  R:������");
			else btsprintf(buf,"    L:NO  R:YES");
			break;
		case 4:
			if( g_Lang == BT_LANG_JAPANESE )
				btsprintf(buf," L:����  R:�������f");
			else btsprintf(buf,"    L:OK  R:Cancel");
			break;

		case 5:
			if( g_Lang == BT_LANG_JAPANESE )
				btsprintf(buf,"      [1]:����");
			else btsprintf(buf,"      [1]:OK");
			break;
		// 2012.12.06 coba �I���W�i�����b�Z�[�W�\����ǉ�
		case 6:
			btstrcpy(buf,guide);
			break;
		default:
			btmemset(buf,0,sizeof(buf));
			break;
	}
	LineOut( buf, 10, 0);

	//����
	switch(guideno){
		case 1://������I�����ĉ�����
		case 2://�ԍ���I�����ĉ�����
		case 3://  L:�͂�  R:������
		case 4:
		case 6:
			return ( NoSelEnableKey( enkeys ) );
			break;
		case 0://�������������ĉ�����
		case 5://    [1]:����
		default:
			return ( NoSel(10,1) );
			break;
	}
	return 0;
}
//Compatible function for Casio DT-950. DT-950�p���ʌ݊��֐�
int DspMsg( char *title,int guideno
						,char* l1,char* l2,char* l3,char* l4,char* l5
						,ENABLE_WAITKEYS* enkeys){
	return( DspMsgBt( title,guideno,l1,l2,l3,l4,l5,"","",enkeys));
}
//DT-950�p���ʌ݊��֐�
int DspMsg2( char *title,int guideno
						,char* l1,char* l2,char* l3,char* l4,char* l5,char* l6
						,ENABLE_WAITKEYS* enkeys){
	return( DspMsgBt( title,guideno,l1,l2,l3,l4,l5,l6,"",enkeys));
}
//-----------------------------------------------------------------------------
//	�֐����Fvoid BuzzerComp(void)
//	�@�\�@�F�����̃u�U�[
//	�����@�F�Ȃ�
//	�Ԓl�@�F�Ȃ�
//	�J�V�I�݊��֐�
//-----------------------------------------------------------------------------
//  Function name: void BuzzerComp(void)
//  Outline      : Ring buzzer when operation is OK.
//-----------------------------------------------------------------------------
void BuzzerComp(void){Beep(0);}
//-----------------------------------------------------------------------------
//	�֐����Fvoid Buzzer(void)
//	�@�\�@�F�G���[�̎��̃u�U�[
//	�����@�F�Ȃ�
//	�Ԓl�@�F�Ȃ�
//	�J�V�I�݊��֐�
//-----------------------------------------------------------------------------
//  Function name: void Buzzer(void)
//  Outline      : Ring buzzer when operation is NG.
//-----------------------------------------------------------------------------
void Buzzer(void){Beep(2);}
//-----------------------------------------------------------------------------
//	�֐����Fvoid BuzzerV(void)
//	�@�\�@�F�G���[�̎��̃u�U�[(�o�C�u���[�V�����t��)
//	�����@�F�Ȃ�
//	�Ԓl�@�F�Ȃ�
//	�J�V�I�݊��֐�
//	2011.02.12 matuda �ǉ�
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Function name: void BuzzerV(void)
//  Outline      : Ring buzzer with vibration when operation is NG.
//-----------------------------------------------------------------------------
void BuzzerV(void)
{
	//BT_OFF ��~
	//BT_ON_ASYNC ����1�i�񓯊��j
	//BT_ON_SYNC ����2�i�����j
	//�u�U�[���Ȃ��Ă���ԐU������(0.7sec)
	btVibration(BT_ON_ASYNC,7,0,1);
	Beep(2);
}
//-----------------------------------------------------------------------------
//	�֐����Fvoid Beep(void);
//	�@�\�@�F�G���[���Ƀr�[�v�����Ȃ炷
//	�����@�F�Ȃ�
//	�Ԓl�@�F�Ȃ�
//		���ݒ�͕ʂ̂Ƃ���ł���Ă��܂�
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
//	�֐����Fvoid OkNgDisp( BOOL okng,int timer )
//	�@�\�@�FCASIO dt-950�̌݊��֐�
//	�����P�F�s�q�t�d�F�n�j�\���A�e�`�k�r�d�F�m�f�\��
//	�����Q�F�^�C�}�[�b �����Ł~100msec����܂�
//	�Ԓl�@�F�Ȃ�
//		���ݒ�͕ʂ̂Ƃ���ł���Ă��܂�
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
	unsigned long dotcolor_tmp = 0; // �J���[�ݒ�ޔ�p
	unsigned long backcolor_tmp = 0;
	unsigned long seldotcolor_tmp = 0;
	unsigned long selbackcolor_tmp = 0;
	int ret;

	btClear();
	// Back up color settings. �J���[�ݒ�̑ޔ�
	ret = btGetColor( &dotcolor_tmp, &backcolor_tmp, &seldotcolor_tmp, &selbackcolor_tmp );

	// Set display attribute.�\�������ݒ�
	btmemset( &datr, 0, sizeof(DSPATTR));
	datr.mag = BT_MAG_25;
	datr.fold = BT_FOLD_OFF;
	datr.attribute = BT_ATRB_NORM;

	// �s
	datr.gposition.x=0;
	datr.gposition.y=70;

	if( timer < 0 || timer > 50 ) timer=1;
	//fix to 1 for a long time.���ʂ�1�Œ�ɂ��܂��B
	timer=1;

	//NG
	if(okng==FALSE){	
		if( g_Lang == BT_LANG_JAPANESE )
			btstrcpy(dsp,"�m�f");
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
			btstrcpy(dsp,"�n�j");
		else btstrcpy(dsp," OK ");
		ret = btSetColor( BLUE ,WHITE, BLACK,BLACK);
		btPrintg( dsp, &datr);

		btLed(BT_ON_ASYNC,2,0,1,BT_LED_GREEN);
		btBuzzer(BT_ON_ASYNC,2,0,1,5);
		Wait(10);
	}
	// Return the color setting.�J���[�ݒ��߂�
	ret = btSetColor( dotcolor_tmp, backcolor_tmp, seldotcolor_tmp, selbackcolor_tmp );

	btClear();
}

//=============================================================================
//	void get_nowtime( char *buf, short mode)
//	�@�\�F�������̎擾
//=============================================================================
//  Function name :void get_nowtime( char *buf, short mode)
//  Outline       :Get current time
//  Arg1          :destinatin data pointer.
//  Arg2          :mode
//=============================================================================
void get_nowtime( char *buf, short mode)
{
	DATEINFO di; //���t���\���̕ϐ���
	TIMEINFO ti; //�������\���̕ϐ���
	TMSTRCT *outTime;
	short ret;
	char dummy[50];

	ret = btGetDateTime (&di, &ti); //���t����ю����̎擾���s���܂�
	if( ret ) return;	//�ُ�I��
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
//	�֐����Fint Calctime( char *src,char *dest,short days)
//	�@�\�@�F���t�̌v�Z
//�@�����P�F�v�Z�����t
//�@�����Q�F�v�Z���ʓ��t�o�̓|�C���^
//�@�����R�F���Z�E���Z������t
//  �Ԓl�@�F�Ȃ�
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
//	�֐����Fvoid Wait(long time);
//	�@�\�@�F�E�G�C�g��������
//	�����@�F�E�G�C�g����b��(*100msec)
//	�Ԓl�@�F�Ȃ�
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
//	�֐����Fint set_nowtime( char *setdate
//	�@�\�@�F�����Ŏw�肳�ꂽYYYYMMDDhhmmss�ɐݒ肷��
//	�Ԓl�@�F0=����
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
	if( !btmemcmp(setdate,buf,12) ) return 0;	//���܂œ����Ȃ�Z�b�g���Ȃ�

	//�N�����̾��
	btmemcpy(buf,setdate,4);buf[4]=0;
	d_info.year = (short)Char2Num(buf);
	btmemcpy(buf,setdate+4,2);buf[2]=0;
	d_info.month = (char)Char2Num(buf);
	btmemcpy(buf,setdate+6,2);buf[2]=0;
	d_info.day = (char)Char2Num(buf);
	//�����b�̾��
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
//	�֐����Fshort htoi( char *hexstr , short len )
//	�@�\�@�F16�i�i������j��10�i(short�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
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
//	�@�\�@�F�E���̽�߰����͂���
//	�J�V�I����ڐA
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
//	�@�\�@�F�X�y�[�X��S�Ă��O��
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
//	�@�\�@�F�f�[�^����͂���
//	�����P�F���͐ݒ�\����
//	
//	�߂�l�F0=OK;SC(R)=-1,FN+CLR=-2,�󕶎���=-101
//	CASIO DT-950�̌݊��֐�(���S�݊��ł͂Ȃ��̂Œ���)
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
	char dat[2048+1];	//2048�����܂œǂ݂Ƃ��悤�Ɋg��	//2013.06.12 sanno
	short bk;
	//unsigned int len=256;
	unsigned int len=2048;	//2048�����܂œǂ݂Ƃ��悤�Ɋg��	//2013.06.12 sanno

	DSPATTR dspattr;
	INPUTSTRATTR strattr;
	BCRCONF bconf;

	LcdClear();
	set->bar_read=FALSE;

	// ��ʕ\��
	LineOut( set->title,0,1);	//Reversal display for title.�^�C�g���̂ݔ��]
	LineOut2( set->l1,1,0);
	ClineOut( set->l2,3,set->c2);
	ClineOut( set->l3,4,set->c3);
	ClineOut( set->l4,5,set->c4);
	ClineOut( set->l5,6,set->c5);
	ClineOut( set->l6,7,set->c6);
	if(btstrlen(set->l7)){
		ClineOut( set->l7,9,set->c7);
	}
	// �\�������ݒ�
	btmemset( &dspattr, 0, sizeof(DSPATTR));
	dspattr.mag = BT_MAG_NORM;
	dspattr.fold = BT_FOLD_ON;
	dspattr.tposition.x=1;
	dspattr.tposition.y=17;

	dspattr.attribute=BT_ATRB_NORM;

	// Input setting. ���͐ݒ�
	btmemset( &strattr, 0, sizeof(INPUTSTRATTR));
	strattr.inputarea=20;
	if( set->emptyOK==TRUE )strattr.mindigit=0;	//Allow empty input.�J�����͋��̏ꍇ
	else strattr.mindigit=1;
	strattr.maxdigit=len;

	//Check mode.���[�h�m�F
	if( set->mode < 0 || set->mode > 2 ) set->mode=2;

	//This mode is scanner input and key input. �o�[�R�[�h���� & �L�[����
	if( set->mode == 1 || set->mode == 2){
		// �o�[�R�[�h�ǂݎ��I�v�V�����̕ύX
		btGetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_MOMENTARY;
		btSetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_AUTO_OFF;			// �߂��p
		if( set->mode == 1 ){
			strattr.inputtype=BT_INPUT_BCR|BT_INPUT_ESCAPE_ON;	// �o�[�R�[�h����
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "  [�o�[�R�[�h����]  ",10,0);
			else LineOut( "  [Scanner Input]  ",10,0);
		}
		if( set->alpha !=1 && set->mode == 2 ){
			//BT_INPUT_ESCAPE_ON �r�����͂̒��f����
			strattr.inputtype=BT_INPUT_KEYBCR|BT_INPUT_ESCAPE_ON;// �o�[�R�[�h���� & �L�[����
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "[�o�[�R�[�h�������]",10,0);
			else LineOut( " [ Scanner & Key ]",10,0);
		}
		// Allow input alphabet .�A���t�@�x�b�gOK
		if( set->alpha==1 && set->mode==2 ) {
			strattr.inputtype |= BT_INPUT_KEYBCR_SFT_OFF|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON �r�����͂̒��f���� �V�t�g�@�\����
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "[�o�[�R�[�h�������]",10,0);
			else LineOut( " [ Scanner & Key ]",10,0);
		}

		bk=1;
		while(bk){
			// Input function.����
			btmemset( dat, 0, sizeof(dat));
			keycode = btInputString(dat,&dspattr,&strattr);

			switch(keycode){
				// �o�[�R�[�h���ǂݍ��܂ꂽ�ꍇ�ALED�_�����A�u�U�[��炵�Đ��l���͉�ʂ�\��
				case KEYCODE_TRG:
					btLed(BT_ON_ASYNC,3,0,1,BT_LED_GREEN);
					btBuzzer(BT_ON_ASYNC,1,0,1,13);
					// Copy data. �o�b�t�@�ɃR�s�[
					btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize : len);
					btSetBcrCommonOption(&bconf);
					while(btKeySense(1));		// key���b�����܂ő҂�
					// Turn on scanner flag. �o�[�R�[�h�ǂݎ��t���OON
					set->bar_read=TRUE;
					return 0;

				case KEYCODE_ENT:
					// Copy data.�o�b�t�@�ɃR�s�[
					if( !btstrlen(dat) )return -101;	//Allow input empty.�J�������񂪋��e����Ă���ꍇ
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
						// 2011.04.12 coba Enable L key when only scanner input,�o�[�R�[�honly���[�h�̎��AL��L���ɂ���
						if(set->mode==1){
							return(-101);
						}
						// 2012.04.18 okamura Enable L key when scanner and key input.t��d�l�ɂ��ް���ދy�ю���͂ł��AL��L���ɂ���
						if(set->mode==2 ) return(-101);
					}
					else{		//In English mode,L key is cancel R key is OK
						// 2011.04.12 coba Enable L key when only scanner input,�o�[�R�[�honly���[�h�̎��AL��L���ɂ���
						if(set->mode==1)return(-1);
						// 2012.04.18 okamura Enable L key when scanner and key input.t��d�l�ɂ��ް���ދy�ю���͂ł��AL��L���ɂ���
						if(set->mode==2 ) return(-1);
					}
					break;


				default:
					break;
			}
		}
	}

	// key Input mode.����
	// 2�s�ڂɕ\��
	dspattr.tposition.x=1;
	dspattr.tposition.y=17;

	// �A���t�@�x�b�gOK
	if( set->alpha==1  )
		strattr.inputtype=BT_INPUT_KEY_SFT_ON|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON �r�����͂̒��f���� �V�t�g�@�\����
	// �����̂�(�]���ʂ�)
	else	strattr.inputtype=BT_INPUT_KEY|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON �r�����͂̒��f����V�t�g�@�\�Ȃ�

	if(btstrlen(set->defdata))strattr.inputtype = strattr.inputtype | BT_INPUT_INIT_ERASE_OFF;	//2012.12.06 sanno

	if( set->emptyOK==TRUE )strattr.mindigit=0;	//�J�����͋��̏ꍇ
	else strattr.mindigit=1;

	if( g_Lang == BT_LANG_JAPANESE )
		LineOut( "   [�e���L�[����]   ",10,0);
	else LineOut( "   [ Key input ]   ",10,0);
	while(1){
		// ����
		//�����l���ݒ肳��Ă���ꍇ�͏����l��\��	//2012.12.06 sanno
		btmemset( dat, 0, sizeof(dat));

		if(btstrlen(set->defdata))		//2012.12.06 sanno
			btstrcpy(dat,set->defdata);
		
		keycode = btInputString(dat,&dspattr,&strattr);

		// ���͊���
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
				return -1;		//cancel �L�����Z��
			else		//In English mode,L key is cancel R key is OK
				return 0;
		}
		else if( keycode == KEYCODE_F1 ) return -2;		//cancel �L�����Z��
		else if( keycode == KEYCODE_F2 ) return 202;
	}
	if( !btstrlen(dat) )return -101;	//Allow input empty.�J�������񂪋��e����Ă���ꍇ

	// �o�b�t�@�ɃR�s�[
	btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize : len );

	// �J�[�\��OFF
	btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);
	LineOut("",1,0);
	return 0;

}
//---------------------------------------------------------------------------------------
//	int DataInputBt2( DINPUT_SETTING* set )
//	�@�\�@�F�f�[�^����͂���
//	�����P�F���͐ݒ�\����
//	(2012.04.24 okamura �쐬�@����O/R��蕶���̑傫�����C��������Ή�)
//	�߂�l�F0=OK;SC(R)=-1,FN+CLR=-2,�󕶎���=-101
//	CASIO DT-950�̌݊��֐�(���S�݊��ł͂Ȃ��̂Œ���)
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

	LineOut( set->title,0,1);	//�^�C�g���̂ݔ��]
	LineOut2( set->l1,1,0);
	LineOut( set->l2,3,0);
	btSetColor( RED ,WHITE, BLACK,BLACK);	// ��ʕ\��
	LineOut2( set->l4,4,0);
	btSetColor( BLACK,WHITE, BLACK,BLACK);

	LineOut( set->l5,6,0);
	LineOut( set->l6,7,0);
	if(btstrlen(set->l7)){
		LineOut( set->l7,9,0);
	}
	// �\�������ݒ�
	btmemset( &dspattr, 0, sizeof(DSPATTR));
	dspattr.mag = BT_MAG_NORM;
	dspattr.fold = BT_FOLD_ON;
	dspattr.tposition.x=1;
	dspattr.tposition.y=17;

	dspattr.attribute=BT_ATRB_NORM;

	// �A���t�@�x�b�gOK
/*	if( set->alpha==1 && set->mode==2 ) {
		strattr.inputtype |= BT_INPUT_KEY_SFT_ON|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON �r�����͂̒��f���� �V�t�g�@�\����
		LineOut( "[�o�[�R�[�h�������]",10,0);
	}
*/	// ���͐ݒ�
	btmemset( &strattr, 0, sizeof(INPUTSTRATTR));
	strattr.inputarea=20;
	if( set->emptyOK==TRUE )strattr.mindigit=0;	//�J�����͋��̏ꍇ
	else strattr.mindigit=1;
	strattr.maxdigit=len;

	//���[�h�m�F
	if( set->mode < 0 || set->mode > 2 ) set->mode=2;

	// �o�[�R�[�h���� & �L�[����
	if( set->mode == 1 || set->mode == 2){
		// �o�[�R�[�h�ǂݎ��I�v�V�����̕ύX
		btGetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_MOMENTARY;
		btSetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_AUTO_OFF;			// �߂��p
		if( set->mode == 1 ){
			strattr.inputtype=BT_INPUT_BCR|BT_INPUT_ESCAPE_ON;	// �o�[�R�[�h����
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "  [�o�[�R�[�h����]  ",10,0);
			else LineOut( "  [Scanner Input]  ",10,0);
		}
		if( set->alpha !=1 && set->mode == 2 ){
			//BT_INPUT_ESCAPE_ON �r�����͂̒��f����
			strattr.inputtype=BT_INPUT_KEYBCR|BT_INPUT_ESCAPE_ON;// �o�[�R�[�h���� & �L�[����
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "[�o�[�R�[�h�������]",10,0);
			else LineOut( " [ Scanner & Key ]",10,0);
		}
		// Allow input alphabet .�A���t�@�x�b�gOK
		if( set->alpha==1 && set->mode==2 ) {
			strattr.inputtype |= BT_INPUT_KEYBCR_SFT_OFF|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON �r�����͂̒��f���� �V�t�g�@�\����
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "[�o�[�R�[�h�������]",10,0);
			else LineOut( " [ Scanner & Key ]",10,0);
		}

		bk=1;
		while(bk){
			// ����
			btmemset( dat, 0, sizeof(dat));
			keycode = btInputString(dat,&dspattr,&strattr);
			switch(keycode){
				// �o�[�R�[�h���ǂݍ��܂ꂽ�ꍇ�ALED�_�����A�u�U�[��炵�Đ��l���͉�ʂ�\��
				case KEYCODE_TRG:
					btLed(BT_ON_ASYNC,3,0,1,BT_LED_GREEN);
					btBuzzer(BT_ON_ASYNC,1,0,1,13);
					// �o�b�t�@�ɃR�s�[
					btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize : len);
					btSetBcrCommonOption(&bconf);
					while(btKeySense(1));		// key���b�����܂ő҂�
					//�o�[�R�[�h�ǂݎ��t���OON
					set->bar_read=TRUE;
					return 0;

				case KEYCODE_ENT:
					// �o�b�t�@�ɃR�s�[
					if( !btstrlen(dat) )return -101;	//�J�������񂪋��e����Ă���ꍇ
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
				// 2011.04.12 coba �o�[�R�[�honly���[�h�̎��AL��L���ɂ���
					if(set->mode==1)return(-101);
				// 2012.04.18 okamura ����d�l�ɂ��ް���ދy�ю���͂ł��AL��L���ɂ���
					if(set->mode==2 ) return(-101);

				default:
					break;
			}
		}
	}

	// key����
	// 2�s�ڂɕ\��
	dspattr.tposition.x=1;
	dspattr.tposition.y=17;

	// �A���t�@�x�b�gOK
	if( set->alpha==1  ) 
		strattr.inputtype=BT_INPUT_KEY_SFT_ON|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON �r�����͂̒��f���� �V�t�g�@�\����
	// �����̂�(�]���ʂ�)
	else	strattr.inputtype=BT_INPUT_KEY|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON �r�����͂̒��f����V�t�g�@�\�Ȃ�
//	strattr.inputtype=BT_INPUT_KEY_SFT_ON|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON �r�����͂̒��f���� �V�t�g�@�\����
	if( set->emptyOK==TRUE )strattr.mindigit=0;	//�J�����͋��̏ꍇ
	else strattr.mindigit=1;

	if( g_Lang == BT_LANG_JAPANESE )
		LineOut( "   [�e���L�[����]   ",10,0);
	else LineOut( "   [ Key input ]   ",10,0);

	while(1){
		// ����
		btmemset( dat, 0, sizeof(dat));
		keycode = btInputString(dat,&dspattr,&strattr);
		// ���͊���
		if( keycode == KEYCODE_ENT) break;
		else if( keycode == KEYCODE_R ) return -1;		//�L�����Z��
		else if( keycode == KEYCODE_F1 ) return -2;		//�L�����Z��
		else if( keycode == KEYCODE_F2 ) return 202;		//�L�����Z��
	}
	if( !btstrlen(dat) )return -101;	//�J�������񂪋��e����Ă���ꍇ

	// �o�b�t�@�ɃR�s�[
	btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize : len );

	// �J�[�\��OFF
	btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);
	LineOut("",1,0);
	return 0;

}

//---------------------------------------------------------------------------------------
//	int DataInputBt( DINPUT_SETTING* set )
//	�@�\�@�F�f�[�^����͂���
//	�����P�F���͐ݒ�\����
//	�����Q�F�����\�����[�h�@���s�ڂ܂ł�傫�������ɂ��邩�w��(3�s�ڂ܂ł����ł��Ȃ�)
//	�߂�l�F0=OK;SC(R)=-1,FN+CLR=-2,�󕶎���=-101
//	CASIO DT-950�̌݊��֐�(���S�݊��ł͂Ȃ��̂Œ���)
//---------------------------------------------------------------------------------------
int DataInputBt3( DINPUT_SETTING *set, short col_count )
{
	short keycode;
	//char dat[256+1];
	char dat[2048+1];	//2048�����܂œǂ݂Ƃ��悤�Ɋg��	//2013.06.12 sanno
	short bk;
	//unsigned int len=256;
	unsigned int len=2048;	//2048�����܂œǂ݂Ƃ��悤�Ɋg��	//2013.06.12 sanno

	DSPATTR dspattr;
	INPUTSTRATTR strattr;
	BCRCONF bconf;

	LcdClear();
	set->bar_read=FALSE;

	// ��ʕ\��
	if(col_count == 2){
		LineOut( set->title,0,1);	//Reversal display for title.�^�C�g���̂ݔ��]
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
		LineOut( set->title,0,1);	//Reversal display for title.�^�C�g���̂ݔ��]
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
		LineOut( set->title,0,1);	//Reversal display for title.�^�C�g���̂ݔ��]
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

	// �\�������ݒ�
	btmemset( &dspattr, 0, sizeof(DSPATTR));
	dspattr.mag = BT_MAG_NORM;
	dspattr.fold = BT_FOLD_ON;
	dspattr.tposition.x=1;
	dspattr.tposition.y=17;

	dspattr.attribute=BT_ATRB_NORM;

	// Input setting. ���͐ݒ�
	btmemset( &strattr, 0, sizeof(INPUTSTRATTR));
	strattr.inputarea=20;
	if( set->emptyOK==TRUE )strattr.mindigit=0;	//Allow empty input.�J�����͋��̏ꍇ
	else strattr.mindigit=1;
	strattr.maxdigit=len;

	//Check mode.���[�h�m�F
	if( set->mode < 0 || set->mode > 2 ) set->mode=2;

	//This mode is scanner input and key input. �o�[�R�[�h���� & �L�[����
	if( set->mode == 1 || set->mode == 2){
		// �o�[�R�[�h�ǂݎ��I�v�V�����̕ύX
		btGetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_MOMENTARY;
		btSetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_AUTO_OFF;			// �߂��p
		if( set->mode == 1 ){
			strattr.inputtype=BT_INPUT_BCR|BT_INPUT_ESCAPE_ON;	// �o�[�R�[�h����
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "  [�o�[�R�[�h����]  ",10,0);
			else LineOut( "  [Scanner Input]  ",10,0);
		}
		if( set->alpha !=1 && set->mode == 2 ){
			//BT_INPUT_ESCAPE_ON �r�����͂̒��f����
			strattr.inputtype=BT_INPUT_KEYBCR|BT_INPUT_ESCAPE_ON;// �o�[�R�[�h���� & �L�[����
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "[�o�[�R�[�h�������]",10,0);
			else LineOut( " [ Scanner & Key ]",10,0);
		}
		// Allow input alphabet .�A���t�@�x�b�gOK
		if( set->alpha==1 && set->mode==2 ) {
			strattr.inputtype |= BT_INPUT_KEYBCR_SFT_OFF|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON �r�����͂̒��f���� �V�t�g�@�\����
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "[�o�[�R�[�h�������]",10,0);
			else LineOut( " [ Scanner & Key ]",10,0);
		}

		bk=1;
		while(bk){
			// Input function.����
			btmemset( dat, 0, sizeof(dat));
			keycode = btInputString(dat,&dspattr,&strattr);
			switch(keycode){
				// �o�[�R�[�h���ǂݍ��܂ꂽ�ꍇ�ALED�_�����A�u�U�[��炵�Đ��l���͉�ʂ�\��
				case KEYCODE_TRG:
					btLed(BT_ON_ASYNC,3,0,1,BT_LED_GREEN);
					btBuzzer(BT_ON_ASYNC,1,0,1,13);
					// Copy data. �o�b�t�@�ɃR�s�[
					btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize : len);
					btSetBcrCommonOption(&bconf);
					while(btKeySense(1));		// key���b�����܂ő҂�
					// Turn on scanner flag. �o�[�R�[�h�ǂݎ��t���OON
					set->bar_read=TRUE;
					return 0;

				case KEYCODE_ENT:
					// Copy data.�o�b�t�@�ɃR�s�[
					if( !btstrlen(dat) )return -101;	//Allow input empty.�J�������񂪋��e����Ă���ꍇ
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
						// 2011.04.12 coba Enable L key when only scanner input,�o�[�R�[�honly���[�h�̎��AL��L���ɂ���
						if(set->mode==1)return(-101);
						// 2012.04.18 okamura Enable L key when scanner and key input.t��d�l�ɂ��ް���ދy�ю���͂ł��AL��L���ɂ���
						if(set->mode==2 ) return(-101);
					}
					else{		//In English mode,L key is cancel R key is OK
						// 2011.04.12 coba Enable L key when only scanner input,�o�[�R�[�honly���[�h�̎��AL��L���ɂ���
						if(set->mode==1)return(-1);
						// 2012.04.18 okamura Enable L key when scanner and key input.t��d�l�ɂ��ް���ދy�ю���͂ł��AL��L���ɂ���
						if(set->mode==2 ) return(-1);
					}
					break;

				default:
					break;
			}
		}
	}

	// key Input mode.����
	// 2�s�ڂɕ\��
	dspattr.tposition.x=1;
	dspattr.tposition.y=17;

	// �A���t�@�x�b�gOK
	if( set->alpha==1  )
		strattr.inputtype=BT_INPUT_KEY_SFT_ON|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON �r�����͂̒��f���� �V�t�g�@�\����
	// �����̂�(�]���ʂ�)
	else	strattr.inputtype=BT_INPUT_KEY|BT_INPUT_ESCAPE_ON;	//BT_INPUT_ESCAPE_ON �r�����͂̒��f����V�t�g�@�\�Ȃ�

	if(btstrlen(set->defdata))strattr.inputtype = strattr.inputtype | BT_INPUT_INIT_ERASE_OFF;	//2012.12.06 sanno

	if( set->emptyOK==TRUE )strattr.mindigit=0;	//�J�����͋��̏ꍇ
	else strattr.mindigit=1;

	if( g_Lang == BT_LANG_JAPANESE )
		LineOut( "   [�e���L�[����]   ",10,0);
	else LineOut( "   [ Key input ]   ",10,0);
	while(1){
		// ����
		//�����l���ݒ肳��Ă���ꍇ�͏����l��\��	//2012.12.06 sanno
		btmemset( dat, 0, sizeof(dat));

		if(btstrlen(set->defdata))		//2012.12.06 sanno
			btstrcpy(dat,set->defdata);
		
		keycode = btInputString(dat,&dspattr,&strattr);

		// ���͊���
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
				return -1;		//cancel �L�����Z��
			else		//In English mode,L key is cancel R key is OK
				return 0;
		}
		else if( keycode == KEYCODE_F1 ) return -2;		//cancel �L�����Z��
		else if( keycode == KEYCODE_F2 ) return 202;
	}
	if( !btstrlen(dat) )return -101;	//Allow input empty.�J�������񂪋��e����Ă���ꍇ

	// �o�b�t�@�ɃR�s�[
	btmemcpy( set->buffer, dat,len > set->bufsize ? set->bufsize : len );

	// �J�[�\��OFF
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
//	�֐����Flong Char2Num( char *pString )
//	�@�\�@�F������𐮐��ɕϊ����� atoi�̕ς��
//	�����P�F�ϊ���������
//	�Ԓl�@�F�ϊ���̐��l
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
//	�֐����Fshort Char2Dec( char *pString , DECIMAL* pDecimal )
//	�@�\�@�F������𕂓������_���ɕϊ����� atof�̑���
//	atoi�ɂ��Ă�Keyence�W���֐���btchar2num���g���ĉ�����
//	�����P�F�ϊ���������
//	�����Q�F�o�͗p�f�[�^�|�C���^(DECIMAL�^�j
//	�Ԓl�@�F0�F�����A����ȊO�F���s
//	���ӁI�I DECIMAL�^�́uDECIMAL�͏����l��1000�{�����l�ň����܂��B�v�Ƃ̎��Ȃ̂Œ��ӁI�I
//-----------------------------------------------------------------------------
short Char2Dec( char *pString , DECIMAL* pDecimal )
{
	char buf[50];
	short inlen;			// ���͕�����
	short stpos = 0;		// �ϊ��J�n�ʒu(�}�C�i�X���t���ꍇ��1�ɂȂ�)
	short seppos = 0;		// �������ʒu
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
			} else if ( i == 0 && (buf[i] == '-' || buf[i] == ' ') ) {	// ������
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
//	�֐����Fint MasterFind( char *keyname,char *data )
//	�@�\�@�F�����Ŏw�肳�ꂽ�f�[�^���}�X�^�t�@�C�����猟������
//	�����P�F��������L�[����
//	�����Q�F�o�͗p�f�[�^�|�C���^
//	�Ԓl�@�F0:�f�[�^����A����ȊO�f�[�^�Ȃ�
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

	if( !btstrlen(keyname) ) return -100;		//�L�[�w��Ȃ��̓_��

	btmemset(&index,0,sizeof(index));
	index.datatype = 1;	//0:Fix record length.�Œ蒷�A1:not fix record length�ϒ�
	index.separator=',';//The sepalater.�Z�p���[�^
	index.fieldcnt=2;	//Field qty.( key field, data field)�t�B�[���h����2�i�L�[���A�f�[�^���j

	//File open.�t�@�C���I�[�v��
	mstno = btMasterOpen( MTRFILE_NAME,MTRIDXFILE_NAME, NULL, &index);
	if(mstno<0) return -1;	//�t�@�C�����J���Ȃ�==�ُ�

	//�����������
	btmemset(&find,0,sizeof(find));
	find.ope=1;	// ���Z�q(1:AND�A2:OR�j
	find.searchdata[0]=keyname;			// ��P�t�B�[���h�Ɍ����������
	find.incdeldata = BT_OFF; // �폜�f�[�^���܂߂Č������s����(BT_ON:�܂�, BT_OFF:�܂܂Ȃ�)
	
	recno = btMasterGetFirst(mstno, &find);
	if( recno < 0 ){	//Data does not exists.�f�[�^�Ȃ�
		btMasterClose(mstno);
		return -2;
	}

	btmemset(buf,0,sizeof(buf));
	reclen = btMasterRead(mstno,recno,BT_OFF,buf);
	if( reclen < 0 ){
		btMasterClose(mstno);
		return -3;	//�ǂݏo���G���[
	}
	btmemcpy(data,buf+btstrlen(keyname)+1,btstrlen(buf)-(btstrlen(keyname)+1));

	//close
	btMasterClose(mstno);
	return 0;
}

//-----------------------------------------------------------------------------
//	�֐����Fint MasterUpd( char *keyname,char *data )
//	�@�\�@�F�����Ŏw�肳�ꂽ�f�[�^���}�X�^�t�@�C���ɒǉ��E�X�V����
//	�����P�F��������L�[����
//	�����Q�F�o�͗p�f�[�^�|�C���^
//	�Ԓl�@�F0:�f�[�^����A����ȊO�f�[�^�Ȃ�
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
	index.datatype = 1;	//0:�Œ蒷�A1:�ϒ�
	index.separator=',';//�Z�p���[�^
	index.fieldcnt=2;	//�t�B�[���h����2�i�L�[���A�f�[�^���j

	//Open Actual file.�{�ԃt�@�C���I�[�v��
	mstno = btMasterOpen( MTRFILE_NAME,MTRIDXFILE_NAME, NULL, &index);
	if(mstno<0) return -1;	//�t�@�C�����J���Ȃ�==�ُ�
	//Close Actual file.�{�ԃt�@�C����U�N���[�Y
	btMasterClose(mstno);

	//Delete temporary files.�ҏW�p�t�@�C���Q����U�폜
	btRemove(MODMTRFILE_NAME);
	btRemove(MODMTRIDXFILE_NAME);
	btRemove(MODMTRADDFILE_NAME);
	btRemove(MODMTRSYSFILE_NAME);

	//Data copy to temporary file from actual file.�{�ԗp�t�@�C�����ҏW�p�t�@�C���փR�s�[
	if( btCopy(MTRFILE_NAME, MODMTRFILE_NAME, BT_ON) ) return -1;//	�R�s�[���s=�ُ�

	//Open templrary file.�ҏW�p�t�@�C���I�[�v��
	mstno = btMasterOpen( MODMTRFILE_NAME,MODMTRIDXFILE_NAME, MODMTRADDFILE_NAME, &index);
	if(mstno<0) return -1;	//�t�@�C�����J���Ȃ�==�ُ�

	//Delete actual files.�{�ԗp�̃t�@�C���Q���폜
	btRemove(MTRFILE_NAME);
	btRemove(MTRIDXFILE_NAME);
	btRemove(MTRSYSFILE_NAME);


	//�����������
	btmemset(&find,0,sizeof(find));
	find.ope=1;	// ���Z�q(1:AND�A2:OR�j
	find.searchdata[0]=keyname;			// ��P�t�B�[���h�Ɍ����������
	find.incdeldata = BT_OFF; // �폜�f�[�^���܂߂Č������s����(BT_ON:�܂�, BT_OFF:�܂܂Ȃ�)

	recno = btMasterGetFirst(mstno, &find);
	if( recno >= 0 )upd = 1;	//�f�[�^����=�X�V���[�h

	//�Z�p���[�^���J���}�ɂ��āA�f�[�^�������
	btsprintf(buf,"%s,%s",keyname,data);
	if( upd ) ret = btMasterModify(mstno,recno,buf);
	else ret = (short)btMasterAdd(mstno,buf);
	if( ret<0 ){
		btMasterClose(mstno);
		return -1;		//Insert error�ǉ����s�I�I
	}
	//Restore master file.�}�X�^�č\�z
	btMasterRestore(mstno,MTRFILE_NAME);
	//close
	btMasterClose(mstno);

	//Delete temporary files.�ҏW�p�t�@�C���Q�폜
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
//	����LAN�̏�����
//	�P�D�{�̂Őݒ肳��Ă���z�X�g�A�h���X��ǂݍ���
//	�Q�D����ł��Ȃ�������}�X�^�t�@�C������ǂݍ���
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
			LineOut("IP�A�h���X���ݒ�ł�",2,0);
			LineOut("�ݒ肵�ĉ�����",4,0);
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
			LineOut("�߰Ĕԍ����ݒ�ł�",2,0);
			LineOut("�ݒ肵�ĉ�����",4,0);
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
//	IP�ƃ|�[�g�̐ݒ�
//	�P�D�{�̂Őݒ肳��Ă���z�X�g�A�h���X��ǂݍ���
//	�Q�D����ł��Ȃ�������}�X�^�t�@�C������ǂݍ���
//=============================================================================
//	Function name:int WLAN_setting(void)
//  Outline      :Input server address and TCP port No.
//=============================================================================
int WLAN_setting(void)
{
	char buf1[40],buf2[40],buf3[40],buf4[40];
	if( g_Lang == BT_LANG_JAPANESE ){
		btstrcpy(buf1,"server_addr\0");
		btstrcpy(buf2,"���ް���ڽ�����\0");
		btstrcpy(buf3,"port_no\0");
		btstrcpy(buf4,"�߰Ĕԍ������\0");
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

	//IP�ݒ�
	//Read setting from master file.�}�X�^�[�t�@�C������ǂݏo��
	btmemset(addr,0,sizeof(addr));
	MasterFind(keyname_ip,addr);

	LcdClear();

	btmemset(&st,0,sizeof(st));
	st.mode=0;
	if( g_Lang == BT_LANG_JAPANESE ){
		btstrcpy(st.title,"IP�A�h���X����");
		if( btstrlen(addr) )btstrcpy(st.l5,"�ύX�Ȃ��̏ꍇ��ENT");
	}
	else{
		btstrcpy(st.title,"Input IP Address.");
		if( btstrlen(addr) )btstrcpy(st.l5,"Don't change -> ENT");
	}
	btsprintf(st.l1,"%s",dspname_ip);	//������\��
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
		//�`�F�b�N
		cnt=0;
		for(i=0;i<btstrlen(dat);i++){
			if( dat[i]=='.' )cnt++;
		}
		if( cnt != 3 ){
			if( g_Lang == BT_LANG_JAPANESE )
				DspMsg("  �������G���[������",0,"IP�A�h���X��","�s���ł�(1)",dat,"","",NULL);
			else
				DspMsg(" ***** Error *****",0,"IP address is ","not correct.(1)",dat,"","",NULL);
				
			continue;
		}
		if( btSocketInet_addr(dat)==BTSOCK_INADDR_NONE ){
			if( g_Lang == BT_LANG_JAPANESE )
				DspMsg("  �������G���[������",0,"IP�A�h���X��","�s���ł�(2)",dat,"","",NULL);
			else
				DspMsg(" ***** Error *****",0,"IP address is ","not correct.(2)",dat,"","",NULL);
			continue;
		}
		else{
			MasterUpd(keyname_ip,dat);
			break;
		}
	}
	//TCP Port No. setting �|�[�g�ݒ�
	btmemset(port,0,sizeof(port));
	MasterFind(keyname_port,port);

	LcdClear();

	btmemset(&st,0,sizeof(st));
	st.mode=0;
	if( g_Lang == BT_LANG_JAPANESE ){
		btstrcpy(st.title,"�|�[�g�ԍ�����");
		if(Char2Num(port)) btstrcpy(st.l5,"�ύX�Ȃ��̏ꍇ��ENT");
	}
	else{
		btstrcpy(st.title,"Input Port No.");
		if(Char2Num(port)) btstrcpy(st.l5,"Don't change -> ENT");
	}
	btsprintf(st.l1,"%s",dspname_port);//������\��
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
				DspMsg("  �������G���[������",0,"�|�[�g�ԍ���","�s���ł�(1)",dat,"","",NULL);
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

	//IP�ݒ�
	//Read setting from master file.�}�X�^�[�t�@�C������ǂݏo��
	btmemset(addr,0,sizeof(addr));
	MasterFind(keyname_ip,addr);

	LcdClear();

	btmemset(&st,0,sizeof(st));
	st.mode=0;
	if( g_Lang == BT_LANG_JAPANESE ){
		btstrcpy(st.title,"IP�A�h���X����");
		if( btstrlen(addr) )btstrcpy(st.l5,"�ύX�Ȃ��̏ꍇ��ENT");
	}
	else{
		btstrcpy(st.title,"Input IP Address.");
		if( btstrlen(addr) )btstrcpy(st.l5,"Don't change -> ENT");
	}
	btsprintf(st.l1,"%s",dspname_ip);	//������\��
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
		//�`�F�b�N
		cnt=0;
		for(i=0;i<btstrlen(dat);i++){
			if( dat[i]=='.' )cnt++;
		}
		if( cnt != 3 ){
			if( g_Lang == BT_LANG_JAPANESE )
				DspMsg("  �������G���[������",0,"IP�A�h���X��","�s���ł�(1)",dat,"","",NULL);
			else
				DspMsg(" ***** Error *****",0,"IP address is ","not correct.(1)",dat,"","",NULL);
				
			continue;
		}
		if( btSocketInet_addr(dat)==BTSOCK_INADDR_NONE ){
			if( g_Lang == BT_LANG_JAPANESE )
				DspMsg("  �������G���[������",0,"IP�A�h���X��","�s���ł�(2)",dat,"","",NULL);
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
//	�O���[�o���ϐ��̃T�[�o�ݒ��ύX����
//	2007.03.17 ������NULLor0���w�肵���Ƃ��͌��s�ݒ�������p��
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
//	Casio�݊��̂���
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
//	�f�[�^�𑗐M���A���ʂ���M����
//	�����P�F���M�f�[�^
//	�����Q�F���M�f�[�^�T�C�Y
//	�����R�F��M�o�b�t�@�i�o�͗p�j
//	�����S�F��M�o�b�t�@�T�C�Y
//	�����T�F��M�f�[�^�����i�o�͗p�j
//	�����U�F
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
	int chk,dlen;		// 2013.09.24 coba ��M�f�[�^�����`�F�b�N���Ă��ׂĎ�M����
	long err;

	if( !btstrlen(g_HostAddr) ){
		LcdClear();
		if( g_Lang == BT_LANG_JAPANESE ){
			LineOut("IP�A�h���X���ݒ�ł�",2,0);
			LineOut("�ݒ肵�ĉ�����",4,0);
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
			LineOut("�|�[�g�ԍ����ݒ�ł�",2,0);
			LineOut("�ݒ肵�ĉ�����",4,0);
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
		LineOut4("  �ʐM��  ",5,0);
	}
	else LineOut("  Connecting...  ",5,0);

	//The preparation of headre data. �f�[�^�̃w�b�_�[����
	btsprintf(buffer,"%05ld",size);
	btmemcpy(buffer+5,data,size);

	// RF�|�[�g���I�[�v������(�u���b�L���O���[�h�FBT_ON_ASYNC)
/*	ret = btNetworkOpen(BT_NETWORK_WLAN_INFRA, BT_ON_SYNC);
	if(ret == BT_RF_API_SET_ERR){
		btsprintf(ErrMsg,"�����ڑ��Ɏ��s���܂����B(%d)",ret);
		btMessageBox(ErrMsg,BT_MB_CONFONLY);
		return -1;
	}
*/
	// Open socket. �\�P�b�g���I�[�v������
	socketClient = btSocketOpen( BTSOCK_AF_INET, BTSOCK_STREAM, 0 );
	if( socketClient <= 0 ){
#ifdef WLAN_DEBUG
		btsprintf(ErrMsg,"�\�P�b�g�̃I�[�v���Ɏ��s���܂����B(%d)",socketClient);
		btMessageBox(ErrMsg,BT_MB_CONFONLY);
#endif
		return -2;
	}
	btsockaddr_in.sin_family = BTSOCK_AF_INET;
	btsockaddr_in.sin_port = btSocketHtons( (unsigned short)g_PortNo );
	btsockaddr_in.sin_addr.S_un.S_addr = btSocketInet_addr(g_HostAddr);

	// Connect to Socket.�\�P�b�g��ڑ�����
	ret = btSocketConnect(socketClient, (BTSOCKADDR*)&btsockaddr_in, sizeof(BTSOCKADDR) );
	if( ret != BTSOCK_SOCKET_OK ){
#ifdef WLAN_DEBUG
		btsprintf(ErrMsg,"�\�P�b�g�̐ڑ��Ɏ��s���܂����",ret);
		btMessageBox(ErrMsg,BT_MB_CONFONLY);
#endif
		// �\�P�b�g�����
		btSocketShutdown(socketClient,BTSOCK_SD_BOTH);
		btSocketClose(socketClient);
		// RF�|�[�g�����
//		btNetworkClose(BT_NETWORK_WLAN_INFRA);
		return -3;
	}

	// Wait the connection .�ڑ����ł���܂ő҂����킹��
	btFD_ZERO( &writefds );
	btFD_SET(socketClient, &writefds);
	timeval.tv_sec=send_to;	//Time out time. �^�C���A�E�g����
	timeval.tv_usec=0;
	ret = btSocketSelect( 0, 0, &writefds, 0, &timeval );
	//Time out.�^�C���A�E�g
	if( !ret ){
		CloseSocket(socketClient);
//		btNetworkClose(BT_NETWORK_WLAN_INFRA);
#ifdef WLAN_DEBUG
		btMessageBox("�T�[�o����̉���������܂���",BT_MB_CONFONLY);
#endif
		return -4;
	}
	else if( ret == BTSOCK_SOCKET_ERROR ){
#ifdef WLAN_DEBUG
		btsprintf(ErrMsg,"�T�[�o�ւ̐ڑ��Ɏ��s���܂����(%d)",btSocketGetLastError() );
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

	// Sending operation. ���M
	len = size+5;
	len2 = 0;
	wait_cnt=0;
	do{
		ret = btSocketSendto(socketClient, &buffer[len2],len-len2, 0 ,(BTSOCKADDR*)&btsockaddr_in ,sizeof(BTSOCKADDR));
//		ret = btSocketSend(socketClient, &buffer[len2],len-len2, 0);
		if( ret < 0 ){
			// �������������Ă��Ȃ��ꍇ������̂ł��̏ꍇ�͌p�� 2013.09.24 coba
			err=btSocketGetLastError();
			if( err == BTSOCK_EWOULDBLOCK){
				ret=0;
			}
			else{
				CloseSocket(socketClient);
				return(-7);
			}
		}
		// The length of alredy sent data. ���M�ς݃o�C�g��
		len2 += ret;
		if( len !=len2 ){
			if( ++wait_cnt > 10 ){
				CloseSocket(socketClient);
//				btNetworkClose(BT_NETWORK_WLAN_INFRA);
				return -8;										// �f�[�^���M���s
			}
			btWait(1,KEY_CLR);//1=100msec
		}
		else wait_cnt=0;

	}while(len !=len2);
	// Receive operation.
	// �ڑ����ł���܂ő҂����킹��
	btFD_ZERO( &writefds );
	btFD_SET(socketClient, &writefds);
    timeval.tv_sec  = recv_to;  // Time out time of receiving.��M�^�C���A�E�g
    timeval.tv_usec = 0;
	ret = btSocketSelect( 0, 0, &writefds, 0, &timeval );
	if( !ret ){
		CloseSocket(socketClient);
//		btNetworkClose(BT_NETWORK_WLAN_INFRA);
#ifdef WLAN_DEBUG
		btMessageBox("�T�[�o����̉���������܂���",BT_MB_CONFONLY);
#endif
		return -9;
	}
	else if( ret == BTSOCK_SOCKET_ERROR ){
#ifdef WLAN_DEBUG
		btsprintf(ErrMsg,"�T�[�o�ւ̐ڑ��Ɏ��s���܂����(%d)",btSocketGetLastError() );
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

	//Receiving...�f�[�^��M
	wait_cnt=0;
	len=0;
	rcv_sock_len=sizeof(BTSOCKADDR);
	btmemset(buffer,0,sizeof(buffer));
	chk=0;		// ���M�f�[�^���`�F�b�N���A�K�v���҂� 2013.09.24
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
		if( wait_cnt > recv_to * 10 ){	//recv_to�͕b�Ȃ̂�10��������
#ifdef WLAN_DEBUG
			btMessageBox("�f�[�^����M�ł��܂���",BT_MB_CONFONLY);
#endif
			btSocketShutdown(socketClient,BTSOCK_SD_BOTH);
			btSocketClose(socketClient);
//			btNetworkClose(BT_NETWORK_WLAN_INFRA);
			return -12;
		}
		//Check received data rength. ��M�f�[�^���𒲂ׂ� 2013.09.24
		if( len >=5 && !chk ){
			btmemcpy( buf,buffer,5),buf[5]=0;
			dlen=atoi_bt(buf);
			chk=1;
		}
		// �w��f�[�^������M������I��
		if( chk && len-5 >=dlen ) break;

	}
	// �o��
	//2013.09.27 matsuda following part is not necessary because checking received data in above process.
//	len -=5;	// ���M�o�C�g���̃w�b�_������
//	if(len< 1 ) return(-13);
	//2008.04.25 ���� �ǉ�
//	if( len > rsize-1) len = rsize-1;
	btmemcpy( rcv, buffer+5, dlen );
	buffer[ dlen ]=0;
	*rcvlen=dlen;

	// Close socket.�\�P�b�g�����
	btSocketShutdown(socketClient,BTSOCK_SD_BOTH);
	btSocketClose(socketClient);
	// RF�|�[�g�����
//	btNetworkClose(BT_NETWORK_WLAN_INFRA);
	return 0;
}
// ******************************************************************************
void CloseSocket( long socketno )
//	����		:	�\�P�b�g����܂�
//  Outline     :   Close the socket.
// ******************************************************************************
{
	// �\�P�b�g�����
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
		//���C����ʕ\��
		LcdClear();

		keycode=StartDisplay("BT Program Updater","BT Program updater","","","","tcp port=35963","","L:Exec F3:IP setting");
		switch(keycode){
			//2013.10.15 matsuda
			case KEYCODE_F1:
				return -1;
				break;

			//IP�ݒ�
			case KEYCODE_F3:
				WLAN_settingIP("prg_updater_addr","Input IP");
				break;

			//���s
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

	// �͂�/�������m�F
	btmemset( &key,0,sizeof(ENABLE_WAITKEYS));
	key.keyL=TRUE;
	key.keyR=TRUE;

	//�@PC����X�V�p�̖��O��Ⴄ
	rcvlen=0;
	btmemset(dt_name,0,sizeof(dt_name));
	btsprintf(sendstr,"R0");//��۸��і��̗v��
	ret=WLAN_sendBt(sendstr,btstrlen(sendstr),dt_name,sizeof(dt_name)-1,&rcvlen,3,3);
	if(ret){
		DspMsg("BT update",3,"Can not connect","to server","","","",&key);
		return(-1);
	}
	// �G���[����
	if( !btmemcmp( dt_name,"NG",2)){
		DspMsg("BT update",3,"PC return error","Stop operation","","","",&key);
		return(-2);
	}
	if( !btstrlen(dt_name) ) return -1;

	//�m�F
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

		//End��E
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
//	�֐����Fvoid LcdOut( void *line1, void *line2, void *line3)
//	�@�\�@�F��ʕ\���i�\��only�j
//	�����P�F�\���p�̍\����
//		���\���\�ȕ������́A�P�s������20�o�C�g�ł��B�܂��A�����̍Ō��
//		�@�K��Null��t�����ĉ������B
//-----------------------------------------------------------------------------
void LcdOut( BT900_LCD* lcd)
{
	int i;
	DSPATTR datr;

	btClear();

	// �\�������ݒ�
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
//	�֐����Fvoid LcdOutLeg( void *line1, void *line2, void *line3)
//	�@�\�@�F��ʕ\���i�\��only�j
//	�����P�F�\���p�̍\����
//		���\���\�ȕ������́A�P�s������20�o�C�g�ł��B�܂��A�����̍Ō��
//		�@�K��Null��t�����ĉ������B
//	SMG�̈ڐA�̂��߂ɒǉ� 2011.04.24 matsuda
//	Legacy��Leg
//-----------------------------------------------------------------------------
void LcdOutLeg( char* msg1,char *msg2,char *msg3)
{
//	int i;
//	DSPATTR datr;

	btClear();
	
	//2011.11.08 matsuda ���܂�ɂ��\�������������߉��L����ύX
	LineOut2(msg1,0,0);
	LineOut2(msg2,2,0);
	LineOut2(msg3,4,0);

/*
	// �\�������ݒ�
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
//	�֐����Fshort BarRead(char *databuf, short len,
//								char *name, char *msg, short mode);
// 	�@�\�@�F�o�[�R�[�h�̓ǂݍ��݁B���͂����f�[�^�𕶎���ŕԂ��B
//			�w�肵�������ȓ��̃o�[�R�[�h�ł���ΑS�ēǂݍ��ށI
//			��ʕ\���͉������Ȃ��̂ŃA�v�����ł���
//	�����P�Fchar *databuf �擾������i�[�|�C���^(�o��)�A�ő包���K�v�I
//	�����Q�Fshort len	�@�ǂݎ��\�ȍő包�i'*'�͊܂܂Ȃ��j
//	�����R�Fshort mode �@ �ް�����Ӱ��	0=�I���@�\�Ȃ�
//						  F1=1,F2=2,F3=4�Ƃ��āA�I������or�Ŏw��
//	�Ԓl�@�F0=����over�@:'-1=C'�L�[ -11=F1�L�[�ŏI���@:����ȊO=�ǂݎ��������
//			-12=F2�L�[�ŏI��	:-13=F3�ŏI��
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


	// L�L�[��CLR�L�[�Ɋ��蓖�Ă�
	btGetKeyFunction(&keyfunc);
	keyfunc.L = KEYCODE_CLR;
	btSetKeyFunction(&keyfunc);

	// �ǂݎ�苖�R�[�h�̐ݒ�
	bkind = 0;		//�ر
	bkind =bkind | 0x01;		// code39
	bkind =bkind | 0x03;		// code128
	bkind =bkind | 0x0a;		// QR code
//	bkind = 0;		//�ر
	// �o�[�R�[�h�ǂݎ��I�v�V�����̕ύX
	if( btSetBcrProperty(BT_PROP_ENABLE_SYMBOLS,&bkind)!= NO_ERR ){
		btMessageBox("�o�[�R�[�h�ǂݎ�胂�[�h�ݒ�G���[",BT_MB_CONFONLY);
	}
	// �g���K���[�h�̕ύX
	stat = BT_BCR_MOMENTARY;
	if( btSetBcrProperty( BT_PROP_TRIGGER_MODE,&stat ) != NO_ERR ){
		btMessageBox("�g���K���[�h�ݒ�G���[",BT_MB_CONFONLY);
	}

	// �\�������ݒ�
	btmemset( &dspattr, 0, sizeof(DSPATTR));
	dspattr.mag = BT_MAG_NORM;
	dspattr.fold = BT_FOLD_ON;
	// ��ʊO�ŕ\��
	dspattr.tposition.x=1;
	dspattr.tposition.y=3;
	dspattr.attribute=BT_ATRB_NORM;


	// ���͐ݒ�
	btmemset( &strattr, 0, sizeof(INPUTSTRATTR));
	strattr.inputarea=20;
	if( len > 512 ) len=512;
	strattr.mindigit=0;
	strattr.maxdigit=len;
	strattr.inputtype=BT_INPUT_BCR;

	ret =1;
	while(ret==1){
		// ����
		btmemset( dat, 0, sizeof(dat));
		keycode = btInputString(dat,&dspattr,&strattr);
		switch(keycode){
			// �o�[�R�[�h���ǂݍ��܂ꂽ�ꍇ�ALED�_�����A�u�U�[��炵�Đ��l���͉�ʂ�\��
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

	// �J�[�\��OFF
	btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);
	LineOut("",1,0);

	// �o�[�R�[�h�ǂݎ��I�v�V�����̕ύX
	bconf.mode = BT_BCR_AUTO_OFF;
	btSetBcrCommonOption(&bconf);

	// �o�b�t�@�ɃR�s�[
	if(!ret) btmemcpy( databuf, dat,btstrlen(dat) > len ? len :btstrlen(dat));
	while(btKeySense(1));		// key���b�����܂ő҂�
	return(ret);
}
//-----------------------------------------------------------------------------
//	�֐����Fshort KeyInput(char *databuf,char *name,
//										char *msg,short len,char moji );
//	�@�\�@�F�ް����L�[���͂Ŏ擾����B
//			��ʕ\���͉������Ȃ��̂ŃA�v�����ł��邱��
//	�����Q�Fchar *databuf  �f�[�^�i�[�|�C���^(��4����+1�ȏ�m�ۂ̂���)
//	�����R�Fshort len  �ő���͉\�������i20�ȏ��20�ɂȂ�܂�)
//	�Ԓl�@�F�L�[���͂��ꂽ�ް���('C'�L�[��:: -1)
//-----------------------------------------------------------------------------
int KeyInput( char *databuf, short len)
{
	short keycode;
	char dat[50+1];

	DSPATTR dspattr;
	INPUTSTRATTR strattr;

	// �\�������ݒ�
	btmemset( &dspattr, 0, sizeof(DSPATTR));
	dspattr.mag = BT_MAG_NORM;
	dspattr.fold = BT_FOLD_ON;

	// ���͐ݒ�
	btmemset( &strattr, 0, sizeof(INPUTSTRATTR));
	strattr.inputarea=20;
	if( len > 20 || len < 1) len=20;
	strattr.mindigit=0;
	strattr.maxdigit=len;
	dspattr.attribute=BT_ATRB_NORM;

	// key����
	// 2�s�ڂɕ\��
	dspattr.tposition.x=1;
	dspattr.tposition.y=3;
	strattr.inputtype=BT_INPUT_KEY;
	while(1){
		// ����
		btKeyBufClear();
		btmemset( dat, 0, sizeof(dat));
		keycode = btInputString(dat,&dspattr,&strattr);
		// ���͊���
		if( keycode == KEYCODE_ENT ) break;
		if( keycode == KEYCODE_F1 ){
			// �J�[�\��OFF
			LineOut("",1,0);
			btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);
			return-1;
		}
	}

	// �J�[�\��OFF
	LineOut("",1,0);
	btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);

	// �o�b�t�@�ɃR�s�[
	btmemcpy( databuf, dat,btstrlen(dat));
	while(btKeySense(1));		// key���b�����܂ő҂�
	return((int)btstrlen(dat));

}
//-----------------------------------------------------------------------------
//	�֐����Fshort DataInput(short mode, char *databuf, 
//								char *name, char *msg, short len);
// 	�@�\�@�F�o�[�R�[�h�̓ǂݍ���or�f�[�^�̎���́B���͂����f�[�^�𕶎���ŕԂ��B
//	�����P�Fshort mode �@�ް�����Ӱ��
//								1=�ް���ނ̂�
//								2=����͂̂�
//								3=����
//								4=�ް���ނ̂�/�I���@�\���E�\����"[F1]��ݾ�"
//	�����Q�Fchar *databuf �擾������i�[�|�C���^(�o��)
//	�����R�Fchar *name  ���͍��ږ�(1�s�ڂɕ\�����܂�)
//	�����S�Fchar *msg 	���b�Z�[�W(3�s�ڂɕ\�����܂�)
//	�����T�Fshort len	���͉\�ȍő啶�����B�ް���ނ̏ꍇ�͂���ȊO�̒�����NG
//	�Ԓl�@�F-1=�����I���@:0=����@:1=�I�� -11=F1�I��
//	�f�B�X�v���C��5-7�s�̓N���A����܂���
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

	// ��ʕ\��
	LineOut2( name,0,0);
	LineOut( msg,4,0);

	// �\�������ݒ�
	btmemset( &dspattr, 0, sizeof(DSPATTR));
	dspattr.mag = BT_MAG_NORM;
	dspattr.fold = BT_FOLD_ON;
	dspattr.tposition.x=1;
	dspattr.tposition.y=20;
	dspattr.attribute=BT_ATRB_NORM;

	// ���͐ݒ�
	btmemset( &strattr, 0, sizeof(INPUTSTRATTR));
	strattr.inputarea=20;
	if( len > 50 ) len=50;
//	strattr.mindigit=len;
	strattr.mindigit=1;
	strattr.maxdigit=len;

	// �o�[�R�[�h���� & �L�[����
	if( mode != 2 ){
		// �o�[�R�[�h�ǂݎ��I�v�V�����̕ύX
		btGetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_MOMENTARY;
		btSetBcrCommonOption(&bconf);
		bconf.mode = BT_BCR_AUTO_OFF;			// �߂��p
		if( mode == 1 )	strattr.inputtype=BT_INPUT_BCR;		// �o�[�R�[�h���� & �L�[����
		if( mode == 3 )	strattr.inputtype=BT_INPUT_KEYBCR;		// �o�[�R�[�h���� & �L�[����
		bk=1;
	//	if( mode == 3 )	LineOut( "[�k]   �L�[����",8,0);
		if( mode == 4 ){
			if( g_Lang == BT_LANG_JAPANESE )
				LineOut( "[�e�P] �L�����Z��",8,0);
			else LineOut( "[F1] CANCEL",8,0);
		}
		while(bk){
			// ����
			btmemset( dat, 0, sizeof(dat));
			keycode = btInputString(dat,&dspattr,&strattr);
			switch(keycode){
				// �o�[�R�[�h���ǂݍ��܂ꂽ�ꍇ�ALED�_�����A�u�U�[��炵�Đ��l���͉�ʂ�\��
				case KEYCODE_TRG:
					//btVibration(BT_ON_ASYNC,4,0,1);
					btLed(BT_ON_ASYNC,3,0,1,BT_LED_GREEN);
					btBuzzer(BT_ON_ASYNC,1,0,1,13);
					// �o�b�t�@�ɃR�s�[
					btmemcpy( databuf, dat,len);
					btSetBcrCommonOption(&bconf);
					while(btKeySense(1));		// key���b�����܂ő҂�
					return 0;

				case KEYCODE_ENT:
					// �o�b�t�@�ɃR�s�[
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
				// F3��OK�ɂ��� 2015.10.22 coba
				case KEYCODE_F3:
					return -4;
					break;

				default:
					break;
			}
		}
	}

	// key����
	// 2�s�ڂɕ\��
	dspattr.tposition.x=1;
	dspattr.tposition.y=5;
	strattr.inputtype=BT_INPUT_KEY;
	strattr.mindigit=1;
//	LineOut( "01234567890123456789",8,0);
	if( g_Lang == BT_LANG_JAPANESE )
		LineOut( "   [�e���L�[����]   ",7,0);
	else LineOut( "    [KEY INPUT]   ",7,0);
	LineOut( "[ENT] OK    [C] Back",8,0);
	while(1){
		// ����
		btmemset( dat, 0, sizeof(dat));
		keycode = btInputString(dat,&dspattr,&strattr);
		// ���͊���
		if( keycode == KEYCODE_ENT ) break;
	}

	// �o�b�t�@�ɃR�s�[
	btmemcpy( databuf, dat,len);

	// �J�[�\��OFF
	btCursor(BT_CURSOR_HIDE,0,BT_MAG_NORM);
	LineOut("",1,0);
	return 0;
}
//-----------------------------------------------------------------------------
//	�֐����Fshort FuncSel(short mode)
//	�@�\�@�F�t�@���N�V�����L�[�̑I����́B
//	�����@�F����Ӱ��
//					 0="�����L�[�������Ă�������"�̂Ƃ��B
//					 1="[L]OK [R]NG"�̂Ƃ��B
//					 2="L=OK R=NG ENT=�ǉ�"�̂Ƃ��B
//					 3="[L]YES [R]NO" �̂Ƃ�
//					 4="�����L�[�������Ă�������"�̂Ƃ��B
//						�B���L�[����
//	�Ԓl�@�F�����ꂽ�L�[�̎�ށi0=L :-1=R :-3=ENT :-11=F1 :-13=F3)
//-----------------------------------------------------------------------------
short FuncSel(short mode)
{
	long key;
	char keycode;
	short ret;

	btKeyBufClear();

	// mode0�͕�
	if(mode==4){
		while(btKeySense(1));		// key���b�����܂ő҂�
	    btWait(1,0);
		btKeyBufClear();
		if(g_Lang==BT_LANG_JAPANESE){
			LineOut("�������������ĉ�����",10,0);
		}
		else LineOut("    PUSH ANY KEY",10,0);
		ret=1;
	
		while(ret==1){
		    btWait(1,0);
			// �L�[���͂�҂�
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
		while(btKeySense(1));		// key���b�����܂ő҂�
		return(ret);
	}

	// mode !=0
//	if( mode == 2) LineOut("01234567890123456789",8,0);
	if( mode == 2) LineOut(" L:OK  R:NG ENT:�ǉ�",10,0);
	else if( mode == 3) LineOut(" [�k] YES  [�q] NO",10,0);
	else if (mode ==0){
		if(g_Lang==BT_LANG_JAPANESE){
			LineOut("�������������ĉ�����",10,0);
		}
		else{
			LineOut("    PUSH ANY KEY",10,0);
		}
	}
	else LineOut("  [�k] OK  [�q] NG",10,0);
	while(1){
		// �L�[���͂�҂�
		btKeyBufClear();
		key = btKeyWait(0);
		if(!mode)return(0);

		// �_���L�[�ɕϊ�����
		btTbl2Code(key,&keycode);

		// L�L�[
		if( keycode == KEYCODE_L || keycode == KEYCODE_ENT){
			ret=0;
			break;
		}
		// R�L�[
		else if( keycode == KEYCODE_R){
			ret=-1;
			break;
		}
		// ENT�L�[
		else if( keycode == KEYCODE_ENT && mode == 2){
			ret=-3;
			break;
		}
	}

	return(ret);
}
//-----------------------------------------------------------------------------
//	�֐����Fvoid GetNowtime( char *data )
//	�@�\�@�F���݂̓��t��YYYYMMDDHHMMSS�̌`�Ŏ擾����B
//�@�����@�F�擾�f�[�^�i�[�|�C���^�i�o�́j *14�o�C�g�ȏ�m�ەK�v�I
//  �Ԓl�@�F�Ȃ�
//-----------------------------------------------------------------------------
short GetNowtime( char *data)
{
	char buf[40];
	DATEINFO di;
	TIMEINFO ti;

	btmemset( buf,0,sizeof(buf));

	if( btGetDateTime(&di,&ti)) return(-1);

	// �N
	if( di.year <1000 && di.year > 9999 ) return(-2);
	btnum2char((long)di.year,buf);

	// ��
	if( di.month <1 && di.month > 12 ) return(-3);
	if( di.month < 10 ){
		buf[4]='0';
		btnum2char((long)di.month,&buf[5]);
	}
	else {
		btnum2char((long)di.month,&buf[4]);
	}
	btmemcpy( data, buf,14);

	// ��
	if( di.day <1 && di.day > 31 ) return(-4);
	if( di.day < 10 ){
		buf[4]='0';
		btnum2char((long)di.day,&buf[7]);
	}
	else {
		btnum2char((long)di.day,&buf[6]);
	}

	// ��
	if( (int)ti.hour <0 && ti.hour > 24 ) return(-5);
	if( ti.hour < 10 ){
		buf[4]='0';
		btnum2char((long)ti.hour,&buf[9]);
	}
	else {
		btnum2char((long)ti.hour,&buf[8]);
	}
	// ��
	if( (int)ti.min <0 && ti.min >= 60 ) return(-6);
	if( ti.min < 10 ){
		buf[4]='0';
		btnum2char((long)ti.min,&buf[11]);
	}
	else {
		btnum2char((long)ti.min,&buf[10]);
	}
	// �b
	if( (int)ti.sec <0 && ti.sec >= 60 ) return(-7);
	if( ti.sec < 10 ){
		buf[4]='0';
		btnum2char((long)ti.sec,&buf[13]);
	}
	else {
		btnum2char((long)ti.sec,&buf[12]);
	}

	// �o��
	btmemcpy( data, buf,14);
	return(0);
}
//-----------------------------------------------------------------------------
//	�֐����F long Ltime( void )
//	�@�\�@�F�ߑO0������̌o�ߕb�����擾
//�@�����@�F�擾�f�[�^�i�[�|�C���^�i�o�́j *12�o�C�g�ȏ�m�ەK�v�I
//  �Ԓl�@�F�Ȃ�
//-----------------------------------------------------------------------------
long Ltime( void )
{
	TIMEINFO ti;

	if( btGetDateTime(NULL,&ti)) return(-1);

	return( (long)ti.hour * 3600+ (long)ti.sec*60+ti.min);

}