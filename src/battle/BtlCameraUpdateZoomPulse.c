// bdc 0x088471fc BtlCameraUpdateZoomPulse
#include "bdc.h"

/* Called by `BtlCameraUpdate`: while the default target `+0x2a0` or its target
   (`BtlBakuganGetTarget`) is in the slowed combo state (`BtlBakuganIsInSlowedComboState`),
   ramps `zoomPulse` toward 1 (0.025/frame, capped at 1; otherwise it resets to 0) and sets the
   camera's frustum scale to `1 - 0.35 * (1 - c) / 2`, where
   `c = cos(pi * (1 - (zoomPulse - 1)^2))`. */
void BtlCameraUpdateZoomPulse(BtlCamera *camera)
{
    BtlBakugan *other = BtlBakuganGetTarget(camera->target);
    float pulse;
    float t;
    float angle;
    float cosine;

    if (BtlBakuganIsInSlowedComboState(camera->target) != 0 ||
        (other != NULL && BtlBakuganIsInSlowedComboState(other) != 0)) {
        pulse = camera->zoomPulse + 0.0250000004f;
    } else {
        pulse = 0.0f;
    }
    camera->zoomPulse = pulse;
    if (!(pulse <= 1.0f)) {
        camera->zoomPulse = 1.0f;
    }

    t = camera->zoomPulse - 1.0f;
    angle = (1.0f - t * t) * 3.14159274f;
    /* vmul.s by the bank's S703 (2/π) then vcos.s (quarter turns): the cosine of the angle. */
    cosine = __builtin_cosf(angle);

    camera->base.frustumScale = 1.0f - (1.0f - cosine) * 0.5f * 0.349999994f;
}
