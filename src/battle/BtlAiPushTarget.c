// bdc 0x0888f284 BtlAiPushTarget
#include "bdc.h"

/* Temporarily retargets the AI: when no target is saved yet (`savedTarget` NULL), saves the
   current `target` in `savedTarget` and switches to `unit` (`BtlAiSetTarget`); otherwise does
   nothing. Undone by `BtlAiPopTarget`. */
void BtlAiPushTarget(BtlAi *self, void *unit)
{
    if (self->savedTarget == NULL) {
        self->savedTarget = self->target;
        BtlAiSetTarget(self, unit);
    }
}
