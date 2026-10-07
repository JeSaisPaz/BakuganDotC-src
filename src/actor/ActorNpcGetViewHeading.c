// bdc 0x088e5e80 ActorNpcGetViewHeading
#include "bdc.h"

/* Returns the NPC's view heading: the body heading `+0x34` plus the yaw of the head node `+0x3e0`
   (its `localMatrix` applied to the axis `g_vecX` by `vtfm4`, then `atan2f(z, x)`). Without a
   head the yaw is 0. The listing tests the model id (0x51..0x53) first, but both of its paths
   compute the same value. */

float ActorNpcGetViewHeading(ActorNpc *self)
{
    float yaw = 0.0f;
    const GmoNode *head = (const GmoNode *)self->head;
    const float *m;
    float x;
    float z;

    if (head != NULL) {
        m = head->localMatrix;
        x = m[0] * g_vecX.x + m[4] * g_vecX.y + m[8] * g_vecX.z + m[12] * g_vecX.w;
        z = m[2] * g_vecX.x + m[6] * g_vecX.y + m[10] * g_vecX.z + m[14] * g_vecX.w;
        yaw = atan2f(z, x);
    }
    return self->base.base.rot[1] + yaw;
}
