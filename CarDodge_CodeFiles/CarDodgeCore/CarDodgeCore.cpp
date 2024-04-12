//
// CarDodgeCore.cpp - Houses the CarDodgeCore class.
//

#include <random>
#include <chrono>
#include <fstream>
#include "../../CarDodge_Definitions/ZTConstDefinitions.h"
#include "../../CarDodge_Definitions/ZTFormattingDefinitions.h"
#include "../../CarDodge_Definitions/ZeeTerminalCore.h"
#include "../../RyRyCryptor/RyRyCryptor.h"
#include "../CarInfo/CarInfo.h"
#include "CarDodgeCore.h"

CarInfo EnemyCars[128];
CarInfo UserCar;
const int nEnemyCarArraySize = sizeof(EnemyCars) / sizeof(CarInfo);

// InitialiseCarDodgeCore
void CarDodgeCore::InitialiseCarDodgeCore() {
	// Set session console window height - cannot be less than 30
	CONSOLE_SCREEN_BUFFER_INFO csbiTest;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiTest);
	nSessionConsoleHeight = csbiTest.srWindow.Bottom - csbiTest.srWindow.Top;
	if (nSessionConsoleHeight < 29) {
		nSessionConsoleHeight = 29;
	}

	// Initialise random engine
	std::random_device rdRandNum;
	RandEngine.seed(rdRandNum());

	// To improve performance, do not sync with stdio - it's not being used
	bSyncWithStdioPrevious = std::cout.sync_with_stdio(false);

	// Update high score global variable
	UpdateHighScoreFromFile();

	// Game Initialised - Set flag to true
	bCarDodgeCoreInitialised = true;

	return;
}

// GetNextEmptyEnemyCarIndex
int CarDodgeCore::GetNextEmptyEnemyCarIndex() {
	int nEnemyCarIndexFind = 0;

	// Reiterate through whole enemy car array to find next empty cell
	for (; nEnemyCarIndexFind < nEnemyCarArraySize; nEnemyCarIndexFind++) {
		if (EnemyCars[nEnemyCarIndexFind].bInUse == false) {
			return nEnemyCarIndexFind;
		}
	}

	return nEnemyCarArraySize + 1; // This is to indicate that there are no more spaces left
}

// RenderCar
void CarDodgeCore::RenderCar(CarInfo ciCarToRender) {
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

// EraseCar
void CarDodgeCore::EraseCar(CarInfo ciCarToErase) {
	// Set starting point
	SetCursorPosition(ciCarToErase.bottomLeft.X, ciCarToErase.bottomLeft.Y - 3);

	// Erase all 4 rows
	for (int i = 0; i < 4; i++) {
		// 4 because cars are 4 cells in width
		std::cout.write("    ", 4);
		// Move to next position down for erase
		if (i < 3) SetCursorPosition(ciCarToErase.bottomLeft.X, ciCarToErase.bottomLeft.Y + (i - 2));
	}

	return;
}

// RenderBorders
void CarDodgeCore::RenderBorders(std::string sSetBorderColourFore, std::string sSetBorderColourBack)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) {
		// Error handling later
	}

	// Initialise game if not initialised
	if (!bCarDodgeCoreInitialised) {
		InitialiseCarDodgeCore();
	}

	colour(sColourGlobal, sColourGlobalBack);
	cls(); // Set colours to whole screen, clear for gameplay
	SetCursorPosition(0, 0); // Ensure that the cursor position is at the top left of terminal

	// Set border colours for CarDodgeMain::UpdatePanelInfo()
	sBorderColourFore = sSetBorderColourFore;
	sBorderColourBack = sSetBorderColourBack;

	colour(sSetBorderColourFore, sSetBorderColourBack);

	// 1. Draw left side border
	for (short int i = 0; i <= nSessionConsoleHeight; i++) {
		// Repeat creation of sides with std::string buffer
		std::cout << std::string(nLeftBorderWidth - 1, ' ') << '\xb2';
		if (i < nSessionConsoleHeight) std::cout << '\n';
	}

	// 2. Draw right side border
	for (short int i = 0; i <= nSessionConsoleHeight; i++) {
		// Repeat creation of sides with std::string buffer
		SetCursorPosition(nScreenWidth - nRightBorderWidth, i); // 25 is width of information pane
		std::cout << '\xb2' << std::string(nRightBorderWidth - 1, ' ');
		if (i < nSessionConsoleHeight) std::cout << '\n';

	}

	colour(sColourGlobal, sColourGlobalBack);

	return;
}

// MoveUserCarLeft
void CarDodgeCore::MoveUserCarLeft() {
	// Check if user car is already at the left border
	if (UserCar.bottomLeft.X <= nLeftBorderWidth) {
		return;
	}

	EraseCar(UserCar);

	// Move car left by 2
	UserCar.bottomLeft.X -= 2;
	UserCar.bottomRight.X -= 2;

	RenderCar(UserCar);

	return;
}

// MoveUserCarRight
void CarDodgeCore::MoveUserCarRight() {
	// Check if user car is already at the right border
	if (UserCar.bottomRight.X >= nScreenWidth - nRightBorderWidth) {
		return;
	}

	EraseCar(UserCar);

	// Move car right by 2
	UserCar.bottomLeft.X += 2;
	UserCar.bottomRight.X += 2;

	RenderCar(UserCar);

	return;
}

// RenderNewEnemyCar
void CarDodgeCore::RenderNewEnemyCar() {
	static std::uniform_int_distribution<short int> Dist(nLeftBorderWidth, nScreenWidth - nRightBorderWidth - 4);

	// Too many rendered enemy cars at once
	if (nNumOfCurrentRenderedEnemyCars > 128) return;

	// Get next empty enemy car index now
	int nEnemyCarIndex = GetNextEmptyEnemyCarIndex();
	// Get random x co-ord for enemy car
	short int nRandomXCoord = Dist(RandEngine); // 4 because enemy cars are 4 chars in width

	// Set co-ords of this enemy car (the enemy car Y co-ordinate can be used as an identifier)
	EnemyCars[nEnemyCarIndex].bInUse = true;
	EnemyCars[nEnemyCarIndex].bottomLeft = { nRandomXCoord, 3 };
	nRandomXCoord += 4;
	EnemyCars[nEnemyCarIndex].bottomRight = { nRandomXCoord, 3 };

	// Render the new car now
	RenderCar(EnemyCars[nEnemyCarIndex]);

	// Finally, increment the number of currently rendered enemy cars
	nNumOfCurrentRenderedEnemyCars++;
}

// MoveAllEnemyCars
void CarDodgeCore::MoveAllEnemyCars() {
	for (int nEnemyCarIterator = 0; nEnemyCarIterator < nEnemyCarArraySize; nEnemyCarIterator++) {
		// Don't attempt to render if enemy car is not in use
		if (!EnemyCars[nEnemyCarIterator].bInUse) continue;

		EraseCar(EnemyCars[nEnemyCarIterator]);

		// Increment Y-values to make enemy cars appear 1 lower on screen
		EnemyCars[nEnemyCarIterator].bottomLeft.Y++;
		EnemyCars[nEnemyCarIterator].bottomRight.Y++;

		RenderCar(EnemyCars[nEnemyCarIterator]);
	}
}

// DeleteEnemyCarOnLowerBorderHit
bool CarDodgeCore::DeleteEnemyCarOnLowerBorderHit() {
	for (int nEnemyCarIterator = 0; nEnemyCarIterator < nEnemyCarArraySize; nEnemyCarIterator++) {
		// Don't attempt to delete if enemy car is not in use
		if (!EnemyCars[nEnemyCarIterator].bInUse) continue;

		// Check if found enemy car is touching lower border
		if (EnemyCars[nEnemyCarIterator].bottomLeft.Y >= nSessionConsoleHeight) {
			// Erase car
			EraseCar(EnemyCars[nEnemyCarIterator]);

			// Reset car to defaults
			EnemyCars[nEnemyCarIterator].bInUse = false;
			EnemyCars[nEnemyCarIterator].bottomLeft = { 0,0 };
			EnemyCars[nEnemyCarIterator].bottomRight = { 0,0 };
			nNumOfCurrentRenderedEnemyCars--;

			// Because of how cars are rendered one-by-one, we shouldn't expect another car touching the border at this moment, so exit
			return true;
		}
	}

	return false;
}

// CheckForCarCollision
bool CarDodgeCore::CheckForCarCollision() {
	// Reiterate through all enemy cars to check each one for crash
	for (int nEnemyCarIterator = 0; nEnemyCarIterator < nEnemyCarArraySize; nEnemyCarIterator++) {
		// Don't attempt to check if enemy car is not in use
		if (!EnemyCars[nEnemyCarIterator].bInUse) continue;

		// Check if car is in the necessary Y-coordinates to be in a crash
		if (EnemyCars[nEnemyCarIterator].bottomLeft.Y >= nSessionConsoleHeight - 3) {
			// Check if car is in the necessary X-coordinates to be in a crash
			if (EnemyCars[nEnemyCarIterator].bottomLeft.X < UserCar.bottomRight.X && EnemyCars[nEnemyCarIterator].bottomRight.X > UserCar.bottomLeft.X) {
				return true;
			}
		}
	}

	return false;
}

// CentreTextCarDodge
std::string CarDodgeCore::CentreTextCarDodge(std::string sText, size_t nCustomStringLength)
{
	// Set string length for this function call
	size_t nStringLength;
	if (nCustomStringLength < std::numeric_limits<size_t>::max()) {
		nStringLength = nCustomStringLength;
	}
	else {
		nStringLength = sText.length();
	}

	// This will centre text between nLeftBorderWidth and (nScreenWidth - nRightBorderWidth)
	const short int nAvailableScreenSpace = (nScreenWidth - nRightBorderWidth) - nLeftBorderWidth;

	// Check to prevent memory overspill
	if (nStringLength >= nAvailableScreenSpace) return sText;

	// Return centred text
	return std::string((nAvailableScreenSpace - nStringLength) / 2, ' ') + sText;
}

// UpdateHighScoreFromFile
bool CarDodgeCore::UpdateHighScoreFromFile() {
	// Open file
	std::ifstream HighScoreFileIn(sHighScoreFileName, std::ios::binary);

	// Check if failed
	if (HighScoreFileIn.fail()) {
		return false;
	}

	// Get length of file
	// Use seek to find file length in bytes to initialise buffer of that size later
	HighScoreFileIn.seekg(0, HighScoreFileIn.end);
	uint64_t nFileByteLength = HighScoreFileIn.tellg();
	HighScoreFileIn.seekg(0, HighScoreFileIn.beg); // move back to beginning

	// Copy all file data to buffer
	std::string sBuffer = std::string(nFileByteLength, '\0');

	// Retrieve data
	HighScoreFileIn.read(sBuffer.data(), nFileByteLength);
	HighScoreFileIn.close();

	// Decrypt data
	RyRyCryptor HighScoreDecryptor;
	std::string sDecryptedData = HighScoreDecryptor.DecryptString(sBuffer, nHighScoreKey1, nHighScoreKey2);

	// Check for errors in decryption
	if (HighScoreDecryptor.GetErrorLevel() > HighScoreDecryptor.N_SUCCESSFUL) {
		return false;
	}

	// Check decrypted data
	if (!isNumberull(sDecryptedData)) {
		return false;
	}

	// Set high score to decrypted number IF the retrieved score is more than the currently-set high score
	SetHighScore(std::stoull(sDecryptedData));

	// Exit
	return true;
}

// UpdateHighScoreInFile
bool CarDodgeCore::UpdateHighScoreInFile() {
	// Encrypt high score and put into string
	RyRyCryptor HighScoreEncryptor;
	std::string sEncryptedHighScore = HighScoreEncryptor.EncryptString(std::to_string(static_cast<long double>(nCurrentPointsHighScore)), nHighScoreKey1, nHighScoreKey2);

	// Open file from fresh in binary mode
	std::ofstream HighScoreFileOut(sHighScoreFileName, std::ios::binary);

	// Check if failed
	if (HighScoreFileOut.fail()) {
		return false;
	}

	// Write to new high score file
	HighScoreFileOut.write(sEncryptedHighScore.data(), sEncryptedHighScore.size());

	// Check if failed again
	if (HighScoreFileOut.fail()) {
		return false;
	}

	// Exit
	return true;
}
