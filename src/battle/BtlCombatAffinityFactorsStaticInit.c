// bdc 0x08889cd0 BtlCombatAffinityFactorsStaticInit
#include "bdc.h"

/* Static initialiser of the combat translation unit: fills the attribute-affinity factor pairs
   `g_btlAffinityFactors` (`{strong, weak}` per tier) used by `BtlGetAttributeAffinity`. */
void BtlCombatAffinityFactorsStaticInit(void)
{
    g_btlAffinityFactors[0][0] = 1.4f;
    g_btlAffinityFactors[0][1] = 0.6f;
    g_btlAffinityFactors[1][0] = 1.5f;
    g_btlAffinityFactors[1][1] = 0.3f;
    g_btlAffinityFactors[2][0] = 10.0f;
    g_btlAffinityFactors[2][1] = 0.5f;
    g_btlAffinityFactors[3][0] = 1.5f;
    g_btlAffinityFactors[3][1] = 0.5f;
}
