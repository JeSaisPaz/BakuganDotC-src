// bdc 0x08866068 BtlInitBakuganList
#include "bdc.h"

/* Starts a battle's unit list: sets `g_btlBakuganList` to `list`, resets
   `g_btlAnimPhaseCounter` to 0 and the unit-id counter `g_btlNextUnitId` to 1. Called by
   `GameFieldPhaseLoad`; compare `BtlSetBakuganList`. */
void BtlInitBakuganList(void *list)
{
    g_btlAnimPhaseCounter = 0;
    g_btlNextUnitId = 1;
    g_btlBakuganList = list;
}
