// bdc 0x088ba588 GameFieldCameraBeginTalkViewWith
#include "bdc.h"

/* Like `GameFieldCameraBeginTalkView` but frames the target with an explicit `other` actor: needs
   both a target and `other` (else returns at once). Switches the field camera
   (`GameFieldCameraCtor`) to mode 1, clears `talkBlend`/`stepState`/`yawVel`, takes the heading
   target->other (`atan2f` on the xz delta) and picks the side from the folded difference between
   the target heading + pi and the current eye->look-at heading: below 0.1 it turns +117 degrees
   (`talkSideOffset` a further +25 degrees), else -117 (and -25). The look-at base is the target
   position plus 15 units along that angle; `yaw` = `talkSideOffset` (wrapped to (-pi, pi]);
   `talkEye.y` = |other - target| (xyz) / cos(0); `talkLookAt` = base + `talkEye.y` units along
   `yaw` + pi (w = target position w). */

void GameFieldCameraBeginTalkViewWith(GameFieldCamera *cam, Actor *other)

{
  Actor *target;
  ScePspFVector4 delta;
  ScePspFVector4 side;
  ScePspFVector4 base;
  ScePspFVector4 offset;
  float heading;
  float relative;
  float folded;
  float angle;
  float yaw;
  float len;

  if (cam->target == NULL) {
    return;
  }
  if (other == NULL) {
    return;
  }
  GameFieldCameraSetMode(cam, 1);
  cam->talkBlend = 0;
  cam->stepState = 0;
  cam->yawVel = 0.0f;
  target = (Actor *)cam->target;
  heading = atan2f(other->base.pos[2] - target->base.pos[2], other->base.pos[0] - target->base.pos[0]);
  target = (Actor *)cam->target;
  relative = target->base.rot[1] + 3.1415927f;
  if (!(relative <= 3.1415927f)) {
    relative = relative - 6.2831855f;
  }
  else if (relative <= -3.1415927f) {
    relative = relative + 6.2831855f;
  }
  angle = atan2f(cam->base.eye[2] - cam->base.target[2], cam->base.eye[0] - cam->base.target[0]);
  if (!(angle <= 3.1415927f)) {
    angle = angle - 6.2831855f;
  }
  else if (angle <= -3.1415927f) {
    angle = angle + 6.2831855f;
  }
  relative = relative - angle;
  relative = relative - (float)(s32)(relative * 0.31830987f) * 6.2831855f;
  if (relative < 0.0f) {
    relative = relative + 6.2831855f;
  }
  if (relative < 3.1415927f) {
    relative = -relative;
  }
  else {
    relative = 6.2831855f - relative;
  }
  folded = relative;
  if (!(folded <= 3.1415927f)) {
    folded = relative - 6.2831855f;
  }
  else if (relative <= -3.1415927f) {
    folded = relative + 6.2831855f;
  }
  if (folded < 0.1f) {
    heading = heading + 2.042035f;
    cam->talkSideOffset = heading + 0.4363672f;
  }
  else {
    heading = heading - 2.042035f;
    cam->talkSideOffset = heading - 0.4363672f;
  }
  if (!(heading <= 3.1415927f)) {
    heading = heading - 6.2831855f;
  }
  else if (heading <= -3.1415927f) {
    heading = heading + 6.2831855f;
  }
  target = (Actor *)cam->target;
  /* delta = other - target (xyz), w from other */
  delta.x = other->base.pos[0] - target->base.pos[0];
  delta.y = other->base.pos[1] - target->base.pos[1];
  delta.z = other->base.pos[2] - target->base.pos[2];
  delta.w = other->base.pos[3];
  /* side = 15 units along heading (vrot [C,0,S,0], vscl.t leaves w = 0) */
  side.x = __builtin_cosf(heading) * 15.0f;
  side.y = 0.0f * 15.0f;
  side.z = __builtin_sinf(heading) * 15.0f;
  side.w = 0.0f;
  target = (Actor *)cam->target;
  base.x = target->base.pos[0] + side.x;
  base.y = target->base.pos[1] + side.y;
  base.z = target->base.pos[2] + side.z;
  base.w = target->base.pos[3];
  if (!(cam->talkSideOffset <= 3.1415927f)) {
    cam->talkSideOffset = cam->talkSideOffset - 6.2831855f;
  }
  else if (cam->talkSideOffset <= -3.1415927f) {
    cam->talkSideOffset = cam->talkSideOffset + 6.2831855f;
  }
  yaw = cam->talkSideOffset;
  cam->yaw = yaw;
  len = __builtin_sqrtf(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
  cam->talkEye.y = len;
  cam->talkEye.y = len / __builtin_cosf(0.0f);
  yaw = yaw + 3.1415927f;
  if (!(yaw <= 3.1415927f)) {
    yaw = yaw - 6.2831855f;
  }
  else if (yaw <= -3.1415927f) {
    yaw = yaw + 6.2831855f;
  }
  len = cam->talkEye.y;
  offset.x = __builtin_cosf(yaw) * len;
  offset.y = 0.0f * len;
  offset.z = __builtin_sinf(yaw) * len;
  offset.w = 0.0f;
  cam->talkLookAt.x = base.x + offset.x;
  cam->talkLookAt.y = base.y + offset.y;
  cam->talkLookAt.z = base.z + offset.z;
  cam->talkLookAt.w = base.w;
}
