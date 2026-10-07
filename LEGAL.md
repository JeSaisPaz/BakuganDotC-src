# Legal notice

*Bakugan Battle Brawlers: Defenders of the Core* and *Bakugan* are trademarks of their respective
owners. This project is not affiliated with, endorsed by, or sponsored by Spin Master, Sega Toys,
Activision, Now Production, Sony Interactive Entertainment, or any other rights holder.

## What this repository contains

- C source written for this project: one file per function of the game's main executable, produced by
  reverse engineering (disassembly and decompilation), then cleaned up for readability by AI agents and
  reviewed against the disassembly.
- A header (`src/include/bdc.h`) of names, types and prototypes recovered the same way.

## What it does not contain

- No game executable, no disc image, no extracted assets (models, textures, audio, text, video).
- No encryption keys and no circumvention tools.
- No initialised game data: global variables are declared `extern` only.

To study or use this code you need your own legally obtained copy of the game. Nothing here is
playable on its own.

## Licensing

The GNU GPL v3 (`LICENSE`) covers the contributions of this project's authors, to the extent they hold
rights in them. It does not grant any rights in the original game, its code, or its other content, which
remain the property of their owners. Types and signatures of PSP system functions are derived from the
PSPSDK headers (BSD-style licence, `LICENSE.pspsdk`) and the uOFW headers (MIT licence, `LICENSE.uofw`).

The project is shared for research, preservation, education and interoperability. If you are a
rights holder and have a concern, open an issue or contact the maintainer, and it will be addressed
promptly.
