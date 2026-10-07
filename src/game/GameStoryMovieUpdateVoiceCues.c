// bdc 0x088ce4d8 GameStoryMovieUpdateVoiceCues
#include "bdc.h"

/* While a movie plays, converts its time to frames (`GfxMovieGetPlayer()->+0x54`, 90 kHz → 30 fps) and
   fires the next cue of the movie's cue table `+0x2c` (6-byte `{u16 frame, u16 voice, u16 ?}`
   entries, index `+0x24`) when its frame is reached. */

void GameStoryMovieUpdateVoiceCues(GameStoryMovie *self)

{
  GfxMoviePlayer *player;
  const u16 *cue;
  s32 frame;
  s32 idx;
  s32 last;

  if (GfxMovieHasPlayer() && self->cues != NULL) {
    player = GfxMovieGetPlayer();
    frame = ((s32)player->pts * 0x1e) / 90000;
    idx = self->cueIndex;
    cue = (const u16 *)self->cues + idx * 3;
    last = self->cueFrame;
    while (last < (s32)cue[0]) {
      if (frame < (s32)cue[0]) {
        self->cueFrame = frame;
        return;
      }
      if (cue[1] == 0) {
        SndBgmPlayVoice(cue[2]);
        idx = self->cueIndex;
        last = self->cueFrame;
      }
      idx++;
      self->cueIndex = idx;
      cue = (const u16 *)self->cues + idx * 3;
    }
    self->cueFrame = frame;
  }
}
