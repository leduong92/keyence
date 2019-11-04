#ifndef APP400_H
#define APP400_H

#include "P:\pdrv\barlib\Keyence\nokbt1500.h"
#include "Common.h"

//============================== KHOA ============================= 
int ReturnBoxToProduction(void);

int BorrowBoxForOS(void);

int TakeBoxBackForOS(void);

int InStockWhenCheckInventory(void);

int SwitchBoxInPallete(void);

int AddBoxFromOutsideIntoShippingTo(void);

int ReturnBoxFromShippingToReceived(void);
//==========================  End of KHOA ========================= 


//============================== DUONG ============================= 
int OSReturnBoxFromInstockToReceive(void);

int ORReturnBoxFromInstockToReceive(void);

int ChangeEtdAndNOT(void);

//==========================  End of DUONG ========================= 


//============================== HIEU ============================== 
int PrintLostShippingToOR(void);

int PrintShippingToSpecify(void);

int PrintReprintShippingTo(void);

int PrintLostShippingToOS(void);

int TranferNumPLT(void);
//==========================  End of HIEU ========================== 


//=============================== MY =============================== 
int TransferPalleteTempInShippingTo(void);
int TransferInstockToReserShip(void);
int TransferAfterReser(void);
int CheckBoxCanInsertToShippingTo(void);
int CheckStatusOfShippingTo(void);
//===========================  End of MY =========================== 

int FindBox(void);

int WhPackageRecord(void);
#endif