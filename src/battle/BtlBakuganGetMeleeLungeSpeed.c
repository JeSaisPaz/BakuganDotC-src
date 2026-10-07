// bdc 0x08865b7c BtlBakuganGetMeleeLungeSpeed
#include "bdc.h"

/* Returns the unit's melee lunge speed stat (`BtlUnitStatTable` `meleeLungeSpeed`, or
   `meleeLungeSpeedCombo3` when the combo index is 3) × `BtlBakuganGetSpeedStatusFactor`; used by
   the melee attack state `BtlBakuganState07Update`. */

float BtlBakuganGetMeleeLungeSpeed(BtlBakugan *bakugan)
{
  BtlUnitStatTable *stats = bakugan->combat.stats;
  float speed;

  if (bakugan->comboIndex == 3) {
    speed = stats->meleeLungeSpeedCombo3;
  }
  else {
    speed = stats->meleeLungeSpeed;
  }
  return speed * BtlBakuganGetSpeedStatusFactor(bakugan);
}
