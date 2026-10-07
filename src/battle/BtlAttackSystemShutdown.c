// bdc 0x08878d34 BtlAttackSystemShutdown
#include "bdc.h"

/* Destroys every attack in `g_btlAttackList` (`CoreObjectListClear`) and releases the attacks'
   shared sound object `g_btlAttackSndObject` created by `BtlAttackSystemInit`
   (`SndObjectRelease`). */
void BtlAttackSystemShutdown(void)
{
    CoreObjectListClear(&g_btlAttackList);
    SndObjectRelease(SndGetObjectList(), g_btlAttackSndObject);
}
