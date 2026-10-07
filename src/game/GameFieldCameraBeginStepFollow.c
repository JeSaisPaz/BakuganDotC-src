// bdc 0x088bab64 GameFieldCameraBeginStepFollow
#include "bdc.h"

/* Switches the field camera (`GameFieldCameraCtor`, embedded at `+0x20` of the field task; target
   `+0x2a0`, eye `+0x50`, look-at `+0x60`, mode `+0x2ac`) to mode 3 (`GameFieldCameraFollowStep`):
   yaw behind the target, pitch 0.1222, distance `g_gameFieldCameraDefaultDistance`, look-at raised
   by `g_gameFieldCameraLookAtHeight`, and resets the yaw/pitch velocities.
   The trailing VFPU block of the original (an inlined axis-angle helper) only writes stack temps that
   are never read, so the C leaves it out. */

void GameFieldCameraBeginStepFollow(GameFieldCamera *cam)

{
  Actor *target;

  GameFieldCameraSetMode(cam, 3);
  cam->stepState = 0;
  cam->yawVel = 0.0f;
  cam->pitchVel = 0.0f;
  cam->distance = g_gameFieldCameraDefaultDistance;
  cam->pitch = 0.12217305f;
  target = (Actor *)cam->target;
  cam->yaw = target->base.rot[1] + 3.1415927f;
  if (!(cam->yaw <= 3.1415927f)) {
    cam->yaw = cam->yaw - 6.2831855f;
  }
  else if (cam->yaw <= -3.1415927f) {
    cam->yaw = cam->yaw + 6.2831855f;
  }
  target = (Actor *)cam->target;
  cam->followLookAt.x = target->base.pos[0];
  cam->followLookAt.y = target->base.pos[1];
  cam->followLookAt.z = target->base.pos[2];
  cam->followLookAt.w = target->base.pos[3];
  cam->followLookAt.y = cam->followLookAt.y + g_gameFieldCameraLookAtHeight;
  cam->followGoal = cam->followLookAt;
  /* The original ends with an inlined axis-angle helper that turns the yaw/distance offset, its
     normalised perpendicular and a -0.1222 rad rotation into stack temporaries that are never read;
     no caller reads the VFPU registers it leaves, so it has no effect and is dropped. */
  cam->talkBlend = 0;
  cam->pitchVel = 1.0f;
}
