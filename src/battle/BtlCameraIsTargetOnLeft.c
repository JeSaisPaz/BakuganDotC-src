// bdc 0x088483bc BtlCameraIsTargetOnLeft
#include "bdc.h"

/* Returns true when the battle camera's lock-on `focusPoint` lies to the left of the line from the
   camera eye through its `lookPoint`: it takes the XZ headings eye→lookPoint and
   lookPoint→focusPoint (`atan2f(z, x)`), reduces their difference by `trunc(d / π) * 2π` (as the
   binary does: 1/π, not 1/2π), adds 2π when negative, maps it to `-d` below π and `2π - d`
   otherwise, and tests that value for `< 0`. `BtlCutInTaskCtor` uses it to pick the mirrored
   cut-in effect (0x86 instead of 0x85). */
bool BtlCameraIsTargetOnLeft(BtlCamera *camera)
{
    float lookToFocus[3];
    float eyeToLook[3];
    float eyeHeading;
    float diff;
    float wrapped;

    /* vsub.t: lane-wise differences (lane 1 is unused). */
    lookToFocus[0] = camera->focusPoint[0] - camera->lookPoint[0];
    lookToFocus[1] = camera->focusPoint[1] - camera->lookPoint[1];
    lookToFocus[2] = camera->focusPoint[2] - camera->lookPoint[2];
    eyeToLook[0] = camera->lookPoint[0] - camera->base.eye[0];
    eyeToLook[1] = camera->lookPoint[1] - camera->base.eye[1];
    eyeToLook[2] = camera->lookPoint[2] - camera->base.eye[2];

    eyeHeading = atan2f(eyeToLook[2], eyeToLook[0]);
    diff = eyeHeading - atan2f(lookToFocus[2], lookToFocus[0]);
    diff = diff - (float)(int)(diff * 0.318309873f) * 6.28318548f;
    if (diff < 0.0f) {
        diff = diff + 6.28318548f;
    }
    if (diff < 3.14159274f) {
        wrapped = -diff;
    } else {
        wrapped = 6.28318548f - diff;
    }
    return wrapped < 0.0f;
}
