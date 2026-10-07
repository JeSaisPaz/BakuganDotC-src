// bdc 0x0884dc70 BtlCameraFocusUnit
#include "bdc.h"

/* Starts a scripted camera focus on a unit: locks AI/control of all units with
   `BtlSetControlLockAll``(1)`, then (fov = the follow fov the camera eases to) `BtlCameraSetFollowTarget``(distance, height, fov,
   &cameraTask->camera, unit, snap, mode, yawSrc)`. If `mode == 1` and global script variable 1
   (the scene id) equals 0x12 it also stores `g_btlCameraFocusYawScene18` in the camera's
   yaw offset `camera.followOffset3f4`.
   Finally sets the task's `phase` and `drawPhase` to 6 (focus mode) and clears
   `g_btlCameraDefaultMode`. */

void BtlCameraFocusUnit(float distance, float height, float fov, BtlMain *cameraTask, void *unit, u8 snap, int mode, void *yawSrc)
{
    BtlSetControlLockAll(1);
    BtlCameraSetFollowTarget(distance, height, fov, &cameraTask->camera, unit, snap, mode, yawSrc);
    if (mode == 1 && g_scriptGlobalVars[1] == 0x12) {
        cameraTask->camera.followOffset3f4 = g_btlCameraFocusYawScene18;
    }
    cameraTask->phase = 6;
    cameraTask->drawPhase = 6;
    g_btlCameraDefaultMode = 0;
}
