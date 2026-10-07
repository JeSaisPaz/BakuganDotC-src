// bdc 0x0889fd0c GameGimmickCorePointDraw
#include "bdc.h"

/* Draw method of the core-point gimmick (vtable `0x08af234c` slot 8): skips drawing while the
   player is hidden (`player+0x3a1` clear while `+0x1a8` is set) and otherwise calls the gimmick
   draw `GameGimmickDraw` when the fade alpha `+0x178` is positive. */

typedef struct {
  u8 pad[0x3a1];
  u8 hudFlag;
} GameGimmickPlayerView;

void GameGimmickCorePointDraw(GameGimmickCorePoint *obj, void *ctx)

{
  GameGimmickPlayerView *player = (GameGimmickPlayerView *)ActorFindPlayer();

  if (player != (GameGimmickPlayerView *)0x0 && obj->invisible != '\0' && player->hudFlag == 0) {
    return;
  }
  if (!((obj->base).alpha <= 0.0f)) {
    GameGimmickDraw(&obj->base,ctx);
  }
  return;
}
