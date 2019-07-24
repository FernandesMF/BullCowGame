#include <iostream>
#include <string>
#include "FBullCowGame.h"

using FString = std::string;
using int32 = int;

void PrintIntro();
void SetGame();
void PrintStartMessage();
void PlayGame();
void PrintBCCount(FBullCowCount);
void PrintSummary();
FString GetGuess();
FString LowerCaseGuess(FString);
void DifficultyErrorMessage(EDifficultyStatus);
void GuessErrorMessage(FString);
bool AskToPlayAgain();

FBullCowGame BCGame;						// instantiate a new game, globally 



// top-level function for the Bulls and Cows game
int main()
{
	do{
		PrintIntro();
		SetGame();
		PrintStartMessage();
		PlayGame();
		PrintSummary();
	}
	while(AskToPlayAgain());
	
	std::cout << std::endl;
	return 0;								// exit the application
}



// introduces the game
void PrintIntro()
{
	std::cout << "\n\n\n";
	std::cout << "Art thanks to: https://asciiart.website/index.php?art=animals/cows\n\n\n\n";
	std::cout << "   ,/         \\,         /             \\\n";
	std::cout << "  ((__,-\"\"\"-,__))       ((__-^^-,-^^-__))\n";
	std::cout << "   `--)~   ~(--`         `-_---' `---_-'\n";
	std::cout << "  .-'(       )`-,         <__|o` 'o|__>\n";
	std::cout << "  `~~`d\\   /b`~~`            \\  `  /\n";
	std::cout << "      |     |                 ): :(\n";
	std::cout << "      (6___6)                 :o_o:\n";
	std::cout << "       `---`                  \" - \"\n\n\n";	
	std::cout << "Welcome to Bulls and Cows, a fun word game!\n\n";
	std::cout << "In this game, you have to guess the isogram that I will be thinking of.\n";
	std::cout << "(An isogram is a word without any repeatiing letters.)\n";
	return;
}

// asks how many letters the player wants
void SetGame()
{
	FWordLengthRange WLRange = BCGame.GetWordLengthRange();
	FString Difficulty = "";
	EDifficultyStatus DStatus = EDifficultyStatus::Not_Number;
	std::cout << "\nLet's set the difficulty.";
	while(DStatus!=EDifficultyStatus::OK){
		std::cout << " How many letters would you like the isogram to have? (";
		std::cout << WLRange.MyWLMin << "-" << WLRange.MyWLMax << "):";
		getline(std::cin, Difficulty);								// gets input from the player
		DStatus = BCGame.CheckDifficultyValidity(Difficulty);		// checks if it is valid
		if (DStatus != EDifficultyStatus::OK) { DifficultyErrorMessage(DStatus); };		// if not, prints error message
	};
	BCGame.Set(Difficulty);											// set hidden word in BCGame
	return;
}

// prints an error message if difficulty request is not valid
void DifficultyErrorMessage(EDifficultyStatus DStatus)
{
	std::cout << std::endl << "This is not a valid amount of letters: ";
	switch (DStatus) {
	case EDifficultyStatus::Not_Number:
		std::cout << "it is not a number.";
		break;
	case EDifficultyStatus::Out_of_Bounds:
		FWordLengthRange WLRange = BCGame.GetWordLengthRange();
		std::cout << "it is out of bounds (";
		std::cout << WLRange.MyWLMin << "-" << WLRange.MyWLMax << ").";
		break;
	}
	std::cout << " Let's try again.";
	return;
}

// confirms how many letters to use, informs how many max. tries, and invites to start
void PrintStartMessage()
{
	std::cout << "\nOkay, I thought of a " << BCGame.GetWordLength() << " letter word!\n";
	std::cout << "Can you guess it within " << BCGame.GetMaxTries() << " tries?\n";
	return;
}

// iterates the number of turns, asking guesses and processing them
void PlayGame()
{
	while (BCGame.GetCurrentTry() <= BCGame.GetMaxTries()) {
		FString Guess = GetGuess();										// get guess from user
		if (!BCGame.IsLowercase(Guess)) { Guess = LowerCaseGuess(Guess); }		// convert guess to lowercase, if necessary
		std::cout << "Your guess was: " << Guess;
		
		
		if (BCGame.CheckGuessValidity(Guess)==EGuessStatus::OK)			// check if guess is valid
		{
			BCGame.IncreaseCurrentTry();								// if so, increase try number...
			FBullCowCount BCCount = BCGame.CountBullsCows(Guess);		// ... print bulls and cows number, ...
			PrintBCCount(BCCount);
			if (BCGame.IsGameWon()) {									// and check if game is won
				return;
			}
		}
		else {															// otherwise warns user what was wrong
			GuessErrorMessage(Guess);
		}
	}
	return;
}

// gets a guess from the player and turns it to lowercase
FString GetGuess()
{
	FString Guess="";
	std::cout << std::endl << std::endl << "Try " << BCGame.GetCurrentTry() << " of ";
	std::cout << BCGame.GetMaxTries() << ". Enter your guess: ";
	getline(std::cin, Guess);
	return Guess;
}

FString LowerCaseGuess(FString Guess)
{
	for (int32 i = 0; i != Guess.length(); i++)		// makes each char in Guess lowercase
	{
		Guess[i] = std::tolower(Guess[i]);
	}
	return Guess;
}

void GuessErrorMessage(FString Guess)
{
	std::cout << std::endl << "This guess is not valid: ";
	switch (BCGame.CheckGuessValidity(Guess)) {
	case EGuessStatus::Forbidden_Characters:
		std::cout << "it has forbidden characters.";
		break;
	case EGuessStatus::Not_Isogram:
		std::cout << "it has repeated letters.";
		break;
	case EGuessStatus::Wrong_Size:
		std::cout << "it has wrong size (should be " << BCGame.GetWordLength() << " letters).";
		break;
	}
	std::cout << " Let's try again.";
}

// prints bulls and cows counts
void PrintBCCount(FBullCowCount BCCount)
{
	std::cout << std::endl << "\tBulls: " << BCCount.MyBulls;
	std::cout << std::endl << "\tCows: " << BCCount.MyCows;
}

// prints final message of the game, either for victory or failure
void PrintSummary()
{
	if (BCGame.IsGameWon()) {
		std::cout << std::endl;
		std::cout << "                            ..---.\n";
		std::cout << "                           // |\\||\\\n";
		std::cout << "    \\   /                 ///;||\\|;\\               \\   /\n";
		std::cout << "      o               .__// \\\\____//\\_,.             o\n";
		std::cout << "    /   \\              Y{_\\_/  \\'' = __/           /   \\\n";
		std::cout << "                       \\___   (o) (o)  }       /\n";
		std::cout << "                          /         :--'   SACRE MOO!\n";
		std::cout << "                     .---/ \\_    `__\\-.\n";
		std::cout << "                    /     `--\\___(o'o) \\\n";
		std::cout << "             \\   / |     \\      `===='  |\\   /\n";
		std::cout << "               o    `.    `.    .'    .'   o\n";
		std::cout << "             /   \\   (`.    `. '    .')  /   \\\n";
		std::cout << "                     (  `.    `...'   )\n";
		std::cout << "                     (   .+.    `-.   )\n";
		std::cout << "                     (.-'  .>-._   `-.)\n";
		std::cout << "    \\   /           (___.-'      `-.___)            \\   /\n";
		std::cout << "      o              (                )               o\n";
		std::cout << "    /   \\            (                )             /   \\\n";
		std::cout << std::endl << "Congratulations, you guessed it!!";
	}
	else {
		std::cout << std::endl << "Your tries are up... better luck next time!";
	}
}

// asks if player wants to play again and gets response
bool AskToPlayAgain()
{
	std::cout << std::endl <<  "Would you like to play again? (y/n)";
	FString Answer = "";
	getline(std::cin, Answer);
	return (Answer[0] == 'y' || Answer[0] == 'Y');
}