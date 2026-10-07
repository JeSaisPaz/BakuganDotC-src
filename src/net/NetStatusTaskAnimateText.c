// bdc 0x08944054 NetStatusTaskAnimateText
#include "bdc.h"

/* Animates the message glyphs of `NetStatusTask`: each glyph sprite (`glyphs`,
   `glyphCount`) bobs around Y = 136 with a cosine wave (phase frame/2, advancing 8 per glyph) whose
   amplitude is (3 cos(frame/2 * pi/60))^5 outside frame/2 in [30, 90) and 0 inside it, and takes the
   overlay `alpha` (visible flag bit 0 cleared at alpha 0, set again when coming from 0). The frame
   counter then advances by 2 (rounded to even when the display is not frame-skipping) and wraps at
   240. */

void NetStatusTaskAnimateText(NetStatusTask *self)
{
    GfxSprite *glyph;
    float amp;
    float c;
    int phase;
    int frame;
    int i;

    glyph = self->glyphs;
    phase = self->frame / 2;
    amp = 0.0f;
    if (phase < 30 || phase >= 90) {
        c = __builtin_cosf((float)phase * 0.0523598790f);
        amp = c * 3.0f;
        amp = amp * amp * amp * amp * amp;
    }
    if (glyph != NULL) {
        for (i = 0; i < self->glyphCount; i++, phase += 8, glyph++) {
            c = __builtin_cosf((float)phase * 0.104719758f);
            glyph->posY = c * amp + 136.0f;
            if (glyph->alpha != self->alpha) {
                if (self->alpha == 0.0f) {
                    glyph->flags &= ~1u;
                } else if (glyph->alpha == 0.0f) {
                    glyph->flags |= 1;
                }
                glyph->alpha = self->alpha;
            }
        }
    }
    frame = self->frame + 1;
    self->frame = frame;
    if (g_gfxDisplay->frameSkip != 0 || (frame & 1) != 0) {
        frame++;
        self->frame = frame;
    }
    if (frame >= 240) {
        do {
            frame -= 240;
        } while (frame >= 240);
        self->frame = frame;
    }
}
