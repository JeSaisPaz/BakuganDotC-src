// bdc 0x08952478 UiBattleRuleSelectDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the UiBattleRuleSelect screen (task id 340): reinstalls vtable
   `g_uiBattleRuleSelectVtbl`, waits for the GE. When a NetPlay manager exists and
   `SaveGetProfileFlag0` is set, it also deletes the object in `base.unk24` (virtual dtor, flag 3),
   clears it, clears the remote pad's stick-emulates-d-pad byte and restores `base.pad` to `g_padState`.
   Always stores its task id in `g_lastScreenTaskId` (last closed screen); then
   `UiScreenDtor``(this, 0)` and frees the object when `flags & 1`. */

typedef struct RuleSelectObjVSlot {
  s16 adjust;
  s16 pad;
  void (*fn)(void *, s32);
} RuleSelectObjVSlot;

typedef struct RuleSelectObjVTable {
  u8 unk00[8];
  RuleSelectObjVSlot dtor;
} RuleSelectObjVTable;

typedef struct RuleSelectObj {
  u8 unk00[0x4c];
  RuleSelectObjVTable *vtable;
} RuleSelectObj;

void UiBattleRuleSelectDtor(UiBattleRuleSelect *self, u32 flags)
{
  if (self != NULL) {
    (self->base).base.vtable = g_uiBattleRuleSelectVtbl;
    GfxWaitGeIdle();
    if (NetPlayHasManager() && SaveGetProfileFlag0()) {
      RuleSelectObj *obj = *(RuleSelectObj **)&(self->base).unk24;

      if (obj != NULL) {
        RuleSelectObjVSlot *slot = &obj->vtable->dtor;

        slot->fn((u8 *)obj + slot->adjust, 3);
        (self->base).unk24 = 0;
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
