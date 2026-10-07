// bdc 0x0891c50c UiHologramGalleryMapAttribute
#include "bdc.h"

/* Maps an attribute index 1..5 between two orderings: with `forward` set 1→1, 2→5, 3→2,
   4→3, 5→4; otherwise 1→1, 2→3, 3→4, 4→5, 5→2; 0 for others. */

int UiHologramGalleryMapAttribute(bool forward, u8 index)

{
  if (forward) {
    if (5 < index) {
      return 0;
    }
    if (index == '\x01') {
      return 1;
    }
    if (index == '\x02') {
      return 5;
    }
    if (index != '\x03') {
      if (index == '\x04') {
        return 3;
      }
      if (index != '\x05') {
        return 0;
      }
      return 4;
    }
    return 2;
  }
  if (5 < index) {
    return 0;
  }
  if (index == '\x01') {
    return 1;
  }
  if (index == '\x02') {
    return 3;
  }
  if (index != '\x03') {
    if (index == '\x04') {
      return 5;
    }
    if (index != '\x05') {
      return 0;
    }
    return 2;
  }
  return 4;
}

