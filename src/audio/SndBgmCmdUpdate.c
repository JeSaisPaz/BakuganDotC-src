// bdc 0x089c8980 SndBgmCmdUpdate
#include "bdc.h"

/* Per-frame update of a `SndBgmCmd` (vtable `0x08af526c` slot 2, called by
   `CoreTaskManagerUpdate`): while `done` is 0 it calls the step function of the command's `kind`
   through the pointer-to-member table at `0x08ac5884` (8-byte `{s16 delta, s16 vindex, u32 fn}`
   entries, kind 1 `SndBgmCmdStepPreload`, 2 `SndBgmCmdStepPlayLoaded`, 3 `SndBgmCmdStepPlay`,
   4 `SndBgmCmdStepStop`; `this` is adjusted by `delta`, a non-zero `vindex` would select a
   virtual slot but is never used); once `done` is set it removes and destroys the task
   (`CoreTaskRemove` with the destroy flag). */

void SndBgmCmdUpdate(SndBgmCmd *cmd)
{
    const MemberFnPtr *member;
    u8 *self;
    void *fn;

    if (cmd->done != 0) {
        CoreTaskRemove(&cmd->base, true);
        return;
    }
    member = &g_sndBgmCmdStepFns[cmd->kind];
    self = (u8 *)cmd + member->delta;
    fn = member->pfn;
    if (member->index != 0) {
        const VtblEntry *vtbl = *(const VtblEntry **)(self + (intptr_t)member->pfn);
        const VtblEntry *entry = &vtbl[member->index];

        fn = entry->fn;
        self += entry->delta;
    }
    ((void (*)(void *))fn)(self);
}
