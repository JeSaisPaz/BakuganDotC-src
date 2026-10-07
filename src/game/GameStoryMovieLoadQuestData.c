// bdc 0x088cdaac GameStoryMovieLoadQuestData
#include "bdc.h"

/* Looks up the quest data `"f%01d_%02d.qsd"` for (`area`, `block`) in the pack chain `g_ioLzsPackages`
   into `+0x34` of the story movie task (task id 520, `GameStoryMovieCtor`, 0x38 bytes, vtable
   `0x08af2dbc`; movie set `+0x10`, step `+0x14`, current movie `+0x1c`, quest mode `+0x30`, quest
   data `+0x34`), unless already loaded. */

void GameStoryMovieLoadQuestData(GameStoryMovie *self, s32 area, s32 block)

{
  char name[256];

  if (self->questData == NULL) {
    sprintf(name, "f%01d_%02d.qsd", area, block);
    self->questData = CorePackChainFind(g_ioLzsPackages, name);
  }
}
