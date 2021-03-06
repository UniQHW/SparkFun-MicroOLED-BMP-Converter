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

#include "bin.h"

BMP::BMP()
{
	binpath = binPath; // Assign to path chosen by main args

	/** ASSIGN FLAGS FROM DIB **/

	/* Fetch 2 byte flag from binary file */
	for (int i = 0; i < 2; i++)
	{
		identifier[i].getByte(binpath, 0 + i); // Store Bitmap ID (Usually BM)
		       bpp[i].getByte(binpath, 0x1C + i); // Bits per pixel
	}

	/* Fetch 4 byte flags from binary file */
	for (int i = 0; i < 4; i++)
	{
		     size[i].getByte(binpath, 0x2  + i); // File size
		pxloffset[i].getByte(binpath, 0xA  + i); // Pixel offset
		  DIBSize[i].getByte(binpath, 0xE  + i); // DIB header size
		     pxlw[i].getByte(binpath, 0X12 + i); // Pixel Width
		     pxlh[i].getByte(binpath, 0X16 + i); // Pixel Height
		  bitmask[i].getByte(binpath, 0x36 + i); // Bitmask layout
	}

	if (byteArrayDec(4, pxlw) > 64 || byteArrayDec(4, pxlh) > 48)
	{
		cout << "Resolution too big max 64 x 48!" << endl;
		exit(EXIT_FAILURE);
	}
}

void BMP::dispFlags()
{
	int ID[2] = {identifier[0].dec(), identifier[1].dec()};
	int SZ = byteArrayDec(4, size);
	int PXLOS = byteArrayDec(4, pxloffset);
	int BPP = byteArrayDec(2, bpp);
	int DIBS = byteArrayDec(4, DIBSize);
	int PXW = byteArrayDec(4, pxlw);
	int PXH = byteArrayDec(4, pxlh);
	int BM[4] = {bitmask[0].dec(), bitmask[1].dec(), bitmask[2].dec(), bitmask[3].dec()};

	char IDS1(ID[0]);
	char IDS2(ID[1]);
	char BMS1(BM[0]);
	char BMS2(BM[1]);
	char BMS3(BM[2]);
	char BMS4(BM[3]);

	cout << endl;

        cout <<     "Identifier\t: " << ID[0] << " | " << ID[1] << " | " << IDS1 << IDS2 << endl;

        cout <<      "File Size\t: " << SZ    << endl;
        cout <<   "Pixel Offset\t: " << PXLOS << endl;
        cout << "Bits Per Pixel\t: " << BPP   << endl;
        cout <<     "Identifier\t: " << DIBS  << endl;
	cout <<    "Pixel Width\t: " << PXW   << endl;
	cout <<   "Pixel Height\t: " << PXH   << endl;
        cout <<       "Bit Mask\t: " << BM[0] << " | " <<  BM[1] << " | " << BM[2] << " | " << BM[3] << " | " << BMS1 << BMS2 << BMS3 << BMS4 << endl;
}

int BMP::getWidth()
{
	int PXW = byteArrayDec(4, pxlw);
	return PXW;
}

int BMP::getHeight()
{
	int PXH = byteArrayDec(4, pxlh);
	return PXH;
}

void BMP24_BGR::generateMap ()
{
	bgrmap.clear(); // Clear BGR pixel map before generating it

	int sp = byteArrayDec(4, pxloffset); // Start point
	int ep = byteArrayDec(4, size); // End point

	byte color; // Stores one color at a time
	PXL_BGR24 tmp; // To store date only during loop

	int colorElement = 1;

	for (int i = sp; i <= ep; i++)
	{
		switch (colorElement)
		{
			case 1: // B
				color.getByte(binpath, i);
				tmp.byteToColor(b, color);
				color.reset();
			break;

			case 2: // G
				color.getByte(binpath, i);
				tmp.byteToColor(g, color);
				color.reset();
			break;

			case 3: // R
				color.getByte(binpath, i);
				tmp.byteToColor(r, color);
				color.reset();
				bgrmap.push_back(tmp); // Stamp: to pixel vector
				colorElement = 0; // Reset color element
			break;
		}

		colorElement ++;
	}
}

void BMP_MONOCHROME::generateMap()
{
	PXL_MONOCHROME passpxl;

        bool antiState = invert == true ? true : false;

	passpxl.state = antiState;

        monochromemap.clear(); // Clear monochrome map before generating it

        int sp = byteArrayDec(4, pxloffset); // Start point
		int ep = byteArrayDec(4, size); // End point

		uint8_t brightness; // Stores brightness value of called color (Monochrome only cares about it's brightness, no colors involved)

		int byteCount = 0;
		int byteCountLimit = byteArrayDec(4, bpp) / 8;

		for (int i = sp; i <= ep; i++)
		{

			if (byteCount == byteCountLimit)
			{
				monochromemap.push_back(passpxl);
				passpxl.state = antiState;
				byteCount = 0;
			}

			if (getByteDec(binpath, i) >=	 127.5 && passpxl.state == antiState)
			{
				passpxl.state = antiState ? 0 : 1;
			}

			byteCount ++;
		}
}
