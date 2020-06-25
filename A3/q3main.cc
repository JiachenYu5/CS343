#include "MPRNG.h"
#include "q3buffer.h"
#include "q3consumer.h"
#include "q3producer.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

MPRNG mprng; 

// help method for printing error message
void message( char * argv[] ) {
    cerr << "Usage: " << argv[0] << " [ Cons >= 0 | [ Prods >= 0 | [ Produce >= 0 | "
    << "[ BufferSize >= 0 | [ Delay >= 0 ] ] ] ] ]" << endl;
    exit ( EXIT_FAILURE );
}

// help method for reading commandline input and call message if error when parsing
int parseArgument( char * argv[], int index ) {
    int returnVal = -1;
    string argvName;
    if ( index == 1 ) argvName = "Cons";
    else if ( index == 2 ) argvName = "Prods";
    else if ( index == 3 ) argvName = "Produce";
    else if ( index == 4 ) argvName = "BufferSize";
    else argvName = "Delay"; // if

    try {
        returnVal = stoi( argv[index] );
        if ( returnVal <= 0 ) throw invalid_argument(""); // if
    } catch ( const invalid_argument & ia ) {
        cerr << argvName << " needs to be a positive integer." << endl;
        message( argv );
    } // try
    return returnVal;
}

int main( int argc, char * argv[] ) {
    int sum = 0, Cons = 5, Prods = 3, Produce = 10, BufferSize = 10, Delay = 0;
    switch ( argc ) {
        case 6:
            Delay = parseArgument( argv, 5 );
            // FALL THROUGH
        case 5:
            BufferSize = parseArgument( argv, 4 );
            // FALL THROUGH
        case 4:
            Produce = parseArgument( argv, 3 );
            // FALL THROUGH
        case 3:
            Prods = parseArgument( argv, 2 );
            // FALL THROUGH
        case 2:
            Cons = parseArgument( argv, 1 );
            // FALL THROUGH
        case 1:
            break;
        default:
            cerr << "Incorrect number of arguments" << endl;
            exit ( EXIT_FAILURE );
    } // switch
    if ( Delay == 0 ) Delay = Cons + Prods; // if

    #ifdef __U_MULTI__
    uProcessor p[3] __attribute__(( unused )); // create 3 kernel thread for a total of 4
    #endif

    // Create buffer
    BoundedBuffer<int> buffer(BufferSize);

    // Create and initialize producers
    vector<Producer*> producers;
    for ( int i = 0; i < Prods; i++ ) {
        producers.push_back( new Producer( buffer, Produce, Delay ) );
    } // for

    // Create and initialize consumers
    vector<Consumer*> consumers;
    for ( int i = 0; i < Cons; i++ ) {
        consumers.push_back( new Consumer( buffer, Delay, SENTINEL, sum ) );
    } // for

    // clean producers
    for ( int i = 0; i < Prods; i++ ) {
        delete producers[i];
    } // for

    // insert end value to stop consumers
    for ( int i = 0; i < Cons; i++ ) {
        buffer.insert(SENTINEL);
    } // for

    // clean consumers
    for ( int i = 0; i < Cons; i++ ) {
        delete consumers[i];
    } // for

    // print result
    cout << "total: " << sum << endl;
}