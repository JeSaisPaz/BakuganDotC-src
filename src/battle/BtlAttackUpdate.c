// bdc 0x08876dd0 BtlAttackUpdate
#include "bdc.h"

/* Per-frame step of one attack object. While it is alive (`endFrame == 0`) calls the per-type
   handler `g_btlAttackTypeHandlers``[type]` (`MemberFnPtr`: `this` adjusted by `delta`; a
   virtual entry dispatches through the vtable found at the offset held in `pfn`). Once ended it
   lingers until `age >= endFrame + 0x50` and is then destroyed through vtable entry 1 (deleting
   destructor, flag 3) without touching `age`; otherwise `age` is incremented. */
void BtlAttackUpdate(BtlAttack *self)
{
    if (self->endFrame == 0) {
        const MemberFnPtr *e = &g_btlAttackTypeHandlers[self->type];
        u8 *obj = (u8 *)self + e->delta;
        void *fn = e->pfn;

        if (e->index != 0) {
            const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)e->pfn);
            const VtblEntry *entry = &vtbl[e->index];

            fn = entry->fn;
            obj += entry->delta;
        }
        ((void (*)(void *))fn)(obj);
    } else if (self->age >= self->endFrame + 0x50) {
        if (self != NULL) {
            const VtblEntry *dtor = &((const VtblEntry *)self->base.vtable)[1];

            ((void (*)(void *, s32))dtor->fn)((u8 *)self + dtor->delta, 3);
        }
        return;
    }
    self->age++;
}
