// bdc 0x088d66b0 GameGimmickIrSensorDraw
#include "bdc.h"

/* Draw (vtable slot 8) of the IR sensor gimmick (`GameGimmickIrSensorCtor`, vtables
   `0x08af303c`/`0x08af30e4`): draws (`GameGimmickDraw`) only while the beam is on (timer
   `+0x1f4`: `on` byte, or bit 1 of its counter during the switch flicker `+0x1f8`) and the player's
   sensor view is active (`player+0x3a1`), or in state 1. */

void GameGimmickIrSensorDraw(GameGimmickIrSensor *obj, u32 **dl)
{
  ActorPlayer *player = (ActorPlayer *)ActorFindPlayer();
  u32 visible = 0;

  if (player != NULL) {
    if (player->scan != 0) {
      const u8 *timer = obj->beamTimer;
      const s16 *counter = (const s16 *)timer;

      visible = 1;
      if (timer != NULL) {
        if (obj->flicker == 0) {
          visible = timer[0] != 0;
        } else {
          visible = (counter[1] >> 1) & 1;
        }
      }
    }
  }
  if (obj->base.state == 1) {
    visible = 1;
  }
  if (visible != 0) {
    GameGimmickDraw(&obj->base, dl);
  }
}
