// bdc 0x088ef340 GameEventDeleteProps
#include "bdc.h"

/* Releases and deletes the props of all 10 event prop slots (`ev+0x30` records). */

void GameEventDeleteProps(GameEvent *self)

{
  u8 i;

  for (i = 0; i < 10; i++) {
    if (self->props[i].prop != NULL) {
      GameEventPropReleaseModel(self->props[i].prop);
      GameEventPropDelete(self->props[i].prop);
      self->props[i].prop = NULL;
    }
  }
}
