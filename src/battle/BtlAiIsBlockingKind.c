// bdc 0x0888ef40 BtlAiIsBlockingKind
#include "bdc.h"

/* Returns 1 when raycast result `kind` counts as a real obstruction for `BtlAi`:
   always for kind 1, and for kinds 2 and 0xff only when `rayHitsSoft` is 1. */
s32 BtlAiIsBlockingKind(BtlAi *self, s32 kind)
{
    if (kind == 1) {
        return 1;
    }
    if (kind == 2 && self->rayHitsSoft == 1) {
        return 1;
    }
    if (kind == 0xff && self->rayHitsSoft == 1) {
        return 1;
    }
    return 0;
}
