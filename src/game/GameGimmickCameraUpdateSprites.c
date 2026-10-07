// bdc 0x088d8280 GameGimmickCameraUpdateSprites
#include "bdc.h"

/* Sets the alpha of the view-cone sprite `+0x24c` (0.2 when `+0x254`, else 0) and of the spot
   sprite `+0x250` (1 when `+0x255`, else 0) of the surveillance camera gimmick
   (`GameGimmickCameraCtor`, vtables `0x08af325c`/`0x08af3304`). */

typedef struct {
  u8 pad[0xbc];
  float alpha;
} GameGimmickCameraSprite;

void GameGimmickCameraUpdateSprites(GameGimmickCamera *obj)

{
  GameGimmickCameraSprite *s = (GameGimmickCameraSprite *)obj->coneEffect;

  if (s != NULL) {
    if (obj->coneVisible == 0) {
      s->alpha = 0.0f;
    } else {
      s->alpha = 0.2f;
    }
  }
  s = (GameGimmickCameraSprite *)obj->spotEffect;
  if (s != NULL) {
    if (obj->spotVisible != 0) {
      s->alpha = 1.0f;
      return;
    }
    s->alpha = 0.0f;
  }
}
