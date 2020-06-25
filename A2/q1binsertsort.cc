#include "q1binsertsort.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// non-basic structure for testing
struct DoubleIntPairs{
    int x;
    int y;
    DoubleIntPairs( int x, int y ) : x(x), y(y) {}
    DoubleIntPairs() : x(0), y(0) {}
};

bool operator==( const DoubleIntPairs & pairA, const DoubleIntPairs & pairB ) {
    return ( pairA.x == pairB.x && pairA.y == pairB.y );
}

bool operator<( const DoubleIntPairs & pairA, const DoubleIntPairs & pairB ) {
    return ( pairA.x < pairB.x || pairA.y < pairB.y );
}

istream & operator>>( istream & is, DoubleIntPairs & pair ) {
    is >> pair.x;
    pair.y = 0;
    return is;
}

ostream & operator<<( ostream & os, const DoubleIntPairs & pair ) {
    os << pair.x << "," << pair.y;
    return os;
}

template <typename T>
void Binsertsort<T>::sort( T value ) {          // value to be sorted
    Binsertsort::value = value;
    resume();
} // Binsertsort<T>::sort

template <typename T>
T Binsertsort<T>::retrieve() {                  // retrieve sorted value
    resume();
    return value;
} // Binsertsort<T>::retrieve

template <typename T>
void Binsertsort<T>::main() {
    pivot = value;
    try {
        suspend();                      // return for next value
    } catch( Sentinel & ) {             // end of value set
        // implies leaf node
        value = pivot;                  
        suspend();
        _Resume Sentinel() _At resumer(); // tell parent tree that this leaf node's value has been retrieved
        suspend();
    } // try

    Binsertsort<T> less, greater;
    try {
        for ( ; ; ) {
            _Enable {
                if (value < pivot) {
                    hasless = true;
                    less.sort(value);
                } else {
                    hasgreater = true;
                    greater.sort(value);
                } // if
                suspend();
            }
        } // for
    } catch ( Sentinel & ) {
        // end of list input reached
        if (hasless) _Resume Sentinel() _At less; // if
        if (hasgreater) _Resume Sentinel() _At greater; // if
    } // try
    
    // retrieve less 
    while ( hasless ) {
        try {
            _Enable {
                value = less.retrieve();
                suspend();
            }
        } catch ( Sentinel & ) {
            hasless = false;
        } // try
    } // while

    // retrieve root
    value = pivot;
    suspend();
    
    // retrieve greater
    while ( hasgreater ) {
        try {
            _Enable {
                value = greater.retrieve();
                suspend();
            }
        } catch ( Sentinel & ) {
            hasgreater = false;
        } // try
    } // while
    
    _Resume Sentinel() _At resumer();                   // indicates this node's value has been retrieved
    suspend();
} // Binsertsort<T>::main

int main( int argc, char * argv[] ) {
    istream * unsortedFile = &cin;				// default value for input file
    ostream * sortedFile = &cout;				// default value for output file
    switch( argc ) {
        case 3:
            try {
                sortedFile = new ofstream( argv[2] );
            } catch (uFile::Failure &) {
                cerr << "Error! Could not open output file \"" << argv[2] << "\"" << endl;
                exit ( EXIT_FAILURE );
            } // try
            // FALL THROUGH
        case 2:
            try {
                unsortedFile = new ifstream( argv[1] );
            } catch (uFile::Failure &) {
                cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
                exit ( EXIT_FAILURE );
            } // try
            break;
        default:
            cerr << "Incorrect number of arguments." << endl;
            exit ( EXIT_FAILURE );
    } // switch

    int nodeCount;
    TYPE val;
    for( ; ; ) {
        *unsortedFile >> nodeCount;                                                        
        Binsertsort<TYPE> tree;  

        if( unsortedFile->fail() ) break; // if                                                 

        for ( int i = 0; i < nodeCount; i++ ) {
            if ( i > 0 ) *sortedFile << " "; // if
            *unsortedFile >> val;  
            if ( unsortedFile->fail() ) {
                cerr << "read failed" << endl;
                return -1;
            } // if
            *sortedFile << val; 
            tree.sort( val );
        } // for
        *sortedFile << endl;
        _Resume Binsertsort<TYPE>::Sentinel() _At tree;

        for ( int i = 0; i < nodeCount; i++ ) {
            if ( i > 0 ) *sortedFile << " "; // if
            try {
                _Enable {
                    *sortedFile << tree.retrieve(); 
                }
            } _CatchResume( Binsertsort<TYPE>::Sentinel & ) {
            } // try
        } // for
        *sortedFile << endl << endl;
    }

    delete unsortedFile;		                        // close file
    if ( sortedFile != &cout ) delete sortedFile; // if // close file, do not delete cout!
}