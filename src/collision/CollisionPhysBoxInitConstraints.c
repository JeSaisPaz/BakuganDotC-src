// bdc 0x089e6a60 CollisionPhysBoxInitConstraints
#include "bdc.h"

/* Resets a physics box's simulation state: clears `groundHit`, copies the pins from
   `g_collisionPhysBoxInitPins`, sets the rest length of each of the 28 corner pairs (pair table
   `g_collisionPhysBoxEdges`) to the current distance |pos[b] - pos[a]|, clears `floorY` and
   `contactVec` (bank column C720 = 0), clears the ground flags and `stepCounter`, and sets `gravity`
   to 0.3. */
void CollisionPhysBoxInitConstraints(CollisionPhysBox *self)
{
    s32 i;

    self->groundHit = 0;
    __builtin_memcpy(self->pins, g_collisionPhysBoxInitPins, sizeof(self->pins));
    for (i = 0; i < 28; i++) {
        const ScePspFVector4 *b = &self->pos[g_collisionPhysBoxEdges[i * 2 + 1]];
        const ScePspFVector4 *a = &self->pos[g_collisionPhysBoxEdges[i * 2]];
        float dx = b->x - a->x;
        float dy = b->y - a->y;
        float dz = b->z - a->z;

        self->restLen[i] = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    }
    self->floorY = 0.0f;
    self->contactVec.x = 0.0f;
    self->contactVec.y = 0.0f;
    self->contactVec.z = 0.0f;
    self->contactVec.w = 0.0f;
    self->raycastGround = 0;
    self->groundFlag = 0;
    self->stepCounter = 0;
    self->gravity = 0.3f;
}
