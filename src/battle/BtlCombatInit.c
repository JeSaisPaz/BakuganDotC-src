// bdc 0x088865f8 BtlCombatInit
#include "bdc.h"

/* Zero-initialises a `BtlCombatState`: `owner`, `stats`, the `dead` flag and the bytes/words
   `reserved8c`, `reserved04`, `reserved90`, `maxHp`/`hp`/`energy`, `link`, `regenDelay` and
   `reservedA8`, the three status-damage timers, the three art slots (charge and cooldown 0, id
   -1) with the three listed status ids, `selectedArt` = -1, `artSlotCount`, `artReadyFrames`,
   the six `slotIds`, and all 21 status slots (`total`, `remaining`, `active`); then
   `BtlCombatClearTimedStatuses`. The state is not usable until `BtlCombatSetup` attaches an
   owner and a stat table. */
void BtlCombatInit(BtlCombatState *combat)
{
  s32 i;

  combat->stats = NULL;
  combat->dead = 0;
  combat->reserved90 = 0;
  combat->reserved8c = 0;
  combat->owner = NULL;
  combat->reserved04 = 0;
  combat->maxHp = 0.0f;
  combat->hp = 0.0f;
  combat->energy = 0.0f;
  combat->link = NULL;
  combat->regenDelay = 0.0f;
  combat->reservedA8 = 0.0f;
  combat->regenTimer = 0;
  combat->status12Timer = 0;
  combat->status14Timer = 0;
  for (i = 0; i < 3; i++) {
    combat->artCharge[i] = 0.0f;
    combat->artCooldown[i] = 0.0f;
    combat->artIds[i] = -1;
    combat->listedStatusIds[i] = 0;
  }
  combat->selectedArt = -1;
  combat->artSlotCount = 0;
  combat->artReadyFrames = 0;
  for (i = 0; i < 6; i++) {
    combat->slotIds[i] = 0;
  }
  for (i = 0; i < 21; i++) {
    combat->status[i].active = 0;
    combat->status[i].total = 0;
    combat->status[i].remaining = 0;
  }
  BtlCombatClearTimedStatuses(combat);
}
