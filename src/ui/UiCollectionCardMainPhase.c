// bdc 0x08987030 UiCollectionCardMainPhase
#include "bdc.h"

/* Main phase (entry 2 of the phase table `0x08a9e838`) of the card collection screen (task 313,
   `maybe_UiScreen313Ctor`; pages of ability cards `"collection_ability_%02d"` in
   `"waku_4_a"`/`"waku_4_b"` frames, large card art `"card_L_%03d"`, help text `"DWCardHelp"`):
   opens all parts, then handles cursor movement (`UiCollectionCardMoveCursor`), page change (`UiCollectionCardChangePage` →
   sub-states 6..9 with `UiCollectionCardAnimatePageArrow`), confirm (opens the detail view:
   sub-states 10..0xe with the help text `UiCollectionCardSetHelpText`), the zoom view (0xf..0x13) and
   cancel (close → sub-state 0x14 → next phase). Every frame ends with the card bob and art pulse updates. */

#define PAD_CIRCLE 0x2000
#define PAD_CROSS 0x4000

void UiCollectionCardMainPhase(UiCollectionCard *self)
{
  u8 done;
  u8 confirm;
  PadState *pad;

  switch ((u32)self->base.phaseStep) {
  case 0:
    UiCollectionCardStartBgTween(self, 0);
    UiCollectionCardStartButtonTween(self, 0);
    UiCollectionCardStartFrameTween(self, 0);
    UiCollectionCardStartArrowTween(self, 0);
    UiCollectionCardStartCardArtTween(self, 0);
    self->base.phaseStep++;
    break;
  case 1:
    done = UiCollectionCardUpdateBgTween(self, 0);
    done += UiCollectionCardUpdateButtonTween(self, 0);
    done += UiCollectionCardUpdateFrameTween(self, 0);
    done += UiCollectionCardUpdateArrowTween(self, 0);
    done += UiCollectionCardUpdateCardArtTween(self, 0);
    if (done == 5) {
      UiCollectionCardEnableCardBob(self, 1);
      UiCollectionCardResetCursor(self);
      self->base.phaseStep++;
    }
    break;
  case 2:
    UiCollectionCardPulseCursor(self);
    UiCollectionCardAnimateCell(self);
    UiCollectionCardZoomCursorCell(self);
    UiPulseStep(((GfxSprite **)self->base.data)[65], (UiPulse *)&self->tweens[65]);
    UiCollectionCardUpdateFastScrollTimer(self);
    confirm = (u8)UiCollectionCardCheckConfirm(self);
    if (confirm != 0) {
      UiCollectionCardResetFastScroll(self);
      if (confirm == 1) {
        /* owned card: open the detail view */
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 0, 0, 0);
        }
        UiCollectionCardEnableCardBob(self, 0);
        UiCollectionCardResetCursor(self);
        UiCollectionCardHideCursor(self);
        UiCollectionCardEnableCardBob(self, 0);
        UiCollectionCardStartCellPress(self);
        self->cancelled = 0;
        self->base.phaseStep = 5;
      } else {
        /* empty slot: error sound only */
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 3, 0, 0);
        }
      }
    } else if (self->base.pad->pressed & PAD_CIRCLE) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 2, 0, 0);
      }
      self->cancelled = 1;
      UiCollectionCardEnableCardBob(self, 0);
      UiCollectionCardHideCursor(self);
      self->base.phaseStep = 3;
    } else if (UiCollectionCardMoveCursor(self) == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 1, 0, 0);
      }
      UiCollectionCardResetCursor(self);
      UiCollectionCardResetFastScroll(self);
    } else if (UiCollectionCardChangePage(self) == 1) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 1, 0, 0);
      }
      UiCollectionCardResetCursor(self);
      UiCollectionCardHideCursor(self);
      UiCollectionCardEnableCardBob(self, 0);
      UiCollectionCardCountPageFlip(self);
      self->base.phaseStep = 6;
    }
    break;
  case 3:
    UiCollectionCardStartBgTween(self, 1);
    UiCollectionCardStartButtonTween(self, 1);
    UiCollectionCardStartFrameTween(self, 1);
    UiCollectionCardStartArrowTween(self, 1);
    UiCollectionCardStartCardArtTween(self, 1);
    self->base.phaseStep++;
    break;
  case 4:
    done = UiCollectionCardUpdateBgTween(self, 1);
    done += UiCollectionCardUpdateButtonTween(self, 1);
    done += UiCollectionCardUpdateFrameTween(self, 1);
    done += UiCollectionCardUpdateArrowTween(self, 1);
    done += UiCollectionCardUpdateCardArtTween(self, 1);
    if (done == 5) {
      self->base.phaseStep = 0x14;
    }
    break;
  case 5:
    if (UiCollectionCardWaitPress(self) == 1) {
      self->base.phaseStep = 10;
    }
    break;
  case 6:
    UiCollectionCardStartButtonTween(self, 1);
    UiCollectionCardStartCardArtTween(self, 1);
    UiCollectionCardStartPageArrowPress(self);
    self->base.phaseStep++;
    break;
  case 7:
    UiCollectionCardAnimatePageArrow(self);
    done = UiCollectionCardUpdateButtonTween(self, 1);
    done += UiCollectionCardUpdateCardArtTween(self, 1);
    if (done == 2) {
      self->page = self->targetPage;
      UiCollectionCardSetPageNumber(self);
      self->base.phaseStep++;
    }
    break;
  case 8:
    UiCollectionCardAnimatePageArrow(self);
    UiCollectionCardStartButtonTween(self, 0);
    UiCollectionCardStartCardArtTween(self, 0);
    self->base.phaseStep++;
    break;
  case 9:
    done = (u8)UiCollectionCardAnimatePageArrow(self);
    done += UiCollectionCardUpdateButtonTween(self, 0);
    done += UiCollectionCardUpdateCardArtTween(self, 0);
    if (done == 3) {
      self->pageDir = 0;
      UiCollectionCardResetCursor(self);
      UiCollectionCardEnableCardBob(self, 1);
      UiCollectionCardDimPageArrows(1.0f, &self->base);
      self->base.phaseStep = 2;
    }
    break;
  case 10:
    UiCollectionCardSetHelpText(self);
    UiCollectionCardStartMoveToDetail(self, 0);
    UiCollectionCardStartDim(self, 0, 0);
    UiCollectionCardStartDetailPanelTween(self, 0);
    UiCollectionCardStartDetailNameTween(self, 0);
    UiCollectionCardStartHelpFade(self, 0);
    self->base.phaseStep++;
    break;
  case 0xb:
    done = UiCollectionCardMoveCardToDetail(self, false);
    done += UiCollectionCardUpdateDim(self, 0, 0);
    done += UiCollectionCardUpdateDetailPanelTween(self, 0);
    done += UiCollectionCardUpdateDetailNameTween(self, 0);
    done += UiCollectionCardUpdateHelpFade(self, 0);
    if (done == 5) {
      UiCollectionCardSetHelpIcon(self, 0);
      self->base.phaseStep++;
    }
    break;
  case 0xc:
    /* detail view: Cross opens the zoom view, Circle goes back to the grid */
    pad = self->base.pad;
    if (pad->pressed & PAD_CROSS) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 0, 0, 0);
      }
      UiCollectionCardSetArtPulse(self, 0);
      self->base.phaseStep = 0xf;
    } else if (pad->pressed & PAD_CIRCLE) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 2, 0, 0);
      }
      UiCollectionCardMarkCardSeen(self);
      UiCollectionCardSetArtPulse(self, 0);
      self->base.phaseStep = 0xd;
    }
    break;
  case 0xd:
    UiCollectionCardStartMoveToDetail(self, 1);
    UiCollectionCardStartDim(self, 1, 0);
    UiCollectionCardStartDetailPanelTween(self, 1);
    UiCollectionCardStartDetailNameTween(self, 1);
    UiCollectionCardStartHelpFade(self, 1);
    self->base.phaseStep++;
    break;
  case 0xe:
    done = UiCollectionCardMoveCardToDetail(self, true);
    done += UiCollectionCardUpdateDim(self, 1, 0);
    done += UiCollectionCardUpdateDetailPanelTween(self, 1);
    done += UiCollectionCardUpdateDetailNameTween(self, 1);
    done += UiCollectionCardUpdateHelpFade(self, 1);
    if (done == 5) {
      UiCollectionCardResetCursor(self);
      UiCollectionCardEnableCardBob(self, 1);
      UiCollectionCardSetHelpIcon(self, 1);
      self->base.phaseStep = 2;
    }
    break;
  case 0xf:
    UiCollectionCardStartMoveToZoom(self, 0);
    UiCollectionCardStartDim(self, 0, 1);
    UiCollectionCardStartAltPanelTween(self, 0);
    self->base.phaseStep++;
    break;
  case 0x10:
    done = UiCollectionCardMoveCardToAltDetail(self, false);
    done += UiCollectionCardUpdateDim(self, 0, 1);
    done += UiCollectionCardUpdateAltPanelTween(self, 0);
    if (done == 3) {
      UiCollectionCardShowBgForZoomView(self, 1);
      self->base.phaseStep++;
    }
    break;
  case 0x11:
    UiCollectionCardZoomCard(self);
    UiCollectionCardDimZoomIcons(self);
    if (self->base.pad->pressed & PAD_CIRCLE) {
      if (SndHasManager()) {
        SndManagerPlay(SndGetManager(), 2, 0, 0);
      }
      UiCollectionCardSetArtPulse(self, 0);
      self->base.phaseStep = 0x12;
    }
    break;
  case 0x12:
    UiCollectionCardStartMoveToZoom(self, 1);
    UiCollectionCardStartDim(self, 1, 1);
    UiCollectionCardStartAltPanelTween(self, 1);
    self->base.phaseStep++;
    break;
  case 0x13:
    done = UiCollectionCardMoveCardToAltDetail(self, true);
    done += UiCollectionCardUpdateDim(self, 1, 1);
    done += UiCollectionCardUpdateAltPanelTween(self, 1);
    if (done == 3) {
      UiCollectionCardShowBgForZoomView(self, 0);
      self->base.phaseStep = 0xc;
    }
    break;
  default:
    /* 0x14 (closed) and anything out of range: leave the phase */
    UiCollectionCardSetResultNone(self);
    self->base.phaseStep = 0;
    self->base.phase++;
    break;
  }
  UiCollectionCardUpdateCardBob(self);
  UiCollectionCardUpdateArtPulse(self);
}
