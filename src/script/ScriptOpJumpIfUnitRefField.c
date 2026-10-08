// bdc 0x0880fb44 ScriptOpJumpIfUnitRefField
#include "bdc.h"

/* Conditional jump on a field of a battle unit given by ref (`BtlBakuganListFind`). Operands:
   ref, u16 `field`, u16 `cmp`, float `value`, u16 `target`. `field`: 1 `lastCombo`; 2
   `crystalBreakCount`; 3 position z (`base.pos[2]`); 4 `BtlCountAliveBakugan``()` (live units); 5
   `koCameraDone`; 0 `ballEntryPending == 0`; `field` >= 6 skips the comparison. The fetched value
   (0 when the unit or player is missing) is compared against `value` by `cmp`: 0 `<`, 1 `<=`, 2
   `==`, 3 `!=`, 4 `>`, 5 `>=` (negated forms, so 3..5 are true for NaN); `cmp` >= 6 compares
   nothing. Jumps to `target` (returns 3) when the comparison holds or when the unit or the player
   is missing; otherwise returns 0. */

int ScriptOpJumpIfUnitRefField(Script *script)
{
    u32 refVal;
    u32 field;
    u32 cmp;
    float value;
    u32 target;
    int jump;
    BtlBakugan *unit;
    void *player;
    float cur;

    refVal = *ScriptReadRef(script, 2);
    field = ScriptReadU16(script);
    cmp = ScriptReadU16(script);
    value = ScriptReadFloat(script);
    target = ScriptReadU16(script);
    jump = 0;
    unit = (BtlBakugan *)BtlBakuganListFind((BtlBakugan *)PspPtr(refVal));
    if (unit == NULL) {
        jump = 1;
    }
    player = BtlGetPlayerBakugan();
    if (player == NULL) {
        jump = 1;
    }
    cur = 0.0f;
    if (player != NULL && unit != NULL) {
        if (field < 6) {
            switch (field) {
            case 1:
                cur = (float)unit->lastCombo;
                break;
            case 2:
                cur = (float)unit->crystalBreakCount;
                break;
            case 3:
                cur = unit->base.pos[2];
                break;
            case 4:
                cur = (float)BtlCountAliveBakugan();
                break;
            case 5:
                cur = (float)unit->koCameraDone;
                break;
            default:
                cur = (float)(unit->ballEntryPending == 0);
                break;
            }
        } else {
            cmp = 10;
        }
    }
    if (cmp < 6) {
        switch (cmp) {
        case 1:
            if (cur <= value) {
                jump = 1;
            }
            break;
        case 2:
            if (cur == value) {
                jump = 1;
            }
            break;
        case 3:
            if (!(cur == value)) {
                jump = 1;
            }
            break;
        case 4:
            if (!(cur <= value)) {
                jump = 1;
            }
            break;
        case 5:
            if (!(cur < value)) {
                jump = 1;
            }
            break;
        default:
            if (cur < value) {
                jump = 1;
            }
            break;
        }
    }
    if (jump) {
        script->curTrack->pc = (u16)target;
        return 3;
    }
    return 0;
}
