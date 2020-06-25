#ifndef __Q2PRINTER_H__
#define __Q2PRINTER_H__ 1

#include "q2tallyVotes.h"
#include "q2voter.h"

_Monitor Printer {
    const unsigned int Voters, Undefined; unsigned int prev;
    unsigned int cnt;
    struct Items { 
        unsigned int id;
        Voter::States state; 
        union {
            TallyVotes::Ballot ballot; 
            unsigned int numBlocked; 
            TallyVotes::Tour tour;
        };
    } curr, * columns;
    
  public:
    Printer( unsigned int voters );
    ~Printer();
    void print( unsigned int id, Voter::States state );
    void print( unsigned int id, Voter::States state, TallyVotes::Tour tour );
    void print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot ); 
    void print( unsigned int id, Voter::States state, unsigned int numBlocked );
};

#endif