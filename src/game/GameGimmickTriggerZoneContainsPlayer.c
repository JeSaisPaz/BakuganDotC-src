// bdc 0x088da1ec GameGimmickTriggerZoneContainsPlayer
#include "bdc.h"

/* Tests the player against the zone's shape record (`+0x180`): fills the shared probe shape
   `g_btlBakuganSphereQuery` with the player position (`ActorFindPlayer`) and radius 1.0,
   refreshes it through its virtual `+0x4c` and runs the shape-vs-shape test `CollisionBoxVsSphere`. Returns
   1 when they overlap. */

s32 GameGimmickTriggerZoneContainsPlayer(GameGimmickTriggerZone *gimmick)
{
  Actor *player = (Actor *)ActorFindPlayer();
  ScePspFVector4 contact;
  const VtblEntry *entry;

  g_btlBakuganSphereQuery.radius = 1.0f;
  g_btlBakuganSphereQuery.center.x = player->base.pos[0];
  g_btlBakuganSphereQuery.center.y = player->base.pos[1];
  g_btlBakuganSphereQuery.center.z = player->base.pos[2];
  g_btlBakuganSphereQuery.center.w = player->base.pos[3];
  entry = &g_btlBakuganSphereQuery.vtbl[9];
  ((void (*)(void *))entry->fn)((u8 *)&g_btlBakuganSphereQuery + entry->delta);
  if (CollisionBoxVsSphere(&gimmick->box, &g_btlBakuganSphereQuery, &contact)) {
    return 1;
  }
  return 0;
}
