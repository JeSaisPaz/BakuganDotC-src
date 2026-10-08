// bdc 0x088408dc BtlHudRatingResultScreen
#include "bdc.h"

/* Rated battle result screen (game mode 1), one step per frame from `BtlHudUpdateResultScreen`,
   state machine on `resultScreenState` (`resultScreenCount` count-up value / timer,
   `resultScreenAngle` fade angle). While the state is > 0 and no dialog is busy
   (`BtlIsDialogBusy`) a press of pad bit 0x4000 sets `ratingSkip`; every frame ends with
   `BtlResultScrollStripes`.
   - 0: creates the rating layer (0x80 bytes, 2D, sorted) and its 175-entry sprite table if
     missing, builds UI layout 0x2e, shifts / hides its sprites (language-dependent shift of sprites
     28..30), zeroes the alpha of sprites 0..92, resets the text printer `overlayObj[1]` (font 1,
     wrap 480, width scale 0.6) and prints the 8 localized result labels (`msgTable[0x38c..0x394]`
     via `msgText`) with a 4-way dark outline and an orange / red / blue fill, creates the scroll
     stripes (`BtlResultInitScrollStripes`), puts 12 sprites on texture slot 1
     (`g_btlRatingSlotSprites`), hides sprite 18 (and 19 when result item 0x1c is 0), sets the
     rank letters of sprites 70/71/73 (`"hyouka_moji_02_s/a/b/c"`, `BtlResultGetRank` 0..2) and
     the rank cell of sprite 5 (category 3); then goes on through 1 to 10.
   - 10/11: shows result items 16..30 (`BtlResultShowScoreItem`; 29 as 0, 30 as the profile
     points), then fades the header in (alpha sin(angle), angle += 10 deg, clamped to [0, pi/2],
     pi/2 when `ratingSkip`), stripes at 0.4x and sprites 82/85 at 0.5x, zooming the layer from
     0.8 to 1; at alpha 1 goes to 20.
   - 20/21: fades sprite 20 in (8 deg per frame), then 30/40/41 the rows (5 deg, sprites 7..14
     except 10 and 13, `g_btlRatingFadeSprites`, 22..63 and the printer layer).
   - 50/51: stops BGM channel 1, plays the result voice (`BtlHudPickResultVoice`) and fades
     sprites 3..5 in; 60/61 shows item 0x1d as 0 and item 0x1e as the profile points (both final
     when skipping) and fades sprites 13, 21, 64..69 in, then starts the count-up loop sound 9
     unless skipping or sound word 0x20012d is playing (`ratingSndHandle`, -1 for none).
   - 62: counts item 0x1d (battle score) up by 37 per frame; at the end (or when skipping) stops the
     loop sound, shows the final value; 63/64 wait 15 frames (0 when skipping).
   - 65/66: the same count-up for item 0x1e from the profile points to the new total; 70/80 wait
     1 frame (when skipping) or none, then stay at 80 until a pad press of bit 0x4000 with no
     dialog busy, which goes to 90 and clears `ratingSkip`.
   - 90/91: fades the rating layer and the printer out (10 deg per frame); at alpha <= 0 goes to 100.
   - 100: clears the printer, adds the battle score (`BtlResultGetBattleScore`) to the profile
     points (clamped to 0..9999999), frees `savedAlpha`, activates UI window 10
     (`UiSetWindowActive`), sets `ratingFinished` and goes to 999 (no-op state).
   Each fade is sin(angle) (the listing's `vsin.s` of angle * 2/pi, bank constant S703); each colour
   is a 16-byte copy of four floats into the printer's colour at +0xc0. */

#define RATING_HALF_PI    1.57079637f    /* 0x3fc90fdb */
#define RATING_FADE_STEP  0.174532920f   /* 0x3e32b8c2, 10 degrees */
#define RATING_LABEL_STEP 0.139626339f   /* 0x3e0efa35, 8 degrees */
#define RATING_ROW_STEP   0.0872664601f  /* 0x3db2b8c2, 5 degrees */
#define RATING_SND_LOOP   0x20012d       /* sound word of the count-up loop */
#define RATING_POINTS_MAX 9999999

/* sin(angle): the listing's vsin.s of angle * S703 (2/pi), a quarter-turn sine. */
static inline float RatingSin(float angle)
{
    return __builtin_sinf(angle);
}

/* Clamps the angle to [0, pi/2] (a NaN becomes pi/2). */
static inline float RatingClampAngle(float angle)
{
    if (angle < 0.0f) {
        return 0.0f;
    }
    if (!(angle <= RATING_HALF_PI)) {
        return RATING_HALF_PI;
    }
    return angle;
}

/* Sets the printer colour at +0xc0 (the listing copies the four floats with lv.q/sv.q). */
static inline void RatingSetPrinterColor(UiTextPrinter *printer, float r, float g, float b, float a)
{
    printer->outlineColor[0] = r;
    printer->outlineColor[1] = g;
    printer->outlineColor[2] = b;
    printer->outlineColor[3] = a;
}

/* Prints `text` at (x, y) through the printer's virtual print method (vtable slot 2). */
static void RatingPrint(UiTextPrinter *printer, char *text, s32 x, s32 y)
{
    const VtblEntry *print;

    print = &printer->layer.vtbl[2];
    ((void (*)(float, float, float, void *, char *, s32, s32, s32))print->fn)(
        (float)x, (float)y, 0.0f, (u8 *)printer + print->delta, text, 0, 0, 0);
}

/* Horizontal shift of the language-dependent labels for the current game language. */
static s32 RatingLangShift(void)
{
    switch (SaveProfileGetLanguage(SaveGetProfile())) {
    case 2:
        return 17;
    case 3:
    case 4:
        return 20;
    case 5:
        return 10;
    case 6:
    case 12:
        return 28;
    default:
        return 0;
    }
}

void BtlHudRatingResultScreen(BtlHud *self)
{
    s32 slotSprites[12];
    s32 rankSprites[3];
    s32 headerSprites[7];
    s32 rowSprites[11];
    s32 ids[3];
    UiTextPrinter *printer;
    GfxSpriteLayer *layer;
    GfxSprite **sprites;
    GfxSprite *sprite;
    SaveProfile *profile;
    SndManager *mgr;
    float *saved;
    bool fromLow;
    float t;
    s32 shift;
    s32 score;
    s32 target;
    s32 total;
    s32 voice;
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
    int i;
    int k;

    if (self->resultScreenState > 0 && !BtlIsDialogBusy() && (self->pad->pressed & 0x4000) != 0) {
        self->ratingSkip = 1;
    }

    switch (self->resultScreenState) {
    case 0:
        if (self->ratingLayer == NULL) {
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            layer = MemAlloc(sizeof(GfxSpriteLayer), NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            if (layer != NULL) {
                GfxSpriteLayerCtor(layer, 0);
            }
            self->ratingLayer = layer;
        }
        self->ratingLayer->sorted = 1;
        if (self->ratingSprites == NULL) {
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            sprites = MemAlloc(175 * sizeof(GfxSprite *), NULL, 0); /* 0x2bc bytes */
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            self->ratingSprites = sprites;
        }
        UiLayoutCreateSprites(self->ratingLayer, self->ratingSprites, 0x2e);
        GfxSpriteSetCell(self->ratingSprites[86], 0.0f, 2.0f);
        self->ratingSprites[2]->posZ = self->ratingSprites[2]->posZ + 16.0f;
        self->ratingSprites[1]->posZ = self->ratingSprites[1]->posZ + 16.0f;
        for (i = 0; i < 93; i++) {
            self->ratingSprites[i]->alpha = 0.0f;
        }
        for (i = 83; i < 85; i++) {
            self->ratingSprites[i]->flags &= ~1u;
        }
        self->ratingSprites[4]->posY = self->ratingSprites[4]->posY + 2.0f;
        self->ratingSprites[5]->posY = self->ratingSprites[5]->posY + 2.0f;
        for (i = 19; i < 20; i++) {
            self->ratingSprites[i]->posX = self->ratingSprites[i]->posX + 38.0f;
        }
        for (i = 70; i < 74; i++) {
            self->ratingSprites[i]->posX = self->ratingSprites[i]->posX + 16.0f;
        }
        for (i = 46; i < 49; i++) {
            self->ratingSprites[i]->flags &= ~1u;
        }
        for (i = 49; i < 55; i++) {
            self->ratingSprites[i]->flags &= ~1u;
        }
        for (i = 22; i < 64; i++) {
            self->ratingSprites[i]->posX = self->ratingSprites[i]->posX - 18.0f;
        }
        for (i = 70; i < 74; i++) {
            self->ratingSprites[i]->posX = self->ratingSprites[i]->posX - 18.0f;
        }
        shift = RatingLangShift();
        for (i = 28; i < 31; i++) {
            self->ratingSprites[i]->posX = self->ratingSprites[i]->posX - (float)shift;
        }
        for (i = 14; i < 20; i++) {
            self->ratingSprites[i]->posX = self->ratingSprites[i]->posX - 18.0f;
            if (i == 14) {
                self->ratingSprites[i]->flags &= ~1u;
            }
        }
        for (i = 88; i < 93; i++) {
            self->ratingSprites[i]->posX = self->ratingSprites[i]->posX + 4.0f;
            BtlHudSetSpriteRect(0.0f, 0.0f, 328.0f, 16.0f, self, self->ratingSprites[i]);
        }
        for (i = 7; i < 14; i++) {
            self->ratingSprites[i]->posX = self->ratingSprites[i]->posX + 8.0f;
            self->ratingSprites[i]->flags &= ~1u;
        }
        for (i = 37; i < 40; i++) {
            self->ratingSprites[i]->posX = self->ratingSprites[i]->posX + 16.0f;
        }
        for (i = 55; i < 58; i++) {
            self->ratingSprites[i]->posX = self->ratingSprites[i]->posX + 16.0f;
        }
        for (i = 28; i < 31; i++) {
            self->ratingSprites[i]->posX = self->ratingSprites[i]->posX + 10.0f;
        }
        score = (s32)BtlResultGetScoreItem(self, 0x1c);
        if (score >= 1000) {
            self->ratingSprites[19]->posX = self->ratingSprites[19]->posX - 20.0f;
        } else if (score >= 100) {
            self->ratingSprites[19]->posX = self->ratingSprites[19]->posX - 10.0f;
        }
        self->ratingSprites[17]->posX = self->ratingSprites[17]->posX + 16.0f;
        self->ratingSprites[16]->posX = self->ratingSprites[16]->posX + 16.0f;

        /* Text printer: cleared, font 1, wrap width 480, width scale 0.6. */
        self->overlayDepth = 130.0f;
        printer = self->overlayObj[1];
        self->talkTextAlpha = 0.0f;
        printer->layer.alpha = 0.0f;
        printer = self->overlayObj[0];
        GfxSpriteLayerClear(&printer->layer);
        printer->glyphs = NULL;
        printer = self->overlayObj[1];
        GfxSpriteLayerClear(&printer->layer);
        printer->glyphs = NULL;
        printer = self->overlayObj[1];
        printer->wrapWidth = 480.0f;
        UiTextPrinterSetFont(self->overlayObj[1], 1);
        printer = self->overlayObj[1];
        printer->widthScale = 0.6f;

        /* The 8 result labels, each with a 4-way outline. */
        x = 0;
        y = 0;
        if (self->msgTable != NULL) {
            for (i = 0; i < 8; i++) {
                switch (i) {
                case 0:
                    strcpy(self->msgText, (const char *)PspPtr(self->msgTable[0x393]));
                    x = (s32)self->ratingSprites[7]->posX;
                    y = (s32)(self->ratingSprites[7]->posY - 16.0f);
                    break;
                case 1:
                    strcpy(self->msgText, (const char *)PspPtr(self->msgTable[0x390]));
                    x = (s32)self->ratingSprites[7]->posX;
                    y = (s32)self->ratingSprites[7]->posY;
                    break;
                case 2:
                    strcpy(self->msgText, (const char *)PspPtr(self->msgTable[0x38c]));
                    x = (s32)self->ratingSprites[8]->posX;
                    y = (s32)self->ratingSprites[8]->posY;
                    break;
                case 3:
                    strcpy(self->msgText, (const char *)PspPtr(self->msgTable[0x38d]));
                    x = (s32)self->ratingSprites[9]->posX;
                    y = (s32)self->ratingSprites[9]->posY;
                    break;
                case 4:
                    strcpy(self->msgText, (const char *)PspPtr(self->msgTable[0x394]));
                    x = (s32)self->ratingSprites[10]->posX;
                    y = (s32)(self->ratingSprites[10]->posY + 2.0f);
                    break;
                case 5:
                    strcpy(self->msgText, (const char *)PspPtr(self->msgTable[0x38f]));
                    x = (s32)self->ratingSprites[11]->posX;
                    y = (s32)self->ratingSprites[11]->posY;
                    break;
                case 6:
                    strcpy(self->msgText, (const char *)PspPtr(self->msgTable[0x391]));
                    x = (s32)self->ratingSprites[13]->posX;
                    y = (s32)self->ratingSprites[13]->posY;
                    break;
                case 7:
                    strcpy(self->msgText, (const char *)PspPtr(self->msgTable[0x392]));
                    y = (s32)self->ratingSprites[14]->posY;
                    x = (s32)(self->ratingSprites[14]->posX + 15.0f);
                    x -= RatingLangShift();
                    break;
                }
                RatingSetPrinterColor(self->overlayObj[1], 0.1f, 0.1f, 0.1f, 1.0f);
                /* The listing passes +0x8c8 as the 4th and +0x8c4 as the 5th argument. */
                UiTextMeasure(0.0f, self->overlayObj[1], self->msgText, &self->talkTextHeight,
                              &self->talkTextWidth, NULL);
                for (k = 0; k < 4; k++) {
                    dx = 0;
                    dy = 0;
                    switch (k) {
                    case 0:
                        dx = 1;
                        dy = 1;
                        break;
                    case 1:
                        dx = 1;
                        dy = -1;
                        break;
                    case 2:
                        dx = -1;
                        dy = -1;
                        break;
                    case 3:
                        dx = -1;
                        dy = 1;
                        break;
                    }
                    RatingPrint(self->overlayObj[1], self->msgText, dx + x, dy + y - 1);
                }
                if (i == 0 || i == 4) {
                    RatingSetPrinterColor(self->overlayObj[1], 1.0f, 0.4f, 0.2f, 1.0f);
                } else if (i == 5) {
                    RatingSetPrinterColor(self->overlayObj[1], 1.0f, 0.196f, 0.196f, 1.0f);
                } else {
                    RatingSetPrinterColor(self->overlayObj[1], 0.4f, 0.8f, 1.0f, 1.0f);
                }
                RatingPrint(self->overlayObj[1], self->msgText, x, y - 1);
            }
        }

        BtlResultInitScrollStripes(self);
        memcpy(slotSprites, g_btlRatingSlotSprites, sizeof(slotSprites));
        for (i = 0; i < 12; i++) {
            self->ratingSprites[slotSprites[i]]->textureSlot = 1;
        }
        if (BtlResultGetScoreItem(self, 0x1c) == 0) {
            self->ratingSprites[18]->flags &= ~1u;
            self->ratingSprites[19]->flags &= ~1u;
        } else {
            self->ratingSprites[18]->flags &= ~1u;
        }

        /* Rank letters of categories 0..2. */
        rankSprites[0] = 70;
        rankSprites[1] = 71;
        rankSprites[2] = 73;
        for (i = 0; i < 3; i++) {
            sprite = self->ratingSprites[rankSprites[i]];
            switch (BtlResultGetRank(self, i)) {
            case 0:
                sprite->texture = GfxFindTexture("hyouka_moji_02_s");
                break;
            case 1:
                sprite->texture = GfxFindTexture("hyouka_moji_02_a");
                break;
            case 2:
                sprite->texture = GfxFindTexture("hyouka_moji_02_b");
                break;
            case 3:
                sprite->texture = GfxFindTexture("hyouka_moji_02_c");
                break;
            }
        }
        sprite = self->ratingSprites[5];
        GfxSpriteSetVCell((float)BtlResultGetRank(self, 3), sprite);
        self->resultScreenState++;
        /* fallthrough */
    case 1:
        self->resultScreenState = 10;
        /* fallthrough */
    case 10:
        for (i = 16; i < 31; i++) {
            switch (i) {
            case 29:
                BtlResultShowScoreItem(self, i, 0);
                break;
            case 30:
                if (SaveHasProfile()) {
                    BtlResultShowScoreItem(self, i, SaveGetProfile()->data->points);
                }
                break;
            default:
                BtlResultShowScoreItem(self, i, -999);
                break;
            }
        }
        self->resultScreenCount = 0;
        self->resultScreenAngle = self->ratingSkip ? RATING_HALF_PI : 0.0f;
        self->resultScreenState++;
        /* fallthrough */
    case 11:
        self->resultScreenAngle = self->resultScreenAngle + RATING_FADE_STEP;
        if (self->ratingSkip) {
            self->resultScreenAngle = RATING_HALF_PI;
        }
        self->resultScreenAngle = RatingClampAngle(self->resultScreenAngle);
        t = RatingSin(self->resultScreenAngle);
        headerSprites[0] = 0;
        headerSprites[1] = 1;
        headerSprites[2] = 2;
        headerSprites[3] = 74;
        headerSprites[4] = 6;
        headerSprites[5] = 86;
        headerSprites[6] = 87;
        for (i = 0; i < 7; i++) {
            self->ratingSprites[headerSprites[i]]->alpha = t;
        }
        for (i = 0; i < 41; i++) {
            self->ratingSprites[93 + i]->alpha = t * 0.4f;
            self->ratingSprites[134 + i]->alpha = t * 0.4f;
        }
        self->ratingSprites[85]->alpha = t * 0.5f;
        self->ratingSprites[82]->alpha = t * 0.5f;
        for (i = 75; i < 82; i++) {
            self->ratingSprites[i]->alpha = t;
        }
        GfxSpriteLayerSetZoom(t * 0.2f + 0.8f, 0.0f, self->ratingLayer, NULL);
        if (t < 1.0f) {
            break;
        }
        GfxSpriteLayerSetZoom(1.0f, 0.0f, self->ratingLayer, NULL);
        self->resultScreenState = 20;
        /* fallthrough */
    case 20:
        self->resultScreenAngle = self->ratingSkip ? RATING_HALF_PI : 0.0f;
        self->resultScreenState++;
        /* fallthrough */
    case 21:
        self->resultScreenAngle = self->resultScreenAngle + RATING_LABEL_STEP;
        if (self->ratingSkip) {
            self->resultScreenAngle = RATING_HALF_PI;
        }
        self->resultScreenAngle = RatingClampAngle(self->resultScreenAngle);
        t = RatingSin(self->resultScreenAngle);
        self->ratingSprites[20]->alpha = t;
        if (t < 1.0f) {
            break;
        }
        self->resultScreenState = 30;
        /* fallthrough */
    case 30:
        self->resultScreenAngle = self->ratingSkip ? RATING_HALF_PI : 0.0f;
        self->resultScreenState = 40;
        /* fallthrough */
    case 40:
        self->resultScreenState++;
        /* fallthrough */
    case 41:
        self->resultScreenAngle = self->resultScreenAngle + RATING_ROW_STEP;
        if (self->ratingSkip) {
            self->resultScreenAngle = RATING_HALF_PI;
        }
        self->resultScreenAngle = RatingClampAngle(self->resultScreenAngle);
        t = RatingSin(self->resultScreenAngle);
        for (i = 7; i < 15; i++) {
            if (i != 13 && i != 10) {
                self->ratingSprites[i]->alpha = t;
            }
        }
        printer = self->overlayObj[1];
        printer->layer.alpha = t;
        memcpy(rowSprites, g_btlRatingFadeSprites, sizeof(rowSprites));
        for (i = 0; i < 11; i++) {
            self->ratingSprites[rowSprites[i]]->alpha = t;
        }
        for (i = 22; i < 64; i++) {
            self->ratingSprites[i]->alpha = t;
        }
        if (t < 1.0f) {
            break;
        }
        self->resultScreenState = 50;
        /* fallthrough */
    case 50:
        SndBgmQueueStop(0.0f, 1);
        voice = BtlHudPickResultVoice(self);
        if (voice != -1) {
            SndBgmQueuePlay(1, voice, 0, 0);
        }
        self->resultScreenAngle = self->ratingSkip ? RATING_HALF_PI : 0.0f;
        self->resultScreenState++;
        /* fallthrough */
    case 51:
        self->resultScreenAngle = self->resultScreenAngle + RATING_ROW_STEP;
        if (self->ratingSkip) {
            self->resultScreenAngle = RATING_HALF_PI;
        }
        self->resultScreenAngle = RatingClampAngle(self->resultScreenAngle);
        t = RatingSin(self->resultScreenAngle);
        ids[0] = 3;
        ids[1] = 4;
        ids[2] = 5;
        for (i = 0; i < 3; i++) {
            self->ratingSprites[ids[i]]->alpha = t;
        }
        if (t < 1.0f) {
            break;
        }
        self->resultScreenState = 60;
        /* fallthrough */
    case 60:
        if (self->ratingSkip) {
            self->resultScreenCount = (s32)BtlResultGetScoreItem(self, 0x1d);
            BtlResultShowScoreItem(self, 0x1d, (s32)BtlResultGetScoreItem(self, 0x1d));
            BtlResultShowScoreItem(self, 0x1e, (s32)BtlResultGetScoreItem(self, 0x1e));
            self->resultScreenAngle = RATING_HALF_PI;
        } else {
            self->resultScreenCount = 0;
            BtlResultShowScoreItem(self, 0x1d, 0);
            BtlResultShowScoreItem(self, 0x1e, SaveGetProfile()->data->points);
            self->resultScreenAngle = 0.0f;
        }
        self->resultScreenState++;
        /* fallthrough */
    case 61:
        self->resultScreenAngle = self->resultScreenAngle + RATING_ROW_STEP;
        if (self->ratingSkip) {
            self->resultScreenAngle = RATING_HALF_PI;
        }
        self->resultScreenAngle = RatingClampAngle(self->resultScreenAngle);
        t = RatingSin(self->resultScreenAngle);
        ids[0] = 13;
        ids[1] = 21;
        for (i = 0; i < 2; i++) {
            self->ratingSprites[ids[i]]->alpha = t;
        }
        for (i = 64; i < 70; i++) {
            self->ratingSprites[i]->alpha = t;
        }
        if (t < 1.0f) {
            break;
        }
        if (!self->ratingSkip) {
            self->ratingSndHandle = -1;
            if (!SndManagerIsSoundWordPlaying(SndGetManager(), RATING_SND_LOOP)) {
                self->ratingSndHandle = (s32)SndManagerPlay(SndGetManager(), 9, 0, 0);
            }
        }
        self->resultScreenState++;
        /* fallthrough */
    case 62:
        target = (s32)BtlResultGetScoreItem(self, 0x1d);
        self->resultScreenCount = self->resultScreenCount + 37;
        if (self->resultScreenCount < target && !self->ratingSkip) {
            BtlResultShowScoreItem(self, 0x1d, self->resultScreenCount);
            break;
        }
        if (self->ratingSndHandle != -1 && SndHasManager()) {
            mgr = SndGetManager();
            SndManagerStop(mgr, self->ratingSndHandle);
        }
        self->resultScreenState++;
        BtlResultShowScoreItem(self, 0x1d, target);
        /* fallthrough */
    case 63:
        self->resultScreenCount = 15;
        if (self->ratingSkip) {
            self->resultScreenCount = 0;
        }
        self->resultScreenState++;
        /* fallthrough */
    case 64:
        self->resultScreenCount = self->resultScreenCount - 1;
        if (self->resultScreenCount >= 0) {
            break;
        }
        self->resultScreenState++;
        /* fallthrough */
    case 65:
        if (self->ratingSkip) {
            self->resultScreenCount = (s32)BtlResultGetScoreItem(self, 0x1e);
            self->resultScreenAngle = RATING_HALF_PI;
        } else {
            profile = SaveGetProfile();
            self->resultScreenAngle = 0.0f;
            self->resultScreenCount = profile->data->points;
            self->ratingSndHandle = -1;
            if (!SndManagerIsSoundWordPlaying(SndGetManager(), RATING_SND_LOOP)) {
                self->ratingSndHandle = (s32)SndManagerPlay(SndGetManager(), 9, 0, 0);
            }
        }
        self->resultScreenState++;
        /* fallthrough */
    case 66:
        target = (s32)BtlResultGetScoreItem(self, 0x1e);
        self->resultScreenCount = self->resultScreenCount + 37;
        if (self->resultScreenCount < target && !self->ratingSkip) {
            BtlResultShowScoreItem(self, 0x1e, self->resultScreenCount);
            break;
        }
        if (self->ratingSndHandle != -1 && SndHasManager()) {
            mgr = SndGetManager();
            SndManagerStop(mgr, self->ratingSndHandle);
        }
        self->resultScreenState = 70;
        BtlResultShowScoreItem(self, 0x1e, target);
        /* fallthrough */
    case 70:
        self->resultScreenCount = 0;
        if (self->ratingSkip) {
            self->resultScreenCount = 1;
        }
        self->resultScreenState = 80;
        /* fallthrough */
    case 80:
        self->resultScreenCount = self->resultScreenCount - 1;
        if (self->resultScreenCount >= 0) {
            break;
        }
        self->resultScreenCount = -1;
        if (!BtlIsDialogBusy() && (self->pad->pressed & 0x4000) != 0) {
            self->resultScreenState = 90;
            self->ratingSkip = 0;
        }
        break;
    case 90:
        self->resultScreenAngle = RATING_HALF_PI;
        self->resultScreenState++;
        /* fallthrough */
    case 91:
        self->resultScreenAngle = self->resultScreenAngle + -RATING_FADE_STEP;
        self->resultScreenAngle = RatingClampAngle(self->resultScreenAngle);
        t = RatingSin(self->resultScreenAngle);
        self->ratingLayer->alpha = t;
        printer = self->overlayObj[1];
        printer->layer.alpha = t;
        if (!(t <= 0.0f)) {
            break;
        }
        self->ratingLayer->alpha = 0.0f;
        self->resultScreenState = 100;
        /* fallthrough */
    case 100:
        printer = self->overlayObj[1];
        GfxSpriteLayerClear(&printer->layer);
        printer->glyphs = NULL;
        profile = SaveGetProfile();
        score = BtlResultGetBattleScore(self);
        total = profile->data->points + score;
        if (total > RATING_POINTS_MAX) {
            total = RATING_POINTS_MAX;
        } else if (total < 0) {
            total = 0;
        }
        profile->data->points = total;
        if (self->savedAlpha != NULL) {
            saved = self->savedAlpha;
            MemLock();
            MemFree(saved, NULL, 0);
            MemUnlock();
            self->savedAlpha = NULL;
        }
        UiSetWindowActive(10, 1);
        self->ratingFinished = 1;
        self->resultScreenState = 999;
        break;
    default:
        break;
    }
    BtlResultScrollStripes(self);
}
