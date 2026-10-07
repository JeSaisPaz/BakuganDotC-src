// bdc 0x0894f580 UiNetMenuUpdatePanelTween
#include "bdc.h"

/* Advances the slide/fade of the two arrow sprites (data sprites 2 and 3, records
   `arrowSlide[0..1]`) of the network-play menu (task 1999, `UiNetMenuCtor`; host-or-join choice
   that creates the lobby task 2000, `UiNetLobbyHostPhase`/`UiNetLobbyJoinPhase`); sprite 2 is
   on the left, sprite 3 on the right. Opening (`out` false) eases them in from `fromX` by
   `distance` and fades them up; closing grows, fades and slides them 64 px outwards, then hides
   them. Returns true when both reached t >= 1 this frame. */

bool UiNetMenuUpdatePanelTween(UiScreen *screen, bool out)
{
  UiNetMenu *menu = (UiNetMenu *)screen;
  UiNetMenuButtonSlide *slide;
  GfxSprite *sprite;
  u8 done;
  s32 i;
  float t;
  float u;
  float fromX;

  done = 0;
  if (!out) {
    for (i = 2; i < 4; i++) {
      slide = &menu->arrowSlide[i - 2];
      t = slide->t + 0.125f;
      slide->t = t;
      u = t - 1.0f;
      ((GfxSprite **)screen->data)[i]->alpha = slide->startAlpha + (1.0f - u * u);
      sprite = ((GfxSprite **)screen->data)[i];
      fromX = (float)slide->fromX;
      t = slide->t;
      u = t - 1.0f;
      if (i == 2) {
        sprite->posX = fromX - (1.0f - u * u) * (float)slide->distance;
      } else {
        sprite->posX = fromX + (1.0f - u * u) * (float)slide->distance;
      }
      if (!(slide->t < 1.0f)) {
        ((GfxSprite **)screen->data)[i]->alpha = 1.0f;
        ((GfxSprite **)screen->data)[i]->posX = (float)slide->toX;
        done++;
        ((GfxSprite **)screen->data)[i]->posZ = -20.0f;
      }
    }
  } else {
    for (i = 2; i < 4; i++) {
      slide = &menu->arrowSlide[i - 2];
      t = slide->t + 0.125f;
      slide->t = t;
      ((GfxSprite **)screen->data)[i]->alpha = slide->startAlpha - t * t;
      ((GfxSprite **)screen->data)[i]->scaleX = slide->startScale + slide->t * slide->t;
      sprite = ((GfxSprite **)screen->data)[i];
      sprite->scaleY = sprite->scaleX;
      sprite = ((GfxSprite **)screen->data)[i];
      GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
      sprite = ((GfxSprite **)screen->data)[i];
      t = slide->t;
      fromX = (float)slide->fromX;
      u = t - 1.0f;
      if (i == 2) {
        sprite->posX = fromX - (1.0f - u * u) * 64.0f;
      } else {
        sprite->posX = fromX + (1.0f - u * u) * 64.0f;
      }
      if (!(slide->t < 1.0f)) {
        done++;
        ((GfxSprite **)screen->data)[i]->flags &= ~1u;
      }
    }
  }
  return done == 2;
}
