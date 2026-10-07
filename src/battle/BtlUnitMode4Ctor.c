// bdc 0x0885cea0 BtlUnitMode4Ctor
#include "bdc.h"

/* Constructor of the mode-4 battle unit class `BtlUnitMode4` (0x710 bytes, vtable
   `g_btlUnitMode4Vtbl`, built by `BtlCreateBakugan` for `mode == 4`, which passes
   `variant = mode`): runs the CPU-unit base constructor `BtlCpuUnitCtor` with `variant`
   (stored in `targetPointVariant`), installs its vtable, zeroes `prevPos` and
   `retreatPoint` (stores of the bank zero vector C720), clears the
   retreat state and sets `keepHpFull` when `targetPointVariant` is 0. For playable kinds 1..20 it
   writes the unit kind's loadout words into the profile word table (each only when the table
   exists) and reloads the HP with `BtlCombatLoadLoadoutAndHandicap`; then calls
   `BtlCombatArtSlotsNoOp` and clears the input's allowed-action mask. Returns `unit`. */
void *BtlUnitMode4Ctor(void *unit, int kind, s32 variant)
{
    BtlUnitMode4 *self = (BtlUnitMode4 *)unit;
    BtlBakugan *bakugan = &self->base;
    SaveProfile *profile;
    u32 unitKind;

    BtlCpuUnitCtor(unit, kind, variant);
    bakugan->base.base.vtable = g_btlUnitMode4Vtbl;
    self->started = 0;
    self->prevPos[0] = 0.0f;
    self->prevPos[1] = 0.0f;
    self->prevPos[2] = 0.0f;
    self->prevPos[3] = 0.0f;
    self->travelled = 0.0f;
    self->keepHpFull = bakugan->targetPointVariant == 0;
    self->retreatStep = 0;
    self->stepTimer = 0;
    self->retreatPoint[0] = 0.0f;
    self->retreatPoint[1] = 0.0f;
    self->retreatPoint[2] = 0.0f;
    self->retreatPoint[3] = 0.0f;
    if (kind > 0 && kind < 0x15) {
        unitKind = bakugan->base.base.unk08;
        profile = (SaveProfile *)SaveGetProfile();
        if (profile->words != NULL) {
            profile->words[0x36] = unitKind * 4 - 4;
        }
        profile = (SaveProfile *)SaveGetProfile();
        if (profile->words != NULL) {
            profile->words[0x37] = 0xff;
        }
        profile = (SaveProfile *)SaveGetProfile();
        if (profile->words != NULL) {
            profile->words[0x3e] = 0xff;
        }
        profile = (SaveProfile *)SaveGetProfile();
        if (profile->words != NULL) {
            profile->words[0x3f] = 0xff;
        }
        BtlCombatLoadLoadoutAndHandicap(&bakugan->combat);
    }
    BtlCombatArtSlotsNoOp();
    bakugan->input->allowedActions = 0;
    self->retreating = 0;
    self->arrived = 0;
    return unit;
}
