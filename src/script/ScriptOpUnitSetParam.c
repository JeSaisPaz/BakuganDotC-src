// bdc 0x08811718 ScriptOpUnitSetParam
#include "bdc.h"

/* Script opcode that sets one of 26 parameters of the `idx`-th battle crystal (`BtlGetNthCrystal`,
   validated with `BtlBakuganListFind`; nothing happens when it is gone or `cmd` > 0x19).
   Operands: u16 `idx` (sign-extended), u32 `cmd`, 4 floats `v0..v3` (`v3` a yaw in degrees,
   converted to radians for the vec4 commands). `slot` is `(s32)v0`. Per `cmd`:
   0 battle-unit virtual entry 21 (HP threshold, `BtlBakuganSetHpThreshold`) with `v0 * 0.01`;
   1 `v0 <= 1`: `autoCast = v0 > 0`, else `ActorCrystalCastSpells``(v1, v2, v3, v0 <= 2)`;
   2 `ActorCrystalSetSpellKinds`; 3 `ActorSetOnstage``(v0 > 0)` below 2, otherwise onstage
   plus `mode2Proceed`; 4 `scriptFlag`, 5 `autoFire` (`v0 > 0`); 6 barrier on/off
   (`ActorStartVexosBarrier`/`ActorStopVexosBarrier`); 7 `ActorCrystalSetSpellTypes`;
   8..10 `orbitPoints[0..2]` and 11 `warpPos` = `{v0, v1, v2, rad(v3)}`; 12 fade
   (`ActorCrystalStartFade` for `v0 <= 0`, `ActorCrystalResetTintAndStopFade` below 2,
   else `ActorCrystalResetTintAndStartFade`); 13 `warpPos` = position of the type-8 stage record
   `slot` (`ActorStageObjRecordGetType8Pos`, id 0 unless `v0` is below
   `ActorStageObjRecordCountType8`); 14 `BtlCombatSetLevel` 0..9; 15 `BtlCombatFillHp`
   1..2000; 16 `attackRange` 1..10000; 17 `scriptRange2` 1..10000; 18 `scriptLevel = v0 - 1`
   0..9; 19..23 spell slot `slot`: `ActorCrystalSetSpellType` 1..10,
   `ActorCrystalSetSpellPower` 0..2000, `ActorCrystalSetSpellParamA`/`B` `v1 - 1` 0..9,
   `ActorCrystalSetSpellParamC` 0..9 (all from `v1`); 24 `attackParam1` 1..360;
   25 `attackParam0` 1..1000. Clamps take the upper bound for NaN. Always returns 0. */

/* `v < lo` -> lo, `!(v <= hi)` -> hi (NaN -> hi), else v. */
static inline float ScriptClampF(float v, float lo, float hi)
{
    if (v < lo) {
        return lo;
    }
    if (!(v <= hi)) {
        return hi;
    }
    return v;
}

int ScriptOpUnitSetParam(Script *script)
{
    float vec[4];
    float pos[4];
    float *dst;
    void *unit;
    ActorCrystal *crystal;
    u32 cmd;
    float v0;
    float v1;
    float v2;
    float v3;
    s32 slot;
    s32 id;
    const VtblEntry *entry;

    unit = BtlGetNthCrystal((s16)ScriptReadU16(script));
    cmd = ScriptReadU32(script);
    v0 = ScriptReadFloat(script);
    v1 = ScriptReadFloat(script);
    v2 = ScriptReadFloat(script);
    v3 = ScriptReadFloat(script);
    slot = (s32)v0;
    vec[0] = v0;
    vec[1] = v1;
    vec[2] = v2;
    vec[3] = v3 * 0.017453292f;
    crystal = BtlBakuganListFind(unit);
    if (crystal == NULL || cmd >= 26) {
        return 0;
    }
    switch (cmd) {
    case 0:
        entry = &((const VtblEntry *)crystal->base.base.base.vtable)[21];
        ((void (*)(void *, float))entry->fn)((u8 *)crystal + entry->delta, v0 * 0.01f);
        break;
    case 1:
        if (v0 <= 1.0f) {
            if (v0 <= 0.0f) {
                crystal->autoCast = 0;
            } else {
                crystal->autoCast = 1;
            }
        } else if (v0 <= 2.0f) {
            ActorCrystalCastSpells(crystal, (s32)v1, (s32)v2, (s32)v3, 1);
        } else {
            ActorCrystalCastSpells(crystal, (s32)v1, (s32)v2, (s32)v3, 0);
        }
        break;
    case 2:
        ActorCrystalSetSpellKinds(crystal, (s32)v1, (s32)v2, (s32)v3);
        break;
    case 3:
        if (v0 < 2.0f) {
            if (v0 <= 0.0f) {
                ActorSetOnstage((Actor *)crystal, 0);
            } else {
                ActorSetOnstage((Actor *)crystal, 1);
            }
        } else {
            ActorSetOnstage((Actor *)crystal, 1);
            crystal->mode2Proceed = 1;
        }
        break;
    case 4:
        if (v0 <= 0.0f) {
            crystal->scriptFlag = 0;
        } else {
            crystal->scriptFlag = 1;
        }
        break;
    case 5:
        if (v0 <= 0.0f) {
            crystal->autoFire = 0;
        } else {
            crystal->autoFire = 1;
        }
        break;
    case 6:
        if (v0 <= 0.0f) {
            ActorStopVexosBarrier((Actor *)crystal);
        } else {
            ActorStartVexosBarrier((Actor *)crystal);
        }
        break;
    case 7:
        ActorCrystalSetSpellTypes(crystal, (s32)v1, (s32)v2, (s32)v3);
        break;
    case 8:
    case 9:
    case 10:
    case 11:
        dst = (cmd == 11) ? crystal->warpPos : crystal->orbitPoints[cmd - 8];
        dst[0] = vec[0];
        dst[1] = vec[1];
        dst[2] = vec[2];
        dst[3] = vec[3];
        break;
    case 12:
        if (v0 < 2.0f) {
            if (v0 <= 0.0f) {
                ActorCrystalStartFade(crystal);
            } else {
                ActorCrystalResetTintAndStopFade(crystal);
            }
        } else {
            ActorCrystalResetTintAndStartFade(crystal);
        }
        break;
    case 13:
        id = 0;
        if (!((float)ActorStageObjRecordCountType8() <= v0)) {
            id = slot;
        }
        ActorStageObjRecordGetType8Pos(pos, id);
        crystal->warpPos[0] = pos[0];
        crystal->warpPos[1] = pos[1];
        crystal->warpPos[2] = pos[2];
        crystal->warpPos[3] = pos[3];
        break;
    case 14:
        BtlCombatSetLevel(&crystal->base.combat, (s32)ScriptClampF(v0, 0.0f, 9.0f));
        break;
    case 15:
        BtlCombatFillHp(&crystal->base.combat, ScriptClampF(v0, 1.0f, 2000.0f));
        break;
    case 16:
        crystal->attackRange = ScriptClampF(v0, 1.0f, 10000.0f);
        break;
    case 17:
        crystal->scriptRange2 = ScriptClampF(v0, 1.0f, 10000.0f);
        break;
    case 18:
        crystal->scriptLevel = ScriptClampF(v0 - 1.0f, 0.0f, 9.0f);
        break;
    case 19:
        ActorCrystalSetSpellType(crystal, slot, (s32)ScriptClampF(v1, 1.0f, 10.0f));
        break;
    case 20:
        ActorCrystalSetSpellPower(crystal, slot, ScriptClampF(v1, 0.0f, 2000.0f));
        break;
    case 21:
        ActorCrystalSetSpellParamA(crystal, slot, (s32)ScriptClampF(v1 - 1.0f, 0.0f, 9.0f));
        break;
    case 22:
        ActorCrystalSetSpellParamB(crystal, slot, (s32)ScriptClampF(v1 - 1.0f, 0.0f, 9.0f));
        break;
    case 23:
        ActorCrystalSetSpellParamC(crystal, slot, (s32)ScriptClampF(v1, 0.0f, 9.0f));
        break;
    case 24:
        crystal->attackParam1 = (s32)ScriptClampF(v0, 1.0f, 360.0f);
        break;
    case 25:
        crystal->attackParam0 = (s32)ScriptClampF(v0, 1.0f, 1000.0f);
        break;
    }
    return 0;
}
