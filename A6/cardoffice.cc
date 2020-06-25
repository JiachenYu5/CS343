#include "cardoffice.h"
#include "MPRNG.h"
#include <iostream>
using namespace std;

extern MPRNG mprng;

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ) :
    prt( prt ), bank( bank ), numCouriers( numCouriers ) {
    courierPool = new Courier * [numCouriers];
    for ( unsigned int i = 0; i < numCouriers; i++ ) {
        courierPool[i] = new Courier( i , *this, prt, bank );
    } // for
} // WATCardOffice::WATCardOffice

WATCardOffice::~WATCardOffice() {
    for ( unsigned int i = 0; i < numCouriers; i++ ) {
        delete courierPool[i];
    } // for
    delete courierPool;
} // WATCardOffice::~WATCardOffice

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    // create new card and job
    WATCard * card = new WATCard();
    Job * job = new Job( sid, amount, card );
    jobList.push_back( job );  // add it to job list
    prt.print( Printer::Kind::WATCardOffice, 'C', sid, amount ); // print create call complete message
    return job->result;
} // WATCardOffice::create

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
    // create new job
    Job * job = new Job( sid, amount, card );
    jobList.push_back( job );  // add it to job list
    prt.print( Printer::Kind::WATCardOffice, 'T', sid, amount ); // print transfer call complete message
    return job->result;
} // WATCardOffice::transfer

WATCardOffice::Job * WATCardOffice::requestWork() {
    if ( jobList.empty() ) return NULL; // if
    Job * curJob = jobList.front(); // get the next job in list
    jobList.pop_front(); // remove the taken job from list
    prt.print( Printer::Kind::WATCardOffice, 'W' ); // print request work call complete message
    return curJob;
} // WATCardOffice::requestWork

void WATCardOffice::main() {
    prt.print( Printer::Kind::WATCardOffice, 'S' );
    for ( ; ; ) {
        _Accept( ~WATCardOffice ) {
            // delete all remaining jobs
            for ( ; ; ) {
                if ( jobList.empty() ) break; // if
                delete jobList.front();
                jobList.pop_front();
            } // for
            for ( unsigned int i = 0; i < numCouriers; i++ ) {
                _Accept( requestWork ); // let all couriers terminate as jobList is empty now
            } // for
            break;
        // when there's work, let couriers take work
        } or _When( !jobList.empty() ) _Accept( requestWork ) {
        } or _Accept( create ) {
        } or _Accept( transfer ) {
        } // _Accept
    } // for
    prt.print( Printer::Kind::WATCardOffice, 'F' );
} // WATCardOffice::main

WATCardOffice::Courier::Courier( unsigned int id, WATCardOffice & cardOffice, Printer & prt, Bank & bank ) :
    id( id ), cardOffice( cardOffice ), prt( prt ), bank( bank ) {} // WATCardOffice::Courier::Courier

void WATCardOffice::Courier::main() {
    prt.print( Printer::Kind::WATCardOfficeCourier, id, 'S' );
    for ( ; ; ) {
        Job *curJob = cardOffice.requestWork(); // get next job in line
        // if no job, break
        if ( curJob == NULL ) break; // if
        prt.print( Printer::Kind::WATCardOfficeCourier, id, 't', curJob->sid, curJob->amount ); // print start funds transfer message
        bank.withdraw( curJob->sid, curJob->amount ); // take funds from bank account
        curJob->watcard->deposit( curJob->amount ); // deposit monmey to watcard
        if ( mprng(5) == 0 ) { // if 1/6 change the card is lost
            prt.print( Printer::Kind::WATCardOfficeCourier, id, 'L', curJob->sid ); // print watcard lost message
            curJob->result.exception( new WATCardOffice::Lost ); // insert Lost exception to future
        } else { // no lost happened
            curJob->result.delivery( curJob->watcard ); // deliver card to future
            prt.print( Printer::Kind::WATCardOfficeCourier, id, 'T', curJob->sid, curJob->amount ); // print complete funds transfer message
        } // if
        delete curJob;
    } // for
    prt.print( Printer::Kind::WATCardOfficeCourier, id, 'F' );
} // WATCardOffice::Courier::main