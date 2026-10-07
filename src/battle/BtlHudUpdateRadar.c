// bdc 0x08834548 BtlHudUpdateRadar
#include "bdc.h"

/* HUD radar (`BtlHudPhaseMain`): shows radar sprites 0x60/0x61 (depth 0) and places the radar
   map sprite 0xc6 at (0x1aa - radarX, 0x2f - radarY). With a player Bakugan
   (`BtlHudGetPlayerBakugan`) the map is shifted by the player's scaled x/z position, the
   player arrow (sprite 199, depth -4) is rotated by the player heading (`rot[1]`, 0 replaced by
   1e-5), and one blip is placed per tracked object: 4 opponent units (`BtlHudGetEnemyUnit`,
   sprites 200+i, depth -5, hidden when dead or untargetable (status 9), rotated by heading - pi/2),
   5 crystals (`BtlHudGetNthCrystal`, sprites 210+i, depth -1), 3 prop targets
   (`BtlHudGetNthPropTarget`, sprites 204+i, depth -3) and 3 landmark-attr targets
   (`BtlHudGetNthLandmarkAttrTarget`, sprites 207+i, depth -2); dead or missing objects hide
   their blip. A blip's offset is the object's scaled x/z position relative to the player,
   rotated by -(camera yaw) - pi/2 (camera yaw = atan2f(dir.z, dir.x) of `g_gfxActiveCamera`)
   and clamped to radius 30, then truncated to whole pixels. The radar math is lifted from VFPU
   code; S703 (radians-to-quarter-turn factor of `vrot.q`) and S713 (the `vcmovt`
   fallback for a zero-length offset) are bank constants (2/pi and 0). As compiled, the depth of
   sprites 0x60/0x61 is stored even when that sprite is NULL. */

/* Computes the radar offset of `unit` into `tmp` (x in tmp[0], z in tmp[2]); one copy of the
   per-blip sequence, inlined into each loop as in the listing. */
static inline void BtlHudRadarOffset(BtlHud *self, BtlBakugan *unit, float *tmp, float originX,
                                     float originZ) __attribute__((always_inline));
static inline void BtlHudRadarOffset(BtlHud *self, BtlBakugan *unit, float *tmp, float originX,
                                     float originZ)
{
    float angle;
    float t;
    float c;
    float s;
    float x;
    float y;
    float z;
    float lenSq;
    float k;

    tmp[0] = unit->base.pos[0];
    tmp[1] = unit->base.pos[1];
    tmp[2] = unit->base.pos[2];
    tmp[3] = unit->base.pos[3];
    tmp[1] = 0.0f;
    tmp[0] = tmp[0] * self->radarScale;
    tmp[1] = tmp[1] * self->radarScale;
    tmp[2] = tmp[2] * self->radarScale;
    tmp[0] = tmp[0] - originX;
    tmp[2] = tmp[2] - originZ;
    angle = -atan2f(g_gfxActiveCamera->dir[2], g_gfxActiveCamera->dir[0]) - 1.57079637f;
    t = angle * 0.636619747f;
    c = VfCosQuarter(t);
    s = VfSinQuarter(t);
    x = tmp[0];
    y = tmp[1];
    z = tmp[2];
    /* rotate (x, z) by the rows (c, 0, -s) and (s, 0, c) */
    tmp[0] = x * c + y * 0.0f + z * -s;
    tmp[2] = x * s + y * 0.0f + z * c;
    lenSq = tmp[0] * tmp[0] + 0.0f * 0.0f + tmp[2] * tmp[2];
    if (!(lenSq <= 900.0f)) {
        /* clamp to radius 30: offset * 30 / |offset| (0 when the length is zero) */
        lenSq = tmp[0] * tmp[0] + tmp[1] * tmp[1] + tmp[2] * tmp[2];
        k = (lenSq == 0.0f) ? 0.0f : VfRsq(lenSq);
        k = k * 30.0f;
        tmp[0] = tmp[0] * k;
        tmp[1] = tmp[1] * k;
        tmp[2] = tmp[2] * k;
    }
}

void BtlHudUpdateRadar(BtlHud *self)
{
    GfxSprite *map;
    GfxSprite *sprite;
    BtlBakugan *player;
    BtlBakugan *unit;
    float originX;
    float originZ;
    float heading;
    float enemyPos[4] __attribute__((aligned(16)));
    float crystalPos[4] __attribute__((aligned(16)));
    float propPos[4] __attribute__((aligned(16)));
    float landmarkPos[4] __attribute__((aligned(16)));
    s32 i;

    sprite = self->sprites[0x60];
    if (sprite != NULL) {
        sprite->flags |= 1;
    }
    sprite->posZ = 0.0f;
    sprite = self->sprites[0x61];
    if (sprite != NULL) {
        sprite->flags |= 1;
    }
    sprite->posZ = 0.0f;

    player = (BtlBakugan *)BtlHudGetPlayerBakugan(self);
    map = self->sprites[0xc6];
    map->posX = (float)(0x1aa - self->radarX);
    map->posZ = 0.0f;
    map->posY = (float)(0x2f - self->radarY);
    if (player == NULL) {
        return;
    }
    originX = player->base.pos[0] * self->radarScale;
    heading = player->base.rot[1];
    originZ = player->base.pos[2] * self->radarScale;
    map->posX = map->posX - originX;
    map->posY = map->posY - originZ;
    sprite = self->sprites[199];
    if (heading == 0.0f) {
        heading = 9.99999975e-06f;
    }
    GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, heading, false);
    sprite->posZ = -4.0f;

    for (i = 0; i < 4; i++) {
        unit = (BtlBakugan *)BtlHudGetEnemyUnit(self, i);
        sprite = self->sprites[200 + i];
        if (unit == NULL || unit->combat.dead != 0 || unit->combat.status[9].active != 0) {
            sprite->flags &= ~1u;
            continue;
        }
        BtlHudRadarOffset(self, unit, enemyPos, originX, originZ);
        enemyPos[0] = originX + map->posX + (float)self->radarX + enemyPos[0];
        enemyPos[2] = originZ + map->posY + (float)self->radarY + enemyPos[2];
        sprite->posX = (float)(s32)enemyPos[0];
        sprite->posZ = -5.0f;
        sprite->posY = (float)(s32)enemyPos[2];
        GfxSpriteSetScaleRotation(sprite, 1.0f, 1.0f, unit->base.rot[1] - 1.57079637f, false);
        sprite->flags |= 1;
    }

    for (i = 0; i < 5; i++) {
        unit = (BtlBakugan *)BtlHudGetNthCrystal(self, i);
        sprite = self->sprites[210 + i];
        if (unit == NULL || unit->combat.dead != 0) {
            sprite->flags &= ~1u;
            continue;
        }
        BtlHudRadarOffset(self, unit, crystalPos, originX, originZ);
        crystalPos[0] = originX + map->posX + (float)self->radarX + crystalPos[0];
        crystalPos[2] = originZ + map->posY + (float)self->radarY + crystalPos[2];
        sprite->posX = (float)(s32)crystalPos[0];
        sprite->posZ = -1.0f;
        sprite->flags |= 1;
        sprite->posY = (float)(s32)crystalPos[2];
    }

    for (i = 0; i < 3; i++) {
        unit = (BtlBakugan *)BtlHudGetNthPropTarget(self, i);
        sprite = self->sprites[204 + i];
        if (unit == NULL || unit->combat.dead != 0) {
            sprite->flags &= ~1u;
            continue;
        }
        BtlHudRadarOffset(self, unit, propPos, originX, originZ);
        propPos[0] = originX + map->posX + (float)self->radarX + propPos[0];
        propPos[2] = originZ + map->posY + (float)self->radarY + propPos[2];
        sprite->posX = (float)(s32)propPos[0];
        sprite->posZ = -3.0f;
        sprite->flags |= 1;
        sprite->posY = (float)(s32)propPos[2];
    }

    for (i = 0; i < 3; i++) {
        unit = (BtlBakugan *)BtlHudGetNthLandmarkAttrTarget(self, i);
        sprite = self->sprites[207 + i];
        if (unit == NULL || unit->combat.dead != 0) {
            sprite->flags &= ~1u;
            continue;
        }
        BtlHudRadarOffset(self, unit, landmarkPos, originX, originZ);
        landmarkPos[0] = originX + map->posX + (float)self->radarX + landmarkPos[0];
        landmarkPos[2] = originZ + map->posY + (float)self->radarY + landmarkPos[2];
        sprite->posX = (float)(s32)landmarkPos[0];
        sprite->posZ = -2.0f;
        sprite->flags |= 1;
        sprite->posY = (float)(s32)landmarkPos[2];
    }
}
