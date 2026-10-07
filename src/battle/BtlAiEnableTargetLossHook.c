// bdc 0x0888df34 BtlAiEnableTargetLossHook
#include "bdc.h"

/* Sets the global flag `g_btlAiTargetLossHook` (from `BtlMainTaskCtor`); while it is set
   `BtlAiMarkTargetChanged` also calls `BtlAiUpdateTargetOcclusion`. */

void BtlAiEnableTargetLossHook(void)
{
    g_btlAiTargetLossHook = 1;
}
