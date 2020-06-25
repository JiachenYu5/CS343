#include "PRNG.h"
#include "q2printer.h"
#include "q2player.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

PRNG prng;

// help method for printing error message
void message( char * argv[] ) {
    cerr << "Usage: " << argv[0] << " [ games >= 0 | 'x' [ players >= 2 | 'x' [ cards > 0 | 'x' [ seed > 0 | 'x' ] ] ] ]" << endl;
    exit ( EXIT_FAILURE );
}

// help method for setting game number using either default or commandline input
void setGames( int *games, bool hasValue, char * argv[] ) {
    if ( !hasValue || strcmp( argv[1], "d" ) == 0 ) {
        *games = 5;
    } else {
        try {
            *games = stoi( argv[1] );
            if ( *games < 0 ) throw invalid_argument( "Game needs to be a non-negative integer." ); // if
        } catch ( const invalid_argument & ia ) {
            cerr << ia.what() << endl;
            message( argv );
        } // try
    } // if
}

// help method for setting player number using either default or commandline input
void setPlayers( int *players, bool hasValue, char * argv[] ) {
    if ( !hasValue || strcmp( argv[2], "d" ) == 0 ) {
        *players = (int)prng( 2, 10 );
    } else {
        try {
            *players = stoi( argv[2] );
            if ( *players < 2 || *players > 10 ) throw invalid_argument( "Player needs to be between 2 and 10." ); // if
        } catch ( const invalid_argument & ia ) {
            cerr << ia.what() << endl;
            message( argv );
        } // try
    } // if
}

// help method for setting card number using either default or commandline input
void setCards( int *cards, bool hasValue, char * argv[] ) {
    if ( !hasValue || strcmp( argv[3], "d" ) == 0 ) {
        *cards = (int)prng( 10, 200 );
    } else {
        try {
            *cards = stoi( argv[3] );
            if ( *cards <= 0 ) throw invalid_argument( "Card needs to be a positive integer." ); // if
        } catch ( const invalid_argument & ia ) {
            cerr << ia.what() << endl;
            message( argv );
        } // try
    } // if
}

int main ( int argc, char * argv[] ) {
    int seed = getpid();
    int games, players, cards;
    bool givePlayers = true, giveCards = true;                // flags to indicate whether the players and cards are given;
    switch ( argc ) {
        case 5:
            try {
                if ( strcmp( argv[4], "d" ) == 0 ) {
                    seed = getpid();
                } else {
                    seed = stoi( argv[4] );
                    if ( seed <= 0 ) throw invalid_argument( "Seed needs to be a positive integer." ); // if
                } // if
            } catch ( const invalid_argument & ia) {
                cerr << ia.what() << endl;
                message( argv );
            } // try
            // FALL THROUGH
        case 4:
            setGames(&games, true, argv);
            setPlayers(&players, true, argv);
            setCards(&cards, true, argv);
            break;
        case 3:
            setGames(&games, true, argv);
            setPlayers(&players, true, argv);
            setCards(&cards, false, argv);
            break;
        case 2:
            setGames(&games, true, argv);
            setPlayers(&players, false, argv);
            setCards(&cards, false, argv);
            break;
        case 1:
            setGames(&games, false, argv);
            setPlayers(&players, false, argv);
            setCards(&cards, false, argv);
            break;
        default:
            cerr << "Incorrect number of arguments" << endl;
            exit ( EXIT_FAILURE );
    } // switch
    prng.seed(seed);

    if ( strcmp( argv[2], "d" ) == 0 ) givePlayers = false; // if
    if ( strcmp( argv[3], "d" ) == 0 ) giveCards = false; // if
    // convert players and cards to unsigned int for following code
    unsigned int playersNum = (unsigned int)players;
    unsigned int cardsNum = (unsigned int)cards;

    for ( int i = 0; i < games; i++ ) {
        // if players or cards are not given, generate random number every time
        if ( !givePlayers ) playersNum = (unsigned int)prng( 2, 10 ); // if
        if ( !giveCards ) cardsNum = (unsigned int)prng( 10, 200 ); // if
        Printer printer(playersNum, cardsNum);
        Player * playerList[playersNum];
        // create player list
        for ( unsigned int j = 0; j < playersNum; j++ ) {
            playerList[j] = new Player( printer, j);
        } // for
        Player::players( playersNum );

        // generate all neighbours
        playerList[0]->start( *playerList[playersNum - 1], *playerList[1] );
        for ( unsigned int j = 1; j < playersNum - 1; j++ ) {
            playerList[j]->start( *playerList[j - 1], *playerList[j + 1] );
        } // for
        playerList[playersNum - 1]->start( *playerList[playersNum - 2], *playerList[0] );

        // start game
        int firstID = (int)prng( 0, playersNum - 1 );
        playerList[firstID]->play( cardsNum );

        // delete players
        for ( unsigned int j = 0; j < playersNum; j++ ) {
            delete playerList[j];
        } // for

        // print newline after each games except for the last
        if ( i < games - 1 ) cout << endl << endl; // if 
    } // for
}