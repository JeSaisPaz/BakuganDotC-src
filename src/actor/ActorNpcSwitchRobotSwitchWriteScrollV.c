// bdc 0x088e8104 ActorNpcSwitchRobotSwitchWriteScrollV
#include "bdc.h"

/* Display-list callback bound by `ActorNpcSwitchRobotCheckSwitchHit` to the switch robot's
   `psp_vxsrobo_swich` node (argument vec4 `+0x460`): appends GE `TOFFSETV` = `*scroll` to `*dl`,
   animating the switch texture once the robot is shut down. */
void ActorNpcSwitchRobotSwitchWriteScrollV(u32 **dl, const float *scroll)
{
    union {
        float f;
        u32 bits;
    } v;

    v.f = *scroll;
    **dl = (v.bits >> 8) | 0x4b000000;
    *dl = *dl + 1;
}
