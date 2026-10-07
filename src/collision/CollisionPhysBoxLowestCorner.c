// bdc 0x089e6b0c CollisionPhysBoxLowestCorner
#include "bdc.h"

/* Returns the corner particle of the physics box with the smallest Y. */
ScePspFVector4 *CollisionPhysBoxLowestCorner(CollisionPhysBox *self)
{
    ScePspFVector4 *pos = self->pos;
    int best = 0;
    int i;
    float minY = pos[0].y;

    for (i = 1; i < 8; i++) {
        float y = pos[i].y;
        if (!(minY <= y)) {
            minY = y;
            best = i;
        }
    }
    return pos + best;
}
