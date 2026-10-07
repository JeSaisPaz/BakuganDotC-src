// bdc 0x089e6ce0 CollisionPhysBoxUpdateVelocities
#include "bdc.h"

/* Per corner: d = pos[i] - prevPos[i] - offset[i] (xyz), then prevPos[i] = pos[i] (all four lanes).
   An unpinned corner (pin 0) gets vel[i].xyz = d × 0.98974609375 (half-float 0x3beb); a pin of 1 sets
   vel[i] to zero (bank column C720); other pin values leave vel[i] untouched. */
void CollisionPhysBoxUpdateVelocities(CollisionPhysBox *self, const ScePspFVector4 *offset)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        ScePspFVector4 *prev = &self->prevPos[i];
        const ScePspFVector4 *cur = &self->pos[i];
        float dx = cur->x - prev->x - offset[i].x;
        float dy = cur->y - prev->y - offset[i].y;
        float dz = cur->z - prev->z - offset[i].z;

        *prev = *cur;
        if (self->pins[i] == 0) {
            ScePspFVector4 *vel = &self->vel[i];

            vel->x = dx * 0.98974609375f;
            vel->y = dy * 0.98974609375f;
            vel->z = dz * 0.98974609375f;
        } else if (self->pins[i] < 2) {
            ScePspFVector4 *vel = &self->vel[i];

            vel->x = 0.0f;
            vel->y = 0.0f;
            vel->z = 0.0f;
            vel->w = 0.0f;
        }
    }
}
