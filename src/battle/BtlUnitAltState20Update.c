// bdc 0x0885c31c BtlUnitAltState20Update
#include "bdc.h"

/* State-20 handler of `BtlUnitAlt` (slot 46): kinds other than 0x15 drop to state
   0 and return. Kind 0x15 steps on `subTimer`: step 0 plays motion 0x120 (blend 0.2, looped,
   `BtlBakuganPlayMotion`) and arms a 20-frame wait; step 1 counts it down; step 2 just advances;
   step 3 fires attack type 0x49 from bone 7 (a `BtlAttackCtor` object launched with
   `BtlAttackLaunchAuto`, speed 25) at the current target, or along the unit's heading when it
   has none. Any other step (negative or past 3) disarms the body collider and returns to state 0.
   Every kind-0x15 frame ends with `BtlBakuganApplyHover`. */
void BtlUnitAltState20Update(BtlBakugan *unit)
{
    /* motion-event attack record as read by BtlAttackLaunch */
    struct {
        u8 eventType;     /* 10 = attack */
        u8 extra[3];       /* bytes 1..3, zero here (role unknown) */
        s16 attackType;
        s16 bone;         /* owner model node the attack starts at */
        float paramF2;    /* copied to BtlAttack.paramF2 */
        float heightOffset;
        float speed;
    } ev;
    CollisionCollider *collider;
    void *target;
    BtlAttack *mem;
    BtlAttack *attack;
    bool fromLow;
    float dir[4];
    s32 step;

    if (unit->base.base.unk08 != 0x15) {
        BtlBakuganSetState(unit, 0, 0);
        return;
    }
    step = unit->subTimer;
    if (step == 0) {
        BtlBakuganPlayMotion(0.200000003f, unit, 0x120, 1, 0);
        unit->subWait = 20;
        unit->subTimer++;
    } else if (step == 1) {
        unit->subWait--;
        if (unit->subWait <= 0) {
            unit->subTimer = step + 1;
        }
    } else if (step == 2) {
        unit->subTimer = step + 1;
    } else if (step == 3) {
        ev.eventType = 10;
        ev.extra[0] = 0;
        ev.extra[1] = 0;
        ev.extra[2] = 0;
        ev.attackType = 0x49;
        ev.bone = 7;
        ev.paramF2 = 0.0f;
        ev.heightOffset = 0.0f;
        ev.speed = 25.0f;
        target = BtlBakuganGetTarget(unit);
        ev.attackType = 0x49;
        attack = NULL;
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mem = MemAlloc(0x160, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        if (mem != NULL) {
            BtlAttackCtor(mem, unit, ev.attackType);
            attack = mem;
        }
        if (target != NULL) {
            BtlAttackLaunchAuto(attack, NULL, NULL, (s16 *)&ev);
        } else {
            /* dir = (cos, 0, sin, 0) of the heading (vrot of heading * 2/pi in quarter turns) */
            dir[0] = __builtin_cosf(unit->base.rot[1]);
            dir[1] = 0.0f;
            dir[2] = __builtin_sinf(unit->base.rot[1]);
            dir[3] = 0.0f;
            BtlAttackLaunchAuto(attack, NULL, dir, (s16 *)&ev);
        }
        unit->subTimer++;
    } else {
        collider = unit->collider0;
        collider->hitTimer = 0;
        collider->flags &= ~1u;
        BtlBakuganSetState(unit, 0, 0);
    }
    BtlBakuganApplyHover(unit);
}
