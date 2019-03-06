# Bounce

A simple Boing like demo for AmigaOS 2.04 - 3.x on any Motorola 68K CPU.

Built using [amiga-gcc](https://github.com/bebbo/amiga-gcc), which has saved me
a lot of time setting up a tool-chain or waiting for an Amiga based compiler.

## To Build

Grab a copy of [amiga-gcc](https://github.com/bebbo/amiga-gcc), clone the repo
and run:

```
m68k-amigaos-g++ -fpermissive -noixemul -m68000 -o bounce bounce.cpp
```

Someday I'll set up a makefile for that.

## To Run

Either you need a real Amiga or a virtual one such as UAE. In either case, the
resulting executable can be copied onto the machine and run from there.

# License

2019 Matt Kohls
GPL v3

This program is distributed in the hope it will be useful, but WITHOUT ANY 
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.
