#include "Common.h"

char send[SIZE_OF_DATA], rcv[SIZE_OF_DATA];
char buf[SIZE_OF_BUF_2];
int keyCode;
char loc[3+1], userId[10];
char msg[SIZE_OF_BUF_1], msg2[SIZE_OF_BUF_1];
int ret,cntError = 0;

const int MAX_VALID_YR = 9999; 
const int MIN_VALID_YR = 1800; 

int InputPassword(void)
{
	int ret;
	char indata[256];
	char now[20], password[10];
	DINPUT_SETTING st;

	// key+bar
	// data input setting
	btmemset(&st,0,sizeof(DINPUT_SETTING));
	btstrcpy(st.title,MAINWNDNAME);
	btsprintf(st.l1, " Connection Setting");
	btsprintf(st.l2," Please input Admin");
	btsprintf(st.l3," password");
	btstrcpy(st.l7,"     [back:F1]");
	st.buffer=indata;
	st.bufsize=sizeof(indata);
	st.mode=0;
	st.emptyOK = FALSE;

	ret = 1;
	while(ret){
		// input password
		btmemset( indata,0,sizeof(indata));
		st.bar_read=FALSE;
		ret=DataInputBt(&st);
		if(ret==-2) 
			return(-2);
		else if(ret) 
			continue;

		//get the date end MMDD
		btmemset(now, 0, sizeof(now) );
		get_nowtime(now, 8 );
		btmemset(password, 0, sizeof(password) );
		btmemcpy(password, now+4, 4);

		//check password
		if( btstrlen(indata) != 4 || btmemcmp(indata, password, 4) != 0 ){
			LcdClear();
			Buzzer();
			LineOut4("[*ERROR*]",0,0);
			LineOut(" Wrong password",3,0);
			FuncSel(0);
			ret = 1;
		}
	}

	return 0;
}

int CheckAdminPassword(void)
{	
	DINPUT_SETTING set;
	
	int ret;
	char buf[128];		// send buffer
	char sday[30], now[10];

	get_nowtime(now, 8);
	btmemset(sday, 0, sizeof(sday));
	btmemcpy(sday, now + 4, 4);

	// initialize structure 
	btmemset(&set,0,sizeof(set));
	set.mode=0;		// only key-input
	set.buffer=buf;
	set.bufsize=sizeof(buf);
	btstrcpy(set.l1,"Authentication");
	btstrcpy(set.l2,"Please enter");
	btstrcpy(set.l2,"  Admin password.");
	set.c2=CLR_RED;
	set.alpha=0;		// allow alphabet

	// entering password
	btmemset( buf,0,sizeof(buf));
	ret= DataInputBt( &set );

	if(ret) 
		return(-1);				// cancel

	if(btmemcmp(buf, sday, 4) || btstrlen(buf) != 4)
		return (-2);

	// OK
	return(0);
}

//=====================================================================================================
//	functionFint DataSend(char* data,int size,char* rcv,int rsize)
//  send requset to PC and receive,if error,show error message
//	arg1:pointer to send data
//	arg2:size of send buffer
//	arg3:pointer to recv data
//	arg4:size of recv buffer
//	arg5:show OK message or no
//	return:-1=TCP err, 0=receive OK,1=receive NG and erro message,2=receive other data
//=====================================================================================================

int DataSend(char* data,int size,char* rcv,int rsize,BOOL ok_disp)
{
	ENABLE_WAITKEYS key;
	int ret;
	char buf[20];

	btmemset( &key,0,sizeof(ENABLE_WAITKEYS));
	key.keyL=TRUE;
	key.keyR=TRUE;

	btmemset( rcv,0,rsize);

	// main loop
	do
	{
		// send
		//ret=WLAN_send(data, btstrlen(data) ,rcv,rsize);
		
		int len=0;
		ret = WLAN_sendBt(data,size,rcv,rsize,&len,10,300); //Use direct this function to setting time out for HT

		// confirm re-send wher tcp error
		if(ret)
		{
			btsprintf(buf,"ret=%d",ret);
			Buzzer();

			if(	DspMsgBt(MAINWNDNAME,3,"","**************","com error," ,"retry?","","**************",buf,&key))
			{
				return(-1);	// cancel
			}
		}
	}while(ret);

	// recieve NG and error message 
	if( !btmemcmp( rcv,"NG",2))
	{
		// show received message
		ShowMessage( MAINWNDNAME,(BCR_MSG*)&rcv[2],"",TRUE, 2);
		return(1);
	}

	//Receive data from server with 2 character first not equal OK
	if(btmemcmp(rcv,"OK",2))
	{
		return(2);
	}

	//When 2 character first equal OK
	if(!btmemcmp(rcv, "OK",  2))
	{
		ShowMessage( MAINWNDNAME,(BCR_MSG*)&rcv[2],"",TRUE, 0);
		return 0;
	}

	// receive OK
	// if OK is on,show OK message(3sec)
	if(ok_disp) 
		OkNgDisp(TRUE,3000);
	
	return(0);

}

void ShowMessage(char* title,BCR_MSG* hmsg,char* line5,BOOL buzzer, int beepMode)
{
	char l1[20+1],l2[20+1],l3[20+1],l4[20+1],l5[20+1];
	ENABLE_WAITKEYS key;

	btmemset( &key,0,sizeof(ENABLE_WAITKEYS));
	key.key1 = TRUE;

	btmemset( l1,0,sizeof(l1));
	btmemcpy( l1,hmsg->msg1,20);

	btmemset( l2,0,sizeof(l2));
	btmemcpy( l2,hmsg->msg2,20);

	btmemset( l3,0,sizeof(l3));
	btmemcpy( l3,hmsg->msg3,20);

	btmemset( l4,0,sizeof(l4));
	btmemcpy( l4,hmsg->msg4,20);

	btmemset( l5,0,sizeof(l5));
	btmemcpy( l5,hmsg->msg5,20);

	if(buzzer)
	{
		//Beep(beepMode);
		SetBuzzerV(beepMode);
	}

	DspMsgBt(title,5,l1,l2,l3,l4,l5,"",line5,&key);
}

//Setting location for HT
int SetLocation(void)
{
	char loc[4], dat[50];
	int ret;

	DINPUT_SETTING st;

	btmemset(loc, 0, sizeof(loc));
	MasterFind("location", loc);

	LcdClear();
	btmemset(&st,0,sizeof(st));
	st.mode=0;
	st.alpha = 1;
	btstrcpy(st.title,"Input Your Location.");

	if( btstrlen(loc))
		btstrcpy(st.l5,"Don't change -> ENT");

	btsprintf(st.l1,"%s","Location Setting");
	btsprintf(st.l2,"Location = %s",loc);
	st.c2 = CLR_RED;


	st.buffer=dat;
	st.bufsize=sizeof(dat)-1;
	st.emptyOK=TRUE;

	btmemset(dat,0,sizeof(dat));

	while(1)
	{
		ret = DataInputBt( &st );

		if( btstrlen(loc) && ret==-101 )
			btstrcpy(dat,loc);

		if( !btstrlen(dat)) 
			continue;

		if( btstrlen(dat) != 3 )
		{
			DspMsg(" ***** Error *****",0,"Location is ","not correct.(1)",dat,"","",NULL);
			continue;
		}
		else
		{
			MasterUpd("location",dat);
			break;
		}
	}

	return 0;
}

int MakeDataToSend(char* cmd, char* sub, char* data, char* sendData)
{
	char loc[3+1], userId[10];
	char send[20480];

	btmemset(loc, 0, sizeof(loc));
	btmemset(userId, 0, sizeof(userId));

	MasterFind("location", loc);
	MasterFind("user", userId);

	if(!btmemcmp(data, "", 1))
		btsprintf(send, "%s$%s$", cmd, userId);
	else
		btsprintf(send, "%s%s$%s$%s$%s$", cmd, sub, loc, userId, data);

	btmemcpy(sendData, send, btstrlen(send));

	return 0;
}

int GetBoxNoFromString(char* str, char* boxNo)
{
	char box[BOX_LENGTH + 2];
	char tmp[12+2];

	btmemset(boxNo, 0, sizeof(boxNo));
	btmemset(box, 0, sizeof(box));
	btmemset(tmp, 0, sizeof(tmp));

	if(btstrlen(str) == 300)
	{
		btmemcpy(tmp, str + 163, 12);
		btmemcpy(boxNo, tmp, sizeof(tmp));
	}
	else
		btmemcpy(boxNo, str, sizeof(box));

	if(btmemcmp(boxNo, "VB", 2)  && btmemcmp(boxNo, "VC", 2))
	{
		btVibration(BT_ON_ASYNC,7,0,1);
		Beep(1);
	}

	return 0;
}

int CheckDataByType(char* loc, char* val, int type)
{
	char cmp1[10], cmp2[10];
	int size, cntErr = 0;
	char msg[128];

	btmemset(cmp1, 0, sizeof(cmp1)); //Compare variable for OS
	btmemset(cmp2, 0, sizeof(cmp2)); //Compare variable for OR
	btmemset(msg, 0, sizeof(msg));

	switch (type)
	{	
	case CHECK_PCL://PCL
		btsprintf(cmp1, "LOS");
		btsprintf(cmp2, "LOR");
		btsprintf(msg, "Quet khong dung");
		btsprintf(msg2, "ma vach PCL.");
		size = 3;
		break;
	case CHECK_PALLETE://Pallete
		btsprintf(cmp1, "POS");
		btsprintf(cmp2, "POR");
		btsprintf(msg, "Quet khong dung");
		btsprintf(msg2, "ma vach pallete.");
		size = 3;
		break;
	case CHECK_BOX://Box
		btsprintf(cmp1, "VB");
		btsprintf(cmp2, "VC");
		btsprintf(msg, "Quet khong dung");
		btsprintf(msg2, "ma vach box.");
		size = 2;
		break;
	case CHECK_SHIPPINGTO://Shipping To - Shipping mark
		btsprintf(cmp1, "TOS");
		btsprintf(cmp2, "TOR");
		btsprintf(msg, "Quet khong dung");
		btsprintf(msg2, "ma vach Shipping To.");
		size = 3;
		break;
	case CHECK_SHIPPINGNO://Shippng No
		btsprintf(cmp1, "SOS");
		btsprintf(cmp2, "SOR");
		btsprintf(msg, "Quet khong dung");
		btsprintf(msg2, "ma vach Ship.");
		size = 3;
		break;
	}

	if (!btmemcmp((char*)loc, "OS1", 3))
	{
		if (btmemcmp((char*)val, cmp1, size))
		{
			cntErr++;
		}
	}
	else if (!btmemcmp((char*)loc, "OR1", 3))
	{
		if (btmemcmp((char*)val, cmp2, size))
		{
			cntErr++;
		}
	}

	if(cntErr > 0)
	{
		//Buzzer();
		SetBuzzerV(2);

		do
		{
			keyCode = StartDisplay(MAINWNDNAME, "Warning!!!",msg, msg2, "", "", "C: Thoat", "ENT: Quet Lai");
		}while(keyCode != KEYCODE_ENT && keyCode != KEYCODE_TRG && keyCode != KEYCODE_BS);

		btmemset(msg, 0, sizeof(msg));
		btmemset(msg2, 0, sizeof(msg2));

		return keyCode;
	}

	return 0;
}

int CheckDataExistInString(char* listData, char* data, int flag)
{
	char msg[128],msg2[128];
	char* r;
	r = btstrstr(listData, data);

	btmemset(msg, 0, sizeof(msg));
	btmemset(msg2, 0, sizeof(msg2));

	switch(flag)
	{
		case CHECK_BOX:
			btsprintf(msg, "Thung nay khong co!!!");
			break;
		case CHECK_PCL:
			btsprintf(msg, "PCL nay khong co!!!");
			break;
		case CHECK_PALLETE:
			btsprintf(msg, "Pallete nay khong co!!!");
			break;
		case CHECK_SHIPPINGNO:
			btsprintf(msg, "ShipNo nay khong co!!!");
			break;
		case CHECK_SHIPPINGTO:
			btsprintf(msg, "ShippingTo nay khong ");
			btsprintf(msg2, "co!!!");
			break;
	}

	if(btstrlen(r) <= 0)
	{
		//Buzzer();
		SetBuzzerV(2);
		do
		{
			keyCode = StartDisplay(MAINWNDNAME, "Loi:",msg, msg2, "", "", "C: Thoat", "ENT: Quet Lai");
		}while(keyCode != KEYCODE_ENT && keyCode != KEYCODE_TRG && keyCode != KEYCODE_BS);

		btmemset(msg, 0, sizeof(msg));
		btmemset(msg2, 0, sizeof(msg2));

		return keyCode;
	}

	return 0;
}

int CheckDuplicate(char* listData, char* data, int flag)
{
	char msg[128],msg2[128];
	char* r;
	r = btstrstr(listData, data);

	btmemset(msg, 0, sizeof(msg));
	btmemset(msg2, 0, sizeof(msg2));

	switch(flag)
	{
		case CHECK_BOX:
			btsprintf(msg, "Quet trung thung!!!");
			break;
		case CHECK_PCL:
			btsprintf(msg, "Quet trung PCL!!!");
			break;
		case CHECK_PALLETE:
			btsprintf(msg, "Quet trung Pallete!!!");
			break;
		case CHECK_SHIPPINGNO:
			btsprintf(msg, "Quet trung ShipNo!!!");
			break;
		case CHECK_SHIPPINGTO:
			btsprintf(msg, "Quet trung");
			btsprintf(msg2, "ShippingTo!!!");
			break;
	}

	if(btstrlen(r) > 0)
	{
		//Buzzer();
		SetBuzzerV(2);

		do
		{
			keyCode = StartDisplay(MAINWNDNAME, "Loi:",msg, msg2, "", "", "C: Thoat", "ENT: Quet Lai");
		}while(keyCode != KEYCODE_ENT && keyCode != KEYCODE_TRG && keyCode != KEYCODE_BS);

		btmemset(msg, 0, sizeof(msg));
		btmemset(msg2, 0, sizeof(msg));

		return keyCode;
	}

	return 0;
}

int GetUserAndLocation(void)
{
	btmemset(loc,0,sizeof(loc));
	btmemset(userId,0,sizeof(userId));

	MasterFind("location",loc);
	MasterFind("user",userId);
	return 0;
}

int ReadData(char* value, char* list, char* message1, char* message2, int  mode)
{
	DINPUT_SETTING set;

	int size, type;
	char cmp1[10], cmp2[10];
	char l1[20 +2], l2[20 +2];
	char errMsg[128];
	char inData[SIZE_OF_BUF_2];
	
	LcdClear();
	btClear();

	btmemset(cmp1, 0, sizeof(cmp1));
	btmemset(cmp2, 0, sizeof(cmp2));
	btmemset(l1, 0, sizeof(l1));
	btmemset(errMsg, 0, sizeof(errMsg));
	btmemset(value, 0, sizeof(value));

	btmemset(buf, 0, sizeof(buf));
	btmemset(&set, 0, sizeof(set));

	btmemset(loc, 0, sizeof(loc));
	MasterFind("location", loc);

	btstrcpy(set.title, MAINWNDNAME);
	set.mode = 1;
	set.buffer = buf;
	set.bufsize = sizeof(buf);
	
	switch(mode)
	{
		case SCAN_BOX_NO:
			btsprintf(l1, "Quet thung");
			btsprintf(errMsg, "vach thung");
			btsprintf(cmp1, "VB");
			btsprintf(cmp2, "VC");
			type = CHECK_BOX;
			size = 2;
			break;
		case SCAN_PCL_NO:
			btsprintf(l1, "Quet phieu PCL");
			btsprintf(errMsg, "vach PCL");
			btsprintf(cmp1, "LOS");
			btsprintf(cmp2, "LOR");
			type = CHECK_PCL;
			size = 3;
			break;
		case SCAN_PALLETE_NO:
			btsprintf(l1, "Quet pallete");
			btsprintf(errMsg, "vach pallete");
			btsprintf(cmp1, "POS");
			btsprintf(cmp2, "POR");
			type = CHECK_PALLETE;
			size = 3;
			break;
		case SCAN_SHIPPING_TO:
			btsprintf(l1, "Quet Shipping To");
			btsprintf(errMsg, "vach shipping to");
			btsprintf(cmp1, "TOS");
			btsprintf(cmp2, "TOR");
			type = CHECK_SHIPPINGTO;
			size = 3;
			break;
	}

	btstrcpy(set.l1, l1);
	btstrcpy(set.l2, message1);
	btstrcpy(set.l4, message2);
	//btstrcpy(set.l7, "  [    R:Gui    ]");
	btstrcpy(set.l7, " [L:Thoat | R:Gui]");

	do
	{
		btmemset(inData, 0, sizeof(inData));
		btmemset(buf, 0, sizeof(buf));

		ret = DataInputBt3(&set, 3);

		if(ret)
			return ret;

		if(ret == 0 &&  btstrlen(buf) < 1)
			return 2;

		if(btstrlen(buf) < 1)
			continue;

		if(type == CHECK_BOX)
			GetBoxNoFromString(buf, inData);
		else
			btmemcpy(inData, buf, sizeof(buf));

		if(btmemcmp(inData, cmp1, size)  && btmemcmp(inData, cmp2, size))
		{
			SetBuzzerV(1);

			ret=DspMsgBt3(MAINWNDNAME,0,"**Error**","Quet khong dung ma", errMsg, "","","","" ,"", "",NULL);

			ret = -1;

			continue;
		}
		else
		{
			ret = CheckDataByType(loc, inData, type);

			switch(ret)
			{
			case KEYCODE_BS:
				return -2;
				break;
			case KEYCODE_TRG:
			case KEYCODE_ENT:
				ret = -1;
				continue;
				break;
			}

			if(btstrlen(list) > 0)
			{
				ret = CheckDuplicate(list, inData, type);
				switch(ret)
				{
				case KEYCODE_BS:
					return -2;
					break;
				case KEYCODE_TRG:
				case KEYCODE_ENT:
					ret = -1;
					continue;
					break;
				}
			}
		}

	}while(ret < 0);

	btmemcpy(value, inData, btstrlen(inData));

	return 0;
}

//Check this year is leap year
//return 0 if given year is valid
int isLeapYear(int year)
{
	// Return true if year  
	// is a multiple pf 4 and 
	// not multiple of 100. 
	// OR year is multiple of 400. 

	if(((year % 4 == 0) && (year % 100 != 0)) || 
        (year % 400 == 0))
		 return 0;

	return -1;
}

int isValidDate(char* date)
{
	int ret;
	int d, m, y;
	char day[2+1], month[2+1], year[4+1];

	btmemset(day,0,sizeof(day));
	btmemset(month,0,sizeof(month));
	btmemset(year,0,sizeof(year));

	btmemcpy(day, date, sizeof(day)-1);	
	btmemcpy(month,date + 2,sizeof(month)-1);	
	btmemcpy(year,date + 4,sizeof(year)-1);

	d = atoi(day);
	m = atoi(month);
	y = atoi(year);

	if (y > MAX_VALID_YR || y < MIN_VALID_YR) 
		return -1; 

	if (m < 1 || m > 12) 
		return -1; 

	if (d < 1 || d > 31) 
		return -1; 

	// Handle February month  
    // with leap year 
	if (m == 2) 
	{ 
		ret = isLeapYear(y);

		if (ret == 0) 
		{
			if ( d <= 29) 
				return 0; 
			else 
				return -1;
		}
		else
		{
			if ( d <= 28) 
				return 0; 
			else 
				return -1;
		}
	} 

	// Months of April, June,  
    // Sept and Nov must have  
    // number of days less than 
    // or equal to 30. 
    if (m == 4 || m == 6 || 
        m == 9 || m == 11) 
	{
		if(d <= 30)
			return 0;
		else
			return -1;

	}

	return 0;
}

int GetCurrentDate(char *date, int type)
{
	DATEINFO DateInfo;

	btGetDateTime(&DateInfo,0);

	switch(type)
	{
	case 0: //[dd/mm/yyyy]
		btsprintf(date, "%d/%d/%d", DateInfo.day, DateInfo.month, DateInfo.year);
		break;
	case 1: //[yyyymmdd]
		btsprintf(date, "%d%02d%02d", DateInfo.year, DateInfo.month, DateInfo.day);
		break;
	case 2: //[ddmmyyyy]
		btsprintf(date, "%02d%02d%d", DateInfo.day, DateInfo.month, DateInfo.year);
		break;
	};
	return 0;
}

void SetBuzzerV(int mode)
{
	//Set volumn for Buzzer
	btVolume(2);

	switch(mode){
		case 0:		// OK
			btVibration(BT_ON_ASYNC,7,0,1);
			btLed(BT_ON_ASYNC,1,1,2,BT_LED_GREEN);
			btBuzzer(BT_ON_ASYNC,1,1,2,7);
		break;

		case 1:		// WARNING
			btVibration(BT_ON_ASYNC,7,0,1);
			btLed(BT_ON_ASYNC,2,1,5,BT_LED_ORANGE);
			btBuzzer(BT_ON_ASYNC,1,1,2,10);
		break;

		default:	// ERROE
			btVibration(BT_ON_ASYNC,7,0,1);
			btLed(BT_ON_ASYNC,5,1,3,BT_LED_RED);
			btBuzzer(BT_ON_ASYNC,1,1,2,15);
		break;
	}
}