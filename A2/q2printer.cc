#include "q2printer.h"
#include <iostream>
#include <vector>
using namespace std;

int DEATH_DECK_NUM = 7;

// constructor for player details object, hasFlushed is initialized to ture to prevent initial unnecessary print.
Detail::Detail( int playerID, int takenNum, int remainDeckNum, char direction, bool hasDead ) :
    playerID( playerID ), takenNum( takenNum ), remainDeckNum( remainDeckNum ), direction( direction ), hasDead( hasDead ), hasWon( false ), hasFlushed( true ) {} // Detail::Detail

// private function to print the header info
void Printer::printMetadata() {
    cout << "Players: " << NoOfPlayers << "    " << "Cards: " << NoOfCards << endl;
    for ( unsigned int i = 0; i < NoOfPlayers; i++ ) {
        if ( i > 0 ) cout << "\t"; // if
        cout << "P" << i;
    } // for
    cout << endl;
} // Printer::printMetadata

// private function that do the actual print, every time it prints a whole line
void Printer::flushInfo() {
    // find the last unflushed player
    int endPlayerID;
    for ( unsigned int i = NoOfPlayers - 1; (int)i >= 0; i-- ) {
        if ( !gameDetails[i]->hasFlushed ) {
            endPlayerID = (int)i;
            break;
        } // if
    } // for

    // loop from the beginning to the last unflushed player,, print all player details that has updated
    for ( int i = 0; i <= endPlayerID; i++ ) {
        Detail * currentPlayer = gameDetails[i];
        if ( !currentPlayer->hasFlushed ) {
            if ( currentPlayer->hasWon ) {
                if ( currentPlayer->remainDeckNum == 0 ) {
                    cout << currentPlayer->takenNum << ":" << currentPlayer->remainDeckNum << "#";
                    if ( currentPlayer->hasDead ) cout << "X";
                } else {
                    cout << "#" << currentPlayer->remainDeckNum << "#";
                } // if
            } else if ( currentPlayer->takenNum < 0 ) {
                cout << "D";
            } else {
                cout << currentPlayer->takenNum << ":" << currentPlayer->remainDeckNum;
                cout << ( currentPlayer->direction == 'l' ? "<" : ">" );
                if ( currentPlayer->hasDead ) cout << "X";
            } // if
            currentPlayer->hasFlushed = true;
        } // if
        if ( i < endPlayerID ) cout << "\t"; // if
    } // for
    cout << endl;
} // Printer::flushInfo

Printer::Printer( const unsigned int NoOfPlayers, const unsigned int NoOfCards ) : 
    NoOfPlayers( NoOfPlayers ), NoOfCards( NoOfCards ) {
    
    printMetadata();
    // add all players to the players list initially
    for ( unsigned int i = 0; i < NoOfPlayers; i++ ) {
        gameDetails.push_back( new Detail( (int)i, 0, NoOfCards, 'l', false ) );
    } // for
} // Printer::Printer

Printer::~Printer() {} // Printer::~Printer

void Printer::prt( unsigned int id, int took, int RemainingPlayers ) {
    Detail * currentPlayer = gameDetails[id];
    // if the information has not been printed, call flushInfo to print first
    if ( !currentPlayer->hasFlushed ) flushInfo();

    // Update player details, took < 0 indicates drink 
    currentPlayer->hasDead = NoOfCards % DEATH_DECK_NUM == 0 ? true : false;
    if (took >= 0) NoOfCards -= took;
    currentPlayer->takenNum = took;
    currentPlayer->remainDeckNum = NoOfCards;
    currentPlayer->direction = currentPlayer->remainDeckNum % 2 == 0 ? 'r' : 'l';
    currentPlayer->hasWon = (RemainingPlayers == 1 || NoOfCards == 0);
    currentPlayer->hasFlushed = false;

    // if the player wins, print info as the game already ends
    if ( currentPlayer->hasWon ) {
        flushInfo();
	// clean all Details objects
        for ( unsigned int i = 0; i < NoOfPlayers; i++ ) {
            delete gameDetails[i];
        } // for
    }
}