// bdc 0x08884c88 BtlInputReadActions
#include "bdc.h"

/* Per-frame read of a unit's input controller (`BtlInput`, built by `BtlInputCtorForUnit` /
   `BtlInputCtorForActor`): returns this frame's action bit mask, masked by `allowedActions`.
   Returns 0 (and clears `holdFrames`) while `disabled` is set. `mode` 0 reads the owning
   `BtlBakugan` (`isPlayer`, yaw `base.rot[1]`, `stateFlags` bit 0x10 blocks the charge
   heading latch, `charge` >= 100 makes the charge button a full charge, `playerSlot` picks the net
   record), `mode` 1 the owning `Actor` (`isPlayer`, yaw, `flags` bit 0x10); any other mode acts as a
   player with zero yaw. A non-player owner only gets `aiActions`. Otherwise `chargeCooldown` ticks
   down and the pad state is read from `g_padState`, or in network mode (`SaveGetProfileFlag0`)
   from the `NetCharaMsg` input record of `NetCharaGetByIndex``(0)` / `NetCharaReadSlot`
   (all zero when unavailable). Stick motion (`BtlInputReadLocalStick` /
   `BtlInputReadRemoteStick`) latches `heading` = `stickHeading`, `moveScale` = 2 and bit 1
   (plus 0x2000 when |stickY| < 0.3); `queuedActions` is consumed and cleared; buttons map to
   action bits (pressed 0x4000 → 2, held 0x200 → 4 with the owner yaw latched, pressed 0x10 →
   0x1000000, held 0x40 for 12 frames → 0x1000 else release tap → 0x40, ...); mode 1 adds the
   actor-only bits 0x100000..0x20000000. `dodgeCooldown` and `attackCooldown` tick down on every
   non-disabled call. */

u32 BtlInputReadActions(BtlInput *self)
{
    NetCharaMsg msg;
    NetChara *chara;
    BtlBakugan *unit;
    Actor *actor;
    u32 actions;
    u32 allowed;
    s32 dodge;
    s32 attack;
    s32 mode;
    s32 slot;
    s32 holdFrames;
    u8 isPlayer;
    u8 fullCharge;
    bool blockCharge;
    bool moved;
    u16 buttons;
    u16 pressed;
    u16 released;
    float stickY;
    float ownerYaw;

    if (self->disabled != 0) {
        self->holdFrames = 0;
        return 0;
    }
    stickY = 0.0f;
    ownerYaw = 0.0f;
    isPlayer = 1;
    blockCharge = false;
    fullCharge = 0;
    slot = 0;
    if (self->mode == 0) {
        unit = (BtlBakugan *)self->owner;
        isPlayer = (u8)unit->isPlayer;
        ownerYaw = unit->base.rot[1];
        blockCharge = (unit->stateFlags & 0x10) != 0;
        fullCharge = !(unit->charge < 100.0f);
        slot = unit->playerSlot;
    } else if (self->mode == 1) {
        actor = (Actor *)self->owner;
        isPlayer = actor->isPlayer;
        ownerYaw = actor->base.rot[1];
        blockCharge = (actor->flags & 0x10) != 0;
    }

    if (isPlayer == 0) {
        actions = self->aiActions;
        dodge = self->dodgeCooldown;
        allowed = self->allowedActions;
        attack = self->attackCooldown;
    } else {
        if (self->chargeCooldown != 0) {
            self->chargeCooldown = self->chargeCooldown - 1;
        }
        released = 0;
        buttons = 0;
        pressed = 0;
        moved = false;
        if (SaveGetProfileFlag0() != 0) {
            chara = NetCharaGetByIndex(0);
            memset(&msg, 0, sizeof(msg));
            if (chara != NULL && NetCharaReadSlot(chara, slot, (u32 *)&msg)) {
                buttons = msg.body.input.buttons;
                pressed = msg.body.input.pressed;
                released = msg.body.input.released;
                stickY = msg.body.input.stickY;
                if (BtlInputReadRemoteStick(msg.body.input.stickX, stickY,
                                            msg.body.input.cameraYaw, self) != 0) {
                    moved = true;
                }
            }
        } else {
            buttons = g_padState->buttons;
            pressed = g_padState->pressed;
            released = g_padState->released;
            stickY = g_padState->stickY;
            if (BtlInputReadLocalStick(self) != 0) {
                moved = true;
            }
        }
        actions = self->queuedActions;
        self->queuedActions = 0;
        mode = self->mode;
        dodge = self->dodgeCooldown;
        allowed = self->allowedActions;
        attack = self->attackCooldown;

        if (moved) {
            self->heading = self->stickHeading;
            actions |= 1;
            self->moveScale = 2.0f;
            if (__builtin_fabsf(stickY) < 0.3f) {
                actions |= 0x2000;
            }
        }
        if (dodge == 0 && (pressed & 0x4000) != 0) {
            actions |= 2;
        }
        if ((buttons & 0x4000) != 0) {
            actions |= 0x200;
        }
        if (dodge == 0 && (buttons & 0x200) != 0 && self->chargeCooldown == 0 && !blockCharge) {
            if ((actions & 1) == 0) {
                self->heading = ownerYaw;
            }
            actions |= 4;
        }
        if ((pressed & 0x10) != 0) {
            actions |= 0x1000000;
        }
        if ((pressed & 0x200) != 0) {
            actions |= 0x4000;
        }
        if ((buttons & 0x100) != 0) {
            actions |= 8;
        }
        if ((pressed & 0x100) != 0) {
            actions |= 0x40000;
        }
        if ((pressed & 0xa0) != 0 && (allowed & 0x20) != 0) {
            if ((pressed & 0x80) != 0) {
                actions |= 0xb0;
            }
            if ((pressed & 0x20) != 0) {
                actions |= 0x130;
            }
        }
        if (attack == 0 && (pressed & 0x9000) != 0 && dodge == 0) {
            actions |= 0x10;
            if ((pressed & 0x1000) != 0) {
                if ((allowed & 0x10000) == 0) {
                    actions &= ~0x10u;
                }
                actions |= 0x10000;
            }
            if (self->chargeCooldown == 0 && (buttons & 0x200) != 0) {
                actions |= 4;
            }
        }
        if ((allowed & 0x20000) != 0) {
            if ((released & 0x2000) != 0) {
                if (attack == 0 && dodge == 0) {
                    if (fullCharge != 0) {
                        actions |= 0x20810;
                    } else {
                        actions |= 0x20010;
                        if (self->chargeCooldown == 0 && (buttons & 0x200) != 0) {
                            actions |= 4;
                        }
                    }
                }
            } else if ((buttons & 0x2000) != 0) {
                actions |= 0x400;
            }
        }
        if ((buttons & 0x40) != 0) {
            holdFrames = self->holdFrames + 1;
            self->holdFrames = holdFrames;
            if (holdFrames >= 12) {
                actions |= 0x1000;
            }
        }
        if ((released & 0x40) != 0) {
            if (self->holdFrames < 12) {
                actions |= 0x40;
            }
            self->holdFrames = 0;
        }
        if ((pressed & 0x8000) != 0) {
            actions |= 0x8000;
        }
        if (mode == 1) {
            if ((buttons & 0x100) != 0) {
                actions |= 0x100000;
            }
            if ((buttons & 0x200) != 0) {
                actions |= 0x200000;
            }
            if ((pressed & 0x8000) != 0) {
                actions |= 0x400000;
            }
            if ((buttons & 0x8000) == 0) {
                actions |= 0x800000;
            }
            if ((buttons & 0x2000) != 0) {
                actions |= 0x1000000;
            }
            if ((buttons & 0x100) != 0 && (buttons & 0x200) != 0) {
                actions |= 0x2000000;
            }
            if ((pressed & 0x1000) != 0) {
                actions |= 0x8000000;
            }
            if ((pressed & 0x2000) != 0) {
                actions |= 0x10000000;
            }
            if ((buttons & 0x200) != 0) {
                actions |= 0x20000000;
            }
        }
    }

    if (dodge != 0) {
        self->dodgeCooldown = dodge - 1;
    }
    if (attack != 0) {
        self->attackCooldown = attack - 1;
    }
    return actions & allowed;
}
