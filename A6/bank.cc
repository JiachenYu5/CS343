#include "bank.h"

Bank::Bank( unsigned int numStudents ) : numStudents( numStudents ) {
    // initialize the balance for all student accounts to 0
    accountsBalance = new unsigned int[numStudents];
    for ( unsigned int i = 0; i < numStudents; i++ ) {
        accountsBalance[i] = 0;
    } // for
} // Bank::Bank

Bank::~Bank() {
    delete []accountsBalance;
} // Bank::Bank

void Bank::deposit( unsigned int id, unsigned int amount ) {
    accountsBalance[id] += amount;
} // Bank::deposit

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    // wait until the balance is larger than or equal to the amount of withdraw
    for ( ; ; ) {
        // enough money in account, break the loop and withdraw
        if ( amount <= accountsBalance[id] ) break; // if
        // otherwise allow deposit
        _Accept( deposit ); // _Accept
    } // for
    accountsBalance[id] -= amount;
} // Bank::withdraw