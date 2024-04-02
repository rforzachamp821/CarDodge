//
// CarInfo.cpp - Houses the CarInfo structure.
//

#include <array>
#include <Windows.h>

// CarInfo - Structure that defines the attributes a car should have.
struct CarInfo {
	
	// The following define the bottom left and right of the car position.
	COORD bottomLeft = { 0,0 };
	COORD bottomRight = { 0,0 };

	// Describes if car is in use - by default, it's not
	bool bInUse = false;

	// CarStyle - Defines the design of the car.
	std::array<std::array<char, 4>, 4> CarStyle{};
};

// ResetCarInfoObject - Resets CarInfo object to default values.
// Parameters: ObjToReset - The object to reset (ideally should be an address to an existing object).
// Return value: None
//
void ResetCarInfoObject(CarInfo* ObjToReset) {

	// Reset all values in ObjToReset
	ObjToReset->bottomLeft = { 0,0 };
	ObjToReset->bottomRight = { 0,0 };
	ObjToReset->bInUse = false;
	ObjToReset->CarStyle.fill({});

	return;
}

// CarStyles - structure containing the different styles that a car can have, with names.
struct CarStyles {
	const std::array<std::array<char, 4>, 4> EnemyCar = { '\xb2', 'T', 'T', '\xb2', '|', '0', '0', '|', '\xb2', '0', '0', '\xb2', ']', '|', '|', '[' };
	const std::array<std::array<char, 4>, 4> UserCarDefault = { '=','|','|','=','0','=','=','0','|',':',':','|','0','=','=','0' };
	const std::array<std::array<char, 4>, 4> HoverRocket = { '/','T','T','\\','|','_','_','|','|',' ',' ','|','/','\\','/','\\' };
	const std::array<std::array<char, 4>, 4> TheSweeper = { '\\','=','=','/','0','=','=','0','|','X','X','|','0','=','=','0' };
	const std::array<std::array<char, 4>, 4> TheSlicer = { ' ', '|', '|', ' ', '0', '=', '=', '0', '|', 'X', 'X', '|', '0', '_', '_', '0' };
	const std::array<std::array<char, 4>, 4> GTSpeed = { '/','T','T','\\','0','|','|','0','|',':',':','|','0','/','\\','0' };
	const std::array<std::array<char, 4>, 4> XtraAero = { '=','/','\\','=','0','=','=','0',' ','(',')',' ','0','/','\\','0' };
};