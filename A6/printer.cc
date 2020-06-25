#include "printer.h"
#include <iostream>
using namespace std;

Printer::Printer( unsigned int numStudents, unsigned int numTrains, unsigned int numStops, unsigned int numCouriers ) :
    numStudents( numStudents ), numTrains( numTrains ), numStops( numStops ), numCouriers( numCouriers ) {
    numColumns = numStudents + numTrains * 2 + numStops + numCouriers + 5;
    // initialize buffer that stores information
    columns = new Items[numColumns];
    for ( unsigned int i = 0; i < numColumns; i++ ) {
        columns[i].updated = false;
        columns[i].oid = -1;
        columns[i].value1 = -1;
        columns[i].value2 = -1;
        columns[i].additionalInfo = ' ';
        if ( i >= 5 ) {
            if ( i < numTrains + 5 ) columns[i].id = i - 5;
            else if ( i < numTrains * 2 + 5 ) columns[i].id = i - ( numTrains + 5 );
            else if ( i < numTrains * 2 + numStops + 5 ) columns[i].id = i - ( numTrains * 2 + 5 );
            else if ( i < numTrains * 2 + numStops + numStudents + 5 ) columns[i].id = i - ( numTrains * 2 + numStops + 5 );
            else columns[i].id = i - ( numTrains * 2 + numStops + numStudents + 5 ); // if
        } else {
            columns[i].id = 0;
        } // if
    } // for
    // print first line
    cout << "Parent\tGropoff\tWATOff\tNames\tTimer\t";
    for ( unsigned int i = 0; i < numTrains; i++) {
        cout << "Train" << i << "\t";
    } // for
    for ( unsigned int i = 0; i < numTrains; i++) {
        cout << "Cond" << i << "\t";
    } // for
    for ( unsigned int i = 0; i < numStops; i++) {
        cout << "Stop" << i << "\t";
    } // for
    for ( unsigned int i = 0; i < numStudents; i++ ) {
        cout << "Stud" << i << "\t";
    } // for
    for ( unsigned int i = 0; i < numCouriers; i++ ) {
        cout << "WCour" << i;
        if ( i < numCouriers - 1 ) cout << "\t"; // if
    } // for
    cout << endl;

    // print second line
    for ( unsigned int i = 0; i < numColumns; i++ ) {
        cout << "*******";
        if ( i < numColumns - 1 ) cout << " "; // if
    } // for
    cout << endl;
} // Printer::Printer

Printer::~Printer() {
    flushInfo();
    delete []columns;
    cout << "***********************" << endl;
} // Printer::~Printer

// help function to get the index for each kind
unsigned int Printer::getColumnIndex( Kind kind, unsigned int id ) {
    if ( kind != Kind::Train && kind != Kind::Conductor && kind != Kind::TrainStop && kind != Kind::Student && kind != Kind::WATCardOfficeCourier ) {
        return ( int ) kind;
    } // if

    switch ( kind ) {
        case Kind::Train:
            return 5 + id;
        case Kind::Conductor:
            return 5 + numTrains + id;
        case Kind::TrainStop:
            return 5 + numTrains * 2 + id;
        case Kind::Student:
            return 5 + numTrains * 2 + numStops + id;
        case Kind::WATCardOfficeCourier:
            return 5 + numTrains * 2 + numStops + numStudents + id;
        default: // invalid kind
            cerr << "get wrong kind" << endl;
            exit ( EXIT_FAILURE );
    } // switch
    return -1;
} // Printer::getColumnIndex

void Printer::flushInfo() {
    // find the last column index that needs to print
    unsigned int lastUpdated = -1;
    for ( int i = ( int )numColumns - 1; i >= 0; i-- ) {
        if ( columns[i].updated ) {
            lastUpdated = i;
            break;
        } // if
    } // for

    for ( unsigned int i = 0; i <= lastUpdated; i++ ) {
        if ( columns[i].updated ) {
            // print info and reset values
            bool hasoid = false;
            cout << columns[i].state;
            if ( columns[i].oid != -1 ) {
                hasoid = true;
                cout << columns[i].oid;
                columns[i].oid = -1;
            } // if
            if ( columns[i].value1 != -1 ) {
                if ( hasoid ) cout << ",";
                cout << columns[i].value1;
                columns[i].value1 = -1;
            } // if
            if ( columns[i].value2 != -1 ) {
                cout << "," << columns[i].value2;
                columns[i].value2 = -1;
            } // if
            if ( columns[i].additionalInfo != ' ' ) {
                cout << "," << columns[i].additionalInfo;
                columns[i].additionalInfo = ' ';
            } // if
            columns[i].updated = false;
        } // if
        if ( i < lastUpdated ) cout << "\t"; // if
    } // for
    cout << endl;
} // Printer::flushInfo

void Printer::print( Kind kind, char state ) {
    unsigned int index = getColumnIndex( kind, 0 );
    if ( columns[index].updated ) {
        flushInfo();
    } // if
    columns[index].state = state;
    columns[index].updated = true;
} // Printer::print

void Printer::print( Kind kind, char state, unsigned int value1 ) {
    unsigned int index = getColumnIndex( kind, 0 );
    if ( columns[index].updated ) {
        flushInfo();
    } // if
    columns[index].state = state;
    columns[index].value1 = value1;
    columns[index].updated = true;
} // Printer::print

void Printer::print( Kind kind, char state, unsigned int value1, unsigned int value2 ) {
    unsigned int index = getColumnIndex( kind, 0 );
    if ( columns[index].updated ) {
        flushInfo();
    } // if
    columns[index].state = state;
    columns[index].value1 = value1;
    columns[index].value2 = value2;
    columns[index].updated = true;
} // Printer::print

void Printer::print( Kind kind, unsigned int lid, char state ) {
    unsigned int index = getColumnIndex( kind, lid );
    if ( columns[index].updated ) {
        flushInfo();
    } // if
    columns[index].state = state;
    columns[index].updated = true;
} // Printer::print

void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1 ) {
    unsigned int index = getColumnIndex( kind, lid );
    if ( columns[index].updated ) {
        flushInfo();
    } // if
    columns[index].state = state;
    columns[index].value1 = value1;
    columns[index].updated = true;
} // Printer::print

void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 ) {
    unsigned int index = getColumnIndex( kind, lid );
    if ( columns[index].updated ) {
        flushInfo();
    } // if
    columns[index].state = state;
    columns[index].value1 = value1;
    columns[index].value2 = value2;
    columns[index].updated = true;
} // Printer::print

void Printer::print( Kind kind, unsigned int lid, char state, unsigned int oid, unsigned int value1, unsigned int value2 ) {
    unsigned int index = getColumnIndex( kind, lid );
    if ( columns[index].updated ) {
        flushInfo();
    } // if
    columns[index].state = state;
    columns[index].oid = oid;
    columns[index].value1 = value1;
    columns[index].value2 = value2;
    columns[index].updated = true;
} // Printer::print

void Printer::print( Kind kind, unsigned int lid, char state, char c ) {
    unsigned int index = getColumnIndex( kind, lid );
    if ( columns[index].updated ) {
        flushInfo();
    } // if
    columns[index].state = state;
    columns[index].additionalInfo = c;
    columns[index].updated = true;
} // Printer::print

void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1, char c ) {
    unsigned int index = getColumnIndex( kind, lid );
    if ( columns[index].updated ) {
        flushInfo();
    } // if
    columns[index].state = state;
    columns[index].value1 = value1;
    columns[index].additionalInfo = c;
    columns[index].updated = true;
} // Printer::print

void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2, char c ) {
    unsigned int index = getColumnIndex( kind, lid );
    if ( columns[index].updated ) {
        flushInfo();
    } // if
    columns[index].state = state;
    columns[index].value1 = value1;
    columns[index].value2 = value2;
    columns[index].additionalInfo = c;
    columns[index].updated = true;
} // Printer::print