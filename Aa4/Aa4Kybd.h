#ifndef __AA4KYBD_H
#define __AA4KYBD_H

#include "pkybv100.h"

void interrupt far keyboard(...);
#define shifted SHIFT_VALUE
void InstallKyb(void);
void UnsetKyb(void);
void SetKyb(void);
void ClrKybBuf(void);
void ClrFlags(void);

#endif
