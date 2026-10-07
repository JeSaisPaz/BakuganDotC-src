// bdc 0x0892a9dc UiHologramViewMainPhase
#include "bdc.h"

/* Phase 2 of the hologram detail view (`UiHologramViewCtor`, task 392): sub-state machine on
   `phaseStep` that blinks in, then runs the view kind's `kindTable` entries one by one (step 2
   reads entry `entryIndex` into `entryOp`/`page`): op 0 shows a picture page (fading the previous
   one out first once `helpDone` is set), op 1 shows a help text until pad bit 0x4000 is pressed
   (sound 0, narration stopped), op 2 runs the help dialog, op 3 runs the tip pages, op 4 fades out; ops >= 5
   leave the step at 2. Help dialog, tip pages and op 4 end in the closing blink (steps 16-17);
   any step >= 18 finishes the view and switches to phase 3. */

void UiHologramViewMainPhase(UiHologramView *self)
{
  u8 idx;
  u16 op;
  s32 next;

  switch (self->base.phaseStep) {
  case 0:
    UiHologramViewStartBlink(self, 0);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 1:
    if (UiHologramViewUpdateBlink(self, 0) == 1) {
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 2:
    idx = self->entryIndex;
    self->entryOp = self->kindTable[idx * 2];
    self->page = self->kindTable[idx * 2 + 1];
    self->entryIndex = idx + 1;
    op = self->entryOp;
    if (op < 5) {
      if (op == 1) {
        self->base.phaseStep = 7;
      } else if (op == 2) {
        self->helpStep = 0;
        next = 0xc;
        if (self->helpDone != 0) {
          next = 3;
        }
        self->base.phaseStep = next;
      } else if (op == 3) {
        UiHologramViewPickMessages(self);
        UiHologramViewResetPages(self);
        self->base.phaseStep = 0xd;
      } else if (op == 4) {
        self->base.phaseStep = 0xe;
      } else {
        next = 5;
        if (self->helpDone != 0) {
          next = 3;
        }
        self->base.phaseStep = next;
      }
    }
    break;
  case 3:
    UiHologramViewStartFade(self, 1);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 4:
    if (UiHologramViewUpdateFade(self, 1) == 1) {
      next = 5;
      if (self->entryOp == 2) {
        next = 0xc;
      }
      self->base.phaseStep = next;
    }
    break;
  case 5:
    self->helpDone = 1;
    UiHologramViewLayoutPage(self);
    UiHologramViewStartFade(self, 0);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 6:
    if (UiHologramViewUpdateFade(self, 0) == 1) {
      self->base.phaseStep = 2;
    }
    break;
  case 7:
    UiHologramViewSetHelpText(self, 0, 0x12);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 8:
    if (UiHologramViewFadeText(self, 0) == 1) {
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 9:
    if ((self->base.pad->pressed & 0x4000) != 0) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiHologramViewStopVoice();
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 10:
    UiHologramViewSetHelpText(self, 1, 0x12);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 0xb:
    if (UiHologramViewFadeText(self, 1) == 1) {
      self->base.phaseStep = 2;
    }
    break;
  case 0xc:
    if (UiHologramViewRunHelp(self) == 1) {
      self->base.phaseStep = 0x10;
    }
    break;
  case 0xd:
    if (UiHologramViewRunPages(self) == 1) {
      if (UiHologramViewNextPage(self) == 1) {
        self->step = 0;
        self->base.phaseStep = 0xd;
      } else {
        self->base.phaseStep = 0x10;
      }
    }
    break;
  case 0xe:
    UiHologramViewStartFade(self, 1);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 0xf:
    if (UiHologramViewUpdateFade(self, 1) == 1) {
      self->base.phaseStep = 0x10;
    }
    break;
  case 0x10:
    UiHologramViewStartBlink(self, 1);
    self->base.phaseStep = self->base.phaseStep + 1;
    break;
  case 0x11:
    if (UiHologramViewUpdateBlink(self, 1) == 1) {
      self->base.phaseStep = 0x12;
    }
    break;
  default:
    UiHologramViewFinish(self);
    self->base.phase = 3;
    self->base.phaseStep = 0;
    break;
  }
}
