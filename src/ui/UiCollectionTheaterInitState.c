// bdc 0x08987aec UiCollectionTheaterInitState
#include "bdc.h"

/* Class init of `UiCollectionTheater` called by
   `UiCollectionTheaterCtor`: clears cursor `+0x8e0`, page `+0x8e1`, target page `+0x8e2`, page
   direction `+0x8e3`, builds the scene list (`UiCollectionTheaterBuildSceneList`) and clears the
   thumbnail bob record `+0x918`. */

void UiCollectionTheaterInitState(UiScreen *screen)
{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;

  self->cursor = 0;
  self->page = 0;
  self->unk8e4 = 0;
  self->targetPage = 0;
  self->pageDir = 0;
  UiCollectionTheaterBuildSceneList(screen);
  memset(&self->thumbBobOn, 0, 0xc);
}
