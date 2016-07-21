/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BIN_H_INCLUDED
#define BIN_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>

#include <math.h>

using namespace std;

enum endian
{
	little,
	big
};

extern string bmpPath;
extern string binPath;
enum pxlcol {r, g, b};

/* Modifiable/Adjustable byte */
struct byte
{
	byte ();

	endian targetEndian = little; // Targeted endian value for params (Default little endian)

	bool bit[8]; // Stores 8 bits that can store 1 or 0 (true and false)

	bool getByte(string path, int line);

	uint8_t dec (); // Get decimal uint8 value of byte

	char ch ();

	void reset ();
};

struct PXL_BGR24
{
	typedef byte color_t; // Type define color_t to 1 byte

	color_t B; // Blue
	color_t G; // Green
	color_t R; // Red

	void byteToColor (pxlcol trgetCol, byte colorByte);

};

struct PXL_MONOCHROME
{
	PXL_MONOCHROME(); // Sets state to 0 as def

	bool state;

	bool BGR_ToMono (PXL_BGR24 inPXL, bool invert = false);
};

struct BMP
{
	typedef std::vector<PXL_BGR24> bgrmap_t;
	typedef std::vector<PXL_MONOCHROME> monomap_t;
	typedef byte flag_t;

protected:
	/* DIB MS WIN */
	flag_t identifier[2]; 	// Identifier
	flag_t size[4]; 	// IMG size
	flag_t pxloffset[4]; 	// Pixel Offset
	flag_t bpp[4]; 		// Bits Per Pixel
	flag_t DIBSize[4]; 	// Header Size
	flag_t pxlw[4];		// Pixel Width
	flag_t pxlh[4];		// Pixel Height
	flag_t bitmask[4]; 	// Bit mask

public:
	string binpath = binPath;

	BMP();

//	bmptype getBMPDIB ()

	void dispFlags();

	int getWidth  ();
	int getHeight ();

	virtual void generateMap () = 0;
};

struct BMP24_BGR : BMP
{
	bgrmap_t bgrmap;
	void generateMap ();
};

struct BMP_MONOCHROME : BMP
{
    bool invert;
	monomap_t monochromemap;

	void generateMap();
};

int  byteArrayDec (int sizeOfArray, byte bArray[]); // Get decimal of multiple bytes
int  byteArrayDec (byte *bArray[]); // Get decimal of multiple bytes

uint8_t getByteDec (string path, int line);

char getByteChar(string path, int line);


#endif // BIN_H_INCLUDED
