#include "q2tallyVotes.h"
#include "q2printer.h"

int groupNum = 0;

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ):
    uBarrier( group ), totalVoterNum( voters ), groupSize( group ), finishedVoterNum( 0 ), voteNum( 0 ), 
    pictureVotes( 0 ), statueVotes( 0 ), giftshopVotes( 0 ), printer( printer ) {} // TallyVotes::TallyVotes

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
    // throw error when remaining voters are not able to form a group
    if ( totalVoterNum - finishedVoterNum < groupSize ) {
        uBarrier::block();      // wait for other voters 
        _Throw Failed();
    } // if

    Tour winTour;
    bool printComplete = false; // flag variable to determine if "Complete" message should be printed
    printer.print( id, Voter::States::Vote, ballot );
    // update vote records
    pictureVotes += ballot.picture;
    statueVotes += ballot.statue;
    giftshopVotes += ballot.giftshop;

    if ( uBarrier::waiters() == groupSize - 1 ) {           // all votes are done ( previous waiting + 1 = groupSize )
        uBarrier::block();                                  // wait for other voters
        printComplete = true;               // don't print as the winning tour hasn't been determined yet
        groupNum += 1;      // update group number
    } else {
        printer.print( id, Voter::States::Block, uBarrier::waiters() + 1 );
        uBarrier::block();                                  // // wait for other voters
        printer.print( id, Voter::States::Unblock, uBarrier::waiters() );
    } // if

    if ( totalVoterNum - finishedVoterNum < groupSize ) {
        _Throw Failed();
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
    
    voteNum += 1;                       // update vote number
    if ( voteNum == groupSize ) {       // all votes done
        // reset vote records and increment group number
        pictureVotes = 0;
        statueVotes = 0;
        giftshopVotes = 0;
        voteNum = 0;
    } // if
    return winTour;
} // TallyVotes::vote

void TallyVotes::done( unsigned int id ) {
    finishedVoterNum += 1;
    if ( totalVoterNum - finishedVoterNum < groupSize ) {
        // make sure all remaining voters all throw Failed exception and return
        uBarrier::block();
        printer.print( id, Voter::States::Done );
    } // if
} // TallyVotes::done