// bdc 0x088a3858 ActorCrystalStandCheckOrigin
#include "bdc.h"

/* When the stand sits (within 1 unit) on the fixed point (0, 0, 2150) (`g_actorCrystalStandOrigin`,
   lazily initialised), makes its collider solid (`ActorCrystalStandSetCollidable``(stand, 0)`)
   and returns 1; else returns 0. Used on arenas > 0x23 by `ActorCrystalStandUpdate` and by
   `ActorCrystalStandSpawnAtPoint`. */

int ActorCrystalStandCheckOrigin(ActorCrystalStand *stand)
{
    float dx;
    float dy;
    float dz;
    float dist;

    if (g_actorCrystalStandOriginInit == 0) {
        g_actorCrystalStandOriginInit = 1;
        g_actorCrystalStandOrigin[0] = 0.0f;
        g_actorCrystalStandOrigin[1] = 0.0f;
        g_actorCrystalStandOrigin[2] = 2150.0f;
        g_actorCrystalStandOrigin[3] = 0.0f;
    }
    dx = g_actorCrystalStandOrigin[0] - stand->base.pos[0];
    dy = g_actorCrystalStandOrigin[1] - stand->base.pos[1];
    dz = g_actorCrystalStandOrigin[2] - stand->base.pos[2];
    dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    if (!(dist < 1.0f))
        return 0;
    ActorCrystalStandSetCollidable(stand, 0);
    return 1;
}
