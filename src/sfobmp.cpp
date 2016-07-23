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

#include <iostream>
#include <fstream>

#include "bin.h"

using namespace std;

/* Main Args */
bool invertFlag = false;
bool defFlag = true;
bool combineFlag = false;
bool combineArrayFlag = false;
bool animationFlag = false;

int cFlagCurrentBMPCount;
string combineName; // Stores name of combine file (Set to NULL until next BMP on cmdline is reached)
int combineFlagState = 0; // Stores state of combine flag - Start as OFF

int aFlagCurrentBMPCount;

string getName(string path); // Name filter
bool scanArg(string param); // Argument sorter

int bmpCount = 0; // Amount of bitmaps passed

string binPath; // Stores binary path
string bmpPath; // Stores bmp Path
string name; 	// Stores bmp name

string hguard;

string slash = "/";

string tab = "\t";
string sidecomment = slash + slash;
string comment(string input) {return slash + "* " + input + " */";}

int main (int argv, char *argc[])
{
	for (int run = 1; run < argv; run++)
	{
		bool cArg = scanArg(argc[run]); // Stores scanArg return of current argc

		ofstream sfoheader;

		/* Define combine name "static" on 1st BMP call until -uc is called */
		if (bmpCount == cFlagCurrentBMPCount + 1 && combineFlag ||
	  	bmpCount == cFlagCurrentBMPCount + 1 && combineArrayFlag)
		{
			combineName = name;
			combineFlagState = 1; // Indicate -c has been called
		}

		/* On -c */
		if (combineFlagState == 1)
		{
			/* Check if file already exists */
			sfoheader.open("generated-" + combineName + ".h", ios::out | ios::trunc); // Overwrites it to 0;
			sfoheader.close();

			combineFlagState = 2; // Indicate header has been generated
		}

		/* On -c set */
		if (combineFlagState == 2)
		{
			sfoheader.open("generated-" + combineName + ".h", ios::out | ios::app); // Assign to common header
		}

		/* On -uc */
		if (combineFlagState == 3)
		{
			sfoheader.open("generated-" + combineName + ".h", ios::out | ios::app);
			sfoheader
			<< "}" << endl
			<< endl << "#endif";
			combineFlagState = 0;
		}

		if (!cArg)
		{

			BMP_MONOCHROME sfobmp; // Define Monochrome map before entering loop

			if (!combineFlag)
			{
				sfoheader.open("generated-" + name + ".h", ios::out);
			}


			if (!combineFlag || bmpCount == 1)
			{

				hguard = "GENERATED_" + name + "bmp_H_INCLUDED";

				sfoheader
				<< comment(" This header has been automatically generated via the sfobmp generator") 	<< endl
				<< comment(" Thanks for using the sfobmp tool") 					<< endl
				<< comment(" Made by Patrick Pedersen (CTXz) <ctx.xda@gmail.com>")		<< endl << endl

				<< "#ifndef " << hguard								<< endl
				<< "#define " << hguard 							<< endl << endl


				<< "#include \"SparkFunMicroOLED/SparkFunMicroOLED.h\"" 			<< endl << endl;

				if (defFlag)
						sfoheader << "MicroOLED oled;" << endl << endl;

				sfoheader
				<< "namespace sfobmp" << endl
				<< "{" << endl;
			}

			if (bmpCount > 1 && combineFlag)
				sfoheader << endl << endl; // Create space between functions

			if (animationFlag && aFlagCurrentBMPCount + 1 == bmpCount)
			{
				sfoheader
				<< tab << "void draw" + name + "(int ms)" << endl
				<< tab << "{" << endl;
			}

			else if (!animationFlag)
			{
				sfoheader
				<< tab << "void draw" + name + "()" << endl
				<< tab << "{" << endl;
			}

			sfobmp.dispFlags();

			sfobmp.invert = invertFlag;

			sfobmp.generateMap();

			int limitX = sfobmp.monochromemap.size() / sfobmp.getHeight();

			int x = limitX; // Start at end to avoid inverted pictures
			int y = 0;

			for (int i = sfobmp.monochromemap.size() - 1; i >= 0; i--)
			{
				if (sfobmp.monochromemap[i].state == 1)
				{
					sfoheader << tab << tab << "oled.pixel(" << x << ", " << y << ");" << endl;
					x--;
				}

				else
					x--;

				if (x == 0)
				{
					x = limitX;
					y++;
				}
			}

			sfoheader
			<< tab << tab << "oled.display();" << endl;

			if (animationFlag)
			{
				sfoheader
				<< endl << tab << tab << "delay(ms);"
				<< endl << tab << tab << "oled.clear(PAGE);";
				if (run == argv - 1)
					sfoheader << tab << "}" << endl;
			}

			else
				sfoheader << tab << "}" << endl;

			if (!combineFlag || run == argv - 1)
			{
				sfoheader
				<< "}" << endl
				<< endl << "#endif";
			}

			if (combineFlag)
				cout << "Successfully generated " + name + " into combined " << "generated-" + combineName + ".h" << endl;
			else
				cout << "Successfully generated : generated-" + name + ".h" << endl;
		}

		sfoheader.close(); // Close sfoheader
	}

	cout << endl << "Processed " << bmpCount << " image(s)" << endl << endl;
	return 0;
}

/* Name filter */
string getName (string path)
{
	string processedName;
	processedName = path.erase (path.size() - 4, path.size());

	for (int i = processedName.size() - 1; i >= 0; i--)
	{
		/* On Path outside of current directory */
		if (processedName[i] == '/')
		{
			processedName.erase (0, i + 1); // Short to name only
			break; // Don't waste perfomance
		}
	}

	return processedName;
}

bool scanArg(string param)
{
	if (param.size() > 4) // Avoid Logic error
	{
		/* Check for .bmp extension */
		if
	 	(
			param[param.size() - 4]		== '.'
			&& param[param.size() - 3] 	== 'b'
			&& param[param.size() - 2] 	== 'm'
			&& param[param.size() - 1] 	== 'p'
		)

		{
			bmpPath = param;
			name 	= getName(bmpPath);
			bmpPath = param; // Reset back to param after erase
			binPath = bmpPath;
			bmpCount++;

			return false;
		}
	}

	/* Invert */
	if	(param == "-i" ||
		 param == "--invert")
			invertFlag = true;

	/* Undo Invert */
	else if (param == "-ui" && invertFlag ||
		 param == "--undoInvert" && invertFlag)
			invertFlag = false;

	/* Disable MicroOled object definition at top of header */
	else if (param == "-ndef" ||
		 param == "--noDefine")
			defFlag = false;

	/* (Re)Enable MicroOled object definition */
	else if (param == "-rdef" && !defFlag ||
		 param == "--reDefine" && defFlag == false)
			defFlag = true;

	/* Enable exporting functions of multiple bitmaps into a single header file */
	else if (param == "-c" && combineArrayFlag == false && !animationFlag ||
		 param == "--combine" && combineArrayFlag == false && !animationFlag) {
			cFlagCurrentBMPCount = bmpCount; // Stores how many bitmaps have already been counted during flag entrance
			combineFlag = true;
		}
	/* Disable exporting functions of multiple bitmaps into a single header file */
	else if (param == "-uc" ||
		 param == "--undoCombine") {

			combineFlagState = 3;

			if (combineFlag)
				combineFlag = false;

			else if (combineArrayFlag)
				combineArrayFlag = false;

			else if (animationFlag)
				animationFlag = false;
		}

	/* Enable exporting an ARRAY of functions of multiple butmaps into a single header file (Disable with -!c) */
	else if (param == "-ca" && !combineFlag && !animationFlag ||
		 param == "--combineArray" && !combineFlag &&  !animationFlag)
			combineArrayFlag = true;

	else if (param == "-a" && !combineFlag ||
		param == "--animationFlag" && !combineFlag)
		{
			aFlagCurrentBMPCount = bmpCount; // Stores how many bitmaps have already been counted during flag entrance			
			animationFlag = true;
			combineFlag = true;
		}

	/* Invalid flag/order */
	else
	{
		cout << endl
		<< "sfobmp flags" << endl << endl
		<< "Enable invert Image (Lighter becomes OFF, darker becomes ON): -i || --invert" << endl
		<< "Disable invert Images (If invert has been enabled): -ui || --undoInvert" << endl
		<< endl
		<< "Disable definition of MicroOled object in header: -ndef || --noDefine" << endl
		<< "(Re)enable definition of MicroOled object in header: -rdef || --reDefine" << endl
		<< endl
		<< "Generate draw functions for multiple bitmaps into only one header (does't work with -ca): -c || --combine" << endl
		<< "Disable combining (If enabled): -uc || --undoCombine" << endl
		<< endl
		<< "Generate draw functions for multiple bitmaps into a single array (doesn't work with -c): -ca || --combineArray" << endl
		<< endl;
		exit (EXIT_FAILURE);
	}

	return true;
}

