#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <fstream>
#include <random>
#include <map>


#define FOREGROUND(color, text) "\x1B[" << static_cast<int>(color) << "m" << text << "\033[0m"
#define BACKGROUND(color, text) "\033[3;42;" << static_cast<int>(color) << "m" << text << "\033[0m"

using namespace std;


template <typename T>
void print_vector(const std::vector<T>& vec, std::string sep = " ") // feedback: seems like this function is never used..
{
    for (auto elem : vec)
    {
        std::cout << elem << sep;
    }
    std::cout << std::endl;
}

enum class ForegroundColor : int {
    Yellow = 43,
    Green = 102,
    Gray = 100
};



int randNum(int range)
{
    random_device rd; // obtain a random number from hardware
    mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<> distr(0, range); // define the range

    return distr(gen);
}

bool CheckWordForErrors(string word) { // feedback: string argument could be a reference to avoid string copy

    if (word.length() != 5)
    {
        cout << "Your word isnt 5 letters long" << endl;
        return true;
    }

    if (word.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos) {
        cout << "It contains a special character wile it souldnt " << endl;
        return true;
    }


    return false;
}

int LetterChecking(string wordToGuess, string guessedWord, int potition, map<char, int>& m) {
    bool green = false , yellow = false;


    for (size_t i = 0; i < 5; i++)
    {
        if (guessedWord[potition] == wordToGuess[i]) {
            if (i == potition)
            {
                green = true;
            }
            yellow = true; // feedback: seems like it can be both green and yellow at the same time.
        }
    }
    if (green) {
        return (int)ForegroundColor::Green;
    }

    if (yellow == true){
        return (int)ForegroundColor::Yellow;
    }

    m[guessedWord[potition]] ++;;
    return (int)ForegroundColor::Gray;
}



int main()
{

    map<char, int> m; // feedback: Not sure I fully understand idea behind this map. As the actual values are never used,
                      // only incremented upon inserting keys to the map.
    bool winning = false;
    int numOfTries = 6;
    vector<string> WordList, listOfGuesses; // feedback: listOfGuesses is not used anywhere
    string guessingWord, wordToGuess;
    ifstream file("words.txt");
    string str;

    while (std::getline(file, str)) {
        WordList.push_back(str);
    }
    wordToGuess = WordList[randNum(WordList.size())];
    //wordToGuess = "ADDAX"; //delete me




    //cout << wordToGuess << endl; // remember to delete me

    cout << "This is wordle, a game where you guess a 5 letter word with the information: " << endl;
    cout << "If you got a letter in the right potiotion it will show in " << FOREGROUND(ForegroundColor::Green, "Green") << endl;
    cout << "If you got a letter in the wrong potiotion but its in the word it will show in " << FOREGROUND(ForegroundColor::Yellow, "Yellow") << endl;
    cout << "Or if you got a letter wrong it will show up in " << FOREGROUND(ForegroundColor::Gray, "Gray") << endl;
    cout << "You have 6 tries to get it correct, if you dont you loose" << endl;;
    cout << endl;

    do
    {
        cout << "Guess a 5 leter word: ";
        getline(cin, guessingWord);

        for_each(guessingWord.begin(), guessingWord.end(), [](char& c) {
            c = ::toupper(c);
            });



        if (CheckWordForErrors(guessingWord))
        {
            cout << "You guessed wrong you moron, no special characters or spaces just 5 letters" << endl;
        }
        else {
            // feedback: those calls could be done in a loop
            cout << FOREGROUND(LetterChecking(wordToGuess, guessingWord, 0, m), guessingWord[0]) << " " << FOREGROUND(LetterChecking(wordToGuess, guessingWord, 1, m), guessingWord[1]) << " "
                << FOREGROUND(LetterChecking(wordToGuess, guessingWord, 2, m), guessingWord[2]) << " " << FOREGROUND(LetterChecking(wordToGuess, guessingWord, 3, m), guessingWord[3]) <<
                " " << FOREGROUND(LetterChecking(wordToGuess, guessingWord, 4, m), guessingWord[4]) << endl;
        }

        cout << "Letters that are just wrong: ";

        for (auto const& pair : m) {
            cout << FOREGROUND(ForegroundColor::Gray, pair.first) << "  ";
        }
        cout << endl;
        cout << endl;


        if (guessingWord.compare(wordToGuess) == 0)
        {
            cout << "you guessed correctly you win " << endl;
            winning = true;
            continue;
        }


        numOfTries--;
    } while (numOfTries != 0 && !winning);

    if (winning)
    {
        // feedback: not sure what was the intention here, seems like some old code was not cleaned up.
    }
    else
    {
        cout << "You didnt win, you lost, to manny tries." << endl;
    }

    return 0;
}