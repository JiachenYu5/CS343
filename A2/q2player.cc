#include "q2player.h"
#include <iostream>
using namespace std;

int DEATH_DECK = 7;
unsigned int Player::playerNum = 0;
unsigned int Player::deckNum = 0;
_Event Schmilblick {};

void Player::players( unsigned int num ) {
    playerNum = num;
} // Player::players

Player::Player( Printer &printer, unsigned int id ) : 
    printer(&printer), id(id), leftPlayer(NULL), rightPlayer(NULL), hasDrunk(false) {} // Player::Player

void Player::start( Player &lp, Player &rp ) {
    leftPlayer = &lp;
    rightPlayer = &rp;
    resume();
} // Player::start

void Player::play( unsigned int deck ) {
    deckNum = deck;
    resume();
} // Player::play

void Player::drink() {
    resume();
} // Player::drink

void Player::main() {
    suspend();								// wait for the play operation
    try {
        _Enable {
            for ( ; ; ) {
                if ( playerNum <= 1 ) {                                 // End cases
                    printer->prt(id, 0, playerNum);
                    return;
                } // if

                int random = (int)prng( 1, 8 );				// generate random card draw number
		        int takeNum = (int)deckNum < random ? (int)deckNum : random;	 // pick the remaining deck number if it's less than the random generated value
                printer->prt(id, takeNum, (int)playerNum);		// update player details

                if ( (int)deckNum == takeNum ) {                        // game ends when there's no cards left
                    playerNum--;
                    return;
                } // if

                if ( deckNum % DEATH_DECK == 0 ) {                      // player dies
                    playerNum--;
                    leftPlayer->rightPlayer = rightPlayer;		// updating neighbours or the dead player's neighbouts   
                    rightPlayer->leftPlayer = leftPlayer;
                    if ( playerNum == 1 ) {                     // avoid edge case when there's one player left and start drink at the same round
                        rightPlayer->play( deckNum );           // let the one player left win immediately (i.e. don't check drink case)
                    } // if
                } else {
                    if ( prng(0, 9) == 0 ) {                    // player drinks
                        printer->prt(id, -1, playerNum);        // negative card take value denotes drink
                        try {
                            _Resume Schmilblick() _At *rightPlayer;
                            rightPlayer->drink();
                            _Enable{}
                        } _CatchResume ( Schmilblick & ) {      // player catch original        
                        }
                    } // if
                }
                deckNum -= takeNum;

                if ( deckNum % 2 == 0 ) {                                   // passes turn to the next player
                    rightPlayer->play( deckNum );
                } else {
                    leftPlayer->play( deckNum );
                } // if
            } // for
        }
    } _CatchResume ( Schmilblick & ) {
        _Resume Schmilblick() _At *rightPlayer;							   // catch Schmilblick and raise Schmilblick at player on the right
        printer->prt(id, -1, playerNum);
        rightPlayer->drink();
    }// try
} // Player::main