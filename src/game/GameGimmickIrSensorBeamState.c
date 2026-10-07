// bdc 0x088d682c GameGimmickIrSensorBeamState
#include "bdc.h"

/* Beam state of the IR sensor gimmick (`GameGimmickIrSensorCtor`, vtables
   `0x08af303c`/`0x08af30e4`): when the sensor has a beam timer (`GameGimmickIrSensorBeamTimer`
   at `beamTimer`) it counts it down, flickers 30 frames (`flicker`) when the phase runs out and then
   toggles the beam (on for `onTime`, off for `offTime` frames); without a timer the beam is always
   on. While the beam is on (not flickering) and hits the player (`GameGimmickIrSensorHitsPlayer`),
   it switches to state 1 (step 0); unless the field is already catching the player
   (`GameFieldTask.catchActive`) it also catches the player once: sets `detected`, `ActorSetState`
   9, `ActorPlayerTakePenalty` unless the shared event counter byte `g_gameEventFlags[5]` is set,
   and clears `waitTimer` / `caughtBy` with `behaviourMark = 0xff`. */

void GameGimmickIrSensorBeamState(GameGimmickIrSensor *obj)
{
  GameGimmickIrSensorBeamTimer *timer;
  GameFieldTask *field;
  ActorPlayer *player;
  u8 flicker;
  u8 beamOn;

  timer = (GameGimmickIrSensorBeamTimer *)obj->beamTimer;
  beamOn = 1;
  if (timer != NULL) {
    if (timer->count > 0) {
      timer->count = timer->count - 1;
      flicker = obj->flicker;
    } else if (obj->flicker == 0) {
      obj->flicker = 1;
      timer->count = 30;
      flicker = obj->flicker;
    } else {
      if (timer->on != 0) {
        timer->on = 0;
        ((GameGimmickIrSensorBeamTimer *)obj->beamTimer)->count =
            ((GameGimmickIrSensorBeamTimer *)obj->beamTimer)->offTime;
      } else {
        timer->on = 1;
        ((GameGimmickIrSensorBeamTimer *)obj->beamTimer)->count =
            ((GameGimmickIrSensorBeamTimer *)obj->beamTimer)->onTime;
      }
      obj->flicker = 0;
      flicker = obj->flicker;
    }
    beamOn = 0;
    if (flicker == 0 && ((GameGimmickIrSensorBeamTimer *)obj->beamTimer)->on != 0) {
      beamOn = 1;
    }
  }
  if (beamOn != 0 && GameGimmickIrSensorHitsPlayer(obj) != 0) {
    field = (GameFieldTask *)CoreTaskFind(500);
    if (field->catchActive == 0) {
      player = (ActorPlayer *)ActorFindPlayer();
      if (player != NULL && player->base.detected == 0) {
        player->base.detected = 1;
        ActorSetState(&player->base, 9, 0);
        if (g_gameEventFlags[5] == 0) {
          ActorPlayerTakePenalty(player);
        }
        player->base.waitTimer = 0;
        player->behaviourMark = 0xff;
        player->caughtBy = NULL;
      }
    }
    obj->base.state = 1;
    obj->step = 0;
  }
}
