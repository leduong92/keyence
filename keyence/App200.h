#ifndef APP200_H
#define APP200_H

#include "P:\pdrv\barlib\Keyence\nokbt1500.h"
#include "Common.h"

typedef struct{
  char box[15];
} STR_BOX;

typedef struct{
	STR_BOX ListBox[200];
} STR_BOXES;

int ChangePalleteForOR(void);

int FindBoxLostInPallete(void);

int LoadPalleteToContainer(void);

int SetContainerLockSignForPallete(void);


#endif