# Spark Fun Micro OLED BMP converter
### in short sfobmp

This is a small project that i've been working on during the 2 weeks of my ID tech camp visit.
This project has plenty of room to improve, however i've decided to bring it to an acceptable state before releasing it to git.
I'd like to thank IDtech for the great experience that I had during those 2 weeks.

I've decided to pick C++ as it's a great language to work directly with hardware components and mainly because it's my main programming language.

## About
This is a bmp (short for bitmap) image converter for the Spark Fun Micro OLED.
It converts a max 64x48 bmp image into monochrome and outputs a generated header with a the according functions to draw the image onto the OLED panel.

## How to run

The usage syntax is very open as I wanted to avoid limiting down users, however it is very hard to explain it as you can set/unset flags across the entire cmdline.

Syntax:

```
sfobmp [ <your bmp> | -i | -ndef | -c | -a ] [ <your bmp> | -i | -ndef | -c  | -ui | -redef | -uc ]  [ <your bmp> | -i | -ndef | -c  | -a | -ui | -redef | -uc ] ...

or

sfobmp [ <your bmp> | --invert | --noDefine | --combine | --animation ] [ <your bmp> | --invert | --noDefine | --combine | --animation | -undoInvert | -reDefine | -undoCombine ] ...

```

Flags :

```

Enable invert Image (Lighter becomes OFF, darker becomes ON):	                                  -i    || --invert
Disable invert Images (If invert has been enabled):                                             -ui   || --undoInvert

Disable definition of MicroOled object in header:                                               -ndef || --noDefine
(Re)enable definition of MicroOled object in header:                                            -rdef || --reDefine

Generate draw functions for multiple bitmaps into only one header (does't work with -a or -ca)  -c    || --combine
Disable combining (If enabled):								                                                  -uc   || --undoCombine

Enable animation combine. Combines into a single function with a delay argument (not compatible with -c or -ca)  
                                                                                                -a || --animation

Generate draw functions for multiple bitmaps into a single array (doesn't work with -c or -a):  -ca   || --combineArray

```

Example usage :
```
Converting a single bmp

sfobmp your.bmp


Convertng a single inverted bmp (Color)

sfobmp -i you.bmp


Converting multiple bmps

sfobmp your.bmp yoursecond.bmp


Converting multiple bmps combined in one header

sfobmp -c your.bmp yoursecond.bmp


Converting multiple bmps into a animation header

sfobmp -a your.bmp yoursecond.bmp

Converting inverted, then uninverted bmp

sfobmp -i your.bmp -ui yoursecond.bmp

```

## What isn't ready yet
```
- ca flag

- make install

- converting 16 bit bmps may cause issues (For now 24-32bit bmps work guaranteed)

- Paths (bmp must be in same dir as sfobmp)
```

## How to compile
To compile, run :
```
make
```

## How it works

The Programm starts out by reading command line flags (-c, -i, -ndef etc.), if it recognizes a flag it proceeds to set the flags during runtime. 
Once a BMP has been recognized, it gets read in binary. After importing important flags (See DIB flags https://docs.google.com/document/d/13GNf4dHVOk-X23ZxgC_NlQj95Jx1vuiiI_N07l9Gz1k/edit?usp=sharing) it proceeds to read the bits accordingly.
Those bits later get assigned to bytes, that apporpriately to colors, pixels, then passing trough a monochrome filter that rejects brightness leves accordingly, and finally generates a bitmap out of each and every monochrome pixel.
Once that is done the last thing to do is generate the header by looping trough pixel by pixel and then assigning it to the oled.pixel.

## Why not use drawBitmap()

When running drawBitmap(), not only are you limited to copy pasting each picture one after another (See -c -a or -ca flag on sfobmp), but
the targeted device to run the bitmap, must convert the array first, THEN proceeds to enabling pixel by pixel after running trough EACH pixel (even if it's set off).
