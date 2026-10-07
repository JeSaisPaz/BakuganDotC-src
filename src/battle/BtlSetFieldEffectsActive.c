// bdc 0x0884b518 BtlSetFieldEffectsActive
#include "bdc.h"

/* Turns the battle-field effects on or off: passes `active` to
   `ActorStageObjSetAttrLandmarksVisible` (attribute landmarks) and then to
   `ActorCrystalSetAllEffectsVisible` (crystals). */
void BtlSetFieldEffectsActive(bool active)
{
    ActorStageObjSetAttrLandmarksVisible(active);
    ActorCrystalSetAllEffectsVisible(active);
}

