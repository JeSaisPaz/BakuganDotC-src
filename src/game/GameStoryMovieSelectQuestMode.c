// bdc 0x088cdb04 GameStoryMovieSelectQuestMode
#include "bdc.h"

/* For movie set 0 of the story movie task (task id 520, `GameStoryMovieCtor`): loads the current
   stage's quest data (script variable 1 = stage; area = stage/4, block = stage%4,
   `GameStoryMovieLoadQuestData`) and, when data was found, sets `questMode`: 1 (intro) when
   movie 0 of the current map (script variable 15) was never watched
   (`mapMovieWatched[map][0]`) and the data's first byte is 0; 2 (clear) when movie 0 was watched,
   movie 2 was watched, movie 3 was not and the area is not 8. Otherwise `questMode` is left as is. */

void GameStoryMovieSelectQuestMode(GameStoryMovie *self)

{
  s32 stage;

  if (self->set == 0) {
    stage = g_scriptGlobalVars[1];
    GameStoryMovieLoadQuestData(self, stage / 4, stage % 4);
    if (self->questData != NULL) {
      if (SaveGetProfile()->data->mapMovieWatched[(u8)g_scriptGlobalVars[15]][0] == 0) {
        if (*(u8 *)self->questData == 0) {
          self->questMode = 1;
        }
      }
      else if (SaveGetProfile()->data->mapMovieWatched[(u8)g_scriptGlobalVars[15]][2] != 0) {
        if (SaveGetProfile()->data->mapMovieWatched[(u8)g_scriptGlobalVars[15]][3] == 0 &&
            g_scriptGlobalVars[1] / 4 != 8) {
          self->questMode = 2;
        }
      }
    }
  }
  return;
}
