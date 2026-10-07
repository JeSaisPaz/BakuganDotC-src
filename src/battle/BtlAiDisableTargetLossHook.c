// bdc 0x0888df28 BtlAiDisableTargetLossHook
#include "bdc.h"

/* Clears `g_btlAiTargetLossHook` (from `BtlMainPhaseLoad`); while it is clear
   `BtlAiMarkTargetChanged` does not call `BtlAiUpdateTargetOcclusion`. */
void BtlAiDisableTargetLossHook(void)
{
    g_btlAiTargetLossHook = 0;
}
