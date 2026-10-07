// bdc 0x088ce950 GameStoryMovieSetCueTable
#include "bdc.h"

/* For movie ids up to 0x77 (signed compare: ids below 0x36 also pass, indexing before the table) sets the voice cue table `+0x2c` (`g_gameStoryMovieCueTables[id-0x36]`) and
   resets the cue index `+0x24` of a movie task; returns `(movieId-0x36) < 0x42` (signed), i.e. true unless id > 0x77. */

bool GameStoryMovieSetCueTable(CoreTask *task, s32 movieId)
{
  GameFieldMovieTask *movie = (GameFieldMovieTask *)task;
  s32 index = movieId - 0x36;

  if (index < 0x42) {
    movie->cueIndex = 0;
    movie->cueTable = g_gameStoryMovieCueTables[index];
  }
  return index < 0x42;
}
