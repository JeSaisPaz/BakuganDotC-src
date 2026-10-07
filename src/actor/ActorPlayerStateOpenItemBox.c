// bdc 0x088e3550 ActorPlayerStateOpenItemBox
#include "bdc.h"

/* State 10 handler of the player actor class (model 0x2f `12_Edit_man.gmo`, 0x550 bytes,
   constructor `ActorPlayerCtor`, vtable `0x08af38e4`) (non-virtual entry of the state table
   `0x08a98ae0`). Sub-step `waitTimer`: 0 (or any value outside 1..6) plays motion 0xc (end frame
   5); 1 waits for its end, then plays motion 0xe (end frame 6); 2 waits for its end, then triggers
   the item box in `talkTarget` (`GameGimmickItemBoxSetState``(obj, 1)`) and starts the camera head
   view on its position (`GameFieldCameraBeginHeadView`); 3 waits while the box state is 1; 4 sets
   the field task's `itemBoxOpened`; 5 waits while `GameFieldIsPlayerFree` is true; 6 counts
   `stateDelay` down and, once it is 0 or less, sets save-profile flag `0x20000000`
   (`SaveProfileSetFlags`) every frame. Steps 2 and 3 skip the box when `talkTarget` is NULL. */

void ActorPlayerStateOpenItemBox(ActorPlayer *self)
{
  float boxPos[4];
  GameGimmickItemBox *box;
  float frame;

  switch (self->base.waitTimer) {
  case 1:
    frame = GfxModelMotionFrame((GfxModel *)self);
    if (frame < GfxModelGetMotionEnd((GfxModel *)self)) {
      return;
    }
    ActorPlayMotion(0.2f, self, 0xe, 0, 0);
    GfxModelSetMotionEnd((GfxModel *)self, 6.0f);
    self->base.waitTimer = 2;
    return;
  case 2:
    frame = GfxModelMotionFrame((GfxModel *)self);
    if (frame < GfxModelGetMotionEnd((GfxModel *)self)) {
      return;
    }
    if (self->talkTarget != NULL) {
      box = (GameGimmickItemBox *)self->talkTarget;
      GameGimmickItemBoxSetState(box, 1);
      boxPos[0] = box->base.base.pos[0];
      boxPos[1] = box->base.base.pos[1];
      boxPos[2] = box->base.base.pos[2];
      boxPos[3] = box->base.base.pos[3];
      GameFieldCameraBeginHeadView(self->base.camera, boxPos);
    }
    self->base.waitTimer = 3;
    return;
  case 3:
    if (self->talkTarget != NULL &&
        GameGimmickItemBoxGetState((GameGimmick *)self->talkTarget) == 1) {
      return;
    }
    self->base.waitTimer = 4;
    return;
  case 4:
    ((GameFieldTask *)GameFieldFindTask())->itemBoxOpened = 1;
    self->base.waitTimer = 5;
    return;
  case 5:
    if (GameFieldIsPlayerFree(GameFieldFindTask())) {
      return;
    }
    self->base.waitTimer = 6;
    return;
  case 6:
    if (self->base.stateDelay > 0) {
      self->base.stateDelay = self->base.stateDelay - 1;
      return;
    }
    SaveProfileSetFlags(SaveGetProfile(), 0x20000000);
    return;
  default:
    ActorPlayMotion(0.2f, self, 0xc, 0, 0);
    GfxModelSetMotionEnd((GfxModel *)self, 5.0f);
    self->base.waitTimer = 1;
    return;
  }
}
