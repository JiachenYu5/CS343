#include "config.h"
#include "bank.h"
#include "printer.h"
#include "parent.h"
#include "nameserver.h"
#include "groupoff.h"
#include "cardoffice.h"
#include "train.h"
#include "trainstop.h"
#include "timer.h"
#include "conductor.h"
#include "student.h"
#include "MPRNG.h"
#include <iostream>
#include <string>
using namespace std;

extern MPRNG mprng;

// help method for printing error message
void message( char * argv[] ) {
    cerr << "Usage: " << argv[0] << " [ config-file [ Seed > 0 ] ]" << endl;
    exit ( EXIT_FAILURE );
}

// help method for reading commandline input and call message if error when parsing
int parseArgument( char * argv[] ) {
    int returnVal = -1;
    try {
        returnVal = stoi( argv[2] );
        if ( returnVal <= 0 ) throw invalid_argument(""); // if 
    } catch ( const invalid_argument & ia ) {
        cerr << "Seed needs to be a positive integer." << endl;
        message( argv );
    } // try
    return returnVal;
}

int main( int argc, char * argv[] ) {
    string configFile = "lrt.config";
    int seed = getpid();
    switch (argc) {
        case 3:
            seed = parseArgument( argv );
            // FALL THROUGH
        case 2:
            configFile = argv[1];
            // FALL THROUGH
        case 1:
            break;
        default:
            cerr << "Incorrect number of arguments" << endl;
            exit ( EXIT_FAILURE );
    } // switch

    mprng.set_seed( seed ); // set seed
    // set configuration
    ConfigParms config;
    processConfigFile( configFile.c_str(), config );
    // create objects
    unsigned int maxTripCost = config.numStops / 2 * config.stopCost;
    Printer printer( config.numStudents, 2, config.numStops, config.numCouriers );
    Bank bank( config.numStudents );
    WATCardOffice cardOffice( printer, bank, config.numCouriers );
    NameServer nameServer( printer, config.numStops, config.numStudents );
    Groupoff groupoff( printer, config.numStudents, maxTripCost, config.groupoffDelay );
    Parent parent( printer, bank, config.numStudents, config.parentalDelay, maxTripCost );
    // start the tasks
    TrainStop * trainStops[config.numStops];
    for ( unsigned int i = 0; i < config.numStops; i++ ) {
        trainStops[i] = new TrainStop( printer, nameServer, i, config.stopCost );
    } // for
    Timer timer( printer, nameServer, config.timerDelay );
    Train * trains[2];
    Conductor * conductors[2];
    for ( unsigned int i = 0; i < 2; i++ ) {
        trains[i] = new Train( printer, nameServer, i, config.maxNumStudents, config.numStops );
        conductors[i] = new Conductor( printer, i, trains[i], config.conductorDelay );
    } // for
    Student * students[config.numStudents];
    for ( unsigned int i = 0; i < config.numStudents; i++ ) {
        students[i] = new Student( printer, nameServer, cardOffice, groupoff, i, config.numStops, 
            config.stopCost, config.maxStudentDelay, config.maxStudentTrips );
    } // for

    // finishing tasks
    for ( unsigned int i = 0; i < config.numStudents; i++ ) {
        delete students[i];
    } // for
    for ( unsigned int i = 0; i < 2; i++ ) {
        delete conductors[i];
        delete trains[i];
    } // for
    for ( unsigned int i = 0; i < config.numStops; i++ ) {
        delete trainStops[i];
    } // for
}