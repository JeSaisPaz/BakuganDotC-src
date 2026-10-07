// bdc 0x0895309c UiBattleRuleSelectPulseButtonGlow
#include "bdc.h"

/* Per-frame glow pulse of rule button `index` of `UiBattleRuleSelect`
   (enabled buttons only): sprite 5+index's alpha ramps 0→1 and back over 30 frames each way.
   The direction lives in tween 5+index's `toggle07`; at each end the alpha snaps to 1 or 0, the
   ramp restarts from it and the direction flips. */

void UiBattleRuleSelectPulseButtonGlow(UiBattleRuleSelect *self, u8 index)
{
    UiTween *tw;
    GfxSprite *glow;
    float t;
    float start;
    u8 fadingOut;

    if (self->buttonEnabled[index] == 0)
        return;

    tw = &self->tweens[index + 5];
    t = tw->t + 0.033333335f;
    fadingOut = tw->toggle07;
    start = tw->startAlpha;
    tw->t = t;
    glow = ((GfxSprite **)self->base.data)[5 + index];
    if (fadingOut == 0) {
        glow->alpha = start + t;
        if (!(tw->t < 1.0f)) {
            ((GfxSprite **)self->base.data)[5 + index]->alpha = 1.0f;
            tw->t = 0.0f;
            tw->startAlpha = ((GfxSprite **)self->base.data)[5 + index]->alpha;
            tw->toggle07 = 1;
        }
    } else {
        glow->alpha = start - t;
        if (!(tw->t < 1.0f)) {
            ((GfxSprite **)self->base.data)[5 + index]->alpha = 0.0f;
            tw->t = 0.0f;
            tw->startAlpha = ((GfxSprite **)self->base.data)[5 + index]->alpha;
            tw->toggle07 = 0;
        }
    }
}
