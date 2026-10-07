// bdc 0x088d7b40 GameGimmickJetDoorUpdateOpen
#include "bdc.h"

/* Door logic of the Marucho-jet cabin door gimmick (`GameGimmickJetDoorCtor`, vtables
   `0x08af31ac`/`0x08af324c`). Nothing without a player (`ActorFindPlayer`). While the player is
   within 5 units of the door point `triggerPos`: a closed door starts its open motion (speed 1, no
   loop) with sound `0x2c00019` and becomes `open`/`opening`; an open door holds its motion at frame
   20 once it got there. Either way it sets contact flag bit 1 and the player's `promptA` byte.
   Out of range: an open door closes (motion speed -1 with the same sound, `open` cleared) unless it
   is still `opening` and below frame 20 (`opening` is cleared once frame 20 is reached); contact
   bit 1 and the player's prompt byte are cleared. */

void GameGimmickJetDoorUpdateOpen(GameGimmickJetDoor *obj)

{
  ActorPlayer *player;
  ScePspFVector4 pos;
  const VtblEntry *e;
  bool close;

  player = (ActorPlayer *)ActorFindPlayer();
  if (player == NULL) {
    return;
  }
  pos = obj->triggerPos;
  if (GameIsPlayerWithinRange(5.0f, obj, (const float *)&pos)) {
    if (!obj->open) {
      GfxModelEnableMotion(&obj->base.base);
      e = &((const VtblEntry *)obj->base.base.base.vtable)[6];
      ((void (*)(void *, float))e->fn)((u8 *)obj + e->delta, 1.0f);
      GfxModelSetMotionLoop(&obj->base.base, 0);
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0x2c00019, 0, 0);
      }
      obj->opening = 1;
      obj->open = 1;
    }
    else if (!(GfxModelMotionFrame(&obj->base.base) < 20.0f)) {
      GfxModelSwapMotionFrame(&obj->base.base, 20.0f);
    }
    obj->base.contactFlags |= 2;
    player->promptA = 1;
    return;
  }
  close = false;
  if (obj->opening) {
    if (!(GfxModelMotionFrame(&obj->base.base) < 20.0f)) {
      if (obj->open) {
        close = true;
      }
      obj->opening = 0;
    }
  }
  else if (obj->open) {
    close = true;
  }
  if (close) {
    GfxModelEnableMotion(&obj->base.base);
    e = &((const VtblEntry *)obj->base.base.base.vtable)[6];
    ((void (*)(void *, float))e->fn)((u8 *)obj + e->delta, -1.0f);
    if (SndHasManager()) {
      SndManagerPlay(SndGetManager(), 0x2c00019, 0, 0);
    }
    GfxModelSetMotionLoop(&obj->base.base, 0);
    obj->open = 0;
  }
  obj->base.contactFlags &= ~2;
  player->promptA = 0;
  return;
}
