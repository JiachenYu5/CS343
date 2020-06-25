#include "student.h"
#include "MPRNG.h"
#include <cmath>
using namespace std;

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, 
        unsigned int id, unsigned int numStops, unsigned int stopCost, unsigned int maxStudentDelay, unsigned int maxStudentTrips ) :
    prt( prt ), nameServer( nameServer ), cardOffice( cardOffice ), groupoff( groupoff ), id( id ), numStops( numStops ), 
    stopCost( stopCost ), maxStudentDelay( maxStudentDelay ), maxStudentTrips( maxStudentTrips ), isRejected( false ) {} // Student::Student

Student::~Student() {} // Student::~Student

char Student::findDirection() {
    int halfDistance = numStops / 2;
    if ( abs( distance ) < halfDistance ) {
        if ( distance < 0 ) {
            distance *= -1;
            return '>';
        } else return '<';
    } else if ( abs( distance ) > halfDistance ) {
        if ( distance > 0 ) {
            distance = numStops - distance;
            return '>';
        }
        else {
            distance = numStops - abs( distance );
            return '<';
        }
    } else {
        distance = abs ( distance );
        return '<';
    } // if
} // Student::findDirection

void Student::main() {
    numTrips = mprng( 1, maxStudentTrips );
    unsigned int maxTripCost =  numStops / 2 * stopCost;
    watCard = cardOffice.create( id, maxTripCost );
    giftCard = groupoff.giftCard();
    prt.print( Printer::Kind::Student, id, 'S', numTrips );
    startStop = nameServer.getStop( id, mprng( numStops - 1) );
    endStop = nameServer.getStop( id, mprng( numStops - 1) );
    distance = endStop->getId() - startStop->getId();
    // get the direction of train
    direction = findDirection();
    prt.print( Printer::Kind::Student, id, 'T', startStop->getId(), endStop->getId(), direction );
    bool buyTicket;
    for ( unsigned int i = 0; i < numTrips; i++ ) {
        yield( maxStudentDelay );
        // determine whether the student will buy ticket
        if ( distance == 1 ) {
            buyTicket = ( mprng( 1 ) == 0 );
        } else {
            buyTicket = ( mprng( 9 ) > 2 );
        } // if
        WATCard * card = NULL;
        _Select( giftCard ) {
            try {
                card = giftCard();
                if ( buyTicket ) {
                    startStop->buy( distance, * card );
                    prt.print( Printer::Kind::Student, id, 'G', distance * stopCost, card->getBalance() );
                    giftCard.reset();   // make giftCard unusable
                } else {
                    prt.print( Printer::Kind::Student, id, 'f' );
                } // if
                // wait for train
                prt.print( Printer::Kind::Student, id, 'W', startStop->getId() );
                if ( direction == '<' ) takenTrain = startStop->wait( id, Train::Direction::Clockwise );
                else takenTrain = startStop->wait( id, Train::Direction::CounterClockwise ); // if
                // embark
                prt.print( Printer::Kind::Student, id, 'E', takenTrain->getId() );
                takenTrain->embark( id, endStop->getId(), * card );
                // disembark
                prt.print( Printer::Kind::Student, id, 'D', endStop->getId() );
                endStop->disembark( id );
                card->resetPOP(); // reset proof 
            } catch ( Train::Ejected & ) {
                isRejected = true;
                prt.print( Printer::Kind::Student, id, 'e' );
                break;
            } // try
        } or _Select( watCard ) {
            try {
                card = watCard();
                if ( buyTicket ) {
                    startStop->buy( distance, * card );
                    prt.print( Printer::Kind::Student, id, 'G', distance * stopCost, card->getBalance() );
                    watCard.reset();   // make giftCard unusable
                } else {
                    prt.print( Printer::Kind::Student, id, 'f' );
                } // if
                // wait for train
                prt.print( Printer::Kind::Student, id, 'W', startStop->getId() );
                if ( direction == '<' ) takenTrain = startStop->wait( id, Train::Direction::Clockwise );
                else takenTrain = startStop->wait( id, Train::Direction::CounterClockwise ); // if
                // embark
                prt.print( Printer::Kind::Student, id, 'E', takenTrain->getId() );
                takenTrain->embark( id, endStop->getId(), * card );
                // disembark
                prt.print( Printer::Kind::Student, id, 'D', endStop->getId() );
                endStop->disembark( id );
                card->resetPOP(); // reset proof
            } catch ( TrainStop::Funds & f ) {
                watCard = cardOffice.transfer( id, f.amount + maxTripCost, card );
            } catch ( WATCardOffice::Lost & ) {
                prt.print( Printer::Kind::Student, id, 'L' );
                watCard = cardOffice.create( id, maxTripCost );
            } catch ( Train::Ejected & ) {
                isRejected = true;
                prt.print( Printer::Kind::Student, id, 'e' );
                break;
            } // try
        } // _Select
        if ( i < numTrips - 1 ) { // if the student finished all trips, don't initialize another trip
            startStop = endStop;
            endStop = nameServer.getStop( id, mprng( numStops - 1) );
            distance = endStop->getId() - startStop->getId();
            direction = findDirection();
            prt.print( Printer::Kind::Student, id, 'T', startStop->getId(), endStop->getId(), direction );
        } // if
    } // for
    if ( isRejected ) prt.print( Printer::Kind::Student, id, 'e' ); // if
    prt.print( Printer::Kind::Student, id, 'F' );
} // Student::main