#include "q2tallyVotes.h"
#include "q2printer.h"

int groupNum = 0;

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ):
    totalVoterNum(voters), groupSize(group), finishedVoterNum(0), voteNum(0), 
    pictureVotes(0), statueVotes(0), giftshopVotes(0), printer(printer), mutexSem(1), groupSyncSem(0) {} // TallyVotes::TallyVotes

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
    // throw error when remaining voters are not able to form a group
    if ( totalVoterNum - finishedVoterNum < groupSize ) {
        _Throw Failed();
    } // if

    Tour winTour;
    bool printComplete = false; // flag variable to determine if "Complete" message should be printed
    mutexSem.P();
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
        printer.print( id, Voter::States::Block, voteNum );
        mutexSem.V();
        groupSyncSem.P();                   // block by synchronization semaphore
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

    if ( !groupSyncSem.empty() ) {          // wake up blocked voters
        groupSyncSem.V();
    } else {
        // reset vote records and increment group number
        pictureVotes = 0;
        statueVotes = 0;
        giftshopVotes = 0;
        mutexSem.V();
    } // if
    return winTour;
} // TallyVotes::vote

void TallyVotes::done() {
    mutexSem.P();
    finishedVoterNum += 1;
    if ( totalVoterNum - finishedVoterNum < groupSize ) {
        // wake up all blocked voters so they can throw Failed exception and return
        groupSyncSem.V();
    } // if
    mutexSem.V();
} // TallyVotes::done