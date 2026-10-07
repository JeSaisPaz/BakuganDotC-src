// bdc 0x0888739c BtlGetAttributeAffinity
#include "bdc.h"

/* Attribute affinity multiplier: 1.0 when the battle rule mode (script global var 8) is 2 or either
   attribute is 6 (none); otherwise the tier's strong factor when `attackerAttr` beats `targetAttr`
   (`g_btlAttributeBeats`), the tier's weak factor when `targetAttr` beats `attackerAttr`, else
   1.0. Factors from `g_btlAffinityFactors`. */
float BtlGetAttributeAffinity(s32 attackerAttr, s32 targetAttr, s32 tier)
{
    if (g_scriptGlobalVars[8] == 2 || attackerAttr == 6 || targetAttr == 6) {
        return 1.0f;
    }
    if (g_btlAttributeBeats[attackerAttr] == targetAttr) {
        return g_btlAffinityFactors[tier][0];
    }
    if (g_btlAttributeBeats[targetAttr] == attackerAttr) {
        return g_btlAffinityFactors[tier][1];
    }
    return 1.0f;
}
