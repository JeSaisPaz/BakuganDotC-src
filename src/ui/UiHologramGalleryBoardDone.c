// bdc 0x089240e4 UiHologramGalleryBoardDone
#include "bdc.h"

/* Steps the board tweens started by `UiHologramGalleryTweenBoard` on the hologram gallery
   screen (task 391, `UiHologramGalleryCtor`): `UiTweenUpdate` (scale 1.0 → 1.0, 16 frames,
   flags 9, `fadeOut = hide`, tween slot = sprite index) over the same sprite groups and with the
   same presence checks: `boardIcons[i] != 0xff` (sprites 0xa2.. and 0x45..), `slotIds[i] != 0xff`
   (0x9f..), `i < slotCount` (0xaa..), `i < slotCount` and the save profile's
   `placedHolograms[i]` empty (0x62..) or set (0xa6.., 0x41..), `boardMarks[i][0] != 0xff`
   (0xae.., 0x93.., 0x9c..), sprites 0xb1/0xb2 always. Returns true once any of the stepped
   tweens reports finished (u8 count of finished steps != 0). */

#define BOARD_SPRITE(n) (((UiHologramGalleryData *)self->base.data)->sprites[(n)])
#define BOARD_STEP(n) \
  UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, BOARD_SPRITE(n), &self->tweens[(n)], 9)

bool UiHologramGalleryBoardDone(UiHologramGallery *self, u8 hide)
{
  u8 finished = 0;
  s32 i;
  u8 k;

  /* board icons */
  for (i = 0xa2; i < 0xa6; i++) {
    if (self->boardIcons[(u8)(i - 0xa2)] != 0xff)
      finished = (u8)(finished + BOARD_STEP(i));
  }
  /* holograms on the board icons */
  for (i = 0x45; i < 0x49; i++) {
    if (self->boardIcons[(u8)(i - 0x45)] != 0xff)
      finished = (u8)(finished + BOARD_STEP(i));
  }
  /* field pictures */
  for (i = 0x9f; i < 0xa2; i++) {
    if (self->slotIds[(u8)(i - 0x9f)] != 0xff)
      finished = (u8)(finished + BOARD_STEP(i));
  }
  /* hologram slot frames */
  for (i = 0xaa; i < 0xae; i++) {
    if ((u8)(i - 0xaa) < self->slotCount)
      finished = (u8)(finished + BOARD_STEP(i));
  }
  /* empty slots */
  for (i = 0x62; i < 0x66; i++) {
    k = i - 0x62;
    if (k < self->slotCount && SaveGetProfile()->data->placedHolograms[k] == 0)
      finished = (u8)(finished + BOARD_STEP(i));
  }
  /* placed hologram icons */
  for (i = 0xa6; i < 0xaa; i++) {
    k = i - 0xa6;
    if (k < self->slotCount && SaveGetProfile()->data->placedHolograms[k] != 0)
      finished = (u8)(finished + BOARD_STEP(i));
  }
  /* placed hologram cells */
  for (i = 0x41; i < 0x45; i++) {
    k = i - 0x41;
    if (k < self->slotCount && SaveGetProfile()->data->placedHolograms[k] != 0)
      finished = (u8)(finished + BOARD_STEP(i));
  }
  /* board marks */
  for (i = 0xae; i < 0xb1; i++) {
    if (self->boardMarks[(u8)(i - 0xae)][0] != 0xff)
      finished = (u8)(finished + BOARD_STEP(i));
  }
  for (i = 0x93; i < 0x96; i++) {
    if (self->boardMarks[(u8)(i - 0x93)][0] != 0xff)
      finished = (u8)(finished + BOARD_STEP(i));
  }
  for (i = 0x9c; i < 0x9f; i++) {
    if (self->boardMarks[(u8)(i - 0x9c)][0] != 0xff)
      finished = (u8)(finished + BOARD_STEP(i));
  }
  /* always-present sprites */
  for (i = 0xb1; i < 0xb3; i++)
    finished = (u8)(finished + BOARD_STEP(i));

  return finished != 0;
}
