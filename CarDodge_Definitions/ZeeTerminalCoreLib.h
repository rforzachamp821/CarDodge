#include <Windows.h>
#include <conio.h>
#include <vector>
#include <random>
#include <thread>
#include <iostream>
#include <string>
#include <codecvt>

#pragma once

std::string sLastColourFore = "";
std::string sLastColourBack = "";

std::string sColourGlobal = LWHT;
std::string sColourGlobalBack = BLK;

std::string sColourHighlight = LWHT;
std::string sColourHighlightBack = BLU;

std::string sColourTitle = BLK;
std::string sColourTitleBack = LCYN;

std::string sColourSubheading = LWHT;
std::string sColourSubheadingBack = MAG;

bool bAutoReadableContrast = true;
bool bShowCursor = true;
bool bCursorBlink = true;
bool bWordWrapToggle = true;
bool bDisplayDirections = true;
bool bDisplayVerboseMessages = false;

bool bAnsiVTSequences = true;

int nCursorShape = 3;
int nSlowCharSpeed = 35;



// A switch-case function that returns a colour associated with its number.
std::string NumberToColour(int nNumberChoice) {

	// Switch case based on number choice
	switch (nNumberChoice) {
	case 1:
		return BLK;
		break;
	case 2:
		return BLU;
		break;
	case 3:
		return GRN;
		break;
	case 4:
		return CYN;
		break;
	case 5:
		return RED;
		break;
	case 6:
		return MAG;
		break;
	case 7:
		return YLW;
		break;
	case 8:
		return WHT;
		break;
	case 9:
		return GRAY;
		break;
	case 10:
		return LBLU;
		break;
	case 11:
		return LGRN;
		break;
	case 12:
		return LCYN;
		break;
	case 13:
		return LRED;
		break;
	case 14:
		return LMAG;
		break;
	case 15:
		return LYLW;
		break;
	case 16:
		return LWHT;
		break;
	default:
		VerbosityDisplay("ERROR - An error occured when returning the colour string associated with its number. This is probably a bug.\n");
		break;
	}

	return "";
}

// A switch-case function that returns a number associated with one of the default colour strings.
// 0 is returned on error.
int ColourToNumber(std::string sColourChoice)
{
	if (sColourChoice == BLK) {
		return 1;
	}
	else if (sColourChoice == BLU) {
		return 2;
	}
	else if (sColourChoice == GRN) {
		return 3;
	}
	else if (sColourChoice == CYN) {
		return 4;
	}
	else if (sColourChoice == RED) {
		return 5;
	}
	else if (sColourChoice == MAG) {
		return 6;
	}
	else if (sColourChoice == YLW) {
		return 7;
	}
	else if (sColourChoice == WHT) {
		return 8;
	}
	else if (sColourChoice == GRAY) {
		return 9;
	}
	else if (sColourChoice == LBLU) {
		return 10;
	}
	else if (sColourChoice == LGRN) {
		return 11;
	}
	else if (sColourChoice == LCYN) {
		return 12;
	}
	else if (sColourChoice == LRED) {
		return 13;
	}
	else if (sColourChoice == LMAG) {
		return 14;
	}
	else if (sColourChoice == LYLW) {
		return 15;
	}
	else if (sColourChoice == LWHT) {
		return 16;
	}
	else {
		VerbosityDisplay("ERROR - An error occured when returning the number associated with its colour string. This is probably a bug.\n");
		return 0;
	}

	return 0;
}

// Sets cursor attributes automatically when called
inline void SetCursorAttributes() {

	// Can't do without ANSI VT sequences
	if (bAnsiVTSequences) {
		// Set cursor shape
		std::cout << "\x1b[" << nCursorShape << " q";

		// Set cursor blink
		if (bCursorBlink == true) {
			std::cout << "\x1b[?12h";
		}
		else std::cout << "\x1b[?12l";

		// Set cursor visibility
		if (bShowCursor == true) {
			std::cout << "\x1b[?25h";
		}
		else std::cout << "\x1b[?25l";

	}

	// Non-ANSI terminals
	else {
		// Set cursor visibility using the WIN32 Console API
		CONSOLE_CURSOR_INFO cciAttribSet;
		GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cciAttribSet);

		// Set variables
		if (bShowCursor == true) {
			cciAttribSet.bVisible = true;
		}
		else cciAttribSet.bVisible = false;

		// Set final cursor info
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cciAttribSet);
	}

	return;
}

// Sets the colour and background colour
inline void colour(std::string sColourForegroundChoice, std::string sColourBackgroundChoice) {

	// Set colours to their respective last colours
	sLastColourFore = sColourForegroundChoice;
	sLastColourBack = sColourBackgroundChoice;

	if (bAnsiVTSequences) {

		// Maintain a readable contrast if the setting is set to true
		if (bAutoReadableContrast == true) {
			// To keep contrast good
			if (sColourForegroundChoice == sColourBackgroundChoice) {
				if (sColourBackgroundChoice == BLK
					|| sColourBackgroundChoice == RED
					|| sColourBackgroundChoice == GRN
					|| sColourBackgroundChoice == YLW
					|| sColourBackgroundChoice == BLU
					|| sColourBackgroundChoice == MAG
					|| sColourBackgroundChoice == CYN)
				{
					sColourForegroundChoice = LWHT;
				}
				else if (sColourBackgroundChoice == GRAY
					|| sColourBackgroundChoice == LRED
					|| sColourBackgroundChoice == LGRN
					|| sColourBackgroundChoice == LYLW
					|| sColourBackgroundChoice == LBLU
					|| sColourBackgroundChoice == LMAG
					|| sColourBackgroundChoice == LCYN
					|| sColourBackgroundChoice == LWHT
					|| sColourBackgroundChoice == WHT)
				{
					sColourForegroundChoice = BLK;
				}
			}

			// Colour relationship
			if (sColourBackgroundChoice == LWHT && sColourForegroundChoice == LYLW) {
				sColourForegroundChoice = BLK;
			}
			else if (sColourBackgroundChoice == LYLW && sColourForegroundChoice == LWHT) {
				sColourForegroundChoice = BLK;
			}
			else if (sColourBackgroundChoice == LWHT && sColourForegroundChoice == LGRN) {
				sColourForegroundChoice = BLK;
			}
			else if (sColourBackgroundChoice == LGRN && sColourForegroundChoice == LWHT) {
				sColourForegroundChoice = BLK;
			}
		}

		// Foreground
		std::cout << "\x1b[38;2;" << sColourForegroundChoice << "m";

		// Support for transparency in Windows Terminal; black is usually default background colour so will be default here
		if (sColourBackgroundChoice == BLK) {
			std::cout << "\x1b[49m";
			return;
		}

		// Background
		std::cout << "\x1b[48;2;" << sColourBackgroundChoice << "m";
	}

	// Windows Console API - activated when no ANSI support is detected and is a fallback (16-colours)
	else {

		int nBackgroundColourFinal = 0;
		int nForegroundColourFinal = 0;

		// Now, modify the background input choice 
		// Check for number
		if (isNumberi(sColourForegroundChoice, false) && isNumberi(sColourBackgroundChoice, false)) {
			nBackgroundColourFinal = std::stoi(sColourBackgroundChoice);
			nForegroundColourFinal = std::stoi(sColourForegroundChoice);
		}
		else {
			// Check failed, do not change colour; stream error message to std::cerr without using colours (to prevent forkbomb)
			std::cerr << wordWrap("ERROR: In colour() - Invalid WIN32 colour argument(s) detected. The arguments must be numbers as strings only."
				"\nArguments: sColourForegroundChoice: <" + sColourForegroundChoice + ">, sColourBackgroundChoice: <" + sColourBackgroundChoice + ">.\n");
			return;
		}

		// Maintain a readable contrast if the setting is set to true
		if (bAutoReadableContrast == true)
		{
			if (nBackgroundColourFinal == nForegroundColourFinal) {
				if (nBackgroundColourFinal < 7) {
					nForegroundColourFinal = 15; // Bright White
				}
				else if (nBackgroundColourFinal > 7 || nBackgroundColourFinal == 7) {
					nForegroundColourFinal = 0; // Black
				}
			}

			// Colour relationship
			if (nBackgroundColourFinal == 15 && nForegroundColourFinal == 14) {
				nForegroundColourFinal = 0; // Black due to similarity with yellow and white
			}
			else if (nBackgroundColourFinal == 14 && nForegroundColourFinal == 15) {
				nForegroundColourFinal = 0; // Black due to similarity with yellow and white
			}
			else if (nBackgroundColourFinal == 15 && nForegroundColourFinal == 10) {
				nForegroundColourFinal = 0; // Black due to similarity with green and white
			}
			else if (nBackgroundColourFinal == 10 && nForegroundColourFinal == 15) {
				nForegroundColourFinal = 0; // Black due to similarity with green and white
			}
		}

		nBackgroundColourFinal *= 16; // WINAPI works on a 16 colour system

		// Finally, add the colours and set the colour
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (nBackgroundColourFinal + nForegroundColourFinal));
	}

	return;
}

// colourHighlight - Sets the colour from value inside nColourHighlight(Back).
inline void colourHighlight()
{
	// Use the default colour function; custom solution not needed
	colour(sColourHighlight, sColourHighlightBack);
	return;
}

// Be sure to change back to no underline AND no bold when done
inline void colourTitle()
{
	// Use the default colour function; custom solution not needed
	colour(sColourTitle, sColourTitleBack);
	if (bAnsiVTSequences) std::cout << "\x1b[" << ULINE << "m";

	return;
}

// Be sure to change back to no underline when done
inline void colourSubheading()
{
	// Use the default colour function; custom solution not needed
	colour(sColourSubheading, sColourSubheadingBack);
	if (bAnsiVTSequences) std::cout << "\x1b[" << ULINE << "m";

	return;
}

// Function to put console to sleep so makes programming job easier
// Measures in milliseconds
inline void sleep(long long int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

// Function to choose random number
long double RandNum(long double max, long double min)
{
	std::random_device rdRandNum;

	// distribution in range [min, max]
	std::uniform_real_distribution<> dist(min, max);

	return dist(rdRandNum);
}

// LogWindowsEvent - Logs an event of choice on the Windows Event Log Database.
// Arguments: vsEventDataArray - Vector array of event data strings to forward to the event report.
//            dwBinaryDataSize - The size of the lpBinaryData argument. Set this to 0 if no binary data will be included.
//            lpBinaryData - Binary data to forward to the event report.
// Return Values: TRUE or 1 for success, FALSE or 0 for fail.
// 
bool LogWindowsEvent(std::vector<std::string> vsEventDataArray, DWORD dwBinaryDataSize, LPVOID lpBinaryData)
{
	// Create handle around ZeeTerminal source
	HANDLE hEventSource = RegisterEventSourceA(NULL, "ZeeTerminal");
	size_t nEventDataArraySize = vsEventDataArray.size();

	// Convert std::string array to LPCSTR
	LPCSTR* lpEventData = new LPCSTR[nEventDataArraySize];
	// Use for loop
	for (size_t i = 0; i < nEventDataArraySize; i++) {
		lpEventData[i] = vsEventDataArray[i].c_str();
	}

	// Report the event to event log
	if (!ReportEventA(hEventSource, EVENTLOG_ERROR_TYPE, 0, 0, NULL, nEventDataArraySize, dwBinaryDataSize, lpEventData, lpBinaryData)) {
		VerbosityDisplay("In LogWindowsEvent(): ERROR - Failed to report event on ReportEventA function. Error code " + std::to_string(GetLastError()) + ": " + std::system_category().message(GetLastError()) + ".\n");
		return false;
	}

	// Uninitialise and exit
	DeregisterEventSource(hEventSource);
	delete[] lpEventData;

	return true;
}

// OutputBoxWithText - Outputs a box outline, with text inside.
//                   - The box is drawn to the maximum text width, and if that is too big, it's drawn to the terminal width.
// Arguments: sText - The text to be boxed.
//            sColourOutlineFore - The foreground colour for the outline. 
//            sColourOutlineBack - The background colour for the outline.
//            sColourTextFore - The foreground colour for the text.
//            sColourTextBack - The background colour for the text.
//            bCentreBox - If specified, centre the box on the terminal screen. This is based on the terminal's horizontal width. Defaults to FALSE.
// Return Values: None
//
void OutputBoxWithText(std::string sText, std::string sColourOutlineFore, std::string sColourOutlineBack, std::string sColourTextFore, std::string sColourTextBack, bool bCentreBox)
{
	CONSOLE_SCREEN_BUFFER_INFO csbiDirections;
	int nBoxWidth = 0;
	std::vector<std::string> sLines = {};
	std::string sBuffer = "";

	// Use a for loop to check for box width, height, line length and contents, etc
	for (int i = 0, nCounter = 0; i < sText.length(); i++, nCounter++) {
		if (sText[i] == '\n') {

			// Early check for line size
			if (nBoxWidth < nCounter) {
				// Get current terminal width
				GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiDirections);

				// Check for oversized line length
				if (csbiDirections.srWindow.Right - csbiDirections.srWindow.Left - 3 > nCounter) {
					nBoxWidth = nCounter;
				}
				else {
					nBoxWidth = csbiDirections.srWindow.Right - csbiDirections.srWindow.Left - 3;

					// Create new lines
					// Get change between box width and counter to determine number of lines
					int nNumOfLines = ((nCounter - nBoxWidth) / nBoxWidth) + 1;

					// Append new lines to vector
					for (int j = 0; j <= nNumOfLines; j++) {

						// Get line
						sLines.push_back(sBuffer.substr(0, nBoxWidth));

						// Erase line from string
						sBuffer.erase(0, nBoxWidth);
					}

					// Reset counter
					nCounter = 0;

					// Reset buffer
					sBuffer = "";

					continue;
				}

			}

			// Reset counter
			nCounter = 0;

			// Push accumulated buffer contents into vector
			sLines.push_back(sBuffer);

			// Reset buffer
			sBuffer = "";

			continue;
		}

		if (nBoxWidth < nCounter) {
			// Get current terminal width
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiDirections);

			// Check for oversized line length
			if (csbiDirections.srWindow.Right - csbiDirections.srWindow.Left - 3 > nCounter) {
				nBoxWidth = nCounter;
			}
			else {
				nBoxWidth = csbiDirections.srWindow.Right - csbiDirections.srWindow.Left - 3;
			}
		}

		sBuffer += sText[i];
	}

	// Check for line size - determines final box width
	int nBufferLength = sBuffer.length();
	if (nBoxWidth < nBufferLength) {
		// Get current terminal width
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiDirections);

		// Check for oversized line length
		if (csbiDirections.srWindow.Right - csbiDirections.srWindow.Left - 3 > nBufferLength) {
			nBoxWidth = nBufferLength;
			if (sBuffer != "") sLines.push_back(sBuffer);
		}
		else {
			nBoxWidth = csbiDirections.srWindow.Right - csbiDirections.srWindow.Left - 3;

			// Create new lines
			// Get change between box width and counter to determine number of lines
			int nNumOfLines = ((nBufferLength - nBoxWidth) / nBoxWidth) + 1;

			// Append new lines to vector
			for (int j = 0; j <= nNumOfLines; j++) {

				// Get line
				sLines.push_back(sBuffer.substr(0, nBoxWidth));

				// Erase line from string
				sBuffer.erase(0, nBoxWidth);
			}

			// Reset buffer
			sBuffer = "";
		}
	}
	else sLines.push_back(sBuffer);


	// to support 1 asterisk and 1 whitespace on each side of the text
	nBoxWidth += 4;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiDirections);
	if (csbiDirections.srWindow.Right - csbiDirections.srWindow.Left + 1 < nBoxWidth) {
		nBoxWidth = csbiDirections.srWindow.Right - csbiDirections.srWindow.Left + 1;
	}

	// Calculate left padding width based on box width (for centring text)
	int nLeftPaddingWidth = 0;
	if (bCentreBox) {
		nLeftPaddingWidth = ((csbiDirections.srWindow.Right - csbiDirections.srWindow.Left + 1) - nBoxWidth) / 2;
	}

	// Output padding
	colour(sColourGlobal, sColourGlobalBack);
	std::cout << std::string(nLeftPaddingWidth, ' ');

	// Firstly, output box top
	colour(sColourOutlineFore, sColourOutlineBack);
	std::cout  << std::string(nBoxWidth, '=');

	// Secondly, output text with appropriate padding using for loop to reiterate through line vector
	std::cout << '\n';
	for (int i = 0; i < sLines.size(); i++) {
		sBuffer = sLines[i];
		for (int j = 0; j < nBoxWidth - sLines[i].length() - 4; j++) { // -4 as the box width was first incremented by 4 before this
			sBuffer += ' '; // Add space
		}

		// Output padding
		colour(sColourGlobal, sColourGlobalBack);
		std::cout << std::string(nLeftPaddingWidth, ' ');

		// Output box border character
		colour(sColourOutlineFore, sColourOutlineBack);
		std::cout << "* ";
		colour(sColourTextFore, sColourTextBack);

		// Output line and spaces
		std::cout << sBuffer;

		// Output padding
		colour(sColourOutlineFore, sColourOutlineBack);
		std::cout << " *\n";
	}

	// Output padding
	colour(sColourGlobal, sColourGlobalBack);
	std::cout << std::string(nLeftPaddingWidth, ' ');

	// Finally, output box bottom
	colour(sColourOutlineFore, sColourOutlineBack);
	std::cout << std::string(nBoxWidth, '=');

	std::cout << '\n';
	colour(sColourTextFore, sColourTextBack);

	return;
}

// Converter from wide string to string
std::string ws2s(const std::wstring& wstr) {
	// Use UTF-8 for this
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

// Converter from string to wide string
std::wstring s2ws(const std::string& str) {
	// Use UTF-8 for this
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;

	return converterX.from_bytes(str);
}

// Converter from boolean to string ("true" or "false")
std::string BoolToString(const bool bValue)
{
	// Convert to respective strings
	switch (bValue) {
	case true:
		return "True";
		break;
	case false:
		return "False";
		break;
	default:
		return "Failed";
		break;
	}

	// If switch wasn't exited, return failed (there's no way this could've happened in a normal scenario)
	return "failed";
}

// Checks whether std::string argument is a number or not - long double
bool isNumberld(const std::string sNumberTest) {
	// Not a number as there's nothing in the string
	if (sNumberTest.length() <= 0) return false;

	// Check if all digits are numbers
	for (size_t i = 0; i < sNumberTest.length(); i++) {
		// Check for negative at the beginning if i is 0

		// skip character if it's a decimal point
		if (sNumberTest[i] == '.') continue;
		else if (sNumberTest[i] == '-' && i == 0) {
			// Allowed, since i is at the beginning of the string
			continue;
		}
		else if (isdigit(sNumberTest[i]) == false) return false;
	}

	// Test if above stold() range
	try {
		long double nRangeTest = std::stold(sNumberTest);
	}
	catch (const std::out_of_range&) {
		VerbosityDisplay("In isNumberld(): Exception caught - Number is too high/low (out of range).");
		return false;
	}
	catch (const std::invalid_argument&) {
		VerbosityDisplay("In isNumberld(): Exception caught - Argument cannot be converted whatsoever (invalid argument).");
		return false;
	}

	return true;
}

// Checks whether std::string argument is a number or not - 64-bit integer
bool isNumberll(const std::string sNumberTest) {
	// Not a number as there's nothing in the string
	if (sNumberTest.length() <= 0) return false;

	// Check if all digits are numbers
	for (size_t i = 0; i < sNumberTest.length(); i++) {
		// skip character if it's a decimal point
		if (sNumberTest[i] == '.') continue;
		else if (sNumberTest[i] == '-' && i == 0) {
			// Allowed, since i is at the beginning of the string
			continue;
		}
		else if (isdigit(sNumberTest[i]) == false) return false;
	}

	// Test if above stoll() range
	try {
		long long int nRangeTest = std::stoll(sNumberTest);
	}
	catch (const std::out_of_range&) {
		VerbosityDisplay("In isNumberll(): Exception caught - Number is too high/low (out of range).");
		return false;
	}
	catch (const std::invalid_argument&) {
		VerbosityDisplay("In isNumberll(): Exception caught - Argument cannot be converted whatsoever (invalid argument).");
		return false;
	}

	return true;
}

// Checks whether std::string argument is a number or not - 32-bit integer
bool isNumberl(const std::string sNumberTest) {
	// Not a number as there's nothing in the string
	if (sNumberTest.length() <= 0) return false;

	// Check if all digits are numbers
	for (size_t i = 0; i < sNumberTest.length(); i++) {
		// skip character if it's a decimal point
		if (sNumberTest[i] == '.') continue;
		else if (sNumberTest[i] == '-' && i == 0) {
			// Allowed, since i is at the beginning of the string
			continue;
		}
		else if (isdigit(sNumberTest[i]) == false) return false;
	}

	// Test if above stoll() range
	try {
		long int nRangeTest = std::stol(sNumberTest);
	}
	catch (const std::out_of_range&) {
		VerbosityDisplay("In isNumberl(): Exception caught - Number is too high/low (out of range).");
		return false;
	}
	catch (const std::invalid_argument&) {
		VerbosityDisplay("In isNumberl(): Exception caught - Argument cannot be converted whatsoever (invalid argument).");
		return false;
	}

	return true;
}

// Checks whether std::string argument is a number or not - integer
// bSetErrorsAsVerbose - Any errors that occur are set as verbose when this is TRUE, else the messages are streamed to std::cerr.
bool isNumberi(const std::string sNumberTest, bool bSetErrorsAsVerbose) {
	// Not a number as there's nothing in the string
	if (sNumberTest.length() <= 0) return false;

	// Check if all digits are numbers
	for (size_t i = 0; i < sNumberTest.length(); i++) {
		// skip character if it's a decimal point
		if (sNumberTest[i] == '.') continue;
		else if (sNumberTest[i] == '-' && i == 0) {
			// Allowed, since i is at the beginning of the string
			continue;
		}
		else if (isdigit(sNumberTest[i]) == false) return false;
	}

	// Test if above stoi() range
	try {
		int nRangeTest = std::stoi(sNumberTest);
	}
	catch (const std::out_of_range&) {
		if (bSetErrorsAsVerbose) VerbosityDisplay("In isNumberi(): Exception caught - Number is too high/low (out of range).");
		else std::cerr << wordWrap("In isNumberi(): Exception caught - Number is too high/low (out of range).\n");
		return false;
	}
	catch (const std::invalid_argument&) {
		if (bSetErrorsAsVerbose) VerbosityDisplay("In isNumberi(): Exception caught - Argument cannot be converted whatsoever (invalid argument).");
		else std::cerr << wordWrap("In isNumberi(): Exception caught - Argument cannot be converted whatsoever (invalid argument).\n");
		return false;
	}

	return true;
}

// Checks whether std::string argument is a number or not - unsigned 64-bit integer
bool isNumberull(const std::string sNumberTest) {
	// Not a number as there's nothing in the string
	if (sNumberTest.length() <= 0) return false;

	// Check if all digits are numbers
	for (size_t i = 0; i < sNumberTest.length(); i++) {
		// skip character if it's a decimal point
		if (sNumberTest[i] == '.') continue;
		else if (isdigit(sNumberTest[i]) == false) return false;
	}

	// Test if above stoll() range
	try {
		uint64_t nRangeTest = std::stoull(sNumberTest);
	}
	catch (const std::out_of_range&) {
		VerbosityDisplay("In isNumberull(): Exception caught - Number is too high/low (out of range).");
		return false;
	}
	catch (const std::invalid_argument&) {
		VerbosityDisplay("In isNumberull(): Exception caught - Argument cannot be converted whatsoever (invalid argument).");
		return false;
	}

	return true;
}

// Checks whether std::string argument is a number or not - unsigned 32-bit integer
bool isNumberul(const std::string sNumberTest) {
	// Not a number as there's nothing in the string
	if (sNumberTest.length() <= 0) return false;

	// Check if all digits are numbers
	for (size_t i = 0; i < sNumberTest.length(); i++) {
		// skip character if it's a decimal point
		if (sNumberTest[i] == '.') continue;
		else if (isdigit(sNumberTest[i]) == false) return false;
	}

	// Test if above stoll() range
	try {
		unsigned long int nRangeTest = std::stoul(sNumberTest);
	}
	catch (const std::out_of_range&) {
		VerbosityDisplay("In isNumberul(): Exception caught - Number is too high/low (out of range).");
		return false;
	}
	catch (const std::invalid_argument&) {
		VerbosityDisplay("In isNumberul(): Exception caught - Argument cannot be converted whatsoever (invalid argument).");
		return false;
	}

	return true;
}

// Function to clear keyboard buffer
void ClearKeyboardBuffer() {
	while (_kbhit()) {
		_getch_nolock(); // faster than _getch() - not thread-safe but this function is meant to be single-threaded
	}
	return;
}

// Function to set console cursor position
inline void SetCursorPosition(int x, int y) {
	COORD CursorPos{};
	CursorPos.X = x;
	CursorPos.Y = y;
	if (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPos)) {
		// Don't display on windows terminal VT bug
		bool bHideMessage = false;
		if (CheckIfCursorExceedScreenBufferHeight()) {
			bHideMessage = true;
		}

		VerbosityDisplay("In SetCursorPosition(): ERROR - Failed to set console cursor position with error code " + std::to_string(GetLastError()) + ".\n", 10000, bHideMessage);
	}

	return;
}

// wordWrap - Adds line/word wrapping to a string, based on the console window width.
//
// This takes in an std::string as an argument, and returns another std::string with the word wrapping in the string.
std::string wordWrap(std::string text, long long int nCustomStartCounter, long long int nCustomWidth) {
	std::string result = "";
	long long int counter = nCustomStartCounter;

	// Cannot do word wrapping if disabled
	if (bWordWrapToggle == false) return text;

	// Get console window size
	CONSOLE_SCREEN_BUFFER_INFO csbiWordWrap;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiWordWrap);
	long long int width = csbiWordWrap.srWindow.Right - csbiWordWrap.srWindow.Left + 1;
	if (nCustomWidth > 0) width = nCustomWidth;


	if (counter < 0) {
		// Set counter to the cursor X position, due to the fact that it shouldn't be 
		// assumed that word wrapping will start from the very beginning of a line
		counter = csbiWordWrap.dwCursorPosition.X;
	}

	size_t lastSpace = -1;
	for (size_t i = 0; i < text.length(); i++) {

		// Counter mods
		if (text[i] == '\n') {
			counter = -1;
			lastSpace = -1;
		}
		else if (text[i] == '\t') {

			// tabstop is usually 8 spaces; might be dynamic in future
			size_t nNumOfSpacesForTab = 8 - (counter % 8);
			if (nNumOfSpacesForTab == 0) nNumOfSpacesForTab = 8;
			counter += nNumOfSpacesForTab;
		}

		// Counter check
		if (counter >= width) {
			if (lastSpace != -1) {
				result[lastSpace] = '\n';
				counter = i - lastSpace - 1;
			}
			else {
				result += '\n';
				counter = 0;
			}
		}
		if (text[i] == ' ') {
			lastSpace = result.length();
		}

		result += text[i];

		counter++;
	}
	return result;
}

// Function to display extra help information for specific commands for user's sake
void DirectionsDisplay(std::string sPrompt, bool bLeftPaddingWidth)
{
	if (bDisplayDirections == true)
	{
		// Output directions
		OutputBoxWithText(sPrompt, LRED, sColourGlobalBack, GRN, sColourGlobalBack, bLeftPaddingWidth);
	}

	return;
}

// Function to display verbose messages at specific times
inline void VerbosityDisplay(std::string sPrompt, int nObjectID, bool bForceDontDisplayMessage, bool bForceDontLog) {
	if (bDisplayVerboseMessages == true && bForceDontDisplayMessage == false) {
		colour(GRAY, BLK);
		std::cerr << "Verbose Message: " << sPrompt << std::endl;
		colour(sColourGlobal, sColourGlobalBack);
	}

	return;
}

// Function to display error messages at specific times
inline void UserErrorDisplay(std::string sError, int nObjectID) {

	// Output user-space error in red text with word wrapping
	colour(RED, sColourGlobalBack);
	std::cerr << wordWrap(sError);
	colour(sColourGlobal, sColourGlobalBack);

	return;
}

// Function to handle +/- number input - long double
long double NumInputld(std::string sPrompt) {
	long double num;

	while (true) {
		std::cout << wordWrap(sPrompt);
		colour(LYLW, sColourGlobalBack);
		std::cin >> num;
		colour(sColourGlobal, sColourGlobalBack);

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			colour(RED, sColourGlobalBack);
			std::cerr << wordWrap("Your input was incorrect, or the number inputted was too high/low. Please try again.\n");
			colour(sColourGlobal, sColourGlobalBack);
			continue;
		}
		else {
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			break;
		}
	}

	return num;
}

// Function to handle +/- number input - long long int
long long int NumInputll(std::string sPrompt) {
	long long int num;

	while (true) {
		std::cout << wordWrap(sPrompt);
		colour(LYLW, sColourGlobalBack);
		std::cin >> num;
		colour(sColourGlobal, sColourGlobalBack);

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			colour(RED, sColourGlobalBack);
			std::cerr << wordWrap("Your input was incorrect, or the number inputted was too high/low. Please try again.\n");
			colour(sColourGlobal, sColourGlobalBack);
			continue;
		}
		else {
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			break;
		}
	}

	return num;
}

// Function to handle +/- number input - long int
long int NumInputl(std::string sPrompt) {
	long int num;

	while (true) {
		std::cout << wordWrap(sPrompt);
		colour(LYLW, sColourGlobalBack);
		std::cin >> num;
		colour(sColourGlobal, sColourGlobalBack);

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			colour(RED, sColourGlobalBack);
			std::cerr << wordWrap("Your input was incorrect, or the number inputted was too high/low. Please try again.\n");
			colour(sColourGlobal, sColourGlobalBack);
			continue;
		}
		else {
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			break;
		}
	}

	return num;
}

// Function to handle +/- number input - int
int NumInputi(std::string sPrompt) {
	int num;

	while (true) {
		std::cout << wordWrap(sPrompt);
		colour(LYLW, sColourGlobalBack);
		std::cin >> num;
		colour(sColourGlobal, sColourGlobalBack);

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			colour(RED, sColourGlobalBack);
			std::cerr << wordWrap("Your input was incorrect, or the number inputted was too high/low. Please try again.\n");
			colour(sColourGlobal, sColourGlobalBack);
			continue;
		}
		else {
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			break;
		}
	}

	return num;
}

// Function to handle only positive number input - uint64_t
uint64_t PositiveNumInputull(std::string sPrompt) {
	uint64_t num = 0;
	std::string sInput = "";
	bool bSuccess = true;

	while (true) {
		// Reset variables on next iteration
		bSuccess = true;

		std::cout << wordWrap(sPrompt);
		colour(LYLW, sColourGlobalBack);
		std::cin >> sInput;
		colour(sColourGlobal, sColourGlobalBack);

		// Convert string input for integer into unsigned 64-bit integer, while checking for correct input AND negative number
		// Check if number
		if (isNumberull(sInput) == false) {
			bSuccess = false;
		}

		// Convert to number
		if (bSuccess == true) {
			num = std::stoull(sInput);
		}

		if (std::cin.fail() || num < 0 || bSuccess == false) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			colour(RED, sColourGlobalBack);
			std::cerr << wordWrap("Your input was incorrect, or the number inputted was too high/low. Please try again.\n");
			colour(sColourGlobal, sColourGlobalBack);
			continue;
		}
		else {
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			break;
		}
	}

	return num;
}

// Function to handle only positive number input - unsigned long int
unsigned long int PositiveNumInputul(std::string sPrompt) {
	unsigned long int num = 0;
	std::string sInput = "";
	bool bSuccess = true;

	while (true) {
		// Reset variables on next iteration
		bSuccess = true;

		std::cout << wordWrap(sPrompt);
		colour(LYLW, sColourGlobalBack);
		std::cin >> sInput;
		colour(sColourGlobal, sColourGlobalBack);

		// Convert string input for integer into unsigned 64-bit integer, while checking for correct input AND negative number
		// Check if number
		if (isNumberul(sInput) == false) {
			bSuccess = false;
		}

		// Convert to number
		if (bSuccess == true) {
			num = std::stoul(sInput);
		}

		if (std::cin.fail() || num < 0 || bSuccess == false) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			colour(RED, sColourGlobalBack);
			std::cerr << wordWrap("Your input was incorrect, or the number inputted was too high/low. Please try again.\n");
			colour(sColourGlobal, sColourGlobalBack);
			continue;
		}
		else {
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			break;
		}
	}

	return num;
}

// Function to assist with string output by preventing reuse of code
std::string StrInput(std::string sPrompt) {
	std::string string;

	// Prompt
	std::cout << wordWrap(sPrompt);
	colour(LYLW, sColourGlobalBack);
	getline(std::cin, string);
	colour(sColourGlobal, sColourGlobalBack);

	return string;
}

// YesNoInput - allows for y/n input
bool YesNoInput(std::string sPrompt) {
	char cInput;

	while (true) {
		std::cout << wordWrap(sPrompt);
		colour(LYLW, sColourGlobalBack);
		std::cin >> cInput;
		colour(sColourGlobal, sColourGlobalBack);
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			colour(RED, sColourGlobalBack);
			std::cerr << wordWrap("Your input was incorrect. Please try again.\n");
			colour(sColourGlobal, sColourGlobalBack);
			continue;
		}
		else {
			std::cin.ignore(std::numeric_limits<int>::max(), '\n');
			break;
		}
	}
	if (cInput == 'y' || cInput == 'Y') {
		return true;
	}
	else {
		return false;
	}

	// Unknown error, just post TRUE
	return true;
}

// SetWindowTitle - Function to set title for the console window.
// Arguments: sTitle for the title string.
bool SetWindowTitle(std::string sTitle) {
	if (bAnsiVTSequences) {
		if (sTitle.length() > 256) {
			VerbosityDisplay("ERROR: In SetWindowTitle() - String argument incorrect due to ANSI 256 max string char limit for title.\n");
		}

		// Use ANSI VT sequences if they work on current terminal
		std::cout << "\x1b]0;" << sTitle << "\x1b\x5c";
	}
	else {
		// Use Windows SetConsoleTitleA if ANSI VT sequences don't work on current terminal window
		if (SetConsoleTitleA(sTitle.c_str())) {
			return true;
		}
		else {
			VerbosityDisplay("ERROR: In SetWindowTitle() - String argument incorrect due to Win32 API 256 max string char limit for title.\n");
			return false;
		}
	}

	return true;
}

// Clear screen function (Flushes whole screen buffer)
void cls() {

	// Using ANSI escape codes to clear the screen is a lot neater and cross-platform
	if (bAnsiVTSequences == true) {
		std::cout << "\033c"; // fully clear the scrollback buffer
		// As everything was reset, set everything back to normal
		// Cursor attributes
		SetCursorAttributes();

		colour(sLastColourFore, sLastColourBack); // Set colours to last set colour
		std::cout << "\033[2J\033[1;1H";
		return;
	}
	else {
		// If ANSI escape codes cannot be used, just do the classical method
		HANDLE h;
		CHAR_INFO v3;
		COORD v4;
		SMALL_RECT v5;
		CONSOLE_SCREEN_BUFFER_INFO v6;

		if ((h = (HANDLE)GetStdHandle(0xFFFFFFF5), (unsigned int)GetConsoleScreenBufferInfo(h, &v6)))
		{
			v5.Right = v6.dwSize.X;
			v5.Bottom = v6.dwSize.Y;
			v3.Char.UnicodeChar = 32;
			v4.Y = -v6.dwSize.Y;
			v3.Attributes = v6.wAttributes;
			v4.X = 0;
			*(DWORD*)&v5.Left = 0;
			ScrollConsoleScreenBufferW(h, &v5, 0, v4, &v3);
			v6.dwCursorPosition = { 0 };
			SetCursorPosition(v6.dwCursorPosition.X, v6.dwCursorPosition.Y);
		}
		return;
	}
	return;

}

// Function that outputs text with random colours 
void RandomColourOutput(std::string sText, std::string sCustomBackgroundColour = sColourGlobalBack) {

	// Add word wrapping
	sText = wordWrap(sText);

	// j is for string check, i is for colour indicator
	// for loop
	for (int i = 1, j = 0; j < sText.size(); j++, i++)
	{
		// Colour indicator going past 16 will give an error
		if (i > 16) i = 1;

		if (NumberToColour(i) == sCustomBackgroundColour && !bAutoReadableContrast) {
			colour(sColourGlobal, sCustomBackgroundColour);
		}
		else colour(NumberToColour(i), sCustomBackgroundColour);

		// Output with colour
		std::cout << sText[j];
	}

	return;
}

// CentreColouredText - Takes in a title string and outputs it centred to the console with padding in relation to the console window size.
// Arguments: sText is for the string input, nTypeOfText is for the type of text (title (1), subheading (2), etc).
void CentreColouredText(std::string sText, short int nTypeOfText) {
	// Work out width
	int nWidth = 0;
	// Get console window size
	CONSOLE_SCREEN_BUFFER_INFO csbiCentreCT;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiCentreCT);
	// Assign window width to nWidth
	nWidth = csbiCentreCT.srWindow.Right - csbiCentreCT.srWindow.Left + 1;

	// Check to prevent memory overspill
	if (sText.length() >= (nWidth + 1)) {

		// Output with correct colour
		if (nTypeOfText == 1) colourTitle();
		else if (nTypeOfText == 2) colourSubheading();

		// Word wrapping because text is longer than display width, so word wrapping is necessary
		std::cout << wordWrap(sText);

		// Reset underline and bold
		std::cout << NOULINE_STR << NOBOLD_STR;

		// Default colour
		colour(sColourGlobal, sColourGlobalBack);

		return;
	}

	// Output string of spaces first with default colour
	colour(sColourGlobal, sColourGlobalBack);
	std::cout << wordWrap(std::string((nWidth - sText.length()) / 2, ' '));

	// Then output the string itself, with correct colour
	if (nTypeOfText == 1) colourTitle();
	else if (nTypeOfText == 2) colourSubheading();
	std::cout << sText;

	// Reset underline and bold
	std::cout << NOULINE_STR << NOBOLD_STR;

	// Default colour
	colour(sColourGlobal, sColourGlobalBack);

	return;
}

// CentreText - Takes in a string and outputs it with padding in relation to the console window size.
std::string CentreText(std::string sText) {
	// Work out width
	int nWidth = 0;
	// Get console window size
	CONSOLE_SCREEN_BUFFER_INFO csbiCentreText;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiCentreText);
	// Assign window width to nWidth
	nWidth = csbiCentreText.srWindow.Right - csbiCentreText.srWindow.Left + 1;

	// Check to prevent memory overspill
	if (sText.length() >= (nWidth + 1)) return sText;

	return wordWrap(std::string((nWidth - sText.length()) / 2, ' ') + sText);
}

// slowcharCentredFn - Takes in a string and outputs it centred to the console with padding in relation to the console window size.
// Arguments: sText is for string input, bNewLine is for adding a newline after the output.
void slowcharCentredFn(bool bNewLine, std::string sText) {
	// Work out width
	int nWidth = 0;
	// Get console window size
	CONSOLE_SCREEN_BUFFER_INFO csbiCentreCT;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiCentreCT);
	// Assign window width to nWidth
	nWidth = csbiCentreCT.srWindow.Right - csbiCentreCT.srWindow.Left + 1;

	// Check to prevent memory overspill
	if (sText.length() >= (nWidth + 1)) {
		sText = wordWrap(sText);

		for (int i = 0; i <= sText.length(); i++) {
			// Pause for a couple of milliseconds to make output slow
			if (i > 0) {
				// Space backtrack for word wrapping detection
				if (not (sText[i - 1] == ' ' && sText[i] == ' ')) {
					sleep(nSlowCharSpeed);
				}
			}
			else {
				sleep(nSlowCharSpeed);
			}

			std::cout << sText[i];
		}
		// In case a key was pressed while operation was commencing
		ClearKeyboardBuffer();
		return;
	}

	// Firstly, centre the cursor position
	SetCursorPosition((nWidth - sText.length()) / 2, csbiCentreCT.dwCursorPosition.Y);
	// Then output the string itself like slowcharfn()
	for (int i = 0; i <= sText.length(); i++) {
		sleep(nSlowCharSpeed);
		std::cout << sText[i];
	}
	// Output newline if call says so
	if (bNewLine) std::cout << std::endl;
	// In case a key was pressed while operation was commencing
	ClearKeyboardBuffer();
	return;
}

// Function for outputting characters slowly with the same colour
void slowcolourfn(std::string nColourFore, std::string nColourBack, std::string sSlowchar) {
	size_t size = 0;
	// Add word wrapping
	sSlowchar = wordWrap(sSlowchar);
	// Get size of sSlowChar
	size = sSlowchar.size();
	colour(nColourFore, nColourBack);
	for (int i = 0; i <= size; i++) {

		// Pause for a couple of milliseconds to make output slow
		if (i > 0) {
			// Space backtrack for word wrapping detection
			if (not (sSlowchar[i - 1] == ' ' && sSlowchar[i] == ' ')) {
				sleep(nSlowCharSpeed);
			}
		}
		else {
			sleep(nSlowCharSpeed);
		}

		std::cout << sSlowchar[i];
	}
	// In case a key was pressed while operation was commencing
	ClearKeyboardBuffer();
	colour(sColourGlobal, sColourGlobalBack);

	return;
}

// Function for outputting characters slowly
void slowcharfn(bool nline, std::string sSlowchar) {
	size_t size = 0;

	// Add word wrapping
	sSlowchar = wordWrap(sSlowchar);
	size = sSlowchar.size();
	for (int i = 0; i <= size; i++) {

		if (i > 0) {
			// Space backtrack for word wrapping detection
			if (not (sSlowchar[i - 1] == ' ' && sSlowchar[i] == ' ')) {
				sleep(nSlowCharSpeed);
			}
		}
		else {
			sleep(nSlowCharSpeed);
		}

		std::cout << sSlowchar[i];
	}

	if (nline == true) {
		std::cout << std::endl;
	}

	// In case a key was pressed while operation was commencing
	ClearKeyboardBuffer();

	return;
}

// Function for outputting characters with random colours
void SlowCharColourful(std::string sText, bool bIncludeBackground) {

	// Add word wrapping
	sText = wordWrap(sText);

	// Repeat output with random colour each time
	for (int i = 0; i < sText.size(); i++)
	{
		// Get random colour number
		int nRandomNumber = RandNum(16, 1);

		if (bIncludeBackground == true) {
			// Create another background random number
			int nRandomNumberBack = RandNum(16, 1);

			// Set background and foreground colours to random
			colour(NumberToColour(nRandomNumber), NumberToColour(nRandomNumberBack));
		}
		else {
			// Set foreground colour to the random number
			colour(NumberToColour(nRandomNumber), sColourGlobalBack);
		}

		// Output a character
		std::cout << sText[i];

		// Pause for a couple of milliseconds to make output slow
		if (i > 0) {
			// Space backtrack for word wrapping detection
			if (not (sText[i - 1] == ' ' && sText[i] == ' ')) {
				sleep(nSlowCharSpeed);
			}
		}
		else {
			sleep(nSlowCharSpeed);
		}
	}

	// Finish off with newline
	colour(sColourGlobal, sColourGlobalBack);
	std::cout << '\n';
	ClearKeyboardBuffer();
	return;
}

// A function to display an exiting message without typing too much.
inline void Exiting() {
	colour(YLW, sColourGlobalBack);
	std::cout << "\nExiting...\n";
	colour(sColourGlobal, sColourGlobalBack);
	return;
}

// A function designated for resetting the global colours.
inline void ResetColour()
{
	// Reset all RGB to default
	sColourGlobal = LWHT;
	sColourGlobalBack = BLK;

	// Optional reset-everything command for ANSI VT Sequence-supporting terminals
	if (bAnsiVTSequences) {
		std::cout << "\x1b[" << RESETALL << "m";
	}

	// Output reset message with default colours
	colour(sColourGlobal, sColourGlobalBack);
	std::cout << "\nColours have been reset to default.\n";

	return;
}

// A function that checks for VT Terminal Sequence support, enables them if support is there
// and disables them if support isn't there. Returns true/false respectively, too.
bool EnableVTMode()
{
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		return false;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	dwMode |= ENABLE_WRAP_AT_EOL_OUTPUT;

	if (!SetConsoleMode(hOut, dwMode))
	{
		return false;
	}
	return true;
}

// A function to enable shutdown privileges for commands that require them.
// Returns false if they couldn't be enabled, but returns true otherwise.
bool EnableShutdownPrivileges()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	bool bSuccess = true;

	// Get a token for the current process
	bSuccess = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	if (bSuccess == false) return bSuccess; // Failed

	// Lookup the LUID for the shutdown privilege
	bSuccess = LookupPrivilegeValueW(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	if (bSuccess == false) return bSuccess; // Failed

	tkp.PrivilegeCount = 1; // Set the number of privileges to change
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; // Enable the shutdown privilege

	// Adjust the token privileges
	bSuccess = AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (bSuccess == false) return bSuccess; // Failed

	// Close the token handle
	bSuccess = CloseHandle(hToken);
	if (bSuccess == false) return bSuccess; // Failed

	// Privileges enabled
	VerbosityDisplay("Shutdown Privileges have been successfully enabled.\n");

	return bSuccess;
}

// Check if cursor exceeds the maximum set screen buffer height
// Returns TRUE if this is the case, otherwise FALSE is returned.
// the cls() function is recommended to be called after this function if TRUE is returned,
// as the terminal buffer may need to be reset.
//
bool CheckIfCursorExceedScreenBufferHeight()
{
	// Get console screen buffer info - have all data ready before check
	CONSOLE_SCREEN_BUFFER_INFO csbiCursorScreenBufCheck;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiCursorScreenBufCheck);

	// Check by comparing set screen buffer height with cursor height
	if (csbiCursorScreenBufCheck.dwCursorPosition.Y >= csbiCursorScreenBufCheck.dwSize.Y - 1) {
		return true;
	}
	else return false;
}