#include "timer.h"
using namespace std;

Timer::Timer( Printer & prt, NameServer & nameServer, unsigned int timerDelay ) :
    prt( prt ), nameServer( nameServer ), timerDelay( timerDelay ) {
    trainStopList = nameServer.getStopList();
    numStops = nameServer.getNumStops();
} // Timer::Timer

Timer::~Timer() {} // Timer::~Timer

void Timer::main() {
    prt.print( Printer::Kind::Timer, 'S' );
    unsigned int count = 0;
    for ( ; ; ) {
        _Accept( ~Timer ) {
            break;
        } _Else {
            yield( timerDelay );
            prt.print( Printer::Kind::Timer, 't', count );
            for ( unsigned int i = 0; i < numStops; i++ ) {
                trainStopList[i]->tick();
            } // for
            count += 1;
        } // _Accept
    } // for
    prt.print( Printer::Kind::Timer, 'F' );
} // Timer::main