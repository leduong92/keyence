#include "App300.h"


//M301 - OR/OS
//NHAN HANG THANH PHAM
int InStock(void)
{
	int ret,  cnt = 0; 
	char pclList[4096], tmp[4096];
	int totalBox = 0, boxScan = 0;
	char boxes[10240], boxesTemp[10240];
	char boxNo [BOX_LENGTH + 2];
	char pclNo[PCL_LENGTH + 2];


	GetUserAndLocation();

	btmemset(pclList, 0, sizeof(pclList));
	btmemset(msg, 0, sizeof(msg));

	do
	{
		btsprintf(msg, "[ %d ]", cnt);
		btmemset(pclNo, 0, sizeof(pclNo));

		ret = ReadData(pclNo, pclList, msg, NULL, SCAN_PCL_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(pclList) > 0)
			break;
		else if(ret == 2 && btstrlen(pclNo) == 0)
			continue;

		btsprintf(pclList, "%s@%s", pclList, pclNo);

		cnt++;

	}while(1);

	btmemset(send, 0, sizeof(send));

	MakeDataToSend("M301", "", pclList, send);	

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

			//if(ret < 0) return ret;
			//Khi dang scan box, neu nhan phim L thi khong bi thoat ra ngoai.
			if(ret < 0)
				continue;

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
				btmemset(send, 0, sizeof(send));

				MakeDataToSend("M301", "S01", pclList, send);

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

//M301 - OR
int InStockForOR(void)
{
	//////////////////////
	int ret,  cnt = 0, flag = 0; 
	char pclList[4096], tmp[4096];
	int totalBox = 0, boxScan = 0;
	char boxes[10240], boxesTemp[10240],  boxesRemain[10240];
	char boxNo [BOX_LENGTH + 2];
	char pclNo[PCL_LENGTH + 2];
	char* r;
	char leading[10240], trailing[10240];
	char lots[4096];

	GetUserAndLocation();

	btmemset(pclList, 0, sizeof(pclList));
	btmemset(send, 0, sizeof(send));

	MakeDataToSend("M301", "", "GetAllPCL", send);	

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	btmemcpy(pclList, rcv, sizeof(pclList));

	//Check mitsuba
	btmemset(send, 0, sizeof(send));

	MakeDataToSend("M301", "S03", pclList, send);	

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	btmemset(boxes, 0, sizeof(boxes));
	btmemcpy(boxes, rcv, sizeof(boxes));

	if( btstrlen((char*)boxes)/BOX_LENGTH > 0)
	{
		btsprintf(msg, " Total Box: %ld", btstrlen((char*)boxes)/BOX_LENGTH);
		btMessageBoxEx(msg, BT_MB_OKONLY, "MITSUBA", BT_FNTSIZE_NORMAL);
	}

	//End of check mitsuba

	btmemset(send, 0, sizeof(send));

	MakeDataToSend("M301", "S01", pclList, send);	

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	btmemset(boxes, 0, sizeof(boxes));
	btmemcpy(boxes, rcv, sizeof(boxes));
	btmemcpy(boxesRemain, rcv, sizeof(boxesRemain));

	totalBox = btstrlen((char*)boxes)/BOX_LENGTH;

	if(totalBox > 0)
	{
		btmemset(boxesTemp, 0, sizeof(boxesTemp));

		do
		{
	SCAN_BOX:
			btsprintf(msg, "[ %ld / %ld ]", boxScan, totalBox);

			btmemset(boxNo, 0, sizeof(boxNo));
			ret = ReadData(boxNo, boxesTemp, msg, NULL, SCAN_BOX_NO);

			//if(ret < 0) return ret;
			//Khi dang scan box, neu nhan phim L thi khong bi thoat ra ngoai.
			if(ret < 0)
				continue;

			if(ret == 2 && btstrlen(boxNo) == 0)
				continue;

			//Press F2
			if(ret == 202 && btstrlen(boxNo) == 0)
			{
				flag = 1;
				break;
			}

			if(btstrlen(boxNo) == 0)
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

			r = btstrstr(boxesRemain, boxNo);

			if(btstrlen(r) > 0)
			{
				/*btsprintf(boxesRemain, btmemcpy(boxesRemain, 0, btstrlen(r)), btmemcpy(boxesRemain, r + 13, btstrlen(boxesRemain))), */
				int pos = (int) (r - boxesRemain);
				btmemset(leading, 0, sizeof(leading));
				btmemset(trailing, 0, sizeof(trailing));

				btmemcpy(leading, boxesRemain, pos);
				btmemcpy(trailing, boxesRemain + pos + BOX_LENGTH, btstrlen(boxesRemain));

				btsprintf(boxesRemain, "%s%s", leading, trailing); 
			}

			boxScan++;

			if(boxScan == totalBox)
			{
				btmemset(send, 0, sizeof(send));

				MakeDataToSend("M301", "S02", pclList, send);

				ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

				if(ret == 1)
					return -1;

				if(!ret)
					return 0;
			}
		} while(1);

		//Truong hop khi nhan hang nhung so thung tren line thieu so voi so thung tren PCL tay scan hien thi
		//Gui lenh de check thung thieu.
		if(flag == 1 && btstrlen(boxesRemain) > 0)
		{
			btmemset(send, 0, sizeof(send));

			MakeDataToSend("M301", "S04", boxesRemain, send);

			ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

			if(ret == 1)
				return -1;
			
			btmemset(lots, 0, sizeof(lots));
			btmemcpy(lots, rcv, sizeof(lots));

			ret = btMessageBoxEx(lots , BT_MB_OKCANCEL, "Thong Bao", BT_FNTSIZE_SMALL);
			//goto SCAN_BOX;

			if(ret == BT_MB_OK)
				goto SCAN_BOX;
		}
	}
	else 
	{
		Buzzer();
	}

}

//M306 - OS
//KIEM TON KHO THANH PHAM - OS
int CheckInventoryForOS(void)
{
	DINPUT_SETTING set;
	int ret, totalBox = 0, boxScanned = 0;
	char boxes[20480], boxesTemp[20480];
	char tmp[4096], boxNo [BOX_LENGTH + 2], pallete[10 + 2], shippingTo[16 + 2];
	char* r;
	char mnPallete[20480], mnPalleteTemp[20480];
	int totalPallete = 0, palleteScanned = 0;

	GetUserAndLocation();

	btmemset(msg, 0, sizeof(msg));
	btmemset(boxes, 0, sizeof(boxes));

	do
	{
		btmemset(pallete, 0, sizeof(pallete));
		ret = ReadData(pallete, NULL, NULL, NULL, SCAN_PALLETE_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(boxNo) == 0)
			continue;

		btmemset(send, 0, sizeof(send));
		MakeDataToSend("M306", "", pallete, send);

		ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

		if(ret == 1)
			return -1;

		if(!ret)
			return 0;

		btmemcpy(boxes, rcv, sizeof(boxes));

		totalBox = btstrlen((char*)boxes)/BOX_LENGTH;

		if(totalBox > 0)
		{
			btmemset(boxesTemp, 0, sizeof(boxesTemp));
			do
			{
				btsprintf(msg, "[ %ld / %ld ]", boxScanned, totalBox);
				btmemset(boxNo, 0, sizeof(boxNo));
				//ret = ReadBoxNo(boxNo,boxesTemp, NULL, msg);
				ret = ReadData(boxNo, boxesTemp, msg, NULL, SCAN_BOX_NO);

				if(ret < 0) return ret;

				if(ret == 2 && btstrlen(boxesTemp) > 0)
					goto SEND_DATA;
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

				boxScanned++;

				if(boxScanned == totalBox)
				{
SEND_DATA:
					btmemset(tmp, 0, sizeof(tmp));
					btsprintf(tmp, "%s@%s", pallete, boxesTemp);

					btmemset(send, 0, sizeof(send));
					MakeDataToSend("M306", "S01", tmp, send);

					ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

					if(ret == 1)
						return -1;

					//Sau khi insert cac box vao td_check_instock hoan thanh se kiem tra co pallete nao chua 
					if(ret == 2)
					{
						btmemset(send, 0, sizeof(send));
						MakeDataToSend("M306", "S02", pallete, send);

						ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

						if(ret == 1)
							return -1;

						btmemset(mnPallete, 0, sizeof(mnPallete));
						btmemcpy(mnPallete, rcv, sizeof(rcv));

						totalPallete = btstrlen((char*)mnPallete) / SHIPPING_TO_LENGTH;

						if(totalPallete > 0)
						{
							btmemset(mnPalleteTemp, 0, sizeof(mnPalleteTemp));
		
							do
							{
								btmemset(msg, 0, sizeof(msg));
								btsprintf(msg, "To  [ %ld/%ld ]",palleteScanned, totalPallete);

								btmemset(shippingTo, 0, sizeof(shippingTo));
								ret = ReadData(shippingTo, mnPalleteTemp, msg, NULL, SCAN_SHIPPING_TO);

								if(ret < 0) return ret;

								if(ret == 2 && btstrlen(mnPalleteTemp) > 0)
									goto SEND_DATA_1;
								else if(ret == 2 && btstrlen(shippingTo) == 0)
									continue;

								ret = CheckDataExistInString(mnPallete, shippingTo, CHECK_SHIPPINGTO);
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
								btsprintf(tmp, "%s@%s", mnPalleteTemp, shippingTo);

								btmemset(mnPalleteTemp, 0, sizeof(mnPalleteTemp));
								btsprintf(mnPalleteTemp, "%s", tmp);

								palleteScanned++;

								btsprintf(msg, "  [ %ld/%ld ] ",palleteScanned, totalPallete);
								//keyCode = StartDisplay(MAINWNDNAME, "Quet Shipping To","ke tiep:", shippingTo, "", "", "", " C:Thoat - ENT:Gui ");

								if(palleteScanned == totalPallete)
								//if(keyCode == KEYCODE_ENT)
								{
SEND_DATA_1:
									btmemset(tmp, 0, sizeof(tmp));
									btsprintf(tmp, "%s@%s", pallete, mnPalleteTemp);

									btmemset(send, 0, sizeof(send));
									MakeDataToSend("M306", "S03", tmp, send);

									ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

									if(ret)
										return -1;

									if(!ret)
										return 0;
								}

							}while(1);
						}
						else
						{
							Beep(0);
							btMessageBoxEx("Khong co pallete!!!", BT_MB_OKONLY, "Thong Bao", BT_FNTSIZE_NORMAL);
							return 0;
						}
					}
				}
			}while(1);
		}
		else 
		{
			Buzzer();
			return 0;
		}

	}while(1);
}

//M306 - OR1
//KIEM TON KHO THANH PHAM OR
int CheckInventoryForOR(void)
{
	DINPUT_SETTING set;
	int ret, cntError = 0, totalBox = 0, boxScanned = 0;
	char boxes[20480], boxesTemp[20480];
	char tmp[20480], boxNo [12 + 2], pallete[10 + 2];
	char* r;
	char mnPallete[20480], mnPalleteTemp[20480];
	int totalPallete = 0, palleteScanned = 0;
	char shippingTo[SHIPPING_TO_LENGTH +2];

	GetUserAndLocation();

	btmemset(msg, 0, sizeof(msg));
	btmemset(boxesTemp, 0, sizeof(boxesTemp));


	do
	{
		btsprintf(msg, "[ %ld ]", boxScanned);
		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, boxesTemp, msg, NULL, SCAN_BOX_NO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(boxesTemp) > 0)
			goto SEND_DATA;
		else if(ret == 2 && btstrlen(boxNo) == 0)
			continue;

		//Gui len bcr_server kiem tra xem thung hang nay da duoc scan sap xep chua
		btmemset(send, 0, sizeof(send));

		MakeDataToSend("M306", "R01", boxNo, send);

		ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

		if(ret && !btmemcmp(rcv, "NG", 2))
			continue;

		if(ret == 2)
		{
			boxScanned++;

			btmemset(tmp, 0, sizeof(tmp));
			btsprintf(tmp, "%s", boxesTemp);

			btmemset(boxesTemp, 0, sizeof(boxesTemp));
			btsprintf(boxesTemp, "%s%s", tmp, boxNo);
			continue;
		}

SEND_DATA: 
		btmemset(send, 0, sizeof(send));

		MakeDataToSend("M306", "R02", "VC", send);

		ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

		if(ret == 1)
			return -1;

		btmemset(mnPallete, 0, sizeof(mnPallete));

		btmemcpy(mnPallete, rcv, sizeof(mnPallete));

		totalPallete = btstrlen((char*)mnPallete) / SHIPPING_TO_LENGTH;

		if(totalPallete > 0)
		{
			btmemset(mnPalleteTemp, 0, sizeof(mnPalleteTemp));

			do
			{
				btsprintf(msg, "To  [ %ld/%ld ]",palleteScanned, totalPallete);
				btmemset(shippingTo, 0, sizeof(shippingTo));

				ret = ReadData(shippingTo, mnPalleteTemp, msg, NULL, SCAN_SHIPPING_TO);

				if(ret < 0) return ret;

				if(ret == 2 && btstrlen(mnPalleteTemp) > 0)
					goto SEND_DATA_1;
				else if(ret == 2 && btstrlen(shippingTo) == 0)
					continue;

				ret = CheckDataExistInString(mnPallete, shippingTo, CHECK_SHIPPINGTO);
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
				btsprintf(tmp, "%s@%s", mnPalleteTemp, shippingTo);

				btmemset(mnPalleteTemp, 0, sizeof(mnPalleteTemp));
				btsprintf(mnPalleteTemp, "%s", tmp);

				palleteScanned++;

				btsprintf(msg, "  [%ld/%ld]",palleteScanned, totalPallete);
				//keyCode = StartDisplay(MAINWNDNAME, "Quet Shipping To","ke tiep:", shippingTo, "", "", "", " C:Thoat - ENT:Gui ");

				if(palleteScanned == totalPallete)
				//if(keyCode == KEYCODE_ENT)
				{
SEND_DATA_1:
					btmemset(send, 0, sizeof(send));
					MakeDataToSend("M306", "R03", mnPalleteTemp, send);

					ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

					if(ret)
						return -1;

					if(!ret)
						return 0;
				}

			}while(1);
		}
		else
		{
			Beep(1);
			btMessageBoxEx("Khong co pallete!!!", BT_MB_OKONLY, "Thong Bao", BT_FNTSIZE_NORMAL);
			return 0;
		}

	}while(1);
}

//M300-OR1
//IN PHIEU PCL CHO OR
int PrintPCLOR(void) 
{
	GetUserAndLocation();

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M300", "", buf, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 2)
	{
		Beep(0);
		return -1;
	}
	return 0;
}

//M305 SHIP: Share to use for OR and OS 
int ShipOSOR(void) 
{
	int ret = 0, countNumberOfScansError = 0;
	int numberOfPalleteScaned = 0;
	char palleteTemp[1024], palleteSendToServer[1024], dataSend[1024];
	char shippingTo[SHIPPING_TO_LENGTH + 2];
	char title[20];
	DINPUT_SETTING set;

	GetUserAndLocation();

	btmemset(palleteTemp, 0, sizeof(palleteTemp));
	btmemset(palleteSendToServer, 0, sizeof(palleteSendToServer));
	btmemset(dataSend, 0, sizeof(dataSend));

	do
	{
		btsprintf(msg, "[ %ld ]", numberOfPalleteScaned); 
		btmemset(shippingTo, 0, sizeof(shippingTo));
		ret = ReadData(shippingTo, palleteSendToServer, msg, NULL, SCAN_SHIPPING_TO);

		if(ret < 0 ) return ret;

		if(ret == 2 && btstrlen(palleteSendToServer) > 0)
			break;

		if(ret == 2 && btstrlen(shippingTo) == 0)
			continue;
		
		btmemset(palleteTemp, 0, sizeof(palleteTemp));
		btsprintf(palleteTemp, "%s@%s", palleteSendToServer, buf);
		btmemset(palleteSendToServer, 0, sizeof(palleteSendToServer));
		btsprintf(palleteSendToServer, "%s", palleteTemp);

		numberOfPalleteScaned ++;

	}while(1);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M305", "", palleteSendToServer, send);
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	return 0;
}

//M304 - OS1
//RESER HANG CHO OS
int Reservation()
{
	int ret, countPallete = 0; 
	char arrPallete[2468], message[128], currentPallete[16], sendPallete[2468];
	int checkDuplication;

	GetUserAndLocation();

	btmemset(arrPallete, 0, sizeof(arrPallete));

	do
	{
		btmemset(message, 0, sizeof(message));
		btsprintf(message, "[ %d ]", countPallete);

		btmemset(currentPallete, 0, sizeof(currentPallete));
		ret = ReadData(currentPallete, arrPallete, message, NULL, SCAN_SHIPPING_TO);

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(arrPallete) > 0)
			break;

		countPallete++;

		btsprintf(arrPallete, "%s%s@", arrPallete, currentPallete);
	}while(1);


	btmemset(sendPallete, 0, sizeof(sendPallete));
	btmemcpy(sendPallete, arrPallete, strlen(arrPallete) - 1);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M304", "", sendPallete, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;

	return 0;
}

//M303 : IN PHIEU SHIPPING TO (OS)
int PrintShippingToOS(void)
{
	DINPUT_SETTING set;
	int ret,number = 0, total = 0, boxScan = 0;
	char dateEDTVN[8+1], dateEDTHCM[8+1], palletNo[10+1];
	char boxes[10240],boxesTemp[10240],boxNo[12+2],tmp[4096];
	char dayEDTVN[2+1],monthEDTVN[2+1],yearEDTVN[4+1],dateTempEDTVN[10+1];
	char dayEDTHCM[2+1],monthEDTHCM[2+1],yearEDTHCM[4+1],dateTempEDTHCM[10+1];	
	char dataSend[10240];

	GetUserAndLocation();

	btmemset(msg2, 0, sizeof(msg2));
	
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

	do
	{
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
			ret =  DataInputBt3( &set, 3);	
			if(ret == -1)
				return;
		}while(btstrlen(buf) != 8);

		if(btmemcmp(buf, "00000000", 8))
		{
			ret = isValidDate(buf);

			if(ret != 0)
			{
				SetBuzzerV(1);

				ret=DspMsgBt3(MAINWNDNAME,0,"**Error**","Ngay khong hop le", "", "","","","" ,"", "",NULL);

				ret = -1;

				continue;
			}
			else
				break;
		}
		else
			break;

	}while(ret != 0);

		
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
	do
	{
		SCAN_EDTHCM:
		//Nhap ngay EDTVN
		btmemset(&set, 0, sizeof(set));	
		set.buffer = buf;
		set.bufsize = sizeof(buf);
		btstrcpy(set.l1, "Nhap ngay EDTHCM");	
		btstrcpy(set.l2, "[DD/MM/YYYY]");
		set.alpha=0;

		do
		{
			btmemset( buf, 0, sizeof(buf));
			ret =  DataInputBt3( &set, 3);	
			if(ret == -1)
				return;
		}while(btstrlen(buf) != 8);

		if(btmemcmp(buf, "00000000", 8))
		{
			ret = isValidDate(buf);

			if(ret != 0)
			{
				SetBuzzerV(1);

				ret=DspMsgBt3(MAINWNDNAME,0,"**Error**","Ngay khong hop le", "", "","","","" ,"", "",NULL);

				ret = -1;

				continue;
			}
			else
				break;
		}
		else
			break;

	}while(ret != 0);
		
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

		if(ret < 0) return ret;

		if(ret == 2 && btstrlen(palletNo) == 0)
			continue;
	}while(ret != 0);
			
	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M303","S01",palletNo,send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);
	if(ret == 1)
		return -1;

	btmemset(boxes, 0, sizeof(boxes));
	btmemcpy(boxes, rcv, sizeof(boxes));

	total =  btstrlen((char*) boxes)/BOX_LENGTH;	

	btmemset(boxesTemp, 0, sizeof(boxesTemp));

	do
	{
		btsprintf(msg, "[ %ld / %ld]", boxScan, total);

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
			btmemset(dataSend,0,sizeof(dataSend));
			btsprintf(dataSend,"@%s%s@%s@%d%s",dateEDTVN,dateEDTHCM,palletNo,number,boxesTemp);

			btmemset(send, 0, sizeof(send));
			MakeDataToSend("M303","",dataSend,send);
			ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);
			
			if(ret == 1)
				return -1;

			if(!ret)
				return 0;
		}
		else
			continue;
			
	} while(1);	

	return 0;
}

//M303 : IN PHIEU SHIPPING TO (OR)
int PrintShippingToOR(void)
{
	DINPUT_SETTING set;
	int ret, total = 0, boxScan = 0;	
	char boxes[10240],boxesTemp[10240],boxNo[12+2],tmp[4096];	
	char dataSend[10240];

	btmemset(msg2, 0, sizeof(msg2));
	
	GetUserAndLocation();

	do
	{
		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, NULL, NULL, NULL, SCAN_BOX_NO);

		if(ret < 0) return ret;

	}while(ret != 0);

	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M303","S01",boxNo,send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);
	if(ret == 1)
		return -1;

	memcpy(boxes,rcv,sizeof(boxes));
	total =  btstrlen((char*) boxes)/BOX_LENGTH;

	btmemset(boxesTemp, 0, sizeof(boxesTemp));
	btsprintf(boxesTemp, "%s", boxNo);
	boxScan = 1;	
	
	do
	{
		btsprintf(msg, "[%ld / %ld]", boxScan, total);

		//Truong hop tren pallete chi co 1 box sau khi scan box dau tien xong se cho in shipping to.
		if(total == 1)
			goto PRINT_SHIPPINGTO;

		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, boxesTemp, msg, NULL, SCAN_BOX_NO);

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
PRINT_SHIPPINGTO:
			btmemset(dataSend,0,sizeof(dataSend));
			btsprintf(dataSend,"%s",boxNo);

			btmemset(send, 0, sizeof(boxNo));
			MakeDataToSend("M303","",dataSend,send);
			ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

			if(ret == 1)
				return -1;

			if(!ret)
				return 0;
		}

	}while(1);

	return 0;
}

int ArrangeBoxToPallet(void)
{	
	GetUserAndLocation();

	if(!btmemcmp(loc, "OS1", btstrlen("OS1")))
	{
		ArrangeToPalletOS();
	}
	else if(!btmemcmp(loc, "OR1", btstrlen("OR1")))
	{
		ArrangeToPalletOR();
	}
	else 
	{
		btMessageBoxEx("This function doesn't exists!!!", BT_MB_OKONLY,"ArrangeBoxToPallet", BT_FNT_NORMAL);
	}
}

//M302
//SAP XEP HANG LEN PALLETE
int ArrangeToPalletOS()
{
	int ret;
	char pallete[15];	
	char dataSend[10240], boxes[10240];
	int totalBox = 0;
	char boxNo[BOX_LENGTH + 2];

	btmemset(boxes, 0, sizeof(boxes));
	btmemset(pallete, 0, sizeof(pallete));

	
	btmemset(buf, 0, sizeof(buf));

	btmemset(msg, 0, sizeof(msg)); 
	btmemset(msg2, 0, sizeof(msg2));

	do
	{
		ret = ReadData(pallete, NULL, "can chuyen", NULL, SCAN_PALLETE_NO);

		if(ret < 0) return ret;

		if(ret == 2 & btstrlen(pallete) == 0)
			continue;
	}while(ret != 0);


	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M302", "S01", pallete, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
		return -1;	

	if(btstrlen(rcv) > 0)
		btsprintf(msg, "%s", rcv);

	do
	{
		btsprintf(msg2, "[ %ld ] ", totalBox);

		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, boxes, msg, msg2, SCAN_BOX_NO);

		if(ret == -1)
			return ret;

		if(ret == 2 & btstrlen(boxes) > 0)
			break;

		if(ret != 0)
			continue;

		//20190524 kiem tra hang cho
		btsprintf(dataSend, "%s@%s", pallete ,boxNo);
		MakeDataToSend("M302", "S04", dataSend, send);

		ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);
		if(ret ==1)
		{
			//return -1;
			continue;
		}
		//end kiem tra hang cho

		btsprintf(boxes, "%s@%s", boxes, boxNo);

		totalBox++;

	}while(1);

	/*btmemset(send, 0, sizeof(send));
	btsprintf(dataSend, "%s%s", pallete ,boxes);
	MakeDataToSend("M302", "S02", dataSend, send);

	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if(ret == 1)
	{
		return -1;		
	}*/
}

//M302 - OR
//SAP XEP HANG LEN PALLETE
int ArrangeToPalletOR()
{
	int ret;	
	char dataSend[200];
	char boxNo[BOX_LENGTH + 2];

	btmemset(buf, 0, sizeof(buf));
	btmemset(msg, 0, sizeof(msg));

	do
	{
		btmemset(boxNo, 0, sizeof(boxNo));
		ret = ReadData(boxNo, NULL, "can sap xep", NULL, SCAN_BOX_NO);

		if(ret == -1)
			return ret;

		if(ret != 0)
			continue;

		btmemset(send, 0, sizeof(send));
		MakeDataToSend("M302", "S03", boxNo, send);
		ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	}while(1);
}
