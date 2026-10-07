// bdc 0x0884c2e4 BtlSetCloseUpDistance
#include "bdc.h"

/* Finds the battle main task (id 100) and calls `BtlMainSetCloseUpDistance`. */
void BtlSetCloseUpDistance(float dist)
{
    BtlMain *self = CoreTaskFind(100);
    if (self != NULL) {
        BtlMainSetCloseUpDistance(dist, self);
    }
}
