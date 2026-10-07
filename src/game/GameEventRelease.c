// bdc 0x088ef3d0 GameEventRelease
#include "bdc.h"

/* Frees an event's resources: deletes the props (`GameEventDeleteProps`) when `props` is set,
   clears the action list `actions` and marks it done, disables the talk-balloon portrait
   (`UiTalkBalloonSetPortraitEnabled`), frees and nulls `fades`, `fadeWork`, `camKeys` and
   `props` (each under `MemLock`), and clears `script`. */

void GameEventRelease(GameEvent *self)
{
  void *ptr;

  if (self->props != NULL) {
    GameEventDeleteProps(self);
  }
  if (self->actions != NULL) {
    GameEventActionListClear(self->actions);
    GameEventActionListSetDone(self->actions);
    self->actions = NULL;
  }
  UiTalkBalloonSetPortraitEnabled(0);
  ptr = self->fades;
  if (ptr != NULL) {
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    self->fades = NULL;
  }
  ptr = self->fadeWork;
  if (ptr != NULL) {
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    self->fadeWork = NULL;
  }
  ptr = self->camKeys;
  if (ptr != NULL) {
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    self->camKeys = NULL;
  }
  ptr = self->props;
  if (ptr != NULL) {
    MemLock();
    MemFree(ptr, NULL, 0);
    MemUnlock();
    self->props = NULL;
  }
  self->script = NULL;
}
