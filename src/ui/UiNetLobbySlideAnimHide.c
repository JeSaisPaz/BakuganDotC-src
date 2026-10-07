// bdc 0x08940ff8 UiNetLobbySlideAnimHide
#include "bdc.h"

/* Hide slot of `UiNetLobbySlideAnim`: `reset` = 1 rewinds (frame 0) and returns true. Otherwise
   per frame: fade = 1 − frame·0.05 for 20 frames (0 afterwards), written to the sprite alpha
   (multiplied into the current alpha when `mode` = 1); X = layout X + offset·(1 − ease) with
   ease = (1 − cos(π·(1 − frame/24)²))/2 over 24 frames (0 afterwards). Returns true when both have
   finished (frame ≥ 24) or there is no sprite / layout position, false while running.
   The cosine is vcos of angle·S703 (bank 2/π), i.e. cos of the radian angle. */

bool UiNetLobbySlideAnimHide(UiNetLobbySlideAnim *anim, u8 reset)
{
  GfxSprite *sprite;
  bool done;
  int frame;
  float fade;
  float ease;
  float e;
  float angle;

  sprite = anim->sprite;
  done = true;
  if (sprite == NULL || anim->layoutPos == NULL) {
    return true;
  }
  if (reset != 0) {
    anim->frame = 0;
    return true;
  }
  frame = anim->frame + 1;
  anim->frame = frame;
  fade = 0.0f;
  if (frame < 20) {
    fade = 1.0f - (float)anim->frame * 0.05f;
    done = false;
  }
  if (anim->mode == 1) {
    sprite->alpha = sprite->alpha * fade;
  }
  else {
    sprite->alpha = fade;
  }
  frame = anim->frame;
  ease = 0.0f;
  if (frame < 24) {
    e = 1.0f - (float)frame * 0.041666668f;
    angle = e * e * 3.1415927f;
    ease = (1.0f - __builtin_cosf(angle)) * 0.5f;
    done = false;
  }
  anim->sprite->posX = (float)*anim->layoutPos + (float)anim->offset * (1.0f - ease);
  return done;
}
