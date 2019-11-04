#ifndef APP300_H
#define APP300_H

#include "P:\pdrv\barlib\Keyence\nokbt1500.h"
#include "Common.h"

int InStock(void);

int InStockForOR(void);

int CheckInventoryForOS(void);

int CheckInventoryForOR(void);

int PrintPCLOR (void);

int ShipOSOR(void);

int Reservation(void);

int PrintShippingToOS(void);

int PrintShippingToOR(void);

int ArrangeBoxToPallet(void);
#endif