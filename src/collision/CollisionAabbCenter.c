// bdc 0x08a32430 CollisionAabbCenter
#include "bdc.h"

/* Returns the centre of an axis-aligned box `{min, max}` (floats 0–3 and 4–7 of `aabb`): `(min + max) * 0.5f`
   written to the static vector `g_collisionAabbCenterTmp`, whose address is returned. Only x, y, z are
   computed; `w` is lane 3 of the `vscl.t` destination C710, the bank constant S713 (0.0f). */

ScePspFVector4 *CollisionAabbCenter(const ScePspFVector4 *aabb)
{
    const float *v = &aabb->x; /* {min, max} as 8 floats */
    float x;
    float y;
    float z;

    x = v[0] + v[4];
    y = v[1] + v[5];
    z = v[2] + v[6];
    g_collisionAabbCenterTmp.x = x * 0.5f;
    g_collisionAabbCenterTmp.y = y * 0.5f;
    g_collisionAabbCenterTmp.z = z * 0.5f;
    g_collisionAabbCenterTmp.w = 0.0f; /* S713 */
    return &g_collisionAabbCenterTmp;
}
