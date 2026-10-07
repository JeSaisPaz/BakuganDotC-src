// bdc 0x08868bb4 BtlBakuganOnHit
#include "bdc.h"

/* Hit-reaction virtual of the battle Bakugan (vtable slot `+0xc0`, called from the unit update when
   its body collider registered a hit). Reads the attack id (`collider->hitKind`), attacker
   (`hitAttacker`), hit class (`hitParam164`: 1 melee, 2..3 ability) and heading from the collider;
   while the all-units control lock is set it only clears the collider and the gauges. Otherwise it
   computes the flinch/knock-down/energy amounts (`BtlBakuganGetBasicHitParams` for melee hits,
   `BtlAttackParamsGetHitCount`..`BtlAttackParamsGetHitEnergy` for abilities, scaled by
   `stats->damageScale`), saves the velocity and sets a 40-unit knock-back along the hit heading,
   handles guarding (state flag 0x200000: `BtlBakuganApplyStateEnergy`, a guard spark or a block
   through vtable `+0xc8`), applies the ability status/hit effects, the KO / low-HP statistics,
   counter windows, `BtlCombatTakeHit`, battle-rule score words, and finally switches the unit
   into the reaction state picked by the low nibble of the reaction code (`BtlBakuganSetState`
   3/4/5) with the matching motion, shake, sound and knock-back; reaction 0xb (absorbed flinch)
   restores the saved velocity and returns early. Every path that reaches the reaction switch ends
   with a motion update and vtable `+0xb8` (`BtlBakuganUpdateBoneAnchors`).
   The VFPU code is lifted: the bank constants S703 (2/pi, folded into cosf/sinf of the heading) and
   S713 (0: the zero-length guard-direction fallback and the `w` the `sv.q C710` writes store) are
   literals. */

void BtlBakuganOnHit(BtlBakugan *self)
{
    float savedVel[4];
    float guardDir[4];
    union { float f; u32 u; } distBits;
    float lenSq;
    float invLen;
    float delta[3];
    CollisionCollider *collider;
    BtlBakugan *rawAttacker;
    BtlBakugan *attacker;
    GfxEffect *effect;
    const VtblEntry *entry;
    SaveProfile *profile;
    float *vel;
    s16 *params;
    s32 attackId;
    s32 hitClass;
    s32 hitKind;
    s32 abilityIndex;
    s32 meleeStage;
    s32 flinch;
    s32 knockdown;
    s32 energy;
    s32 hitCount;
    s32 inFront;
    s32 motion;
    s32 fallMotion;
    s32 reaction;
    s32 kind;
    s32 hitEffect;
    s32 statusEffect;
    s32 add;
    s32 mult;
    s32 slot;
    s32 blocked;
    s32 bitIndex;
    u32 word;
    u32 unitKind;
    float heading;
    float dir;
    float back;
    float diff;
    float turn;
    float hpRatio;
    float scale;
    u8 isBasic;
    u8 isAbility;
    u8 lockBroken;
    u8 guardable;
    u8 heavyStun;
    u8 heavy;
    u8 playVoice;

    attackId = self->collider0->hitKind;
    self->stateFlags = self->stateFlags | 0x20;
    if (g_btlControlLockAll != 0) {
        collider = self->collider0;
        collider->flags = collider->flags & ~1u;
        collider->hitTimer = 0;
        self->knockdownGauge = 0;
        self->flinchGauge = 0;
        return;
    }

    self->airRecoverDelay = 7;
    self->tiltStep = 0.0f;
    energy = 0;
    inFront = 1;
    motion = 0xea;
    heading = self->collider0->hitHeading;
    dir = heading;
    hitClass = self->collider0->hitParam164;
    lockBroken = 0;
    meleeStage = BtlBakuganGetMeleeHitStage(self, attackId);
    isBasic = attackId < 0x23;
    vel = self->base.velocity;
    if (meleeStage == 2) {
        dir = heading + 1.25663710f;
    } else if (meleeStage == 2) {
        /* dead: the original tests stage 2 a second time */
        dir = heading - 1.25663710f;
    }
    if (!(dir <= 3.14159274f)) {
        dir = dir - 6.28318548f;
    } else if (dir <= -3.14159274f) {
        dir = dir + 6.28318548f;
    }

    flinch = 0;
    knockdown = 0;
    isAbility = 0;
    hitCount = 1;
    abilityIndex = -1;
    if (!isBasic && attackId < 0xb3) {
        abilityIndex = attackId - 0x23;
        if (abilityIndex < 0) {
            abilityIndex = 0;
        } else if (0x8d < abilityIndex) {
            abilityIndex = 0x8d;
        }
    }

    collider = self->collider0;
    rawAttacker = (BtlBakugan *)collider->hitAttacker;
    hitKind = collider->hitParam164;
    if (hitKind == 1) {
        /* melee hit */
        BtlBakuganClassifyHitDirection(dir, self);
        inFront = BtlBakuganIsObjectInFront(self, rawAttacker);
        params = BtlBakuganGetBasicHitParams(rawAttacker, attackId);
        if (params != NULL) {
            flinch = params[3];
            knockdown = params[2];
            energy = (s32)((float)params[4] * self->combat.stats->damageScale);
        } else {
            flinch = 100;
            knockdown = 0;
        }
        if (attackId == 0xb9 || attackId == 0xb3 || attackId == 0x93 || attackId == 0x86 ||
            attackId == 0x53 || attackId == 0x48) {
            if (attackId == 0xb3) {
                self->knockdownGauge = 0;
                self->flags = self->flags | 0x800;
            }
            flinch = flinch + 100;
            knockdown = 0;
        }
    } else if (hitKind >= 2 && hitKind < 4) {
        /* ability hit */
        BtlBakuganClassifyHitDirection(dir, self);
        hitCount = BtlAttackParamsGetHitCount(abilityIndex);
        flinch = BtlAttackParamsGetFlinchPower(abilityIndex) * hitCount;
        knockdown = BtlAttackParamsGetKnockdownPower(abilityIndex) * hitCount;
        energy = (s32)((float)BtlAttackParamsGetHitEnergy(abilityIndex) *
                       self->combat.stats->damageScale * (float)hitCount);
        isAbility = 1;
    } else {
        /* no attacker unit: in front unless the hit came from more than 135 degrees off */
        rawAttacker = NULL;
        back = dir + 3.14159274f;
        if (!(back <= 3.14159274f)) {
            back = back - 6.28318548f;
        } else if (back <= -3.14159274f) {
            back = back + 6.28318548f;
        }
        diff = self->base.rot[1] - back;
        diff = diff - (float)(s32)(diff * 0.318309873f) * 6.28318548f;
        if (diff < 0.0f) {
            diff = diff + 6.28318548f;
        }
        if (diff < 3.14159274f) {
            turn = -diff;
        } else {
            turn = 6.28318548f - diff;
        }
        if (!(__builtin_fabsf(turn) <= 2.35619450f)) {
            inFront = 0;
        }
        BtlBakuganClassifyHitDirection(dir, self);
    }

    if ((self->stateFlags & 0x200000) == 0) {
        self->flinchGauge = self->flinchGauge + flinch;
        self->knockdownGauge = self->knockdownGauge + knockdown;
    }
    attacker = BtlBakuganListFind(rawAttacker);
    if (self->lockedAttacker != NULL && self->lockedAttacker != attacker) {
        lockBroken = 1;
    }
    if (self->lockedAttacker == NULL) {
        self->flags = self->flags & ~0x800u;
    }
    if (attacker != NULL) {
        attacker->attackResult = 2;
    }
    reaction = 1;

    /* save the velocity, then velocity = 40 * (cos heading, 0, sin heading, 0) (vrot.q [C,0,S,0] of
       heading * 2/pi, vscl.t by 40: lane 3 keeps the vrot zero) */
    savedVel[0] = vel[0];
    savedVel[1] = vel[1];
    savedVel[2] = vel[2];
    savedVel[3] = vel[3];
    vel[0] = __builtin_cosf(heading) * 40.0f;
    vel[1] = 0.0f * 40.0f;
    vel[2] = __builtin_sinf(heading) * 40.0f;
    vel[3] = 0.0f;

    guardable = (u8)BtlBakuganIsGuardableHit(self, attackId);
    if ((self->stateFlags & 0x200000) != 0) {
        /* guarding */
        BtlBakuganApplyStateEnergy(self, 6, energy);
        if (attacker != NULL) {
            guardDir[0] = attacker->base.pos[0];
            guardDir[1] = attacker->base.pos[1];
            guardDir[2] = attacker->base.pos[2];
            guardDir[3] = attacker->base.pos[3];
        } else {
            collider = self->collider0;
            guardDir[0] = collider->hitPos.x;
            guardDir[1] = collider->hitPos.y;
            guardDir[2] = collider->hitPos.z;
            guardDir[3] = collider->hitPos.w;
        }
        guardDir[0] = guardDir[0] - self->guardAnchor[0];
        guardDir[1] = guardDir[1] - self->guardAnchor[1];
        guardDir[2] = guardDir[2] - self->guardAnchor[2];
        guardDir[1] = 0.0f;
        /* normalise x/z (inverse length 0 for a zero vector), clamped to [-1, 1]; w is S713 (0) */
        lenSq = guardDir[0] * guardDir[0] + guardDir[1] * guardDir[1] + guardDir[2] * guardDir[2];
        invLen = VfRsq(lenSq);
        if (lenSq == 0.0f) {
            invLen = 0.0f;
        }
        guardDir[0] = VfSat1(guardDir[0] * invLen);
        guardDir[1] = VfSat1(guardDir[1] * invLen);
        guardDir[2] = VfSat1(guardDir[2] * invLen);
        guardDir[3] = 0.0f;
        if (BtlCombatHasEnergy(&self->combat) == 0 || guardable == 0) {
            GfxEffectSpawnAttachedDir(g_worldEffectMgr, 0x39, self->guardAnchor, guardDir);
            reaction = 7;
        } else {
            blocked = 0;
            entry = &((const VtblEntry *)self->base.base.vtable)[25]; /* +0xc8 BtlBakuganTryBlock */
            if (((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta) != 0) {
                GfxModelUpdateMotion(&self->base);
                GfxModelApplyMotion(&self->base);
                entry = &((const VtblEntry *)self->base.base.vtable)[23]; /* +0xb8 BtlBakuganUpdateBoneAnchors */
                ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
                blocked = 1;
            }
            self->hitQueueCount = 0;
            if (blocked != 0) {
                return;
            }
            if (self->base.base.unk08 != 0x15) {
                return;
            }
            reaction = 7;
        }
    }

    if (abilityIndex != -1 && BtlIsItem0FBoostAttackId(&self->combat, attackId) == 0 &&
        (self->flags & 0x10) != 0 && BtlAttackTypeGetCategory(abilityIndex) != 0xc) {
        self->collider0->cooldown = 0;
        collider = self->collider0;
        collider->flags = collider->flags | 1;
        collider->hitTimer = 2;
        return;
    }
    if (!(self->knockdownGauge < 100)) {
        self->hitQueueCount = 0;
    }

    if (attacker != NULL && isAbility == 0 && attacker->slowMotionActive != 0 && attacker->stats != NULL) {
        hpRatio = BtlCombatGetHpRatio(&self->combat);
        if (hpRatio <= 0.6f && !(self->lastHpRatio <= 0.6f) &&
            (entry = &((const VtblEntry *)self->base.base.vtable)[10], /* +0x50 BtlBakuganIsBakugan */
             ((s32 (*)(void *))entry->fn)((u8 *)self + entry->delta) != 0)) {
            BtlStatsAddCounter(attacker->stats, 0x12, 1);
        } else if (hpRatio <= 0.1f && !(self->lastHpRatio <= 0.1f)) {
            BtlStatsAddCounter(attacker->stats, 0x13, 1);
        }
        self->lastHpRatio = hpRatio;
    }

    heavyStun = 0;
    if (reaction == 7 && self->combat.dead != 0) {
        reaction = 1;
    }
    if (reaction != 7) {
        reaction = BtlBakuganGetHitReaction(self, attackId);
    }
    if (abilityIndex != -1) {
        hitEffect = BtlAttackParamsGetHitEffect(abilityIndex);
        statusEffect = BtlAttackParamsGetStatusEffect(abilityIndex);
        if (statusEffect == 1) {
            BtlBakuganEndStatus(self, 7);
            BtlCombatApplyStatus(&self->combat, 1, 300);
            GfxEffectSpawnAttached(g_worldEffectMgr, 0x53, self->base.pos);
        } else if (statusEffect == 2) {
            BtlBakuganEndStatus(self, 7);
            BtlCombatApplyStatus(&self->combat, 3, 300);
            GfxEffectSpawnAttached(g_worldEffectMgr, 0x55, self->base.pos);
        } else if (statusEffect == 3) {
            BtlBakuganEndStatus(self, 7);
            BtlCombatApplyStatus(&self->combat, 5, 300);
            GfxEffectSpawnAttached(g_worldEffectMgr, 0x54, self->base.pos);
        }
        switch (hitEffect) { /* jump table 0x08a67c10 on hitEffect - 1 */
        case 2:
        case 5:
            reaction = 4;
            break;
        case 3:
            reaction = 4;
            heavyStun = 1;
            break;
        case 4:
            BtlBakuganEndStatus(self, 2);
            BtlCombatApplyStatus(&self->combat, 0x12, 600);
            GfxEffectSpawnAttached(g_worldEffectMgr, 0x46, self->anchorMatrix[3]);
            self->status12Source = attacker;
            self->flinchGauge = 100;
            break;
        case 6:
            BtlBakuganEndStatus(self, 3);
            BtlCombatApplyStatus(&self->combat, 0x13, 0x78);
            GfxEffectSpawnAttached(g_worldEffectMgr, 0x56, self->base.pos);
            self->flinchGauge = 100;
            if (BtlBakuganIsLocalPlayer(self)) {
                BtlBakuganPlaySound(self, 0x2001f7, 0, 0);
            }
            break;
        case 7:
            reaction = 2;
            break;
        default: /* 1 and out of range: no change */
            break;
        }
    }
    if ((reaction & 0xf) == 1) {
        if ((self->stateFlags & 0x100000) != 0) {
            self->flinchGauge = 100;
        }
        if (self->flinchGauge < 100) {
            reaction = 0xb;
        } else {
            self->flinchGauge = 0;
        }
    }

    self->hitCount = self->hitCount + 1;
    self->hitStunTimer = 40;
    BtlBakuganSpawnHitSpark(self, reaction, attacker, attackId);
    if (lockBroken) {
        collider = self->collider0;
        collider->flags = collider->flags & ~1u;
        collider->hitTimer = 0;
        self->knockdownGauge = 0;
        self->flinchGauge = 0;
        return;
    }
    if (self->slowMotionActive != 0) {
        BtlSlowMotionTaskRemove();
        self->slowMotionActive = 0;
    }
    if (lockBroken) { /* always 0 here: the original re-reads its stack copy */
        reaction = 1;
    }
    kind = reaction & 0xf;
    if (self->collider0->hitParam164 == 1 || self->collider0->hitParam164 == 2) {
        self->meleeHitAttacker = attacker;
    } else {
        self->meleeHitAttacker = NULL;
    }
    if ((self->flags & 0x400) != 0) {
        BtlGetCameraTask();
        BtlEndCutIn(); /* the camera task is left in a0 but not read */
        self->flags = self->flags & ~0x400u;
    }
    if (self->isPlayer != 0) {
        GfxSetMotionTimeScale(1.0f);
        ((BtlMain *)BtlGetCameraTask())->flashTarget = 0.0f;
    }

    /* counter window */
    if (kind == 1 && isAbility == 0 && guardable != 0) {
        if (self->collider0->hitParam164 == 1) {
            if (self->attacker == attacker) {
                if ((self->flags & 0x80) != 0) {
                    BtlBakuganStartCounter(self, (self->flags & 0x100) != 0);
                    GfxModelUpdateMotion(&self->base);
                    GfxModelApplyMotion(&self->base);
                    entry = &((const VtblEntry *)self->base.base.vtable)[23]; /* +0xb8 BtlBakuganUpdateBoneAnchors */
                    ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
                    return;
                }
                self->counterWindow = self->counterWindow + 1;
            } else if (self->attacker != NULL) {
                self->counterWindow = 0;
            } else {
                self->counterWindow = self->counterWindow + 1;
            }
            self->attacker = attacker;
        }
    } else {
        self->attacker = NULL;
        self->counterWindow = 0;
    }

    BtlBakuganApplyQueuedHits(self, &flinch, &knockdown);
    if (!lockBroken) {
        BtlCombatTakeHit(&self->combat, attacker, hitClass, attackId, attackId == 0x86, 0, 6);
    }

    /* battle rule 2: per-player score words */
    if (attacker != NULL) {
        hitKind = self->collider0->hitParam164;
        if (hitKind == 1) {
            if (g_scriptGlobalVars[8] == 2 && (self->stateFlags & 0x200000) == 0 &&
                (self->flags & 0x80) == 0) {
                add = 1;
                mult = 1;
                if (BtlIsTimeRunningOut()) {
                    add = 2;
                    mult = 2;
                }
                word = SaveProfileGetWord(SaveGetProfile(), 7);
                if ((s32)word >= 2 && (s32)word < 3) {
                    slot = attacker->playerSlot + 0xe;
                    if (attackId != 0xb9) {
                        if (attacker->state == 9 || attacker->state == 0xb ||
                            BtlAttackIdUsesHitCounter(self, attackId) != 0) {
                            if (attacker->comboDamage < mult * 5) {
                                attacker->comboDamage = attacker->comboDamage + add;
                            }
                        } else if (BtlCameraTaskExists() != 0) {
                            BtlGetCameraTask();
                            if (SaveProfileGetWord(SaveGetProfile(), 2) != 0) {
                                SaveProfileAddWord(SaveGetProfile(), slot, add);
                            }
                        }
                    }
                }
            }
        } else if (hitKind >= 2 && hitKind < 4) {
            if (g_scriptGlobalVars[8] == 2 && (self->stateFlags & 0x200000) == 0 &&
                (self->flags & 0x80) == 0) {
                add = hitCount;
                mult = 1;
                if (BtlIsTimeRunningOut()) {
                    add = hitCount + hitCount;
                    mult = 2;
                }
                word = SaveProfileGetWord(SaveGetProfile(), 7);
                if ((s32)word >= 2 && (s32)word < 3) {
                    slot = attacker->playerSlot + 0xe;
                    if ((attacker->stateFlags & 0x80000) != 0 ||
                        BtlAttackIdUsesHitCounter(self, attackId) != 0) {
                        if (attacker->hitTally < mult * 5) {
                            BtlBakuganAddClampedHitCount(attacker, add, mult);
                        }
                    } else if (BtlCameraTaskExists() != 0) {
                        BtlGetCameraTask();
                        if (SaveProfileGetWord(SaveGetProfile(), 2) != 0) {
                            SaveProfileAddWord(SaveGetProfile(), slot, add);
                        }
                    }
                }
            }
        }
    }

    if (BtlIsAttackIdB3ToB7(attackId) == 0 && self->combat.dead == 0 && self->combat.status[8].active != 0) {
        if (self->combat.status[8].active == 0) { /* re-tested by the original */
            return;
        }
        BtlBakuganPlaySound(self, (s32)(CoreRandNext(4) + 0x200073), 0, 0);
        return;
    }
    if (kind == 7 || kind == 4 || kind == 1) {
        if (self->combat.dead != 0) {
            reaction = 3;
        } else if (!(self->knockdownGauge < 100)) {
            reaction = 2;
            self->knockdownGauge = 0;
            self->flinchGauge = 0;
        }
    }

    if (attacker != NULL) {
        if (self->combat.dead != 0) {
            /* knocked out */
            if (reaction == 0xb || reaction == 1) {
                reaction = 2;
            }
            attacker->knockoutCount = attacker->knockoutCount + 1;
            attacker->knockoutTally = attacker->knockoutTally + 1;
            BtlAttackEndOwnedSustained(self);
            if ((self->flags & 0x10) != 0) {
                self->flags = self->flags & ~0x10u;
            }
            BtlBakuganStopAllStatusEffects(self);
            BtlCombatClearTimedStatuses(&self->combat);
            if (self->knockOutMode == 2) {
                if (BtlCameraTaskExists() != 0) {
                    BtlCameraFocusUnit(600.0f, 100.0f, 40.0f, BtlGetCameraTask(), self, 0, 0, NULL);
                }
                attacker->stage5HintFlag = 1;
            }
            if (g_scriptGlobalVars[8] == 2) {
                mult = 1;
                if (BtlIsTimeRunningOut()) {
                    mult = 2;
                }
                slot = attacker->playerSlot + 0xe;
                word = SaveProfileGetWord(SaveGetProfile(), 7);
                if ((s32)word < 2) {
                    if (0 < (s32)word) {
                        add = mult * 10;
                        if (BtlCameraTaskExists() != 0) {
                            BtlGetCameraTask();
                            if (SaveProfileGetWord(SaveGetProfile(), 2) != 0) {
                                SaveProfileAddWord(SaveGetProfile(), slot, add);
                            }
                        }
                    }
                } else if ((s32)word < 3) {
                    add = mult * 20;
                    if (BtlCameraTaskExists() != 0) {
                        BtlGetCameraTask();
                        if (SaveProfileGetWord(SaveGetProfile(), 2) != 0) {
                            SaveProfileAddWord(SaveGetProfile(), slot, add);
                        }
                    }
                }
            } else if (self->isPlayer == 0) {
                /* CPU unit of kind 9, 0xb..0xf or 0x15..0x20 knocked out: profile bit */
                unitKind = self->base.base.unk08;
                if ((unitKind >= 0x15 && unitKind < 0x21) || (unitKind >= 0xb && unitKind < 0x10) ||
                    unitKind == 9) {
                    profile = SaveGetProfile();
                    bitIndex = (s32)self->base.base.unk08;
                    profile->data->bakuganBitsB[bitIndex / 8] =
                        profile->data->bakuganBitsB[bitIndex / 8] | (u8)(1 << (bitIndex % 8));
                }
            }
        } else if (BtlBakuganGetTarget(self) == NULL) {
            entry = &((const VtblEntry *)attacker->base.base.vtable)[17]; /* +0x88 BtlBakuganIsUntargetable */
            if (((s32 (*)(void *))entry->fn)((u8 *)attacker + entry->delta) == 0 && self->adviceFlag != 0) {
                self->targetId = attacker->base.base.id;
            }
        }
        if (attackId != 0xb9) {
            if (self->base.base.unk08 < 0x21) {
                BtlBakuganAddComboHits(attacker, hitCount);
            }
            attacker->linkedUnit = self;
        }
    }

    kind = reaction & 0xf;
    if (attackId != 0x8b && (self->stateFlags & 0x100000) != 0) {
        BtlBakuganEndStatus(self, 1);
        if (kind == 4) {
            reaction = 2;
            kind = 2;
        }
    }
    playVoice = 1;
    if (!isBasic && attackId < 0xb3) {
        playVoice = 0;
    }
    if (kind != 0xb && (self->stateFlags & 0x100) != 0) {
        self->stateFlags = self->stateFlags & ~0x100u;
    }
    if (attacker != NULL && attacker->status12Source == self) {
        BtlCombatClearStatus(&attacker->combat, 0x12);
    }
    if (self->combat.status[9].active != 0) {
        BtlCombatClearStatus(&self->combat, 9);
        GfxEffectSetStateAttached(g_worldEffectMgr, 0x49, self->anchorMatrix[3], 2);
    }
    if (BtlIsAttackIdB3ToB7(attackId) != 0) {
        BtlBakuganSetTarget(self, attacker);
    }
    self->flags = self->flags & ~2u;
    self->stateFlags = self->stateFlags & ~0x100000u;

    switch (kind) { /* jump table 0x08a67c30 on kind - 1 */
    case 1:
    case 4:
        /* flinch (state 3) */
        if (attackId == 0xb3) {
            collider = self->collider0;
            collider->flags = collider->flags | 1;
            collider->hitTimer = 1;
            self->lockedAttacker = self->meleeHitAttacker;
            self->flags = self->flags | 0x800;
            /* knock-back scaled by 0.0034 * (300 - distance to the attacker), 0 when that is not
               above 0 (bgtz on the float bits); w is S713 (0) */
            delta[0] = self->base.pos[0] - attacker->base.pos[0];
            delta[1] = self->base.pos[1] - attacker->base.pos[1];
            delta[2] = self->base.pos[2] - attacker->base.pos[2];
            distBits.f = 300.0f - __builtin_sqrtf(delta[0] * delta[0] + delta[1] * delta[1] +
                                                  delta[2] * delta[2]);
            if ((s32)distBits.u > 0) {
                scale = distBits.f * 0.00340000005f;
            } else {
                scale = 0.0f * 0.00340000005f;
            }
            vel[0] = vel[0] * scale;
            vel[1] = vel[1] * scale;
            vel[2] = vel[2] * scale;
            vel[3] = 0.0f;
        }
        BtlBakuganStartHitShake(0.2f, self, vel);
        if (playVoice) {
            BtlBakuganPlaySound(self, (s32)(CoreRandNext(4) + 0x200073), 0, 0);
        }
        BtlBakuganSetState(self, 3, 0);
        if (inFront > 0) {
            if (inFront < 2) {
                motion = 0xed;
            }
        } else if (inFront >= 0) {
            motion = 0xea;
        }
        if (BtlBakuganIsAirborne(self, 1) != 0) {
            motion = motion + 1;
        }
        BtlBakuganPlayMotion(0.0f, self, motion, 0, 1);
        self->gravityHold = 0x10;
        if (attackId == 0xb9) {
            entry = &((const VtblEntry *)self->base.base.vtable)[6]; /* +0x30 BtlBakuganSetMotionSpeed */
            ((float (*)(float, void *))entry->fn)(0.7f, (u8 *)self + entry->delta);
        }
        if (reaction == 4 || reaction == 0x14) {
            if (BtlBakuganIsLocalPlayer(self)) {
                BtlBakuganPlaySound(self, 0x2001f5, 0, 0);
            }
            self->stateFlags = self->stateFlags | 0x100000;
            collider = self->collider0;
            collider->flags = collider->flags | 1;
            collider->hitTimer = 0x14;
            if (attackId == 0x1e || attackId == 0x47) {
                heavy = 1;
                self->subTimer = 0x2d;
            } else {
                heavy = heavyStun;
                self->subTimer = (attackId == 0x61) ? 0x46 : 0x78;
            }
            if (heavy) {
                effect = GfxEffectSpawnAttached(g_worldEffectMgr, 0x40, self->anchorMatrix[3]);
                effect->ownerBakugan = self;
                if (self != NULL) {
                    effect->ownerId = self->base.base.id;
                }
                self->flags = self->flags | 1;
            } else {
                effect = GfxEffectSpawnAttached(g_worldEffectMgr, 0x3e, self->anchorMatrix[3]);
                effect->ownerBakugan = self;
                if (self != NULL) {
                    effect->ownerId = self->base.base.id;
                }
                self->flags = self->flags & ~1u;
            }
        }
        if (attackId == 0x46) {
            effect = GfxEffectSpawnAttached(g_worldEffectMgr, 0x40, self->anchorMatrix[3]);
            effect->ownerBakugan = self;
            if (self != NULL) {
                effect->ownerId = self->base.base.id;
            }
        }
        if (attackId == 0x20) {
            entry = &((const VtblEntry *)self->base.base.vtable)[6]; /* +0x30 BtlBakuganSetMotionSpeed */
            ((float (*)(float, void *))entry->fn)(0.5f, (u8 *)self + entry->delta);
            vel[0] = vel[0] * 2.0f;
            vel[1] = vel[1] * 2.0f;
            vel[2] = vel[2] * 2.0f;
            vel[3] = 0.0f;
        }
        break;

    case 2:
    case 5:
    case 6:
    case 8:
    case 9:
        /* knock-back (state 4) */
        self->flags = self->flags & ~0x800u;
        self->gravityHold = 0;
        if (reaction == 8) {
            BtlBakuganStartHitShake(0.05f, self, vel);
            self->stateFlags = self->stateFlags & ~0x20u;
            self->lockedAttacker = NULL;
        } else {
            BtlBakuganStartHitShake(0.125f, self, vel);
        }
        if (playVoice) {
            if (kind == 8) {
                BtlBakuganPlaySound(self, 0x20008a, 0, 0);
            } else {
                BtlBakuganPlaySound(self, 0x200077, 0, 0);
            }
        }
        BtlBakuganSetState(self, 4, 0);
        if (inFront > 0) {
            if (inFront < 2) {
                motion = 0xf3;
                heading = heading + 3.14159274f;
                if (!(heading <= 3.14159274f)) {
                    heading = heading - 6.28318548f;
                } else if (heading <= -3.14159274f) {
                    heading = heading + 6.28318548f;
                }
                BtlBakuganSetHeading(self, heading);
                self->motionIdPair[1] = g_btlBakuganTiltFlipMotionIds[1];
                self->motionIdPair[0] = g_btlBakuganTiltFlipMotionIds[0];
            }
        } else if (inFront >= 0) {
            BtlBakuganSetHeading(self, heading);
            self->motionIdPair[1] = g_btlBakuganTiltFrontMotionIds[1];
            self->motionIdPair[0] = g_btlBakuganTiltFrontMotionIds[0];
            motion = 0xf2;
        }
        self->stateFlags = self->stateFlags | (u32)self->motionIdPair[0];
        vel[0] = vel[0] * 1.20000005f;
        vel[1] = vel[1] * 1.20000005f;
        vel[2] = vel[2] * 1.20000005f;
        vel[3] = 0.0f;
        self->base.velocity[1] = 34.0f;
        BtlBakuganPlayMotion(0.0f, self, motion, 1, 1);
        if (kind == 9) {
            self->base.velocity[1] = 0.0f;
            self->subTimer = -1;
            self->stateFlags = self->stateFlags & ~0x20u;
        } else if (kind == 5 || attackId == 0xb6) {
            vel[0] = vel[0] * 0.180000007f;
            vel[2] = vel[2] * 0.180000007f;
            if (attackId == 0x86) {
                self->base.velocity[1] = 10.0f;
                self->stateFlags = self->stateFlags & ~0x20u;
                vel[0] = vel[0] * 0.100000001f;
                vel[2] = vel[2] * 0.100000001f;
            } else {
                self->base.velocity[1] = 60.0f;
            }
            self->flags = self->flags | 2;
        } else if (kind == 6 || attackId == 0xb7) {
            if (BtlBakuganIsAirborne(self, 1) != 0) {
                vel[0] = vel[0] * 0.625f;
                vel[2] = vel[2] * 0.625f;
                self->base.velocity[1] = -10.0f;
            } else {
                vel[0] = vel[0] * 0.875f;
                vel[2] = vel[2] * 0.875f;
                self->base.velocity[1] = 1.0f;
            }
            self->stateFlags = self->stateFlags & ~0x20u;
        }
        if (attackId == 0x54) {
            BtlCombatApplyStatus(&self->combat, 0x12, 600);
            GfxEffectSpawnAttached(g_worldEffectMgr, 0x47, self->anchorMatrix[3]);
            self->status12Source = attacker;
        }
        self->knockdownGauge = 0;
        self->flinchGauge = 0;
        BtlBakuganResetCounterWindow(self);
        break;

    case 3:
    case 10:
        /* launched (state 5) */
        self->gravityHold = 0;
        BtlBakuganStartHitShake(0.1f, self, vel);
        BtlBakuganPlaySound(self, 0x200078, 0, 0);
        BtlBakuganSetState(self, 5, 0);
        fallMotion = 0xea;
        if (inFront > 0) {
            if (inFront < 2) {
                self->motionIdPair[1] = g_btlBakuganTiltFlipMotionIds[1];
                self->motionIdPair[0] = g_btlBakuganTiltFlipMotionIds[0];
                BtlBakuganSetHeading(self, heading + 3.14159274f);
                fallMotion = 0xed;
            }
        } else if (inFront >= 0) {
            self->motionIdPair[1] = g_btlBakuganTiltFrontMotionIds[1];
            self->motionIdPair[0] = g_btlBakuganTiltFrontMotionIds[0];
            BtlBakuganSetHeading(self, heading);
            fallMotion = 0xea;
        }
        self->stateFlags = self->stateFlags | (u32)self->motionIdPair[0];
        vel[0] = vel[0] * 1.5f;
        vel[1] = vel[1] * 1.5f;
        vel[2] = vel[2] * 1.5f;
        vel[3] = 0.0f;
        self->base.velocity[1] = 50.0f;
        if (BtlBakuganHasMotion(self, 0xf4) != 0) {
            BtlBakuganPlayMotion(0.0f, self, 0xf4, 1, 1);
        } else {
            BtlBakuganPlayMotion(0.0f, self, fallMotion, 1, 1);
        }
        entry = &((const VtblEntry *)self->base.base.vtable)[6]; /* +0x30 BtlBakuganSetMotionSpeed */
        ((float (*)(float, void *))entry->fn)(1.5f, (u8 *)self + entry->delta);
        self->knockdownGauge = 0;
        if (reaction == 10) {
            self->subTimer = -1;
        }
        BtlBakuganResetCounterWindow(self);
        self->flinchGauge = 0;
        break;

    case 7:
        /* guarded */
        BtlBakuganPlaySound(self, 0x200072, 0, 0);
        BtlBakuganSetState(self, 3, 0);
        BtlBakuganPlayMotion(0.0f, self, 0xea, 0, 1);
        break;

    case 0xb:
        /* flinch absorbed: restore the velocity */
        vel[0] = savedVel[0];
        vel[1] = savedVel[1];
        vel[2] = savedVel[2];
        vel[3] = savedVel[3];
        self->collider0->cooldown = 0;
        collider = self->collider0;
        collider->flags = collider->flags & ~1u;
        collider->hitTimer = 0;
        return;

    default:
        break;
    }

    GfxModelUpdateMotion(&self->base);
    GfxModelApplyMotion(&self->base);
    entry = &((const VtblEntry *)self->base.base.vtable)[23]; /* +0xb8 BtlBakuganUpdateBoneAnchors */
    ((void (*)(void *))entry->fn)((u8 *)self + entry->delta);
}
