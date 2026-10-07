// bdc 0x0886bf60 BtlBakuganProcessMotionEvents
#include "bdc.h"

/* Runs the motion events of the Bakugan's current attack (called by `BtlBakuganUpdate` when flag 4
   of `stateFlags` is set) and returns the result of the last hit check that ran (0 when none ran).
   Returns 0 at once when there is no motion set (combo step set from
   `BtlBakuganGetComboStepMotionSet` when `comboIndex` != -1, else `attackMotions[attackIndex]`) or
   when the camera task exists and save-profile word 2 is 0. Otherwise walks the `BtlMotionEvent`
   records after the set head until a type-0 record. Each record's frame window is scaled by 5/7
   (status 4) and 20/13 (status 5) unless `stateFlags` 0x100 is set, clamped to >= 1, and tested
   against three frames: `attackFrame` (window "attack"; for types 2/10/11 also closed by
   `stateFlags` 0x2000000), the motion frame (`GfxModelGetMotionFrameInt`, minus 2 during the
   final step, clamped to >= 0; open only in attack phases 1/2, or always for types 3/6/11 during the
   final step, never with `stateFlags` 0x2000000) and `attackEventFrame`. Types: 1 motion-frame,
   2 attack, 3 final step (motion frame or `forceFinalHit`), 4 event-frame hit checks
   (`BtlBakuganMotionHitCheck`); 5 (any step) / 6 (final step) sound in states 11/9/7, 7 event-frame
   sound (`SndObjectAddEmitter` on `base.sound`); 8 effect `param.ref.id` (0x8e plus the unit's
   attribute, vtable slot 20) on `g_worldEffectMgr`, attached to the anchor position (bone 0x7f,
   `GfxEffectSpawnAttachedDir`) or at a node (`GfxEffectSpawnDirected`), pointing along the
   facing, owned by the unit; 9 motion speed `value` (vtable slot 6); 10 (any step) / 11 (final step)
   spawn a `BtlAttack` (low heap) of type `param.ref.id` (1 becomes 11 under status 6) and launch
   it with `BtlAttackLaunchAuto` at the target, or along the facing without one, unless
   `attackSpawnBlocked`; 12 event-frame dash speed `value`. `attackSpawnBlocked` is restored to its
   entry value on exit. The facing vector is (cos yaw, 0, sin yaw, 0) of `base.rot[1]`. */

int BtlBakuganProcessMotionEvents(BtlBakugan *self)
{
    void *set;
    BtlMotionEvent *ev;
    GfxEffectMgr *mgr;
    GfxEffect *effect;
    BtlAttack *attack;
    const VtblEntry *entry;
    s32 attackFrame;
    s32 frame;
    s32 start;
    s32 end;
    s32 effectId;
    s32 bone;
    s32 attackType;
    u32 stateFlags;
    u32 phase;
    u8 savedBlocked;
    u8 type;
    bool inAttackWindow;
    bool attackWindowOpen;
    bool inMotionWindow;
    bool isFinalType;
    bool fromLow;
    int hit;
    float attackDir[4] __attribute__((aligned(16)));
    float effectDir[4] __attribute__((aligned(16)));
    ScePspFVector4 nodePos __attribute__((aligned(16)));

    set = self->attackMotions[self->attackIndex];
    savedBlocked = self->attackSpawnBlocked;
    if (self->comboIndex != -1) {
        set = BtlBakuganGetComboStepMotionSet(self);
    }
    if (BtlCameraTaskExists() != 0) {
        BtlGetCameraTask();
        if (SaveProfileGetWord(SaveGetProfile(), 2) == 0) {
            return 0;
        }
    }
    if (set == NULL) {
        return 0;
    }
    attackFrame = self->attackFrame;
    ev = (BtlMotionEvent *)((BtlAttackMotionSet *)set + 1);
    frame = GfxModelGetMotionFrameInt(&self->base);
    if (self->finalStepPlaying != 0) {
        frame -= 2;
    }
    if (frame < 0) {
        frame = 0;
    }
    hit = 0;
    for (; (type = ev->type) != 0; ev++) {
        start = ev->startFrame;
        end = ev->endFrame;
        stateFlags = self->stateFlags;
        if (self->combat.status[4].active != 0 && (stateFlags & 0x100) == 0) {
            start = (s32)((float)start * 0.714285731f);
            end = (s32)((float)end * 0.714285731f);
        }
        if (self->combat.status[5].active != 0 && (stateFlags & 0x100) == 0) {
            start = (s32)((float)start * 1.53846157f);
            end = (s32)((float)end * 1.53846157f);
        }
        if (start <= 0) {
            start = 1;
        }
        if (end <= 0) {
            end = 1;
        }
        inAttackWindow = start <= attackFrame && attackFrame <= end;
        attackWindowOpen = inAttackWindow;
        isFinalType = type == 3 || type == 6 || type == 11;
        if ((stateFlags & 0x2000000) != 0) {
            inMotionWindow = false;
            attackWindowOpen = false;
        } else if (frame < start || end < frame) {
            inMotionWindow = false;
        } else if (self->finalStepPlaying != 0 && isFinalType) {
            inMotionWindow = true;
        } else {
            phase = self->attackPhase;
            inMotionWindow = phase == 2 || phase == 1;
        }

        switch (type) {
        case 1:
            if (inMotionWindow) {
                hit = BtlBakuganMotionHitCheck(self, ev);
            }
            break;
        case 2:
            if (attackWindowOpen) {
                hit = BtlBakuganMotionHitCheck(self, ev);
            }
            break;
        case 3:
            if (self->finalStepPlaying != 0 && (inMotionWindow || self->forceFinalHit != 0)) {
                hit = BtlBakuganMotionHitCheck(self, ev);
            }
            break;
        case 4:
            if (start <= self->attackEventFrame && self->attackEventFrame <= end) {
                hit = BtlBakuganMotionHitCheck(self, ev);
            }
            break;
        case 6:
            if (self->finalStepPlaying == 0) {
                break;
            }
            inAttackWindow = true;
            /* fall through */
        case 5:
            if (inAttackWindow && (self->state == 11 || self->state == 9 || self->state == 7)) {
                SndObjectAddEmitter(self->base.sound, ev->param.soundId, 0, 0);
            }
            break;
        case 7:
            if (start <= self->attackEventFrame && self->attackEventFrame <= end) {
                SndObjectAddEmitter(self->base.sound, ev->param.soundId, 0, 0);
            }
            break;
        case 8:
            if (!inAttackWindow) {
                break;
            }
            effectDir[0] = __builtin_cosf(self->base.rot[1]);
            effectDir[1] = 0.0f;
            effectDir[2] = __builtin_sinf(self->base.rot[1]);
            effectDir[3] = 0.0f;
            effectId = ev->param.ref.id;
            if (effectId == 0x8e) {
                entry = &((const VtblEntry *)self->base.base.vtable)[20];
                effectId += ((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta);
            }
            bone = ev->param.ref.bone;
            mgr = g_worldEffectMgr;
            if (bone == 0x7f) {
                effect = (GfxEffect *)GfxEffectSpawnAttachedDir(mgr, effectId, self->anchorMatrix[3],
                                                                effectDir);
            } else {
                GfxModelGetNodeWorldPosByIndex(&self->base, &nodePos, bone);
                effect = GfxEffectSpawnDirected(mgr, effectId, &nodePos.x, effectDir);
            }
            effect->ownerBakugan = self;
            if (self != NULL) {
                effect->ownerId = self->base.base.id;
            }
            break;
        case 9:
            if (inAttackWindow) {
                entry = &((const VtblEntry *)self->base.base.vtable)[6];
                ((float (*)(float, void *))entry->fn)(ev->value, (u8 *)self + entry->delta);
            }
            break;
        case 11:
            if (self->finalStepPlaying == 0) {
                break;
            }
            attackWindowOpen = true;
            /* fall through */
        case 10:
            if (!attackWindowOpen || self->attackSpawnBlocked != 0) {
                break;
            }
            attackType = ev->param.ref.id;
            savedBlocked = 0;
            if (self->combat.status[6].active != 0 && attackType == 1) {
                attackType = 11;
            }
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            attack = MemAlloc(sizeof(BtlAttack), NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            if (attack != NULL) {
                BtlAttackCtor(attack, self, attackType);
            }
            if (BtlBakuganGetTarget(self) != NULL) {
                BtlAttackLaunchAuto(attack, NULL, NULL, (s16 *)ev);
            } else {
                attackDir[0] = __builtin_cosf(self->base.rot[1]);
                attackDir[1] = 0.0f;
                attackDir[2] = __builtin_sinf(self->base.rot[1]);
                attackDir[3] = 0.0f;
                BtlAttackLaunchAuto(attack, NULL, attackDir, (s16 *)ev);
            }
            break;
        case 12:
            if (start <= self->attackEventFrame && self->attackEventFrame <= end) {
                self->dashSpeed = ev->value;
            }
            break;
        default:
            break;
        }
    }
    self->attackSpawnBlocked = savedBlocked;
    return hit;
}
