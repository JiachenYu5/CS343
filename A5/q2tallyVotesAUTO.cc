#include "q2tallyVotes.h"
#include "q2printer.h"

int groupNum = 1;

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ):
    groupDone(false), totalVoterNum(voters), groupSize(group), finishedVoterNum(0), voteNum(0),
    pictureVotes(0), statueVotes(0), giftshopVotes(0), printer(printer) {} // TallyVotes::TallyVotes

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
    // throw error when remaining voters are not able to form a group
    if ( totalVoterNum - finishedVoterNum < groupSize ) {
        EXIT();
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

        // set flag
        groupDone = true;
    } else {    // group is not formed yet
        WAITUNTIL( groupDone, printer.print( id, Voter::States::Block, voteNum ), voteNum -= 1; printer.print( id, Voter::States::Unblock, voteNum ) );
    } // if
    
    // throw error when remaining voters are not able to form a group
    if ( totalVoterNum - finishedVoterNum < groupSize ) {
        EXIT();
        _Throw Failed();
    } // if

    // if this is the last voter for current group, reset flag for next group
    if ( voteNum == 0 ) groupDone = false; // if

    EXIT();
    return winTour;
} // TallyVotes::tour

void TallyVotes::done() {
    finishedVoterNum += 1;
    if ( totalVoterNum - finishedVoterNum < groupSize ) {
        groupDone = true;
        EXIT(); // wake up blocked voters
    }
} // TallyVotes::done