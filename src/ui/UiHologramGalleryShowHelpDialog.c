// bdc 0x08922590 UiHologramGalleryShowHelpDialog
#include "bdc.h"

/* Shows message msgKind of the `DWHologramHelp` table of the hologram gallery screen (task 391,
   `UiHologramGalleryCtor`) in the yes/no dialog (task 510, `UiConfirmDialogCtor`).
   Step 0 creates the dialog: kinds 11 and 10 open with cursor 1 and unk84 0; kind 7 formats
   g_langStrings[24] with save-profile word 0x2d through the message window instead of using the
   table (cursor 1, unk84 0, unk92 cleared); kind 6 cursor 1 / unk84 1; any other kind cursor 0 /
   unk84 1. Step 1 waits for the dialog task to end and stores msgDeclined (0 when
   UiConfirmDialogGetResult is 1, else 1), moving to step 2. Both steps set g_uiKeepSharedBg and
   return 0; from step 2 on it returns 1. */

s32 UiHologramGalleryShowHelpDialog(UiHologramGallery *self)

{
  UiConfirmDialog *dialog;
  void *table;
  u32 kind;
  char *text;
  char *format;
  u32 word;
  char buf[256];

  if (self->msgStep > 0) {
    if (self->msgStep >= 2) {
      return 1;
    }
    g_uiKeepSharedBg = 1;
    if (CoreTaskExists(0x1fe) == 0) {
      if (UiConfirmDialogGetResult() == 1) {
        self->msgDeclined = 0;
      }
      else {
        self->msgDeclined = 1;
      }
      self->msgStep = 2;
    }
    return 0;
  }
  g_uiKeepSharedBg = 1;
  dialog = (UiConfirmDialog *)CoreTaskCreate(0x1fe,100);
  table = SaveFindLocalizedBin("DWHologramHelp");
  UiMesTableRelocate(table);
  kind = self->msgKind;
  if (kind == 11) {
    UiConfirmDialogSetMessage(((char **)table)[kind]);
    dialog->cursor = 1;
    dialog->unk84 = 0;
  }
  else if (kind == 10) {
    UiConfirmDialogSetMessage(((char **)table)[kind]);
    dialog->cursor = 1;
    dialog->unk84 = 0;
  }
  else if (kind == 7) {
    format = g_langStrings[24];
    word = SaveProfileGetWord(SaveGetProfile(),0x2d);
    sprintf(buf,format,word);
    UiMsgWindowPrintfUtf8((UiMsgWindow *)UiMsgWindowGet(),buf);
    UiConfirmDialogSetMessage(((UiMsgWindow *)UiMsgWindowGet())->text);
    dialog->unk92 = 0;
    dialog->cursor = 1;
    dialog->unk84 = 0;
  }
  else {
    text = ((char **)table)[kind];
    if (kind == 6) {
      UiConfirmDialogSetMessage(text);
      dialog->cursor = 1;
      dialog->unk84 = 1;
    }
    else {
      UiConfirmDialogSetMessage(text);
      dialog->cursor = 0;
      dialog->unk84 = 1;
    }
  }
  self->msgStep = self->msgStep + 1;
  return 0;
}
