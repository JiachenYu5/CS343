#include "train.h"
#include "trainstop.h"
#include "nameserver.h"
#include <cmath>
using namespace std;

Train::Train( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int maxNumStudents, unsigned int numStops ) : 
    prt( prt ), nameServer( nameServer ), id( id ), maxNumStudents( maxNumStudents ), numStops( numStops ), curNumStudents( 0 ) {
    trainStopList = nameServer.getStopList( id );
} // Train::Train

Train::~Train() {
} // Train::~Train

_Nomutex unsigned int Train::getId() const {
    return id;
} // Train::getId
 
TrainStop * Train::embark( unsigned int studentId, unsigned int destStop, WATCard & card ) {
    curNumStudents += 1;
    studentIdList.push_back( studentId );
    destStopList.push_back( destStop );
    cardList.push_back( card.paidForTicket() );
    //studentAddress.push_back( static_cast<Student *>( &uThisTask() ) );
    prt.print( Printer::Kind::Train, id, 'E', studentId, curStop->getId() );
    return curStop;
} // Train::embark

void Train::scanPassengers() {
    for ( unsigned int i = 0; i < curNumStudents; i++ ) {
        if ( !cardList[i] ) {
            ejectList.push_back( i );   // add to ejectList
        } // if
    } // for
} // Train::scanPassengers

void Train::main() {
    // determine direction
    if ( id == 0 ) {
        direction = Direction::Clockwise;
        prt.print( Printer::Kind::Train, id, 'S', 0, '<' );
        unsigned int loopIndex = 0;
        for ( ; ; ) {
            if ( loopIndex == numStops ) loopIndex = 0;
            unsigned int index, counter = 0;
            _Accept( ~Train ) {
                break;
            } _Else {
                while ( !ejectList.empty() ) { // eject students (not working)
                    index = ejectList[0];
                    _Resume Ejected() _At resumer();
                    ejectList.erase( ejectList.begin() );
                    studentIdList.erase( studentIdList.begin() + index );
                    destStopList.erase( destStopList.begin() + index );
                    cardList.erase( cardList.begin() + index );
                    curNumStudents -= 1;
                } // while
                curStop = trainStopList[loopIndex]; // get current stop
                unsigned int onboard = curStop->arrive( id, direction, maxNumStudents - curNumStudents ); // arrive
                prt.print( Printer::Kind::Train, id, 'A', curStop->getId(), maxNumStudents - curNumStudents, curNumStudents ); // print arrive message
                for ( unsigned int i = 0; i < onboard; i++ ) { // allow student embark
                    _Accept( embark );
                } // for
                // remove students arrive at their destination stop
                while ( counter < destStopList.size() ) {
                    if ( destStopList[counter] == curStop->getId() ) {
                        studentIdList.erase( studentIdList.begin() + counter );
                        destStopList.erase( destStopList.begin() + counter );
                        cardList.erase( cardList.begin() + counter );
                        curNumStudents -= 1;
                    } else {
                        counter += 1;
                    } // if
                } // while
                _Accept( scanPassengers );
                loopIndex += 1;
            } // _Accept
        } // for
    } else {
        direction = Direction::CounterClockwise;
        prt.print( Printer::Kind::Train, id, 'S', numStops / 2, '>' );
        unsigned int loopIndex = 0;
        for ( ; ; ) {
            if ( loopIndex == numStops ) loopIndex = 0;
            unsigned int index, counter = 0;
            _Accept( ~Train ) {
                break;
            } _Else {
                while ( !ejectList.empty() ) { // eject students (not working)
                    index = ejectList[0];
                    _Resume Ejected() _At resumer();
                    ejectList.erase( ejectList.begin() );
                    studentIdList.erase( studentIdList.begin() + index );
                    destStopList.erase( destStopList.begin() + index );
                    cardList.erase( cardList.begin() + index );
                    curNumStudents -= 1;
                } // while
                if ( loopIndex < numStops - numStops / 2 ) {
                    curStop = trainStopList[numStops / 2 + loopIndex];
                } else {
                    curStop = trainStopList[loopIndex - ( unsigned int )ceil( numStops / 2 )];
                } // if
                unsigned int onboard = curStop->arrive( id, direction, maxNumStudents - curNumStudents ); // arrive
                prt.print( Printer::Kind::Train, id, 'A', curStop->getId(), maxNumStudents - curNumStudents, curNumStudents ); // print arrive message
                for ( unsigned int i = 0; i < onboard; i++ ) { // allow student embark
                    _Accept( embark );
                } // for
                // remove students arrive at their destination stop
                while ( counter < destStopList.size() ) {
                    if ( destStopList[counter] == curStop->getId() ) {
                        studentIdList.erase( studentIdList.begin() + counter );
                        destStopList.erase( destStopList.begin() + counter );
                        cardList.erase( cardList.begin() + counter );
                        curNumStudents -= 1;
                        //studentAddress.erase( studentAddress.begin() + counter );
                    } else {
                        counter += 1;
                    } // if
                } // while
                _Accept( scanPassengers );
                loopIndex += 1;
            } // _Accept
        } // for
    } // if
    prt.print( Printer::Kind::Train, id, 'F' );
} // Train::main