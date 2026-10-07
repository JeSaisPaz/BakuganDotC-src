// bdc 0x088fd214 GameQuestCamModeBaseDtor
#include "bdc.h"

/* Destructor of the quest camera mode base class (`g_gameQuestCamModeBaseVtbl`, constructor
   `GameQuestCamModeBaseCtor`): frees every non-NULL entry of the state vector `states` (count
   `stateCount`) and clears it, frees the vector buffer, restores the parent vtable
   `g_gameQuestCamTargetVtbl` and runs the root destructor `GameQuestCamObjDtor`, then frees the
   object when bit 0 of `flags` is set. Does nothing for a NULL `self`. Out-of-range element
   accesses of the inlined bounds-checked vector accessor go to the zeroed
   `g_questCamNullState`. */

void GameQuestCamModeBaseDtor(GameQuestCamModeBase *self, u32 flags)
{
  void *state;
  void **slot;
  bool inRange;
  s32 i;

  if (self == NULL) {
    return;
  }
  self->base.base.vtbl = g_gameQuestCamModeBaseVtbl;
  for (i = 0; i < self->stateCount; i++) {
    inRange = i > -1;
    /* the first access only checks the lower bound */
    if (inRange) {
      state = self->states[i];
    } else {
      memset(&g_questCamNullState, 0, 4);
      state = g_questCamNullState;
    }
    if (state == NULL) {
      continue;
    }
    if (inRange && i < self->stateCount) {
      state = self->states[i];
    } else {
      memset(&g_questCamNullState, 0, 4);
      state = g_questCamNullState;
    }
    MemLock();
    MemFree(state, NULL, 0);
    MemUnlock();
    if (inRange && i < self->stateCount) {
      slot = &self->states[i];
    } else {
      memset(&g_questCamNullState, 0, 4);
      slot = &g_questCamNullState;
    }
    *slot = NULL;
  }
  /* the original also null-checks the member address self + 0x84, which always holds for the
     non-NULL `self` */
  if (self->states != NULL) {
    slot = self->states;
    MemLock();
    MemFree(slot, NULL, 0);
    MemUnlock();
    self->states = NULL;
  }
  if (self != NULL) {
    self->base.base.vtbl = g_gameQuestCamTargetVtbl;
    GameQuestCamObjDtor(self, 0);
  }
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(self, NULL, 0);
    MemUnlock();
  }
}
