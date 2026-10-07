// bdc 0x088e7b58 ActorNpcResume
#include "bdc.h"

/* Vtable slot 28 of the field NPC/guard classes (base `ActorNpcCtor`): unless the field mode
   (`GameFieldFindTask()+0x61c`) is 5 or 0x1e, restores the AI state saved by `ActorNpcFreeze` and the
   head effect it had (0x29 or 0x2a); then re-applies the placement (`ActorApplyPlacement`). */

void ActorNpcResume(ActorNpc *self)

{
  GameFieldTask *task;
  bool skip;
  int kind;

  task = (GameFieldTask *)GameFieldFindTask();
  skip = false;
  if (task->subState == 5 || task->subState == 0x1e) {
    skip = true;
  }
  if (!skip) {
    self->aiState = self->savedState;
    ActorNpcShowHeadEffect(self, -1, 1, 1);
    kind = self->freezeKind;
    if (0 < kind) {
      if (kind < 2) {
        ActorNpcShowHeadEffect(self, 0x29, 1, 1);
      } else if (kind < 3) {
        ActorNpcShowHeadEffect(self, 0x2a, 1, 1);
      }
    }
  }
  ActorApplyPlacement(&self->base);
}
