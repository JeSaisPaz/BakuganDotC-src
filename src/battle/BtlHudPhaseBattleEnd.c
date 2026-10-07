// bdc 0x08844728 BtlHudPhaseBattleEnd
#include "bdc.h"

/* HUD phase 3 (battle end). Step 0 creates the 0x80-byte result layer `resultLayer`
   (`GfxSpriteLayerCtor`, 2D) if missing and marks it sorted, allocates its 5-entry sprite table
   `resultSprites` if missing, fills it from UI layout 3 (`UiLayoutCreateSprites`) with every
   sprite hidden and sprite 2 transparent, allocates `savedAlpha` (`savedAlphaCount` floats) if
   missing, sets `hudAlpha` = 1, saves the alpha of the first `savedAlphaCount - 2` HUD sprites and
   moves to step 1 in the same call. Step 1 runs `BtlHudUpdateBattleEndBanner` while task 0x14a
   exists, lowers `hudAlpha` by 0.1 (clamped at 0), scales each saved sprite alpha by it (in a
   multi-round battle of at least 3 rounds that is still undecided, the sprites listed in
   `g_btlHudRoundKeepSprites` keep their alpha), sets the four text objects' colour to
   (0, 0, 0, hudAlpha) with a quad copy and the HP gauges' alpha
   (`UiHpGaugeSetAllAlpha`). Once `hudAlpha` reaches 0 (or for any other step) it frees
   `savedAlpha` and switches to phase 4 with step and result slide/timer reset. */
void BtlHudPhaseBattleEnd(BtlHud *self)
{
    float color[4] __attribute__((aligned(16)));
    s32 keep[17];
    GfxSpriteLayer *layer;
    GfxSprite **sprites;
    float *saved;
    GfxFab *fab;
    bool fromLow;
    bool done;
    bool kept;
    s32 size;
    s32 rounds;
    s32 i;
    s32 k;

    if (self->phaseStep < 0 || self->phaseStep > 1) {
        goto finish;
    }
    if (self->phaseStep == 0) {
        if (self->resultLayer == NULL) {
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            layer = MemAlloc(0x80, NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            if (layer != NULL) {
                GfxSpriteLayerCtor(layer, 0);
            }
            self->resultLayer = layer;
        }
        self->resultLayer->sorted = 1;
        if (self->resultSprites == NULL) {
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            sprites = MemAlloc(5 * sizeof(GfxSprite *), NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            self->resultSprites = sprites;
        }
        UiLayoutCreateSprites(self->resultLayer, self->resultSprites, 3);
        for (i = 0; i < 5; i++) {
            self->resultSprites[i]->flags &= ~1u;
        }
        self->resultSprites[2]->alpha = 0.0f;
        if (self->savedAlpha == NULL) {
            size = self->savedAlphaCount * 4;
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            saved = MemAlloc(size, NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            self->savedAlpha = saved;
        }
        self->hudAlpha = 1.0f;
        for (i = 0; i < self->savedAlphaCount - 2; i++) {
            if (self->sprites[i] != NULL) {
                self->savedAlpha[i] = self->sprites[i]->alpha;
            }
        }
        self->resultSlideX = 0;
        self->resultTimer = 0;
        self->phaseStep++;
    }

    done = false;
    if (CoreTaskExists(0x14a)) {
        BtlHudUpdateBattleEndBanner(self);
    }
    self->hudAlpha = self->hudAlpha - 0.1f;
    if (self->hudAlpha <= 0.0f) {
        self->hudAlpha = 0.0f;
        done = true;
    }
    memcpy(keep, g_btlHudRoundKeepSprites, sizeof(keep));
    rounds = (s32)SaveProfileGetWord(SaveGetProfile(), 0x1b);
    if (rounds < 1) {
        rounds = 1;
    } else if (rounds > 5) {
        rounds = 5;
    }
    for (i = 0; i < self->savedAlphaCount - 2; i++) {
        if (BtlCameraTaskExists() && g_scriptGlobalVars[8] == 2 && rounds >= 3 &&
            BtlMainIsMatchUndecided(BtlGetCameraTask())) {
            kept = false;
            for (k = 0; k < 17; k++) {
                if (i == keep[k]) {
                    kept = true;
                    break;
                }
            }
            if (kept) {
                continue;
            }
        }
        if (self->sprites[i] != NULL) {
            self->sprites[i]->alpha = self->savedAlpha[i] * self->hudAlpha;
        }
    }
    for (i = 0; i < 4; i++) {
        if (self->fabs[i] != NULL) {
            fab = (GfxFab *)self->fabs[i];
            color[0] = 0.0f;
            color[1] = 0.0f;
            color[2] = 0.0f;
            color[3] = self->hudAlpha;
            fab->color[0] = color[0];
            fab->color[1] = color[1];
            fab->color[2] = color[2];
            fab->color[3] = color[3];
        }
    }
    UiHpGaugeSetAllAlpha(self->hudAlpha);
    if (!done) {
        return;
    }
    self->phaseStep++;

finish:
    if (self->savedAlpha != NULL) {
        saved = self->savedAlpha;
        MemLock();
        MemFree(saved, NULL, 0);
        MemUnlock();
        self->savedAlpha = NULL;
    }
    self->phase = 4;
    self->resultSlideX = 0;
    self->resultTimer = 0;
    self->phaseStep = 0;
}
