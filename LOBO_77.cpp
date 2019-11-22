// Coded with Visual Studio 2017 in C++.
#include <fstream>  // "fstream" is used for reading and editing files.
#include <string>  // "string" is used for saving texts and words.
#include <vector>  // "vector" is used for saving strings of data.
#include <iostream>  // "iostream" is used for the visual in- and output of the program.
#include <algorithm>  // "algorithm" is used for mixing a vector and getting random numbers.
#include <conio.h>  // "conio.h" is used for the "_getch()" function.
#include <Windows.h>  // "Windows.h" is used for playing sounds.
#include <mmsystem.h>  // "mmsystem.h" is used for playing sounds.
// Sounds from AUDIYOU

namespace LOBO_77
{
	#define English 0  // Assigning numbers to the languages
	#define German 1  // for the compiler.

	#define Play 0  // Assigning
	#define Manual 1 // numbers 
	#define Settings 2  // to the
	#define Credits 3  // menu options
	#define Close 4  // for the compiler.

	#define upKey (unsigned char)72
	#define leftKey (unsigned char)75
	#define rightKey (unsigned char)77
	#define downKey (unsigned char)80
	#define returnKey (unsigned char)13
	#define tabulatorKey (char)'\t'

	#define changeDirection (signed char)1	// Assigning numbers to the special cards
	#define x2 (signed char)20  // for the compiler.

	#define outGame (signed char)-1  // Assigning the number saved in the players variable "chipsAmount", when he´s already out of game.

	#define ü (unsigned char)129  // 129 is the value of a char for a "ü".
	#define ä (unsigned char)132  // 132 is the value of a char for a "ä".

	unsigned char language = English;  // The variable "language" is saving the current in-game used language.

	std::string sloganText, playText, manualText, settingsText, creditsText, closeText, backToMenuText, explanationText, developersText, roundSizeText, chooseNameText, tooLongInputText, refreshedHiddenDeckText, valueText, winnerText;  // In this strings all the output texts are saved in the current selected language.

	std::vector<signed char> hiddenDeck;  // The vector "hiddenDeck" contains all the unused cards.

	std::vector<signed char> openDeck;  // The vector "openDeck" contains all the used cards.

	signed short value = 0;  // The variable "value" saves the total value of all used cards.
	
	bool direction = 1;  // The variable "direction" saves, in which order the players are going to make their moves (1 = clockwise).

	unsigned char activePlayers;  // The variable "activePlayers" saves, how much players are still in the game.

	void printCardValue(const signed char &value)  // This prints the value of the given Card.
	{
		if (value == changeDirection)
		{
			std::printf("<->");
		}
		else if (value == x2)
		{
			std::printf(" x2");
		}
		else
		{
			if (value >= 10)
			{
				std::printf(" ");
				std::printf("%d", value);
			}
			else if (value >= 0)
			{
				std::printf(" %d ", value);
			}
			else
			{
				std::printf("-10");
			}
		}
		return;
	}

	bool takeTwo = false;  // The variable "takeTwo" saves, if the player has to take a second card.

	class Player final  // The class "Player" saves all the information of a player.
	{
	private:  // This variable are only directly accessible from inside the class.
		std::string name;  // "name" saves the displayed name of the player.
		signed char chipAmount = 3;  // "chipAmount" saves, how much chips the player has left (-1 means, the player is out of game).
		std::vector<signed char> deck;  // The vector "deck" contains the players cards (max. 5).
		unsigned char sizeDeck;  // This saves the size of the vector "deck".

	public:  // This functions are also directly accessible from outside the class.
		Player(bool randomName)  // This is the constructor of the class "Player".
		{
			if (randomName)  // If this is an AI.
			{
				std::vector<std::string> nameList = { "1", "2", "3" };  // A list of possible names is getting created.
				std::random_shuffle(nameList.begin(), nameList.end());  // The order of the elements of the vector "nameList" is getting randomized.
				name = nameList[0];  // The string "name" is getting filled with the first element of the vector "nameList".
			}
			else  // If this is an actual player.
			{
				std::cin >> name;  // A user text input is taken and saven in the string "name".
				while (name.size() > 32)  // The length of the the string "name" is getting checked.
				{
					std::system("CLS");  // The console is getting cleared.
					std::printf("\n %s\n  ", tooLongInputText.c_str());  // A warning is getting printed.
					std::cin >> name;  // A user text input is taken and saven in the string "name".
					continue;  // The loop is getting continued;
				}
			}
			return;  // The function is getting ended.
		}

		void getCard(signed char value)  // This adds a card to the players deck.
		{
			deck.push_back(value);  // Adding a card with the given value to the players deck.
			sizeDeck++;  // The variable "sizeDeck" is increased by one.
			return;  // The function is getting ended.
		}

		signed char giveCard(unsigned char number)  // This takes a card from the players deck.
		{
			unsigned char choosenCard = deck[number];  // Getting the value of the choosen card.
			deck.erase(deck.begin() + number);  // Erasing the choosen card from the players deck.
			deck.shrink_to_fit();  // Making sure, that there´s no free space in the vector "deck".
			sizeDeck--;
			return choosenCard;  // The function is getting ended and the value of the choosen card transferred.
		}

		void resetDeck(void)  // This completely clears the players deck.
		{
			deck.erase(deck.begin(), deck.end());  // The content of the vector "deck" is getting completely erased.
			deck.shrink_to_fit();  // Making sure, that there´s no free space in the vector "deck".
			return;  // The function is getting ended.
		}

		void displayDeck(void)  // This prints the cards of the player.
		{
			if (openDeck.size() > 0)
			{
				std::printf("\n   -----    -----\n   |   |    |   |\n   |???|    |");
				printCardValue(openDeck.back());
				std::printf("|\n   |   |    |   |\n   ----- ");
			}
			else
			{
				std::printf("\n   -----\n   |   |\n   |???|\n   |   |\n");
			}
			std::printf("   -----\n\n\n");

			switch (deck.size())
			{
			case 5:
				std::printf(" ----- ----- ----- ----- -----\n |   | |   | |   | |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n |");
				printCardValue(deck[0]);
				std::printf("| |");
				printCardValue(deck[1]);
				std::printf("| |");
				printCardValue(deck[2]);
				std::printf("| |");
				printCardValue(deck[3]);
				std::printf("| |");
				printCardValue(deck[4]);
				std::printf("|");
				if (chipAmount == 3 || chipAmount == 1)
				{
					std::printf("   #");
				}
				std::printf("\n |   | |   | |   | |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n ----- ----- ----- ----- -----");
				break;  // The switch is getting left.
			case 4:
				std::printf(" ----- ----- ----- -----\n |   | |   | |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n |");
				printCardValue(deck[0]);
				std::printf("| |");
				printCardValue(deck[1]);
				std::printf("| |");
				printCardValue(deck[2]);
				std::printf("| |");
				printCardValue(deck[3]);
				std::printf("|");
				if (chipAmount == 3 || chipAmount == 1)
				{
					std::printf("   #");
				}
				std::printf("\n |   | |   | |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n ----- ----- ----- -----");
				break;  // The switch is getting left.
			case 3:
				std::printf(" ----- ----- -----\n |   | |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n |");
				printCardValue(deck[0]);
				std::printf("| |");
				printCardValue(deck[1]);
				std::printf("| |");
				printCardValue(deck[2]);
				std::printf("|");
				if (chipAmount == 3 || chipAmount == 1)
				{
					std::printf("   #");
				}
				std::printf("\n |   | |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n ----- ----- -----");
				break;  // The switch is getting left.
			case 2:
				std::printf(" ----- -----\n |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n |");
				printCardValue(deck[0]);
				std::printf("| |");
				printCardValue(deck[1]);
				std::printf("|");
				if (chipAmount == 3 || chipAmount == 1)
				{
					std::printf("   #");
				}
				std::printf("\n |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n ----- -----");
				break;  // The switch is getting left.
			case 1:
				std::printf(" -----\n |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n |");
				printCardValue(deck[0]);
				std::printf("|");
				if (chipAmount == 3 || chipAmount == 1)
				{
					std::printf("   #");
				}
				std::printf("\n |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n -----");
				break;  // The switch is getting left.
			}
			return;
		}

		void displayHidden(void)
		{
			std::printf("\n %s:\n\n ", name.c_str());
			switch (deck.size())
			{
			case 5:
				std::printf("----- ----- ----- ----- -----\n |   | |   | |   | |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n |???| |???| |???| |???| |???|");
				if (chipAmount == 3 || chipAmount == 1)
				{
					std::printf("   #");
				}
				std::printf("\n |   | |   | |   | |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n ----- ----- ----- ----- -----");
				break;
			case 4:
				std::printf("----- ----- ----- -----\n |   | |   | |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n |???| |???| |???| |???|");
				if (chipAmount == 3 || chipAmount == 1)
				{
					std::printf("   #");
				}
				std::printf("\n |   | |   | |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n ----- ----- ----- -----");
				break;
			case 3:
				std::printf("----- ----- -----\n |   | |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n |???| |???| |???|");
				if (chipAmount == 3 || chipAmount == 1)
				{
					std::printf("   #");
				}
				std::printf("\n |   | |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n ----- ----- -----");
				break;
			case 2:
				std::printf("----- -----\n |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n |???| |???|");
				if (chipAmount == 3 || chipAmount == 1)
				{
					std::printf("   #");
				}
				std::printf("\n |   | |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n ----- -----");
				break;
			case 1:
				std::printf("-----\n |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n |???|");
				if (chipAmount == 3 || chipAmount == 1)
				{
					std::printf("   #");
				}
				std::printf("\n |   |");
				if (chipAmount >= 2)
				{
					std::printf("   #");
				}
				std::printf("\n -----");
				break;
			case 0:
				if (chipAmount >= 0)
				{
					std::printf("\n");
					if (chipAmount >= 2)
					{
						std::printf("   #");
					}
					std::printf("\n");
					if (chipAmount == 3 || chipAmount == 1)
					{
						std::printf("   #");
					}
					std::printf("\n");
					if (chipAmount >= 2)
					{
						std::printf("   #");
					}
				}
				else
				{
					std::printf("\n\n  XXX\n");
				}
				break;
			}
			return;
		}

		void looseChip(void)  // This removes a chip from the player.
		{
			chipAmount--;  // Remove one chip from the player.
			if (chipAmount == outGame)
			{
				activePlayers--;  // The variable "activePlayers" is getting decreased by one.
			}
			return;  // The function is getting ended.
		}

		signed char getChipAmount(void)  // This returns the amount of chips the player has left.
		{
			return chipAmount;  // The function is getting ended and the amount of chips the player has transfered.
		}

		void printName(void)  // This prints the name of the player.
		{
			std::printf("%s", name.c_str());  // The name of the player is getting printed.
			return;  // The function is getting ended.
		}

		unsigned char _getDeckSize(void)  // This checks, if the player still has cards and returns the size of the vector "deck" - 1.
		{
			sizeDeck = deck.size();  // The size of the deck is getting checked and saved.
			if (sizeDeck == 0)  // It is getting checked, if this player has still cards in his deck.
			{
				chipAmount = outGame;  // The chips of this player are getting taken away.
				activePlayers--;  // The variable "activePlayers" is getting decreased by one.
				return NULL;  // The function is getting ended and nothing returned.
			}
			else
			{
				return sizeDeck - 1;  // The function is getting ended and the size of the vector "deck" minus one returned.
			}
		}

		signed char makeDecision(void)
		{
			if (deck[0] == changeDirection)
			{
				return giveCard(0);
			}
			if (takeTwo == false)
			{
				return giveCard(0);
			}
			signed char tempValue = value + deck[0];
			if (tempValue < 77 && tempValue != 11 && tempValue != 22 && tempValue != 33 && tempValue != 44 && tempValue != 55 && tempValue != 66)
			{
				return giveCard(0);
			}
			if (sizeDeck >= 2)
			{
				if (deck[1] == changeDirection)
				{
					return giveCard(1);
				}
				if (takeTwo == false)
				{
					return giveCard(1);
				}
				signed char tempValue = value + deck[1];
				if (tempValue < 77 && tempValue != 11 && tempValue != 22 && tempValue != 33 && tempValue != 44 && tempValue != 55 && tempValue != 66)
				{
					return giveCard(1);
				}
			}
			if (sizeDeck >= 3)
			{
				if (deck[2] == changeDirection)
				{
					return giveCard(2);
				}
				if (takeTwo == false)
				{
					return giveCard(2);
				}
				signed char tempValue = value + deck[2];
				if (tempValue < 77 && tempValue != 11 && tempValue != 22 && tempValue != 33 && tempValue != 44 && tempValue != 55 && tempValue != 66)
				{
					return giveCard(2);
				}
			}
			if (sizeDeck >= 4)
			{
				if (deck[3] == changeDirection)
				{
					return giveCard(3);
				}
				if (takeTwo == false)
				{
					return giveCard(3);
				}
				signed char tempValue = value + deck[3];
				if (tempValue < 77 && tempValue != 11 && tempValue != 22 && tempValue != 33 && tempValue != 44 && tempValue != 55 && tempValue != 66)
				{
					return giveCard(3);
				}
			}
			if (sizeDeck >= 5)
			{
				if (deck[4] == changeDirection)
				{
					return giveCard(4);
				}
				if (takeTwo == false)
				{
					return giveCard(4);
				}
				signed char tempValue = value + deck[4];
				if (tempValue < 77 && tempValue != 11 && tempValue != 22 && tempValue != 33 && tempValue != 44 && tempValue != 55 && tempValue != 66)
				{
					return giveCard(4);
				}
			}
			return giveCard(std::rand() % (sizeDeck -1) +0);
		}

		void takeCard(void)
		{
			getCard(hiddenDeck.back());
			hiddenDeck.pop_back();
			hiddenDeck.shrink_to_fit();
			return;
		}
	};

	std::vector<Player> round;  // The vector "round" contains all players of the current game.

	unsigned char sizeRound;  // The variable "sizeRound" saves the size of the vector "round".

	unsigned char _sizeRound;  // The variable "_sizeRound" contains the value of the variable "sizeRound" - 1.

	unsigned char currentPlayer = 0;  // "currentPlayer" saves, which ones turn it is.

	unsigned char startPlayer = 1;  // "startPlayer" saves, who gets "currentPlayer" first on the next round.

	unsigned char myPlayer;  // The variable "myPlayer" saves, which "object"-object is the one of this player.

	signed char takeCardFromHiddenDeck(void)  // This moves the last card from the hidden deck to the deck of the player.
	{
		signed char lastCard = hiddenDeck.back();  // The top card of the hidden deck is getting saved.
		hiddenDeck.pop_back();  // The top card of the hidden deck is getting erased.
		hiddenDeck.shrink_to_fit();  // Making sure, that there´s no free space left in the vector "hiddenDeck".
		return lastCard;  // The function is getting ended and the value of the former top card of the hidden deck.
	}

	void mix(void)  // This mixes the cards and hands them out.
	{
		hiddenDeck.erase(hiddenDeck.begin(), hiddenDeck.end());  // The content of the vector "hiddenDeck" is getting completely erased.
		hiddenDeck.shrink_to_fit();  // Making sure, that there´s no free space left in the vector "hiddenDeck".
		hiddenDeck = { 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10, 11, 22, 33, 44, 55, 66, changeDirection, changeDirection, changeDirection, changeDirection, changeDirection, x2, x2, x2, x2, -10, -10, -10, -10, 0, 0, 0, 0, 76 };  // The vector "hiddenDeck" is getting all card of LOBO 77, to hand them out in random order at the beginning of a new round.
		openDeck.erase(openDeck.begin(), openDeck.end());  // The content of the vector "openDeck" gis getting completely erased.
		openDeck.shrink_to_fit();  // Making sure, that there´s no free space left in the vector "openDeck".
		std::random_shuffle(hiddenDeck.begin(), hiddenDeck.end());  // The order of the elements of the vector "hiddenDeck" is getting randomized.
		unsigned char iterator = 0;  // An iterator for the vector "cardDispenser" is getting declared.
		for (unsigned char i = 0; i < 5; i++)  // The cards are getting handed out to the players.
		{
			for (unsigned char i2 = 0; i2 < sizeRound; i2++)
			{
				round[i2].getCard(hiddenDeck[iterator]);  // The cards are getting handed out to the players.
				iterator++;  // The value of the iterator is getting raised.
				continue;  // The loop is getting continued;
			}
			continue;  // The loop is getting continued;
		}
		iterator--;
		hiddenDeck.erase(hiddenDeck.begin(), hiddenDeck.begin() + iterator);  // The rest of the cards of the cardDispenser are getting added to the hidden card deck.
		hiddenDeck.shrink_to_fit();  // Making sure, that there´s no free space left in the vector "hiddenDeck".
		return;  // The function is getting ended.
	}

	void updateTexts(void)  // This is adjusting the texts to the current choosen language.
	{
		if (language == English)
		{
			sloganText = "					Someone it hits everytime...";
			playText = "Play";
			manualText = "Manual";
			settingsText = "Settings";
			creditsText = "Credits";
			closeText = "Close";
			backToMenuText = "Back to menu";
			explanationText = "Who looses all his chips looses the game.";
			developersText = "Programmer:\n  Carsten";
			roundSizeText = "Please chosse the number of players:";
			chooseNameText = "Please put your name in:";
			tooLongInputText = "The input was too long";
			refreshedHiddenDeckText = "The hidden deck was refreshed.";
			valueText = "Aggregate value:";
			winnerText = "The winner is";
		}
		else if (language == German)
		{
			sloganText = "					Einen trifft's immer...";
			playText = "Spielen";
			manualText = "Anleitung";
			settingsText = "Einstellungen";
			creditsText = "Mitwirkende";
			closeText = "Beenden";
			backToMenuText = "Zur"; backToMenuText += ü; backToMenuText += "ck zum Men"; backToMenuText += ü;
			explanationText = "Wer alle seine Chips verliert, verliert das Spiel.";
			developersText = "Prgrammierer:\n  Carsten";
			roundSizeText = "Bitte w"; roundSizeText += ä; roundSizeText += "hle die Anzahl an Spielern:";
			chooseNameText = "Bitte gib deinen Namen ein:";
			tooLongInputText = "Die Eingabe war zu lang!";
			refreshedHiddenDeckText = "Der geschlossene Stapel wurde erneuert.";
			valueText = "Gesamtwert:";
			winnerText = "Der Gewinner ist:";
		}
		return;  // The function is getting ended.
	}

	void backToMenuButton(void)  // The bottom of "manual", "settings" and "credits".
	{
		std::printf("\n\n\n > %s", backToMenuText.c_str());  // Displaying the "Back to menu"-button.
		while (_getch() != returnKey)  // Wait for the enter key getting pressed.
		{
			continue;  // Progressing with the loop.
		}
		return;  // Returning to the menu.
	}

	void playOpen(void)  // This is the actual game logic.
	{
		unsigned char selection = 2;  // The variable "selection" saves the position of the cursor.
		bool waiting = true;  // The variable "waiting" saves, if the current loop should getting stain in.
		while (waiting)
		{
			std::system("CLS");  // The console is getting cleared.
			std::printf("\n %s\n", roundSizeText.c_str());  // The player is getting asked for the number of players in this game.
			switch (selection)  // The selection is getting printed.
			{
			case 2:
				std::printf(" > 2\n   3\n   4\n   5\n   6\n   7\n   8");  // The selection is getting printed.
				break;  // The switch is getting left.
			case 3:
				std::printf("   2\n > 3\n   4\n   5\n   6\n   7\n   8");  // The selection is getting printed.
				break;  // The switch is getting left.
			case 4:
				std::printf("   2\n   3\n > 4\n   5\n   6\n   7\n   8");  // The selection is getting printed.
				break;  // The switch is getting left.
			case 5:
				std::printf("   2\n   3\n   4\n > 5\n   6\n   7\n   8");  // The selection is getting printed.
				break;  // The switch is getting left.
			case 6:
				std::printf("   2\n   3\n   4\n   5\n > 6\n   7\n   8");  // The selection is getting printed.
				break;  // The switch is getting left.
			case 7:
				std::printf("   2\n   3\n   4\n   5\n   6\n > 7\n   8");  // The selection is getting printed.
				break;  // The switch is getting left.
			case 8:
				std::printf("   2\n   3\n   4\n   5\n   6\n   7\n > 8");  // The selection is getting printed.
				break;  // The switch is getting left.
			}
			switch (_getch())  // The user input is getting got and used.
			{
			case upKey:
				if (selection == 2)
				{
					selection = 8;
				}
				else
				{
					selection--;
				}
				break;  // The switch is getting left.
			case downKey:
				if (selection == 8)
				{
					selection = 2;
				}
				else
				{
					selection++;
				}
				break;  // The switch is getting left.
			case returnKey:
				sizeRound = selection;  // The selection is getting saved.
				waiting = false;  // The loop is getting broke.
				break;  // The switch is getting left.
			default:  // Another key is getting pressed.
				break;  // The switch is getting left.
			}
		}
		waiting = true;  // The variable "waiting" is getting resetted.
		_sizeRound = sizeRound - 1;  // The size of the variable "sizeRound" is getting saved minus one in the variable "_sizeRound";
		myPlayer = std::rand() % _sizeRound +0;  // The actual player is getting a random position.
		selection = 2;  // The variable "selection" is getting cleared.
		activePlayers = sizeRound;  // The number of active players is set to the size of the round.
		std::system("CLS");  // The console is getting cleared.
		std::printf("\n %s\n  ", chooseNameText.c_str());  // An order to choose a name is getting printed.
		round.push_back(Player(false));  // The "Player"-object for the player is getting created.
		for (unsigned char i = 1; i < sizeRound; i++)
		{
			round.push_back(Player(true));  // A "Player"-object for an AI is getting created.
			continue;  // The loop is getting continued;
		}
		signed char choosenCard;  // This saves the value of the last choosen card.
		unsigned char _sizeDeck;  // This saves the size of the deck of the currentPlayer - 1.
		mix();  // The cards are getting mixed and handed out.

		auto showDecks1 = [](void)  // This prints all the decks till the actual players one.
		{
			std::system("CLS");
			std::printf("\n");
			if (direction == 0)
			{
				std::printf("   <--");
			}
			else
			{
				std::printf("   -->");
			}
			if (takeTwo)
			{
				std::printf("   x2");
			}
			std::printf("\n");
			for (unsigned char i = 0; i < myPlayer; i++)
			{
				round[i].displayHidden();
				continue;
			}
			round[myPlayer].displayDeck();
			std::printf("\n\n");
			return;
		};

		auto showDecks2 = [](void)  // This displays all the decks after the players one.
		{
			for (unsigned char i = myPlayer + 1; i <= _sizeRound; i++)
			{
				round[i].displayHidden();
				continue;  // The loop is getting continued.
			}
			return;  // The lambda is getting ended.
		};

		bool tookCard = false;  // The variable "tookCard" saves, if the player has already taken a new card from the hidden deck.
		bool changePlayer = true;  // The variable "changePlayer" saves, if the current player should get changed at the end of the function "playOpen".

		while (activePlayers >= 2)  // This is the actual game.
		{
			_sizeDeck = round[currentPlayer]._getDeckSize();  // The size of the deck of the current player minus one is getting checked and saved.
			if (round[currentPlayer].getChipAmount() >= 0)
			{
				if (currentPlayer == myPlayer)
				{
					while (_getch() != returnKey)  // The game is waiting for a press of the return-Key.
					{
						continue;  // The loop is getting continued.
					}
					while (waiting)
					{
						showDecks1();  // The decks of the players before the actual player are getting printed.
						if (selection > _sizeDeck)
						{
							selection = _sizeDeck;
						}
						std::printf(" ");
						switch (selection)
						{
						case 0:
							std::printf("  ^");
							break;  // The switch is getting left.
						case 1:
							std::printf("        ^");
							break;  // The switch is getting left.
						case 2:
							std::printf("              ^");
							break;  // The switch is getting left.
						case 3:
							std::printf("                    ^");
							break;  // The switch is getting left.
						case 4:
							std::printf("                          ^");
							break;  // The switch is getting left.
						}
						std::printf("\n");
						showDecks2();  // The decks of the players after the actual player are getting printed.
						switch (_getch())  // An input is getting got.
						{
						case leftKey:
							if (selection == 0)
							{
								selection = _sizeDeck;
							}
							else
							{
								selection--;  // The variable "selection" is getting decreased by one.
							}
							break;  // The switch is getting left.
						case rightKey:
							if (selection == _sizeDeck)
							{
								selection = 0;
							}
							else
							{
								selection++;  // The variable "selection" is getting increased by one.
							}
							break;  // The switch is getting left.
						case returnKey:
							choosenCard = round[currentPlayer].giveCard(selection);
							if (takeTwo == false || choosenCard != x2)
							{
								while (waiting)
								{
									switch (_getch())
									{
									case returnKey:
										waiting = false;
										break;
									case tabulatorKey:
										if (tookCard == false)
										{
											round[myPlayer].takeCard();
											tookCard = true;
										}
										break;
									default:
										break;
									}
								}
							}
							tookCard = false;
							break;  // The switch is getting left.
						default:
							break;  // The switch is getting left.
						}
						continue;  // The loop is getting continued.
					}
					waiting = true;  // The value "waiting" is getting resetted.
					selection = 2;  // The value "selection" is getting resetted.
				}
				else
				{
					showDecks1();
					showDecks2();
					choosenCard = round[currentPlayer].makeDecision();
					if (std::rand() % 256 + 0 != 0)
					{
						round[currentPlayer].takeCard();
					}
				}
				openDeck.push_back(choosenCard);
				if (choosenCard == changeDirection)
				{
					direction = !direction;
				}
				else if (choosenCard == x2)
				{
					takeTwo = true;
				}
				else
				{
					value += choosenCard;
				}
				if (value == 11 || value == 22 || value == 33 || value == 44 || value == 55 || value == 66)
				{
					round[currentPlayer].looseChip();
				}
				else if (value >= 77)
				{
					round[currentPlayer].looseChip();
					if (startPlayer < _sizeRound)
					{
						startPlayer++;  // The variable "startPlayer" is getting increased by one.
					}
					else
					{
						startPlayer = 0;
					}
					currentPlayer = startPlayer;
					mix();  // The cards are getting mixed.
					takeTwo = false;
					changePlayer = false;
				}
				if (hiddenDeck.size() == 0)
				{
					hiddenDeck.insert(hiddenDeck.end(), openDeck.begin(), openDeck.end());
					openDeck.erase(openDeck.begin(), openDeck.end());
					openDeck.shrink_to_fit();
					std::random_shuffle(hiddenDeck.begin(), hiddenDeck.end());
					std::printf("\n\n %s", refreshedHiddenDeckText.c_str());
				}
				if (takeTwo)
				{
					if (currentPlayer == myPlayer)
					{
						while (_getch() != returnKey)
						{
							continue;
						}
						while (waiting)
						{
							showDecks1();
							if (selection > _sizeDeck)
							{
								selection = _sizeDeck;
							}
							std::printf(" ");
							switch (selection)
							{
							case 0:
								std::printf("  ^");
								break;
							case 1:
								std::printf("        ^");
								break;
							case 2:
								std::printf("              ^");
								break;
							case 3:
								std::printf("                    ^");
								break;
							case 4:
								std::printf("                          ^");
								break;
							}
							std::printf("\n");
							showDecks2();
							switch (_getch())
							{
							case leftKey:
								if (selection == 0)
								{
									selection = _sizeDeck;
								}
								else
								{
									selection--;  // The variable "selection" is getting decreased by one.
								}
								break;  // The switch is getting left.
							case rightKey:
								if (selection == _sizeDeck)
								{
									selection = 0;
								}
								else
								{
									selection++;  // The variable "selection" is getting increased by one.
								}
								break;  // The switch is getting left.
							case returnKey:
								choosenCard = round[currentPlayer].giveCard(selection);
								if (takeTwo == false || choosenCard != x2)
								{
									while (waiting)
									{
										switch (_getch())  // An user input is getting got.
										{
										case returnKey:
											waiting = false;
											break;  // The switch is getting left.
										case tabulatorKey:
											if (tookCard == false)
											{
												round[myPlayer].takeCard();
												tookCard = true;
											}
											break;  // The switch is getting left.
										default:
											break;  // The switch is getting left.
										}
									}
								}
								tookCard = false;
								break;  // The switch is getting left.
							default:
								break;  // The switch is getting left.
							}
							continue;  // The loop is getting continued.
						}
						waiting = true;  // The value "waiting" is getting resetted.
						selection = 2;  // The value "selection" is getting resetted.
					}
					else
					{
						showDecks1();  // The decks are
						showDecks2();  // getting printed.
						choosenCard = round[currentPlayer].makeDecision();
						if (std::rand() % 256 + 0 != 0)
						{
							round[currentPlayer].takeCard();
						}
					}
					openDeck.push_back(choosenCard);
					if (choosenCard == changeDirection)
					{
						direction = !direction;
					}
					else if (choosenCard == x2)
					{
						takeTwo = true;
					}
					else
					{
						value += choosenCard;
					}
					if (value == 11 || value == 22 || value == 33 || value == 44 || value == 55 || value == 66)
					{
						round[currentPlayer].looseChip();
					}
					else if (value >= 77)
					{
						round[currentPlayer].looseChip();
						if (startPlayer < _sizeRound)
						{
							startPlayer++;  // The variable "startPlayer" is getting increased by one.
						}
						else
						{
							startPlayer = 0;
						}
						currentPlayer = startPlayer;
						mix();
						takeTwo = false;
						changePlayer = false;
					}
					if (hiddenDeck.size() == 0)
					{
						hiddenDeck.insert(hiddenDeck.end(), openDeck.begin(), openDeck.end());
						openDeck.erase(openDeck.begin(), openDeck.end());
						openDeck.shrink_to_fit();
						std::random_shuffle(hiddenDeck.begin(), hiddenDeck.end());
						std::printf("\n\n %s", refreshedHiddenDeckText.c_str());
					}
				}
				takeTwo = false;
				std::system("CLS");  // The console is getting cleared.
				std::printf("\n %s %d", valueText.c_str(), value);
			}
			if (changePlayer)
			{
				if (direction == 0)
				{
					if (currentPlayer < _sizeRound)
					{
						currentPlayer++;  // The variable "currentPlayer" is getting increased by one.
					}
					else
					{
						currentPlayer = 0;
					}
				}
				else
				{
					if (currentPlayer > 0)
					{
						currentPlayer--;  // The variable "currentPlayer" is getting decreased by one.
					}
					else
					{
						currentPlayer = _sizeRound;
					}
				}
			}
			changePlayer = true;
			continue;  // The loop is getting continued;
		}
		std::system("CLS");  // The console is getting cleared.
		value = 0;
		round.clear();  // The vector "round" is getting cleared.
		round.shrink_to_fit();  // It is maken sure, that the vector "round" is empty.
		std::printf("\n %s ", winnerText.c_str());
		if (round[0].getChipAmount() >= 0)
		{
			round[0].printName();
		}
		else if (round[1].getChipAmount() >= 0)
		{
			round[1].printName();
		}
		else if (round[2].getChipAmount() >= 0)
		{
			round[2].printName();
		}
		else if (round[3].getChipAmount() >= 0)
		{
			round[3].printName();
		}
		else if (round[4].getChipAmount() >= 0)
		{
			round[4].printName();
		}
		else if (round[5].getChipAmount() >= 0)
		{
			round[5].printName();
		}
		else if (round[6].getChipAmount() >= 0)
		{
			round[6].printName();
		}
		else if (round[7].getChipAmount() >= 0)
		{
			round[7].printName();
		}
		while (_getch() != returnKey)  // The game is waiting for a press of the return-Key.
		{
			continue;  // The loop is getting continued;
		}
		return;  // Returning to the menu.
	}

	void manualOpen(void)  // This shows the manual for LOBO 77 PC.
	{
		std::system("CLS");  // The console is getting cleared.
		std::printf("\n %s", explanationText.c_str());  // The manual is getting printed.
		backToMenuButton();  // Create button for getting back to the menu.
		return;  // Returning to the menu.
	}

	void settingsOpen(void)  // This opens the settings for the language.
	{
		unsigned char selection = language;  // "selection" is saving the current cursor position.

		while (true)  // An endless loop, that can be broken through from inside.
		{
			std::system("CLS");  // The console is getting cleared.

			std::printf("\n Choose your language:\n ");  // The "Choose your language"-text is getting printed.

			std::ofstream settingsFile("settings.txt", std::fstream::out | std::fstream::trunc);  // The settings file is getting opened and cleared.

			switch (selection)  // The current language selection is getting printed.
			{
			case English:
				std::printf("> English\n   Deutsch");  // The current language selection is getting printed.
				break;  // The switch is getting left.
			case German:
				std::printf("  English\n > Deutsch");  // The current language selection is getting printed.
				break;  // The switch is getting left.
			}
			switch (_getch())  // The user input is getting read and used.
			{
			case upKey:
				if (selection == English)
				{
					selection = German;  // Moving the cursor to the bottom.
				}
				else
				{
					selection--;  // Moving the cursor up.
				}
				break;  // The switch is getting left.
			case downKey:
				if (selection == German)
				{
					selection = English;  // Moving the cursor to the top.
				}
				else
				{
					selection++;  // Moving the cursor down.
				}
				break;  // The switch is getting left.
			case returnKey:
				settingsFile << selection;  // The selection is getting saved to the settings file.
				settingsFile.close();  // The settings file is getting closed.
				language = selection;  // The selection is getting saved in the memory.
				updateTexts();  // Adjust the texts to the new choosen language.
				return;  // Returning to the menu.
			default:  // This is getting executed, when a different button gets pressed.
				break;  // The switch is getting left.
			}
			continue;  // The loop is getting continued.
		}
	}

	void creditsOpen(void)
	{
		std::system("CLS");  // The console is getting cleared.
		std::printf("\n %s", developersText.c_str());  // The credits are getting printed.
		backToMenuButton();  // Create button for getting back to the menu.
		return;  // Returning to the menu.
	}

	void menuOpen(void)  // From here you can select another function.
	{
		unsigned char selection = Play;  // "selection" is saving the current menu option selection.

		while (true)  // An endless loop, that can be broken through from inside.
		{
			std::system("CLS");  // The console is getting cleared.

			std::printf("\n						LOBO 77\n\n%s\n\n\n", sloganText.c_str());  // The games title is getting printed.

			switch (selection)  // The menu selection is getting printed.
			{
			case Play:
				std::printf(" > %s\n   %s\n   %s\n   %s\n   %s", playText.c_str(), manualText.c_str(), settingsText.c_str(), creditsText.c_str(), closeText.c_str());  // The menu selection is getting printed.
				break;  // The switch is getting left.
			case Manual:
				std::printf("   %s\n > %s\n   %s\n   %s\n   %s", playText.c_str(), manualText.c_str(), settingsText.c_str(), creditsText.c_str(), closeText.c_str());  // The menu selection is getting printed.
				break;  // The switch is getting left.
			case Settings:
				std::printf("   %s\n   %s\n > %s\n   %s\n   %s", playText.c_str(), manualText.c_str(), settingsText.c_str(), creditsText.c_str(), closeText.c_str());  // The menu selection is getting printed.
				break;  // The switch is getting left.
			case Credits:
				std::printf("   %s\n   %s\n   %s\n > %s\n   %s", playText.c_str(), manualText.c_str(), settingsText.c_str(), creditsText.c_str(), closeText.c_str());  // The menu selection is getting printed.
				break;  // The switch is getting left.
			case Close:
				std::printf("   %s\n   %s\n   %s\n   %s\n > %s", playText.c_str(), manualText.c_str(), settingsText.c_str(), creditsText.c_str(), closeText.c_str());  // The menu selection is getting printed.
				break;  // The switch is getting left.
			}
			switch (_getch())  // The user input is getting read and used.
			{
			case upKey:
				if (selection == Play)
				{
					selection = Close;  // Moving the cursor to the bottom.
				}
				else
				{
					selection--;  // Moving the cursor up.
				}
				break;  // The switch is getting left.
			case downKey:
				if (selection == Close)
				{
					selection = Play;  // Moving the cursor to the top.
				}
				else
				{
					selection++;  // Moving the cursor down.
				}
				break;  // The switch is getting left.
			case returnKey:
				switch (selection)
				{
				case Play:
					playOpen();  // The actual game is getting started.
					break;  // The switch is getting left.
				case Manual:
					manualOpen();  // The manual is getting opened.
					break;  // The switch is getting left.
				case Settings:
					settingsOpen();  // The settings are getting opened.
					break;  // The switch is getting left.
				case Credits:
					creditsOpen();  // The credits are getting opened.
					break;  // The switch is getting left.
				case Close:
					return;  // The program is getting closed.
				}
			default:  // This is getting executed, when a different button gets pressed.
				break;  // The switch is getting left.
			}
			continue;  // The loop is getting continued.
		}
	}
}

int main(void)  // This function will be called first, when the game is started.
{
	std::system("title LOBO 77");  // The title of the window is getting changed.
	std::system("color F1");  // The color of the output is getting set to White/Blue.
	PlaySound(TEXT("music.mp3"), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC | SND_LOOP);  // The background music is getting started.
	//std::ifstream("settings.txt", std::fstream::in) >> LOBO_77::language;  // The language selection is getting read.
	LOBO_77::updateTexts();  // The texts are getting loaded.
	LOBO_77::menuOpen();  // LOBO 77 is getting started.
	return 0;  // The program is getting closed and marked as successfully executed.
}