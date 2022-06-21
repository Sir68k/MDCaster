# MDCaster (a.k.a. MDoom)

A simple raycaster demo for Sony NetMD portables, currently targetting **v1.6 CXD2680** devices with single row displays. 

This application serves as a template project for building code for the Sony NetMD portables, while also demonstrating the results from the firmware reverse engineering efforts (see https://github.com/Sir68k/md-firmware).

Demo:

[![Demo](https://img.youtube.com/vi/7OHny4QqUiI/0.jpg)](https://www.youtube.com/watch?v=7OHny4QqUiI)

# About

I started this hoping to make a full Doom like game, but in the end 30x7 pixels is just too small for a game like this. I just left it with the basic raycaster.
The code structure is based on the awesome lodev tutorial (https://lodev.org/cgtutor/raycasting.html), modified to work with fixed point math and some lookup tables.

## Working devices

Developed with a MZ-N520, most likely working on MZ-N510, MZ-N710, .. devices. MZ-N910 might not work due it using a different display.

## Running

This binary (build/mddom.bin) needs to be loaded to `0x02005500` in memory and then executed. Support for loading applications can be found in Asivery's NetMD IDE (https://github.com/asivery/netmd-asm-ide).
