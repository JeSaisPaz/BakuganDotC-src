// bdc 0x088afbfc ActorStageObjBreakPieceCollapse
#include "bdc.h"

/* Collapse sequence of a building/warehouse debris piece, driven by `step` (+0x320): plays the
   collapse motion (`GfxModelUpdateAndApplyMotion`) while copying the scale into the root matrix
   diagonal until the motion ends or reaches 99 % (`GfxModelMotionReached`), releases the motion
   records (`GmoMotionArrayRelease`), waits 300 frames, blinks the piece for 32 frames (`baseAlpha`
   toggles 0/1 every 2 frames, then 1), fades it out by 0.05 per frame and queues it for deletion
   (`CoreObjectDeferDelete`). Steps past 6 do nothing. */

void ActorStageObjBreakPieceCollapse(ActorStageObjBreakPiece *self)

{
  GfxModel *model = &self->base.base;
  GmoModel *data;
  short timer;
  float alpha;

  switch ((u32)self->step) {
  case 0:
    self->step = self->step + 1;
    /* fall through */
  case 1:
    GfxModelUpdateAndApplyMotion(model);
    data = model->data;
    data->rootMatrix[0] = model->scale[0];
    data->rootMatrix[5] = model->scale[1];
    data->rootMatrix[10] = model->scale[2];
    if (model->motionEnded != 0 || GfxModelMotionReached(model, 0.99f)) {
      self->step = self->step + 1;
    }
    break;
  case 2:
    data = model->data;
    self->timer = 300;
    if (data->motions != NULL) {
      GmoMotionArrayRelease((short *)data->motions, data->motionCount);
    }
    self->step = self->step + 1;
    /* fall through */
  case 3:
    timer = self->timer;
    if (timer == 0) {
      self->step = self->step + 1;
    } else {
      self->timer = timer - 1;
    }
    break;
  case 4:
    timer = self->timer;
    if (timer == 0x20) {
      self->baseAlpha = 1.0f;
      self->step = self->step + 1;
    } else {
      if ((timer & 2) != 0) {
        alpha = 1.0f;
      } else {
        alpha = 0.0f;
      }
      self->baseAlpha = alpha;
      self->timer = timer + 1;
    }
    break;
  case 5:
    alpha = self->baseAlpha - 0.05f;
    self->baseAlpha = alpha;
    if (alpha <= 0.0f) {
      self->baseAlpha = 0.0f;
      self->step = self->step + 1;
    }
    break;
  case 6:
    CoreObjectDeferDelete(&model->base, 0);
    self->step = self->step + 1;
    break;
  }
  return;
}
