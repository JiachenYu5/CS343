#include "q2voter.h"
#include "q2printer.h"

void Voter::main() {
    yield( mprng( 19 ) );
    for ( unsigned int i = 0; i < nvotes; i++ ) {
        printer.print( id, States::Start );
        yield( mprng( 4 ) );
        TallyVotes::Ballot ballot = cast();
        TallyVotes::Tour tour;
        try {
            _Enable {
                tour = voteTallier.vote( id, ballot );      // get the tour info
            } // _Enable
        } catch ( TallyVotes::Failed & ) {
            // catch Failed exception and print failed message and return
            printer.print( id, States::Failed );
            #if defined( BAR )               // barrier solution
                printer.print( id, Voter::States::Done );
            #endifx
            printer.print( id, States::Terminated );
            return;
        } // try
        yield( mprng ( 4 ) );
        printer.print( id, States::Going, tour );
    } // for
    voteTallier.done(
    #if defined( BAR )               // barrier solution
        id
    #endif
    );
    printer.print( id, States::Terminated );
} // Voter::main

Voter::Voter( unsigned int id, unsigned int nvotes, TallyVotes & voteTallier, Printer & printer ) :
    id(id), nvotes(nvotes), voteTallier(voteTallier), printer(printer) {} // Voter::Voter