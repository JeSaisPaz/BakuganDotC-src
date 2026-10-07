// bdc 0x08893580 BtlAiFollowLayerCheck
#include "bdc.h"

/* Check method of behaviour layer 1 of `BtlAi` (follow an ally, `MemberFnPtr` at
   `0x08a802f0`): returns 0 in script rule mode 2 (`g_scriptGlobalVars` entry 8). When the layer
   has no `leader` yet, asks the owner's virtual entry 13 (non-zero for a `BtlCpuUnit`); if it
   answers 0, returns 0, otherwise takes the owner's `ally` (resolved by
   `BtlCpuUnitResolveAlly`) as `leader` when it is non-NULL. Then returns the layer's virtual
   entry 2 (IsActive). */
s32 BtlAiFollowLayerCheck(BtlAi *self)
{
    const VtblEntry *entry;

    if (g_scriptGlobalVars[8] == 2) {
        return 0;
    }
    if (self->follow.leader == NULL) {
        BtlBakugan *owner = self->owner;
        BtlCpuUnit *cpu = NULL;
        BtlBakugan *ally;

        entry = &((const VtblEntry *)owner->base.base.vtable)[13];
        if (((s32 (*)(void *))entry->fn)((u8 *)owner + entry->delta) != 0) {
            cpu = (BtlCpuUnit *)self->owner;
        }
        if (cpu == NULL) {
            return 0;
        }
        ally = cpu->ally;
        if (ally != NULL) {
            self->follow.leader = ally;
        }
    }
    entry = &self->follow.base.vtbl[2];
    return ((s32 (*)(void *))entry->fn)((u8 *)&self->follow + entry->delta);
}
