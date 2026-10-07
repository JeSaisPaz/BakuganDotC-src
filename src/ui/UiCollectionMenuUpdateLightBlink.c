// bdc 0x08976558 UiCollectionMenuUpdateLightBlink
#include "bdc.h"

/* Blinks the item-box light mesh of the selected main entry of
   `UiCollectionMenu` (entry 0 `"Sphere_light"`, 1 `"Card_light"`,
   2 `"Figure_light"`, 3 `"Theater_light"`, 4 none): `lightLevel = (1 − cos(π·lightTime))/2` with
   `lightTime += 1/60` per frame. When `selMain` differs from `refresh`, all four lights are set to 0
   and both floats reset first. Does nothing without an item box or with `lightBlink` clear.
   The cosine is VFPU `vcos.s` of `arg · S703` (2/π), i.e. quarter turns, so it is `cos(arg)`. */

void UiCollectionMenuUpdateLightBlink(UiCollectionMenu *self)
{
    float arg;
    float t;
    s8 sel;

    if (self->itemBox == NULL || self->lightBlink == 0) {
        return;
    }
    sel = self->selMain;
    if (sel != self->refresh) {
        self->refresh = sel;
        GfxModelScaleAmbientColorByName(0.0f, self->itemBox, "Card_light");
        GfxModelScaleAmbientColorByName(0.0f, self->itemBox, "Figure_light");
        GfxModelScaleAmbientColorByName(0.0f, self->itemBox, "Sphere_light");
        GfxModelScaleAmbientColorByName(0.0f, self->itemBox, "Theater_light");
        self->lightLevel = 0.0f;
        self->lightTime = 0.0f;
        sel = self->selMain;
    }
    t = self->lightTime + 0.016666668f;
    self->lightTime = t;
    arg = t * 3.1415927f;
    self->lightLevel = (1.0f - __builtin_cosf(arg)) * 0.5f;
    switch (sel) {
    case 0:
        GfxModelScaleAmbientColorByName(self->lightLevel, self->itemBox, "Sphere_light");
        break;
    case 1:
        GfxModelScaleAmbientColorByName(self->lightLevel, self->itemBox, "Card_light");
        break;
    case 2:
        GfxModelScaleAmbientColorByName(self->lightLevel, self->itemBox, "Figure_light");
        break;
    case 3:
        GfxModelScaleAmbientColorByName(self->lightLevel, self->itemBox, "Theater_light");
        break;
    default:
        break;
    }
}
