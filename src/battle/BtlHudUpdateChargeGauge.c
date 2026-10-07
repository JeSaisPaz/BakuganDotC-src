// bdc 0x08830050 BtlHudUpdateChargeGauge
#include "bdc.h"

/* Charge gauge widget of the battle HUD (`BtlHudUpdate`) (`BtlHudPhaseMain`), driven by the
   player Bakugan's charge level (`charge`, 0..100, truncated to an int) and `chargeStep`: while
   charging it advances `.fab` animation 1 proportionally (`chargeFabBase + level × chargeFabRatio`)
   and lerps its colour from white to (1.0, 0.1, 0.2, 1.0) (`MathVec4Lerp`), starting the charge
   loop sound 0x200130 from level 15 (unless 0x20012d is playing); at 100 it plays 0x20012e, spawns
   effect 0x83 on the Bakugan (`GfxEffectSpawnAttached`), flashes it in its attribute colour
   (`BtlGetAttributeColor`, `BtlBakuganStartColorFlash`) and shows the 'full' animation (fab 2,
   then the looping fab 3); when the level drops back to 0 it resets the animations and stops the
   loop sound. */

void BtlHudUpdateChargeGauge(BtlHud *self)
{
    BtlBakugan *bakugan;
    s32 prevLevel;
    s32 level;
    s32 timer;
    s32 i;
    u32 frame;
    const VtblEntry *entry;
    int attribute;
    float *flashColor;
    GfxFab *fab;
    ScePspFVector4 lerped;
    ScePspFVector4 color;
    ScePspFVector4 target;

    bakugan = BtlHudGetPlayerBakugan(self);
    if (bakugan == NULL) {
        return;
    }
    prevLevel = self->chargePrevLevel;
    level = (s32)bakugan->charge;
    self->chargePrevLevel = level;

    if (prevLevel != 0 && level == 0) {
        self->chargeStep = 0;
        BtlHudFabSeek(self, 2, 1);
        BtlHudFabSetAlpha(0.0f, self, 2);
        BtlHudFabSetAlpha(0.0f, self, 3);
        if (SndHasManager()) {
            SndManagerStop(SndGetManager(), (s32)self->chargeSndHandle);
        }
        self->chargeFullTimer = 0;
        self->chargeSndStarted = 0;
        self->chargeFlagAe3 = 0;
        return;
    }
    if (level == 0) {
        return;
    }

    if (level >= 15 && self->chargeSndStarted == 0) {
        if (SndManagerIsSoundWordPlaying(SndGetManager(), 0x20012d) == 0) {
            self->chargeSndHandle = SndManagerPlay(SndGetManager(), 0x200130, 0, 0);
        }
        self->chargeSndStarted = 1;
    }

    switch (self->chargeStep) {
    case 0:
        BtlHudFabSetAlpha(1.0f, self, 1);
        BtlHudFabSeek(self, 0, 2);
        BtlHudFabSetAlpha(0.0f, self, 2);
        BtlHudFabSeek(self, 0, 3);
        BtlHudFabSetAlpha(0.0f, self, 3);
        self->chargeStep++;
        /* fall through */
    case 1:
        if (bakugan->charge < 100.0f) {
            GfxFabAdvanceTo(self->fabs[1],
                            (s32)((float)level * self->chargeFabRatio + (float)self->chargeFabBase));
            color.x = 1.0f;
            color.y = 1.0f;
            color.z = 1.0f;
            color.w = 1.0f;
            target.x = 1.0f;
            target.y = 0.100000001f;
            target.z = 0.200000003f;
            target.w = 1.0f;
            MathVec4Lerp(&color, &lerped, &target, (float)level * 0.00999999978f);
            fab = self->fabs[1];
            fab->color[0] = lerped.x;
            fab->color[1] = lerped.y;
            fab->color[2] = lerped.z;
            fab->color[3] = lerped.w;
            return;
        }
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0x20012e, 0, 0);
        }
        GfxEffectSpawnAttached(g_worldEffectMgr, 0x83, bakugan->base.pos);
        entry = &((const VtblEntry *)bakugan->base.base.vtable)[20];
        attribute = ((int (*)(void *))entry->fn)((u8 *)bakugan + entry->delta);
        flashColor = BtlGetAttributeColor(attribute);
        BtlBakuganStartColorFlash(1.0f, bakugan, flashColor);
        GfxFabAdvanceTo(self->fabs[1], (u32)self->chargeFabFull);
        GfxFabUpdate(self->fabs[1]);
        BtlHudFabSetAlpha(1.0f, self, 2);
        self->chargeStep++;
        return;
    case 2:
        timer = self->chargeFullTimer;
        self->chargeFullTimer = timer + 1;
        if (timer == 6 && SndHasManager()) {
            SndManagerStop(SndGetManager(), (s32)self->chargeSndHandle);
        }
        frame = GfxFabGetFrame(self->fabs[2]);
        for (i = 0; i < 2; i++) {
            GfxFabUpdate(self->fabs[2]);
            if (self->chargeFullEndFrame < (s32)frame) {
                BtlHudFabSetAlpha(0.0f, self, 2);
                BtlHudFabSetAlpha(1.0f, self, 3);
                self->chargeStep++;
                return;
            }
        }
        return;
    case 3:
        GfxFabUpdate(self->fabs[3]);
        GfxFabUpdate(self->fabs[3]);
        return;
    default:
        return;
    }
}
