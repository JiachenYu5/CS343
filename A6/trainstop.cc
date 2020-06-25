#include "trainstop.h"
using namespace std;

TrainStop::TrainStop( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int stopCost ) :
    prt( prt ), nameServer( nameServer ), id( id ), stopCost( stopCost ), studentWait_1( 0 ), studentWait_2( 0 ) { 
    studentCond1 = new uCondition[100];
    studentCond2 = new uCondition[100];
} // TrainStop::~TrainStop

TrainStop::~TrainStop() {
    delete[] studentCond1;
    delete[] studentCond2;
} // TrainStop::~TrainStop

_Nomutex unsigned int TrainStop::getId() const {
    return id;
} // TrainStop::getId

void TrainStop::buy( unsigned int numStops, WATCard & card ) {
    unsigned int cost = numStops * stopCost;
    if ( card.getBalance() < cost ) { // not enough money in card
        _Throw Funds( cost - card.getBalance() ); // throw exception
    } else { 
        card.withdraw( cost );
        prt.print( Printer::Kind::TrainStop, id, 'B', cost ); // print purchase ticket message
    } // if
} // TrainStop::buy

Train * TrainStop::wait( unsigned int studentId, Train::Direction direction ) {
    if ( direction == Train::Direction::Clockwise ) {
        prt.print( Printer::Kind::TrainStop, id, 'W', studentId, '<' ); // print wait message
        studentWait_1 += 1;
        studentCond1[studentId].wait();
        return curTrain_1;
    } else {
        prt.print( Printer::Kind::TrainStop, id, 'W', studentId, '>' ); // print wait message
        studentWait_2 += 1;
        studentCond2[studentId].wait();
        return curTrain_2;
    } // if
} // TrainStop::wait

void TrainStop::disembark( unsigned int studentId ) {
    prt.print( Printer::Kind::TrainStop, id, 'D', studentId ); // print disembark message
} // TrainStop::disembark

void TrainStop::tick() {
    prt.print( Printer::Kind::TrainStop, id, 't' ); // print tick message
    while ( !trainCond.empty() ) {
        trainCond.signal();
    } // while
} // TrainStop::tick

unsigned int TrainStop::arrive( unsigned int trainId, Train::Direction direction, unsigned int maxNumStudents ) {
    curTrain = static_cast<Train *>( &uThisTask() ); // get current train
    unsigned int willTake = 0; // return value
    if ( direction == Train::Direction::Clockwise ) {
        curTrain_1 = curTrain;
        for ( int i = 0; i < 100; i++ ) { // wake up blocked student
            if ( !studentCond1[i].empty() ) {
                studentCond1[i].signal();
            } // if
        } // for
        trainCond.wait(); // block train
        prt.print( Printer::Kind::TrainStop, id, 'A', trainId, maxNumStudents, studentWait_1 ); // print arrive message
        willTake = ( studentWait_1 > maxNumStudents ) ? maxNumStudents : studentWait_1;
        studentWait_1 -= willTake;
    } else {
        curTrain_2 = curTrain;
        for ( int i = 0; i < 100; i++ ) { // wake up blocked student
            if ( !studentCond2[i].empty() ) {
                studentCond2[i].signal();
            } // if
        } // for
        trainCond.wait(); // block train
        prt.print( Printer::Kind::TrainStop, id, 'A', trainId, maxNumStudents, studentWait_2 ); // print arrive message
        willTake = ( studentWait_2 > maxNumStudents ) ? maxNumStudents : studentWait_2;
        studentWait_2 -= willTake;
    } // if
    return willTake;
} // TrainStop::arrive

void TrainStop::main() {
    prt.print( Printer::Kind::TrainStop, id, 'S' );
    nameServer.registerStop( id );
    for ( ; ; ) {
        _Accept( ~TrainStop ) {
            break;
        } or _Accept( buy ) {
        } or _Accept( wait ) {
        } or _Accept( arrive ) {
        } or _Accept( tick ) {
        } or _Accept( disembark ) {
        } // _Accept
    } // for
    prt.print( Printer::Kind::TrainStop, id, 'F' );
} // TrainStop::main