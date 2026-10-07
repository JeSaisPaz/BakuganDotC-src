// bdc 0x08a2ce20 GameQuestCamDirSwitchUpdate
#include "bdc.h"

/* Update of the quest camera mode's sub-state 2 (`GameQuestCamSubState`, vtable 0x08af6f90, entry 4):
   immediately exits the current sub-state and switches to the sub-state index `next` (stored by
   its enter method `GameQuestCamDirSwitchEnter` from the argument), setting `mode->stateIndex`
   and entering the new state. An out-of-range index selects the zeroed dummy
   `g_questCamNullState`. */

void GameQuestCamDirSwitchUpdate(void *state, const float *move, float dt)

{
  GameQuestCamSubState *self = (GameQuestCamSubState *)state;
  GameQuestCamModeBase *mode = self->mode;
  s32 next = self->next;
  /* UB (original binary): the enter argument is the uninitialised stack slot at sp+4 */
  s32 ub_arg;
  GameQuestCamSubState *cur = (GameQuestCamSubState *)mode->state;
  GameQuestCamSubState *newState;

  if (cur != (GameQuestCamSubState *)0) {
    ((void (*)(void *))cur->vtbl[2].fn)((u8 *)cur + cur->vtbl[2].delta);
  }
  mode->stateIndex = next;
  if (next >= 0 && next < mode->stateCount) {
    newState = (GameQuestCamSubState *)mode->states[next];
  }
  else {
    memset(&g_questCamNullState, 0, 4);
    newState = (GameQuestCamSubState *)g_questCamNullState;
  }
  mode->state = newState;
  ((void (*)(void *, void *))newState->vtbl[1].fn)((u8 *)newState + newState->vtbl[1].delta, &ub_arg);
}
