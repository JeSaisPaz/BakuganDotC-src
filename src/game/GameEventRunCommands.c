// bdc 0x088eece0 GameEventRunCommands
#include "bdc.h"

/* Runs the command list of the field event task (task id 470, `GameEvent470Ctor`, base
   `GameEventCtor`; message pack `mes_f<area>_<room>_<lang>.bin`, command script `cmd_f%d_%02d.cut`)
   on sub-state `runStep`. Step 0: once the screen fader is finished it clears `waitFrames`, steps
   past the header (`cmd`, `cmdIndex`), stops all actions, sets `bgmTrack` to 0xff (no change) and
   copies `blocking == 1` into flag 4 of `flags`. Step 1: executes commands with
   `GameEventExecCommand` until one clears `blocking` or `cmdEnd` is reached, then switches the
   field BGM when `bgmTrack != 0xff` (cancel channel 0, stop over 0.4 s, play the track unless 0),
   removes task 420 (0x1a4) if present, resets the active camera (`GameFieldCameraReset(cam, 1, 0)`)
   when its mode is non-zero and, with flag 4, starts a 5-frame fade from black. Step 2 waits for
   the fader. Step 3 resets the message position (`msgPos = msgCount`, `msgState = 0`), sets
   `state` 2, clears flag 1 and returns to step 0. Other steps do nothing. */

void GameEventRunCommands(GameEvent *self)
{
  GameEventCommand *cmd;
  GfxFader *fader;
  GfxCamera *cam;
  u8 step;

  step = self->runStep;
  if (step < 2) {
    if (step == 0) {
      if (GfxFaderIsFinished(GfxGetActiveFader())) {
        self->waitFrames = 0;
        self->cmdIndex = self->cmdIndex + 1;
        self->cmd = self->cmd + 1;
        GameEventActionListStopAll(self->actions);
        self->bgmTrack = 0xff;
        if (self->blocking == 1) {
          self->flags = self->flags | 4;
        } else {
          self->flags = self->flags & ~4;
        }
        self->runStep = self->runStep + 1;
      }
    } else {
      while (self->cmdIndex < self->cmdEnd) {
        cmd = self->cmd;
        GameEventExecCommand(self, cmd->op, cmd->flag, cmd->arg);
        if (self->blocking == 0) {
          break;
        }
        self->cmdIndex = self->cmdIndex + 1;
        self->cmd = self->cmd + 1;
      }
      if (self->bgmTrack != 0xff) {
        SndBgmCancelChannel(0);
        SndBgmQueueStop(0.4f, 0);
        if (self->bgmTrack != 0) {
          SndBgmQueuePlay(0, self->bgmTrack, 1, 0);
        }
      }
      if (CoreTaskExists(0x1a4) != 0) {
        CoreTaskRemove(CoreTaskFind(0x1a4), true);
      }
      cam = g_gfxActiveCamera;
      if (((GameFieldCamera *)cam)->mode != 0) {
        GameFieldCameraReset((GameFieldCamera *)cam, 1, 0);
      }
      if ((self->flags & 4) != 0) {
        fader = GfxGetActiveFader();
        fader->start[0] = 0.0f;
        fader->start[1] = 0.0f;
        fader->start[2] = 0.0f;
        fader->start[3] = 1.0f;
        fader = GfxGetActiveFader();
        fader->end[0] = 0.0f;
        fader->end[1] = 0.0f;
        fader->end[2] = 0.0f;
        fader->end[3] = 0.0f;
        GfxFaderStart(GfxGetActiveFader(), 5);
      }
      self->runStep = self->runStep + 1;
    }
  } else if (step < 3) {
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->runStep = self->runStep + 1;
    }
  } else if (step < 4) {
    self->msgPos = self->msgCount;
    self->msgState = 0;
    self->state = 2;
    self->flags = self->flags & ~1;
    self->runStep = 0;
  }
}
