#include <string>
#include <iostream>
#include <ctime>
#include <windows.h>

using std::string;
using std::cout;
using std::cin;

void start(string& name1, string& name2);
void deckCreate(int deck[9][12]); // creates an uno deck
void drawCard(int hand[][2], int index, int deck[9][12]); // adds a new card to a user's hand
void gameplay(int p1Hand[][2], int& p1HandSize, int pOppHand[][2], int& pOppHandSize, int deck[9][12], string name, int& card, int& cardColor, int& cardsDrawn, bool& pTurn, bool& game); // a turn function
void printHand(int player[][2], int arrSize, string name); // prints the hand of a player
int colorCode(int row, int column, int deck[9][12]); // codes the color of a card
void displayCard(int card, int color); // displays the colored card
bool compatible(int card, int color, int selection, int selectionColor); // checks if the cards played by the players are correct
int changeColor(); // sets a new color selected by the user after a color change or a +4 cards are played

// used as a placeholder for cards in the deck that have been drawn
#define EMPTY_DECK_SLOT 99
// used to define color change and +4 cards as having no card value other than their color
#define NO_CARD_VALUE -1

// each card color has an integer value assigned to it
#define WHITE 0
#define RED 1
#define BLUE 2
#define GREEN 3
#define YELLOW 4

int main()
{
    // Initializes random
    std::srand(static_cast<unsigned>(time(NULL)));

    int deck[9][12], p1Hand[100][2], p2Hand[100][2];
    int p1HandSize = 7, p2HandSize = 7, cardcardsDrawn = 14; // players' hand sizes + amount of cards drawn by the players
    int card, cardColor; // the top-most card in the game of uno
    string p1, p2;

    deckCreate(deck);//creates an uno deck

    // assigns the starting hands to both players
    for (int i = 0; i < 7; i++) 
    {
        drawCard(p1Hand, i, deck);
        drawCard(p2Hand, i, deck);
    }
    
    // introduction to the game, asks for player names
    start(p1, p2); 

    // selects a starting card for the game
    { 
        int row, column;
        do 
        {
            row = rand() % 9;
            column = rand() % 12;
        } while (deck[row][column] == 20 || deck[row][column] == 30 || deck[row][column] == 10 ||
            deck[row][column] == 11 || deck[row][column] == 12 || deck[row][column] == EMPTY_DECK_SLOT); // the card CANNOT be a reverse, skip, +4, change color, +2

        card = deck[row][column];
        cardColor = colorCode(row, column, deck);

        cout << "The starting card is: "; 
        displayCard(card, cardColor);
        cout << '\n';

        deck[row][column] = EMPTY_DECK_SLOT;
    }

    bool game = true, p1Turn = true; // switches between players' turns

    while (game) // main game loop, until a player wins
    { 
        if (p1Turn) // all of the turn happens in these functions
            gameplay(p1Hand, p1HandSize, p2Hand, p2HandSize, deck, p1, card, cardColor, cardcardsDrawn, p1Turn, game);
        else
            gameplay(p2Hand, p2HandSize, p1Hand, p1HandSize, deck, p2, card, cardColor, cardcardsDrawn, p1Turn, game);
    }

    cin.get();
    return 0;
}

void start(string& name1, string& name2) {
    cout << "Welcome to UNO!\nThis is a two-player game, so please enter your names below!\n";
    cout << "Player 1 name: ";
    getline(cin, name1);
    cout << "Player 2 name: ";
    getline(cin, name2);
    cout << "Alright, " << name1 << " & " << name2 << "! We're all set to go!\n"
         << "It is currently " << name1 << "'s turn, so " <<  name2 << " should look away!\n";
    Sleep(1000);
    cout << "Press ENTER when ready to begin!";
    cin.ignore(80, '\n');
    Sleep(2000);
    // Clears the tutorial screen
    system("cls");
}

void deckCreate(int deck[9][12]) { // creates an uno deck
    for (int row = 0; row < 9; row++) {
        for (int column = 0; column < 12; column++) {
            if (row < 8)
                deck[row][column] = column + 1; // 1-9 + wild cards (10 = skip, 11 = reverse, 12 = +2)
            else 
            {
                if (column < 4)
                    deck[row][column] = 0; // zeros
                else if (column < 8)
                    deck[row][column] = 20; // change color
                else
                    deck[row][column] = 30; // +4
            }
            // to get a visual representation of how the deck is created, please uncomment the two lines below
            //cout << deck[row][column] << " ";
        }
        //cout << '\n';
    }
}

void drawCard(int hand[][2], int index, int deck[9][12]) {
    int row, column;
    do
    {
        row = rand() % 9;
        column = rand() % 12;
    } while (deck[row][column] == EMPTY_DECK_SLOT);

    hand[index][0] = deck[row][column];
    hand[index][1] = colorCode(row, column, deck);

    deck[row][column] = EMPTY_DECK_SLOT;
}

void gameplay(int pHand[][2], int& pHandSize, int pOppHand[][2], int& pOppHandSize, int deck[9][12], string name, int& cardValue, int& cardColor, int& cardsDrawn, bool& pTurn, bool& game){
    const HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    printHand(pHand, pHandSize, name);

    int index, draws = NULL;

    while (draws != 2) // while a matching card has NOT been put down OR the user has NOT drawn two cards
    { 
        while(true) // error check
        { 
            cout << "\nPlease enter the index of the card that you want to play(same color/ same value): ";
            cin >> index;

            if (!cin || index < 0 || index > pHandSize)
                cout << "Invalid Input!\n";
            else
                break;

            cin.clear();
            cin.ignore(80, '\n');
        }

        if (index != 0) // checks that the user's card matches the one currently on top
        {
            // if the user puts down a correct card, break out of the loop
            if (!compatible(cardValue, cardColor, pHand[index - 1][0], pHand[index - 1][1]))
                cout << "The cards don't match!\n";
            else
                break;
        }
        else 
        { // if the user wants to draw a card
            drawCard(pHand, pHandSize, deck);
            pHandSize++;
            printHand(pHand, pHandSize, name);
            cardsDrawn++;
            draws++;
        }
    }

    if (draws != 2) {
        system("cls"); // clears the screen after every turn

        cardValue = pHand[index - 1][0];
        cardColor = pHand[index - 1][1];
        pHand[index - 1][0] = pHand[pHandSize - 1][0];
        pHand[index - 1][1] = pHand[pHandSize - 1][1];
        pHandSize--;

        cout << name << " played the card: ";
        displayCard(cardValue, cardColor);


        if (pHandSize == 0) {
            game = false;
            cout << name << " Wins!\n";
        }
        else {
            switch (cardValue) { // if any wild cards are played
                case 10: // skip & reverse
                case 11:
                    Sleep(1000);
                    cout << "The other player is skipped! It is your turn again!\n";
                break;

                case 12: // +2
                    cout << "The other player is skipped AND will pick up 2 cards next turn! It is your turn again!\n";
                    for (int i = 0; i < 2; i++) { // adds 2 cards to the opponent
                        drawCard(pOppHand, pOppHandSize, deck);
                        pOppHandSize++;
                        cardsDrawn++;
                    }
                    Sleep(2000);
                break;

                case 30: // +4
                    cout << "The other player is skipped AND will pick up 4 cards next turn! It is your turn again!\n";
                    for (int i = 0; i < 4; i++) { // adds 4 cards to the opponent
                        drawCard(pOppHand, pOppHandSize, deck);
                        pOppHandSize++;
                        cardsDrawn++;
                    }
                    Sleep(2000);
                    cardColor = changeColor();
                    cardValue = NO_CARD_VALUE;
                break;

                default:
                {
                    bool colorChangeCardPlayed = false;

                    if (cardValue == 20) // color change card
                    {
                        cardColor = changeColor();
                        cardValue = NO_CARD_VALUE;
                        colorChangeCardPlayed = true;
                    }

                    pTurn = !pTurn; // next players' turn
                    SetConsoleTextAttribute(h, 12);
                    cout << "Please look away! It's now the other player's turn!\n";
                    Sleep(3000); // brief pause between turns
                    

                    if (!colorChangeCardPlayed)
                    {
                        system("cls");
                        SetConsoleTextAttribute(h, 7);
                        cout << name << " played the card: ";
                        displayCard(cardValue, cardColor);
                    }

                    SetConsoleTextAttribute(h, 3);
                    cout << "Your opponent has " << pHandSize << " cards left!\n";
                    SetConsoleTextAttribute(h, 7);
                }
            }
        }
    }
    else 
    {
        pTurn = !pTurn; // next players' turn
        SetConsoleTextAttribute(h, 12);
        cout << "Two cards have been drawn, transferring the turn to your opponent...\n";
        Sleep(3000);
        system("cls");
        SetConsoleTextAttribute(h, 7);
        cout << name << " drew two cards, the top card is: ";
        displayCard(cardValue, cardColor);
        SetConsoleTextAttribute(h, 3);
        cout << "Your opponent has " << pHandSize << " cards left!\n";
        SetConsoleTextAttribute(h, 7);
    }

    if (cardsDrawn > 99) {// if there are less than 8 cards remaining in the deck, create it again
        SetConsoleTextAttribute(h, 5);
        cout << "Few cards remaining in the deck. Reshuffling...\n";
        deckCreate(deck);//creates an uno deck
        Sleep(2000);
        cout << "Deck is reshuffled!\n";
        SetConsoleTextAttribute(h, 7);
    }
}

int colorCode(int row, int column, int deck[9][12]) {
    int cardColor;

    if (row == 0 || row == 1)
        cardColor = RED;
    else if (row == 2 || row == 3)
        cardColor = BLUE;
    else if (row == 4 || row == 5)
        cardColor = GREEN;
    else if (row == 6 || row == 7)
        cardColor = YELLOW;
    else if (deck[row][column] == 0) // special treatment for 0's, as there are onlrow 4 of them in the deck
        cardColor = ++column;
    else // no color for the special cards
        cardColor = WHITE;

    return cardColor;
}

void printHand(int player[][2], int arrSize, string name) { // displays colors of cards in the users' hands
    cout << name << "'s Hand:\n";
    for (int i = 0; i < arrSize; i++) {
        cout << i + 1 << ". ";
        displayCard(player[i][0], player[i][1]);
    }
    cout << "0. Draw a card\n";
}

void displayCard(int card, int color) { // changes the console colors depending on the card
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 7); // sets the font to white
    switch (color) {
        case RED:
            SetConsoleTextAttribute(h, 12);
        break;

        case BLUE:
            SetConsoleTextAttribute(h, 9);
        break;

        case GREEN:
            SetConsoleTextAttribute(h, 10);
        break;

        case YELLOW:
            SetConsoleTextAttribute(h, 6);
        break;

        default://WHITE
            SetConsoleTextAttribute(h, 7);
    }

    if (card != 10 && card != 11 && card != 12 && card != 20 && card != 30) // if it's not a special card
        cout << card << '\n';
    else {
        if (card == 20) { // color change card
            SetConsoleTextAttribute(h, 12);
            cout << "C";
            SetConsoleTextAttribute(h, 9);
            cout << "O";
            SetConsoleTextAttribute(h, 10);
            cout << "L";
            SetConsoleTextAttribute(h, 6);
            cout << "O";
            SetConsoleTextAttribute(h, 7);
            cout << "R\n";
        }
        else if (card == 30)// +4 card
            cout << "+4\n";
        else if (card == 10) // skip
            cout << "SKIP\n";
        else if (card == 11) // reverse
            cout << "REVERSE\n";
        else if (card == 12) // +2
            cout << "+2\n";
    }
    SetConsoleTextAttribute(h, 7);
}

bool compatible(int card, int color, int selectedCard, int selectionColor) 
{
    // the color change card and +4 card can be played no matter what, if the selected card is the same value or color
    if (selectionColor == WHITE || card == selectedCard || color == selectionColor) 
        return true;

    return false;
}

int changeColor() { // sets a new color selected by the user after a color change or a +4 cards are played
    string colors[4] = { "RED", "BLUE", "GREEN", "YELLOW" };
    int cardColor;

    while (true)
    { // error check
        cout << "Please choose a new color! (1 - Red, 2 - Blue, 3 - Green, 4 - Yellow)\n";
        cin >> cardColor;

        if (!cin || cardColor < 1 || cardColor > 4)
            cout << "Invalid Input!\n";
        else
            break;

        cin.clear();
        cin.ignore(80, '\n');
    }

    system("cls"); // clears the screen after every turn

    cout << "The new color is now ";
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (cardColor) {
        case RED:
            SetConsoleTextAttribute(h, 12);
        break;

        case BLUE:
            SetConsoleTextAttribute(h, 9);
        break;

        case GREEN:
            SetConsoleTextAttribute(h, 10);
        break;

        case YELLOW:
            SetConsoleTextAttribute(h, 6);
        break;
    }
    cout << colors[cardColor - 1] << '\n';
    SetConsoleTextAttribute(h, 7);
    return cardColor;
}
