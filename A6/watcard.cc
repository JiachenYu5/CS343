#include "watcard.h"

WATCard::WATCard(): balance( 0 ), hasPOP( false ) {} // WATCard::WATCard

WATCard::~WATCard() {} // WATCard::~WATCard

void WATCard::markPaid() {
    hasPOP = true;
} // WATCard::markPaid

void WATCard::deposit( unsigned int amount ) {
    balance += amount;
} // WATCard::deposit

void WATCard::withdraw( unsigned int amount ) {
    balance -= amount;
    markPaid();
} // WATCard::withdraw

unsigned int WATCard::getBalance() {
    return balance;
} // WATCard::getBalance

bool WATCard::paidForTicket() {
    return hasPOP;
} // WATCard::paidForTicket

void WATCard::resetPOP() {
    hasPOP = false;
} // WATCard::resetPOP