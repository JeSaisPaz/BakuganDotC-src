// bdc 0x088cde84 GameStoryMovieCheckClearMode
#include "bdc.h"

/* Resets the quest mode of the story movie task (task id 520, `GameStoryMovieCtor`, 0x38 bytes,
   vtable `0x08af2dbc`; movie set `+0x10`, step `+0x14`, current movie `+0x1c`, quest mode `+0x30`,
   quest data `+0x34`) from 2 (clear) to 0 when profile bytes `+0x453` is set and `+0x454`, `+0x462`
   are clear. */

void GameStoryMovieCheckClearMode(GameStoryMovie *self)

{
  if (self->questMode == 2 && SaveGetProfile()->data->mapMovieWatched[16][2] != 0 &&
      SaveGetProfile()->data->mapMovieWatched[16][3] == 0 &&
      SaveGetProfile()->data->playthrough == 0) {
    self->questMode = 0;
  }
}
