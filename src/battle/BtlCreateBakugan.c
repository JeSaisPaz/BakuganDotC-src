// bdc 0x0886f8f4 BtlCreateBakugan
#include "bdc.h"

/* Factory for battle Bakugan objects. `kind` (1..0x14 = the 20 playable Bakugan) falls back to
   0x15 when its model `g_btlModelNames[kind]` is not in the loaded packages
   (`CorePackChainFindEntry`). The object is allocated from the low end of the heap: mode 0
   (player) builds a 0x670-byte `BtlBakuganCtor` unit for kinds 1..20 or a 0x6e0-byte
   `BtlUnitAltCtor` unit (variant 0) otherwise and sets `isPlayer` (also on a failed
   allocation); mode 4 builds a 0x710-byte `BtlUnitMode4Ctor` unit; other modes build a 0x6d0-byte
   `BtlCpuUnitCtor` unit for kinds 1..20, or a 0x6e0-byte `BtlUnitAltCtor` unit (variant
   `mode`) followed by `BtlBakuganSetColliderLayer``(unit, 5)`. On stages 0/13
   (`GameStageIs0Or13`) the model colour becomes (0.45, 0.45, 0.55, 1) and the ambient is scaled
   by 0.7 with alpha 1. With `spawn` (x, y, z, heading) the unit is dropped onto the ground from it
   (`CollisionRaycastPoint`) and turned to the heading. Finally sets the voice bank and the
   texture variant of the species copy. Returns the unit (NULL when allocation failed; the code
   after construction does not check). */

void *BtlCreateBakugan(int kind, int mode, void *spawn)
{
    const float *spawnPos = spawn;
    BtlBakugan *unit;
    void *mem;
    bool fromLow;

    if (CorePackChainFindEntry(g_ioLzsPackages, g_btlModelNames[kind]) == NULL) {
        kind = 0x15;
    }
    if (mode == 0) {
        if (kind >= 1 && kind <= 0x14) {
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            mem = MemAlloc(sizeof(BtlBakugan), NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            unit = NULL;
            if (mem != NULL) {
                BtlBakuganCtor(mem, kind);
                unit = mem;
            }
        } else {
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            mem = MemAlloc(sizeof(BtlUnitAlt), NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            unit = NULL;
            if (mem != NULL) {
                BtlUnitAltCtor(mem, kind, 0);
                unit = mem;
            }
        }
        unit->isPlayer = 1;
    } else if (mode == 4) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mem = MemAlloc(sizeof(BtlUnitMode4), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        unit = NULL;
        if (mem != NULL) {
            BtlUnitMode4Ctor(mem, kind, mode);
            unit = mem;
        }
    } else if (kind >= 1 && kind <= 0x14) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mem = MemAlloc(sizeof(BtlCpuUnit), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        unit = NULL;
        if (mem != NULL) {
            BtlCpuUnitCtor(mem, kind, mode);
            unit = mem;
        }
    } else {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mem = MemAlloc(sizeof(BtlUnitAlt), NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        unit = NULL;
        if (mem != NULL) {
            BtlUnitAltCtor(mem, kind, mode);
            unit = mem;
        }
        BtlBakuganSetColliderLayer(unit, 5);
    }
    if (GameStageIs0Or13() != 0 && unit != NULL) {
        unit->base.color[0] = 0.45f;
        unit->base.color[1] = 0.45f;
        unit->base.color[2] = 0.55f;
        unit->base.color[3] = 1.0f;
        /* ambient *= 0.7 (vscl.q), then alpha = 1 */
        unit->base.ambient[0] *= 0.7f;
        unit->base.ambient[1] *= 0.7f;
        unit->base.ambient[2] *= 0.7f;
        unit->base.ambient[3] *= 0.7f;
        unit->base.ambient[3] = 1.0f;
    }
    if (spawnPos != NULL) {
        CollisionRaycastPoint((float *)spawnPos, unit->base.pos);
        BtlBakuganSetHeading(unit, spawnPos[3]);
    }
    unit->voiceBank = BtlBakuganCalcVoiceBank(unit);
    BtlBakuganApplyTextureVariant(unit, BtlBakuganCountEarlierSameSpecies(unit));
    return unit;
}
