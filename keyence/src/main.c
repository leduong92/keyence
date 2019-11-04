#if 0
/********************************************************************************
File name: main.c
Version: Ver.1.00
Operation explanation: BT-1000 Series sample application
Note: This is a sample program, so it is not supported by KEYENCE.
Copyright(c) 2013 KEYENCE CORPORATION. All rights reserved.

********************************************************************************/
#endif

// Include header file
#include "btlib.h"
#include "btext.h"
#include "version.h"
#include "Common.h"
#include "App200.h"
#include "App300.h"
#include "App400.h"
#include "App500.h"
//#include "nokbt1500.h"
#include "P:\pdrv\barlib\Keyence\nokbt1500.h"
//#include "BCR.h"

#define NOTEXTERN

// Type definitions
typedef struct s_logdata {
	int line;
	char data[20];
} LOGDATA;


void CommandDistribution(char* command);
int ReadFunction(void);
void App200Command(int cmd);
void App300Command(int cmd);
void App400Command(int cmd);
void App500Command(int cmd);

int main(void) 
{
	int ret;

	ENABLE_WAITKEYS key;


	char send[64], rcv[64];
	int keycode;

	AllInitial(TRUE);

	btmemset(&key, 0, sizeof(ENABLE_WAITKEYS));
	key.key1 = TRUE;
	key.key2 = TRUE;

	while(1)
	{
		LcdClear();

		keycode = StartDisplay(MAINWNDNAME, PGM_NAME,APP_VERSION, "","","       VNN (c) 2018"," L: Run", "F3: Set IP");

		switch(keycode)
		{
		case KEYCODE_F3:
			//if(CheckAdminPassword()) // Check password for setting Wireless
			//	break; 
			if(InputPassword())
				break;

			WLAN_setting();
			break;
		case KEYCODE_F2:
			if(CheckAdminPassword())
				break;
			SetLocation();
			break;
		case KEYCODE_L:
		case KEYCODE_ENT:
			MainProcess();
			break;
		}
	}

	return (ret);
}

int MainProcess(void)
{
	DINPUT_SETTING set;

	char buf[1024];
	char send[128], rcv[128];
	int keyCode;
	int ret;
	char loc[3+1], userId[10], containerNo[5], containerQty[5], operationCode[6+1], pressNo[3+1], jobClassCode[3+1],jobtag[12],data[50];
	char author[2];
	char box[BOX_LENGTH];
	char str[302];

	BM_USER user;

	MasterUpd("user", "");
	MasterUpd("location", "");
	MasterUpd("author","");

	btmemset(&set, 0, sizeof(set));
	set.mode = 1;
	set.buffer = buf;
	set.bufsize = sizeof(buf);

	btstrcpy(set.title, MAINWNDNAME);
	//btstrcpy(set.l1, PGM_NAME);
	btstrcpy(set.l1, "Quet ma nhan vien");
	set.c2 = CLR_BLUE;

	do
	{
		btmemset(buf, 0 , sizeof(buf));
		ret = DataInputBt3(&set, 3);

		if(ret)
			return -1;
	}
	while(btstrlen(buf) < 7 || btstrlen(buf) > 9);

	btmemset(userId, 0, sizeof(userId));
	btmemcpy(userId, buf, 10);

	btmemset(loc, 0, sizeof(loc));

	MasterUpd("user", userId);


	btmemset(send, 0, sizeof(send));
	MakeDataToSend("M100", "   ", "", send);

	//Send data to Bcr Server
	ret = DataSend(send, btstrlen(send), rcv, sizeof(rcv), FALSE);

	if (ret != 2)
		return -1;

	btmemcpy(&user, rcv, sizeof(BM_USER));
	btmemset(buf,0, sizeof(buf));
	btmemset(author, 0, sizeof(author));
	
	btmemcpy(buf, user.Name,30);
	btmemcpy(author, user.AdminSign, 1);

	MasterUpd("user", userId);
	MasterUpd("author", author);

	ReadFunction();
}


int ReadFunction(void)
{
	DINPUT_SETTING set;
	btmemset(&set, 0, sizeof(set));
	set.mode = 1;
	set.buffer = buf;
	set.bufsize = sizeof(buf);
	btstrcpy(set.title, MAINWNDNAME);
	btstrcpy(set.l1, "Quet chuc nang");
	btstrcpy(set.l2, "[...]");

	do
	{
		btmemset(buf, 0, sizeof(buf));
		ret = DataInputBt(&set);

		if(btmemcmp(buf,"M",1))
			btsprintf(buf,"");

		if(ret)
			return -1;

		//}while(btstrlen(buf) != 7);
	}while(btmemcmp(buf,"M",1));

	CommandDistribution((char*) buf);
}

void CommandDistribution(char* command)
{
	char buf[5], loc[3+1];
	int ret, cmd, cmd1;

	btmemset(loc, 0, sizeof(loc));
	btmemset(buf,0, sizeof(buf));

	btmemcpy(buf, command + 1, 3);
	btmemcpy(loc, command + 4, 3);
	MasterUpd("location", loc);

	cmd = atoi(buf);
	cmd1 = cmd / 100;

	switch(cmd1)
	{
	case 1: 
		break;
	case 2:
		App200Command(cmd);
		break;
	case 3:
		App300Command(cmd);
		break;
	case 4:
		App400Command(cmd);
		break;
	case 5:
		App500Command(cmd);
		break;
	default:
		btMessageBoxEx("This function doesn't exists!!!", BT_MB_OKONLY,"Command Dist.", BT_FNT_NORMAL);
		break;
	};

	ReadFunction();
}

void App200Command(int cmd)
{
	int ret;

	switch(cmd)
	{
	case 200:
		ret = ChangePalleteForOR();
		break;
		/*case 201:
		ret = GetBoxAndReceiveByPclNo();
		break;*/
	case 203: //Tim kiem thung hang bi thieu tren Pallete
		ret = FindBoxLostInPallete();
		break;
	case 204: //Dua pallet len cong, luu so container va so lan xuat
		ret = LoadPalleteToContainer();
		break;
	case 205: //Khoa Container
		ret = SetContainerLockSignForPallete();
		break;
	default:
		btMessageBoxEx("This function doesn't exists!!!", BT_MB_OKONLY,"App200Command", BT_FNT_NORMAL);
		break;
	};
}

void App300Command(int cmd)
{
	int ret;
	char loc[3+1];

	btmemset(loc, 0, sizeof(loc));
	MasterFind("location", loc);

	switch(cmd)
	{
	//20191015 - Chuc nang in phieu PCL OR se khong su dung nua.
	//case 300: //IN PHIEU PCL NHAP KHO CHO OR
	//	ret = PrintPCLOR();
	//	break;
	case 301: // NHAN HANG THANH PHAM
		//ret = InStock();
		if(!btmemcmp(loc, "OR1", 3))
			ret = InStockForOR();
		else if(!btmemcmp(loc, "OS1", 3))
			ret = InStock();

		break;
	case 302: //SAP XEP HANG LEN PALLETE
		ret = ArrangeBoxToPallet();
		break;
	case 303://IN MAT SHIPPING TO/ SHIPPING MARK
		if(!btmemcmp(loc, "OS1", 3))
		{
			ret = PrintShippingToOS();
		}
		else if(!btmemcmp(loc, "OR1", 3))
		{
			ret = PrintShippingToOR();
		}
		break;
	//case 304: // RESER HANG CHO OS
	//	ret = Reservation();
	//	break;
	case 305: // SHIP HANG OS/OR
		ret = ShipOSOR();
		break;
	case 306: //KIEM TON KHO THANH PHAM
		if(!btmemcmp(loc, "OS1", 3))
		{
			ret = CheckInventoryForOS();
		}
		else if(!btmemcmp(loc, "OR1", 3))
		{
			ret = CheckInventoryForOR();
		}
		break;
	
	default:
		btMessageBoxEx("This function doesn't exists!!!", BT_MB_OKONLY,"App300Command", BT_FNT_NORMAL);
		break;
	};
}

void App400Command(int cmd)
{
	int ret;
	char loc[3+1], author[2];

	btmemset(loc, 0, sizeof(loc));
	MasterFind("location", loc);

	btmemset(author, 0, sizeof(author));
	MasterFind("author", author);

	switch(cmd)
	{

	case 400: //TRA HANG VE SAN XUAT
		if(!btmemcmp(loc, "OS1", 3))
		{
SCAN_AGAIN:
			keyCode = StartDisplay("Chon chuc nang", "1. Tra ve SX", "2. Cho SX Muon",  "3. Lay Thung Ve Kho", "", "", "", "");

			switch(keyCode)
			{
			case KEYCODE_1: // TRA VE SAN XUAT LUON
				ret = ReturnBoxToProduction();
				break;
			case KEYCODE_2: // CHO SAN XUAT MUON LAI THUNG HANG
				ret = BorrowBoxForOS();
				break;
			case KEYCODE_3: // NHAN LAI THUNG HANG DA CHO SAN XUAT MUON
				ret = TakeBoxBackForOS();
				break;
			case KEYCODE_L:
				break;
			default:
				btMessageBoxEx("Khong co chuc nang", BT_MB_OKONLY, "M400OS1", BT_FNT_NORMAL);
				goto SCAN_AGAIN;
				break;
			};
		}
		else if(!btmemcmp(loc, "OR1", 3))
		{
			ret = ReturnBoxToProduction();
		}

		LcdClear();
		break;
	case 401: //TRA THUNG HANG TU KHU VUC INSTOCK RA RECEIVE
		if (!btmemcmp(loc, "OS1", 3))
			ret = OSReturnBoxFromInstockToReceive();
		else 
			ret = ORReturnBoxFromInstockToReceive();

		break;
	case 402: //HOAN DOI GIUA 2 SHIIPING TO
		ret = TransferPalleteTempInShippingTo();
		break;
	case 403: //IN MAT PHIEU SHIPPING TO	
		if(btmemcmp(author, "A", 1))
		{
			btMessageBoxEx("Ban khong the su dung chuc nang nay.", BT_MB_OKONLY,"M403", BT_FNT_NORMAL);
			break;
		}
		else
			ret = PrintLostShippingToOR();	

		break;
	case 404://NHAN HANG THANH PHAM NGAY KIEM KE
		ret = InStockWhenCheckInventory();
		break;
	case 405: //IN PHIEU SHIPPING TO CHI DINH
		ret = PrintShippingToSpecify();			
		break;
	case 406://HOAN DOI THUNG TREN PALLETE
		ret = SwitchBoxInPallete();
		break;
	case 407: //IN LAI PHIEU SHIPPING TO
		ret = PrintReprintShippingTo();
		break;
	case 408: //DOI SO THU TU PHIEU SHIPPING TO
		ret = TranferNumPLT();
		break;
	case 409: // CHINH SUA NGAY EDT, EDTVN, SHIP_SEQ
		ret = ChangeEtdAndNOT();
		break;
	case 411: //IN LAI PHIEU SHIPPING TO(HANG DA SHIP)
		ret = PrintLostShippingToOS();
		break;
	case 412: //INSTOCK TO RESER/SHIP
		ret = TransferInstockToReserShip();
		break;
	case 414://THEM HANG TU KHU VUC NGOAI VAO SHIPPING TO
		ret = AddBoxFromOutsideIntoShippingTo();
		break;	
	case 415:
		ret = ReturnBoxFromShippingToReceived();
		break;
	case 416: //KIEM TRA BOX CO THE DUA VAO SHIPPING TO NAO
		ret = CheckBoxCanInsertToShippingTo();
		break;
	//case 417://HOAN DOI SAU RESER
	//	ret = TransferAfterReser();
	//	break;
	case 418://Tim kiem thung hang
		ret = FindBox();
		break;
	case 419://Tim kiem thung hang
		ret = CheckStatusOfShippingTo();
		break;
	case 420://GHI NHAN DU LIEU DAI KIEN / HUY DAI KIEN
		ret = WhPackageRecord();
		break;
	default:
		btMessageBoxEx("This function doesn't exists!!!", BT_MB_OKONLY,"App400Command", BT_FNT_NORMAL);
		break;
	};
}


void App500Command(int cmd)
{
	int ret;

	switch(cmd)
	{

	default:
		btMessageBoxEx("This function doesn't exists!!!", BT_MB_OKONLY,"App500Command", BT_FNT_NORMAL);
		break;
	};
}