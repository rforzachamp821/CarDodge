#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include "CarDodge_Definitions\Definitions.h"
#include "CarDodge_Definitions\ZeeTerminalCoreLib.h"
#include "RyRyEncryptionAlgorithm/RyRyEncrypt.h"
#include <Windows.h>
#include <iomanip>
#include <fstream>
#include "CarDodge_CodeFiles\CarInfo.cpp"
#include "CarDodge_CodeFiles\OptionSelectEngine.cpp"
#include "CarDodge_CodeFiles\CarDodgeCore.cpp"
#include "CarDodge_CodeFiles\CarDodgeMain.cpp"

extern CarInfo EnemyCars[128];
extern CarInfo UserCar;
extern const int nEnemyCarArraySize;
CarStyles Style;

// Menu colours
std::string sMenuColourFore = LWHT;
std::string sMenuColourBack = LBLU;


// InitialiseCarDodge - Initialises the game elements that are external of the core system (colour system, etc)
//
void InitialiseCarDodge() 
{
	// Attempt to Enable Virtual Terminal Sequences
	if (EnableVTMode()) {
		bAnsiVTSequences = true;

		// Set colour definitions with RGB ANSI
		// Standard colours
		BLK = BLK_ANSI;
		BLU = BLU_ANSI;
		GRN = GRN_ANSI;
		CYN = CYN_ANSI;
		RED = RED_ANSI;
		MAG = MAG_ANSI;
		YLW = YLW_ANSI;
		WHT = WHT_ANSI;

		// Bright colours
		GRAY = GRAY_ANSI;
		LBLU = LBLU_ANSI;
		LGRN = LGRN_ANSI;
		LCYN = LCYN_ANSI;
		LRED = LRED_ANSI;
		LMAG = LMAG_ANSI;
		LYLW = LYLW_ANSI;
		LWHT = LWHT_ANSI;
	}
	else {
		bAnsiVTSequences = false;

		// Set colour definitions to support the older Windows Console API
		// Standard colours
		BLK = BLK_WIN32;
		BLU = BLU_WIN32;
		GRN = GRN_WIN32;
		CYN = CYN_WIN32;
		RED = RED_WIN32;
		MAG = MAG_WIN32;
		YLW = YLW_WIN32;
		WHT = WHT_WIN32;

		// Bright colours
		GRAY = GRAY_WIN32;
		LBLU = LBLU_WIN32;
		LGRN = LGRN_WIN32;
		LCYN = LCYN_WIN32;
		LRED = LRED_WIN32;
		LMAG = LMAG_WIN32;
		LYLW = LYLW_WIN32;
		LWHT = LWHT_WIN32;

		NOULINE_STR = "";
		ULINE_STR = "";
	}

	// Initialise the special colour variables using the colour formats were just initialised
	sColourHighlight = LWHT;
	sColourHighlightBack = BLU;

	sColourTitle = BLK;
	sColourTitleBack = LCYN;

	sColourSubheading = LWHT;
	sColourSubheadingBack = MAG;

	// Initialise menu colours using the colour formats that were just initialised
	sMenuColourFore = LWHT;
	sMenuColourBack = LBLU;

	// Initialise EnemyCars array
	for (int i = 0; i < nEnemyCarArraySize; i++) {
		EnemyCars[i].CarStyle = Style.EnemyCar;
	}

	// Initialise default car
	UserCar.CarStyle = Style.UserCarDefault;

	// Set window title
	SetWindowTitle("Car Dodge v0.5.0");

	// Menu screen colours
	colour(sMenuColourFore, sMenuColourBack);

	// Clear screen to set colours to whole screen
	cls();
}

// VerifyIfGameIsRunnable - Verify if game is runnable with current terminal attributes and settings.
//
bool VerifyIfGameIsRunnable(bool bInMainGame, bool bDisplayErrorMessage) {
	CONSOLE_SCREEN_BUFFER_INFO csbiMainMenu;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiMainMenu);

	// Check console height and width
	if ((csbiMainMenu.srWindow.Bottom - csbiMainMenu.srWindow.Top + 1 < 30) || (csbiMainMenu.srWindow.Right - csbiMainMenu.srWindow.Left + 1 < 120)) {
		std::cout << '\r';
		if (bDisplayErrorMessage)
			UserErrorDisplay("Failed to " + std::string((bInMainGame ? "continue operation of " : "start ")) + "game. The game window is too small in height and/or width to run the game."
				"\nPlease ensure that it is at least 30 character cells in height and 120 character cells in width, and try again.\n");
		return false; // Game is not runnable with current height
	}

	return true;
}

// GetWindowWidthInChars - Returns the console window width, in character cells.
//
int GetWindowWidthInChars() {
	CONSOLE_SCREEN_BUFFER_INFO csbiWidth;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiWidth);
	return csbiWidth.srWindow.Right - csbiWidth.srWindow.Left;
}

// RenderCar - Renders a car specified by CarInfo argument.
//
void RenderCar(CarInfo ciCarToRender) {
	// Set starting point
	SetCursorPosition(ciCarToRender.bottomLeft.X, ciCarToRender.bottomLeft.Y - 3);

	// Render all 4 rows
	for (int i = 0; i < 4; i++) {
		// Render car character on screen
		std::cout.write(ciCarToRender.CarStyle[i].data(), 4);
		// Move to next position down for render
		if (i < 3) SetCursorPosition(ciCarToRender.bottomLeft.X, ciCarToRender.bottomLeft.Y + (i - 2));
	}

	return;
}

// RenderMainMenuCars - Renders the main menu cars (decorative).
//
inline void RenderMainMenuCars() {
	CONSOLE_SCREEN_BUFFER_INFO csbiMainMenu;
	CarInfo ciMainMenuUserCar;
	CarInfo ciMainMenuEnemyCar;
	short int nScreenCentre = 0;
	short int nDirectionsTextLeftColumn = 0;
	short int nDirectionsTextRightColumn = 0;

	// Initialise console info struct
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiMainMenu);

	// Get screen centre
	nScreenCentre = (csbiMainMenu.srWindow.Right - csbiMainMenu.srWindow.Left) / 2;

	// Get left and right columns of directions text that will come up in menu, derivin off of nScreenCentre
	nDirectionsTextLeftColumn = nScreenCentre - 26; // 26 to count for the centre column
	nDirectionsTextRightColumn = nScreenCentre + 27; // 27 is half of 54

	// Set car styles
	ciMainMenuUserCar.CarStyle = Style.UserCarDefault;
	ciMainMenuEnemyCar.CarStyle = Style.EnemyCar;

	// Set up left side of main menu
	ciMainMenuUserCar.bottomLeft = { short(nDirectionsTextLeftColumn - 7), 14 };
	ciMainMenuUserCar.bottomRight = { short(ciMainMenuUserCar.bottomLeft.X + 4), ciMainMenuUserCar.bottomLeft.Y };

	ciMainMenuEnemyCar.bottomLeft = { short(nDirectionsTextLeftColumn - 10), 9 };
	ciMainMenuEnemyCar.bottomRight = { short(ciMainMenuEnemyCar.bottomLeft.X + 4), ciMainMenuEnemyCar.bottomLeft.Y };

	// Render left side of main menu
	colour(LYLW, sMenuColourBack);
	RenderCar(ciMainMenuEnemyCar);
	RenderCar(ciMainMenuUserCar);

	// Set up right side of main menu
	ciMainMenuUserCar.bottomLeft = { short(nDirectionsTextRightColumn + 3), 14 };
	ciMainMenuUserCar.bottomRight = { short(ciMainMenuUserCar.bottomLeft.X + 4), ciMainMenuUserCar.bottomLeft.Y };
	ciMainMenuUserCar.CarStyle = Style.XtraAero;

	ciMainMenuEnemyCar.bottomLeft = { short(nDirectionsTextRightColumn + 8), 11 };
	ciMainMenuEnemyCar.bottomRight = { short(ciMainMenuEnemyCar.bottomLeft.X + 4), ciMainMenuEnemyCar.bottomLeft.Y };

	// Render left side of main menu
	RenderCar(ciMainMenuEnemyCar);
	RenderCar(ciMainMenuUserCar);

	// Render annotations for the cars on both sides of the main menu
	colour(LGRN, sMenuColourBack);
	SetCursorPosition(nDirectionsTextRightColumn + 9, 13);
	std::cout << "||";
	SetCursorPosition(nDirectionsTextRightColumn + 9, 14);
	std::cout << "VV";

	SetCursorPosition(nDirectionsTextRightColumn + 4, 8);
	std::cout << "^^";
	SetCursorPosition(nDirectionsTextRightColumn + 4, 9);
	std::cout << "||";

	colour(LMAG, sMenuColourBack);
	SetCursorPosition(nDirectionsTextLeftColumn, 9);
	std::cout << "\\/";
	SetCursorPosition(nDirectionsTextLeftColumn, 11);
	std::cout << "/\\";
	SetCursorPosition(nDirectionsTextLeftColumn - 6, 10);
	std::cout << "<-- CRASH";

	// Set colours back to normal
	colour(sColourGlobal, sColourGlobalBack);

	// Set cursor back to original place
	SetCursorPosition(0, 4);
}

int main(int argc, char** argv) 
{
	std::cout << "Getting ready...\n";
	
	// Initialise game
	InitialiseCarDodge();

	// Set menu colours as defaults - the actual game engine will change those automatically
	sColourGlobal = sMenuColourFore;
	sColourGlobalBack = sMenuColourBack;

	// Main menu loop
	while (true) {
		CarDodgeMain Main;
		OptionSelectEngine oseCarDodgeMenu;

		// Ensure that terminal is suitable for playing the game
		if (!VerifyIfGameIsRunnable(true, true)) {
			std::cout << "Take your time to resize the main menu window. The CarDodge main menu will reappear when the window is resized.\nPress any key to exit the game.\n";
			while (true) {
				// Check if user inputted anything
				if (_kbhit()) {
					_getch();
					return 1;
				}

				// Check window size repeatedly
				if (VerifyIfGameIsRunnable(true, false)) {
					// Exit loop - the window has been resized.
					break;
				}

				sleep(1);
			}

			// Clear screen to start terminal afresh from any error messages
			cls();
		}

		// Display title with background
		std::cout << "\n\n";
		OutputBoxWithText("Welcome to Car Dodge!", RED, YLW, LWHT, YLW, true);
		colour(sColourGlobal, sColourGlobalBack);

		// Render the decorative main menu cars
		RenderMainMenuCars();

		oseCarDodgeMenu.nSizeOfOptions = 5;
		std::string sOptions[] = {
			"Play",
			"Change Car",
			"How To Play",
			"High Score",
			"Exit"
		};
		oseCarDodgeMenu.sOptions = sOptions;

		// Prompt
		oseCarDodgeMenu.SetDirectionsText("Press the 'W' key or up arrow key to move up.\nPress the 'S' key or down arrow key to move down.\nPress ENTER to continue, or ESC to exit game.");
		int nInput = oseCarDodgeMenu.OptionSelect("Please select what you want to do:", "", true, true, true);

		if (nInput != 1) cls(); // optimisation for not executing too many cls() function calls, otherwise clear screen

		// Play main game
		if (nInput == 1) {
			Main.CarDodgeMainGame();
		}

		// Change car
		else if (nInput == 2) {
			Main.CarDodgeChangeUserCar();
		}

		// Instructions
		else if (nInput == 3) {
			Main.CarDodgeInstructions();
		}

		// High Scores
		else if (nInput == 4) {
			Main.CarDodgeHighScore();
		}

		// Exit
		else if (nInput == 5 || nInput == -1) {
			break;
		}

		// Incorrect input
		else {
			UserErrorDisplay("ERROR - Incorrect input. Please try again.\n");
			continue;
		}

		cls();
	}
	
	return 0;
}