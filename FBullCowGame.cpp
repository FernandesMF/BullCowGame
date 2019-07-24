#include "FBullCowGame.h"
#include <map>
#include <string>

#define TMap std::map

using FString = std::string;
using int32 = int;


TMap<FString, FString> WordLengthToHiddenWord{
		{"3","dog"},
		{"4","bike"},
		{"5","quite"},
		{"6","borrow"},
		{"7","pikachu"}
};

FBullCowGame::FBullCowGame() {	Constructor(); }		// default constructor

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetWordLength() const { return MyHiddenWord.length(); }

// sets dummy values for first initialization of the game instance
void FBullCowGame::Constructor()
{
	MyHiddenWord = "";
	MyCurrentTry = 0;
	MyGuess = "";
	MyGuessValidity = false;
	MyBCCount.MyBulls = 0;
	MyBCCount.MyCows = 0;
	return;
}


FWordLengthRange FBullCowGame::GetWordLengthRange() const
{
	// HACK access actual values in WordLengthToHiddenWord in the future, instead of hard coding them here
	FWordLengthRange MyWLRange;
	MyWLRange.MyWLMin = 3;
	MyWLRange.MyWLMax = 7;
	return MyWLRange;
}

//TODO make empty Difficulty return Not_Number
// checks if a difficulty request is valid
EDifficultyStatus FBullCowGame::CheckDifficultyValidity(FString Difficulty) const
{
	std::size_t FoundForbidden = Difficulty.find_first_not_of("1234567890");
	if (FoundForbidden != std::string::npos) {					
		return EDifficultyStatus::Not_Number;		// if Difficulty has a non - number character, returns Not_number	
	}
	if (WordLengthToHiddenWord[Difficulty].length() == 0) {										
		return EDifficultyStatus::Out_of_Bounds;		// if it is not a number within the range, returns Out_of_Bounds
	}
	return EDifficultyStatus::OK;						// otherwise, it is valid
}

// receives a VALID ("3"-"7" string) difficulty, sets the game hidden word and the current try to 1
void FBullCowGame::Set(FString Difficulty)
{
	MyCurrentTry = 1;
	MyHiddenWord = WordLengthToHiddenWord[Difficulty];
	return;
}

int32 FBullCowGame::GetMaxTries() const
{
	// maximum number of tries are returned from this hard-coded map
	std::map<int, int> WordLengthToMaxTries{ {3, 10}, {4, 15}, {5, 20}, {6, 25}, {7, 30} };
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

// checks if Guess is lower case
bool FBullCowGame::IsLowercase(FString Guess) const
{
	for (auto Letter : Guess) {
		if (!islower(Letter)) {
			return false;
		}
	}
	return true;
}

// checks if the guess has the correct number of letters, if it has forbidden characters and then if it is an isogram
EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (Guess.length() != GetWordLength())
	{	// if input has wrong size, return Wrong_Size
		return EGuessStatus::Wrong_Size;
	} 
	else if (HasForbidChar(Guess))
	{	// if input has a forbidden character, return Forbidden_Character
		return EGuessStatus::Forbidden_Characters;
	}
	else if (!IsIsogram(Guess))
	{	// if input is not isogram, return Not_Isogram
		return EGuessStatus::Not_Isogram;
	}
	else 
	{	// otherwise, return OK
		return EGuessStatus::OK;
	}
}

// checks if Guess is an isogram by hash table of already-seen letters (efficient)
bool FBullCowGame::IsIsogram(FString Guess) const
{	
	// strings of length 0 and 1 should be considered as isograms
	// strings with non-repeating characters are considered isograms (even forbid. chars.)
	TMap<char, bool> LetterSeen;			// initialize hash table to empty
	for (auto Letter : Guess) {				// iterate through characters in guess
		if (LetterSeen[Letter]) {
			return false;					// if current character already has a mapped value, (it is not an isogram)
		}
		else {
			LetterSeen[Letter] = true;		// otherwise add a mapped value of 1
		}
	}
	// if no problems were found after looping through all the guess, return true (it is an isogram!)
	return true;
}

// checks if Guess has characters that are not lowercase letters
bool FBullCowGame::HasForbidChar(FString Guess) const
{
	std::size_t found = Guess.find_first_not_of("abcdefghijklmnopqrstuvwxyz");
	if (found != std::string::npos)
	{
		return true;
	}
	else {
		return false;
	}
}

void FBullCowGame::IncreaseCurrentTry()
{
	MyCurrentTry++;
	return;
}

// receives a VALID guess and counts how many Bulls and Cows were scored
FBullCowCount FBullCowGame::CountBullsCows(FString Guess)
{
	MyBCCount.MyBulls = 0;
	MyBCCount.MyCows = 0;
	// loop through the guess characters
	for(int32 GChar = 0; GChar != GetWordLength(); GChar++) {
		// loop through the HiddenWord characters
		for (int32 MHWChar = 0; MHWChar != GetWordLength(); MHWChar++) {
			// check if the characters match
			if (Guess[GChar] == MyHiddenWord[MHWChar]) {
				// if they are in the same position
				if (GChar == MHWChar) {
					// increase bulls
					MyBCCount.MyBulls++;
				}
				// else
				else {				
					// increase cows
					MyBCCount.MyCows++;
				}
			}
		}
	}
	return MyBCCount;
}

// game is won when all input characters are bulls
bool FBullCowGame::IsGameWon() const
{
	if (MyBCCount.MyBulls == GetWordLength()) {
		return true;
	} else {
		return false;
	}
	
}

/* old IsIsogram method (direct comparison)
bool FBullCowGame::IsIsogram(FString Guess) const
{
	// loops though guess characters
	for (int32 i = 0; i != GetWordLength() - 1; i++) {
		// loops through guess characters again
		for (int32 j = i + 1; j != GetWordLength(); j++) {
			// if the two characters are equal, return false
			if (Guess[i] == Guess[j]) {
				return false;
			}
		}
	}
	// if no coincidences were found, return true
	return true;

}
*/
