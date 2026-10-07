// bdc 0x08878db4 BtlAttackHitPointStaticInit
#include "bdc.h"

/* Static initialiser of the attack translation unit: zeroes the hit point of the shared attack
   hit record (filled by `BtlAttackSweepHit` and read by the attack updates). */
void BtlAttackHitPointStaticInit(void)
{
    g_btlAttackHitPoint.x = 0.0f;
    g_btlAttackHitPoint.y = 0.0f;
    g_btlAttackHitPoint.z = 0.0f;
    g_btlAttackHitPoint.w = 0.0f;
}
