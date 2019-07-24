#pragma once
#include <string>

using FString = std::string;
using int32 = int;

// TODO finish to package everything to distribute and asn for playtesting
// TODO find an online terminal to make things easier for people to play this (this might not be that easy...)
// TODO find the sweet spot to these word lengths -> play test (post in gamedev; tell some friends; search some online terminal)
// TODO posts in gamedev forum:
//			-lec. 52 (range-based for loop) : why does this implementation work? doesn't the computer 
//			 have too look up all key values until it finds a value or reaches the end of the map?
//			-lec ? : ascii art that i used
//			-lec ? : ask for playtesting; also help with playtesting



enum class EDifficultyStatus {
	OK,
	Not_Number,
	Out_of_Bounds,
};

struct FWordLengthRange
{
	int32 MyWLMax = 0;
	int32 MyWLMin = 0;
};

enum class EGuessStatus
{
	OK,
	Not_Isogram,
	Forbidden_Characters,
	Wrong_Size
};

struct FBullCowCount
{
	int32 MyBulls = 0;
	int32 MyCows = 0;
};

class FBullCowGame
{
public:
	FBullCowGame(); // constructor

	int32 GetCurrentTry() const;
	int32 GetWordLength() const;
	int32 GetMaxTries() const;

	FWordLengthRange GetWordLengthRange() const;
	EDifficultyStatus CheckDifficultyValidity(FString) const;
	EGuessStatus CheckGuessValidity(FString) const;
	bool IsIsogram(FString) const;
	bool IsLowercase(FString)const;
	bool HasForbidChar(FString) const;
	bool IsGameWon() const;	

	void Constructor();
	void Set(FString);
	void IncreaseCurrentTry();
	FBullCowCount CountBullsCows(FString);
	

private:
	// initializations in constructor
	FString MyHiddenWord;
	int32 MyCurrentTry;
	FString MyGuess;
	bool MyGuessValidity;
	FBullCowCount MyBCCount;
};