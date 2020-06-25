#include "q2tallyVotes.h"
#include "q2voter.h"
#include "q2printer.h"
#include "MPRNG.h"
#include <string>
#include <iostream>
using namespace std;

MPRNG mprng;

// help method for printing error message
void message( char * argv[] ) {
    cerr << "Usage: " << argv[0] << " [ voters > 0 | [ group > 0 | [ votes > 0 | "
    << "[ seed > 0 | [ processors > 0 ] ] ] ] ]" << endl;
    exit ( EXIT_FAILURE );
}

// help method for reading commandline input and call message if error when parsing
int parseArgument( char * argv[], int index ) {
    int returnVal = -1;
    string argvName;
    if ( index == 1 ) argvName = "Voters";
    else if ( index == 2 ) argvName = "Group";
    else if ( index == 3 ) argvName = "Votes";
    else if ( index == 4 ) argvName = "Seed";
    else argvName = "Processors"; // if

    try {
        if ( strcmp( argv[index], "d" ) == 0 ) {
            if ( index == 1 ) returnVal = 6;
            else if ( index == 2 ) returnVal = 3;
            else if ( index == 3 ) returnVal = 1;
            else if ( index == 4 ) returnVal = getpid();
            else returnVal = 1; // if
        } else {
            returnVal = stoi( argv[index] );
            if ( returnVal <= 0 ) throw invalid_argument(""); // if
        } // if 
    } catch ( const invalid_argument & ia ) {
        cerr << argvName << " needs to be a positive integer." << endl;
        message( argv );
    } // try
    return returnVal;
}

int main( int argc, char * argv[] ) {
    int voters = 6, group = 3, votes = 1, seed = getpid(), processors = 1;
    switch ( argc ) {
        case 6:
            processors = parseArgument( argv, 5 );
            // FALL THROUGH
        case 5:
            seed = parseArgument( argv, 4 );
            // FALL THROUGH
        case 4:
            votes = parseArgument( argv, 3 );
            // FALL THROUGH
        case 3:
            group = parseArgument( argv, 2 );
            // FALL THROUGH
        case 2:
            voters = parseArgument( argv, 1 );
            // FALL THROUGH
        case 1:
            break;
        default:
            cerr << "Incorrect number of arguments" << endl;
            exit ( EXIT_FAILURE );
    } // switch
    uProcessor p[processors - 1]; // number of kernel threads

    mprng.set_seed( seed );       // set seed
    // create Voter, Printer and Tallyvote objects
    Voter *voterList[voters];
    Printer printer( voters );
    TallyVotes voteTallier(voters, group, printer);

    // initialize all Voter tasks
    for ( int i = 0; i < voters; i++ ) {
        voterList[i] = new Voter( i, votes, voteTallier, printer );
    } // for

    // wait for all voter tasks to complete
    for ( int i = 0; i < voters; i++ ) {
        delete voterList[i];
    } // for
}