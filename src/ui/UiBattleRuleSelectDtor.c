// bdc 0x08952478 UiBattleRuleSelectDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiBattleRuleSelect screen (task id 340): reinstalls vtable
   `g_uiBattleRuleSelectVtbl`, waits for the GE. When a NetPlay manager exists and
   `SaveGetProfileFlag0` is set, it also deletes the net-play pad `base.netPad` (virtual dtor, flag 3),
   clears it, clears the remote pad's stick-emulates-d-pad byte and restores `base.pad` to `g_padState`,
   like `UiPauseDtor`.
   Always stores its task id in `g_lastScreenTaskId` (last closed screen); then
   `UiScreenDtor``(this, 0)` and frees the object when `flags & 1`. */

void UiBattleRuleSelectDtor(UiBattleRuleSelect *self, u32 flags)
{
  if (self != NULL) {
    (self->base).base.vtable = g_uiBattleRuleSelectVtbl;
    GfxWaitGeIdle();
    if (NetPlayHasManager() && SaveGetProfileFlag0()) {
      PadState *netPad = (self->base).netPad;

      if (netPad != NULL) {
        const VtblEntry *dtor = &((const VtblEntry *)netPad->vtable)[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)netPad + dtor->delta, 3);
        (self->base).netPad = NULL;
      }
      ((NetPlay *)NetPlayGetManager())->remotePad->stickEmulatesDpad = 0;
      (self->base).pad = g_padState;
    }
    g_lastScreenTaskId = (self->base).base.id;
    UiScreenDtor(&self->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, NULL, 0);
      MemUnlock();
    }
  }
}
