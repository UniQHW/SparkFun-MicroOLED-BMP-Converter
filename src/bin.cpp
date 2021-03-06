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

using namespace std;

/* Get ammount of elements in an pointer array */
int arraySize(byte *bArray[])
{
    int count;

    while(bArray[count + 1] != NULL) // Stop loop once we've reached an out of bounce element
        count ++;

    return count;
}

/* Set all bits to 0 as default */
byte::byte ()
{
	for (int i=0; i < 8; i++)
		bit[i] = 0;
}

/* Return decimal value of byte in uint8 */
uint8_t byte::dec ()
{
	uint8_t val = 0; // Stores final/return value

	for(int  i = 0; i < 8; i++)
	{
			val += (bit[i] == 1) ? pow (2, i) : 0;
	}

	return val;
}

/* Get character from byte */
char byte::ch ()
{
        return dec(); // Recognizes ASCII
}

uint8_t getByteDec (string path, int line)
{
	ifstream file(path, ios::in | ios::binary); // Define input file in binary mode

	if (file.is_open())
	{
		char lineOfBytes[line]; // Holds all bytes from line 0 to line <line param> in ascii chars

		file.read (lineOfBytes, line + 1); // Assigns bytes in dec to lineOfBytes

		int targetByteDec = lineOfBytes[line];

		file.close();

		return targetByteDec;
	}
	else
	{
		cout << "Error: " +  bmpPath + " not found" << endl;
		exit(EXIT_FAILURE);
		return 0;
	}
}

/* Get character from file byte */
char getByteChar(string path, int line)
{
	char lineOfBytes[line]; // Holds all bytes from line 0 to line <line param> in ascii chars

	ifstream file(path, ios::in | ios::binary); // Define input file in binary mode

	if (file.is_open())
	{
		file.read (lineOfBytes, line + 1); // Assigns bytes in dec to lineOfBytes

		char targetByteChar = lineOfBytes[line];

		file.close();

		return targetByteChar;
	}

	else
	{
		cout << "Error: " +  bmpPath + " not found" << endl;
		exit(EXIT_FAILURE);

		return 0;
	}
}

/* Set all bits to 0 */
void byte::reset ()
{
	for (int i=0; i < 8; i++)
		bit[i] = 0;
}

/* Get decimal value of an byte array (ex. Passing an array of 4 bytes) */
int byteArrayDec(int sizeOfArray, byte bArray[])
{
    int val = 0; // Store final/return decimal
    int nBytes = sizeOfArray;
    int bits = nBytes * 8; // Get total ammount of bits by getting the ammount of elements in the array, then multiply it by 8 as 1 byte has 8 bits
    int rc = 0; // Reverse count (As we're reading little endian)

    /* Loop by the ammount of total bits that the array contains */
    for(int i = 0; i < bits; i++) // i = position of currenttly assigned/looped bit
    {
        val += (bArray[rc / 8].bit[rc % 8] == 1) ? pow (2, i) : 0; // Add bits to val by using i as the exponent of 2;

        rc++; // Add 1 to the reverse counter (We're reading little endian
    }

    return val;

}

/* getByte */
/* Function: Convert bytes into abstract C++ classes */
/* Usage : Abstract bits from a binary file into the byte class that contains a bit array with 8 elements */
/* These bits may later be used inside any function as they are then usable in C++ code */
bool byte::getByte(string path, int line)
{
	char lineOfBytes[line]; // Holds all bytes from line 0 to line <line param> in ascii chars

	ifstream file(path, ios::in | ios::binary); // Define input file in binary mode

	if (file.is_open())
	{
		file.read (lineOfBytes, line + 1); // Assigns bytes in dec to lineOfBytes

		int targetByteDec = lineOfBytes[line];

		for (int i = 0; i < 8; i++)
		{
			if (targetEndian == little)
			{
				bit[i]=((uint8_t)(pow(2,i))&targetByteDec)?1:0;
			}
			else
			{
				bit[7 - i]=((uint8_t)(pow(2,i))&targetByteDec)?1:0;
			}
		}

		file.close();

		return true;
	}

	else
	{
		cout << "Error: " +  bmpPath + " not found" << endl;
		exit(EXIT_FAILURE);

		return false;
	}
}
