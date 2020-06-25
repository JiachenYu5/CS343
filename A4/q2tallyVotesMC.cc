#include "q2tallyVotes.h"
#include "q2printer.h"

int groupNum = 0;

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ):
    totalVoterNum(voters), groupSize(group), finishedVoterNum(0), voteNum(0), bargeBlockedNum(0),
    pictureVotes(0), statueVotes(0), giftshopVotes(0), printer(printer), isTaken(false) {} // TallyVotes::TallyVotes

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
    // throw error when remaining voters are not able to form a group
    if ( totalVoterNum - finishedVoterNum < groupSize ) {
        _Throw Failed();
    } // if

    Tour winTour;               // return value
    bool printComplete = false; // flag variable to determine if "Complete" message should be printed
    mutexLock.acquire();
    try {
        if ( isTaken ) {    // if barging, force outcoming voters to wait
            bargeBlockedNum += 1;
            printer.print( id, Voter::States::Barging, bargeBlockedNum );
            bargeLock.wait( mutexLock );
            bargeBlockedNum -= 1;
        } // if
        printer.print( id, Voter::States::Vote, ballot );
        // update vote records
        voteNum += 1;
        pictureVotes += ballot.picture;
        statueVotes += ballot.statue;
        giftshopVotes += ballot.giftshop;

        if ( voteNum == groupSize ) {           // all votes are done
            printComplete = true;               // don't print as the winning tour hasn't been determined yet
            groupNum += 1;      // update group number
            voteNum--;
        } else {
            if ( bargeLock.empty() ) {          // allow outside voters to come in
                isTaken = false;
            } else {
                isTaken = true;
                bargeLock.signal();             // wait up blocked bargeing voters
            } // if
            printer.print( id, Voter::States::Block, voteNum );
            groupCondLock.wait( mutexLock );    // block by synchronization lock

            voteNum--;
            printer.print( id, Voter::States::Unblock, voteNum );
        } // if
        
        // get the information for winning tour
        TourKind winTourKind;
        if ( giftshopVotes >= pictureVotes && giftshopVotes >= statueVotes )
            winTourKind = TourKind::GiftShop;
        else if ( pictureVotes >= giftshopVotes && pictureVotes >= statueVotes )
            winTourKind =  TourKind::Picture;
        else
            winTourKind =  TourKind::Statue; // if
        winTour.tourkind = winTourKind;
        winTour.groupno = groupNum;
        // print complete message if the voter is the last one to vote
        if ( printComplete ) {
            printer.print( id, Voter::States::Complete, winTour ); // if
        }

        if ( !groupCondLock.empty() ) {         // wake up blocked voters
            isTaken = true;
            groupCondLock.signal();
        } else if ( !bargeLock.empty() ) {      // wake up blocked barging voters
            isTaken = true;
            bargeLock.signal();
            // reset vote records and increment group number
            pictureVotes = 0;
            statueVotes = 0;
            giftshopVotes = 0;
        } else {
            isTaken = false;                    // allow outside voters to come in
            // reset vote records and increment group number
            pictureVotes = 0;
            statueVotes = 0;
            giftshopVotes = 0;
        } // if
    }   _Finally {
        mutexLock.release();
    } // try
    return winTour;
} // TallyVotes::tour

void TallyVotes::done() {
    mutexLock.acquire();
    finishedVoterNum += 1;
    if ( totalVoterNum - finishedVoterNum < groupSize && ( !bargeLock.empty() || !groupCondLock.empty() ) ) {
        // wake up all blocked voters so they can throw Failed exception and return
        if ( !bargeLock.empty() ) bargeLock.signal();
        else groupCondLock.signal();
    } // if
    mutexLock.release();
} // TallyVotes::done