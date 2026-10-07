// bdc 0x08892c40 BtlAiIsNonNull
#include "bdc.h"

/* Returns 1 when `ptr` is non-NULL. Condition helper of `BtlAiRunReactionRules` (e.g. for the
   incoming attack from `BtlAiFindIncomingAttack`). */
s32 BtlAiIsNonNull(BtlAi *self, void *ptr)
{
    (void)self;
    return ptr != NULL;
}
