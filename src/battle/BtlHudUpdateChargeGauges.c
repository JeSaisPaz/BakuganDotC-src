// bdc 0x08830f8c BtlHudUpdateChargeGauges
#include "bdc.h"

/* Updates the two vertical charge gauges of the HUD for `unit` (called by
   `BtlHudUpdateCardCutIns` with the player Bakugan). Does nothing unless the unit is a Bakugan
   (vtable slot 10, `BtlBakuganIsBakugan`). For each gauge i < 2: past
   `combat.artSlotCount` only the glow and frame sprites get identity scale. Otherwise the fill
   ratio is the combat status ratio (`BtlCombatGetStatusRatio`) when `combat.listedStatusIds[i]`
   is 6..10, else `combat.artCharge[i] * 0.0001` (0..10000); the fill sprite (`sprites[194 + i]`)
   moves up by `32 * ratio` from `gaugeAnchor[i][0]` and is cropped to height `64 * (1 - ratio)`
   (`BtlHudSetSpriteRect`); the surface sprite (`sprites[114 + i]`) is shown only while
   0 < 1 - ratio < 1, rides the fill level, and flips U/V whenever `gaugeFlipTimer[i]` reaches 1.5
   (+1 per frame). While full, the glow sprite pulses: tint lerps yellow→white by
   `0.4 + 0.2 * sin(gaugeGlowAngle[i])` (angle +0.2793 rad per frame, wrapped at 2π). */

void BtlHudUpdateChargeGauges(BtlHud *self, BtlBakugan *unit)
{
    const VtblEntry *vtbl;
    GfxSprite **sprites;
    GfxSprite *glow;
    GfxSprite *ring;
    GfxSprite *fill;
    GfxSprite *frame;
    GfxSprite *surface;
    s32 i;
    u8 slot;
    s32 id;
    float ratio;
    float empty;
    float angle;
    float sinv;
    float level;
    float timer;
    float glowStage;

    vtbl = (const VtblEntry *)unit->base.base.vtable;
    if (((int (*)(void *))vtbl[10].fn)((u8 *)unit + vtbl[10].delta) == 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        sprites = self->sprites;
        glow = sprites[33 + 2 * i];
        ring = sprites[34 + 2 * i];
        fill = sprites[194 + i];
        frame = sprites[111 + i];
        surface = sprites[114 + i];
        if (!(i < unit->combat.artSlotCount)) {
            GfxSpriteSetScaleRotation(glow, 1.0f, 1.0f, 0.0f, false);
            GfxSpriteSetScaleRotation(frame, 1.0f, 1.0f, 0.0f, false);
            continue;
        }
        slot = (u8)i;
        id = (slot < 3) ? unit->combat.listedStatusIds[slot] : unit->combat.listedStatusIds[0];
        if ((u32)(id - 6) < 5) {
            ratio = BtlCombatGetStatusRatio(&unit->combat, id);
        } else {
            ratio = ((slot < 3) ? unit->combat.artCharge[slot] : unit->combat.artCharge[0]) *
                    9.99999975e-05f;
        }
        empty = 1.0f - ratio;
        fill->posY = self->gaugeAnchor[i][0] - (1.0f - empty) * 32.0f;
        if (!(ratio < 1.0f) && self->gaugeRatio[i] < 1.0f) {
            if (SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x200132, 0, 0);
            }
            if (BtlBakuganIsLocalPlayer(unit) && SndHasManager()) {
                SndManagerPlay(SndGetManager(), 0x5100000, 0, 0);
            }
        }
        self->gaugeRatio[i] = ratio;
        if (!(ratio < 1.0f)) {
            angle = self->gaugeGlowAngle[i] + 0.279252678f;
            self->gaugeGlowAngle[i] = angle;
            if (!(angle < 6.28318548f)) {
                self->gaugeGlowAngle[i] = self->gaugeGlowAngle[i] - 6.28318548f;
            }
            if (UiGetWindowActive(0xb) == 1) {
                /* vsin.s of angle * S703 (2/pi): sine of the angle in radians */
                sinv = __builtin_sinf(self->gaugeGlowAngle[i]);
                glowStage = sinv * 0.2f + 0.4f;
                /* tint/alpha = yellow + (white - yellow) * glowStage */
                glow->tint[0] = g_colorYellow.x + (g_colorWhite.x - g_colorYellow.x) * glowStage;
                glow->tint[1] = g_colorYellow.y + (g_colorWhite.y - g_colorYellow.y) * glowStage;
                glow->tint[2] = g_colorYellow.z + (g_colorWhite.z - g_colorYellow.z) * glowStage;
                glow->alpha = g_colorYellow.w + (g_colorWhite.w - g_colorYellow.w) * glowStage;
                glow->alpha = glowStage;
            }
        } else {
            self->gaugeGlowAngle[i] = 0.0f;
            glow->alpha = 0.0f;
        }
        GfxSpriteSetScaleRotation(glow, 1.0f, 1.0f, 0.0f, false);
        GfxSpriteSetScaleRotation(ring, 0.380950004f, 0.380950004f, 0.0f, false);
        GfxSpriteSetScaleRotation(frame, 1.0f, 1.0f, 0.0f, false);
        BtlHudSetSpriteRect(0.0f, 0.0f, 48.0f, empty * 64.0f, self, fill);
        GfxSpriteSetScaleRotation(surface, 1.0f, 1.0f, 0.0f, false);
        surface->flags &= ~0x20u;
        if (empty <= 0.0f || !(empty < 1.0f)) {
            surface->flags &= ~1u;
            continue;
        }
        surface->flags |= 1u;
        level = self->gaugeAnchor[i][1] + empty * 59.0f;
        surface->posY = (float)(s32)level;
        if (!(self->gaugeFlipTimer[i] < 1.5f)) {
            self->gaugeFlipTimer[i] = self->gaugeFlipTimer[i] - 1.5f;
            GfxSpriteFlipU(surface);
            GfxSpriteFlipV(surface);
        }
        timer = self->gaugeFlipTimer[i];
        self->gaugeFlipTimer[i] = timer + 1.0f;
    }
}
