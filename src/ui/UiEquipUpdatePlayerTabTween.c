// bdc 0x0895f3c8 UiEquipUpdatePlayerTabTween
#include "bdc.h"

/* Advances the tween started by `UiEquipStartPlayerTabTween` of the player tab sprites of the
   UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`): the per-player tabs
   (`spriteIdx[0x19]` and `spriteIdx[0x1b]`, `playerCount` sprites each) and the per-player sprite
   groups `spriteIdx[0x1d]`/`[0x41]`/`[0x49]` (`playerCount * spriteIdx[0x1e]`/`[0x42]`/`[0x4a]`
   sprites) (`UiTweenUpdate` flags 3, scale 1.5→1.0 in, 1.0→1.5 out, 8 frames), then re-anchors
   the edited player's three groups and `spriteIdx[0x1b]` tab to its `spriteIdx[0x19]` tab with
   `UiEquipAlignSpritesToAnchor`; returns true once any of the tweens has finished (they run in
   lockstep). */

bool UiEquipUpdatePlayerTabTween(UiEquip *self, u8 out)
{
    GfxSprite **sprites;
    UiTween *tween;
    s32 i;
    u8 doneCount;

    doneCount = 0;

    i = self->spriteIdx[0x19];
    if (i < i + self->playerCount) {
        tween = &self->tweens[i];
        do {
            sprites = (GfxSprite **)self->base.data;
            if (out == 0) {
                doneCount += UiTweenUpdate(1.5f, 1.0f, 8.0f, out, sprites[i], tween, 3);
            } else {
                doneCount += UiTweenUpdate(1.0f, 1.5f, 8.0f, out, sprites[i], tween, 3);
            }
            i++;
            tween++;
        } while (i < self->spriteIdx[0x19] + self->playerCount);
    }

    i = self->spriteIdx[0x1b];
    if (i < i + self->playerCount) {
        tween = &self->tweens[i];
        do {
            sprites = (GfxSprite **)self->base.data;
            if (out == 0) {
                doneCount += UiTweenUpdate(1.5f, 1.0f, 8.0f, out, sprites[i], tween, 3);
            } else {
                doneCount += UiTweenUpdate(1.0f, 1.5f, 8.0f, out, sprites[i], tween, 3);
            }
            i++;
            tween++;
        } while (i < self->spriteIdx[0x1b] + self->playerCount);
    }

    i = self->spriteIdx[0x1d];
    if (i < i + self->playerCount * self->spriteIdx[0x1e]) {
        tween = &self->tweens[i];
        do {
            sprites = (GfxSprite **)self->base.data;
            if (out == 0) {
                doneCount += UiTweenUpdate(1.5f, 1.0f, 8.0f, out, sprites[i], tween, 3);
            } else {
                doneCount += UiTweenUpdate(1.0f, 1.5f, 8.0f, out, sprites[i], tween, 3);
            }
            i++;
            tween++;
        } while (i < self->spriteIdx[0x1d] + self->playerCount * self->spriteIdx[0x1e]);
    }

    i = self->spriteIdx[0x41];
    if (i < i + self->playerCount * self->spriteIdx[0x42]) {
        tween = &self->tweens[i];
        do {
            sprites = (GfxSprite **)self->base.data;
            if (out == 0) {
                doneCount += UiTweenUpdate(1.5f, 1.0f, 8.0f, out, sprites[i], tween, 3);
            } else {
                doneCount += UiTweenUpdate(1.0f, 1.5f, 8.0f, out, sprites[i], tween, 3);
            }
            i++;
            tween++;
        } while (i < self->spriteIdx[0x41] + self->playerCount * self->spriteIdx[0x42]);
    }

    i = self->spriteIdx[0x49];
    if (i < i + self->playerCount * self->spriteIdx[0x4a]) {
        tween = &self->tweens[i];
        do {
            sprites = (GfxSprite **)self->base.data;
            if (out == 0) {
                doneCount += UiTweenUpdate(1.5f, 1.0f, 8.0f, out, sprites[i], tween, 3);
            } else {
                doneCount += UiTweenUpdate(1.0f, 1.5f, 8.0f, out, sprites[i], tween, 3);
            }
            i++;
            tween++;
        } while (i < self->spriteIdx[0x49] + self->playerCount * self->spriteIdx[0x4a]);
    }

    UiEquipAlignSpritesToAnchor(self, self->spriteIdx[0x19], self->spriteIdx[0x1a],
                                self->spriteIdx[0x1b], self->spriteIdx[0x1c], self->editPlayer);
    UiEquipAlignSpritesToAnchor(self, self->spriteIdx[0x19], self->spriteIdx[0x1a],
                                self->spriteIdx[0x1d], self->spriteIdx[0x1e], self->editPlayer);
    UiEquipAlignSpritesToAnchor(self, self->spriteIdx[0x19], self->spriteIdx[0x1a],
                                self->spriteIdx[0x41], self->spriteIdx[0x42], self->editPlayer);
    UiEquipAlignSpritesToAnchor(self, self->spriteIdx[0x19], self->spriteIdx[0x1a],
                                self->spriteIdx[0x49], self->spriteIdx[0x4a], self->editPlayer);
    return doneCount != 0;
}
