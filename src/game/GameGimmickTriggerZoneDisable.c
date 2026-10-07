// bdc 0x088da5b0 GameGimmickTriggerZoneDisable
#include "bdc.h"

/* Vtable `0x08af3474` slot 16 of the trigger-zone gimmick: clears the active flag `+0x15e`. */

void GameGimmickTriggerZoneDisable(GameGimmickTriggerZone *gimmick)

{
  if ((gimmick->base).active != '\0') {
    (gimmick->base).active = '\0';
  }
  return;
}

