// bdc 0x0884df58 BtlIsCutInRunning
#include "bdc.h"

/* True while task 0x1e1 (cut-in) or 0x1e0 exists. */
bool BtlIsCutInRunning(void)
{
    return CoreTaskExists(0x1e1) != 0 || CoreTaskExists(0x1e0) != 0;
}
