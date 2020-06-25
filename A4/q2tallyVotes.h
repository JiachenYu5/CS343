#ifndef __Q2TALLYVOTES_H__
#define __Q2TALLYVOTES_H__ 1

_Monitor Printer;

#if defined( MC )                   // mutex/condition solution
class TallyVotes {
    unsigned int totalVoterNum;     // total number of voters
    unsigned int groupSize;         // group size
    unsigned int finishedVoterNum;  // already terminated voters
    unsigned int voteNum;           // number of votes done in current round
    unsigned int bargeBlockedNum;   // number of barge blocked voters
    unsigned int pictureVotes;      // total votes for picture tour
    unsigned int statueVotes;       // total votes for statue tour
    unsigned int giftshopVotes;     // total votes for gift shop tour
    Printer & printer;              // printer
    uOwnerLock mutexLock;
    uCondLock groupCondLock;
    uCondLock bargeLock;            // lock used to prevent barging
    bool isTaken;                   // flag to indicae signalling
#elif defined( SEM )                // semaphore solution
#include <uSemaphore.h>
class TallyVotes {
    unsigned int totalVoterNum;     // total number of voters
    unsigned int groupSize;         // group size
    unsigned int finishedVoterNum;  // already terminated voters
    unsigned int voteNum;           // number of votes done in current round
    unsigned int pictureVotes;      // total votes for picture tour
    unsigned int statueVotes;       // total votes for statue tour
    unsigned int giftshopVotes;     // total votes for gift shop tour
    Printer & printer;              // printer
    uSemaphore mutexSem;
    uSemaphore groupSyncSem;
#elif defined( BAR )                // barrier solution
#include <uBarrier.h>
_Cormonitor TallyVotes : public uBarrier {
    unsigned int totalVoterNum;     // total number of voters
    unsigned int groupSize;         // group size
    unsigned int finishedVoterNum;  // already terminated voters
    unsigned int voteNum;           // number of votes done in current round
    unsigned int pictureVotes;      // total votes for picture tour
    unsigned int statueVotes;       // total votes for statue tour
    unsigned int giftshopVotes;     // total votes for gift shop tour
    Printer & printer;              // printer
#else
    #error unsupported voter type
#endif
    // common declarations
  public:                            // common interface
    _Event Failed {};
    TallyVotes( unsigned int voters, unsigned int group, Printer & printer );
    struct Ballot { unsigned int picture, statue, giftshop; };
    enum TourKind { Picture = 'p', Statue = 's', GiftShop = 'g' };
    struct Tour { TourKind tourkind; unsigned int groupno; };
    Tour vote( unsigned int id, Ballot ballot );
    void done(
    #if defined( BAR )               // barrier solution
        unsigned int id
    #endif
    );
};

#endif