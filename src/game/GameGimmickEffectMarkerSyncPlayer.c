// bdc 0x088da91c GameGimmickEffectMarkerSyncPlayer
#include "bdc.h"

/* For marker type 0x73 (`+0x188`) with a second effect (`+0x184`): caches the player
   (`ActorFindPlayer`) at `+0x18c` and shows the effect (alpha `+0xbc` = 1) only while the
   player's flag `+0x3a1` is 1. */

typedef struct {
  u8 pad[0x3a1];
  u8 hudFlag;
} GameGimmickPlayerView;

void GameGimmickEffectMarkerSyncPlayer(GameGimmickEffectMarker *gimmick)

{
  if ((gimmick->typeId == 0x73) && (gimmick->effect2 != (void *)0x0)) {
    GameGimmickPlayerView *player = (GameGimmickPlayerView *)ActorFindPlayer();

    gimmick->player = player;
    if (player != (GameGimmickPlayerView *)0x0) {
      GfxSprite *effect = (GfxSprite *)gimmick->effect2;

      if (player->hudFlag == 1) {
        effect->alpha = 1.0f;
      }
      else {
        effect->alpha = 0.0f;
      }
    }
  }
  return;
}
