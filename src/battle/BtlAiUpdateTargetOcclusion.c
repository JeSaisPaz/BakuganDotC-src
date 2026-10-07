// bdc 0x0888f0dc BtlAiUpdateTargetOcclusion
#include "bdc.h"

/* Recomputes whether the current target of the CPU AI is hidden: clears `targetOccluded`; with a
   target, casts owner → target (`BtlAiLineOfSightBlocked`). A blocked ray sets move flag 0x400,
   cleared again when the blocking kind is 3 or 4, or when `BtlAiIsBlockingKind` accepts the kind,
   an enemy is within 550 units (`BtlAiHasEnemyWithin`) and the target is a Bakugan (vtable slot
   10, `BtlBakuganIsBakugan`). A clear ray leaves `moveFlags` unchanged. Finally `targetOccluded`
   = move flag 0x400 set (also when there is no target). */

void BtlAiUpdateTargetOcclusion(BtlAi *self)
{
    s32 kind;
    BtlBakugan *target;
    const VtblEntry *vtbl;

    self->targetOccluded = 0;
    if (self->target != NULL) {
        kind = BtlAiLineOfSightBlocked(self, self->owner, self->target);
        if (kind != 0) {
            self->moveFlags |= 0x400;
            if (kind == 3 || kind == 4) {
                self->moveFlags &= ~0x400u;
            }
            if (BtlAiIsBlockingKind(self, kind) != 0 && BtlAiHasEnemyWithin(550.0f, self) != 0) {
                target = self->target;
                if (target != NULL) {
                    vtbl = (const VtblEntry *)target->base.base.vtable;
                    if (((int (*)(void *))vtbl[10].fn)((u8 *)target + vtbl[10].delta) != 0) {
                        self->moveFlags &= ~0x400u;
                    }
                }
            }
        }
    }
    self->targetOccluded = (self->moveFlags & 0x400) != 0;
}
