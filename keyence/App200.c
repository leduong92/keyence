#include "App200.h"

//M200 - CHANGE PALLTE OR
int ChangePalleteForOR()
{
	char seq[25], pallete[SHIPPING_TO_LENGTH + 2];
	char boxNo[BOX_LENGTH + 2];

	GetUserAndLocation();

	do
	{
		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, NULL, NULL, NULL, SCAN_BOX_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(boxNo) == 0)
			continue;

		btmemset(send, 0, sizeof(send));
		MakeDataToSend("M200", "", boxNo, send);

		ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

		if(ret != 2)
			continue;

		btmemset(pallete, 0, sizeof(pallete));
		btmemset(seq, 0, sizeof(seq));

		btmemcpy(seq, rcv, 20);
		btmemcpy(pallete, rcv + 20, 15);

		SetBuzzerV(0);
		keyCode = StartDisplay(MAINWNDNAME,"", seq, pallete, "", "", "", "C: Back");

		if(keyCode == KEYCODE_BS)
			return -2;

	}while(1);
}

//M203 - TIM KIEM THUNG BI THIEU TREN PALLETE
int FindBoxLostInPallete(void)
{
	char boxNo[BOX_LENGTH + 2], boxes[10240], boxesTemp[10240];
	int totalBox = 0, scanned = 0;

	do
	{
		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, NULL, NULL, NULL, SCAN_BOX_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(boxNo) == 0)
			continue;
	}while(ret != 0);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M203", "", boxNo, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);
	if(ret == 1)
		return -1;

	btmemset(boxes, 0, sizeof(boxes));
	btmemcpy(boxes, rcv, sizeof(boxes));
	totalBox = btstrlen((char*)boxes)/BOX_LENGTH;

	if(totalBox < 1)
	{
		SetBuzzerV(0);
		btMessageBoxEx("Khong co thung.", BT_MB_OKONLY, "M203", BT_FNTSIZE_NORMAL);
		return 0;
	}

	btmemset(boxesTemp, 0, sizeof(boxesTemp));
	btsprintf(boxesTemp, "%s", boxNo);
	scanned = 1;

	do
	{
		btsprintf(msg, " [ %ld / %ld ]", scanned, totalBox);
		
		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, boxesTemp, msg, NULL, SCAN_BOX_NO);
		
		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(boxesTemp) > 0)
			break;

		if(ret == 2 && btstrlen(boxNo) > 0)
			continue;

		ret = CheckDataExistInString(boxes, boxNo, CHECK_BOX);
		switch(ret)
		{
		case KEYCODE_BS:
			return -2;
		case KEYCODE_TRG:
		case KEYCODE_ENT:
			continue;
		}

		btsprintf(boxesTemp, "%s@%s", boxesTemp, boxNo);

		scanned++;
	}while(1);

	btmemset(send, 0, sizeof(send));

	MakeDataToSend("M203", "S01", boxesTemp, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	if(!ret)
		return 0;
}

//M204 - DUA PALLETE LEN CONTINAER
int LoadPalleteToContainer(void)
{

	int ret, cntError = 0; 
	int totalBox = 0, boxScan = 0;
	char container[60], pallete[20];
	char dataSend[60];
	int flag = 0;

	DINPUT_SETTING set;

	GetUserAndLocation();

	do
	{
		btmemset(buf, 0, sizeof(buf));
		btmemset(&set, 0, sizeof(set));
		set.mode = 1;
		set.buffer = buf;
		set.bufsize = sizeof(buf);

		btstrcpy(set.title,MAINWNDNAME);
		btstrcpy(set.l1, "Quet Phieu Container");
		btstrcpy(set.l2, "");

		do{
			btmemset( buf,0,sizeof(buf));
			ret= DataInputBt( &set );
			if(ret) return(-1);				
		}while(btmemcmp(buf, "CON", 3));

		btmemset(container, 0, sizeof(container));
		btmemcpy(container, buf, btstrlen(buf));


		btstrcpy(set.l1,"Quet Pallete");
		btstrcpy(set.l2,"");

		btmemset( buf,0,sizeof(buf));

		do{
			ret= DataInputBt( &set );
			if(ret) 
				return(-1);	

			ret = CheckDataByType(loc, buf, CHECK_SHIPPINGTO);

			if(!ret)
				break;
			switch(ret)
			{
			case KEYCODE_BS:
				return -2;
				break;
			case KEYCODE_TRG:
			case KEYCODE_ENT:
				continue;
				break;
			}
		}while(1);

		btmemset(pallete, 0, sizeof(pallete));
		btmemcpy(pallete, buf, btstrlen(buf));

		btsprintf(dataSend, "%s%s", container, pallete);

		MakeDataToSend("M204", "", dataSend, send);

		ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

		/*if(ret == 1)
		return -1;*/
	} while(1);
}

int SetContainerLockSignForPallete(void)
{
	int ret, cntError = 0; 
	char container[60];

	DINPUT_SETTING set;

	GetUserAndLocation();

	btmemset(buf, 0, sizeof(buf));
	btmemset(&set, 0, sizeof(set));
	set.mode = 1;
	set.buffer = buf;
	set.bufsize = sizeof(buf);

	do
	{
		btstrcpy(set.title,MAINWNDNAME);
		btstrcpy(set.l1, "Quet Phieu Container");
		btstrcpy(set.l2, "");

		do{
			btmemset( buf,0,sizeof(buf));
			ret= DataInputBt( &set );
			if(ret) return(-1);				
		}while(btmemcmp(buf, "CON", 3));

		btmemset(container, 0, sizeof(container));
		btmemcpy(container, buf, btstrlen(buf));

		MakeDataToSend("M205", "", container, send);

		ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	}while(1);
}
