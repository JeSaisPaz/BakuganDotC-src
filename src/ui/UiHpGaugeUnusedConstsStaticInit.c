// bdc 0x0888bf90 UiHpGaugeUnusedConstsStaticInit
#include "bdc.h"

/* Static initialiser at the end of the HUD hit-point gauge translation unit: stores the floats
   196096.0, 784384.0, 2097152.0 and 8388608.0 in `g_uiHpGaugeUnusedConsts`. */

void UiHpGaugeUnusedConstsStaticInit(void)

{
  g_uiHpGaugeUnusedConsts[0] = 196096.0f;
  g_uiHpGaugeUnusedConsts[1] = 784384.0f;
  g_uiHpGaugeUnusedConsts[2] = 2097152.0f;
  g_uiHpGaugeUnusedConsts[3] = 8388608.0f;
  return;
}
