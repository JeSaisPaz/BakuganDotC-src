// bdc 0x0883eaf0 BtlHudDrawLayers
#include "bdc.h"

/* Draws the battle HUD. First the .fab objects of `BtlHud``.fabs` (`GfxFabDraw`): entry 0
   unless `stageIntroDone`, entries 1..3 unless `fabsHidden`. Returns at once when the HUD has no
   sprite table. Otherwise it places the 3 opponent name plates and their markers (sprites
   0xbc..0xbe and 0xbf..0xc1) over the units of `BtlHudGetNthOtherBakugan` (rule mode 2) or
   `BtlHudGetEnemyUnit`: each unit's position, raised by its stat-table height, is projected
   with `GfxCameraProjectPoint` on `g_gfxActiveCamera` (clip depth from
   `MathVfpuStoreC100`); both sprites are hidden for a missing or dead unit, a point behind
   depth -3 or a plate `BtlHudIsNamePlateHidden` hides, else shown at the screen point minus
   (64, 30) / (12, 8) and, for a battle Bakugan (virtual slot 10) with an HP gauge, given the
   gauge's alpha; in rule mode 1 the marker is hidden again. The same is done for the HP-gauge
   sprites 0xfb..0xfd of the 3 standing HP objects (`ActorStageObjGetStandingTarget`), shown
   only while `BtlHudIsVisible` is false; their icons (sprites 0x78, 0x7a, 0x7c) are shown
   with alpha 0 (no gauge), 1 (alive and the battle not over, `g_btlBattleOver`) or the
   gauge's alpha. Finally the HUD layers are submitted as render packets (`GfxNewRenderPacket`,
   `GfxSpriteLayerDraw`): `layer` five times with layer masks 8, 0x40, 1, 0x10, 0x20 at depths
   3, 80, 100, 105, 115 (`GfxSpriteLayerSetLayerMask`), speed lines into the last packet while
   `speedLines` is set (`GfxPacketDrawSpeedLines`), then the optional `resultLayer` (115),
   `msgLayer` (106), `ratingLayer` (125, followed once the battle is over by `overlayObj[1]` at
   `overlayDepth`) and `arenaLayer` (130), and `BtlHudDrawOverlayObjects`. */
void BtlHudDrawLayers(BtlHud *self)
{
    float screen[4];
    float pos[4];
    float clip[4];
    ScePspFVector4 proj;
    BtlBakugan *unit;
    ActorStageObjBase *obj;
    void *packet;
    void *overlay;
    s32 i;

    if (self->fabs != NULL) {
        if (self->fabs[0] != NULL && self->stageIntroDone == 0) {
            GfxFabDraw(self->fabs[0]);
        }
        if (self->fabs[1] != NULL && self->fabsHidden == 0) {
            GfxFabDraw(self->fabs[1]);
        }
        if (self->fabs[2] != NULL && self->fabsHidden == 0) {
            GfxFabDraw(self->fabs[2]);
        }
        if (self->fabs[3] != NULL && self->fabsHidden == 0) {
            GfxFabDraw(self->fabs[3]);
        }
    }
    if (self->sprites == NULL) {
        return;
    }

    for (i = 0; i < 3; i++) {
        if (g_scriptGlobalVars[8] == 2) {
            unit = (BtlBakugan *)BtlHudGetNthOtherBakugan(self, i);
        } else {
            unit = (BtlBakugan *)BtlHudGetEnemyUnit(self, i);
        }
        if (unit == NULL) {
            self->sprites[0xbc + i]->flags &= ~1u;
            self->sprites[0xbf + i]->flags &= ~1u;
            continue;
        }
        if (unit->combat.dead == 0) {
            pos[0] = unit->base.pos[0];
            pos[1] = unit->base.pos[1];
            pos[2] = unit->base.pos[2];
            pos[3] = unit->base.pos[3];
            pos[1] = pos[1] + unit->combat.stats->height;
            proj = GfxCameraProjectPoint(g_gfxActiveCamera, screen, pos);
            MathVfpuStoreC100(clip, proj);
            if (clip[2] <= -3.0f || BtlHudIsNamePlateHidden(self, unit) != 0) {
                self->sprites[0xbc + i]->flags &= ~1u;
                self->sprites[0xbf + i]->flags &= ~1u;
            } else {
                const VtblEntry *isBakugan = &((const VtblEntry *)unit->base.base.vtable)[10];

                self->sprites[0xbc + i]->flags |= 1u;
                self->sprites[0xbc + i]->posX = (float)(s32)(screen[0] - 64.0f);
                self->sprites[0xbc + i]->posY = (float)(s32)(screen[1] - 30.0f);
                self->sprites[0xbf + i]->flags |= 1u;
                self->sprites[0xbf + i]->posX = (float)(s32)(screen[0] - 12.0f);
                self->sprites[0xbf + i]->posY = (float)(s32)(screen[1] - 8.0f);
                if (((s32 (*)(void *))isBakugan->fn)((u8 *)unit + isBakugan->delta) != 0 &&
                    unit->hpGauge != NULL) {
                    self->sprites[0xbc + i]->alpha = ((UiHpGauge *)unit->hpGauge)->alpha;
                    self->sprites[0xbf + i]->alpha = ((UiHpGauge *)unit->hpGauge)->alpha;
                }
            }
        } else {
            self->sprites[0xbc + i]->flags &= ~1u;
            self->sprites[0xbf + i]->flags &= ~1u;
        }
        if (g_scriptGlobalVars[8] == 1) {
            self->sprites[0xbf + i]->flags &= ~1u;
        }
    }

    for (i = 0; i < 3; i++) {
        obj = (ActorStageObjBase *)ActorStageObjGetStandingTarget(i);
        if (obj == NULL) {
            self->sprites[0x78 + i * 2]->flags &= ~1u;
            continue;
        }
        pos[0] = obj->gaugePos[0];
        pos[1] = obj->gaugePos[1];
        pos[2] = obj->gaugePos[2];
        pos[3] = obj->gaugePos[3];
        proj = GfxCameraProjectPoint(g_gfxActiveCamera, screen, pos);
        MathVfpuStoreC100(clip, proj);
        if (clip[2] <= -3.0f || BtlHudIsVisible()) {
            self->sprites[0xfb + i]->flags &= ~1u;
        } else {
            self->sprites[0xfb + i]->flags |= 1u;
            self->sprites[0xfb + i]->posX = (float)(s32)(screen[0] - 64.0f);
            self->sprites[0xfb + i]->posY = (float)(s32)(screen[1] - 30.0f);
            if (obj->hpGauge != NULL) {
                self->sprites[0xfb + i]->alpha = ((UiHpGauge *)obj->hpGauge)->alpha;
            }
        }
        self->sprites[0x78 + i * 2]->flags |= 1u;
        if (obj->hpGauge == NULL) {
            self->sprites[0x78 + i * 2]->alpha = 0.0f;
        } else if (obj->dead == 0 && g_btlBattleOver == 0) {
            self->sprites[0x78 + i * 2]->alpha = 1.0f;
        } else {
            self->sprites[0x78 + i * 2]->alpha = ((UiHpGauge *)obj->hpGauge)->alpha;
        }
    }

    packet = GfxNewRenderPacket(3.0f);
    GfxSpriteLayerSetLayerMask(self->layer, 0x8);
    GfxSpriteLayerDraw(self->layer, packet);
    packet = GfxNewRenderPacket(80.0f);
    GfxSpriteLayerSetLayerMask(self->layer, 0x40);
    GfxSpriteLayerDraw(self->layer, packet);
    packet = GfxNewRenderPacket(100.0f);
    GfxSpriteLayerSetLayerMask(self->layer, 0x1);
    GfxSpriteLayerDraw(self->layer, packet);
    packet = GfxNewRenderPacket(105.0f);
    GfxSpriteLayerSetLayerMask(self->layer, 0x10);
    GfxSpriteLayerDraw(self->layer, packet);
    packet = GfxNewRenderPacket(115.0f);
    GfxSpriteLayerSetLayerMask(self->layer, 0x20);
    GfxSpriteLayerDraw(self->layer, packet);
    if (self->speedLines != 0) {
        GfxPacketDrawSpeedLines(240.0f, 136.0f, 90.0f, 51.0f, 0.5f, packet, &g_colorWhite.x);
    }
    if (self->resultLayer != NULL) {
        packet = GfxNewRenderPacket(115.0f);
        GfxSpriteLayerDraw(self->resultLayer, packet);
    }
    if (self->msgLayer != NULL) {
        packet = GfxNewRenderPacket(106.0f);
        GfxSpriteLayerDraw(self->msgLayer, packet);
    }
    if (self->ratingLayer != NULL) {
        packet = GfxNewRenderPacket(125.0f);
        GfxSpriteLayerDraw(self->ratingLayer, packet);
        if (g_btlBattleOver != 0 && self->overlayObj[1] != NULL) {
            overlay = self->overlayObj[1];
            packet = GfxNewRenderPacket(self->overlayDepth);
            GfxSpriteLayerDraw(overlay, packet);
        }
    }
    if (self->arenaLayer != NULL) {
        packet = GfxNewRenderPacket(130.0f);
        GfxSpriteLayerDraw(self->arenaLayer, packet);
    }
    BtlHudDrawOverlayObjects(self);
}
