#include "q2polymult.h"
#include <uCobegin.h>
#include <uActor.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
using namespace std;

// help function that calculate the coefficient for given index
int calculateCoef( const poly_t & a, const poly_t & b, int index ) {
    int sum = 0;
    int aCounter = max(index - a.degree + 1, 0);
    int bCounter = index - aCounter;
    while ( aCounter < a.degree && (aCounter + bCounter) <= index) {
        sum += a.values[aCounter] * b.values[bCounter];
        aCounter++;
        bCounter = max(index - aCounter, 0);
    } // while
    return sum;
}

#ifdef CFOR
void polymultiply( const poly_t & a, const poly_t & b, poly_t & c, const size_t delta ) {
    int *Coef = c.values;
    int taskNum = ceil( (double)c.degree / delta );
    COFOR(i, 0, delta,
        for (int j = 0; j < taskNum; j++) {
            int index = j * delta + i;
            if ( index >= c.degree ) break; // if
            Coef[index] = calculateCoef(a, b, index);
        } // for
    ); // COFOR
}
#endif

#ifdef ACTOR
struct PolyMsg : public uActor::Message {
    int i;                      // current task number 
    size_t delta;
    int taskNum;                // the number of coeffs the actor needs to calculate
    const poly_t & a;
    const poly_t & b;
    poly_t & c;
    PolyMsg( int i, size_t delta, int taskNum, const poly_t & a, const poly_t & b, poly_t & c) 
        : Message( uActor::Delete ), i( i ), delta( delta ), taskNum(taskNum), a( a ), b( b ), c( c ) {} 
};

_Actor PolyCoeff {
    Allocation receive( Message & msg ) {
        Case( PolyMsg, msg ) { 
            for ( int j = 0; j < msg_d->taskNum; j++ ) {
                int index = j * msg_d->delta + msg_d->i;
                if ( index >= msg_d->c.degree) break;
                msg_d->c.values[index] = calculateCoef(msg_d->a, msg_d->b, index);
            } // for
        }
        return Finished;
    }
};

void polymultiply( const poly_t & a, const poly_t & b, poly_t & c, const size_t delta ) {
    int taskNum = ceil( (double)c.degree / delta );
    uActorStart();
    PolyCoeff coeffs[delta];
    for ( int i = 0; i < (int)delta; i++ ) {
        PolyMsg *msg = new PolyMsg(i, delta, taskNum, a, b, c);
        coeffs[i] | *msg;
    } // for
    uActorStop();
}
#endif

#ifdef TASK
Multiply::Multiply( const poly_t & a, const poly_t & b, poly_t & c, const size_t startIndex, const size_t endIndex, const size_t delta )
    : a( a ), b( b ), c( c ), startIndex( startIndex ), endIndex( endIndex ), delta( delta ) {} // Multiply::Multiply

void Multiply::main() {
    if ( startIndex == endIndex ) {
        c.values[startIndex] = calculateCoef( a, b, startIndex );
        return;
    } else if ( startIndex > endIndex ) return; // if
    Multiply( a, b, c, startIndex + delta, endIndex - delta, delta );
    c.values[startIndex] = calculateCoef( a, b, startIndex );
    c.values[endIndex] = calculateCoef( a, b, endIndex );
} // Multiply::main

void polymultiply( const poly_t & a, const poly_t & b, poly_t & c, const size_t delta ) {
    int taskNum = ceil( (double)c.degree / delta );
    int endIndex, defaultEnd;
    for ( int i = 0; i < (int)delta; i++ ) {
        defaultEnd = i + (taskNum - 1) * delta;
        endIndex = ( defaultEnd >= c.degree ) ? defaultEnd - delta : defaultEnd;
        Multiply(a, b, c, i, endIndex, delta);
    } // for
}
#endif


// help method for printing error message
void message( char * argv[] ) {
    cerr << "Usage: " << argv[0] << " [ -p processors (> 0) ][ polyfileA polyfileB | -t numCoeff (> 0) ]" << endl;
    exit( EXIT_FAILURE );
}

// help method for reading commandline input and call message if error when parsing
int parseArgument( char * argv[], int index, int argc ) {
    int returnVal = -1;
    string argvName;
    if ( argc == 3 || ( index == 4 && argc == 5 ) ) argvName = "numCoeff";
    else if ( index == 2 && argc == 5 ) argvName = "processors"; // if

    try {
        returnVal = stoi( argv[index] );
        if ( returnVal <= 0 ) throw invalid_argument(""); // if
    } catch ( const invalid_argument & ia ) {
        cerr << argvName << " needs to be a positive integer." << endl;
        message( argv );
    } // try
    return returnVal;
}

// help method for reading input file and create ploy_t struct from the file
poly_t readFile( const char * fileName ) {
    poly_t result;
    string str;
    stringstream ss;
    int input, counter = 0;
    ifstream inputFile( fileName );
    if ( !inputFile.is_open() ) {
        cerr << "Unable to open the file." << endl;
        exit( EXIT_FAILURE );
    } else {
        getline( inputFile, str );
        ss.str( str );
        ss >> input;
        result.degree = input;
        result.values = new int[result.degree];
        while ( ss >> input ) {
            result.values[counter] = input;
            counter++;
        } // while
        for ( ; ; ) {
            if ( !getline( inputFile, str ) ) break; // if
            ss.clear();
            ss.str( str );
            while ( ss >> input ) {
                result.values[counter] = input;
                counter++;
            } // while
        } // for
    } // if
    return result;
}

int main( int argc, char * argv[] ) {
    int processors = 1, numCoeff;
    bool needPrint = true;
    poly_t polyA, polyB;
    switch( argc ) {
        case 5: 
            if ( strcmp( argv[1], "-p" ) != 0 ) {
                cerr << "Wrong format of commandline input" << endl;
                exit( EXIT_FAILURE );
            } // if
            processors = parseArgument( argv, 2, 5 );
            if ( strcmp( argv[3], "-t" ) == 0 ) {
                numCoeff = parseArgument( argv, 4, 5 );
                needPrint = false;
            } else {
                polyA = readFile( argv[3] );
                polyB = readFile( argv[4] );
            } // if
            break;
        case 3:
            if ( strcmp( argv[1], "-t" ) == 0 ) {
                numCoeff = parseArgument( argv, 2, 3 );
                needPrint = false;
            } else {
                polyA = readFile( argv[1] );
                polyB = readFile( argv[2] );
            } // if
            break;
        default:
            cerr << "Incorrect number of arguments" << endl;
            exit ( EXIT_FAILURE );
    }

    uProcessor p[processors - 1]; // number of kernel threads

    // if no input file specified, initalize polyA and polyB to default value
    if ( !needPrint ) {
        polyA.degree = numCoeff;
        polyB.degree = numCoeff;
        polyA.values = new int[numCoeff];
        polyB.values = new int[numCoeff];
        for ( int i = 0; i < numCoeff; i++ ) {
            polyA.values[i] = i + 1;
            polyB.values[i] = i + 1;
        } // for
    } // if

    // create polyC and calculate coefficients
    poly_t polyC;
    polyC.degree = polyA.degree * 2 - 1;
    polyC.values = new int[polyC.degree];
    const size_t delta = floor( sqrt(polyC.degree) );
    polymultiply( polyA, polyB, polyC, delta);

    // print output if needed
    if ( needPrint ) {
        for ( int i = polyA.degree - 1; i >= 0; i-- ) {
            cout << polyA.values[i] << "x^" << i;
            if ( i != 0 ) cout << " + "; // if
        } // for
        cout << endl;
        for ( int i = polyB.degree - 1; i >= 0; i-- ) {
            cout << polyB.values[i] << "x^" << i;
            if ( i != 0 ) cout << " + "; // if
        } // for
        cout << endl;
        for ( int i = polyC.degree - 1; i >= 0; i-- ) {
            cout << polyC.values[i] << "x^" << i;
            if ( i != 0 ) cout << " + "; // if
        } // for
        cout << endl;
    } // if

    // clean memory
    delete[] polyA.values;
    delete[] polyB.values;
    delete[] polyC.values;
}