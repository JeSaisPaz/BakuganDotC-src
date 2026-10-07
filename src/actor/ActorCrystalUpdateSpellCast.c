// bdc 0x0885789c ActorCrystalUpdateSpellCast
#include "bdc.h"

/* Spell-cast step machine of the crystal on `castStep` (from `ActorCrystalUpdateLogic`). Runs
   only while the crystal is alive, casting is on (`autoCast` or an armed `scriptedCast`), the
   `onstageFlag` is clear and no cut-in runs (`BtlIsCutInRunning`). Steps 0..2 wait (600 frames
   first, then 60 between checks while egg crystals stand, `ActorStageObjCountStandingEggCrystals`)
   until the crystal's virtual slot 22 check fails and its HP is above 1. Step 10 raises `castPos`
   above the model, plays sound `0x20025a` and spawns effect 0x31 attached to it; after 50 frames
   effect 0x32 and sound `0x20025b`; after 40 more frames, for each of the three `orbitPoints` whose
   egg-crystal slot is empty (up to `spellTotal` for a scripted cast, else `spellCount` minus the
   standing egg crystals), it creates a type-0x80 `BtlAttack` (`BtlAttackCtor`,
   `BtlAttackLaunchStage`) with parameters from `ActorCrystalPickSpell`, focuses the camera on
   the first one (`focusCamera` with script flag 5, `BtlCameraFocusUnit`), applies
   `BtlCombatApplyDamage``(50)` to itself and, when slot 22 passes, refills its HP to
   `hpThreshold` × max. A scripted cast then waits 20 frames (step 0x1e), clears `scriptedCast` and
   sets a 1800-frame timer; an automatic cast waits a random 1200..2399 frames (step 0xf). Steps
   past the end (0x20, 99) restart at step 1. */

void ActorCrystalUpdateSpellCast(ActorCrystal *self)
{
    const VtblEntry *entry;
    BtlAttack *attack;
    void *mem;
    GfxEffect *effect;
    bool fromLow;
    s32 cast;
    s32 slot;
    s32 limit;
    s32 kind;
    s32 type;
    float power;
    s32 a;
    s32 b;
    s32 c;
    float orbitW;
    float threshold;
    u32 maxHp;
    u32 rnd;
    s32 timer;
    float dir[4];
    float vec[4];
    float pos[4];

    if (self->base.combat.dead != 0) {
        return;
    }
    if (self->autoCast == 0 && self->scriptedCast == 0) {
        return;
    }
    if (self->onstageFlag != 0) {
        return;
    }
    if (BtlIsCutInRunning()) {
        return;
    }
    switch (self->castStep) {
    case 0:
        self->castTimer = 600;
        self->castStep = self->castStep + 1;
        /* fallthrough */
    case 1:
        if (--self->castTimer > 0) {
            break;
        }
        self->castStep = self->castStep + 1;
        /* fallthrough */
    case 2:
        entry = &((const VtblEntry *)self->base.base.base.vtable)[22];
        if (((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta) != 0) {
            break;
        }
        if (BtlCombatGetHp(&self->base.combat) <= 1.0f) {
            break;
        }
        if (ActorStageObjCountStandingEggCrystals() == 0) {
            self->castStep = 10;
        } else {
            self->castTimer = 60;
            self->castStep = 1;
        }
        break;

    case 10:
        self->castPos[0] = self->base.base.pos[0];
        self->castPos[1] = self->base.base.pos[1];
        self->castPos[2] = self->base.base.pos[2];
        self->castPos[3] = self->base.base.pos[3];
        self->castPos[1] = self->base.base.data->bbox[5] * self->base.base.scale[1] + 10.0f +
                           self->castPos[1];
        if (self->focusCamera != 0 && CoreBitsetTest(5, g_scriptGlobalBits)) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x20025a, 0, 0);
            }
        } else if (SndHasListener()) {
            SndEmitterCreateAtPos(SndGetListener(), 0x20025a, self->castPos, 0, 1);
        }
        effect = (GfxEffect *)GfxEffectSpawn(g_btlUnitEffectMgr, 0x31, self->castPos);
        effect->attachPos = self->castPos;
        self->castTimer = 50;
        self->castStep = self->castStep + 1;
        /* fallthrough */
    case 11:
        if (--self->castTimer > 0) {
            break;
        }
        GfxEffectSpawn(g_btlUnitEffectMgr, 0x32, self->castPos);
        if (self->focusCamera != 0 && CoreBitsetTest(5, g_scriptGlobalBits)) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x20025b, 0, 0);
            }
        } else if (SndHasListener()) {
            SndEmitterCreateAtPos(SndGetListener(), 0x20025b, self->castPos, 0, 1);
        }
        self->castTimer = 40;
        self->castStep = self->castStep + 1;
        /* fallthrough */
    case 12:
        if (--self->castTimer > 0) {
            break;
        }
        self->castStep = self->castStep + 1;
        /* fallthrough */
    case 13:
        cast = 0;
        /* draws one random number and discards it (advances the generator) */
        (void)PlatformRandU32();
        if (self->scriptedCast != 0) {
            limit = self->spellTotal;
        } else {
            limit = self->spellCount;
        }
        for (slot = 0; slot < 3; slot++) {
            if (self->scriptedCast != 0) {
                if (!(cast < limit)) {
                    continue;
                }
            } else if (!(ActorStageObjCountStandingEggCrystals() + cast < limit)) {
                continue;
            }
            if (self->crystalSlots[slot] != NULL) {
                continue;
            }
            dir[2] = 0.0f;
            dir[1] = 0.0f;
            dir[0] = 0.0f;
            dir[3] = 0.0f;
            pos[0] = self->orbitPoints[slot][0];
            pos[1] = self->orbitPoints[slot][1];
            pos[2] = self->orbitPoints[slot][2];
            pos[3] = self->orbitPoints[slot][3];
            pos[1] = pos[1] + self->base.base.scale[1] * 1500.0f;
            orbitW = self->orbitPoints[slot][3];
            dir[1] = -1.0f;

            attack = NULL;
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            mem = MemAlloc(0x160, NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            if (mem != NULL) {
                BtlAttackCtor((BtlAttack *)mem, self, 0x80);
                attack = (BtlAttack *)mem;
            }
            BtlAttackLaunchStage(attack, pos, dir, NULL);
            if (cast == 0 && self->focusCamera != 0 && CoreBitsetTest(5, g_scriptGlobalBits)) {
                BtlCameraFocusUnit(1300.0f, -150.0f, 40.0f, (BtlMain *)BtlGetCameraTask(), attack,
                                   0, 1, NULL);
            }
            BtlCombatApplyDamage(50.0f, &self->base.combat, 0, 1);
            entry = &((const VtblEntry *)self->base.base.base.vtable)[22];
            if (((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta) != 0) {
                threshold = self->base.hpThreshold;
                maxHp = (u32)BtlCombatGetMaxHp(&self->base.combat);
                BtlCombatSetHp((float)maxHp * threshold, &self->base.combat);
            }

            kind = 0;
            type = 0;
            power = 250.0f;
            a = 0;
            b = 0;
            c = 0;
            ActorCrystalPickSpell(self, &kind, &type, cast, (int *)&power, &a, &b, &c);
            attack->param0 = kind;
            attack->param1 = self->style;
            attack->auxVec[0] = (float)type;
            attack->auxVec[1] = (float)self->spawnFlag;
            attack->auxVec[2] = (float)slot;
            attack->auxVec[3] = orbitW;
            vec[0] = power;
            vec[1] = (float)a;
            vec[2] = (float)b;
            vec[3] = (float)c;
            attack->summonVec[0] = vec[0];
            attack->summonVec[1] = vec[1];
            attack->summonVec[2] = vec[2];
            attack->summonVec[3] = vec[3];
            cast++;
        }
        self->spellTotal = 0;
        if (self->scriptedCast != 0) {
            self->castTimer = 20;
            self->castStep = 0x1e;
            break;
        }
        self->castStep = self->castStep + 1;
        /* fallthrough */
    case 14:
        rnd = PlatformRandU32();
        self->castTimer = (s32)(((rnd >> 16) * 1200 >> 16) + 1200);
        self->castStep = self->castStep + 1;
        /* fallthrough */
    case 15:
        if (--self->castTimer > 0) {
            break;
        }
        self->castStep = 99;
        break;

    case 0x1e:
        if (--self->castTimer > 0) {
            break;
        }
        self->castStep = self->castStep + 1;
        /* fallthrough */
    case 0x1f:
        timer = 1800;
        if (self->focusCamera != 0) {
            CoreBitsetTest(5, g_scriptGlobalBits); /* result unused */
        }
        self->castTimer = timer;
        self->scriptedCast = 0;
        self->castStep = self->castStep + 1;
        break;

    default:
        self->castStep = 1;
        break;
    }
}
