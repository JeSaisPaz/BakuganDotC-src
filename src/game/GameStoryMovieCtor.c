// bdc 0x088cdefc GameStoryMovieCtor
#include "bdc.h"

/* Constructor of the story movie task (task id 520, `GameStoryMovie`, 0x38 bytes, vtable
   `g_gameStoryMovieVtbl`): `CoreTaskInit`, black clear colour (`g_colorBlack`), frame skip 1,
   pad repeat 16/2, stores the movie set `arg` (0..3 per map, 4 = special) and resets the movie
   state; picks the quest mode from the stage's `.qsd` data (`GameStoryMovieSelectQuestMode`,
   `GameStoryMovieRedirectHubToStage0`, `GameStoryMovieCheckClearMode`); for sets other than 4
   it marks the set as watched in the profile (`mapMovieWatched[map][set]`, map = script variable
   15) or skips straight to the end (step 100) when already watched. Returns `self`. */

GameStoryMovie *GameStoryMovieCtor(GameStoryMovie *self, u32 arg)

{
  CoreTaskInit(&self->base);
  self->base.vtable = g_gameStoryMovieVtbl;
  self->questMode = 0;
  self->questData = NULL;
  g_gfxDisplay->clearColor[0] = g_colorBlack.x;
  g_gfxDisplay->clearColor[1] = g_colorBlack.y;
  g_gfxDisplay->clearColor[2] = g_colorBlack.z;
  g_gfxDisplay->clearColor[3] = g_colorBlack.w;
  g_gfxDisplay->frameSkip = 1;
  PadSetRepeatDelay(g_padState, 16);
  PadSetRepeatInterval(g_padState, 2);
  self->set = arg;
  self->step = 0;
  self->reserved18 = 0;
  self->prevMovie = 0;
  self->movie = 0;
  self->cueIndex = 0;
  self->cueFrame = -1;
  self->cues = NULL;
  GameStoryMovieSelectQuestMode(self);
  GameStoryMovieRedirectHubToStage0(self);
  GameStoryMovieCheckClearMode(self);
  if (self->set != 4) {
    if (SaveGetProfile()->data->mapMovieWatched[(u8)g_scriptGlobalVars[15]][(u8)self->set] == 0) {
      SaveGetProfile()->data->mapMovieWatched[(u8)g_scriptGlobalVars[15]][(u8)self->set] = 1;
    }
    else {
      self->step = 100;
    }
  }
  return self;
}
