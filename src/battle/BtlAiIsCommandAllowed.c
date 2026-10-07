// bdc 0x0888f99c BtlAiIsCommandAllowed
#include "bdc.h"

/* Returns whether `BtlAi` may issue rule command `cmd` now (`BtlAiSelectRule` skips
   rules whose command fails): 0xb/0x10 are refused while `chargeCooldown` (set by
   `BtlAiRunAttackRules`) is positive; 0xa, 0xb and 0x10 need a `target`; the moves that hold
   button 4 (2, 4, 6, 8, 0x18) need ability flag 4 of `allowedCmds`, an owner kind outside
   0x15..0x1f and an owner state in {0, 1, 2, 0xc, 0xd, 0x13}, unless owner state flag 0x1000000 is
   set; 9 (random-direction dash) needs flag 0x200 and state 0/1/0xc/0xd/0x13; 0xa..0xe and 0x10
   need `targetOccluded` clear; 0x11 needs the owner free (no state flag 0x400000/0x100, not in
   state 8/10, no command bits 0xc00); arts 0x12..0x15 need `BtlCombatIsArtTierReady``(cmd − 0x12)`
   (and `targetOccluded` clear when tier 1 is ready); 0x1a is refused on stages 36/38
   (`GameStageIs36Or38`) and otherwise tested like the button-4 moves; everything else is
   allowed. */

bool BtlAiIsCommandAllowed(BtlAi *self, s32 cmd)
{
    BtlBakugan *owner;
    bool allowed;
    bool busy;
    bool restrictedKind;
    s32 kind;

    if ((cmd == 0x10 || cmd == 0xb) && self->chargeCooldown > 0) {
        return false;
    }
    if (self->target == NULL) {
        if (cmd < 0xc) {
            if (cmd >= 10) {
                return false;
            }
        } else if (cmd == 0x10) {
            return false;
        }
    }

    switch (cmd) {
    case 0x1a:
        if (GameStageIs36Or38() != 0) {
            return false;
        }
        /* fall through: tested like the button-4 moves */
    case 2:
    case 4:
    case 6:
    case 8:
    case 0x18:
        owner = self->owner;
        if ((owner->stateFlags & 0x1000000) != 0) {
            return true;
        }
        allowed = false;
        if ((self->allowedCmds & 4) != 0) {
            kind = -1;
            if (owner != NULL) {
                kind = (s32)owner->base.base.unk08;
            }
            restrictedKind = kind > 0x14 && kind < 0x21;
            if (restrictedKind && (s32)self->owner->base.base.unk08 == 0x20) {
                restrictedKind = false;
            }
            if (!restrictedKind) {
                switch (self->owner->state) {
                case 0:
                case 1:
                case 2:
                case 0xc:
                case 0xd:
                case 0x13:
                    allowed = true;
                    break;
                }
            }
        }
        return allowed;
    case 9:
        allowed = false;
        if ((self->allowedCmds & 0x200) != 0) {
            switch (self->owner->state) {
            case 0:
            case 1:
            case 0xc:
            case 0xd:
            case 0x13:
                allowed = true;
                break;
            }
        }
        return allowed;
    case 10:
    case 0xb:
    case 0xc:
    case 0xd:
    case 0xe:
    case 0x10:
        return self->targetOccluded == 0;
    case 0x11:
        owner = self->owner;
        if ((owner->stateFlags & 0x400000) != 0) {
            return false;
        }
        busy = owner->state == 8 || owner->state == 10;
        if (busy || (owner->commands & 0xc00) != 0 || (owner->stateFlags & 0x100) != 0) {
            return false;
        }
        return true;
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
        allowed = false;
        if (self->owner != NULL && BtlCombatIsArtTierReady(&self->owner->combat, cmd - 0x12) != 0) {
            allowed = true;
        }
        if (!allowed) {
            return false;
        }
        if (BtlCombatIsArtTierReady(&self->owner->combat, 1) != 0) {
            return self->targetOccluded == 0;
        }
        return true;
    default:
        return true;
    }
}
