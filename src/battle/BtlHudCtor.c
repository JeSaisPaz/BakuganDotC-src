// bdc 0x0883ad88 BtlHudCtor
#include "bdc.h"

/* Constructor of the battle/field HUD and talk-window task `BtlHud` (task id 110 = 0x6e, 0xc10
   bytes, created by `CoreTaskNewById`; the object `UiGetTalkTask` returns): runs
   `CoreTaskInit`, installs `g_btlHudVtbl`, builds the `IoLzsPackageCtor` package and resets
   every widget's state: layouts, blenders, lock-on marker, combo, enemy arrows, cut-ins, gate
   cut-in, score, guard, banner, talk window, status icons, finish/multi-round banners, lamps,
   charge gauge, advice slots, button guide, caption and control hints. It clears the 15 UI
   window-active flags `g_uiWindowActive` and then marks window 0xb (talk) active
   (`UiSetWindowActive`), takes `pad` from `g_padState`, allocates the 16-byte `.fab` table
   `fabs` from the low end of the heap (under `MemLock`) and zeroes it, empties the name, voice
   and caption strings, and sets `hudAlpha` 1, every `arrowPulse` 1, `overlayDepth` 107,
   `hintScale` 0.9 and `talkVoiceId`/`msgAux`/`ratingSndHandle`/`talkFlags`/every `adviceMsg` to
   -1. Returns `self`.
   The quads `arrowShownPos`, `reserved3c0`, `reserved410`, `gateCutInKeyPos`,
   `gateCutInFromPos`, `iconTarget`, `finishTarget` and `multiRoundPos` are zeroed (sv.q of the
   bank's zero column C720) and each `gaugeFlipTimer[i]` is a random value in [0, 8)
   (`(vrndf1 - 1) * 8`, the 1 being the bank's S733). */

/* sv.q of the bank's zero column C720: clears the 4 floats at `dst`. */
static void ZeroQuad(float *dst)
{
    dst[0] = 0.0f;
    dst[1] = 0.0f;
    dst[2] = 0.0f;
    dst[3] = 0.0f;
}

BtlHud *BtlHudCtor(BtlHud *self)
{
    s32 i;
    s32 j;
    bool fromLow;
    GfxFab **fabs;

    CoreTaskInit(&self->base);
    self->base.vtable = g_btlHudVtbl;
    IoLzsPackageCtor((IoLzsPackage *)self->package);
    self->layer = NULL;
    self->sprites = NULL;
    self->resultLayer = NULL;
    self->resultSprites = NULL;
    self->msgLayer = NULL;
    self->msgSprites = NULL;
    self->ratingLayer = NULL;
    self->ratingSprites = NULL;
    self->arenaLayer = NULL;
    self->arenaSprites = NULL;
    self->extraBlend = NULL;
    self->reserved020 = 0;
    self->phase = 0;
    self->phaseStep = 0;
    self->adviceFrame = 0;
    self->introStep = 0;
    self->introTimer = 0;
    self->markerFade = 0.0f;
    self->markerSpin = 0.0f;
    self->markerPulse = 0.0f;
    self->markerShownTarget = NULL;
    self->markerTarget = NULL;
    self->comboSpare0fc = 0;
    self->comboCount = 0;
    self->comboSpare104 = 0;
    self->comboState = 0;
    self->comboSpare108 = 0.0f;
    self->comboAngle = 0.0f;
    self->comboFade = 0.0f;
    self->bestCombo = 0;
    self->savedAlpha = NULL;
    self->talkVoiceId = -1;
    self->markerPrevStyle = 0;
    self->markerStyle = 0;
    self->markerStep = 0;
    self->resultScreenState = 0;
    self->msgStep = 0;
    self->msgWait = 0;
    self->msgFade = 0.0f;
    self->msgIndex = 0;
    self->msgAux = -1;
    self->ratingFinished = 0;
    self->ratingSkip = 0;
    self->arenaSkip = 0;
    self->radarScale = 0.0f;
    self->hudAlpha = 1.0f;
    self->reserved0c4 = 0;
    /* radarX/radarY: a two-step word loop in the binary */
    self->radarX = 0;
    self->radarY = 0;
    for (i = 0; i < 2; i++) {
        self->timerScroll[i] = NULL;
    }
    for (i = 0; i < 3; i++) {
        self->markerBlend[i] = NULL;
    }
    for (i = 0; i < 4; i++) {
        self->arrowBlend[i] = NULL;
        self->arrowShown[i] = 0;
        self->arrowAngle[i] = 0.0f;
        self->arrowWarn[i] = 0;
        self->arrowPulse[i] = 1.0f;
        self->arrowFade[i] = 0.0f;
        self->arrowSoundTimer[i] = 0;
    }
    /* 12 consecutive quads (one loop in the binary) */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            ZeroQuad(&self->arrowShownPos[i][4 * j]);
        }
    }
    /* each RGBA entry is cleared with one word store */
    for (i = 0; i < 3; i++) {
        memset(self->countdownGlow[i], 0, sizeof(self->countdownGlow[i]));
    }
    for (i = 0; i < 3; i++) {
        memset(self->cutInColor[i], 0, sizeof(self->cutInColor[i]));
    }
    for (i = 0; i < 15; i++) {
        g_uiWindowActive[i] = 0;
    }
    UiSetWindowActive(0xb, 1);
    self->chargePrevLevel = 0;
    self->chargeSndStarted = 0;
    self->chargeFlagAe3 = 0;
    self->chargeFullTimer = 0;
    self->cutInState = 0;
    self->cutInTimer = 0;
    self->cutInFadeFrame = 0;
    self->reserved39c = 0;
    self->reserved3a0 = 0;
    self->reserved3a4 = 0;
    self->reserved460 = 0.0f;
    self->reserved464 = 0.0f;
    self->speedLines = 0;
    self->cutInPixelsA = NULL;
    self->cutInTexA = NULL;
    self->cutInAlpha = 0.0f;
    self->cutInPixelsB = NULL;
    self->cutInTexB = NULL;
    self->cutInFlag = 0;
    self->reserved484 = 0;
    self->artFlashAlpha = 0.0f;
    self->artFlashDir = 0;
    for (i = 0; i < 2; i++) {
        self->buildPixels[i] = NULL;
        self->buildTex[i] = NULL;
    }
    for (i = 0; i < 5; i++) {
        ZeroQuad(self->reserved3c0[i]);
        ZeroQuad(self->reserved410[i]);
    }
    for (i = 0; i < 3; i++) {
        self->gaugeFlipTimer[i] = (PlatformRandFloat12() - 1.0f) * 8.0f;
    }
    self->gateCutInState = 0;
    self->gateCutInPhaseA = 0.0f;
    self->gateCutInPhaseB = 0.0f;
    self->gateCutInFrame = 0;
    self->gateCutInKeyFrame = 0;
    self->gateCutInPrevKeyFrame = 0;
    ZeroQuad(self->gateCutInKeyPos);
    ZeroQuad(self->gateCutInFromPos);
    self->gateCutInUnit = NULL;
    self->abilityCutInUnit = NULL;
    self->countdownState = 0;
    for (i = 0; i < 4; i++) {
        self->scoreSeen[i] = 0;
        self->scoreShown[i] = 0;
        self->gainPopupState[i] = 0;
        self->scoreCountUp[i] = 0;
        self->scoreCountDone[i] = 0;
    }
    self->guardState = 0;
    self->guardTimer = 0;
    self->guardFade = 0.0f;
    self->endBannerState = 0;
    self->bannerClosing = 0;
    self->bannerZoom = 0.0f;
    self->fabs = NULL;
    self->fabList.tail = NULL;
    self->fabList.head = NULL;
    self->fabList.count = 0;
    self->fabsHidden = 0;
    self->chargeLoopStartFrame = 0;
    self->chargeLoopFrameCount = 0;
    self->stageIntroDone = 0;
    self->chargeStep = 0;
    self->pad = g_padState;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    fabs = MemAlloc(4 * sizeof(GfxFab *), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    self->fabs = fabs;
    memset(fabs, 0, 4 * sizeof(GfxFab *));

    self->overlayObj[0] = NULL;
    self->overlayObj[1] = NULL;
    self->mesTable0 = NULL;
    self->overlayDepth = 107.0f;
    self->msgTable = NULL;
    self->reserved8ac = 0;
    self->reserved8b0 = 0;
    strcpy(self->nameBuf, "");
    strcpy(self->voiceName, "");
    self->talkTextAlpha = 0.0f;
    self->talkTextAlphaSet = 0.0f;
    self->talkOverlayAlpha = 0.0f;
    self->talkOverlayAlphaSet = 0.0f;
    self->talkTextWidth = 0.0f;
    self->talkTextHeight = 0.0f;
    self->talkFade = 0.0f;
    self->talkRamp = 0.0f;
    self->talkState = 0;
    self->talkPageHold = 0;
    self->talkHold = 0;
    self->talkKind = 0;
    self->talkMsgId = 0;
    self->talkStyle = 0;
    self->talkUnitDelta = 0;
    self->talkPrintBusy = 0;
    self->talkScriptSuspended = 0;
    self->talkForce = 0;
    self->talkUnitLast = 0;
    self->iconCount = 0;
    for (i = 0; i < 6; i++) {
        ZeroQuad(self->iconTarget[i]);
        self->iconState[i] = 0;
        self->iconAux[i] = 0;
        self->iconSlot[i] = 0;
        self->iconAngle[i] = 0.0f;
    }
    ZeroQuad(self->finishTarget);
    self->finishState = 0;
    self->finishAngle = 0.0f;
    self->finishHold = 0.0f;
    self->finishStarted = 0;
    for (i = 0; i < 3; i++) {
        ZeroQuad(self->multiRoundPos[i]);
    }
    self->multiRound = 0;
    self->multiRoundFade = 0.0f;
    self->multiRoundHold = 0.0f;
    self->introVoiceDone = 0;
    self->lampState = 0;
    self->lampAngle = 0.0f;
    self->reserveda70 = 0.0f;
    self->lampLit = NULL;
    self->lampPulse = NULL;
    strcpy(self->captionText, "");
    self->hintPage = 0;
    self->hintTimer = 0;
    self->hintState = 0;
    self->hintFade = 0.0f;
    self->captionX = 0.0f;
    self->reservedbf8 = 0.0f;
    self->hintStick = 0;
    self->hintExtraShown = 0;
    self->hintScaleGrowing = 0;
    self->hintScale = 0.9f;
    self->ratingSndHandle = -1;
    self->talkFlags = 0xffffffffu;
    for (i = 0; i < 27; i++) {
        self->adviceState[i] = 0;
        self->adviceMsg[i] = -1;
        self->adviceAux[i] = 0;
    }
    self->adviceThreshold = 0;
    self->guideState = 0;
    self->guidePage = 0;
    self->reservedb98 = 0;
    self->guideFade = 0.0f;
    self->guideShow = 0;
    return self;
}
