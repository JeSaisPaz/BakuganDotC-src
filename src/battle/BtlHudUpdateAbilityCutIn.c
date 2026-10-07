// bdc 0x0883147c BtlHudUpdateAbilityCutIn
#include "bdc.h"

/* HUD widget: runs the ability cut-in state machine `cutInState` for `unit` (nothing when the
   state is 0 or `unit` is NULL). When profile flag 0 is set and the profile has any of the flags
   0x4880 (tutorial), a running cut-in is cut short: its four sprites are hidden and the state
   jumps to 100. States: 1 resets the colours, alpha, timers and `cutInFlag`, then (with 2, a
   `resultSlideX` countdown) 3 decompresses `"abi_baku_%03d.lzs"` (`(selectedArt & 0xfffc) + 1`)
   into `cutInPixelsA` and `"card_L_%03d.lzs"` (`selectedArt + 1`) into `cutInPixelsB`
   (`CorePackChainFind`, `CoreLzssDecompress`); 4 pauses the battle main task unless profile
   flag 0 is set, plays sound `0x200131` and the unit's voice bank (`voiceBank << 20`), builds the
   textures `cutInTexA`/`cutInTexB` (`GfxTextureCtor`) and puts them on the portrait
   (`sprites[0x67]`, 256x272) and card (`sprites[0xe7]`, 128x176) sprites, then goes to 10;
   10-11 slide the portrait (from 192 px left) and the card (from 272 px below) to their rest
   positions `cutInPortraitPos`/`cutInCardPos` while fading in `cutInAlpha` = sin over 10 frames,
   with the band (`sprites[0x66]`, alpha x3) and the two counter-rotating glows (`sprites[0x65]`,
   `sprites[0xe8]`, alpha x0.5); 12-13 hold for 20 frames with a pulsing band; 20-21 fade
   everything out over 15 frames while growing the portrait and sliding the card down; with
   `cutInFlag` set the scene dim colour of the battle main task fades to white from frame 7, and
   once the portrait alpha reaches 0 the state goes 30 -> 50 (51 finishes the white-out, then 100);
   otherwise 30 -> 31 hides the sprites and 32 frees the textures (`UiTalkFreeCutInTextures`),
   then 100. State 100 resumes the main task and resets the state to 0. Other states do nothing.
   The VFPU bank constants S703 (2/pi: `vsin` of radians) and S701 (255, colour packing) are literals. */

/* Packs the float RGBA quad `c` to RGBA8 into `dst` (lane 0 first): each lane saturated to [0, 1],
   scaled by 255 and truncated (vsat0 / vscl by the bank's 255 / vf2iz 23 / vi2uc). */
static inline void CutInPackColor(u8 *dst, const ScePspFVector4 *c)
{
    dst[0] = VfI2uc(VfF2iz(VfSat0(c->x) * 255.0f, 23));
    dst[1] = VfI2uc(VfF2iz(VfSat0(c->y) * 255.0f, 23));
    dst[2] = VfI2uc(VfF2iz(VfSat0(c->z) * 255.0f, 23));
    dst[3] = VfI2uc(VfF2iz(VfSat0(c->w) * 255.0f, 23));
}

/* Moves the sprite position a fraction `k` of the way to `target`: pos += (target - pos) * k. */
static inline void CutInLerp(GfxSprite *sprite, const float *target, float k)
{
    sprite->posX = sprite->posX + (target[0] - sprite->posX) * k;
    sprite->posY = sprite->posY + (target[1] - sprite->posY) * k;
    sprite->posZ = sprite->posZ + (target[2] - sprite->posZ) * k;
    sprite->posW = sprite->posW + (target[3] - sprite->posW) * k;
}

/* x < lo gives lo, x <= hi gives x, anything else (incl. NaN) gives hi. */
static inline float CutInClamp(float x, float lo, float hi)
{
    if (x < lo) {
        return lo;
    }
    if (x <= hi) {
        return x;
    }
    return hi;
}

void BtlHudUpdateAbilityCutIn(BtlHud *self, BtlBakugan *unit)
{
    char path[32];
    char name[32];
    ScePspFVector4 black;
    float pos[4];
    GfxSprite *portrait;
    GfxSprite *band;
    GfxSprite *spinA;
    GfxSprite *sprite;
    BtlMain *scene;
    CoreObject *obj;
    CoreObject *tex;
    void *blob;
    u32 size;
    s32 artNo;
    s32 frame;
    s32 i;
    float t;
    float s;
    float k;
    float fade;
    float scale;
    float spinAlpha;
    u8 a;
    bool skip;
    bool fromLow;
    bool done;

    if (self->cutInState == 0 || unit == NULL) {
        return;
    }
    skip = false;
    portrait = self->sprites[0x67];
    band = self->sprites[0x66];
    spinA = self->sprites[0x65];
    if (SaveGetProfileFlag0() != 0 && SaveHasProfile() &&
        SaveProfileHasFlags(SaveGetProfile(), 0x4880)) {
        skip = true;
    }
    if (skip && self->cutInState > 0) {
        portrait->flags &= ~1u;
        band->flags &= ~1u;
        spinA->flags &= ~1u;
        self->sprites[0xe7]->flags &= ~1u;
        self->cutInState = 100;
    }

    switch (self->cutInState) {
    case 1:
        self->cutInFlag = 0;
        for (i = 0; i < 3; i++) {
            memset(self->cutInColor[i], 0, sizeof(self->cutInColor[i]));
        }
        self->cutInAlpha = 0.0f;
        self->cutInTimer = 0;
        self->resultSlideX = 0;
        self->cutInFadeFrame = 0;
        self->cutInState++;
        /* fall through */
    case 2:
        if (--self->resultSlideX > 0) {
            break;
        }
        self->cutInState++;
        /* fall through */
    case 3:
        artNo = unit->combat.selectedArt + 1;
        sprintf(path, "abi_baku_%03d.lzs", ((artNo - 1) & 0xfffc) + 1);
        blob = CorePackChainFind(g_ioLzsPackages, path);
        if (blob != NULL) {
            size = CoreLzssGetSize(blob);
            self->cutInPixelsA = MemAllocAligned(size, true);
            CoreLzssDecompress(blob, self->cutInPixelsA);
            sceKernelDcacheWritebackInvalidateRange(self->cutInPixelsA, size);
        }
        sprintf(path, "card_L_%03d.lzs", artNo);
        blob = CorePackChainFind(g_ioLzsPackages, path);
        if (blob != NULL) {
            size = CoreLzssGetSize(blob);
            self->cutInPixelsB = MemAllocAligned(size, true);
            CoreLzssDecompress(blob, self->cutInPixelsB);
            sceKernelDcacheWritebackInvalidateRange(self->cutInPixelsB, size);
        }
        self->cutInState++;
        break;

    case 4:
        artNo = unit->combat.selectedArt + 1;
        if (SaveGetProfileFlag0() == 0 && BtlCameraTaskExists() != 0) {
            CoreTaskSetFlags(BtlGetCameraTask(), 1);
        }
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), 0x200131, 0, 0);
        }
        if (SndHasManager()) {
            SndManagerPlay(SndGetManager(), unit->voiceBank << 20, 0, 0);
        }
        if (self->cutInPixelsA != NULL) {
            sprintf(name, "abi_baku_%03d", ((artNo - 1) & 0xfffc) + 1);
            tex = NULL;
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            obj = MemAlloc(0x140, NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            if (obj != NULL) {
                GfxTextureCtor(obj, name, self->cutInPixelsA, 1);
                tex = obj;
            }
            self->cutInTexA = tex;
            portrait->texture = tex;
            portrait->flags &= ~1u;
            BtlHudSetSpriteRect(0.0f, 0.0f, 256.0f, 272.0f, self, portrait);
            GfxSpriteInsetUv(0.5f, portrait);

            sprintf(name, "card_L_%03d", artNo);
            tex = NULL;
            MemLock();
            fromLow = MemIsAllocFromLow();
            MemSetAllocFromLow(true);
            obj = MemAlloc(0x140, NULL, 0);
            MemSetAllocFromLow(fromLow);
            MemUnlock();
            if (obj != NULL) {
                GfxTextureCtor(obj, name, self->cutInPixelsB, 1);
                tex = obj;
            }
            self->cutInTexB = tex;
        }
        for (i = 0; i < 1; i++) {
            self->sprites[0xe7 + i]->flags &= ~1u;
            if (self->cutInTexB != NULL) {
                self->sprites[0xe7 + i]->texture = self->cutInTexB;
            }
            BtlHudSetSpriteRect(0.0f, 0.0f, 128.0f, 176.0f, self, self->sprites[0xe7 + i]);
        }
        self->cutInState = 10;
        break;

    case 10:
        ((BtlMain *)BtlGetCameraTask())->flashTarget = 0.0f;
        portrait->flags |= 1;
        portrait->alpha = 0.0f;
        portrait->posX = self->cutInPortraitPos[0];
        portrait->posY = self->cutInPortraitPos[1];
        portrait->posZ = self->cutInPortraitPos[2];
        portrait->posW = self->cutInPortraitPos[3];
        portrait->posX -= 192.0f;
        for (i = 0; i < 1; i++) {
            self->sprites[0xe7 + i]->alpha = 0.0f;
            self->sprites[0xe7 + i]->flags |= 1;
            self->sprites[0xe7 + i]->posX = self->cutInCardPos[0];
            self->sprites[0xe7 + i]->posY = self->cutInCardPos[1];
            self->sprites[0xe7 + i]->posZ = self->cutInCardPos[2];
            self->sprites[0xe7 + i]->posW = self->cutInCardPos[3];
            self->sprites[0xe7 + i]->posY += 272.0f;
        }
        band->alpha = 0.0f;
        band->flags |= 1;
        GfxSpriteSetScaleRotation(band, 20.0f, 1.0f, 0.0f, false);
        spinA->alpha = 0.0f;
        spinA->flags |= 1;
        GfxSpriteSetScaleRotation(spinA, 2.0f, 2.0f, 0.0f, false);
        spinA->blendMode = 2;
        self->sprites[0xe8]->flags |= 1;
        self->sprites[0xe8]->alpha = 0.0f;
        GfxSpriteSetScaleRotation(self->sprites[0xe8], 2.0f, 2.0f, 0.0f, false);
        self->sprites[0xe8]->blendMode = 2;
        self->cutInSpinFrame = 0;
        self->cutInState++;
        /* fall through */
    case 11:
        t = (float)self->cutInTimer * 0.1f * 1.5707964f;
        self->cutInTimer++;
        t = CutInClamp(t, 0.0f, 1.5707964f);
        self->cutInAlpha = __builtin_sinf(t);
        black = g_colorBlack;
        for (i = 0; i < 3; i++) {
            CutInPackColor(self->cutInColor[i], &black);
            if (i & 1) {
                a = 0;
            } else {
                a = (u8)(s32)(self->cutInAlpha * 255.0f);
            }
            self->cutInColor[i][3] = a;
        }
        CutInLerp(portrait, self->cutInPortraitPos, CutInClamp(self->cutInAlpha, 0.0f, 1.0f));
        portrait->alpha = self->cutInAlpha;
        for (i = 0; i < 1; i++) {
            k = CutInClamp(self->cutInAlpha, 0.0f, 1.0f);
            CutInLerp(self->sprites[0xe7 + i], self->cutInCardPos, k);
            self->sprites[0xe7 + i]->alpha = self->cutInAlpha;
        }
        t = self->cutInAlpha * 3.0f;
        band->alpha = t;
        band->alpha = CutInClamp(t, 0.0f, 1.0f);
        spinA->alpha = self->cutInAlpha * 0.5f;
        frame = self->cutInSpinFrame++;
        GfxSpriteSetScaleRotation(spinA, 2.0f, 2.0f, (float)frame * 0.1f, false);
        self->sprites[0xe8]->alpha = self->cutInAlpha * 0.5f;
        GfxSpriteSetScaleRotation(self->sprites[0xe8], 2.0f, 2.0f,
                                  (float)self->cutInSpinFrame * -0.1f, false);
        if (!(self->cutInAlpha < 1.0f)) {
            self->cutInState++;
        }
        break;

    case 12:
        self->cutInTimer = 20;
        self->cutInState++;
        /* fall through */
    case 13:
        if (--self->cutInTimer > 0) {
            s = __builtin_sinf((float)self->cutInSpinFrame * 0.5235988f);
            t = s * 0.1f + 0.9f;
            band->alpha = t;
            band->alpha = CutInClamp(t, 0.0f, 1.0f);
            frame = self->cutInSpinFrame++;
            GfxSpriteSetScaleRotation(spinA, 2.0f, 2.0f, (float)frame * 0.1f, false);
            GfxSpriteSetScaleRotation(self->sprites[0xe8], 2.0f, 2.0f,
                                      (float)self->cutInSpinFrame * -0.1f, false);
            break;
        }
        self->cutInState = 20;
        /* fall through */
    case 20:
        self->cutInTimer = 0;
        self->cutInFadeFrame = 0;
        self->cutInState++;
        /* fall through */
    case 21:
        t = (float)self->cutInTimer * 0.06666667f * 1.5707964f;
        self->cutInTimer++;
        done = false;
        t = CutInClamp(t, 0.0f, 1.5707964f);
        fade = 1.0f - __builtin_sinf(t);
        scale = (float)self->cutInTimer * 0.05f + 1.0f;
        pos[0] = self->cutInPortraitPos[0];
        pos[1] = self->cutInPortraitPos[1];
        pos[2] = self->cutInPortraitPos[2];
        pos[3] = self->cutInPortraitPos[3];
        portrait->alpha = fade;
        GfxSpriteSetScaleRotation(portrait, scale, scale, 0.0f, false);
        pos[0] = self->cutInCardPos[0];
        pos[1] = self->cutInCardPos[1];
        pos[2] = self->cutInCardPos[2];
        pos[3] = self->cutInCardPos[3];
        pos[1] -= 272.0f;
        t = 1.0f - fade;
        spinAlpha = fade * 0.2f;
        for (i = 0; i < 1; i++) {
            self->sprites[0xe7 + i]->alpha = fade;
            CutInLerp(self->sprites[0xe7 + i], pos, CutInClamp(t, 0.0f, 1.0f));
        }
        band->alpha = fade;
        spinA->alpha = spinAlpha;
        if (spinAlpha <= 0.05f) {
            spinA->alpha = 0.0f;
        }
        frame = self->cutInSpinFrame++;
        GfxSpriteSetScaleRotation(spinA, 2.0f, 2.0f, (float)frame * 0.1f, false);
        self->sprites[0xe8]->alpha = spinAlpha;
        if (self->sprites[0xe8]->alpha <= 0.05f) {
            self->sprites[0xe8]->alpha = 0.0f;
        }
        GfxSpriteSetScaleRotation(self->sprites[0xe8], 2.0f, 2.0f,
                                  (float)self->cutInSpinFrame * -0.1f, false);
        if (self->cutInFlag != 0) {
            if (self->cutInTimer >= 7) {
                t = (float)self->cutInFadeFrame * 0.05f * 1.5707964f;
                self->cutInFadeFrame++;
                t = CutInClamp(t, 0.0f, 1.5707964f);
                s = __builtin_sinf(t);
                scene = BtlGetCameraTask();
                scene->dimColor[0] = 1.0f;
                scene->dimColor[1] = 1.0f;
                scene->dimColor[2] = 1.0f;
                scene->dimColor[3] = s;
            }
        } else {
            black = g_colorBlack;
            a = (u8)(s32)(fade * 255.0f);
            for (i = 0; i < 3; i++) {
                CutInPackColor(self->cutInColor[i], &black);
                self->cutInColor[i][3] = (i & 1) ? 0 : a;
            }
        }
        if (portrait->alpha <= 0.0f) {
            done = true;
        } else if (portrait->alpha <= 0.3f) {
            ((BtlMain *)BtlGetCameraTask())->flashTarget = 0.0f;
        }
        if (!done) {
            break;
        }
        self->cutInState = 30;
        /* fall through */
    case 30:
        ((BtlMain *)BtlGetCameraTask())->flashTarget = 0.0f;
        if (self->cutInFlag != 0) {
            self->cutInState = 50;
            break;
        }
        self->cutInState++;
        /* fall through */
    case 31:
        self->sprites[0x67]->flags &= ~1u;
        for (i = 0; i < 1; i++) {
            self->sprites[0xe7 + i]->flags &= ~1u;
        }
        self->cutInTimer = 0;
        self->cutInAlpha = 1.0f;
        self->cutInState++;
        break;

    case 32:
        UiTalkFreeCutInTextures(UiGetTalkTask());
        self->cutInState = 100;
        break;

    case 50:
        self->cutInTimer = 0;
        self->cutInState++;
        /* fall through */
    case 51:
        t = (float)self->cutInFadeFrame * 0.05f * 1.5707964f;
        self->cutInFadeFrame++;
        t = CutInClamp(t, 0.0f, 1.5707964f);
        s = __builtin_sinf(t);
        scene = BtlGetCameraTask();
        scene->dimColor[0] = 1.0f;
        scene->dimColor[1] = 1.0f;
        scene->dimColor[2] = 1.0f;
        scene->dimColor[3] = s;
        if (s < 0.9f) {
            break;
        }
        scene = BtlGetCameraTask();
        scene->dimColor[0] = 1.0f;
        scene->dimColor[1] = 1.0f;
        scene->dimColor[2] = 1.0f;
        scene->dimColor[3] = 1.0f;
        self->cutInState = 100;
        /* fall through */
    case 100:
        if (BtlCameraTaskExists() != 0) {
            CoreTaskClearFlags(BtlGetCameraTask(), 1);
        }
        self->cutInTimer = 0;
        self->cutInAlpha = 0.0f;
        self->cutInState = 0;
        break;

    default:
        break;
    }
}
