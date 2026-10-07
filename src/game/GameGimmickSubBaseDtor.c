// bdc 0x08a2c36c GameGimmickSubBaseDtor
#include "bdc.h"

/* Destructor of the gimmicks' secondary base class (vtable `g_gameGimmickSubBaseVtbl`, whose pointer
   lives at `+0x20` of the sub-object): reinstalls that vtable and frees the object when `flags & 1`. */

void GameGimmickSubBaseDtor(void *obj, u32 flags)
{
    GameGimmickSubBase *self = obj;

    if (obj != NULL) {
        self->vtbl = g_gameGimmickSubBaseVtbl;
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(obj, NULL, 0);
            MemUnlock();
        }
    }
}
