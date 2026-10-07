// bdc 0x0884c8b4 BtlIsDemoRunning
#include "bdc.h"

/* True while a battle demo task exists: the intro demo (id 0x65, `BtlDemoCtor`) or the Bakugan
   appear demo (id 0x67). */
bool BtlIsDemoRunning(void)
{
    return CoreTaskExists(0x65) != 0 || CoreTaskExists(0x67) != 0;
}
