// bdc 0x088ce5bc GameStoryMovieUpdate
#include "bdc.h"

/* Update (vtable slot 2) of the story movie task (task id 520, `GameStoryMovieCtor`), a step
   machine on `step`:
   0: remembers the movie in `prevMovie` and picks the next one: for set 4 the movie the task was
      created with plays once (−1 from the second pass on), otherwise
      `g_gameStoryMovieIds``[map][set][``g_gameStoryMovieSeq``]`, whose theater scene
      (`UiCollectionTheaterMapSceneId`) is marked unlocked in the profile (`newItemGroups[0x2b..]`).
      −1 ends the sequence (step 100, counter reset); ids below 0x78 select their cue table
      (`g_gameStoryMovieCueTables``[id-0x36]`) and load voice pack `id-0xd` (`SndVoicePacLoad`,
      next step once accepted, else releases the busy loader);
   1: waits for the pack (`SndVoicePacIsLoaded`);
   2: creates the movie task 10040 if missing, else starts the movie (`GfxMovieRequestPlay`) and,
      once accepted, clears the active fader's colours to black and advances;
   3: plays voice cues (`GameStoryMovieUpdateVoiceCues`) until `GfxMovieIsDone`, then removes
      the movie tasks, cancels BGM channel 1 and fades it out over 0.1 s;
   4: once BGM 1 is stopped (or absent), releases the voice pack and advances when the loader is idle;
   5: back to step 0 when the loader is idle;
   any other step (100): removes and destroys the task. */

void GameStoryMovieUpdate(GameStoryMovie *self)
{
  int sceneId;
  int idx;

  switch ((u32)self->step) {
  case 0:
    self->prevMovie = self->movie;
    if (self->set == 4) {
      if (g_gameStoryMovieSeq != 0) {
        self->movie = -1;
      }
    }
    else {
      self->movie = g_gameStoryMovieIds[g_scriptGlobalVars[15]][self->set][g_gameStoryMovieSeq];
      sceneId = UiCollectionTheaterMapSceneId(1, (u16)self->movie);
      if (sceneId != -1) {
        SaveGetProfile()->data->newItemGroups[0x2b + sceneId / 8] |= (u8)(1 << (sceneId % 8));
      }
    }
    g_gameStoryMovieSeq++;
    if (self->movie == -1) {
      self->step = 100;
      g_gameStoryMovieSeq = 0;
    }
    else {
      idx = self->movie - 0x36;
      if (idx < 0x42) {
        self->cueIndex = 0;
        self->cues = g_gameStoryMovieCueTables[idx];
        self->cueFrame = -1;
        if (SndVoicePacLoad(idx + 0x29)) {
          self->step++;
        }
        else if (!SndVoicePacIsIdle()) {
          SndVoicePacRelease();
        }
      }
    }
    break;
  case 1:
    if (SndVoicePacIsLoaded()) {
      self->step++;
    }
    break;
  case 2:
    if (CoreTaskFind(0x2738) == NULL) {
      CoreTaskCreate(0x2738, 100);
    }
    else if (GfxMovieRequestPlay(self->movie)) {
      if (GfxFaderIsReady()) {
        GfxGetActiveFader()->color[2] = 0.0f;
        GfxGetActiveFader()->color[1] = 0.0f;
        GfxGetActiveFader()->color[0] = 0.0f;
        GfxGetActiveFader()->end[2] = 0.0f;
        GfxGetActiveFader()->end[1] = 0.0f;
        GfxGetActiveFader()->end[0] = 0.0f;
        GfxGetActiveFader()->start[2] = 0.0f;
        GfxGetActiveFader()->start[1] = 0.0f;
        GfxGetActiveFader()->start[0] = 0.0f;
      }
      self->step++;
    }
    break;
  case 3:
    if (GfxMovieIsDone()) {
      CoreTaskRemoveAllById(0x2738);
      self->step++;
      SndBgmCancelChannel(1);
      SndBgmQueueStop(0.1f, 1);
    }
    else {
      GameStoryMovieUpdateVoiceCues(self);
    }
    break;
  case 4:
    if (SndBgmPlayerExists(1) && !SndBgmPlayerIsStopped(SndBgmPlayerGet(1))) {
      return;
    }
    if (SndVoicePacIsIdle()) {
      self->step++;
    }
    else {
      SndVoicePacRelease();
    }
    break;
  case 5:
    if (SndVoicePacIsIdle()) {
      self->step = 0;
    }
    break;
  default:
    CoreTaskRemove(&self->base, true);
    break;
  }
}
