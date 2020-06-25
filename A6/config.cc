#include "config.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

static bool skipComment( ifstream & file, string & str ) {
    for ( ; ; ) {
        file >> str;
        // eof
        if ( file.fail() ) return true; // if
        // not a comment, must be the parameter name
        if ( str.substr(0, 1) != "#" ) break; // if
        // else read the start of comment
        file.ignore( numeric_limits<int>::max(), '\n' ); // ignore all comment values until newline occurs
    } // for
    return false;
}

static unsigned int indexOfParams( string & str ) {
    if ( str == "StopCost" ) return 0;
    else if ( str == "NumStudents" ) return 1;
    else if ( str == "NumStops" ) return 2;
    else if ( str == "MaxNumStudents" ) return 3;
    else if ( str == "TimerDelay" ) return 4;
    else if ( str == "MaxStudentDelay" ) return 5;
    else if ( str == "MaxStudentTrips" ) return 6;
    else if ( str == "GroupoffDelay" ) return 7;
    else if ( str == "ConductorDelay" ) return 8;
    else if ( str == "ParentalDelay" ) return 9;
    else return 10; // if
}

static void assignValue( ConfigParms & cparms, unsigned int index, unsigned int value ) {
    if ( index == 0 ) cparms.stopCost = value;
    else if ( index == 1 ) cparms.numStudents = value;
    else if ( index == 2 ) cparms.numStops = value;
    else if ( index == 3 ) cparms.maxNumStudents = value;
    else if ( index == 4 ) cparms.timerDelay = value;
    else if ( index == 5 ) cparms.maxStudentDelay = value;
    else if ( index == 6 ) cparms.maxStudentTrips = value;
    else if ( index == 7) cparms.groupoffDelay = value;
    else if ( index == 8 ) cparms.conductorDelay = value;
    else if ( index == 9 ) cparms.parentalDelay = value;
    else cparms.numCouriers = value; // if
}

void processConfigFile( const char * configFile, ConfigParms & cparms ) {
    string paramName;
    unsigned int readValue;
    unsigned int currentIndex;
    unsigned int paramCountRead = 0;
    unsigned int const paramCountTotal = 11;
    // create a struct for each paramters to help with reading
    struct ParamRead {
        bool hasRead;
    }; // struct
    static ParamRead params[paramCountTotal];
    // initialize all parameters struct in order of representing the original struct
    // params[0] "StopCost"
    // params[1] "NumStudents"
    // params[2] "NumStops"
    // params[3] "MaxNumStudents"
    // params[4] "TimerDelay"
    // params[5] "MaxStudentDelay"
    // params[6] "MaxStudentTrips"
    // params[7] "GroupoffDelay"
    // params[8] "ConductorDelay"
    // params[9] "ParentalDelay"
    // params[10] "NumCouriers"
    for ( unsigned int i = 0; i < paramCountTotal; i++ ) {
        params[i] = ParamRead{ false };
    } // for

    try {
        ifstream file( configFile ); // open the file
        // loop until all parameters are read
        while ( paramCountRead < paramCountTotal ) {
            // if reach eof, break; else read in the parameter name in paramName
            if ( skipComment( file, paramName ) ) break; // if
            currentIndex = indexOfParams( paramName );
            if ( params[currentIndex].hasRead ) {
                cerr << "Input file " << configFile << " has duplicate parameters." << endl;
                exit( EXIT_FAILURE );
            } // if
            file >> readValue; // read value
            assignValue( cparms, currentIndex, readValue );
            params[currentIndex].hasRead = true;
            paramCountRead += 1;
            // read eof, break;
            if ( file.fail() ) break; // if
            file.ignore( numeric_limits<int>::max(), '\n' ); // ignore all comment values until newline occurs
        } // while
        // not eof, config file has extra data, exit
        if ( !skipComment( file, paramName ) ) {
            cerr << "Input file " << configFile << " has extra data." << endl;
            exit( EXIT_FAILURE );
        } // if
    } catch( uFile::Failure & ) {
        cerr << "Could not open input file " << configFile << "." << endl;
        exit( EXIT_FAILURE );
    } // try
} // processConfigFile