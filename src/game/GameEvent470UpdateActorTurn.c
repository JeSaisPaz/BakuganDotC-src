// bdc 0x088f24a4 GameEvent470UpdateActorTurn
#include "bdc.h"

/* Per-frame step (vtable slot 8, `+0x44`) of the task-470 event class (derived from the field event
   base `GameEventCtor`, vtable `0x08af425c`, constructor `GameEvent470Ctor`). With no turn
   pending (`turnPending == 0`) it calls vtable slot 7 and sets `state = 1`. Otherwise it steps
   `turnPhase`: phase 0 resets `turnFrame`, sets the target heading `endRot[1]` of talk actor `talkB`
   to `turnHeading`, and, when no transition is requested (`g_gameEventTransitionKind` == 0) and
   the active field camera is not in mode 0, starts `GameFieldCameraBeginBlendToFollow`; phase 1
   turns that actor's heading from `startRot[1]` to `endRot[1]` over 8 frames, copying the angles into
   the placed character (`rot`, heading into `work2c`), and on frame 8 snaps start and current angles
   to `endRot` and advances; phase 2 calls slot 7 and sets `state = 1`; other phases do nothing. */

void GameEvent470UpdateActorTurn(GameEvent470 *self)
{
  const VtblEntry *slot;
  GameEventActorRecord *rec;
  GameFieldPlacedChar *ch;
  GameFieldCamera *cam;
  s32 frame;

  if (self->turnPending == 0) {
    slot = &((const VtblEntry *)self->base.base.vtable)[7];
    ((void (*)(void *))slot->fn)((u8 *)self + slot->delta);
    self->base.state = 1;
    return;
  }
  switch (self->turnPhase) {
  case 0:
    self->turnFrame = 0;
    self->actors[self->talkB].endRot[1] = self->turnHeading;
    if (g_gameEventTransitionKind == 0) {
      cam = (GameFieldCamera *)g_gfxActiveCamera;
      if (cam->mode != 0) {
        GameFieldCameraBeginBlendToFollow(cam);
      }
    }
    self->turnPhase = self->turnPhase + 1;
    break;
  case 1:
    rec = &self->actors[self->talkB];
    frame = self->turnFrame + 1;
    self->turnFrame = frame;
    if (frame < 8) {
      ch = rec->actor;
      rec->rot[1] = (s16)(rec->startRot[1] +
                          (s16)(rec->endRot[1] - rec->startRot[1]) * frame / 8);
      ch->rot[0] = rec->rot[0];
      ch->rot[1] = rec->rot[1];
      ch->rot[2] = rec->rot[2];
      ch = rec->actor;
      ch->work2c = rec->rot[1];
      break;
    }
    rec->startRot[0] = rec->endRot[0];
    rec->startRot[1] = rec->endRot[1];
    rec->startRot[2] = rec->endRot[2];
    rec->rot[0] = rec->endRot[0];
    rec->rot[1] = rec->endRot[1];
    rec->rot[2] = rec->endRot[2];
    ch = rec->actor;
    ch->rot[0] = rec->rot[0];
    ch->rot[1] = rec->rot[1];
    ch->rot[2] = rec->rot[2];
    ch = rec->actor;
    ch->work2c = rec->rot[1];
    self->turnPhase = self->turnPhase + 1;
    break;
  case 2:
    slot = &((const VtblEntry *)self->base.base.vtable)[7];
    ((void (*)(void *))slot->fn)((u8 *)self + slot->delta);
    self->base.state = 1;
    break;
  default:
    break;
  }
}
