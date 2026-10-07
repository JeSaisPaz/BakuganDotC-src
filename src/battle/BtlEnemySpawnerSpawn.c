// bdc 0x088a5770 BtlEnemySpawnerSpawn
#include "bdc.h"

/* Spawns one enemy unit 120 units from the spawner centre in a random horizontal direction: the
   angle is `(PlatformRandFloat12() - 1) * 2pi` (a random float in [0, 2pi)), turned into
   (cos, 0, sin) * 120 and added to the centre (`centerW` unused, pos.w = 0).
   Creates a mode-4 unit of kind 0x15 at that point (`BtlCreateBakugan`), gives it an HP gauge
   (`BtlBakuganCreateHpGauge`), player slot 4, collider layer 5
   (`BtlBakuganSetColliderLayer`), AI level 3 (`BtlBakuganSetAiLevel`), arms its ball entry
   (`BtlBakuganBeginBallEntry`), sets its input's allowed-action mask to `0xffffbff3`, combat
   level 0 (`BtlCombatSetLevel`), tags it with the spawner id, clears `keepHpFull`, sets HP to 60
   (`BtlCombatFillHp`), calls its vtable entry 7 and increments the spawner's `spawnCount`. */

void BtlEnemySpawnerSpawn(void *spawner)
{
    BtlEnemySpawner *self = spawner;
    float pos[4] __attribute__((aligned(16)));
    float angle;
    float quarter;
    void *obj;
    BtlBakugan *unit;
    const VtblEntry *entry;

    angle = (PlatformRandFloat12() - 1.0f) * 6.28318548f;
    quarter = angle * 0.636619772f;
    /* pos = (cos, 0, sin, 0) * 120 (xyz scaled), then + centre (xyz only) */
    pos[0] = VfCosQuarter(quarter) * 120.0f;
    pos[1] = 0.0f * 120.0f;
    pos[2] = VfSinQuarter(quarter) * 120.0f;
    pos[3] = 0.0f;
    pos[0] = pos[0] + self->centerX;
    pos[1] = pos[1] + self->centerY;
    pos[2] = pos[2] + self->centerZ;
    obj = BtlCreateBakugan(0x15, 4, pos);
    unit = obj;
    BtlBakuganCreateHpGauge(unit);
    unit->playerSlot = 4;
    BtlBakuganSetColliderLayer(unit, 5);
    BtlBakuganSetAiLevel(unit, 3);
    BtlBakuganBeginBallEntry(unit, 1);
    unit->input->allowedActions = 0xffffbff3;
    BtlCombatSetLevel(&unit->combat, 0);
    ((BtlCpuUnit *)obj)->spawnerId = self->id;
    ((BtlUnitMode4 *)obj)->keepHpFull = 0;
    BtlCombatFillHp(&unit->combat, 60.0f);
    entry = &((const VtblEntry *)unit->base.base.vtable)[7];
    ((void (*)(void *))entry->fn)((u8 *)obj + entry->delta);
    self->spawnCount++;
}
