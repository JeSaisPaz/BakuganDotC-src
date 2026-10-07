// bdc 0x08850b58 BtlMainSpawnBakugan
#include "bdc.h"

/* Spawns the next battle Bakugan of kind `kind` (`kind < 1` only advances `*index` and returns
   NULL): takes stage point `*index` (`BtlGetStagePoint` table 0; `*index >= 4` returns NULL
   without advancing), drops it onto the ground (`CollisionRaycastPoint`), creates the unit
   (`BtlCreateBakugan`, mode 0 for slot 0 offline, or for slots below profile word 0x12 in
   network battles, else mode 2; NULL is returned as is without advancing), sets heading,
   position, spawn point `+0x560` (150 above the ground), `playerSlot = index`, texture variant,
   loadout, collider layer (unless rule mode 1) and HP gauge; for the local player (slot 0
   offline, the local slot online) also sets `isPlayer`, makes it the camera target
   (`BtlCameraSetTarget`, 20° pitch for kind 10), the active camera and sets its collider
   layer. Increments `*index` and returns the unit. */

void *BtlMainSpawnBakugan(BtlMain *self, int *index, int kind)
{
    float pos[4] __attribute__((aligned(16)));
    float spawn[4] __attribute__((aligned(16)));
    int slot;
    int isLocal;
    int mode;
    BtlBakugan *unit;
    BtlCamera *camera;
    int i;

    slot = *index;
    isLocal = 0;
    if (kind <= 0) {
        *index = *index + 1;
        return NULL;
    }
    if (slot >= 4) {
        return NULL;
    }
    BtlGetStagePoint(self, pos, slot, 0);
    pos[1] = pos[1] + 1000.0f;
    CollisionRaycastPoint(pos, pos);
    if (SaveGetProfileFlag0() == 0) {
        if (slot == 0) {
            mode = 0;
            isLocal = 1;
        } else {
            mode = 2;
        }
    } else {
        mode = 2;
        if (slot < (s32)SaveProfileGetWord(SaveGetProfile(), 0x12)) {
            mode = 0;
            if (slot == BtlMainGetLocalSlot(self)) {
                isLocal = 1;
            }
        }
    }
    unit = BtlCreateBakugan(kind, mode, NULL);
    if (unit != NULL) {
        BtlBakuganSetHeading(unit, pos[3]);
        /* quad copies: position, then spawn point 150 above the ground (via a stack temp) */
        for (i = 0; i < 4; i++) {
            unit->base.pos[i] = pos[i];
        }
        pos[1] = pos[1] + 150.0f;
        for (i = 0; i < 4; i++) {
            spawn[i] = pos[i];
        }
        for (i = 0; i < 4; i++) {
            unit->spawnPoint[i] = spawn[i];
        }
        unit->playerSlot = slot;
        BtlBakuganApplyTextureVariant(unit, BtlBakuganCountEarlierSameSpecies(unit));
        BtlCombatLoadLoadoutAndHandicap(&unit->combat);
        if (g_scriptGlobalVars[8] != 1) {
            BtlBakuganSetColliderLayer(unit, slot + 1);
        }
        BtlBakuganCreateHpGauge(unit);
        if (isLocal) {
            camera = &self->camera;
            unit->isPlayer = 1;
            BtlCameraSetTarget(camera, unit);
            if (unit->base.base.unk08 == 10) {
                self->camera.basePitch = 0.34906584f;
                self->camera.lockOnPitch = 0.34906584f;
            }
            BtlCameraSetDefaultFollow(camera, 1);
            g_gfxActiveCamera = &camera->base;
            BtlBakuganSetColliderLayer(unit, slot + 1);
        }
        *index = *index + 1;
    }
    return unit;
}
