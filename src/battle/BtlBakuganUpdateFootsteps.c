// bdc 0x088680b8 BtlBakuganUpdateFootsteps
#include "bdc.h"

/* Per-frame footstep cues of a Bakugan (battle counterpart of `ActorUpdateFootsteps`), driven by
   the motion progress (`GfxModelGetMotionProgress`) and the step counter `footstep`:
   - kind 15 in states 0, 1 and 13: step sound (kind sound slot 7, `BtlBakuganPlayKindSound`) once
     the progress reaches 0.35, re-armed below it;
   - state 13 with stat `stepSoundMode` 1: kind 1 steps at 0.3 / 0.63 / 0.93, kinds 3 and 16 at
     0.4 / 0.86 (the counter re-arms below the first point); other kinds and states > 2 stop here;
   - states 1 and 2 (state 2 only for `moveStyle` 2 units that are not airborne,
     `BtlBakuganIsAirborne`): kind 10 steps on fixed frames of its walk motions
     (`GfxModelGetMotionFrameInt`, motion 4 or the other one, `BtlBakuganIsMotion`); the other
     kinds step whenever the progress crosses 0.4 (even counter) or 0.9 (odd counter), flipping bit 0
     of `footstep` (kind 14 runs motion 1 at double cadence). A step plays floor sound
     `base + CoreRandNext(4)` (base 12, 16 on floor material 3, 20 in stage water,
     `BtlBakuganIsInStageWater`) unless the kind is silent (15, 21, 22, 24, 26..32), then spawns
     effect 3 at the foot on `g_worldEffectMgr` (kind 10, `BtlBakuganGetFootPosition` at ground
     height) or, for non-hovering units, a footstep effect (`BtlBakuganSpawnFootstepEffect`, foot 2
    on odd steps, 3 on even); the foot vector is copied (one quad) into a stack vector whose Y is
    replaced by `groundY`. */

void BtlBakuganUpdateFootsteps(BtlBakugan *self)
{
    float progress;
    int state;
    int emit;
    s32 frame;
    int foot;
    int variant;
    int soundBase;
    int material;
    float pos[4];
    float *footPos;

    progress = GfxModelGetMotionProgress(&self->base);
    if (self->base.base.unk08 == 15) {
        state = self->state;
        if ((state < 2 && state >= 0) || state == 13) {
            if (progress < 0.35f) {
                self->footstep = 0;
            } else if (self->footstep == 0) {
                BtlBakuganPlayKindSound(self, 7, 0, 0);
                self->footstep++;
            }
        }
    }

    state = self->state;
    if (state >= 3) {
        if (state != 13) {
            return;
        }
        /* low nibble of the stat byte, sign-extended */
        if ((s8)(self->combat.stats->stepSoundMode << 4) >> 4 != 1) {
            return;
        }
        if (self->base.base.unk08 == 1) {
            if (progress < 0.3f) {
                self->footstep = 0;
            } else if (progress < 0.63f) {
                if (self->footstep == 0) {
                    BtlBakuganPlayKindSound(self, 7, 0, 0);
                    self->footstep++;
                }
            } else if (progress < 0.93f) {
                if (self->footstep == 1) {
                    BtlBakuganPlayKindSound(self, 7, 0, 0);
                    self->footstep++;
                }
            } else if (self->footstep != 0) {
                BtlBakuganPlayKindSound(self, 7, 0, 0);
                self->footstep = 0;
            }
            return;
        }
        if (self->base.base.unk08 != 3 && self->base.base.unk08 != 16) {
            return;
        }
        if (progress < 0.4f) {
            self->footstep = 0;
        } else if (progress < 0.86f) {
            if (self->footstep == 0) {
                BtlBakuganPlayKindSound(self, 7, 0, 0);
                self->footstep++;
            }
        } else if (self->footstep != 0) {
            BtlBakuganPlayKindSound(self, 7, 0, 0);
            self->footstep = 0;
        }
        return;
    }
    if (state <= 0) {
        return;
    }
    if (state >= 2) {
        if (self->combat.stats->moveStyle != 2) {
            return;
        }
        if (BtlBakuganIsAirborne(self, 1) != 0) {
            return;
        }
    }

    emit = 0;
    if (self->base.base.unk08 == 10) {
        frame = GfxModelGetMotionFrameInt(&self->base);
        if (BtlBakuganIsMotion(self, 4) != 0) {
            switch (frame) {
            case 5:
                emit = 1;
                foot = 3;
                variant = 1;
                break;
            case 9: /* foot chosen, no step */
                emit = 0;
                foot = 2;
                variant = 1;
                break;
            case 13:
                emit = 1;
                foot = 3;
                variant = 0;
                break;
            case 17: /* foot chosen, no step */
                emit = 0;
                foot = 2;
                variant = 0;
                break;
            default:
                break;
            }
        } else {
            switch (frame) {
            case 5:
                emit = 1;
                foot = 2;
                variant = 1;
                break;
            case 12: /* foot chosen, no step */
                emit = 0;
                foot = 3;
                variant = 0;
                break;
            case 19:
                emit = 1;
                foot = 3;
                variant = 1;
                break;
            case 28: /* foot chosen, no step */
                emit = 0;
                foot = 2;
                variant = 0;
                break;
            default:
                break;
            }
        }
    } else {
        if (self->base.base.unk08 == 14 && BtlBakuganIsMotion(self, 1) != 0) {
            progress = progress * 2.0f;
            if (!(progress <= 1.0f)) {
                progress = progress - 1.0f;
            }
        }
        if ((self->footstep & 1) == 0) {
            if (!(progress <= 0.4f) && progress < 0.9f) {
                emit = 1;
                self->footstep ^= 1;
            }
        } else if (!(progress <= 0.9f)) {
            emit = 1;
            self->footstep ^= 1;
        }
    }
    if (emit == 0) {
        return;
    }

    material = self->floorMaterial;
    if (material < 4) {
        if (material < 3) {
            soundBase = 12;
        } else {
            soundBase = 16;
        }
    } else {
        soundBase = 12;
        if (material < 5 && BtlBakuganIsInStageWater(self) != 0) {
            soundBase = 20;
        }
    }

    switch (self->base.base.unk08) {
    case 15:
    case 21:
    case 22:
    case 24:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
        break;
    default:
        BtlBakuganPlayKindSound(self, soundBase + CoreRandNext(4), 0, 0);
        break;
    }

    if (self->base.base.unk08 == 10) {
        footPos = BtlBakuganGetFootPosition(self, foot, variant);
        pos[0] = footPos[0];
        pos[1] = footPos[1];
        pos[2] = footPos[2];
        pos[3] = footPos[3];
        pos[1] = self->groundY;
        GfxEffectSpawn(g_worldEffectMgr, 3, pos);
    } else if (self->combat.stats->hoverHeight == 0.0f) {
        BtlBakuganSpawnFootstepEffect(0.0f, 0.0f, self, (self->footstep & 1) != 0 ? 2 : 3);
    }
}
