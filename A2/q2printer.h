#ifndef __PRINTER_H__
#define __PRINTER_H__ 1

#include <vector>
#pragma once

// Structure that sotres one player's details
struct Detail {
    int playerID;
    int takenNum;
    int remainDeckNum;
    char direction;                                         // 'l' for left turn and 'r' for right turn
    bool hasDead;                                           // indicator for whether the player is dead
    bool hasWon;                                            // indicator for whether the player has won
    bool hasFlushed;                                        // indicator for whether detail has been flushed
    Detail(int playerID, int takenNum, int remainDeckNum, char direction, bool hasDead);
}; 

class Printer {
    unsigned int NoOfPlayers;
    unsigned int NoOfCards;
    std::vector<Detail *> gameDetails;                     // stores the datails for each player for printing
    void printMetadata();                             // print player num and card num;
    void flushInfo();                                 // print the game detail
  public:
    Printer( const unsigned int NoOfPlayers, const unsigned int NoOfCards );
    ~Printer();
    void prt( unsigned int id, int took, int RemainingPlayers );  // update buffer info
}; // Printer

#endif