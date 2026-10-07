// bdc 0x08940ec4 UiNetLobbySlideAnimShow
#include "bdc.h"

/* Show slot of `UiNetLobbySlideAnim`: `reset` = 1 rewinds (frame 0, alpha 0) and returns true.
   Otherwise per frame: alpha = frame·0.05 up to 1 (20 frames), X = layout X + offset·(1 − ease)
   with ease = (1 − cos(π·(1 − (frame/24 − 1)²)))/2 over 16 frames (ease = 1 afterwards). Returns
   true when both have finished (frame ≥ 20) or there is no sprite / layout position.
   The cosine is vcos of angle·S703 (bank 2/π), i.e. cos of the radian angle. */

bool UiNetLobbySlideAnimShow(UiNetLobbySlideAnim *anim, u8 reset)

{
  GfxSprite *sprite;
  bool done;
  int frame;
  float alpha;
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
    sprite->alpha = 0.0f;
    return true;
  }
  frame = anim->frame + 1;
  anim->frame = frame;
  alpha = 1.0f;
  if (frame < 20) {
    alpha = (float)anim->frame * 0.05f;
    done = false;
  }
  sprite->alpha = alpha;
  frame = anim->frame;
  ease = 1.0f;
  if (frame < 16) {
    e = (float)frame * 0.041666668f - 1.0f;
    angle = (1.0f - e * e) * 3.1415927f;
    ease = (1.0f - __builtin_cosf(angle)) * 0.5f;
    done = false;
  }
  anim->sprite->posX = (float)*anim->layoutPos + (float)anim->offset * (1.0f - ease);
  return done;
}
