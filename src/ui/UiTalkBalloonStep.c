// bdc 0x088c98c8 UiTalkBalloonStep
#include "bdc.h"

/* Main step of the talk balloon task (`UiTalkBalloon`, speech-bubble message window of field
   events: 0x220 bytes, vtable `0x08af2d4c`, created by `UiTalkBalloonCreate`) (called from
   `UiTalkBalloonUpdate` with the pad). Does nothing (returns 0) while `textCursor` is NULL.
   State 0 copies `origin` to `pen` (timer forced to 5 without a frame or with
   `g_talkBalloonSkipAnim`); state 1, from timer 5 on, adds `revealSpeed` to `revealTimer` and
   prints one glyph per whole unit of it (`UiFontNextGlyph`, `UiFontGlyphAdvance`, word wrap
   with `UiTextCanBreakBefore` against 480 - 2 x `g_uiTalkBalloonVecB`.x + 6), creating a glyph
   sprite per character (`UiTalkBalloonGlyphCtor`) and a highlight record per line; button 0x4000
   (when task 100 does not exist) or the skip flag reveals the rest of the page. Control codes:
   -1 end of text, -3 new line, -4 new page, -6..-10 outline colour, -11 voice line on BGM player 1
   (`SndBgmPlayerPlayTrack`), -16 header line, -17/-5 spaces, -18 choice list, -13..-15/-19
   inserted text. State 2 waits for a button at page end and restarts state 1; state 3 runs the
   choice cursor (`"huki_check"`/`"huki_cursor"` icons, `UiTalkBalloonIconCtor`, result to script
   variable 2) or waits at the end of the text (`UiTalkBalloonClearText`); state 4 waits for the
   frame to close and state 10 for the close button. Returns 1 after the balloon removed and
   destroyed itself (`CoreTaskRemove`), else 0 after updating the printer's sprite layer
   (`GfxSpriteLayerUpdateAll`). A new highlight record starts from the VFPU bank zero vector
   (C720) plus the pen. */

s32 UiTalkBalloonStep(UiTalkBalloon *self, PadState *pad)
{
  float wordEndX;
  void *pauseTask;
  UiTalkBalloonSprite *sprite;
  void *mem;
  bool fromLow;
  u16 advanceMask;
  s32 len;
  s32 nextLen;
  s32 scanPos;
  s32 peekPos;
  s32 code;
  s32 glyph;
  s32 wordGlyphs;
  s32 held;
  s32 n;
  s32 voiceIndex;
  SndBgmPlayer *player;
  float adv;
  float half;
  const float *colorSrc;

  pauseTask = CoreTaskFind(100);
  self->stateTimer = self->stateTimer + 1;
  if (self->textCursor == NULL) {
    return 0;
  }
  switch (self->state) {
  case 0:
    if (self->hasFrame == 0 || g_talkBalloonSkipAnim != 0) {
      self->stateTimer = 5;
    }
    self->pen[0] = self->origin[0];
    self->pen[1] = self->origin[1];
    self->pen[2] = self->origin[2];
    self->pen[3] = self->origin[3];
    self->state = self->state + 1;
    /* fall through */
  case 1:
    if (self->stateTimer < 5) {
      break;
    }
    self->revealTimer = self->revealTimer + self->revealSpeed;
    for (;;) {
      if (self->revealTimer < 1.0f && self->headerLine == 0) {
        break;
      }
      advanceMask = 0x4000;
      if (pauseTask != NULL) {
        advanceMask = 0;
      }
      if (self->stateTimer >= 7 && self->hasIcon == 1) {
        if ((pad->pressed & advanceMask) != 0 || g_talkBalloonSkipAnim != 0) {
          self->revealTimer = 1000.0f;
        }
      }
      if (self->headerLine == 0) {
        self->revealTimer = self->revealTimer - 1.0f;
      }
      len = 0;
      if (self->inInsert != 0) {
        code = UiFontNextGlyph(self->insertText, &len);
        self->insertText = self->insertText + len;
      }
      else {
        code = UiFontNextGlyph(self->textCursor, &len);
        self->textCursor = self->textCursor + len;
      }
      if (code == -11 && self->voiceTable != NULL) {
        voiceIndex = (u8)*self->textCursor + self->voiceBase;
        player = SndBgmPlayerGet(1);
        SndBgmPlayerPlayTrack(player, self->voiceTable[voiceIndex], 0, 0);
      }
      if (code == -12 || code == -11) {
        /* skip the argument byte */
        self->textCursor = self->textCursor + 1;
        continue;
      }

      /* word wrap: measure the word starting at the next glyph */
      nextLen = 0;
      if (UiTextCanBreakBefore(code, UiFontNextGlyph(self->textCursor, &nextLen)) != 0) {
        scanPos = 0;
        wordGlyphs = 0;
        /* from pen.x + floor(glyphWidth * 0.5) (vf2id.s rounds towards -inf) */
        wordEndX = self->pen[0];
        half = (float)(s32)__builtin_floorf(self->glyphWidth * 0.5f);
        wordEndX = wordEndX + half;
        for (;;) {
          glyph = UiFontNextGlyph(self->textCursor, &scanPos);
          if (glyph <= 0) {
            /* French: a space (-5) before a glyph that cannot start a line belongs to the word */
            if (!SaveLanguageIsFrench() || glyph != -5) {
              break;
            }
            peekPos = scanPos;
            if (UiTextCanBreakBefore(code, UiFontNextGlyph(self->textCursor, &peekPos)) != 0) {
              break;
            }
          }
          wordGlyphs = wordGlyphs + 1;
          if (glyph == -5) {
            half = (float)(s32)__builtin_floorf(self->glyphWidth * 0.5f);
            wordEndX = wordEndX + half;
          }
          else {
            adv = UiFontGlyphAdvance(self->printer, glyph);
            wordEndX = wordEndX + adv;
          }
        }
        if (wordGlyphs > 0) {
          if (!(wordEndX <= (480.0f - g_uiTalkBalloonVecB.x * 2.0f) + 6.0f)) {
            code = -3;
          }
        }
      }
      /* a new line past the last line becomes a new page */
      if (self->lineCount > 0 && code == -3 && !(self->lineIndex + 1 < self->lineCount)) {
        code = -4;
      }

      if (code < -5 && code >= -10) {
        /* outline colour */
        switch (code) {
        case -10:
          colorSrc = &g_colorBlue.x;
          break;
        case -9:
          colorSrc = &g_colorRed.x;
          break;
        case -8:
          colorSrc = &g_colorBlack.x;
          break;
        case -7:
          colorSrc = &g_colorWhite.x;
          break;
        default: /* -6: back to the text colour */
          colorSrc = self->printer->color;
          break;
        }
        self->printer->outlineColor[0] = colorSrc[0];
        self->printer->outlineColor[1] = colorSrc[1];
        self->printer->outlineColor[2] = colorSrc[2];
        self->printer->outlineColor[3] = colorSrc[3];
        self->revealTimer = self->revealTimer + 1.0f;
        continue;
      }

      if (code == -1) {
        /* end of the inserted text, or of the message */
        if (self->inInsert != 0) {
          self->inInsert = 0;
          if (self->headerLine != 0) {
            self->headerLine = 0;
            if (self->pageFlags[2] != 0) {
              self->pen[1] = self->pen[1] + 4.0f;
            }
            self->lineIndent = 16.0f;
          }
          continue;
        }
        if (self->choiceMode != 0) {
          n = self->choiceCount + 1;
          self->choiceCount = n;
          self->choiceLineStart[n] = -(self->glyphSerial + self->glyphSerial);
        }
        if (self->iconSprite != NULL && self->pageFlags[1] == 0) {
          ((UiTalkBalloonSprite *)self->iconSprite)->state = 1;
        }
        self->state = 3;
        if (self->choiceMode != 0) {
          self->choiceFrame = -self->choiceLineStart[0];
        }
        self->stateTimer = 0;
        break;
      }
      if (code == -3) {
        /* new line */
        self->lineHighlighted = 0;
        if (self->inInsert != 0) {
          self->inInsert = 0;
        }
        if (self->headerLine != 0) {
          self->headerLine = 0;
          if (self->pageFlags[2] != 0) {
            self->pen[1] = self->pen[1] + 4.0f;
          }
          self->lineIndent = 16.0f;
        }
        self->pen[0] = self->origin[0] + self->lineIndent;
        self->pen[1] = self->pen[1] + self->lineHeight;
        if (self->choiceMode != 0) {
          n = self->choiceCount + 1;
          self->choiceCount = n;
          self->choiceLineStart[n] = -(self->glyphSerial + self->glyphSerial);
        }
        self->lineIndex = self->lineIndex + 1;
        continue;
      }
      if (code == -4) {
        /* new page */
        self->lineIndex = 0;
        if (self->pageFlags[0] != 0) {
          self->pen[0] = self->origin[0] + self->lineIndent;
          self->lineHighlighted = 0;
          self->pen[1] = self->pen[1] + self->lineHeight;
          continue;
        }
        self->pen[0] = self->origin[0];
        self->pen[1] = self->origin[1];
        self->pen[2] = self->origin[2];
        self->pen[3] = self->origin[3];
        self->pen[0] = self->pen[0] + self->lineIndent;
        if (self->pageFlags[2] != 0 && self->hasHeader != 0) {
          self->pen[1] = (self->lineHeight + 4.0f) + self->pen[1];
        }
        self->state = 2;
        if (self->iconSprite != NULL) {
          ((UiTalkBalloonSprite *)self->iconSprite)->state = 1;
        }
        break;
      }
      if ((code < -12 && code >= -15) || code == -19) {
        self->inInsert = 1;
        continue;
      }
      if (code == -16) {
        self->headerLine = 1;
        self->hasHeader = 1;
        continue;
      }
      if (code == -18) {
        self->choiceTop = self->pen[1];
        self->choiceMode = 1;
        self->choiceLineStart[0] = -(self->glyphSerial + self->glyphSerial);
        continue;
      }
      if (code == -2) {
        continue;
      }
      if (code == -17) {
        self->pen[0] = self->pen[0] + self->glyphWidth;
        continue;
      }
      if (code == -5) {
        adv = UiFontGlyphAdvance(self->printer, 0);
        self->pen[0] = self->pen[0] + adv;
        continue;
      }

      /* printable glyph */
      if (self->lineHighlighted == 0) {
        /* new highlight record = bank zero vector C720 + pen */
        n = self->highlightCount;
        self->highlights[n][0] = 0.0f;
        self->highlights[n][1] = 0.0f;
        self->highlights[n][2] = 0.0f;
        self->highlights[n][3] = 0.0f;
        self->highlights[n][0] = self->highlights[n][0] + self->pen[0];
        self->highlights[n][1] = self->highlights[n][1] + self->pen[1];
        self->highlights[n][2] = self->highlights[n][0];
        if (n < 7) {
          self->highlightCount = self->highlightCount + 1;
        }
      }
      self->lineHighlighted = 1;
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      mem = MemAlloc((sizeof(UiTalkBalloonSprite) + 0xf) & ~0xfu, NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      sprite = NULL;
      if (mem != NULL) {
        UiTalkBalloonGlyphCtor((GfxSprite *)mem, code, self->pen, self);
        sprite = (UiTalkBalloonSprite *)mem;
      }
      adv = UiFontGlyphAdvance(self->printer, code);
      self->pen[0] = self->pen[0] + adv;
      /* highlightCount >= 1 here: the record of this line is the last one */
      n = self->highlightCount;
      self->highlights[n - 1][3] = (self->pen[0] - self->highlights[n - 1][2]) - self->glyphWidth;
      if (self->headerLine != 0) {
        sprite->keep = 1;
      }
      if (self->choiceMode != 0) {
        sprite->frame = -(self->glyphSerial + self->glyphSerial);
        sprite->holdTimer = self->choiceCount + 1;
      }
      self->glyphSerial = self->glyphSerial + 1;
      if (self->autoAdvance < -1) {
        self->autoAdvance = self->autoAdvance + 1;
      }
    }
    break;

  case 2:
    /* page end: wait for the button, then print the next page */
    if (self->autoAdvance > 0) {
      self->autoAdvance = self->autoAdvance - 1;
      if (self->autoAdvance == 0) {
        self->autoAdvance = 1;
        UiTalkBalloonClearText(self);
        break;
      }
    }
    else if (self->autoAdvance < 0) {
      self->autoAdvance = self->autoAdvance + 1;
      if (self->autoAdvance == 0) {
        self->autoAdvance = 1;
        UiTalkBalloonClearText(self);
        break;
      }
    }
    held = 0;
    if ((pad->buttons & 0x2000) != 0) {
      held = self->holdFrames + 1;
    }
    self->holdFrames = held;
    if (self->hasIcon != 1) {
      break;
    }
    if ((pad->pressed & 0x4000) == 0 && self->holdFrames < 26 && g_talkBalloonSkipAnim == 0) {
      break;
    }
    UiTalkBalloonRemoveGlyphs(self, false);
    if (self->iconSprite != NULL) {
      ((UiTalkBalloonSprite *)self->iconSprite)->state = 0;
    }
    self->state = 1;
    self->revealTimer = 1000.0f;
    self->highlightCount = self->hasHeader != 0;
    self->lineHighlighted = 0;
    self->holdFrames = 0;
    break;

  case 3:
    if (self->choiceMode != 0) {
      /* choice list: move the selection with up/down, confirm with 0x2000 */
      if (self->cursorSprite == NULL) {
        MemLock();
        fromLow = MemIsAllocFromLow();
        MemSetAllocFromLow(true);
        mem = MemAlloc((sizeof(UiTalkBalloonSprite) + 0xf) & ~0xfu, NULL, 0);
        MemSetAllocFromLow(fromLow);
        MemUnlock();
        sprite = NULL;
        if (mem != NULL) {
          UiTalkBalloonIconCtor((GfxSprite *)mem, GfxFindTexture("huki_check"), 3, self);
          sprite = (UiTalkBalloonSprite *)mem;
        }
        self->cursorSprite = (GfxSprite *)sprite;
      }
      self->choiceFrame = self->choiceFrame + 1;
      if (self->stateTimer < 8) {
        break;
      }
      n = ((UiTalkBalloonSprite *)self->cursorSprite)->state;
      if (n > 0) {
        if (n == 2) {
          UiTalkBalloonClearText(self);
          self->hasHeader = 0;
          self->highlightCount = 0;
          self->lineHighlighted = 0;
        }
        break;
      }
      if (n < 0) {
        break;
      }
      MemLock();
      fromLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      mem = MemAlloc((sizeof(UiTalkBalloonSprite) + 0xf) & ~0xfu, NULL, 0);
      MemSetAllocFromLow(fromLow);
      MemUnlock();
      sprite = NULL;
      if (mem != NULL) {
        UiTalkBalloonIconCtor((GfxSprite *)mem, GfxFindTexture("huki_cursor"), 4, self);
        sprite = (UiTalkBalloonSprite *)mem;
      }
      sprite->frame = self->stateTimer;
      if ((pad->pressed & 0x2000) != 0) {
        g_scriptGlobalVars[2] = self->choiceIndex;
        ((UiTalkBalloonSprite *)self->cursorSprite)->state = 1;
        break;
      }
      if (self->choiceIndex > 0 && (pad->repeat & 0x10) != 0) {
        n = self->choiceIndex - 1;
        self->choiceIndex = n;
        self->choiceFrame = -self->choiceLineStart[n];
        break;
      }
      if (self->choiceIndex < self->choiceCount - 1 && (pad->repeat & 0x40) != 0) {
        n = self->choiceIndex + 1;
        self->choiceIndex = n;
        self->choiceFrame = -self->choiceLineStart[n];
      }
      break;
    }
    /* end of the text */
    if (self->autoAdvance > 0) {
      self->autoAdvance = self->autoAdvance - 1;
      if (self->autoAdvance == 0) {
        self->autoAdvance = 1;
        UiTalkBalloonClearText(self);
        break;
      }
    }
    else if (self->autoAdvance < 0) {
      self->autoAdvance = self->autoAdvance + 1;
      if (self->autoAdvance == 0) {
        self->autoAdvance = 1;
        UiTalkBalloonClearText(self);
        break;
      }
    }
    if (self->hasIcon != 1 || self->pageFlags[1] != 0) {
      self->state = 10;
      break;
    }
    held = 0;
    if ((pad->buttons & 0x2000) != 0) {
      held = self->holdFrames + 1;
    }
    self->holdFrames = held;
    if ((pad->pressed & 0x4000) != 0 || self->holdFrames >= 26 || g_talkBalloonSkipAnim != 0) {
      UiTalkBalloonClearText(self);
      self->hasHeader = 0;
      self->highlightCount = 0;
      self->lineHighlighted = 0;
      self->holdFrames = 0;
    }
    break;

  case 4:
    /* wait for the frame sprite to finish closing */
    if (self->frameSprite != NULL && ((UiTalkBalloonSprite *)self->frameSprite)->state != 3) {
      break;
    }
    if (self->pageFlags[1] != 0) {
      self->state = 10;
      break;
    }
    CoreTaskRemove(&self->base, true);
    return 1;

  case 10:
    if (self->autoAdvance > 0) {
      self->autoAdvance = self->autoAdvance - 1;
      if (self->autoAdvance == 0) {
        CoreTaskRemove(&self->base, true);
        return 1;
      }
    }
    else if (self->autoAdvance < 0) {
      self->autoAdvance = self->autoAdvance + 1;
      if (self->autoAdvance == 0) {
        CoreTaskRemove(&self->base, true);
        return 1;
      }
    }
    if (self->holdOpen == 0 && (pad->pressed & 0x4000) != 0) {
      CoreTaskRemove(&self->base, true);
      return 1;
    }
    break;

  default:
    break;
  }
  GfxSpriteLayerUpdateAll(&self->printer->layer);
  return 0;
}
