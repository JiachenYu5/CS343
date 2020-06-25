#include "q2tallyVotes.h"
#include "q2printer.h"

int groupNum = 1;

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ):
    ticketNum(1), allowedTicketNum(group), bargeBlockedNum(0), totalVoterNum(voters), groupSize(group), finishedVoterNum(0),
    voteNum(0), pictureVotes(0), statueVotes(0), giftshopVotes(0), printer(printer) {} // TallyVotes::TallyVotes

void TallyVotes::wait() {
    bench.wait();                            // wait until signalled
    while ( rand() % 2 == 0 ) {              // multiple bargers allowed
        try {
            _Accept( vote, done ) {          // accept barging callers
            } _Else {                        // do not wait if no callers
            } // _Accept
        } catch( uMutexFailure::RendezvousFailure & ) {}
    } // while
}

void TallyVotes::signalAll() {               // also useful
    while ( ! bench.empty() ) bench.signal();// drain the condition
}

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
    // throw error when remaining voters are not able to form a group
    if ( totalVoterNum - finishedVoterNum < groupSize ) {
        _Throw Failed();
    } // if

    // assign current ticket
    unsigned int curTicket = ticketNum;
    ticketNum += 1;

    // if current ticket exceeds ticket count limit, block it
    while ( curTicket > allowedTicketNum ) { // use while loop to recheck if awake barging voters should enter
        bargeBlockedNum += 1;
        printer.print( id, Voter::States::Barging, bargeBlockedNum );
        wait();
        bargeBlockedNum -= 1;
    } // if

    // check if allowed barging voters can form a group
    if ( totalVoterNum - finishedVoterNum < groupSize ) {
        _Throw Failed();
    } // if

    printer.print( id, Voter::States::Vote, ballot );
    // update vote records
    voteNum += 1;
    pictureVotes += ballot.picture;
    statueVotes += ballot.statue;
    giftshopVotes += ballot.giftshop;

    if ( voteNum == groupSize ) {   // all votes are done, print complete message
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

        printer.print( id, Voter::States::Complete, winTour );
        voteNum -= 1;

        // reset vote records and increment group number
        pictureVotes = 0;
        statueVotes = 0;
        giftshopVotes = 0;
        groupNum += 1;

        // wake up blocked voters
        signalAll();
    } else {    // group is not formed yet
        // blocking the voter, printing messages and decrement voteNum
        printer.print( id, Voter::States::Block, voteNum );
        wait();
        voteNum -= 1;
        printer.print( id, Voter::States::Unblock, voteNum );
    } // if

    if (voteNum == 0) { // current group voting is done
        allowedTicketNum += groupSize; // increment ticket buffer size by a group
        signalAll(); // allow blocked voters to enter
    } // if
    
    return winTour;
} // TallyVotes::tour

void TallyVotes::done() {
    finishedVoterNum += 1;
    if ( totalVoterNum - finishedVoterNum < groupSize ) signalAll(); // if
} // TallyVotes::done