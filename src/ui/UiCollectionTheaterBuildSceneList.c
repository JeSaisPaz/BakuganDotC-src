// bdc 0x089879b8 UiCollectionTheaterBuildSceneList
#include "bdc.h"

/* Builds the unlocked-scene list of `UiCollectionTheater`: fills
   `sceneId` (21 slots, 6 per page) with 0xff and clears `sceneNew`, then for every scene whose bit
   is set in the profile's unlocked-scene bits (`newItemGroups[0x2b..]`, data `+0x5fe`) stores its
   index in its own slot and marks it new when its viewed bit (`newItemGroups[0x2e..]`, data
   `+0x601`) is clear. */

void UiCollectionTheaterBuildSceneList(UiScreen *screen)
{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  int i;

  memset(self->sceneId, 0xff, 21);
  memset(self->sceneNew, 0, 21);
  for (i = 0; i < 21; i++) {
    if ((SaveGetProfile()->data->newItemGroups[0x2b + i / 8] & (u8)(1 << (i % 8))) != 0) {
      self->sceneId[i] = (u8)i;
      if ((SaveGetProfile()->data->newItemGroups[0x2e + i / 8] & (u8)(1 << (i % 8))) == 0) {
        self->sceneNew[i] = 1;
      }
    }
  }
}
