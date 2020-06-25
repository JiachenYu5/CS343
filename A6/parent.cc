#include "parent.h"
#include "MPRNG.h"
#include <algorithm>
#include <iostream>
using namespace std;

extern MPRNG mprng;

Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay, unsigned int maxTripCost ) :
    prt( prt ), bank( bank ), numStudents( numStudents ), parentalDelay( parentalDelay ), maxTripCost( maxTripCost ) {} // Parent::Parent

void Parent::main() {
    prt.print( Printer::Kind::Parent, 'S' );
    for ( ; ; ) { 
        _Accept( ~Parent ) {
            break;
        } _Else {
            yield( parentalDelay ); // yield before giving money
            unsigned int pickedStudent = mprng( numStudents - 1 ); // get the student that accept the gift
            unsigned int gift = ( unsigned int )max(1, ( int )( mprng( 1, 3 ) * maxTripCost / 3 ) ); // get the amount of money
            prt.print( Printer::Kind::Parent, 'D', pickedStudent, gift); // print info
            bank.deposit( pickedStudent, gift ); // deposit money
        } // _Accept
    } // for
    prt.print( Printer::Kind::Parent, 'F' );
} // Parent::main