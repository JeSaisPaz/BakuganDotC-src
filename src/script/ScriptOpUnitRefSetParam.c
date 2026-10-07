// bdc 0x08811ee4 ScriptOpUnitRefSetParam
#include "bdc.h"

/* Script opcode that sets one property of a battle unit given by ref (`BtlBakuganListFind`; needs
   the camera/main task 100). Operands: u16 `cmd` (0..0x10), ref, 4 floats (`v`, …). `cmd`: 0
   place at the position (ground point via `CollisionRaycastPoint`, yaw in degrees via
   `BtlBakuganSetHeading`); 1 set HP from `v` (`BtlCombatSetHp`; at 0 sets `dead` `+0x4c1` and
   `BtlBakuganSetState``(unit, 6, 0)`); 2 `BtlBakuganSetAiLevel`; 3
   `BtlBakuganRetarget``(unit, 1)`; 4 level 0..9 (`BtlCombatSetLevel`); 5 `BtlCombatFillHp`; 6
   byte `+0x59c`; 7 `BtlUnitMode4StartRetreat`; 8 virtual `+0xac` with `v*0.01`; 9
   `BtlBakuganSetInert`; 10/11 `BtlBakuganSetAttackLevel`/`BtlBakuganSetDefenseLevel``(unit,
   v-1)`; 12 `BtlBakuganSetAiTargetStyle`; 13 `+0x660`; 14 `BtlBakuganSetTarget``(unit, last
   unit whose virtual +0x9c is true)`; 15 `BtlBakuganSetTarget``(player, unit)`; 16
   `BtlBakuganSetTarget``(unit, idx-th unit (v))`. Returns 0. */

int ScriptOpUnitRefSetParam(Script *script)
{
    u32 cmd;
    BtlBakugan **ref;
    BtlBakugan *refUnit;
    BtlBakugan *unit;
    BtlBakugan *player;
    CoreObjectList *list;
    CoreObject *obj;
    CoreObject *found;
    void *crystal;
    const VtblEntry *entry;
    float scale;
    float v[4] __attribute__((aligned(16)));

    cmd = ScriptReadU16(script);
    ref = (BtlBakugan **)ScriptReadRef(script, 2);
    refUnit = *ref;
    v[2] = 0.0f;
    v[1] = 0.0f;
    v[0] = 0.0f;
    v[3] = 0.0f;
    v[0] = ScriptReadFloat(script);
    v[1] = ScriptReadFloat(script);
    v[2] = ScriptReadFloat(script);
    v[3] = ScriptReadFloat(script);
    unit = BtlBakuganListFind(refUnit);
    if (BtlCameraTaskExists() == 0 || unit == NULL) {
        return 0;
    }
    switch (cmd) {
    case 0:
        v[3] = v[3] * 0.017453292f;
        CollisionRaycastPoint(v, unit->base.pos);
        BtlBakuganSetHeading(unit, v[3]);
        break;
    case 1:
        scale = v[0] * 0.01f;
        v[0] = scale;
        BtlCombatSetHp((float)(u32)BtlCombatGetMaxHp(&unit->combat) * scale, &unit->combat);
        if (v[0] == 0.0f) {
            unit->combat.dead = 1;
            BtlBakuganSetState(unit, 6, 0);
        }
        break;
    case 2:
        entry = &((const VtblEntry *)unit->base.base.vtable)[13];
        if (((int (*)(void *))entry->fn)((u8 *)unit + entry->delta) != 0) {
            BtlBakuganSetAiLevel(unit, (int)v[0]);
        }
        break;
    case 3:
        BtlBakuganRetarget(unit, 1);
        break;
    case 4:
        if (v[0] < 0.0f) {
            v[0] = 0.0f;
        } else if (!(v[0] <= 9.0f)) {
            v[0] = 9.0f;
        }
        BtlCombatSetLevel(&unit->combat, (int)v[0]);
        break;
    case 5:
        if (v[0] < 1.0f) {
            v[0] = 1.0f;
        } else if (!(v[0] <= 2000.0f)) {
            v[0] = 2000.0f;
        }
        BtlCombatFillHp(&unit->combat, v[0]);
        break;
    case 6:
        if (v[0] <= 0.0f) {
            unit->untargetable = 0;
        } else {
            unit->untargetable = 1;
        }
        break;
    case 7:
        entry = &((const VtblEntry *)unit->base.base.vtable)[18];
        if (((int (*)(void *))entry->fn)((u8 *)unit + entry->delta) != 0) {
            BtlUnitMode4StartRetreat((BtlUnitMode4 *)unit);
        }
        break;
    case 8:
        entry = &((const VtblEntry *)unit->base.base.vtable)[21];
        ((void (*)(void *, float))entry->fn)((u8 *)unit + entry->delta, v[0] * 0.01f);
        break;
    case 9:
        if (v[0] <= 0.0f) {
            BtlBakuganSetInert(unit, 0);
        } else {
            BtlBakuganSetInert(unit, 1);
        }
        break;
    case 10:
        entry = &((const VtblEntry *)unit->base.base.vtable)[13];
        if (((int (*)(void *))entry->fn)((u8 *)unit + entry->delta) != 0) {
            BtlBakuganSetAttackLevel(unit, (int)(v[0] - 1.0f));
        }
        break;
    case 11:
        entry = &((const VtblEntry *)unit->base.base.vtable)[13];
        if (((int (*)(void *))entry->fn)((u8 *)unit + entry->delta) != 0) {
            BtlBakuganSetDefenseLevel(unit, (int)(v[0] - 1.0f));
        }
        break;
    case 12:
        entry = &((const VtblEntry *)unit->base.base.vtable)[13];
        if (((int (*)(void *))entry->fn)((u8 *)unit + entry->delta) != 0) {
            BtlBakuganSetAiTargetStyle(unit, (int)v[0]);
        }
        break;
    case 13:
        unit->knockOutMode = (int)v[0];
        break;
    case 14:
        found = NULL;
        list = (CoreObjectList *)BtlGetBakuganList();
        if (list != NULL) {
            for (obj = list->head; obj != NULL; obj = obj->next) {
                entry = &((const VtblEntry *)obj->vtable)[19];
                if (((int (*)(void *))entry->fn)((u8 *)obj + entry->delta) != 0) {
                    found = obj;
                }
            }
        }
        if (found != NULL) {
            BtlBakuganSetTarget(unit, found);
        }
        break;
    case 15:
        player = BtlBakuganListFind(BtlGetPlayerBakugan());
        if (player != NULL) {
            BtlBakuganSetTarget(player, unit);
        }
        break;
    case 16:
        crystal = BtlGetNthCrystal((int)v[0]);
        if (crystal != NULL) {
            BtlBakuganSetTarget(unit, crystal);
        }
        break;
    }
    return 0;
}
