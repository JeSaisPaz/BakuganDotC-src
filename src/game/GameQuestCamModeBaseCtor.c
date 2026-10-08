// bdc 0x088fcb30 GameQuestCamModeBaseCtor
#include "bdc.h"

/* Base constructor of the quest camera mode classes (`GameQuestCamModeBase`, called by
   `GameQuestCamPathModeCtor` / `GameQuestCamPointModeCtor`): runs `GameQuestCamTargetCtor`,
   installs `g_gameQuestCamModeBaseVtbl`, copies `entry` from `desc+0x40` (the look-spring slot), allocates a 10-entry
   `states` vector from the low heap, sets `targetPos`/`lead` to (0, 0, 0, 0) (bank constant C720) and
   `smoothPos`/`smoothVel` to the `zero` vector of `g_gameQuestCamModeBaseAxisConsts`, clears
   `watchTime`/`behind`/`leadFilter`, sets `firstCall`. Then pushes three sub-states
   (`GameQuestCamSubState`, each first given `g_gameQuestCamSubStateVtbl`): watch back
   (`g_gameQuestCamDirWatchBackVtbl`), watch forward (`g_gameQuestCamDirWatchFwdVtbl`) and
   direction switch (`g_gameQuestCamDirSwitchVtbl`, `_f0c` = 0.0314159); a failed allocation
   pushes NULL, a full vector doubles first. Enters sub-state 0 when the quest camera table's
   current set is its first set (out of range: `g_gameQuestCamNullSet`), else sub-state 1
   (exiting the previous `state` if any; out-of-range index: `g_questCamNullState`), captures the
   followed object's position (its vtable slot 2) into `targetPos` and sets `watchDelay` = 0.1.
   Returns `self`.
   VFPU bank constant C720 = (0, 0, 0, 0); the C000 it leaves is read by no caller. */

typedef struct CamModeFollowed {
  void *unk0;
  const VtblEntry *vtbl; /* +0x04, slot 2 returns the position */
} CamModeFollowed;

/* Leading part of the mode descriptors built by GameQuestCamCtrlStartPathMode/StartPointMode. */
typedef struct CamModeBaseDesc {
  GameQuestCamSpringDesc spring; /* +0x00 */
  ScePspFVector4 point;          /* +0x30 */
  void *lookSlot;                /* +0x40 &ctrl->look */
} CamModeBaseDesc;

GameQuestCamModeBase *GameQuestCamModeBaseCtor(GameQuestCamModeBase *self, void *desc)

{
  bool fromLow;
  void **buf;
  void **old;
  GameQuestCamSubState *sub;
  GameQuestCamSubState *cur;
  GameQuestCamSubState *push;
  GameQuestCamPtrVec *curSet;
  GameQuestCamPtrVec *firstSet;
  CamModeFollowed *followed;
  const VtblEntry *e;
  float *pos;
  s32 newCap;
  s32 index;
  /* UB (original binary): the enter argument is an uninitialised stack slot */
  s32 ub_arg;

  GameQuestCamTargetCtor(&self->base, desc);
  self->base.base.vtbl = g_gameQuestCamModeBaseVtbl;
  self->entry = ((CamModeBaseDesc *)desc)->lookSlot;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  buf = (void **)MemAlloc(10 * sizeof(void *), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->states = buf;
  self->stateCap = 10;
  self->stateCount = 0;
  self->state = NULL;
  /* bank constant C720 = (0, 0, 0, 0) */
  self->targetPos.x = 0.0f;
  self->targetPos.y = 0.0f;
  self->targetPos.z = 0.0f;
  self->targetPos.w = 0.0f;
  self->watchTime = 0.0f;
  self->behind = 0;
  self->smoothPos = g_gameQuestCamModeBaseAxisConsts.zero;
  self->lead.x = 0.0f;
  self->lead.y = 0.0f;
  self->lead.z = 0.0f;
  self->lead.w = 0.0f;
  self->smoothVel = g_gameQuestCamModeBaseAxisConsts.zero;
  self->leadFilter = 0.0f;
  self->firstCall = 1;

  /* sub-state 0: watch backwards */
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sub = (GameQuestCamSubState *)MemAlloc(__builtin_offsetof(GameQuestCamSubState, next), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  push = NULL;
  if (sub != NULL) {
    sub->vtbl = g_gameQuestCamSubStateVtbl;
    sub->mode = self;
    sub->vtbl = g_gameQuestCamDirWatchBackVtbl;
    push = sub;
  }
  if (!(self->stateCount < self->stateCap)) {
    newCap = self->stateCap + self->stateCap;
    if (newCap != 0) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      buf = (void **)MemAlloc(newCap * sizeof(void *), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (newCap < self->stateCount) {
        self->stateCount = newCap;
      }
      memcpy(buf, self->states, self->stateCount * sizeof(void *));
      self->stateCap = newCap;
      if (self->states != NULL) {
        old = self->states;
        MemLock();
        MemFree(old, NULL, 0);
        MemUnlock();
        self->states = NULL;
      }
      self->states = buf;
    }
  }
  if (self->stateCount < self->stateCap) {
    self->states[self->stateCount] = push;
    self->stateCount = self->stateCount + 1;
  }

  /* sub-state 1: watch forwards */
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sub = (GameQuestCamSubState *)MemAlloc(__builtin_offsetof(GameQuestCamSubState, next), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  push = NULL;
  if (sub != NULL) {
    sub->vtbl = g_gameQuestCamSubStateVtbl;
    sub->mode = self;
    sub->vtbl = g_gameQuestCamDirWatchFwdVtbl;
    push = sub;
  }
  if (!(self->stateCount < self->stateCap)) {
    newCap = self->stateCap + self->stateCap;
    if (newCap != 0) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      buf = (void **)MemAlloc(newCap * sizeof(void *), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (newCap < self->stateCount) {
        self->stateCount = newCap;
      }
      memcpy(buf, self->states, self->stateCount * sizeof(void *));
      self->stateCap = newCap;
      if (self->states != NULL) {
        old = self->states;
        MemLock();
        MemFree(old, NULL, 0);
        MemUnlock();
        self->states = NULL;
      }
      self->states = buf;
    }
  }
  if (self->stateCount < self->stateCap) {
    self->states[self->stateCount] = push;
    self->stateCount = self->stateCount + 1;
  }

  /* sub-state 2: direction switch */
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sub = (GameQuestCamSubState *)MemAlloc(sizeof(GameQuestCamSubState), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  push = NULL;
  if (sub != NULL) {
    sub->vtbl = g_gameQuestCamSubStateVtbl;
    sub->mode = self;
    sub->vtbl = g_gameQuestCamDirSwitchVtbl;
    sub->_f0c = 0.03141593f;
    push = sub;
  }
  if (!(self->stateCount < self->stateCap)) {
    newCap = self->stateCap + self->stateCap;
    if (newCap != 0) {
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      buf = (void **)MemAlloc(newCap * sizeof(void *), NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      if (newCap < self->stateCount) {
        self->stateCount = newCap;
      }
      memcpy(buf, self->states, self->stateCount * sizeof(void *));
      self->stateCap = newCap;
      if (self->states != NULL) {
        old = self->states;
        MemLock();
        MemFree(old, NULL, 0);
        MemUnlock();
        self->states = NULL;
      }
      self->states = buf;
    }
  }
  if (self->stateCount < self->stateCap) {
    self->states[self->stateCount] = push;
    self->stateCount = self->stateCount + 1;
  }

  /* initial sub-state: 0 when the current camera set is the first one, else 1
     (the binary inlines the state switch once per branch) */
  curSet = g_questCamTable->cur;
  if (0 < g_questCamTable->count) {
    firstSet = (GameQuestCamPtrVec *)g_questCamTable->data[0];
  }
  else {
    memset(&g_gameQuestCamNullSet, 0, 4);
    firstSet = g_gameQuestCamNullSet;
  }
  index = (curSet == firstSet) ? 0 : 1;
  cur = (GameQuestCamSubState *)self->state;
  if (cur != NULL) {
    ((void (*)(void *))cur->vtbl[2].fn)((u8 *)cur + cur->vtbl[2].delta);
  }
  self->stateIndex = index;
  if (index > -1 && index < self->stateCount) {
    cur = (GameQuestCamSubState *)self->states[index];
  }
  else {
    memset(&g_questCamNullState, 0, 4);
    cur = (GameQuestCamSubState *)g_questCamNullState;
  }
  self->state = cur;
  ((void (*)(void *, void *))cur->vtbl[1].fn)((u8 *)cur + cur->vtbl[1].delta, &ub_arg);

  followed = (CamModeFollowed *)self->base.base.followed;
  e = &followed->vtbl[2];
  pos = ((float *(*)(void *))e->fn)((u8 *)followed + e->delta);
  self->targetPos.x = pos[0];
  self->targetPos.y = pos[1];
  self->targetPos.z = pos[2];
  self->targetPos.w = pos[3];
  self->watchDelay = 0.1f;
  return self;
}
