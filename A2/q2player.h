#ifndef __PLAYER_H__
#define __PLAYER_H__ 1

#include "q2printer.h"
#include "PRNG.h"

extern PRNG prng;

_Coroutine Player {
    static unsigned int playerNum;
    Printer *printer;
    unsigned int id;
    Player *leftPlayer;
    Player *rightPlayer;
    static unsigned int deckNum;
    bool hasDrunk;				// variable to indicate whether the player has drunk
    void main();
  public:
    enum { DEATH_DECK_DIVISOR = 7 }; 
    static void players( unsigned int num ); 
    Player( Printer &printer, unsigned int id ); 
    void start( Player &lp, Player &rp );
    void play( unsigned int deck );
    void drink();
}; // Player

#endif