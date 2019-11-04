#include "App400.h"

//M400OS1 - 1. Tra hang ve san xuat	
int ReturnBoxToProduction()
{
	int ret, boxScanned = 0;
	char boxes[10240], boxesTemp[10240], tmp[10240];
	char boxNo [BOX_LENGTH + 2];

	btmemset(boxesTemp, 0, sizeof(boxesTemp));

	GetUserAndLocation();

	do
	{
		btmemset(msg, 0, sizeof(msg));
		btsprintf(msg, "[ %ld ]", boxScanned);

		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, boxesTemp, msg, NULL, SCAN_BOX_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(boxesTemp) > 0)
			break;

		if(ret == 2 && btstrlen(boxNo) == 0)
			continue;

		btsprintf(boxesTemp, "%s@%s", boxesTemp, boxNo);
		boxScanned++;

	}while(1);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M400", "S01", boxesTemp, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret)
		return -1;

	if(!ret)
		return 0;
}


//M400OS1 - 2. Cho san xuat muon lai thung hang
int BorrowBoxForOS(void)
{
	int ret, boxScanned = 0;
	char boxes[10240], boxesTemp[10240], tmp[10240];
	char boxNo [BOX_LENGTH + 2];

	GetUserAndLocation();
	
	btmemset(boxesTemp, 0, sizeof(boxesTemp));

	do
	{
		btmemset(msg, 0, sizeof(msg));
		btmemset(boxNo, 0, sizeof(boxNo));

		btsprintf(msg, "[ %ld ]", boxScanned);

		ret = ReadData(boxNo, boxesTemp, msg, NULL, SCAN_BOX_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(boxesTemp) > 0)
			break;
		
		if(ret == 2 && btstrlen(boxNo) == 0)
			continue;

		//btmemset(tmp, 0, sizeof(tmp));
		//btsprintf(tmp, "%s@%s", boxesTemp, boxNo);

		//btmemset(boxesTemp, 0, sizeof(boxesTemp));
		//btsprintf(boxesTemp, "%s", tmp);

		btsprintf(boxesTemp, "%s@%s", boxesTemp, boxNo);

		boxScanned++;
	}while(1);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M400", "S02", boxesTemp, send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret)
		return -1;

	if(!ret)
		return 0;
	
}

//M400OS1 - 3. Nhan lai thung hang da cho san xuat muon
int TakeBoxBackForOS(void)
{
	DINPUT_SETTING set;
	int ret, boxScanned = 0, totalBox = 0;
	char boxes[10240], boxesTemp[10240], tmp[10240];
	char boxNo [12 + 2], pallete[16 + 2];;
	
	GetUserAndLocation();

	btmemset(boxesTemp, 0, sizeof(boxesTemp));

	do
	{
		btmemset(buf, 0, sizeof(buf));
		btmemset(&set, 0, sizeof(set));
		btmemset(msg, 0, sizeof(msg));

		set.mode = 1;
		set.buffer = buf;
		set.bufsize = sizeof(buf);
		btstrcpy(set.l1, "Quet Pallete hoac ");
		btstrcpy(set.l2, "Shipping To");

		do
		{
			btmemset(buf, 0, sizeof(buf));

			ret = DataInputBt3(&set, 3);

			if(ret)
				return -1;

			if(btstrlen(buf) < 1)
				continue;

		}while(btmemcmp(buf, "POS", 3)  && btmemcmp(buf, "POR", 3) && btmemcmp(buf, "TOS", 3)  && btmemcmp(buf, "TOR", 3));

		if(!btmemcmp(buf, "POS", 3)  || !btmemcmp(buf, "POR", 3))
			ret = CheckDataByType(loc, buf, CHECK_PALLETE);
		else
			ret =  CheckDataByType(loc, buf, CHECK_SHIPPINGTO);

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

		btmemset(send, 0, sizeof(send));

		btsprintf(pallete, "%s", buf);

		MakeDataToSend("M400", "S03", buf, send);

		ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

		if(ret == 1)
			return -1;

		btmemcpy(boxes, rcv, sizeof(boxes));

		totalBox = btstrlen((char*)boxes)/BOX_LENGTH;

		if(totalBox > 0)
		{
			btmemset(boxesTemp, 0, sizeof(boxesTemp));

			do
			{
				btsprintf(msg, "[ %ld / %ld ]", boxScanned, totalBox);
				btmemset(boxNo, 0, sizeof(boxNo));

				ret = ReadData(boxNo, boxesTemp, msg, NULL, SCAN_BOX_NO);

				if(ret < 0) return ret;

				if(ret == 2 && btstrlen(boxesTemp) > 0)
					break;
				
				if(ret == 2 && btstrlen(boxNo) == 0)
					continue;

				ret = CheckDataExistInString(boxes, boxNo, CHECK_BOX);
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

				/*btmemset(tmp, 0, sizeof(tmp));
				btsprintf(tmp, "%s@%s", boxesTemp, boxNo);

				btmemset(boxesTemp, 0, sizeof(boxesTemp));
				btsprintf(boxesTemp, "%s", tmp);*/

				btsprintf(boxesTemp, "%s@%s", boxesTemp, boxNo);

				boxScanned++;
			}while(1);

			btmemset(tmp, 0, sizeof(tmp));
			btsprintf(tmp, "%s@%s", pallete, boxesTemp);

			btmemset(send, 0, sizeof(send));
			MakeDataToSend("M400", "S04", tmp, send);

			ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

			if(ret == 1)
				return -1;

			if(!ret)
				return 0;
			
		}
		else
		{
			btMessageBoxEx("Khong co thung", BT_MB_OKONLY, "M400S03", BT_FNTSIZE_NORMAL);
			return 0;
		}

	}while(1);
}

//M404 For OS -OR
//NHAN HANG THANH PHAM NGAY KIEM KE.
int InStockWhenCheckInventory()
{
	DINPUT_SETTING set;

	char now[10 +2];
	int ret, cnt = 0;
	int totalBox = 0, boxScan = 0;
	char boxes[10240], boxesTemp[10240];
	char pclList[4096], tmp[4096];
	char boxNo [BOX_LENGTH + 2];
	char date[8 + 2];
	char pclNo[PCL_LENGTH + 2];
	char day[2+1], month[2+1], year[4+1];

	btmemset(pclList, 0, sizeof(pclList));
	btmemset(msg, 0, sizeof(msg));

	do
	{
		btmemset(&set, 0, sizeof(set));	
		set.buffer = buf;
		set.bufsize = sizeof(buf);	
		btstrcpy(set.l1, "Nhap ngay nhap kho");	
		btstrcpy(set.l2, "[ DD/MM/YYYY ]");	
		set.alpha=0;

		do
		{
			btmemset( buf, 0, sizeof(buf));
			ret =  DataInputBt3( &set, 3 );	
			if(ret == -1)
				return ret;

		}while(btstrlen(buf) != 8);

		ret = isValidDate(buf);

		if(ret < 0)
		{
			SetBuzzerV(1);
			ret=DspMsgBt3(MAINWNDNAME,0,"**Error**","Ngay ngap kho khong", "hop le", "","","","" ,"", "",NULL);
			ret = -1;
		}
		else
		{	
			btmemset(day,0,sizeof(day));
			btmemset(month,0,sizeof(month));
			btmemset(year,0,sizeof(year));

			btmemcpy(day, buf, sizeof(day)-1);	
			btmemcpy(month, buf + 2,sizeof(month)-1);	
			btmemcpy(year, buf + 4,sizeof(year)-1);

			btmemset(date, 0, sizeof(date));
			btsprintf(date, "%s%s%s", year, month, day);

			btmemset(now, 0, sizeof(now));
			GetCurrentDate(now, 1);

			if(atoi(date) <= atoi(now))
			{
				SetBuzzerV(1);
				ret=DspMsgBt3(MAINWNDNAME,0,"**Error**","Ngay nhap kho phai", "lon hon ngay ", "hien tai.","","","" ,"", "",NULL);
				ret = -1;
			}
		}

	}while(ret != 0);

	do
	{
		btmemset(msg, 0, sizeof(msg));
		btsprintf(msg, "[%d]", cnt);
		btmemset(pclNo, 0, sizeof(pclNo));

		ret = ReadData(pclNo, pclList, msg, NULL, SCAN_PCL_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(pclList) > 0)
			break;

		if(ret == 2 && btstrlen(pclNo) == 0)
			continue;

		btmemset(tmp, 0, sizeof(tmp));
		btsprintf(tmp, "%s", pclList);

		btmemset(pclList, 0, sizeof(pclList));
		btsprintf(pclList, "%s@%s", tmp, pclNo);

		cnt++;

	}while(1);

	btmemset(send, 0, sizeof(send));

	MakeDataToSend("M404", "", pclList, send);	

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	btmemcpy(boxes, rcv, sizeof(boxes));

	totalBox = btstrlen((char*)boxes)/BOX_LENGTH;

	if(totalBox > 0)
	{
		btmemset(boxesTemp, 0, sizeof(boxesTemp));

		do
		{
			btsprintf(msg, "[ %ld / %ld ]", boxScan, totalBox);

			btmemset(boxNo, 0, sizeof(boxNo));
			ret = ReadData(boxNo, boxesTemp, msg, NULL, SCAN_BOX_NO);

			if(ret < 0) return ret;

			if(ret == 2 && btstrlen(boxNo) == 0)
				continue;

			//Neu thung dang scan khong nam trong danh sach thung cua server tra ve thi bao loi.
			ret = CheckDataExistInString(boxes, boxNo, CHECK_BOX);
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

			btmemset(tmp, 0, sizeof(tmp));
			btsprintf(tmp, "%s%s", boxesTemp, boxNo);

			btmemset(boxesTemp, 0, sizeof(boxesTemp));
			btsprintf(boxesTemp, "%s", tmp);

			boxScan++;

			if(boxScan == totalBox)
			{
				btmemset(tmp, 0, sizeof(tmp));
				btsprintf(tmp, "%s%s", date, pclList);

				btmemset(send, 0, sizeof(send));
				MakeDataToSend("M404", "S01", tmp, send);

				ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

				if(ret == 1)
					return -1;

				if(!ret)
					return 0;
			}
		} while(1);
	}
	else 
	{
		Buzzer();
	}
}

///M406-OS HOAN DOI THUNG - PALLETE
int SwitchBoxInPallete(void)
{
	int ret, totalBox = 0, boxScanned = 0;
	char fPallete[10 +2], tPallete[10 +2];
	char boxes[10240], boxesTemp[10240];
	char boxNo[BOX_LENGTH + 2];
	char tmp[10240];

	btmemset(msg, 0, sizeof(msg));
	btmemset(msg2, 0, sizeof(msg2));

	do
	{
		btmemset(fPallete, 0, sizeof(fPallete));
		ret = ReadData(fPallete, NULL, "Can Chuyen", NULL, SCAN_PALLETE_NO);

		if(ret < 0) return ret;
	}while(ret != 0);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M406", "", fPallete, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	btmemset(boxes, 0, sizeof(boxes));
	btmemcpy(boxes, rcv, sizeof(boxes));

	totalBox = btstrlen((char*)boxes)/BOX_LENGTH;

	if(totalBox < 1)
	{
		btMessageBoxEx("Khong co thung", BT_MB_OKONLY, "M406", BT_FNTSIZE_NORMAL);
		return 0;
	}

	do
	{
		btmemset(tPallete, 0, sizeof(tPallete));
		ret = ReadData(tPallete, NULL, "Duoc Chuyen", NULL, SCAN_PALLETE_NO);

		if(ret < 0) return ret;
	}while(ret != 0);

	btmemset(boxesTemp, 0, sizeof(boxesTemp));

	do
	{
		btsprintf(msg, "[ %ld ]", boxScanned);
		btmemset(boxNo, 0, sizeof(boxNo));

		ret = ReadData(boxNo, boxesTemp, msg, NULL, SCAN_BOX_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(boxesTemp) > 0)
			break;
		
		if(ret == 2 && btstrlen(boxNo) == 0)
			continue;

		ret = CheckDataExistInString(boxes, boxNo, CHECK_BOX);
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

		btmemset(tmp, 0, sizeof(tmp));
		btsprintf(tmp, "%s@%s", boxesTemp, boxNo);

		btmemset(boxesTemp, 0, sizeof(boxesTemp));
		btsprintf(boxesTemp, "%s", tmp);

		boxScanned++;
	}while(1);

	btmemset(tmp, 0, sizeof(tmp));
	btsprintf(tmp, "%s@%s@%s", fPallete, tPallete, boxesTemp);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M406", "S01", tmp, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	if(!ret)
		return 0;
}

//M414 OS1 - THEM THUNG TU KV NGOAI VAO SHIPPING TO
int AddBoxFromOutsideIntoShippingTo(void)
{
	int totalBox = 0, boxScanned = 0;
	char boxes[10240], boxesTemp[10240];
	char pallete[10 +2], shippingTo[SHIPPING_TO_LENGTH + 2];
	char boxNo[BOX_LENGTH +2];
	char tmp[10240];

	btmemset(msg, 0, sizeof(msg));
	btmemset(msg2, 0, sizeof(msg2));

	do
	{
		btmemset(pallete, 0, sizeof(pallete));
		ret = ReadData(pallete, NULL, "Hang O Ngoai", NULL, SCAN_PALLETE_NO);

		if(ret < 0) return ret;
	}while(ret != 0);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M414", "", pallete, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	btmemset(boxes, 0, sizeof(boxes));
	btmemcpy(boxes, rcv, sizeof(boxes));

	totalBox = btstrlen((char*)boxes)/BOX_LENGTH;

	if(totalBox < 1)
	{
		btMessageBoxEx("Khong co thung", BT_MB_OKONLY, "M414", BT_FNTSIZE_NORMAL);
		return 0;
	}

	do
	{
		btmemset(shippingTo, 0, sizeof(shippingTo));
		ret = ReadData(shippingTo, NULL, "", NULL, SCAN_SHIPPING_TO);

		if(ret < 0) return ret;
	}while(ret != 0);

	btmemset(boxesTemp, 0, sizeof(boxesTemp));

	do
	{
		btsprintf(msg, "[ %ld ]", boxScanned);

		btmemset(boxNo, 0, sizeof(boxNo));

		ret = ReadData(boxNo, boxesTemp, msg, NULL, SCAN_BOX_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(boxesTemp) > 0)
			break;
		
		if(ret == 2 && btstrlen(boxNo) == 0)
			continue;

		ret = CheckDataExistInString(boxes, boxNo, CHECK_BOX);
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

		btmemset(tmp, 0, sizeof(tmp));
		btsprintf(tmp, "%s@%s", boxesTemp, boxNo);

		btmemset(boxesTemp, 0, sizeof(boxesTemp));
		btsprintf(boxesTemp, "%s", tmp);

		boxScanned++;
	}while(1);

	btmemset(tmp, 0, sizeof(tmp));
	btsprintf(tmp, "%s@%s@%s", pallete, shippingTo, boxesTemp);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M414", "S01", tmp, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	if(!ret)
		return 0;
}
//M415OS1 - TRA THUNG TU SHIPPING TO RA KHU VUC SAP XEP
int ReturnBoxFromShippingToReceived(void){
	int ret, totalBox = 0, boxScanned = 0;
	char shippingNo[SHIPPING_TO_LENGTH + 2];
	char boxes[10240], boxesTemp[10240];
	char boxNo[BOX_LENGTH + 2];
	char tmp[10240];

	btmemset(msg, 0, sizeof(msg));
	btmemset(msg2, 0, sizeof(msg2));

	do
	{
		btmemset(shippingNo, 0, sizeof(shippingNo));
		ret = ReadData(shippingNo, NULL, "", NULL, SCAN_SHIPPING_TO);

		if(ret < 0) return ret;
	}while(ret != 0);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M415", "S01", shippingNo, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	btmemset(boxes, 0, sizeof(boxes));
	btmemcpy(boxes, rcv, sizeof(boxes));

	totalBox = btstrlen((char*)boxes)/BOX_LENGTH;

	if(totalBox < 1)
	{
		btMessageBoxEx("Khong co thung", BT_MB_OKONLY, "M418", BT_FNTSIZE_NORMAL);
		return 0;
	}

	btmemset(boxesTemp, 0, sizeof(boxesTemp));

	do
	{
		btsprintf(msg, "[ %ld ]", boxScanned);
		btmemset(boxNo, 0, sizeof(boxNo));

		ret = ReadData(boxNo, boxesTemp, msg, NULL, SCAN_BOX_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(boxesTemp) > 0)
			break;
		
		if(ret == 2 && btstrlen(boxNo) == 0)
			continue;

		ret = CheckDataExistInString(boxes, boxNo, CHECK_BOX);
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

		btsprintf(boxesTemp, "%s@%s", boxesTemp, boxNo);

		boxScanned++;
	}while(1);

	btmemset(tmp, 0, sizeof(tmp));
	btsprintf(tmp, "%s@%s", shippingNo, boxesTemp);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M415", "S02", tmp, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	if(!ret)
		return 0;
}

//M401 - TRA THUNG TU INSTOCK VE RECEIVE (OS)
int OSReturnBoxFromInstockToReceive(void)
{
	char palleteNo[PALLETE_LENGTH + 2];
	char boxes[10240], boxNo[BOX_LENGTH + 2], boxTemp[10240];
	int totalBox = 0, scanned = 0;

	do
	{
		btmemset(palleteNo, 0, sizeof(palleteNo));
		ret = ReadData(palleteNo, NULL, NULL, NULL, SCAN_PALLETE_NO);
		if(ret < 0) return ret;

	}while(ret != 0);

	btmemset(send, 0, sizeof(send));

	MakeDataToSend("M401", "S01", palleteNo, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1) return -1;

	btmemset(boxes, 0, sizeof(boxes));
	btmemcpy(boxes, rcv, sizeof(boxes));
	
	totalBox = btstrlen((char*)boxes) / BOX_LENGTH;

	if(totalBox < 1)
	{
		LcdClear();
		btMessageBoxEx("Khong co thung!!!", BT_MB_OKONLY, "Tra thung tu Instock ve Receive", BT_FNT_NORMAL);
		return 0;
	}

	btmemset(boxTemp, 0, sizeof(boxTemp));

	do
	{
		btsprintf(msg, " [ %ld ] ", scanned);
		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, boxTemp, msg, NULL, SCAN_BOX_NO);

		if(ret < 0) return ret;
		
		if(ret == 2 & btstrlen(boxTemp) > 0)
			break;

		if(ret == 2 & btstrlen(boxNo) == 0)
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

		btsprintf(boxTemp, "%s@%s", boxTemp, boxNo);

		scanned++;

	}while(1);

	btsprintf(boxTemp, "%s%s", palleteNo, boxTemp);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M401", "", boxTemp, send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1) return -1;

	if(!ret) return 0;
}

//M401 - TRA THUNG TU INSTOCK VE RECEIVE (OR)
int ORReturnBoxFromInstockToReceive(void)
{
	char boxNo[BOX_LENGTH + 2], boxTemp[10240];
	int scanned = 0;

	btmemset(boxTemp, 0, sizeof(boxTemp));

	do
	{
		btsprintf(msg, " [ %ld ]", scanned);

		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, boxTemp, msg, NULL, SCAN_BOX_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(boxTemp) > 0)
			break;

		if(ret == 2 && btstrlen(boxNo) == 0)
			continue;

		btsprintf(boxTemp, "%s@%s", boxTemp, boxNo);

		scanned++;

	}while(1);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M401", "", boxTemp, send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1) return -1;

	if(!ret) return 0;
}

//M409 - THAY DOI NGAY ETD, ETDVN, SHIP_SEQ
int ChangeEtdAndNOT(void) //NOT (Number of times)
{
	DINPUT_SETTING set;
	int ret, numberOfTimes = 0;
	char dateEDTVN[8+1], dateEDTHCM[8+1], palletNo[15+1];
	char dayEDTVN[2+1], monthEDTVN[2+1], yearEDTVN[4+1], dateTempEDTVN[10+1];
	char dayEDTHCM[2+1], monthEDTHCM[2+1], yearEDTHCM[4+1], dateTempEDTHCM[10+1];	
	char dataSend[10240];

	GetUserAndLocation();

	btmemset(&set, 0, sizeof(set));	
	set.buffer = buf;
	set.bufsize = sizeof(buf);	
	btstrcpy(set.l1, "Nhap so lan xuat");	
	btstrcpy(set.l2, "[>>...<<]");	
	set.alpha = 0;
	do
	{
	INPUT_NUMBER:
		btmemset( buf, 0, sizeof(buf));
		ret =  DataInputBt(&set);	
		if (ret < 0 || !btmemcmp(buf, "", 1))
		{
			Buzzer();
			keyCode = StartDisplay(MAINWNDNAME, "Warning!!!","Nhap so lan xuat", "", "", "", "C: Thoat", "Enter: Nhap lai");
			if (keyCode == 25)
				return (-2);
			else if (keyCode == 13)
				goto INPUT_NUMBER;
		}
	}while(!atoi(buf) && buf == "0");

	numberOfTimes = atoi(buf);
	//Nhap ngay EDTVN
	btmemset(&set, 0, sizeof(set));	
	set.buffer = buf;
	set.bufsize = sizeof(buf);
	btstrcpy(set.l1, "Nhap ngay EDT-VN");
	btstrcpy(set.l2, "[DD/MM/YYYY]");	
	set.alpha = 0;
	do
	{
	INPUT_ETDVN:
		btmemset(buf, 0, sizeof(buf));
		ret = DataInputBt3(&set, 3);	

		if (ret < 0 || !btmemcmp(buf, "", 1))
		{
			Buzzer();
			keyCode = StartDisplay(MAINWNDNAME, "Warning!!!","Nhap ngay ETD-VN", "", "", "", "C: Thoat", "Enter: Nhap lai");
			if (keyCode == 25)
				return (-2);
			else if (keyCode == 13)
				continue;
		}

		ret = isValidDate(buf);

		if(ret < 0)
		{
			btVibration(BT_ON_ASYNC,7,0,1);
			Beep(1);
			ret=DspMsgBt3(MAINWNDNAME,0,"**Error**","Ngay ETD-VN khong", "hop le", "","","","" ,"", "",NULL);
			continue;
		}
	}while(btstrlen(buf) != 8);
		
	btmemset(dateEDTVN, 0, sizeof(dateEDTVN));
	btmemcpy(dateEDTVN, buf, sizeof(dateEDTVN)-1);	

	//format ngay
	btmemset(dayEDTVN, 0, sizeof(dayEDTVN));
	btmemset(monthEDTVN, 0, sizeof(monthEDTVN));
	btmemset(yearEDTVN, 0, sizeof(yearEDTVN));

	btmemcpy(dayEDTVN, dateEDTVN, sizeof(dayEDTVN)-1);	
	btmemcpy(monthEDTVN, dateEDTVN + 2, sizeof(monthEDTVN)-1);	
	btmemcpy(yearEDTVN, dateEDTVN + 4, sizeof(yearEDTVN)-1);

	btmemset(dateTempEDTVN, 0, sizeof(dateTempEDTVN)-1);
	btsprintf(dateTempEDTVN, "%s/%s/%s", dayEDTVN, monthEDTVN, yearEDTVN);

	//Nhao ngay EDTHCM
SCAN_EDTHCM:
	btmemset(&set, 0, sizeof(set));	
	set.buffer = buf;
	set.bufsize = sizeof(buf);		
	btstrcpy(set.l1, "Nhap ngay EDT-HCM");	
	btstrcpy(set.l2, "[DD/MM/YYYY]");	
	set.alpha = 0;

	do
	{
	INPUT_ETDHCM:
		btmemset(buf, 0, sizeof(buf));
		ret =  DataInputBt3(&set, 3);	

		if (ret < 0 || !btmemcmp(buf, "", 1))
		{
			Buzzer();
			keyCode = StartDisplay(MAINWNDNAME, "","Nhap ngay ETD-HCM", "", "", "", "C: Thoat", "Enter: Nhap lai");
			if (keyCode == 25)
				return (-2);
			else if (keyCode == 13)
				continue;
		}

		ret = isValidDate(buf);

		if(ret < 0)
		{
			btVibration(BT_ON_ASYNC,7,0,1);
			Beep(1);
			ret=DspMsgBt3(MAINWNDNAME,0,"**Error**","Ngay ETD-VN khong", "hop le", "","","","" ,"", "",NULL);
			continue;
		}
	}while(btstrlen(buf) != 8);
		
	btmemset(dateEDTHCM, 0, sizeof(dateEDTHCM));
	btmemcpy(dateEDTHCM, buf, sizeof(dateEDTHCM)-1);

	//format ngay
	btmemset(dayEDTHCM, 0, sizeof(dayEDTHCM));
	btmemset(monthEDTHCM, 0, sizeof(monthEDTHCM));
	btmemset(yearEDTHCM, 0, sizeof(yearEDTHCM));

	btmemcpy(dayEDTHCM, dateEDTHCM, sizeof(dayEDTHCM)-1);	
	btmemcpy(monthEDTHCM, dateEDTHCM + 2, sizeof(monthEDTHCM)-1);	
	btmemcpy(yearEDTHCM, dateEDTHCM + 4, sizeof(yearEDTHCM)-1);	

	if(atoi(yearEDTVN) > atoi(yearEDTHCM))
	{
		Beep(0);
		goto SCAN_EDTHCM;
	}
	if(atoi(monthEDTVN) > atoi(monthEDTHCM))
	{
		Beep(0);
		goto SCAN_EDTHCM;
	}
	if(atoi(dayEDTVN) > atoi(dayEDTHCM))
	{
		if (atoi(monthEDTVN) == atoi(monthEDTHCM))
		{
			Beep(0);
			goto SCAN_EDTHCM;
		}
	}

	btmemset(dateTempEDTHCM, 0, sizeof(dateTempEDTHCM));
	btsprintf(dateTempEDTHCM, "%s/%s/%s", dayEDTHCM, monthEDTHCM, yearEDTHCM);

	keyCode = StartDisplay(MAINWNDNAME, "Ngay EDTVN - EDTHCM", dateTempEDTVN, dateTempEDTHCM, "", "", "", "ENT: Tiep tuc");

	// Quet Shipping To

	do
	{
		btmemset(palletNo, 0, sizeof(palletNo));
		ret = ReadData(palletNo, NULL, NULL, NULL, SCAN_SHIPPING_TO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(palletNo) > 0)
			break;
		else if(ret == 2 && btstrlen(palletNo) == 0)
			continue;

	}while(ret != 0);


	btsprintf(dataSend, "%d@%s@%s@%s", numberOfTimes, dateEDTVN, dateEDTHCM, palletNo);
	
	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M409","", dataSend, send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret)
	{
		return -1;
	}

	return 0;
}




//M403 - IN MAT PHIEU SHIPPING TO
int PrintLostShippingToOR(void)
{
	int ret;	
	char boxes[10240],boxNo[BOX_LENGTH + 2];	
	char dataSend[10240];

	GetUserAndLocation();
	
	do
	{
		memset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, NULL, NULL, NULL, SCAN_BOX_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(boxNo) == 0)
			continue;
	}while(ret != 0);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M403","",boxNo, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;	
	
	return 0;
}

//M405 - IN PHIEU SHIPPING TO CHI DINH
int PrintShippingToSpecify(void)
{
	DINPUT_SETTING set;
	int ret,number = 0, total = 0, boxScan = 0;
	char dateEDTVN[8+1], dateEDTHCM[8+1], palletNo[10+1];
	char boxes[10240],boxesTemp[10240],boxNo[12+2],tmp[4096];
	char dayEDTVN[2+1],monthEDTVN[2+1],yearEDTVN[4+1],dateTempEDTVN[10+1];
	char dayEDTHCM[2+1],monthEDTHCM[2+1],yearEDTHCM[4+1],dateTempEDTHCM[10+1];	
	char dataSend[10240];

	GetUserAndLocation();

	btmemset(&set, 0, sizeof(set));	
	set.buffer = buf;
	set.bufsize = sizeof(buf);	
	btstrcpy(set.l1, "Nhap so lan xuat");	
	set.alpha=0;

	do
	{
		btmemset( buf, 0, sizeof(buf));
		ret =  DataInputBt( &set );	
	}while(!atoi(buf) && buf == "0");

	number = atoi(buf);


	//Nhap ngay EDTVN
	btmemset(&set, 0, sizeof(set));	
	set.buffer = buf;
	set.bufsize = sizeof(buf);
	btstrcpy(set.l1, "Nhap ngay EDTVN");	
	btstrcpy(set.l2, "[DD/MM/YYYY]");	
	set.alpha=0;

	do
	{
		btmemset( buf, 0, sizeof(buf));
		ret =  DataInputBt( &set );	
		if(ret == -1)
			return;
	}while(btstrlen(buf) != 8);
		
	btmemset(dateEDTVN,0,sizeof(dateEDTVN));
	btmemcpy(dateEDTVN,buf,sizeof(dateEDTVN)-1);	
	//format ngay
	btmemset(dayEDTVN,0,sizeof(dayEDTVN));
	btmemset(monthEDTVN,0,sizeof(monthEDTVN));
	btmemset(yearEDTVN,0,sizeof(yearEDTVN));

	btmemcpy(dayEDTVN,dateEDTVN,sizeof(dayEDTVN)-1);	
	btmemcpy(monthEDTVN,dateEDTVN + 2,sizeof(monthEDTVN)-1);	
	btmemcpy(yearEDTVN,dateEDTVN + 4,sizeof(yearEDTVN)-1);

	btmemset(dateTempEDTVN,0,sizeof(dateTempEDTVN)-1);
	btsprintf(dateTempEDTVN,"%s/%s/%s",dayEDTVN,monthEDTVN,yearEDTVN);

	//Nhao ngay EDTHCM
SCAN_EDTHCM:
	btmemset(&set, 0, sizeof(set));	
	set.buffer = buf;
	set.bufsize = sizeof(buf);		
	btstrcpy(set.l1, "Nhap ngay EDTHCM");	
	btstrcpy(set.l2, "[DD/MM/YYYY]");	
	set.alpha=0;
	do
	{
		btmemset( buf, 0, sizeof(buf));
		ret =  DataInputBt( &set );	
		if(ret == -1)
			return;
	}while(btstrlen(buf) != 8);
		
	btmemset(dateEDTHCM,0,sizeof(dateEDTHCM));
	btmemcpy(dateEDTHCM,buf,sizeof(dateEDTHCM)-1);
	//format ngay
	btmemset(dayEDTHCM,0,sizeof(dayEDTHCM));
	btmemset(monthEDTHCM,0,sizeof(monthEDTHCM));
	btmemset(yearEDTHCM,0,sizeof(yearEDTHCM));

	btmemcpy(dayEDTHCM,dateEDTHCM,sizeof(dayEDTHCM)-1);	
	btmemcpy(monthEDTHCM,dateEDTHCM + 2,sizeof(monthEDTHCM)-1);	
	btmemcpy(yearEDTHCM,dateEDTHCM + 4,sizeof(yearEDTHCM)-1);	


	if(atoi(yearEDTVN) > atoi(yearEDTHCM))
	{
		Beep(0);
		goto SCAN_EDTHCM;
	}
	/*if(atoi(monthEDTVN) > atoi(monthEDTHCM))
	{
		Beep(0);
		goto SCAN_EDTHCM;
	}
	if(atoi(dayEDTVN) > atoi(dayEDTHCM))
	{
		Beep(0);
		goto SCAN_EDTHCM;
	}	*/

	if(atoi(monthEDTVN) > atoi(monthEDTHCM))
	{
		Beep(0);
		goto SCAN_EDTHCM;
	}

	if(atoi(monthEDTVN) >= atoi(monthEDTHCM))
	{
		if(atoi(dayEDTVN) > atoi(dayEDTHCM))
		{
			Beep(0);
			goto SCAN_EDTHCM;
		}	
		/*Beep(0);
		goto SCAN_EDTHCM;*/
	}

	btmemset(dateTempEDTHCM,0,sizeof(dateTempEDTHCM));
	btsprintf(dateTempEDTHCM,"%s/%s/%s",dayEDTHCM,monthEDTHCM,yearEDTHCM);

	keyCode = StartDisplay(MAINWNDNAME, "Ngay EDTVN - EDTHCM",dateTempEDTVN, dateTempEDTHCM,"", "","", "ENT: Tiep tuc");

	do
	{
		btmemset(palletNo, 0, sizeof(palletNo));
		ret = ReadData(palletNo, NULL, NULL, NULL, SCAN_PALLETE_NO);

		if (ret < 0 ) return ret;

	} while(ret != 0);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M303","S01",palletNo,send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	memcpy(boxes,rcv,sizeof(boxes));
	total =  btstrlen((char*) boxes)/BOX_LENGTH;	

	btmemset(boxesTemp, 0, sizeof(boxesTemp));

	do
	{
		btsprintf(msg, "[ %ld / %ld ]", boxScan, total);
		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, boxesTemp, msg, NULL, SCAN_BOX_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(boxesTemp) > 0)
			break;
		else if(ret == 2 && btstrlen(boxNo) == 0)
			continue;

		ret = CheckDataExistInString(boxes, boxNo, CHECK_BOX);
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

		btmemset(tmp, 0, sizeof(tmp));
		btsprintf(tmp, "%s@%s", boxesTemp, boxNo);

		btmemset(boxesTemp, 0, sizeof(boxesTemp));
		btsprintf(boxesTemp, "%s", tmp);
		boxScan++;

		if(boxScan == total )
			break;
		else
			continue;

	}while(1);

	btmemset(dataSend,0,sizeof(dataSend));
	btsprintf(dataSend,"@%s%s@%s@%d%s",dateEDTVN,dateEDTHCM,palletNo,number,boxesTemp);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M405","",dataSend,send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	if(!ret)
		return 0;

	return 0;
}

//M407 - IN LAI PHIEU SHIPPING TO
int PrintReprintShippingTo(void)
{
	int ret,number = 0, total = 0, boxScan = 0;
	char palletNo[SHIPPING_TO_LENGTH + 2];
	char boxes[10240],boxesTemp[10240],boxNo[12+2],tmp[4096];

	GetUserAndLocation();

	do
	{
		btmemset(palletNo, 0, sizeof(palletNo));
		ret = ReadData(palletNo, NULL, NULL, NULL, SCAN_SHIPPING_TO);

		if (ret < 0) return ret;

	}while(ret != 0);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M407", "S01", palletNo, send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	btmemset(boxes, 0, sizeof(boxes));
	btmemcpy(boxes,rcv,sizeof(boxes));

	total =  btstrlen((char*) boxes)/BOX_LENGTH;	

	btmemset(boxesTemp, 0, sizeof(boxesTemp));

	do
	{
		btsprintf(msg, "[ %ld / %ld ]", boxScan, total);

		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, boxesTemp, msg, NULL, SCAN_BOX_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(boxNo) == 0)
			continue;

		ret = CheckDataExistInString(boxes, boxNo, CHECK_BOX);
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

		btmemset(tmp, 0, sizeof(tmp));
		btsprintf(tmp, "%s@%s", boxesTemp, boxNo);

		btmemset(boxesTemp, 0, sizeof(boxesTemp));
		btsprintf(boxesTemp, "%s", tmp);
		boxScan++;

		if(boxScan == total)
		{
			btmemset(send, 0, sizeof(send));
			MakeDataToSend("M407", "", palletNo, send);
			ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

			if(ret == 1)
				return -1;

			if(!ret)
				return 0;
		}
		else
			continue;
	}while(1);
	
	return 0;
}

//M411 - IN LAI PHIEU SHIPPING TO(HANG DA SHIP)
int PrintLostShippingToOS(void)
{
	DINPUT_SETTING set;
	int ret;	
	char shippingTo[SHIPPING_TO_LENGTH + 2];
	char dataSend[10240];

	GetUserAndLocation();	

	do
	{
		btmemset(shippingTo, 0, sizeof(shippingTo));
		ret = ReadData(shippingTo, NULL, NULL, NULL, SCAN_SHIPPING_TO);
		
		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(shippingTo) == 0)
			continue;

	}while(ret != 0);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M411", "", shippingTo,send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;	
	
	return 0;
}

//M408 - DOI SO THU TU PHIEU SHIPPING TO
int TranferNumPLT(void)
{
	int scanned = 0;
	char shippingTo[SHIPPING_TO_LENGTH + 2];
	char mnShippingTo[5120];
	char tmp[5120];

	btmemset(mnShippingTo, 0, sizeof(mnShippingTo));
	btmemset(msg, 0, sizeof(msg));

	do
	{
		btsprintf(msg, "[ %ld ]", scanned);
		btmemset(shippingTo, 0, sizeof(shippingTo));
		ret = ReadData(shippingTo, mnShippingTo, msg, NULL, SCAN_SHIPPING_TO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(shippingTo) == 0)
			continue;

		btmemset(tmp, 0, sizeof(tmp));
		btsprintf(tmp, "%s@%s", mnShippingTo, shippingTo);

		btmemset(mnShippingTo, 0, sizeof(mnShippingTo));
		btsprintf(mnShippingTo, "%s", tmp);

		scanned++;

		if((btstrlen((char*)mnShippingTo) / SHIPPING_TO_LENGTH) == 2 && scanned == 2)
			break;

	}while(1);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M408","", mnShippingTo, send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);
			
	if(ret == 1)
		return -1;

	if(!ret)
		return 0;
}

//M402S01 Hoan doi thung giua 2 shipping to, hoac tra tu shipping to ve chua in shipping to
int TransferPalleteTempInShippingTo(void)
{
	int ret;
	char palleteOld[15+1], palleteNew[15+1];	
	char dataSend[10240], boxes[10240], boxesInPallete[10240];
	int totalBox = 0;
	char boxNo[BOX_LENGTH+2];
	char buf[SIZE_OF_BUF_2];
	char* r;
	
	DINPUT_SETTING set;

	//1.Quet shipping to old
	btmemset(palleteOld, 0, sizeof(palleteOld));
	btmemset(palleteNew, 0, sizeof(palleteNew));
	btmemset(boxesInPallete, 0, sizeof(boxesInPallete));
	btmemset(boxes, 0, sizeof(boxes));
	btmemset(boxNo, 0, sizeof(boxNo));
	btmemset(buf, 0, sizeof(buf));
	btmemset(send, 0, sizeof(send));
	btmemset(dataSend, 0, sizeof(dataSend));

	btmemset(msg, 0, sizeof(msg)); 
	do {
		ret = ReadData(palleteOld, NULL, "can chuyen di", NULL, SCAN_SHIPPING_TO);		
		if(ret == 2 && btstrlen(palleteOld) <= 0)
			continue;
		if(ret != 0)
			return -2;
	}while(ret != 0);

	btsprintf(dataSend, "%s", palleteOld);
	MakeDataToSend("M402", "S01", dataSend, send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);
	if(ret == 1)
	{
		return -1;		
	}
	btsprintf(boxesInPallete, "%s", rcv);

	//2. Quet or nhap shipping to new
	ret = ReadData(palleteNew, NULL, "chuyen den", NULL, SCAN_SHIPPING_TO);
	if(ret == -1)
		return ret;
	if(btstrlen(palleteNew) == 0)
	{
		btmemcpy(palleteNew, "0", 1);
	}

	//3. Doc tung box can chuyen di tren shipping to old
	do
	{	
		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, boxes, msg, NULL, SCAN_BOX_NO);
		if(ret == -1)
			return ret;
		if(ret == 2)
			break;
		if(ret != 0)
			continue;
		r = btstrstr(boxesInPallete, boxNo);
		if(btstrlen(r) <= 0)
		{
			do
			{
				keyCode = StartDisplay(MAINWNDNAME, "Loi:","Thung khong nam tren", "Pallete!!!", "", "", "C: Thoat", "ENT: Quet Lai");
			}while(keyCode != KEYCODE_ENT && keyCode != KEYCODE_TRG && keyCode != KEYCODE_BS);
			if(keyCode == KEYCODE_BS)
				return -2;
			keyCode = 0;
			continue;
		}
		btsprintf(boxes, "%s@%s", boxes, boxNo);
		totalBox = totalBox + 1;
		btsprintf(msg, "Tong so Box: %ld", totalBox);

	}while(1);

	btsprintf(dataSend, "%s@%s%s", palleteOld, palleteNew, boxes);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M402", "S02", dataSend, send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);
	if(ret == 1)
	{
		return -1;		
	}
}

//M412S01 Chuyen thung tu instock vao reser hoac ship
int TransferInstockToReserShip(void)
{
	int ret;
	char palleteOld[15+1], shippingToNew[15+1];	
	char dataSend[10240], boxes[10240], boxesInPallete[10240];
	int totalBox = 0;
	char boxNo[BOX_LENGTH+2];
	char buf[SIZE_OF_BUF_2];
	char* r;
	char typeShippingTo[2];
	char destination[20];
	
	DINPUT_SETTING set;

	btmemset(palleteOld, 0, sizeof(palleteOld));
	btmemset(shippingToNew, 0, sizeof(shippingToNew));
	btmemset(boxesInPallete, 0, sizeof(boxesInPallete));
	btmemset(boxes, 0, sizeof(boxes));
	btmemset(boxNo, 0, sizeof(boxNo));
	btmemset(buf, 0, sizeof(buf));
	btmemset(send, 0, sizeof(send));
	btmemset(typeShippingTo, 0, sizeof(typeShippingTo));
	btmemset(dataSend, 0, sizeof(dataSend));
	btmemset(destination, 0, sizeof(destination));

	btmemset(msg, 0, sizeof(msg)); 

	//1. Quet Pallete
	do{
		ret = ReadData(palleteOld, NULL, "can chuyen di", NULL, SCAN_PALLETE_NO);
		if(ret == 2 && btstrlen(palleteOld) <= 0)
			continue;
		if(ret != 0)
			return -2;
	}while(ret != 0);

	btsprintf(dataSend, "%s", palleteOld);
	MakeDataToSend("M412", "S01", dataSend, send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);
	if(ret == 1)
	{
		return -1;		
	}
	if(btstrlen(rcv) > 0)
		btsprintf(destination, "%s", rcv);

	//2. Chon 1: Reser, 2: Ship
	/*btmemset(&set, 0, sizeof(set));
	set.mode = 2;
	set.buffer = buf;
	set.bufsize = sizeof(buf);

	btsprintf(set.title, "%s", msg);
	btsprintf(set.l1, "%s", "Stock Chuyen den");
	btsprintf(set.l2, "%s", "1. Reser");
	btsprintf(set.l4, "%s", "2. Ship");

	do{
		btmemset(buf,0,sizeof(buf));
		ret= DataInputBt3(&set,3);
		if(ret)
		{
			return -1;
		}
	}while(btmemcmp(buf, "1", 1) && btmemcmp(buf, "2", 1));*/

	btmemcpy(typeShippingTo, "2", 1);


	//3. Quet Shipping To
	btsprintf(set.l1, "%s", "Shipping To");
	btsprintf(set.l2, "%s", "chuyen den");

	ret = ReadData(shippingToNew, NULL, "", NULL, SCAN_SHIPPING_TO);
	if(ret != 0)
		return -2;

	//4. Scan box chuyen di
	do
	{		
		ret = ReadData(boxNo, boxes, destination, msg, SCAN_BOX_NO);
		if(ret == -1)
			return ret;
		if(ret == 2)
			break;
		if(ret != 0)
			continue;
		
		btsprintf(boxes, "%s@%s", boxes, boxNo);
		totalBox = totalBox + 1;
		btsprintf(msg, "Tong so Box: %ld", totalBox);

	}while(1);

	btsprintf(dataSend, "%s@%s@%s%s", palleteOld, typeShippingTo,shippingToNew, boxes);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M412", "S02", dataSend, send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);
	if(ret == 1)
	{
		return -1;		
	}
}

//M417S01 Chuyen thung sau reser
int TransferAfterReser(void)
{
	int ret;
	char palleteOld[15+1], palleteNew[15+1];	
	char dataSend[10240], boxes[10240], boxesInPallete[10240];
	int totalBox = 0;
	char boxNo[BOX_LENGTH+2];
	char buf[SIZE_OF_BUF_2];
	char* r;
	
	DINPUT_SETTING set;

	//1.Quet shipping to old
	btmemset(palleteOld, 0, sizeof(palleteOld));
	btmemset(palleteNew, 0, sizeof(palleteNew));
	btmemset(boxesInPallete, 0, sizeof(boxesInPallete));
	btmemset(boxes, 0, sizeof(boxes));
	btmemset(boxNo, 0, sizeof(boxNo));
	btmemset(buf, 0, sizeof(buf));
	btmemset(send, 0, sizeof(send));
	btmemset(dataSend, 0, sizeof(dataSend));

	btmemset(msg, 0, sizeof(msg)); 

	do{
		ret = ReadData(palleteOld, NULL, "can chuyen di", NULL, SCAN_SHIPPING_TO);
		if(ret == 2 && btstrlen(palleteOld) <= 0)
			continue;
		if(ret != 0)
			return -2;
	}while(ret != 0);

	btsprintf(dataSend, "%s", palleteOld);
	MakeDataToSend("M417", "S01", dataSend, send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);
	if(ret == 1)
	{
		return -1;		
	}
	btsprintf(boxesInPallete, "%s", rcv);

	//2. Quet or nhap shipping to new
	ret = ReadData(palleteNew, NULL, "chuyen den", NULL, SCAN_SHIPPING_TO);
	if(ret == -1)
		return ret;
	if(btstrlen(palleteNew) == 0)
	{
		btmemcpy(palleteNew, "0", 1);
	}

	//3. Doc tung box can chuyen di tren shipping to old
	do
	{		
		ret = ReadData(boxNo, boxes, msg, NULL, SCAN_BOX_NO);
		if(ret == -1)
			return ret;
		if(ret == 2) 
			break;
		if(ret != 0)
			continue;
		r = btstrstr(boxesInPallete, boxNo);
		if(btstrlen(r) <= 0)
		{
			do
			{
				keyCode = StartDisplay(MAINWNDNAME, "Loi:","Thung khong nam tren", "Pallete!!!", "", "", "C: Thoat", "ENT: Quet Lai");
			}while(keyCode != KEYCODE_ENT && keyCode != KEYCODE_TRG && keyCode != KEYCODE_BS);
			if(keyCode == KEYCODE_BS)
				return -2;
			keyCode = 0;
			continue;
		}
		btsprintf(boxes, "%s@%s", boxes, boxNo);
		totalBox = totalBox + 1;
		btsprintf(msg, "Tong so Box: %ld", totalBox);

	}while(1);

	btsprintf(dataSend, "%s@%s%s", palleteOld, palleteNew, boxes);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M417", "S02", dataSend, send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);
	if(ret == 1)
	{
		return -1;		
	}
}

int CheckBoxCanInsertToShippingTo(void)
{
	int ret;	
	char dataSend[200];
	char boxNo[BOX_LENGTH + 2];

	btmemset(buf, 0, sizeof(buf));
	btmemset(msg, 0, sizeof(msg));

	do
	{
		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, NULL, "", NULL, SCAN_BOX_NO);

		if(ret == -1)
			return ret;

		if(ret != 0)
			continue;

		btmemset(send, 0, sizeof(send));
		MakeDataToSend("M416", "S01", boxNo, send);
		ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	}while(1);
}

int FindBox(void) {
	int ret;	
	char dataSend[200];
	char boxNo[BOX_LENGTH + 2];

	btmemset(buf, 0, sizeof(buf));
	btmemset(msg, 0, sizeof(msg));

	do
	{
		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, NULL, "", NULL, SCAN_BOX_NO);

		if(ret == -1)
			return ret;

		if(ret != 0)
			continue;

		btmemset(send, 0, sizeof(send));
		MakeDataToSend("M418", "", boxNo, send);
		ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);
		if (ret==1)
			continue;

	}while(1);
}

//Kiem tra tinh trang pallete
int CheckStatusOfShippingTo(void)
{
	int ret;
	char shippingTo[15+1];	
	char dataSend[10240];
	char buf[SIZE_OF_BUF_2];
	char* r;
	
	DINPUT_SETTING set;

	//1.Quet shipping to old
	btmemset(shippingTo, 0, sizeof(shippingTo));
	btmemset(buf, 0, sizeof(buf));
	btmemset(send, 0, sizeof(send));
	btmemset(dataSend, 0, sizeof(dataSend));

	btmemset(msg, 0, sizeof(msg)); 

	do{
		ret = ReadData(shippingTo, NULL, "Xem tinh trang", NULL, SCAN_SHIPPING_TO);
		if(ret == 2 && btstrlen(shippingTo) <= 0)
			continue;
		if(ret != 0)
			return -2;
	}while(ret != 0);

	btsprintf(dataSend, "%s", shippingTo);
	MakeDataToSend("M419", "S01", dataSend, send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);
	if(ret == 1)
	{
		return -1;		
	}
}

//M420 - GHI NHAN DU LIEU DAI KIEN / HUY DAI KIEN.
int WhPackageRecord(void)
{
	DINPUT_SETTING set;
	int ret;
	char type[5];
	char pallete[15 + 2];
	char dataSend[128];
	
	GetUserAndLocation();

	btmemset(&set, 0, sizeof(set));
	set.mode = 2;
	set.buffer = buf;
	set.bufsize = sizeof(buf);

	btsprintf(set.title, MAINWNDNAME);
	btsprintf(set.l1, "%s", "Chon chuc nang");
	btsprintf(set.l2, "%s", "1. Dai kien");
	btsprintf(set.l4, "%s", "2. Huy dai kien");

	do{
		btmemset(buf,0,sizeof(buf));
		ret= DataInputBt3(&set,3);

		if(ret)
			return -1;

	}while(btmemcmp(buf, "1", 1) && btmemcmp(buf, "2", 1));

	btmemset(type, 0, sizeof(type));
	btmemcpy(type, buf, sizeof(type));

	do
	{
		btmemset(pallete, 0, sizeof(pallete));
		ret = ReadData(pallete, NULL, "", NULL, SCAN_SHIPPING_TO);

		if(ret == -1)
			return ret;

		if(ret != 0)
			continue;

		btmemset(send, 0, sizeof(send));
		btmemset(dataSend,0,sizeof(dataSend));
		btsprintf(dataSend,"%s@%s",type, pallete);

		MakeDataToSend("M420", "", dataSend, send);
		ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

		if(ret == 1)
			return -1;		

	}while(ret != 0);
}