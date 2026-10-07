// bdc 0x088dc1e8 ActorLoadMotions
#include "bdc.h"

/* Loads the motion set of a field actor (`ActorCtor`) by its kind (`base.base.unk08`) and fills
   its motion slot table `motionSlots`. Returns at once when no motion manager exists; otherwise
   enables motion on the model (`GfxModelEnableMotion`) first.
   - Kind < 0x21 (Bakugan, list `g_charMotionNameLists``[kind]`, nothing when NULL): unless the
     list's first name is already registered (`GmoMotionIndexOfName`), loads the motion files
     (`GmoMotionLoadFile`) — on stage 1 kind 7 gets `04_wil_stay/b_stagger/defense_c.gmo` and kind
     0x1f `10_for_stay/one_shot.gmo`, else `<first name>.gmo` and `g_charMotionFiles``[kind]`; then
     allocates 297 slots and stores each name's motion index (0xffff for a NULL name).
   - Kind >= 0x21 (list `g_npcMotionLists``[kind - 0x23]`): unless the first entry's name is
     registered, loads `g_npcMotionFiles``[kind - 0x23]` for kinds < 0x30, `npc_new_robo_mot.gmo`
     for 0x51..0x53, `npc_vxs_001_mot.gmo` for 0x4e..0x50, the Mylene/boat sets for 0x54/0x55,
     `npc_jpn_003_mot.gmo` for 0x32, else `npc_mot.gmo`; then allocates 50 slots set to 0xffff and
     fills the slots named by the list, stopping at the first `slot >= 50` (a NULL list is still
     dereferenced here). */

void ActorLoadMotions(Actor *self)

{
  bool fromLow;
  void *mgr;
  s16 *slots;
  u32 kind;
  u32 cur;
  s32 stage;
  s32 i;
  u16 index;
  char **names;
  const ActorNpcMotionEntry *entry;
  char path[64];

  if (!GmoMotionMgrExists()) {
    return;
  }
  GfxModelEnableMotion(&self->base);
  kind = self->base.base.unk08;
  if (kind < 0x21) {
    names = g_charMotionNameLists[kind];
    if (names == NULL) {
      return;
    }
    if (GmoMotionIndexOfName(GmoMotionMgrGet(), names[0]) == -1) {
      stage = g_scriptGlobalVars[1];
      if (stage == 1 && self->base.base.unk08 == 7) {
        GmoMotionLoadFile(GmoMotionMgrGet(), "04_wil_stay.gmo");
        GmoMotionLoadFile(GmoMotionMgrGet(), "04_wil_b_stagger.gmo");
        GmoMotionLoadFile(GmoMotionMgrGet(), "04_wil_defense_c.gmo");
      }
      else if (stage == 1 && self->base.base.unk08 == 0x1f) {
        GmoMotionLoadFile(GmoMotionMgrGet(), "10_for_stay.gmo");
        GmoMotionLoadFile(GmoMotionMgrGet(), "10_for_one_shot.gmo");
      }
      else {
        sprintf(path, "%s.gmo", names[0]);
        GmoMotionLoadFile(GmoMotionMgrGet(), path);
        mgr = GmoMotionMgrGet();
        GmoMotionLoadFile(mgr, g_charMotionFiles[self->base.base.unk08]);
      }
    }
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    slots = MemAlloc(0x252, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->motionSlots = slots;
    for (i = 0; i < 0x129; i++) {
      index = 0xffff;
      if (names[i] != NULL) {
        index = (u16)GmoMotionIndexOfName(GmoMotionMgrGet(), names[i]);
      }
      self->motionSlots[i] = (s16)index;
    }
    return;
  }

  entry = g_npcMotionLists[kind - 0x23];
  if (entry != NULL && GmoMotionIndexOfName(GmoMotionMgrGet(), entry->name) == -1) {
    cur = self->base.base.unk08;
    if (cur < 0x30) {
      GmoMotionLoadFile(GmoMotionMgrGet(), g_npcMotionFiles[kind - 0x23]);
    }
    else if (cur >= 0x51 && cur < 0x54) {
      GmoMotionLoadFile(GmoMotionMgrGet(), "npc_new_robo_mot.gmo");
    }
    else if (cur >= 0x4e && cur < 0x51) {
      GmoMotionLoadFile(GmoMotionMgrGet(), "npc_vxs_001_mot.gmo");
    }
    else if (cur == 0x54) {
      GmoMotionLoadFile(GmoMotionMgrGet(), "f4_quest_boat01_mylene_mot.gmo");
    }
    else if (cur == 0x55) {
      GmoMotionLoadFile(GmoMotionMgrGet(), "f4_quest_boat01_mot.gmo");
    }
    else if (cur == 0x32) {
      GmoMotionLoadFile(GmoMotionMgrGet(), "npc_jpn_003_mot.gmo");
    }
    else {
      GmoMotionLoadFile(GmoMotionMgrGet(), "npc_mot.gmo");
    }
  }
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  slots = MemAlloc(100, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->motionSlots = slots;
  for (i = 0; i < 0x32; i++) {
    self->motionSlots[i] = (s16)0xffff;
  }
  for (i = 0; i < 0x32; i++) {
    if (entry->slot >= 0x32) {
      return;
    }
    index = (u16)GmoMotionIndexOfName(GmoMotionMgrGet(), entry->name);
    self->motionSlots[entry->slot] = (s16)index;
    entry++;
  }
}
