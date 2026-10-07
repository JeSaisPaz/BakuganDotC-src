// bdc 0x089ec060 UiMsgBoxSetText
#include "bdc.h"

/* Replaces the box's font-encoded text: frees the old buffer at `text`, and for non-NULL text
   measures it with the box printer (`UiTextBoxGetPrinter``(textBox)`, `UiTextMeasure`, size
   stored in `extents[2]`/`extents[3]`), allocates a low-heap copy and `memcpy`s it into `text`.
   Returns true when the text was stored; false for NULL text or a box without a printer (the old
   buffer is freed either way). */

bool UiMsgBoxSetText(UiMsgBox *self, const void *encodedText)
{
  void *old = self->text;
  bool stored = false;
  void *printer;
  float height;
  int byteCount;
  float width;
  bool wasLow;
  void *copy;

  if (old != NULL) {
    MemLock();
    MemFree(old, NULL, 0);
    MemUnlock();
    self->text = NULL;
  }
  if (encodedText != NULL) {
    printer = UiTextBoxGetPrinter(self->textBox);
    height = 0.0f;
    byteCount = 0;
    if (printer != NULL) {
      width = 0.0f;
      UiTextMeasure(0.0f, printer, (char *)encodedText, &width, &height, &byteCount);
      self->extents[2] = height;
      self->extents[3] = width;
      MemLock();
      wasLow = MemIsAllocFromLow();
      MemSetAllocFromLow(true);
      copy = MemAlloc(byteCount, NULL, 0);
      MemSetAllocFromLow(wasLow);
      MemUnlock();
      self->text = copy;
      memcpy(copy, encodedText, (size_t)byteCount);
      stored = true;
    }
  }
  return stored;
}
