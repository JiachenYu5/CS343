#include "q2tallyVotes.h"
#include "q2printer.h"

int groupNum = 1;

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ):
    hasFailed(false), totalVoterNum(voters), groupSize(group), finishedVoterNum(0), voteNum(0),
    pictureVotes(0), statueVotes(0), giftshopVotes(0), printer(printer) {} // TallyVotes::TallyVotes

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
    // store information in private members
    curVoterID = id;
    curBallot = ballot;
    // wait for group to be formed
    bench.wait(curVoterID); // pass the voter ID so when printin unblocking message, we know who we are unblocking
    // return winning tour result or throw Failed error if the remaining voters cannot form a group
    if ( hasFailed ) _Throw Failed(); // if
    return winTour;
} // TallyVotes::tour

void TallyVotes::done() {
    finishedVoterNum += 1;
} // TallyVotes::done

void TallyVotes::main() {
    for ( ; ; ) {
        _Accept( ~TallyVotes ) {
            return; // end current task when calling destructor
        } or _Accept( vote ) {
            // start the vote
            printer.print( curVoterID, Voter::States::Vote, curBallot );
    
            // update vote records
            voteNum += 1;
            pictureVotes += curBallot.picture;
            statueVotes += curBallot.statue;
            giftshopVotes += curBallot.giftshop;

            // print block message
            printer.print( curVoterID, Voter::States::Block, voteNum );
            // throw error when remaining voters are not able to form a group
            if ( totalVoterNum - finishedVoterNum < groupSize ) {
                hasFailed = true;

            } else {
            if ( voteNum == groupSize ) {           // group is formed, print complete message, otherwise, keep looping
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

                printer.print( curVoterID, Voter::States::Complete, winTour );

                // reset vote records and increment group number
                pictureVotes = 0;
                statueVotes = 0;
                giftshopVotes = 0;
                groupNum += 1;

                // wake up all blocked voters
                while ( !bench.empty() ) {
                    voteNum -= 1;
                    printer.print( bench.front(), Voter::States::Unblock, voteNum );
                    bench.signalBlock();
                } // while
            } // if
            }
        } or _Accept( done ) {
            printer.print( curVoterID, Voter::States::Done );
            // set the error flag and wake up remaining tasks when remaining voters are not able to form a group
            if ( totalVoterNum - finishedVoterNum < groupSize ) {
                hasFailed = true; // if
                if ( !bench.empty() ) {
                    curVoterID = bench.front(); // update voter ID to print done
                    bench.signalBlock(); // while
                }
            }
        } // _Accept
    }
}