#ifndef __Q2TALLYVOTES_H__
#define __Q2TALLYVOTES_H__ 1

_Monitor Printer;

#if defined( EXT )                           // external scheduling monitor solution
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
#elif defined( INT )                         // internal scheduling monitor solution
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
    uCondition bench;          // condition variable to block voters when group vote not completed
#elif defined( INTB )                        // internal scheduling monitor solution with barging
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
    uCondition bench;                        // only one condition variable (variable name may be changed)
    void wait();                             // barging version of wait
    void signalAll();                        // unblock all waiting tasks
    unsigned int ticketNum;                  // current ticket count
    unsigned int allowedTicketNum;           // ticket count allowed
    unsigned int bargeBlockedNum;            // number of voters blocked in barging
#elif defined( AUTO )                        // automatic-signal monitor solution
#include "AutomaticSignal.h"
_Monitor TallyVotes {
    // private declarations for this kind of vote-tallier
    AUTOMATIC_SIGNAL;
    bool groupDone;                          // boolean flag that indicate a group of voting is done
#elif defined( TASK )                        // internal/external scheduling task solution
_Task TallyVotes {
    // private declarations for this kind of vote-tallier
    uCondition bench;                        // condition variable for blocked tasks
    bool hasFailed;                          // flag to denote if the group formation is successful
    unsigned int curVoterID;                 // store the current voter id
    void main();
#else
    #error unsupported voter type
#endif
    // common declarations
  public:                                    // common interface
    _Event Failed {};
    TallyVotes( unsigned int voters, unsigned int group, Printer & printer );
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum TourKind { Picture = 'p', Statue = 's', GiftShop = 'g' };
    struct Tour { TourKind tourkind; unsigned int groupno; };
    Tour vote( unsigned int id, Ballot ballot );
    void done();
  private:
    unsigned int totalVoterNum;     // total number of voters
    unsigned int groupSize;         // group size
    unsigned int finishedVoterNum;  // already terminated voters
    unsigned int voteNum;           // number of votes done in current round
    unsigned int pictureVotes;      // total votes for picture tour
    unsigned int statueVotes;       // total votes for statue tour
    unsigned int giftshopVotes;     // total votes for gift shop tour
    Tour winTour;                   // the winner tour
    Printer & printer;              // printer
#if defined( TASK )
    Ballot curBallot;               // store the current ballot of the voter
#endif
};

#endif