// bdc 0x0885fc40 BtlBakuganIsLocalPlayer
#include "bdc.h"

/* True when the unit is player-controlled (`isPlayer`) and either profile flag 0 is clear
   (offline) or a NetPlay manager exists and the unit's `playerSlot` equals the local
   session slot (`NetPlayGetLocalSlot`). False for a networked player unit when no
   manager exists. */

bool BtlBakuganIsLocalPlayer(BtlBakugan *self)
{
    bool result;

    result = false;
    if (self->isPlayer != 0) {
        if (SaveGetProfileFlag0() == 0) {
            result = true;
        } else if (NetPlayHasManager()) {
            if (self->playerSlot == NetPlayGetLocalSlot(NetPlayGetManager())) {
                result = true;
            }
        }
    }
    return result;
}
