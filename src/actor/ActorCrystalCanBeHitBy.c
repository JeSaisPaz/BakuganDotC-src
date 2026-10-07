// bdc 0x088575c8 ActorCrystalCanBeHitBy
#include "bdc.h"

/* True when a collider hit of kind `hitKind` affects this crystal: type `+0x934` 4 only kind 1,
   type 5 kinds 2..4, type 6 any while `+0x943` is set. */

bool ActorCrystalCanBeHitBy(ActorCrystal *self, int hitKind)
{
    int type = self->type;

    if (type < 5) {
        if (type >= 4 && hitKind == 1) {
            return true;
        }
    } else if (type < 6) {
        if (hitKind >= 2 && hitKind < 5) {
            return true;
        }
    } else if (type < 7 && self->hitByAll != 0) {
        return true;
    }
    return false;
}
