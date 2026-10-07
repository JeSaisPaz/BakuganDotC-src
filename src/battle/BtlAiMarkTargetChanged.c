// bdc 0x0888f1d4 BtlAiMarkTargetChanged
#include "bdc.h"

/* Sets the target-changed byte of `BtlAi` to 1 and, while
   `g_btlAiTargetLossHook` is set, refreshes the target occlusion state at once
   (`BtlAiUpdateTargetOcclusion`). */
void BtlAiMarkTargetChanged(BtlAi *self)
{
    self->targetChanged = 1;
    if (g_btlAiTargetLossHook != 0) {
        BtlAiUpdateTargetOcclusion(self);
    }
}
