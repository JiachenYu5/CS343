#include "conductor.h"
using namespace std;

Conductor::Conductor( Printer & prt, unsigned int id, Train * train, unsigned int delay ) :
    prt( prt ), id( id ), train( train ), conductorDelay( delay ) {} // Conductor::Conductor

Conductor::~Conductor() {} // Conductor::Conductor

void Conductor::main() {
    prt.print( Printer::Kind::Conductor, id, 'S' );
    for ( ; ; ) {
        _Accept( ~Conductor ) {
            break;
        } _Else {
            yield( conductorDelay );
            prt.print( Printer::Kind::Conductor, id, 'c' );
            train->scanPassengers();
        } // _Accept
    } // for
    prt.print( Printer::Kind::Conductor, id, 'F' );
} // Conductor::main