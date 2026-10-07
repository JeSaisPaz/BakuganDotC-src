// bdc 0x08889554 BtlCombatUpdateStatuses
#include "bdc.h"

/* Per-frame tick of the 21 timed status slots of a `BtlCombatState` (`BtlBakuganTickCombat`),
   skipped while the owner's input is disabled or `BtlIsDemoRunning()` holds. For a local player
   unit the first active attribute status 0xb..0x10 of the frame heals instead of counting down:
   while HP is below max and `regenTimer` runs out (every 90 frames) it heals 3% of max HP (x1.5
   when status - 0xb equals the owner's virtual entry 20 result), spawns effect 0x5b attached to
   the anchor position and plays sound `0x200191` unless already playing. Every other active slot:
   status 0x12 deals 10 non-lethal damage every 30 frames (`status12Timer`; a local player also
   queries sound `0x200073`), status 0x14 deals 5 non-lethal damage (x1.5 for attribute 4, none for
   unit kind 0xf) every 15 frames (`status14Timer`); `remaining` counts down (-1 = permanent) and
   `BtlCombatClearStatus` runs when it reaches 0. */

void BtlCombatUpdateStatuses(BtlCombatState *combat)
{
    BtlBakugan *owner = combat->owner;
    BtlCombatStatusSlot *slot;
    const VtblEntry *entry;
    GfxEffect *effect;
    float scale;
    float hp;
    s16 remaining;
    bool healed;
    s32 id;

    if (owner == NULL || owner->input->disabled != 0 || BtlIsDemoRunning()) {
        return;
    }
    healed = false;
    for (id = 0; id < 0x15; id++) {
        slot = &combat->status[id];
        if (slot->active == 0) {
            continue;
        }
        if (BtlBakuganIsLocalPlayer(combat->owner) && id >= 0xb && id <= 0x10 && !healed) {
            if (BtlCombatGetHpRatio(combat) < 1.0f && --combat->regenTimer <= 0) {
                owner = combat->owner;
                scale = 1.0f;
                entry = &((const VtblEntry *)owner->base.base.vtable)[20];
                if (id - 0xb == ((s32 (*)(void *))entry->fn)((u8 *)owner + entry->delta)) {
                    scale = 1.5f;
                }
                owner = combat->owner;
                effect = (GfxEffect *)GfxEffectSpawnAttached(g_worldEffectMgr, 0x5b,
                                                             owner->anchorMatrix[3]);
                hp = BtlCombatGetHp(combat);
                BtlCombatSetHp(hp + (float)(u32)BtlCombatGetMaxHp(combat) * (scale * 0.03f),
                               combat);
                if (SndManagerIsSoundWordPlaying(SndGetManager(), 0x200191) == 0 &&
                    SndHasManager()) {
                    SndManagerPlay(SndGetManager(), 0x200191, 0, 0);
                }
                combat->regenTimer = 0x5a;
                if (effect != NULL) {
                    owner = combat->owner;
                    effect->ownerBakugan = owner;
                    if (owner != NULL) {
                        effect->ownerId = owner->base.base.id;
                    }
                }
            }
            healed = true;
            continue;
        }
        if (id == 0x12 && --combat->status12Timer <= 0) {
            BtlCombatApplyDamage(10.0f, combat, -1, 1);
            if (BtlBakuganIsLocalPlayer(combat->owner)) {
                (void)SndManagerIsSoundWordPlaying(SndGetManager(), 0x200073);
            }
            combat->status12Timer = 0x1e;
        }
        if (id == 0x14) {
            scale = 1.0f;
            if (--combat->status14Timer <= 0) {
                if (combat->stats->attribute == 4) {
                    scale = 1.5f;
                }
                owner = combat->owner;
                if ((s32)owner->base.base.unk08 != 0xf) {
                    BtlCombatApplyDamage(scale * 5.0f, combat, -1, 1);
                }
                combat->status14Timer = 0xf;
            }
        }
        remaining = slot->remaining;
        if (remaining != -1) {
            slot->remaining = remaining - 1;
            if ((s16)(remaining - 1) <= 0) {
                BtlCombatClearStatus(combat, id);
            }
        }
    }
}
