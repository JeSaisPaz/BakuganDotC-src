// bdc 0x088ac81c ActorStageObjBaseBreak
#include "bdc.h"

/* Default break virtual (base vtable `0x08af2904` slot 11, called by `ActorStageObjUpdate` when
   the object is destroyed): unless task 0x14a runs, makes the collider `+0x140` non-solid, builds
   the `<model>_bk.gmo` name (unused), marks the layout record `+0x154` done and releases it
   (`ActorStageObjReleaseRecord`), schedules the object for deletion (`CoreObjectDeferDelete`)
   and unlinks the collider (`CoreNodeUnlink`). */

void ActorStageObjBaseBreak(ActorStageObjBase *self)
{
  float pos[4];
  char path[132];
  char *dot;

  if (CoreTaskExists(0x14a) != 0) {
    return;
  }
  ActorStageObjGetBounds(self);
  if (self->collider != (void *)0x0) {
    ((volatile CollisionCollider *)self->collider)->flags |= 2;
    /* Unused copy of the position into a stack temp. */
    pos[0] = self->base.pos[0];
    pos[1] = self->base.pos[1];
    pos[2] = self->base.pos[2];
    pos[3] = self->base.pos[3];
    ((volatile CollisionCollider *)self->collider)->flags &= ~2u;
    strcpy(path, self->base.name);
    dot = strchr(path, '.');
    if (dot != (char *)0x0) {
      *dot = '\0';
    }
    strcat(path, "_bk.gmo");
  }
  if (self->record != (void *)0x0) {
    ((ActorStageObjRecord *)self->record)->doneFlags[0] = 1;
    ActorStageObjReleaseRecord(self->record);
    self->record = (void *)0x0;
  }
  CoreObjectDeferDelete((CoreObject *)self, 0);
  CoreNodeUnlink(self->collider);
}
