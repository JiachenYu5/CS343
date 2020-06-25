#include "q2printer.h"
#include <iostream>
using namespace std;

Printer::Printer( unsigned int voters ) {
    Voters = voters;
    columns = new Items[voters];
    for ( unsigned int i = 0; i < voters; i++ ) {
        columns[i].id = i;
        columns[i].updated = false;
    }
    for ( unsigned int i = 0; i < voters; i++ ) {
        cout << "V" << i;
        if ( i < voters - 1 ) cout << "\t";
    }
    cout << endl;
    for ( unsigned int i = 0; i < voters; i++ ) {
        cout << "*******";
        if ( i < voters - 1 ) cout << " ";
    }
    cout << endl;
}

Printer::~Printer() {
    flushInfo();
    delete []columns;
    cout << "*****************" << endl;
    cout << "All tours started" << endl;
}

void Printer::print( unsigned int id, Voter::States state ) {
    if ( columns[id].updated ) {
        flushInfo();
    }
    columns[id].state = state;
    columns[id].updated = true;
}

void Printer::print( unsigned int id, Voter::States state, TallyVotes::Tour tour ) {
    if ( columns[id].updated ) {
        flushInfo();
    }

    columns[id].state = state;
    columns[id].tour = tour;
    columns[id].updated = true;
}

void Printer::print( unsigned int id, Voter::States state, TallyVotes::Ballot ballot ) {
    if ( columns[id].updated ) {
        flushInfo();
    }

    columns[id].state = state;
    columns[id].ballot = ballot;
    columns[id].updated = true;
}

void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked ) {
    if ( columns[id].updated ) {
        flushInfo();
    }

    columns[id].state = state;
    columns[id].numBlocked = numBlocked;
    columns[id].updated = true;
}

void Printer::flushInfo() {
    for ( unsigned int i = 0; i < Voters; i++ ) {
        curr = columns[i];
        if ( curr.updated ) {
            Voter::States curState = curr.state;
            cout << (char)curState;
            if ( curState ==  Voter::States::Vote ) {
                cout << " " << curr.ballot.picture << "," << curr.ballot.statue << "," << curr.ballot.giftshop;
            } else if ( curState == Voter::States::Complete ) {
                cout << " " << (char)curr.tour.tourkind;
            } else if ( curState == Voter::States::Going ) {
                cout << " " << (char)curr.tour.tourkind << " " << curr.tour.groupno;
            } else if ( curState == Voter::States::Block || curState == Voter::States::Unblock || curState == Voter::States::Barging ) {
                cout << " " << curr.numBlocked;
            }
        }
        columns[i].updated = false;
        if ( i < Voters - 1 ) cout << "\t";
    }
    cout << endl;
}