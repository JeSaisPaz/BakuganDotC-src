// bdc 0x088ce380 GameStoryMovieDtor
#include "bdc.h"

/* Destructor (vtable slot 1) of the story movie task (task id 520, `GameStoryMovieCtor`, 0x38
   bytes, vtable `g_gameStoryMovieVtbl`): restores the vtable; unless the movie set is 4, moves the
   story on after some finished movies (previous movie 0x70/0x71 → stage 0x25, 0x72 → 0x18, 0x3b →
   0x24: script global 1 and profile word 0x33) and applies the quest result
   (`GameStoryMovieApplyQuestResult`); then `CoreTaskDestroy` and frees the object when
   `flags & 1`. */

void GameStoryMovieDtor(GameStoryMovie *self, u32 flags)
{
  int movie;

  if (self != NULL) {
    self->base.vtable = g_gameStoryMovieVtbl;
    if (self->set != 4) {
      movie = self->prevMovie;
      if (movie == 0x70) {
        g_scriptGlobalVars[1] = 0x25;
        SaveProfileSetWord(SaveGetProfile(), 0x33, 0x25);
      }
      else if (movie == 0x71) {
        g_scriptGlobalVars[1] = 0x25;
        SaveProfileSetWord(SaveGetProfile(), 0x33, 0x25);
      }
      else if (movie == 0x72) {
        g_scriptGlobalVars[1] = 0x18;
        SaveProfileSetWord(SaveGetProfile(), 0x33, 0x18);
      }
      else if (movie == 0x3b) {
        g_scriptGlobalVars[1] = 0x24;
        SaveProfileSetWord(SaveGetProfile(), 0x33, 0x24);
      }
      GameStoryMovieApplyQuestResult(self);
    }
    CoreTaskDestroy(&self->base, 0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self, NULL, 0);
      MemUnlock();
    }
  }
}
