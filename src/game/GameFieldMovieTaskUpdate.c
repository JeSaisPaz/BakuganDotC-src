// bdc 0x0882bc4c GameFieldMovieTaskUpdate
#include "bdc.h"

/* Update of the field movie task (`GameFieldMovieTaskCtor`) (slot 2), a step machine on `+0x14`: 10
   loads the movie's voice-cue table (`GameStoryMovieSetCueTable`), locks all units
   (`BtlSetControlLockAll`) and fades out (black, or white over 60 frames for movie 0x74); 0xc
   pauses the camera and talk tasks (`CoreTaskSetFlags`); 0x14 creates the movie task 0x2738
   (`CoreTaskCreate`) and requests the movie (`GfxMovieRequestPlay`); 0x15 plays voice cues
   (`GameStoryMovieUpdateVoiceCues`) until `GfxMovieIsDone`, then removes the movie task and
   stops BGM channel 1 (`SndBgmQueueStop`); 0x16 waits for that BGM player to stop; 0x17 resumes
   the paused tasks and unlocks the units; 0x18/0x19 fade back in; 100 restores the stage clear
   colour (`BtlStageGetClearColor`) for movie 0x74 and removes the task. The colours are
   copied as 4-float vectors (lv.q/sv.q in the binary). */

#define FIELD_MOVIE_WHITE 0x74
#define FIELD_MOVIE_TASK_ID 0x2738

void GameFieldMovieTaskUpdate(CoreTask *task)
{
  GameStoryMovie *self = (GameStoryMovie *)task;
  /* transparent / opaque fade colours: black, or white for movie 0x74 */
  float clear[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  float opaque[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  s32 step;

  if (self->movie == FIELD_MOVIE_WHITE) {
    {
      float *dst = g_gfxDisplay->clearColor;
      dst[0] = g_colorWhite.x;
      dst[1] = g_colorWhite.y;
      dst[2] = g_colorWhite.z;
      dst[3] = g_colorWhite.w;
    }
  }
  if (self->movie == FIELD_MOVIE_WHITE) {
    clear[0] = 1.0f;
    clear[1] = 1.0f;
    clear[2] = 1.0f;
    clear[3] = 0.0f;
    opaque[0] = 1.0f;
    opaque[1] = 1.0f;
    opaque[2] = 1.0f;
    opaque[3] = 1.0f;
  }

  step = self->step;
  switch (step) {
  case 0:
    self->step = 10;
    /* fall through */
  case 10:
    if (GameStoryMovieSetCueTable(task, self->movie)) {
      BtlSetControlLockAll(1);
      {
      float *dst = GfxGetActiveFader()->start;
      dst[0] = clear[0];
      dst[1] = clear[1];
      dst[2] = clear[2];
      dst[3] = clear[3];
    }
      {
      float *dst = GfxGetActiveFader()->end;
      dst[0] = opaque[0];
      dst[1] = opaque[1];
      dst[2] = opaque[2];
      dst[3] = opaque[3];
    }
      if (self->movie == FIELD_MOVIE_WHITE) {
        GfxFaderStart(GfxGetActiveFader(), 60);
      } else {
        GfxFaderStart(GfxGetActiveFader(), 15);
      }
      self->step++;
    }
    break;
  case 11:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->step++;
    }
    break;
  case 12:
    if (BtlCameraTaskExists()) {
      CoreTaskSetFlags((CoreTask *)BtlGetCameraTask(), 3);
    }
    if (UiTalkTaskExists()) {
      CoreTaskSetFlags(&UiGetTalkTask()->base, 3);
    }
    {
      float *dst = GfxGetActiveFader()->end;
      dst[0] = clear[0];
      dst[1] = clear[1];
      dst[2] = clear[2];
      dst[3] = clear[3];
    }
    GfxFaderStart(GfxGetActiveFader(), 1);
    self->step++;
    break;
  case 13:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->step = 0x14;
    }
    break;
  case 0x14:
    if (CoreTaskFind(FIELD_MOVIE_TASK_ID) == NULL) {
      CoreTaskCreate(FIELD_MOVIE_TASK_ID, 100);
      if (self->movie == FIELD_MOVIE_WHITE) {
        {
      float *dst = g_gfxDisplay->clearColor;
      dst[0] = g_colorWhite.x;
      dst[1] = g_colorWhite.y;
      dst[2] = g_colorWhite.z;
      dst[3] = g_colorWhite.w;
    }
      }
    } else if (GfxMovieRequestPlay((u32)self->movie)) {
      self->step++;
    }
    break;
  case 0x15:
    if (GfxMovieIsDone()) {
      CoreTaskRemoveAllById(FIELD_MOVIE_TASK_ID);
      {
      float *dst = GfxGetActiveFader()->start;
      dst[0] = opaque[0];
      dst[1] = opaque[1];
      dst[2] = opaque[2];
      dst[3] = opaque[3];
    }
      {
      float *dst = GfxGetActiveFader()->end;
      dst[0] = opaque[0];
      dst[1] = opaque[1];
      dst[2] = opaque[2];
      dst[3] = opaque[3];
    }
      GfxFaderStart(GfxGetActiveFader(), 1);
      self->step++;
      SndBgmCancelChannel(1);
      SndBgmQueueStop(0.1f, 1);
    } else {
      GameStoryMovieUpdateVoiceCues(self);
    }
    break;
  case 0x16:
    if (!SndBgmPlayerExists(1) || SndBgmPlayerIsStopped(SndBgmPlayerGet(1))) {
      self->step++;
    }
    break;
  case 0x17:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      if (BtlCameraTaskExists()) {
        CoreTaskClearFlags((CoreTask *)BtlGetCameraTask(), 3);
      }
      if (UiTalkTaskExists()) {
        CoreTaskClearFlags(&UiGetTalkTask()->base, 3);
      }
      BtlSetControlLockAll(0);
      self->step++;
    }
    break;
  case 0x18:
    {
      float *dst = GfxGetActiveFader()->start;
      dst[0] = opaque[0];
      dst[1] = opaque[1];
      dst[2] = opaque[2];
      dst[3] = opaque[3];
    }
    {
      float *dst = GfxGetActiveFader()->end;
      dst[0] = clear[0];
      dst[1] = clear[1];
      dst[2] = clear[2];
      dst[3] = clear[3];
    }
    if (self->movie == FIELD_MOVIE_WHITE) {
      GfxFaderStart(GfxGetActiveFader(), 60);
    } else {
      GfxFaderStart(GfxGetActiveFader(), 15);
    }
    self->step++;
    break;
  case 0x19:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->step = 100;
    }
    break;
  case 100:
    if (self->movie == FIELD_MOVIE_WHITE) {
      GfxDisplay *disp = g_gfxDisplay;
      {
      const float *src = BtlStageGetClearColor();
      disp->clearColor[0] = src[0];
      disp->clearColor[1] = src[1];
      disp->clearColor[2] = src[2];
      disp->clearColor[3] = src[3];
    }
    }
    CoreTaskRemove(task, true);
    break;
  default:
    break;
  }
}
