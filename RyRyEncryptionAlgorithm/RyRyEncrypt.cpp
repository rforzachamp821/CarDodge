//
// RyRyEncrypt.cpp - Contains the implementation of the RyRyEncrypt class.
//
#include <random>
#include <string>
#include <fstream>

#include "RyRyEncrypt.h" // Interface

// Protected
//
// EncryptStringUsingRyryAlgorithm
bool RyRyEncrypt::EncryptStringUsingRyryAlgorithm(std::string* sStringToEncrypt, RYRYKEY nKey1, RYRYKEY nKey2)
{
	// Mersenne twister engines
	std::mt19937_64 keyRandomiser1;
	std::mt19937_64 keyRandomiser2;

	// 1. Seed the engines with keys
	keyRandomiser1.seed(nKey1 ^ nKey2);
	keyRandomiser2.seed(nKey2 & nKey1);
	const uint64_t nSecretFinalKey = (nKey1 ^ nKey2) ^ (nKey2 & nKey1);

	// Append decryption verification string to check for correct keys inputted when decrypting
	sStringToEncrypt->reserve(sStringToEncrypt->length() + nDecryptionVerifierStringSize);
	sStringToEncrypt->append(std::string(czDecryptionVerifierString));

	// For the for loop and the append process
	const size_t nStringLengthWithoutAppend = sStringToEncrypt->length();

	// Append encoding verifier string to the rear of the string to indicate that this is a RyRy-encrypted string
	sStringToEncrypt->reserve(nStringLengthWithoutAppend + nEncodingVerifierStringSize);
	sStringToEncrypt->append(std::string(czEncodingVerifierString));

	// Loop until whole string has been encrypted
	// nStringLengthWithoutAppend to avoid the encoding verifier string
	for (uint64_t i = 0; i < nStringLengthWithoutAppend; i++) {
		//
		// 2. Work on first engine
		//
		// a. Create uniform_int_distribution for first engine
		std::uniform_int_distribution<uint64_t> DistForRandomiser1(0, (std::numeric_limits<uint64_t>::max)());

		// b. Generate random number from first engine
		const uint64_t nGeneratedNumberFromRandomiser1 = DistForRandomiser1(keyRandomiser1);

		//
		// 3. Work on second engine
		//
		// a. Create uniform_int_distribution for second engine
		std::uniform_int_distribution<uint64_t> DistForRandomiser2(0, nGeneratedNumberFromRandomiser1);

		// b. Generate random number from second engine
		const uint64_t nGeneratedNumberFromRandomiser2 = DistForRandomiser2(keyRandomiser2);

		// 4. Work out final generated random number by performing XOR between both generated numbers
		const uint64_t nFinalGeneratedNumber = nGeneratedNumberFromRandomiser1 ^ nGeneratedNumberFromRandomiser2;

		// 5. Declare final increment variable, work out final increment number by:
		//    1. doing XOR between 2 keys
		//    2. doing inclusive OR between answer 1 and nFinalGeneratedNumber
		const uint64_t nFinalIncrementNumber = nSecretFinalKey ^ nFinalGeneratedNumber;

		// 6. Increment next character to encrypt with final increment number
		const unsigned char nEncryptedChar = (*sStringToEncrypt)[i] + nFinalIncrementNumber;

		// 7. Append the encryped character to the encrypted string - this round is done
		// sEncryptedText.push_back(nEncryptedChar);
		(*sStringToEncrypt)[i] = nEncryptedChar;
	}

	nErrorLevel = N_SUCCESSFUL;
	return true;
}
//
// DecryptStringUsingRyryAlgorithm
bool RyRyEncrypt::DecryptStringUsingRyryAlgorithm(std::string* sStringToDecrypt, RYRYKEY nKey1, RYRYKEY nKey2)
{
	// Mersenne twister engines
	std::mt19937_64 keyRandomiser1;
	std::mt19937_64 keyRandomiser2;

	// 1. Seed the engines with keys
	keyRandomiser1.seed(nKey1 ^ nKey2);
	keyRandomiser2.seed(nKey2 & nKey1);
	const uint64_t nSecretFinalKey = (nKey1 ^ nKey2) ^ (nKey2 & nKey1);

	// Check if encoding verifier string is there; if it is, decrypt. If not, exit as this decryptor can't be used
	if (sStringToDecrypt->length() >= nEncodingVerifierStringSize && 
		sStringToDecrypt->substr(sStringToDecrypt->length() - nEncodingVerifierStringSize, nEncodingVerifierStringSize) == std::string(czEncodingVerifierString)
		) {
		sStringToDecrypt->erase(sStringToDecrypt->length() - nEncodingVerifierStringSize, nEncodingVerifierStringSize);
	}
	else {
		nErrorLevel = N_DATA_INCORRECT_ENCODING; // Not correctly encoded for decryption
		return false;
	}

	// Loop until whole string has been decrypted
	uint64_t nStringLength = sStringToDecrypt->length();
	for (uint64_t i = 0; i < nStringLength; i++) {
		//
		// 2. Work on first engine
		//
		// a. Create uniform_int_distribution for first engine
		std::uniform_int_distribution<uint64_t> DistForRandomiser1(0, (std::numeric_limits<uint64_t>::max)());

		// b. Generate random number from first engine
		const uint64_t nGeneratedNumberFromRandomiser1 = DistForRandomiser1(keyRandomiser1);

		//
		// 3. Work on second engine
		//
		// a. Create uniform_int_distribution for second engine
		std::uniform_int_distribution<uint64_t> DistForRandomiser2(0, nGeneratedNumberFromRandomiser1);

		// b. Generate random number from second engine
		const uint64_t nGeneratedNumberFromRandomiser2 = DistForRandomiser2(keyRandomiser2);

		// 4. Work out final generated random number by performing XOR between both generated numbers
		const uint64_t nFinalGeneratedNumber = nGeneratedNumberFromRandomiser1 ^ nGeneratedNumberFromRandomiser2;

		// 5. Declare final increment variable, work out final increment number by
		//    1. doing XOR between 2 keys
		//    2. doing XOR between answer 1 and nFinalGeneratedNumber
		const uint64_t nFinalIncrementNumber = nSecretFinalKey ^ nFinalGeneratedNumber;

		// 6. Decrement next character to decrypt with supposed final increment number
		const unsigned char nDecryptedChar = (*sStringToDecrypt)[i] - nFinalIncrementNumber;

		// 7. Append the decrypted character to the decrypted string - this round is done
		(*sStringToDecrypt)[i] = nDecryptedChar;
	}

	// Check if correct keys were used to decrypt; the secret decryption verification string should have been decrypted correctly if the right keys were used
	if (sStringToDecrypt->length() >= nDecryptionVerifierStringSize) {
		if (sStringToDecrypt->substr(sStringToDecrypt->length() - nDecryptionVerifierStringSize, nDecryptionVerifierStringSize) == std::string(czDecryptionVerifierString)) {
			// Remove the decryption verification string so it doesn't interfere with the file
			sStringToDecrypt->erase(sStringToDecrypt->length() - nDecryptionVerifierStringSize, nDecryptionVerifierStringSize); 
		}
		else {
			// Incorrect keys
			nErrorLevel = N_INCORRECT_KEYS;
			return false;
		}
	}
	else {
		// The lack of the decryption verification string means that the data wasn't encrypted properly - error
		nErrorLevel = N_DATA_INCORRECT_ENCODING;
		return false;
	}

	nErrorLevel = N_SUCCESSFUL;
	return true;
}

// Public
//
// RyRyEncrypt constuctor
RyRyEncrypt::RyRyEncrypt() {
	// Ensure all necessary variables are set
	nErrorLevel = N_SUCCESSFUL;
	
	return;
}
//
// GetErrorLevelInfo
std::string RyRyEncrypt::GetErrorLevelInfo() {
	// Use switch-case statement for error level info
	switch (nErrorLevel) {
	case N_SUCCESSFUL:
		return "Success - no error has occured.";
	case N_FAILED_OPENFILE_READING:
		return "Fail - Failed to open file for reading. Possibly lack of permissions, nonexistent file, out of memory, etc.";
	case N_FAILED_OPENFILE_WRITING:
		return "Fail - The file failed to open for writing. Possibly lack of permissions, lack of storage space, out of memory, etc.";
	case N_DATA_INCORRECT_ENCODING:
		return "Fail - The data provided does not have the correct encoding to decrypt using the RyRy Decryptor.";
	case N_FAILED_READING:
		return "Fail - Failed to read data from file. Possibly lack of permissions, nonexistent file, out of memory, etc.";
	case N_FAILED_WRITING:
		return "Fail - Failed to write data to file. Possibly lack of permissions, lack of storage space, out of memory, etc.";
	case N_INCORRECT_KEYS:
		return "Fail - The keys provided for decryption are incorrect. Source the right keys and try again.";
	case N_UNKNOWN_ERROR:
		return "Fail - An unknown error has occured.";
	default:
		return "Failed to retrieve error information. nErrorLevel contains invalid error code.";
	}
}
//
// EncryptString
std::string RyRyEncrypt::EncryptString(std::string sStringToEncrypt, const RYRYKEY nKey1, const RYRYKEY nKey2) {
	// Encrypt string and return
	EncryptStringUsingRyryAlgorithm(&sStringToEncrypt, nKey1, nKey2);
	return sStringToEncrypt;
}
//
// DecryptString
std::string RyRyEncrypt::DecryptString(std::string sStringToDecrypt, const RYRYKEY nKey1, const RYRYKEY nKey2) {
	// Decrypt string and return
	DecryptStringUsingRyryAlgorithm(&sStringToDecrypt, nKey1, nKey2);
	return sStringToDecrypt;
}
//
// EncryptFile
bool RyRyEncrypt::EncryptFile(const std::string sFilePath, const RYRYKEY nKey1, const RYRYKEY nKey2)
{
	// All variables
	uint64_t nFileByteLength = 0;
	std::string sBuffer = "";

	// Open file for reading
	std::ifstream FileIn(sFilePath, std::ios::binary);

	// Check for read errors
	if (FileIn.fail()) {
		nErrorLevel = N_FAILED_OPENFILE_READING;
		return false;
	}

	// Now use seek to find file length in bytes to initialise buffer of that size later
	FileIn.seekg(0, FileIn.end);
	nFileByteLength = FileIn.tellg();
	FileIn.seekg(0, FileIn.beg); // move back to beginning

	// Copy all file data to buffer
	sBuffer = std::string(nFileByteLength, '\0');
	FileIn.read(sBuffer.data(), nFileByteLength);

	// Check for read errors
	if (FileIn.fail()) {
		nErrorLevel = N_FAILED_READING;
		return false;
	}

	FileIn.close();

	// Now that data has been retrieved, encrypt it
	EncryptStringUsingRyryAlgorithm(&sBuffer, nKey1, nKey2);

	// Check for errors in encryption
	if (nErrorLevel > N_SUCCESSFUL) {
		return false; // There is no need to change nErrorLevel, as it already describes what happened.
	}

	// Open an output stream of file to send encrypted data to file
	std::ofstream FileOut(sFilePath, std::ios::binary | std::ios::trunc);

	// Check for write errors
	if (FileOut.fail()) {
		nErrorLevel = N_FAILED_OPENFILE_WRITING;
		return false;
	}

	// Write encrypted data to file
	FileOut.write(sBuffer.data(), static_cast<uint64_t>(sBuffer.size()));

	// Check for write errors when writing to file
	if (FileOut.fail()) {
		nErrorLevel = N_FAILED_WRITING;
		return false;
	}

	FileOut.close();
	nErrorLevel = N_SUCCESSFUL;
	return true;
}
//
// DecryptFile
bool RyRyEncrypt::DecryptFile(const std::string sFilePath, const RYRYKEY nKey1, const RYRYKEY nKey2)
{
	// All variables
	std::streamsize nFileByteLength = 0;
	std::string sBuffer = "";

	// Open file for reading
	std::ifstream FileIn(sFilePath, std::ios::binary);

	// Check for read errors
	if (FileIn.fail()) {
		nErrorLevel = N_FAILED_OPENFILE_READING;
		return false;
	}

	// Now use seek to find file length in bytes to initialise buffer of that size later
	FileIn.seekg(0, FileIn.end);
	nFileByteLength = FileIn.tellg();
	FileIn.seekg(0, FileIn.beg); // move back to beginning

	// Copy all file data to buffer
	sBuffer = std::string(nFileByteLength, '\0');
	FileIn.read(sBuffer.data(), nFileByteLength);

	if (FileIn.fail()) {
		nErrorLevel = N_FAILED_READING;
		return false;
	}

	FileIn.close();

	// Now that data has been retrieved, decrypt it using passed keys
	DecryptStringUsingRyryAlgorithm(&sBuffer, nKey1, nKey2);

	// Check for errors in decryption
	if (nErrorLevel > N_SUCCESSFUL) {
		return false; // There is no need to change nErrorLevel, as it already describes the error that occured.
	}

	// Open an output stream of file to send decrypted data to file
	std::ofstream FileOut(sFilePath, std::ios::binary | std::ios::trunc);

	// Check for write errors
	if (FileOut.fail()) {
		nErrorLevel = N_FAILED_OPENFILE_WRITING;
		return false;
	}

	// Write decrypted data to file
	FileOut.write(sBuffer.data(), static_cast<uint64_t>(sBuffer.size()));

	// Check for write errors after decryption
	if (FileOut.fail()) {
		nErrorLevel = N_FAILED_WRITING;
		return false;
	}

	FileOut.close();
	nErrorLevel = N_SUCCESSFUL;
	return true;
}