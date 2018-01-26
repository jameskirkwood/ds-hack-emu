# Hack Emulator for NDS

**Hack** is the name of the educational computer system developed by **Noam Nisan** and **Shimon Schocken** for the book *The Elements of Computing Systems* and the *Nand2Tetris* online course. More information about the course and accompanying text (which describes the Hack machine) can be found at [http://www.nand2tetris.org/](http://www.nand2tetris.org/).

This project is an experiment in embedded systems programming. Well, if the Nintendo DS comes under that classification. (There doesn't appear to be an agreed upon definition of "embedded".) I set myself the challenge of emulating the Hack machine on an NDS to see how much performance I could get out of it.

This software is published under the MIT licence (see [LICENCE](LICENCE)).

## Usage

You will need either a physical NDS and means of launching unsigned code on it, or an NDS emulator, in order to run the Hack emulator. If you want to run the emulator in an emulator, I recommend DeSmuME.

The prebuilt executable will run a small program that draws stripes on the screen.

## Building

Currently the emulator will run a single program linked into the final executable, as this makes it easy to test with an NDS emulator. Once you have a program in .hack format (text encoded binary), follow the steps below to build the .nds executable:

- Compile `util/pack`

  ```bash
  cd util
  make pack
  ```

- Convert your ROM (text encoded binary, 16 bits per line) to little-endian 16-bit raw binary

  ```bash
  util/pack < <rom_file> > data/rom.bin
  ```

- Build the Hack Emulator, linking the binary file

  ```bash
  make
  ```

## Design

### CPU

This is a work in progress. Currently it simply interprets the binary bit by bit, but I hope to implement JIT binary translation (to 32-bit ARM machine code) later.

### Display

As the Hack display is twice the width of one of the NDS', I use subpixel rendering to approximate a 2x2 pixel region of the Hack display in one real pixel. Since the Hack display is 1bpp black and white, there are 16 possible underlying patterns, so I have used a 16 colour precomputed subpixel palette. The dots of the NDS display are arranged in the order Blue, Green, Red, from left to right, within each pixel, which may be different from your display's arrangement if you are running it in an emulator, in which case edges will tend to look fuzzy (and colourful).

### Keyboard

Currently only the left and right D-pad buttons are mapped, to the left and right arrow keys. I intend to map the other buttons and implement a touch screen keyboard later.