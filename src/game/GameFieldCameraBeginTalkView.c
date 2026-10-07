// bdc 0x088ba17c GameFieldCameraBeginTalkView
#include "bdc.h"

/* Switches the field camera (`GameFieldCameraCtor`) to mode 1 framing the target and its talk
   partner (`target->talkPartner`, `+0x328`); returns at once without a target or partner.
   Clears `talkBlend`/`stepState`/`yawVel`, takes the heading target->partner (`atan2f` on the xz
   delta) and picks the side from the folded difference between the target heading + pi and the
   current eye->look-at heading: below 0.1 it turns +117 degrees (`talkSideOffset` a further +25
   degrees), else -117 (and -25). The look-at base is the target position plus 15 units along that
   angle (cos on x, sin on z); `yaw` = `talkSideOffset` (wrapped to (-pi, pi]); `talkEye.y` =
   |partner - target| (xyz) / cos(0); `talkLookAt` = base + `talkEye.y` units along `yaw` + pi
   (w = target position w). */

void GameFieldCameraBeginTalkView(GameFieldCamera *cam)

{
  Actor *target;
  Actor *partner;
  ScePspFVector4 delta;
  ScePspFVector4 base;
  ScePspFVector4 lookAt;
  float partnerX;
  float partnerZ;
  float eyeX;
  float eyeZ;
  float heading;
  float relative;
  float folded;
  float angle;
  float yaw;
  float len;
  float dist;

  if (cam->target == NULL) {
    return;
  }
  if (((Actor *)cam->target)->talkPartner == NULL) {
    return;
  }
  GameFieldCameraSetMode(cam, 1);
  cam->talkBlend = 0;
  cam->stepState = 0;
  cam->yawVel = 0.0f;
  target = (Actor *)cam->target;
  partnerZ = target->talkPartner->base.pos[2];
  partnerX = target->talkPartner->base.pos[0];
  heading = atan2f(partnerZ - target->base.pos[2], partnerX - target->base.pos[0]);
  target = (Actor *)cam->target;
  relative = target->base.rot[1] + 3.1415927f;
  if (!(relative <= 3.1415927f)) {
    relative = relative - 6.2831855f;
  }
  else if (relative <= -3.1415927f) {
    relative = relative + 6.2831855f;
  }
  eyeZ = cam->base.eye[2];
  eyeX = cam->base.eye[0];
  angle = atan2f(eyeZ - cam->base.target[2], eyeX - cam->base.target[0]);
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
  partner = target->talkPartner;
  delta.x = partner->base.pos[0] - target->base.pos[0];
  delta.y = partner->base.pos[1] - target->base.pos[1];
  delta.z = partner->base.pos[2] - target->base.pos[2];
  target = (Actor *)cam->target;
  base.x = target->base.pos[0] + __builtin_cosf(heading) * 15.0f;
  base.y = target->base.pos[1] + 0.0f * 15.0f;
  base.z = target->base.pos[2] + __builtin_sinf(heading) * 15.0f;
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
  dist = cam->talkEye.y;
  lookAt.x = base.x + __builtin_cosf(yaw) * dist;
  lookAt.y = base.y + 0.0f * dist;
  lookAt.z = base.z + __builtin_sinf(yaw) * dist;
  lookAt.w = base.w;
  cam->talkLookAt = lookAt;
}
