// bdc 0x08a2c290 GameGimmickCameraRunState
#include "bdc.h"

/* Runs the current state of the surveillance camera gimmick (`GameGimmickCameraCtor`): calls the
   `MemberFnPtr` `g_gameGimmickCameraStateFns[obj->base.state]` (`{s16 delta, s16 vindex, fn}`, virtual
   when `vindex != 0`) on `obj + delta`. */

void GameGimmickCameraRunState(GameGimmickCamera *obj)
{
    const MemberFnPtr *member = &g_gameGimmickCameraStateFns[obj->base.state];
    u8 *self = (u8 *)obj + member->delta;
    void *fn = member->pfn;

    if (member->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(self + (intptr_t)member->pfn);
        const VtblEntry *entry = &vtbl[member->index];

        fn = entry->fn;
        self += entry->delta;
    }
    ((void (*)(void *))fn)(self);
}
