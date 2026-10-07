// bdc 0x088998b0 BtlCpuUnitCtor
#include "bdc.h"

/* Constructor of the CPU-controlled battle Bakugan class `BtlCpuUnit` (0x6d0 bytes, vtable
   `g_btlCpuUnitVtbl`, built by `BtlCreateBakugan` for non-player modes): runs
   `BtlBakuganCtor`, sets the vtable, stores `arg` in `targetPointVariant`, creates the AI
   (`BtlAiCreate`), loads the loadout (`BtlCombatLoadLoadout`), clears the ally (index -1),
   the ball-entry fields (step 9999), zeroes both `ctorVecs` (the VFPU bank's zero C720)
   and sets the remaining fields. AI level 5 by default; in script mode 1 (script var 8) sets collider layer 5
   (`BtlBakuganSetColliderLayer`) and, for kinds 0x15..0x1a while script bit 3 is clear,
   knock-out mode 1; otherwise profile word 0x18 (difficulty) picks the level: 0 → 1, 1 → 5,
   2 → 10, other values keep 5. Returns `self`. */

void *BtlCpuUnitCtor(BtlCpuUnit *self, s32 kind, s32 arg)
{
    s32 difficulty;

    BtlBakuganCtor(&self->base, kind);
    self->base.base.base.vtable = g_btlCpuUnitVtbl;
    self->base.targetPointVariant = arg;
    self->ai = BtlAiCreate(&self->base);
    BtlCombatLoadLoadout(&self->base.combat);
    self->aiStarted = 0;
    self->base.ballEntryPending = 0;
    self->ballEntryStep = 9999;
    self->allyIndex = -1;
    self->ally = NULL;
    self->ballModel = NULL;
    self->ctorVecs[0][0] = 0.0f;
    self->ctorVecs[0][1] = 0.0f;
    self->ctorVecs[0][2] = 0.0f;
    self->ctorVecs[0][3] = 0.0f;
    self->ctorVecs[1][0] = 0.0f;
    self->ctorVecs[1][1] = 0.0f;
    self->ctorVecs[1][2] = 0.0f;
    self->ctorVecs[1][3] = 0.0f;
    self->flag6b0 = 1;
    self->spawnerId = -1;
    self->targetStyle = 0;
    self->flag6bc = 0;
    self->word6c8 = NULL;
    self->aiLevel = 5;
    if (g_scriptGlobalVars[8] == 1) {
        BtlBakuganSetColliderLayer(&self->base, 5);
        if ((u32)(self->base.base.base.unk08 - 0x15) < 6 && !CoreBitsetTest(3, g_scriptGlobalBits)) {
            self->base.knockOutMode = 1;
        }
    } else {
        difficulty = (s32)SaveProfileGetWord(SaveGetProfile(), 0x18);
        if (difficulty == 0) {
            self->aiLevel = 1;
        } else if (difficulty == 1) {
            self->aiLevel = 5;
        } else if (difficulty == 2) {
            self->aiLevel = 10;
        }
    }
    return self;
}
