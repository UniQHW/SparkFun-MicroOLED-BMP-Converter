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

/* Convert a pixel into B, G or R */
void PXL_BGR24::byteToColor (pxlcol trgetCol, byte colorbyte)
{
	switch (trgetCol)
	{
		case b :
			B = colorbyte;
			break;
		case g :
			G = colorbyte;
			break;
		case r :
			R = colorbyte;
			break;
	}
}


/* Constructor */
PXL_MONOCHROME::PXL_MONOCHROME ()
{
	state = 0;
}

/* Converts BGR Pixel into a more primitive Monochrome Pixel */
bool PXL_MONOCHROME::BGR_ToMono(PXL_BGR24 inPXL, bool invert /* = false */)
{
	bool targetstate = true;

	if (invert)
		targetstate = false;

	if 		(inPXL.B.dec() >= 127.5) return targetstate;
	else if (inPXL.G.dec() >= 127.5) return targetstate;
	else if (inPXL.R.dec() >= 127.5) return targetstate;
	else				 return invert; // Return true if invert is ON
}
