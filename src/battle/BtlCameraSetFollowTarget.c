// bdc 0x08847e08 BtlCameraSetFollowTarget
#include "bdc.h"

/* Points the battle follow camera (mode 2) at a target: saves the current eye/target/up/fov,
   stores distance, height and the fov to ease to, clears the wobble and follow offsets, and picks
   what to track by `mode` (2 = the point `target` with yaw from `yawSrc`; 1 = the GfxModel field at
   +0x60 of `target`, yaw source kept; otherwise the unit's pos with yaw from its rot). Copies the
   tracked point into followFocus, sets the initial yaw
   wrapped into (-pi, pi], and either starts a smooth blend (`snap == 0`) or cuts at once through
   BtlCameraUpdateFollow. */
void BtlCameraSetFollowTarget(float distance, float height, float fov, void *cameraObj, void *target, u8 snap, int mode, float *yawSrc)
{
    /* The controller embedded in BtlMain (BtlCameraFocusUnit, BtlMainPhaseIntro). */
    BtlCamera *camera = (BtlCamera *)cameraObj;
    GfxModel *unit = (GfxModel *)target;
    s32 i;

    camera->mode = 2;
    for (i = 0; i < 4; i++) {
        camera->savedEye[i] = camera->base.eye[i];
    }
    for (i = 0; i < 4; i++) {
        camera->savedTarget[i] = camera->base.target[i];
    }
    for (i = 0; i < 4; i++) {
        camera->savedUp[i] = camera->base.up[i];
    }
    camera->savedFov = camera->base.fov;
    camera->followDistance = distance;
    camera->followHeight = height;
    camera->followFov = fov;
    camera->followWobble = 0.0f;
    camera->followDistanceOffset = 0.0f;
    camera->followOffsetY = 0.0f;
    camera->followOffset3f4 = 0.0f;
    if (mode == 1) {
        if (camera->followTarget == NULL) {
            camera->followTarget = camera->target;
        }
        camera->followPos = unit->ambient;
    } else if (mode == 2) {
        camera->followPos = (float *)target;
        camera->followTarget = camera->target;
        camera->followYawSrc = yawSrc;
    } else {
        camera->followTarget = target;
        camera->followPos = unit->pos;
        camera->followYawSrc = unit->rot;
    }
    for (i = 0; i < 4; i++) {
        camera->followFocus[i] = camera->followPos[i];
    }
    if (camera->followYawSrc != NULL) {
        camera->yaw = camera->followYawSrc[1];
        if (!(camera->yaw <= 3.14159274f)) {
            camera->yaw = camera->yaw - 6.28318548f;
        } else if (camera->yaw <= -3.14159274f) {
            camera->yaw = camera->yaw + 6.28318548f;
        }
    }
    camera->turnFrames = 0;
    camera->yawOffset = 0.0f;
    camera->lockOnFlag = 0;
    if (snap == 0) {
        camera->eyeSnap = 0.0f;
        return;
    }
    camera->eyeSnap = 1.0f;
    BtlCameraUpdateFollow(camera);
}
