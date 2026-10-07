// bdc 0x088c4660 GameFieldPlayTransition
#include "bdc.h"

/* Field counter transition of the field (world map) scene task (`GameFieldTask`, called by
   `GameFieldPhaseMain`): a step machine in `g_fieldTransStep` / `g_fieldTransFrame` that
   shows the player's field counter (`SaveProfileData.fieldCounter`, capped at 99) as two digit
   pairs and rolls it down by one. Step 0 stops the alarm loop, recreates the location label
   `locationLabel` (layout 0x11 entry 0xe), the banner and the four digit sprites; step 1 zooms the
   label from (26,121) to (192,24) while fading everything in over 11 frames; step 2 runs a white
   flash (`GfxFaderStart` 8 then 5 frames); step 3 plays sound `0x2c00041` and rolls the digits
   that change; step 4 fades out over 5 frames and releases all six sprites from `spriteLayer`.
   Returns true while the step is below 5 (transition still running). */

bool GameFieldPlayTransition(CoreTask *task, char start)
{
    GameFieldTask *field = (GameFieldTask *)task;
    SaveProfile *profile;
    GfxFader *fader;
    s16 *record;
    s32 count;
    bool tensChange;
    bool onesChange;
    float t;
    float u;
    float a;

    profile = SaveGetProfile();
    count = profile->data->fieldCounter;
    if (!(count < 100)) {
        count = 99;
    }
    if (start != 0) {
        g_fieldTransStep = 0;
    }

    switch (g_fieldTransStep) {
    case 0:
        if (SndHasManager()) {
            SndManagerStop(SndGetManager(), field->alarmLoopHandle);
        }
        if (field->locationLabel != NULL) {
            UiSpriteLayerRelease(field->spriteLayer, field->locationLabel);
            field->locationLabel = NULL;
        }
        field->locationLabel = GameFieldCreateLayoutSprite(task, UiLayoutGetEntry(0x11, 0xe));
        if (g_fieldTransBanner != NULL) {
            UiSpriteLayerRelease(field->spriteLayer, g_fieldTransBanner);
            g_fieldTransBanner = NULL;
        }
        if (g_fieldTransTensCur != NULL) {
            UiSpriteLayerRelease(field->spriteLayer, g_fieldTransTensCur);
            g_fieldTransTensCur = NULL;
        }
        if (g_fieldTransOnesCur != NULL) {
            UiSpriteLayerRelease(field->spriteLayer, g_fieldTransOnesCur);
            g_fieldTransOnesCur = NULL;
        }
        if (g_fieldTransTensNext != NULL) {
            UiSpriteLayerRelease(field->spriteLayer, g_fieldTransTensNext);
            g_fieldTransTensNext = NULL;
        }
        if (g_fieldTransOnesNext != NULL) {
            UiSpriteLayerRelease(field->spriteLayer, g_fieldTransOnesNext);
            g_fieldTransOnesNext = NULL;
        }
        g_fieldTransBanner = GameFieldCreateLayoutSprite(task, UiLayoutGetEntry(0xc, 8));
        record = UiLayoutGetEntry(0xc, 9);
        g_fieldTransTensCur = GameFieldCreateLayoutSprite(task, record);
        g_fieldTransTensNext = GameFieldCreateLayoutSprite(task, record);
        record = UiLayoutGetEntry(0xc, 10);
        g_fieldTransOnesCur = GameFieldCreateLayoutSprite(task, record);
        g_fieldTransOnesNext = GameFieldCreateLayoutSprite(task, record);
        GameFieldSetNumberSprites(g_fieldTransTensCur, g_fieldTransOnesCur, count);
        GameFieldSetNumberSprites(g_fieldTransTensNext, g_fieldTransOnesNext, count - 1);
        field->locationLabel->posX = 26.0f;
        field->locationLabel->posY = 121.0f;
        GfxSpriteSetScaleRotation(field->locationLabel, 0.1f, 0.1f, 0.0f, false);
        g_fieldTransOnesCur->alpha = 0.0f;
        g_fieldTransTensCur->alpha = 0.0f;
        g_fieldTransBanner->alpha = 0.0f;
        field->locationLabel->alpha = 0.0f;
        g_fieldTransOnesNext->alpha = 0.0f;
        g_fieldTransTensNext->alpha = 0.0f;
        g_fieldTransBanner->posX = 288.0f;
        g_fieldTransBanner->posY = 130.0f;
        GfxSpriteSetScaleRotation(g_fieldTransBanner, 2.0f, 2.0f, 0.0f, false);
        GfxSpriteSetScaleRotation(g_fieldTransTensCur, 2.0f, 2.0f, 0.0f, false);
        GfxSpriteSetScaleRotation(g_fieldTransOnesCur, 2.0f, 2.0f, 0.0f, false);
        GfxSpriteSetScaleRotation(g_fieldTransTensNext, 2.0f, 2.0f, 0.0f, false);
        GfxSpriteSetScaleRotation(g_fieldTransOnesNext, 2.0f, 2.0f, 0.0f, false);
        g_fieldTransFrame = 0;
        g_fieldTransStep = g_fieldTransStep + 1;
        break;

    case 1:
        t = (float)g_fieldTransFrame * 0.1f + 0.1f;
        if (!(t <= 1.0f)) {
            t = 1.0f;
        }
        u = 1.0f - t;
        field->locationLabel->posX = u * 26.0f + t * 192.0f;
        field->locationLabel->posY = u * 121.0f + t * 24.0f;
        GfxSpriteSetScaleRotation(field->locationLabel, t, t, 0.0f, false);
        g_fieldTransOnesCur->alpha = t;
        g_fieldTransTensCur->alpha = t;
        g_fieldTransBanner->alpha = t;
        field->locationLabel->alpha = t;
        if (g_fieldTransFrame < 10) {
            g_fieldTransFrame = g_fieldTransFrame + 1;
        } else {
            field->locationLabel->posX = 192.0f;
            field->locationLabel->posY = 24.0f;
            GfxSpriteSetScaleRotation(field->locationLabel, 1.0f, 1.0f, 0.0f, false);
            field->locationLabel->alpha = 1.0f;
            g_fieldTransOnesCur->alpha = 1.0f;
            g_fieldTransTensCur->alpha = 1.0f;
            g_fieldTransBanner->alpha = 1.0f;
            g_fieldTransFrame = 0;
            g_fieldTransStep = g_fieldTransStep + 1;
        }
        break;

    case 2:
        if (g_fieldTransFrame == 4) {
            fader = GfxGetActiveFader();
            fader->start[0] = 0.0f;
            fader->start[1] = 0.0f;
            fader->start[2] = 0.0f;
            fader->start[3] = 0.0f;
            fader = GfxGetActiveFader();
            fader->end[0] = 1.0f;
            fader->end[1] = 1.0f;
            fader->end[2] = 1.0f;
            fader->end[3] = 1.0f;
            GfxFaderStart(GfxGetActiveFader(), 8);
        } else if (g_fieldTransFrame == 14) {
            fader = GfxGetActiveFader();
            fader->start[0] = 1.0f;
            fader->start[1] = 1.0f;
            fader->start[2] = 1.0f;
            fader->start[3] = 1.0f;
            fader = GfxGetActiveFader();
            fader->end[0] = 0.0f;
            fader->end[1] = 0.0f;
            fader->end[2] = 0.0f;
            fader->end[3] = 0.0f;
            GfxFaderStart(GfxGetActiveFader(), 5);
        }
        if (g_fieldTransFrame < 20) {
            g_fieldTransFrame = g_fieldTransFrame + 1;
        } else {
            g_fieldTransStep = g_fieldTransStep + 1;
            g_fieldTransFrame = 0;
        }
        break;

    case 3:
        onesChange = count != 0;
        tensChange = count % 10 == 0;
        if (g_fieldTransFrame == 6) {
            SndManagerPlay(SndGetManager(), 0x2c00041, 0, 0);
        }
        if (g_fieldTransFrame >= 6) {
            t = (float)(g_fieldTransFrame - 6) * 0.16666667f;
            if (!(t <= 1.0f)) {
                t = 1.0f;
            }
            if (tensChange) {
                g_fieldTransTensCur->posY = g_fieldTransTensCur->posY - 0.5f;
                g_fieldTransTensCur->alpha = 1.0f - t;
                g_fieldTransTensNext->alpha = t;
            }
            if (onesChange) {
                g_fieldTransOnesCur->posY = g_fieldTransOnesCur->posY - 0.5f;
                g_fieldTransOnesCur->alpha = 1.0f - t;
                g_fieldTransOnesNext->alpha = t;
            }
        }
        if (g_fieldTransFrame < 20) {
            g_fieldTransFrame = g_fieldTransFrame + 1;
        } else {
            g_fieldTransStep = g_fieldTransStep + 1;
            g_fieldTransFrame = 0;
        }
        break;

    case 4:
        t = 1.0f - (float)g_fieldTransFrame * 0.2f;
        if (t < 0.0f) {
            t = 0.0f;
        }
        a = g_fieldTransTensCur->alpha;
        g_fieldTransTensCur->alpha = (a < t) ? a : t;
        a = g_fieldTransOnesCur->alpha;
        g_fieldTransOnesCur->alpha = (a < t) ? a : t;
        a = g_fieldTransTensNext->alpha;
        g_fieldTransTensNext->alpha = (a < t) ? a : t;
        a = g_fieldTransOnesNext->alpha;
        g_fieldTransOnesNext->alpha = (a < t) ? a : t;
        field->locationLabel->alpha = t;
        g_fieldTransBanner->alpha = t;
        g_fieldTransFrame = (g_fieldTransFrame < 5) ? g_fieldTransFrame : 5;
        if (!(g_fieldTransFrame < 5)) {
            field->locationLabel->flags &= ~1u;
            g_fieldTransBanner->flags &= ~1u;
            g_fieldTransTensCur->flags &= ~1u;
            g_fieldTransOnesCur->flags &= ~1u;
            g_fieldTransTensNext->flags &= ~1u;
            g_fieldTransOnesNext->flags &= ~1u;
            UiSpriteLayerRelease(field->spriteLayer, field->locationLabel);
            field->locationLabel = NULL;
            UiSpriteLayerRelease(field->spriteLayer, g_fieldTransBanner);
            g_fieldTransBanner = NULL;
            UiSpriteLayerRelease(field->spriteLayer, g_fieldTransTensCur);
            g_fieldTransTensCur = NULL;
            UiSpriteLayerRelease(field->spriteLayer, g_fieldTransOnesCur);
            g_fieldTransOnesCur = NULL;
            UiSpriteLayerRelease(field->spriteLayer, g_fieldTransTensNext);
            g_fieldTransTensNext = NULL;
            UiSpriteLayerRelease(field->spriteLayer, g_fieldTransOnesNext);
            g_fieldTransOnesNext = NULL;
            g_fieldTransStep = g_fieldTransStep + 1;
        }
        g_fieldTransFrame = g_fieldTransFrame + 1;
        break;

    case 5:
    default:
        break;
    }
    return g_fieldTransStep < 5;
}
