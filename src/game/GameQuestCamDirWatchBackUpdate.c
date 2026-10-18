// bdc 0x08a2cad0 GameQuestCamDirWatchBackUpdate
#include "bdc.h"

/* Update of the quest camera mode's sub-state 0 (`GameQuestCamSubState`, vtable 0x08af6f40, entry
   4, created by `GameQuestCamModeBaseCtor`): while the followed target moves
   (`GameQuestCamModeTargetMoved`) against the current path direction (`dot(move, dir) < 0`, `dir`
   = the `step` of the look spring the mode's `entry` slot holds, `&ctrl->look`), accumulates `dt` in `mode->watchTime`;
   otherwise (not moving, or dot not negative) the timer is reset to 0. Once the timer exceeds
   `mode->watchDelay`, it is reset, the current sub-state is exited (vtable entry 2), `stateIndex`
   becomes 2 and sub-state 2 (`GameQuestCamDirSwitchUpdate`) is entered (vtable entry 1) with
   argument 1 (out of range: the zeroed dummy `g_questCamNullState`). */

void GameQuestCamDirWatchBackUpdate(void *state, const float *move, float dt)
{
  GameQuestCamSubState *self = (GameQuestCamSubState *)state;
  GameQuestCamModeBase *mode;
  GameQuestCamSubState *cur;
  GameQuestCamSubState *newState;
  ScePspFVector4 dir;
  s32 next;
  s32 arg;
  float dot;
  float t;

  if (GameQuestCamModeTargetMoved(self->mode) == 0) {
    self->mode->watchTime = 0.0f;
    return;
  }
  dir = (*(GameQuestCamSpring **)self->mode->entry)->step;
  dot = VfDot3(dir.x, dir.y, dir.z, move[0], move[1], move[2]);
  if (dot < 0.0f) {
    t = self->mode->watchTime + dt;
  } else {
    t = 0.0f;
  }
  self->mode->watchTime = t;
  if (!(self->mode->watchTime <= self->mode->watchDelay)) {
    self->mode->watchTime = 0.0f;
    mode = self->mode;
    next = 2;
    arg = 1;
    cur = (GameQuestCamSubState *)mode->state;
    if (cur != NULL) {
      ((void (*)(void *))cur->vtbl[2].fn)((u8 *)cur + cur->vtbl[2].delta);
    }
    mode->stateIndex = next;
    if (next > -1 && next < mode->stateCount) {
      newState = (GameQuestCamSubState *)mode->states[next];
    } else {
      memset(&g_questCamNullState, 0, 4);
      newState = (GameQuestCamSubState *)g_questCamNullState;
    }
    mode->state = newState;
    ((void (*)(void *, void *))newState->vtbl[1].fn)((u8 *)newState + newState->vtbl[1].delta, &arg);
  }
}
