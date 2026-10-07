// bdc 0x0884910c BtlFinishTaskUpdateCamera
#include "bdc.h"

/* Camera step of `BtlFinishTaskUpdate`: ramps `camBlend` by 0.1 (capped at 1) and, once it is
   above 0.5, `flashT` by 0.075 (capped at 1); then eases `camDistance` toward 300 (700 when
   `farCamera` is set) plus the swing `(1 - cos((1 - flashT) * pi)) * 0.5 * sin(flashT * 3pi) * 500`
   at rate `camBlend^2 * 0.5`. */
void BtlFinishTaskUpdateCamera(BtlFinishTask *task)
{
    float blend = task->camBlend + 0.100000001f;
    float t = task->flashT;
    float dist = task->camDistance;
    float c;
    float s;
    float swing;
    float target;

    if (!(blend <= 1.0f)) {
        blend = 1.0f;
    }
    task->camBlend = blend;
    if (!(blend <= 0.5f)) {
        t = t + 0.0750000030f;
        if (!(t <= 1.0f)) {
            t = 1.0f;
        }
        task->flashT = t;
    }
    /* vcos/vsin of radians * S703 (2/pi): plain cos/sin of the angle */
    c = __builtin_cosf((1.0f - t) * 3.14159274f);
    s = __builtin_sinf(t * 9.42477798f);
    swing = (1.0f - c) * 0.5f * s * 500.0f;
    if (task->farCamera != 0) {
        target = swing + 700.0f;
    } else {
        target = swing + 300.0f;
    }
    task->camDistance = dist + (target - dist) * (blend * blend) * 0.5f;
}
