// bdc 0x088f7d98 GameQuestCamPathModeUpdateEye
#include "bdc.h"

/* Slot 6 of the path camera mode: when attached to a segment, updates the pitch
   (`GameQuestCamUpdatePitch`), resets the attached state when a previous segment exists
   (`GameQuestCamModeResetAttached`), interpolates the segment end nodes' `distance` (`+0x44`) by
   the unclamped `t` into the far smoothing time, and their `extra0`/`extra1` (`+0x50`/`+0x54`) by
   `t` clamped below at 0 into the controller's `extra[0]`/`extra[1]`; without a segment the far
   time is 1.8. Then spring-accelerates towards `point` (`GameQuestCamSpringAccelerateByDistance`,
   1.2 near, 400/1200 distances), resolves collision unless the segment's start node has `collide`
   (`+0x61`) clear, integrates, and captures the target position. The segment nodes are
   `GameQuestCamEntry` records (see `GameQuestPathSegment` notes). */

void GameQuestCamPathModeUpdateEye(float dt, GameQuestCamPathMode *self)

{
  GameQuestPathSegment *seg;
  const GameQuestCamEntry *from;
  const GameQuestCamEntry *to;
  GameQuestCamSpring *spring = &self->base.base.base;
  ScePspFVector4 *pos = &self->base.base.point;
  float farTime = 1.8f;
  float t;
  float v0;
  float v1;

  if (self->segment != NULL) {
    GameQuestCamUpdatePitch(self);
    if (self->prevSegment != NULL) {
      GameQuestCamModeResetAttached(&self->base);
    }
    seg = (GameQuestPathSegment *)self->segment;
    t = seg->t;
    from = seg->from;
    to = seg->to;
    farTime = from->distance * (1.0f - t) + to->distance * t;
    if (t < 0.0f) {
      t = 0.0f;
    }
    v0 = from->extra0 * (1.0f - t) + to->extra0 * t;
    v1 = from->extra1 * (1.0f - t) + to->extra1 * t;
    spring->ctrl->extra[1] = v1;
    spring->ctrl->extra[0] = v0;
  }
  GameQuestCamSpringAccelerateByDistance(dt, 1.2f, farTime, 400.0f, 1200.0f, spring, &pos->x);
  if ((self->segment == NULL) ||
      (((GameQuestPathSegment *)self->segment)->from->collide != 0)) {
    GameQuestCamResolveCollision(dt, spring);
  }
  GameQuestCamIntegrate(dt, spring, &pos->x, &spring->vel.x, &spring->accel.x);
  GameQuestCamModeCaptureTargetPos(&self->base);
  return;
}
