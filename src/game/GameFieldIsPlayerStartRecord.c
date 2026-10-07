// bdc 0x088bddb8 GameFieldIsPlayerStartRecord
#include "bdc.h"

/* Returns 1 when the record is the player start record of the block, from a hard-coded set of
   `(area, block, record)` triples of the object layout (`GameFieldLoadObjects` passes the record
   index): (0,0,10), (0,1,10), (1,0,9), (1,1,21), (2,0,18), (2,1,16), (3,0,31), (3,2,10), (4,0,13),
   (4,1,6), (5,0,9); 0 otherwise. */

s32 GameFieldIsPlayerStartRecord(s32 area, s32 block, s32 record)

{
  if (area == 0) {
    if (block == 0 && record == 10) {
      return 1;
    }
    if (block == 1 && record == 10) {
      return 1;
    }
  }
  if (area == 1) {
    if (block == 0 && record == 9) {
      return 1;
    }
    if (block == 1 && record == 21) {
      return 1;
    }
  }
  if (area == 2) {
    if (block == 0 && record == 18) {
      return 1;
    }
    if (block == 1 && record == 16) {
      return 1;
    }
  }
  if (area == 3) {
    if (block == 0 && record == 31) {
      return 1;
    }
    if (block == 2 && record == 10) {
      return 1;
    }
  }
  if (area == 4) {
    if (block == 0 && record == 13) {
      return 1;
    }
    if (block == 1 && record == 6) {
      return 1;
    }
  }
  if (area == 5 && block == 0 && record == 9) {
    return 1;
  }
  return 0;
}
