// bdc 0x08a2cc78 GameQuestCamDirWatchFwdUpdate
#include "bdc.h"

/* Update of the quest camera mode's sub-state 1 (`GameQuestCamSubState`, vtable 0x08af6f68, entry
   4): while the followed target moves (`GameQuestCamModeTargetMoved`) along the current path
   direction (`dot(move, dir) > 0`, `dir` at +0x50 of the object the mode's `entry` points to),
   accumulates `dt` in `mode->watchTime`; otherwise (not moving, or dot <= 0) the timer is reset to
   0. Once the timer exceeds `mode->watchDelay`, it is reset, the current sub-state is exited
   (vtable entry 2), `stateIndex` becomes 2 and sub-state 2 (`GameQuestCamDirSwitchUpdate`) is
   entered (vtable entry 1) with argument 0 (out of range: the zeroed dummy
   `g_questCamNullState`). */

/* Partial view of the object the mode's `entry` refers to (first word). */
typedef struct CamDirWatchNode {
  u8 _unk00[0x50];
  ScePspFVector4 dir; /* +0x50 path direction */
} CamDirWatchNode;

typedef struct CamDirWatchEntry {
  CamDirWatchNode *node; /* +0x00 */
} CamDirWatchEntry;

void GameQuestCamDirWatchFwdUpdate(void *state, const float *move, float dt)
{
  GameQuestCamSubState *self = (GameQuestCamSubState *)state;
  GameQuestCamModeBase *mode;
  GameQuestCamSubState *cur;
  GameQuestCamSubState *newState;
  ScePspFVector4 dir __attribute__((aligned(16)));
  s32 next;
  s32 arg;
  float dot;
  float t;

  if (GameQuestCamModeTargetMoved(self->mode) == 0) {
    self->mode->watchTime = 0.0f;
    return;
  }
  dir = ((CamDirWatchEntry *)self->mode->entry)->node->dir;
  dot = dir.x * move[0] + dir.y * move[1] + dir.z * move[2];
  if (dot <= 0.0f) {
    t = 0.0f;
  } else {
    t = self->mode->watchTime + dt;
  }
  self->mode->watchTime = t;
  if (!(self->mode->watchTime <= self->mode->watchDelay)) {
    self->mode->watchTime = 0.0f;
    mode = self->mode;
    next = 2;
    arg = 0;
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
